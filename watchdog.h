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
 *  
 *  Original code from https://github.com/cyrusbuilt/SAMCrashMonitor/blob/master/SAMCrashMonitor.cpp
 *  
 */  

 #ifndef __WATCHDOG_H__
 #define __WATCHDOC_H__

void initWatchdog();
int enableWatchdog(int maxPeriodMS);
void disableWatchdog();
void iAmAlive();

 #endif
