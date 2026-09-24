import os
import serial
from getch import _Getch
import threading
import time


class ListenOnSerialPort(threading.Thread):
    def __init__(self, keyboard_thread):
        threading.Thread.__init__(self)
        self.keyboard_thread = keyboard_thread

    def run(self):
        while True:
            if not self.keyboard_thread.input_active:
                try:
                    ser_in = ser.readline().decode('utf-8').strip('\n')
                    print(ser_in)
                    if "DEAUTH" in ser_in:
                        mac = ser_in.split("from ")[1]
                        #print("Got MAC: ", mac)
                        mac = mac.split("\n")
                        f = open("C:\\Users\\bryan\\Downloads\\arduino-cli_nightly-20231117_Windows_64bit\\BlinkMe\\BlinkMe.ino", "w")
                        text = "#include <Wire.h>\n#include <SPI.h>\n#include <TinyScreen.h>\nchar *MAC = \"" + mac[0][:-1] + "\";\nTinyScreen display = TinyScreen(TinyScreenDefault);\nvoid setup(void) {\nWire.begin();\ndisplay.begin();\ndisplay.setBrightness(10);\n}\nvoid loop() {\nwriteText();\ndelay(1000);\n}\nvoid writeText()\n{display.clearScreen();\ndisplay.setFont(liberationSans_8ptFontInfo);\nint width=display.getPrintWidth(MAC);\ndisplay.setCursor(48-(width/2),32);\ndisplay.fontColor(TS_8b_White,TS_8b_Red);\ndisplay.print(MAC);\ndelay(1000);\n}"
                        f.write(text)
                        f.close()
                        
                        os.system('cmd /c "cd C:\\Users\\bryan\\Downloads\\arduino-cli_nightly-20231117_Windows_64bit\\BlinkMe && C:\\Users\\bryan\\Downloads\\arduino-cli_nightly-20231117_Windows_64bit\\arduino-cli.exe compile -b TinyCircuits:samd:tinyzero"')
                        os.system('cmd /c "cd C:\\Users\\bryan\\Downloads\\arduino-cli_nightly-20231117_Windows_64bit\\BlinkMe && C:\\Users\\bryan\\Downloads\\arduino-cli_nightly-20231117_Windows_64bit\\arduino-cli.exe upload -b TinyCircuits:samd:tinyzero -p COM15"')
                        os._exit(1)
                        
                except UnicodeDecodeError:
                    print("Serial Error")


class ListenOnKeyboard(threading.Thread):
    input_active = False

    def run(self):
        while True:
            cmd_in = getch()
            print(cmd_in)
            if cmd_in == "i":  # if key 'i' is pressed
                self.input_active = True
                time.sleep(0.1)
                string = input(">> ")
                ser.write(string.encode())
                self.input_active = False
            elif cmd_in == "q":
                os._exit(1)



print("Press 'i' for sending data")
print("Press 'q' to quit")
port = "COM12"
baud = "115200"

getch = _Getch()
ser = serial.Serial(port, baud)

kb_listen = ListenOnKeyboard()
sp_listen = ListenOnSerialPort(kb_listen)
kb_listen.start()
sp_listen.start()
