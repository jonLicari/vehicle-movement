/* author: jlicari, ELE800, NMK05 - Autonomous Fetching Robot
 *  Created: 10 Feb 2020
 *  mtr.h
 *  This header file contains definitions corresponding to mtr.ino 
 */
#ifndef mtr_h
#define mtr_h

  #define ymax  45

  // Pin Assignments
  const int im = 2; // Object detect port
  const int trig = 4;
  const int echo = 10;
  const int grp_cnfm = 11; // Grip signal port
  
  // Motor PWM pin declaration
  const int ml = 3; // L motor
  const int lfwd = 5; 
  const int lbwd = 6; 
  const int mr = 9; // R motor
  const int rfwd = 8;
  const int rbwd = 7;

  // Miscellaneous 
  float op1;
  long dur;  
  unsigned long bgn, tot_time; // Start/ Stop time 
  int select, dist, posx, posy; 
  volatile int img; // Object detect signal 
  volatile int grp; // Grip confirm variable
  int joy_x = 0;
  int joy_y = 0;

  // Rx Declarations
  const int rx = 12;
  const int rx_led = 13;
  int cntl; // Operating mode

  struct Data_Packet {
    int x_dat;
    int y_dat;
    int state;
  };

  Data_Packet data;

#endif
