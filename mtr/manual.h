/* author: jlicari, ELE800, NMK05 - Autonomous Fetching Robot
 *  Created: 20 Feb 2020
 *  manual.h
 *  Contains functions for manual operation corresponding to mtr.ino 
 */
 
#ifndef manual_h
#define manual_h

#include "mtr.h"

void idle() {                         // Do-nothing state
  /* Do-nothing state
   * Retains previously set values of motors
   * Controls can still be manipulated and printed to Serial Monitor
   * Control input does not affect the motor speed
   */
  delay(5000);
}

void drive() {                       //PWM output
  // Joystick input
  if (joy_x < 120) {                    // Left turn
    if ((128 - joy_y) < 0)
      analogWrite(ml, 0);
    else  
      analogWrite(ml, (128 - joy_y));   // L motor decreases speed
    analogWrite(mr, (256 - joy_y));     // R motor increases speed
  }
  else if (joy_x > 136) {               // Right turn
    analogWrite(ml, joy_y);             // L motor increases speed
    analogWrite(mr, (255 - joy_y));     // R motor decreases speed
  }
  else {                                // Forward, rest position  
    analogWrite(ml, joy_y);       
    analogWrite(mr, joy_y);       
  }
  
  delay(5);
}

void slow() {
  /* Precision Movement State is activated when 
   * img is detected AND within 20cm 
   */
   if (joy_x > 136) {     // Right Turn
    analogWrite(ml, 75);  // L motor increases speed
    analogWrite(mr, 25);  // R motor decreases speed
   }
   else if (joy_x < 120) {// Left Turn
    analogWrite(ml, 25);  // L motor decreases speed
    analogWrite(mr, 75);  // R motor increases speed
   }
   else { // Forward movement
    analogWrite(ml, 75);
    analogWrite(mr, 75);
   }
}

manual_grab() {
  
}

#endif
