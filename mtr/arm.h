/* author: jlicari, ELE800, NMK05 - Autonomous Fetching Robot
 *  Created: 18 Feb 2020
 *  arm.h
 *  Robotic Arm Operation 
 */
 
#ifndef arm_h
#define arm_h

#define ymax  45
grab() {
  // sx operation
  while(grip_cnfm != true) { // or digitalRead(pin12) != HIGH
    sx.write(posx);
    posx++;
  }
  
  // sy operation
  for (posy = 0; posy < ymax; posy++) {
    sy.write(posy);
  }
}

#endif
