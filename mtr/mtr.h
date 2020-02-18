/* author: jlicari, ELE800, NMK05 - Autonomous Fetching Robot
 *  Created: 10 Feb 2020
 *  mtr.h
 *  This header file contains definitions corresponding to mtr.ino 
 */
#ifndef mtr_h
#define mtr_h

  // Define analog constants for the Mode Select debouncing
  #define exp0 1014 // 1k
  #define exp1 931 // 10k
  #define exp2 840 // 22k
  #define exp3 770 // 33k

  // Pin Assignments
  const int jx = A0; 
  const int jy = A1;
  const int md = A2; // Mode select
  const int im = 2; // Object detect port
  const int trig = 4;
  const int echo = 10;
  const int grp_cnfm = 11; // Grip signal port
  const int x = // x-axis servo pin
  const int y = // y-axis servo pin
  
  // Motor PWM pin declaration
  const int ml = 3; // L motor
  const int lfwd = 5; 
  const int lbwd = 6; 
  const int mr = 9; // R motor
  const int rfwd = 8;
  const int rbwd = 7;

  // TX Variables
  const int tx_led = 13;
  const int tx = 12;

  // Analog variables
  int joy_x = 0;
  int joy_y = 0;
  int mode = 0;

  // Miscellaneous 
  float op1;
  long dur;  
  unsigned long bgn, tot_time; // Start/ Stop time 
  int old, state, dist, posx, posy; 
  volatile bool img; // Object detect signal 
  volatile bool grp; // Grip detect signal

struct Data_Packet {
  int x_dat;
  int y_dat;
  int state;
};

Data_Packet data;

#endif
