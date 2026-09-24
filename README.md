# 50-50
> dumb ways to die

# ESP32 Development Env Install
## [VSCode Extension](https://github.com/espressif/vscode-esp-idf-extension/blob/master/docs/tutorial/install.md) Install
    Click above for link to install
    On the ESP-IDF set up, select the master(development) branch from Github
    
<img src="docs/img/esp-idf installer.jpg" alt="Install 5.2" width="975">

## ESP32 Vscode Setup
1. Open repo folder as vscode workspace
    > <p>File -> Open Workspaces from Files</p>
    > <p>OR</p>
    > <p>Add repository as sub workspace to current workspace </br> <kbd>CTRL</kbd> <kbd>SHIFT</kbd> <kbd>P</kbd>->Add folder to workspace</p>

2. Select Workspace using ESP-IDF extension
    1. **ESP-IDF: Pick a workspace folder** from <kbd>CTRL</kbd> <kbd>SHIFT</kbd> <kbd>P</kbd> and select 50-50
    2. **ESP-IDF: Add vscode configuration folder** It should automatically generate a `c_cpp_properties.json` file containing include paths to ESP library components folder.
        > <p>ESP Library is installed in current working user directory by default as `esp`.</p>

3.  If needed, configure project using menuconfig. Use the **ESP-IDF: SDK Configuration editor** command <kbd>CTRL</kbd> <kbd>E</kbd> <kbd>G</kbd> to modify the ESP-IDF project settings.
    Save once all changes are made

4. To build the project, use the **ESP-IDF: Build your project** command (<kbd>CTRL</kbd> <kbd>E</kbd> <kbd>B</kbd> keyboard shortcut).

5. Specify the serial port of the device with the **ESP-IDF: Select port to use** command <kbd>CTRL</kbd> <kbd>E</kbd> <kbd>P</kbd>
    The device will initialize with 2 COM ports on windows or /dev/ttyUSB* ports on Linux. Lower number port is for JTAG, higher port is for Serial connection. 
    Choose the higher numbered port

6. Run **ESP-IDF: Flash (UART) your project** to flash program. Alternatively, **ESP-IDF: Flash your project** command <kbd>CTRL</kbd> <kbd>E</kbd> <kbd>F</kbd>, then choose `UART` flash mode.  
    If the flash operation fails with unable to connect to the ESP board, re-flash program while pressing the BOOT button on the board.

7. Run **ESP-IDF: Monitor your device** command <kbd>CTRL</kbd> <kbd>E</kbd> <kbd>M</kbd> to launch the monitor output terminal

Application size Information: **ESP-IDF: Size analysis of the binaries**

### New Project
Run **ESP-IDF: New Project** 
    Select ESP-WROVER kit 3.3V
    Higher numbered serial port (COM1 if COM0 and COM1 listed)
    Click `Choose Template`
    > template-app for the most barebone project
    > Example templates available by chooseing ESP-IDF in the upper-left dropdown
    Follow above steps to set up project workspace and flash program.
