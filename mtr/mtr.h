/* author: jlicari, ELE800, NMK05 - Autonomous Fetching Robot
 *  Created: 10 Feb 2020
 *  mtr.h
 *  This header file contains definitions corresponding to mtr.ino 
 */
#ifndef mtr_h
#define mtr_h

  // Define the expected analog values for the Flight Mode Select button - dlete
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
  
  // Motor PWM pin declaration
  const int ml = 3; // L motor
  const int lfwd = 5; 
  const int lbwd = 6; 
  const int mr = 9; // R motor
  const int rfwd = 8;
  const int rbwd = 7;

  // Analog variables
  int joy_x = 0;
  int joy_y = 0;
  int mode = 0;

  // Miscellaneous 
  float op1;
  long dur;  
  unsigned long bgn, tot_time; // Start/ Stop time 
  int old, state, dist; 
  volatile bool img = false; // Object detect signal 

  // TX Variables
  const int tx_led = 13;
  const int tx = 12;

struct Data_Packet {
  int x_dat;
  int y_dat;
  int state;
};

Data_Packet data;

#endif
