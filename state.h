/**
 * This file is part of Wio Helium Monitor.
 *
 *   Wio Helium Monitor is free software created by Paul Pinault aka disk91. 
 *   You can redistribute it and/or modify it under the terms of the 
 *   GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   any later version.
 *
 *  Wio Helium Monitor is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Wio LoRaWan Field Tester.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  Author : Paul Pinault (disk91.com)
 */  

#ifndef __STATE_H
#define __STATE_H

#define SSID_SZ     64
#define SSID_PWD_SZ 64
#define IP_SZ       16

#define MAXBUFFER    32

typedef struct s_state {

  uint8_t   ssid[SSID_SZ];          // WiFi ssid
  uint8_t   password[SSID_PWD_SZ];  // WiFi password
  uint8_t   extPingIp[IP_SZ];       // an IP address on Internet 
  uint8_t   intPingIp[IP_SZ];       // the hotspot IP address on LAN

  uint8_t   avgExtPing[MAXBUFFER];  // ping rtt history / 8 ms (apparently the step we have)
  uint8_t   avgIntPing[MAXBUFFER];

  int       readPtr;          // circular buffer
  int       writePtr;
  int       elements;         // number of data in buffer
  boolean   hasRefreshed;     // Data has refreshed

} state_t;

extern state_t state;

bool readConfig();
void storeConfig();


void initState();
void runMonitor();

uint8_t getIndexInBuffer(int i);

#endif
