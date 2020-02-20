/* author: jlicari, ELE800, NMK05 - Autonomous Fetching Robot
 *  Created: 20 Feb 2020
 *  manual.h
 *  Contains functions for manual operation corresponding to mtr.ino 
 */
 
#ifndef manual_h
#define manual_h

#include arm.h

void manual() {
  // Read in state variable
  mode = analogRead(md);

  // Only run state check if there has been a change in ANALOG voltage
  // Debouncing
  if (mode != old && mode != 0) { 
    // Determine state based on the analog voltage
    if (mode >= exp0*0.995 && mode <= exp0*1.005)
      state = 0;
    else if (mode >= exp1*0.995 && mode <= exp1*1.005)
      state = 1;
    else if (mode >= exp2*0.995 && mode <= exp2*1.005)
      state = 2;
    else if (mode >= exp3*0.995 && mode <= exp3*1.005)
      state = 3;

    old = mode;
  }   
  
  //rdVolt(); // Print the controls' analog voltage to the Serial monitor

  // output_var = analogRead(pin_var);
  joy_x = analogRead(jx);
  joy_y = analogRead(jy);
  
  // Map the 10-bit ADC digital signal to an 8-bit digital signal for the DAC
  joy_x = map(joy_x, 0, 1023, 0, 255);
  joy_y = map(joy_y, 0, 1023, 0, 255);

  // Switch case for flight mode selection
  switch (state) {
     case 0:
      Serial.println("Enter Idle State");
      idle();
      break;

     case 1:
      Serial.println("Enter Drive State");
      drive();
      break;

     case 2:
      Serial.println("Enter Precision Movement State");
      slow();
      break;
      
     case 3:
      Serial.println("Enter Robotic Arm State");
      grab();
  }
  
  // Prints the motor speed to the Serial Monitor
  //mtrSpeed(); 
}

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

void rdVolt() { // Print values to serial monitor
  Serial.print("\nJoystick x-axis position {0,1023} = ");
  Serial.print(joy_x);
  Serial.print("\nJoystick y-axis position {0,1023} = ");
  Serial.print(joy_y);
  Serial.print("\n\n-----------------");
}

void mtrSpeed() { // Prints motor speed to serial monitor
  // Show rotor speed as a percentage of max speed
  Serial.print("\nMotor operating speed = ");
  op1 = (joy_y/255.0)*100.00; // FIX *****
  Serial.print(op1);
  Serial.print(" %");
  
  Serial.print("\n\n-----------------");
}

#endif
