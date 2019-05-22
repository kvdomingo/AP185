# LoLAN-PresS
Live Feed-over-LAN Pressure Sensor

This repository contains source code, calibration files and some example characterization files. To start, either download/clone the project or use bash:

```
git clone https://github.com/kvdomingo/LoLAN-PresS
```

Open [IP_test.ipynb](IP_test.ipynb) with Jupyter Notebook and run the necessary code. In order to use the command `runlive()`, you will need to first make sure you are connected to the same network as your microcontroller, then determine your microcontroller's local IP address. Replace the `url` variable with this IP in string format, then initialize and run the program.
