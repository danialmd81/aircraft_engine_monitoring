# Aircraft Engine Monitoring

## Project Description

This project is designed for monitoring aircraft engines. The core implementation is primarily in C++ with some components written in Python and QMake.

## Features

- **Real-time Engine Monitoring**: Continuously monitors engine parameters.
- **Data Analysis**: Analyzes collected data to detect anomalies.
- **Alert System**: Notifies in case of any critical engine conditions.
- **Historical Data Storage**: Stores historical data for future analysis.
- **User-friendly Interface**: Intuitive UI for easy interaction and monitoring.
- **Customizable Alerts**: Allows users to set custom alert thresholds.
- **Remote Monitoring**: Supports monitoring from remote locations.
- **Report Generation**: Generates detailed reports on engine performance and anomalies.

## Requirements

### Software

- **Qt**: Qt 6.6.3 or later for building the UI components.
- **QMake**: For building the project.
- **Python**: Python 3.x for running the `message_generator.py` script.
- **C++ Compiler**: A C++ compiler compatible with C++17 or later (e.g., GCC, Clang, or MSVC).

### Libraries

- **Qt Widgets**: Required for the UI components.
- **Qt Core**: Required for core functionalities.
- **QXlsx**: Required for handling Excel files.
- **QcGaugeWidget**: Required for creating gauge widgets.

### Development Environment

- **Visual Studio Code**: Recommended for development with the provided settings in `.vscode/`.
- **Qt Creator**: Optional, but useful for working with `.pro` files and UI design.

### Operating System

- **Windows**: Development and testing are primarily done on Windows.
- **Linux**: Should be compatible but may require additional setup.

### Additional Tools

- **Git**: For version control.

## Installation Guide

### Windows Installation

1. Download the Windows executable:
    - Visit the [releases page](https://github.com/danialmd81/aircraft_engine_monitoring/releases)
    - Download `aircraft_engine_monitoring_x64bit.exe`
    - Or use PowerShell:

      ```powershell
      Invoke-WebRequest -Uri "https://github.com/danialmd81/aircraft_engine_monitoring/releases/latest/download/aircraft_engine_monitoring_x64bit.exe" -OutFile "aircraft_engine_monitoring_x64bit.exe"
      ```

2. Run the installer:
    - Double-click the downloaded `.exe` file
    - Follow the installation wizard prompts
    - Accept the default installation location or choose your preferred path

### Linux Installation

1. Download the Linux installer:
    - Visit the [releases page](https://github.com/danialmd81/aircraft_engine_monitoring/releases)
    - Download `aircraft_engine_monitoring_x64bit.run`
    - Or use wget:

      ```bash
      wget https://github.com/danialmd81/aircraft_engine_monitoring/releases/download/v1.0.0/aircraft_engine_monitoring_x64bit.run
      ```

2. Make the installer executable and run:

    ```bash
    chmod +x aircraft_engine_monitoring_x64bit.run
    ./aircraft_engine_monitoring_x64bit.run
    ```

3. Follow the installation wizard prompts.

## Notes

- Ensure all dependencies are correctly installed and configured.
- For any issues during installation, refer to the project's documentation or seek help from the community.

## Demo

![main](./pic/main.png)
![monitoring](./pic/monitoring.gif)

## Deployment Tools

### Windows (Inno Setup)

1. Install Inno Setup from the [official website](https://jrsoftware.org/isdl.php)

2. Create a new script file (`setup.iss`):

```inno
[Setup]
AppName=Aircraft Engine Monitoring
AppVersion=1.0
DefaultDirName={pf}\AircraftEngineMonitoring
DefaultGroupName=Aircraft Engine Monitoring
OutputDir=./windows_installer
OutputBaseFilename=aircraft_engine_monitoring_setup

[Files]
Source: "release\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs

[Icons]
Name: "{group}\Aircraft Engine Monitor"; Filename: "{app}\aircraft_engine_monitoring.exe"
```

### Linux (CQtDeployer)

1. Install CQtDeployer:

```bash
wget https://github.com/QuasarApp/CQtDeployer/releases/download/1.4.1/CQtDeployer_1.4.1_Linux_x86_64.deb
sudo dpkg -i CQtDeployer_1.4.1_Linux_x86_64.deb
```

2. Deploy using CQtDeployer:

```bash
cqtdeployer -bin ./build/aircraft_engine_monitoring \
            -qmake $QT_DIR/bin/qmake \
            -targetDir ./linux_installer \
            qif noTranslation
```

## Test Serial Ports

### Create Virtual Serial Ports

1. Install socat:

```sh
sudo apt-get install socat
```

2. Create permanent rules by adding to `/etc/udev/rules.d/99-serial.rules`:

```sh
KERNEL=="ttySOCAT*", SYMLINK+="/home/danial/4"
KERNEL=="ttySOCAT*", SYMLINK+="/home/danial/5"
```

3. Start the virtual ports:

```sh
socat -d -d pty,raw,echo=0,link=/home/danial/4 pty,raw,echo=0,link=/home/danial/5
```

### Remove Virtual Ports

1. Remove udev rules:

```sh
sudo rm /etc/udev/rules.d/99-serial.rules
```

2. Reload udev rules:

```sh
sudo udevadm control --reload-rules
sudo udevadm trigger
```

3. Kill socat process:

```sh
pkill socat
```
