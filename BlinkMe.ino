#include <Wire.h>
#include <SPI.h>
#include <TinyScreen.h>
char *MAC = "46:C6:90:40:D0:AF";
TinyScreen display = TinyScreen(TinyScreenDefault);
void setup(void) {
Wire.begin();
display.begin();
display.setBrightness(10);
}
void loop() {
writeText();
delay(1000);
}
void writeText()
{display.clearScreen();
display.setFont(liberationSans_8ptFontInfo);
int width=display.getPrintWidth(MAC);
display.setCursor(48-(width/2),32);
display.fontColor(TS_8b_White,TS_8b_Red);
display.print(MAC);
delay(1000);
}