#include <WiFi.h>
//#include "Mac.h"

#include "esp_wifi.h"
const wifi_promiscuous_filter_t filt={
    .filter_mask=WIFI_PROMIS_FILTER_MASK_MGMT|WIFI_PROMIS_FILTER_MASK_DATA
};
  
typedef struct {
  uint8_t mac[6];
} __attribute__((packed)) MacAddr;

typedef struct {
  int16_t fctl;
  int16_t duration;
  MacAddr da;
  MacAddr sa;
  MacAddr bssid;
  int16_t seqctl;
  unsigned char payload[];
} __attribute__((packed)) WifiMgmtHdr;

//===== SETTINGS =====//
#define channel 1 //the channel to start scanning (1-14)
#define channelHopping true //scan on all channels
#define maxChannel 11 //US = 11, EU = 13, Japan = 14
#define ledPin 2 //led pin ( 2 = built-in LED)
#define inverted true // invert HIGH/LOW for the LED
#define packetRate 3 //min. packets before it gets recognized as an attack

#define scanTime 500 //scan time per channel in ms

unsigned long count = 0;
unsigned long prevTime = 0;
int curChannel = channel;
uint8_t attacker_mac_raw[6];

// void dumpPacket(uint8_t* buf, uint16_t len) {
//   if(buf == nullptr  len <= 27)
//     return;
    
//   Mac from(buf[16],buf[17],buf[18],buf[19],buf[20],buf[21]);
//   Mac to(buf[22],buf[23],buf[24],buf[25],buf[26],buf[27]);

//   Serial.print("Chan ");
//   Serial.println(curChannel);
//   Serial.print("From ");
//   from._println();
//   Serial.print("To ");
//   to._println();
//   Serial.println();
// }


void format_MAC_as_string(uint8_t *mac_addr, char* mac_string) {
    for (int i = 0; i < 6; i++) {
        int string_pos = i * 3;
        char h = '0';
        h = (((mac_addr[i] & 0xF0)>>4)&0x0F);
        h += (h >9) ? ('A' - 10): '0';
        mac_string[string_pos] = h;
        h = '0';
        h = (mac_addr[i] & 0x0F);
        h += (h >9) ? ('A' - 10): '0';
        mac_string[++string_pos] = h;
        mac_string[++string_pos] = (i < 5)? ':': '\0';
    }
    
}
void sniffer(void* buf, wifi_promiscuous_pkt_type_t type) 
{

  if (type == WIFI_PKT_MGMT) {
    wifi_promiscuous_pkt_t *p = (wifi_promiscuous_pkt_t*)buf;
    int len = p->rx_ctrl.sig_len;
    WifiMgmtHdr *wh = (WifiMgmtHdr*)p->payload;
    len -= sizeof(WifiMgmtHdr);
    if (len < 0) return;
    int fctl = ntohs(wh->fctl);
    if ((fctl & 0xF000) == 0xA000 || (fctl & 0xF000) == 0xC000)  {
      count++;
      memcpy(attacker_mac_raw, &(wh->sa), sizeof(attacker_mac_raw));
    }
  }
}

void setup() {
  Serial.begin(115200);
  #if defined(ESP8266)
    wifi_set_opmode(STATION_MODE);
    wifi_promiscuous_enable(0);
    WiFi.disconnect();
    wifi_set_promiscuous_rx_cb(sniffer);
    wifi_set_channel(curChannel);
    wifi_promiscuous_enable(1);
  #else
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    //esp_wifi_set_country(WIFI_COUNTRY_EU);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    esp_wifi_set_mode(WIFI_MODE_NULL);
    esp_wifi_start();
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_filter(&filt);
    esp_wifi_set_promiscuous_rx_cb(&sniffer);
    esp_wifi_set_channel(curChannel, WIFI_SECOND_CHAN_NONE);
  #endif
  pinMode(ledPin, OUTPUT);
  Serial.println("starting!");
}

void loop() {
  unsigned long curTime = millis();
  unsigned long delta = curTime - prevTime;
  if (delta < scanTime)
    delay(scanTime - delta);

  digitalWrite(ledPin, (count >= packetRate) ^ inverted);  
  if (count >= packetRate) {
    
        char attacker_mac[17];
        format_MAC_as_string(attacker_mac_raw, attacker_mac);
        Serial.print("DEAUTH: on channel ");
        Serial.print(curChannel);
        Serial.print(" from ");
        Serial.println(attacker_mac);
  }
  // Serial.print(curChannel);
  // Serial.print(": ");
  // Serial.println(count);
  
  prevTime = curTime;
  count = 0;

if(channelHopping){
    curChannel = curChannel % maxChannel + 1;
    #if defined(ESP8266)
      wifi_set_channel(curChannel);
    #else
      esp_wifi_set_channel(curChannel, WIFI_SECOND_CHAN_NONE);
    #endif
  }
}
