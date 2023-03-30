# Tricorder-ESP32
A portable sensor server for my tricorder project
## About
Using the Adafruit Feather ESP32 V2 microcontroller board to read various sensor data through I2C, digital, and analog read. 

The MCU board, sensors, and other electrical components are placed on three custom double-layered PCBs, stacked vertically to make one complete circuit. Modular design allows for each PCB to also be used independently.

Two 128x64 OLED displays provide status (battery percentage, charging time, current GPS time, etc.) and basic sensor readings. Four capacitive touch pads on the PCB allow for a basic UI. 

Powered by its own 2500mAh LiPo battery and power supply + charging circuitry. 

A RaspberryPi communicates with the ESP32 through serial link (USB or bluetooth) and visualizes the requested data in real time.