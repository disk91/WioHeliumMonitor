*Doc in progress, code in progress, not yet ready to be used.*


# WioHeliumMonitor

The purpose of this project is to monitor your Helium Hotspot and be alarmed when it is not working correctly.

The solution currently ping the miner over the LAN and ping Internet to make sure the Hotspot is connected to the LAN and have an access to Internet.

## Coming soon
- report the information into the watchium application
- test the LoRa communication
- report the information over LoRa (using the network redundancy when the LAN is not available)
- store configuration in an external flash

## Requirements
The project uses a WIO Terminal you need to make some specific stuffs:
- Upgrade the WiFi firmware folowing the (Seeed documentation)[https://wiki.seeedstudio.com/Wio-Terminal-Network-Overview/]
- Modifiy the ping.c file in the libraries according to the ping.cpp content in this archive

