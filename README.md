# ModbusGateway (English)

English fork of [louisir/ModbusGateway](https://github.com/louisir/ModbusGateway) with full English UI.

ModbusGateway is a Qt-based Modbus software gateway for forwarding requests and responses between Modbus TCP and Modbus RTU. It supports both `TCP Master → RTU Slave` and `RTU Master → TCP Slave` modes, with GUI settings for the serial port, TCP listen/target address, and real-time TCP/RTU frame logs.

## Features

- **TCP Master → RTU Slave**: Modbus TCP masters access Modbus RTU slave devices over a serial/USB-RS485 adapter.
- **RTU Master → TCP Slave**: Modbus RTU masters access a Modbus TCP slave.
- TCP frame parsing based on the MBAP length field (handles fragmentation and coalescing).
- Automatic Modbus RTU CRC16 append and validation.
- Serialized RTU request queue (prevents concurrent access to the same RS-485 bus).
- Supported function codes: 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x0F, 0x10.
- Modbus exception responses and gateway timeout exceptions (fixed 2000 ms).
- Real-time hex logs for both TCP and RTU sides.

## Requirements

- Qt 5.15 or Qt 6.x
- Modules: Core, Gui, Widgets, Network, SerialPort
- C++17 compiler (MinGW or MSVC on Windows)

## Build (Windows)

### Option A – Qt Creator (easiest)

1. Install [Qt](https://www.qt.io/download) (open-source) with **MinGW** and the **SerialPort** module.
2. Open `ModbusGateway.pro` in Qt Creator.
3. Select a kit that includes Qt SerialPort.
4. Click **Build** → **Run**.

### Option B – Command line (MinGW)

```powershell
# Adjust path to your Qt installation
$env:PATH = "C:\\Qt\\6.8.3\\mingw_64\\bin;C:\\Qt\\Tools\\mingw1310_64\\bin;" + $env:PATH

qmake ModbusGateway.pro
mingw32-make -j4
```

The executable will be in the build directory (often `release\\ModbusGateway.exe`).

### Create a portable package

```powershell
windeployqt --release release\\ModbusGateway.exe
```

## Usage

1. Start the application.
2. Select mode:
   - **TCP Master → RTU Slave** (most common for USB-RS485 sticks)
   - **RTU Master → TCP Slave**
3. Configure serial port (COM port of your USB-RS485 adapter), baud rate, parity, etc.
4. Set TCP IP and port (default listen on port `502`).
5. Click **Start**.
6. Connect your Modbus TCP client (Node-RED, SCADA, QModMaster, …) to `127.0.0.1:502`.

## Limitations

- Only one TCP client is accepted at a time in TCP→RTU mode.
- Response timeout is fixed at 2000 ms.
- Broadcast address 0 is not forwarded in RTU→TCP mode.

## Original project

Upstream: https://github.com/louisir/ModbusGateway  
This fork changes the UI language to English and keeps the same license and functionality.

## License

See [LICENSE](LICENSE).
