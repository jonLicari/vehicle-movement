/* author: jlicari, ELE800, NMK05 - Autonomous Fetching Robot
 *  Created: 10 Mar 2020
 *  txm.h
 *  This header file contains definitions corresponding to txm.ino 
 */
 
#ifndef txm_h
#define txm_h

  // Define the expected analog values for the Mode Select button
  #define exp0 1014 // 1k
  #define exp1 931 // 10k
  #define exp2 840 // 22k
  #define exp3 770 // 33k

  // Pin Assignments
  const int jx = A0; 
  const int jy = A1;
  const int md = A2; // Mode select
  const int cntl = A3; // 0 - auto, 1 - manual

  // LED pins
  const int s0 = 7;
  const int s1 = 6;
  const int s2 = 5;
  const int s3 = 4;

  // TX Variables
  const int tx_led = 13;
  const int tx = 12;
  
  // Analog variables
  int joy_x = 0;
  int joy_y = 0;
  int old, sel, select;

  struct Data_Packet {
    int x_dat;
    int y_dat;
    int state;
  };

Data_Packet data;


#endif
