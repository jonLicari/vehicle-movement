/* author: jlicari, ELE 800, Ryerson University
 *  Created: 8 February 2020
 *  mtr.ino
 *  Vehicle mode selection, sensor assisted navigation, wireless control
 */

#include <VirtualWire.h>
#include <Scheduler.h>
#include "mtr.h"

void setup() {
  
  state = 3; // Initialize mode to 0 - Idle, 3 -demo
  
  // Sensor Port Initialization
  pinMode(jx, INPUT);
  pinMode(jy, INPUT);
  pinMode(md, INPUT);
  pinMode(im, INPUT_PULLUP);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // Motor Port Initialization
  pinMode(ml, OUTPUT);
  pinMode(lfwd, OUTPUT);
  pinMode(lbwd, OUTPUT);
  pinMode(mr, OUTPUT);
  pinMode(rfwd, OUTPUT);
  pinMode(rbwd, OUTPUT);

  // Motor Signal Initialization
  digitalWrite(lfwd, HIGH);
  digitalWrite(lbwd, LOW);
  digitalWrite(rfwd, HIGH);
  digitalWrite(rfwd, LOW);
  
  // Tx initialization
  vw_set_tx_pin(tx); // Set transmit pin
  vw_setup(2000); // Bits per second
  pinMode(tx_led, OUTPUT);

  // Interrupt 
  attachInterrupt(digitalPinToInterrupt(im), detect, CHANGE);

  // Initialize Serial monitor
  Serial.begin(9600);
}

void loop() {

  // Read in state variable
  mode = digitalRead(md);

  // Only run state check if there has been a change in ANALOG voltage
  // This only works if the ANALOG signal is consistent and steady
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
  
  // output_var = analogRead(pin_var);
  joy_x = analogRead(jx);
  joy_y = analogRead(jy);
  
  rdVolt(); // Print the controls' analog voltage to the Serial monitor

  // Map the 10-bit ADC digital signal to an 8-bit digital signal for the DAC
  joy_x = map(joy_x, 0, 1023, 0, 255);
  joy_y = map(joy_y, 0, 1023, 0, 255);

  // Switch case for flight mode selection
  switch (state) {
     case 0:
      idle();
      Serial.println("Enter Idle State");
      break;

     case 1:
      drive();
      Serial.println("Enter Drive State");
      break;

     case 2:
      slow();
      Serial.println("Enter Precision Movement State");
      break;
      
     case 3:
      demo();
      Serial.println("Enter Autonomous State");
      break;   
  }

  // Prints the motor speed to the Serial Monitor
  mtrSpeed(); 

  //send_data(); // test serial connect before wireless
}

void detect() {
  img = !img;
}

void send_data() {
  digitalWrite(tx_led, HIGH); // Begin transmit
  set_data(); // Substitute for reading values
  vw_send((uint8_t *)&data, sizeof(data));
  vw_wait_tx(); // Wait until the whole message sends
  digitalWrite(tx_led, LOW);
  // delay(5);
}

void set_data() {
  data.x_dat = joy_x;
  data.y_dat = joy_y;
  data.state = state;
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

void demo() {
  /* Scan state has the vehicle turn in a slow 360 
   *  camera is actively scanning for object 
   *  once object is detected, jump to drive state
   *  slow once in range of object
   *  stop in range of claw
   *  
   */
  int spd, d;
   
  if (img == false) {     // object not detected
    analogWrite(ml, 140);
    analogWrite(mr, 50);
    tot_time = 0;    
  }
  else {
    d = measure();
    if (d > 20) { // Distance > 20cm from object
      spd = 200;
      analogWrite(ml, spd);
      analogWrite(mr, spd);
    }
    else if (d > 8 && d < 20) { // Slow: 20cm > Distance > 8cm
      spd = 50;
      analogWrite(ml, spd);
      analogWrite(mr, spd);
    }
    else { // Stop: Distance <= 7cm
      analogWrite(ml, 0);
      analogWrite(mr, 0);
      grab(); // Control Servo to grab object
    }
  }
}

int measure() {
  int i;
  for (i = 0; i < 2; i++) {
    // Clears the trig
    digitalWrite(trig, LOW);
    delayMicroseconds(2);

    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    dur = pulseIn(echoPin, HIGH);

    // Calculating the distance
    dist += dur*0.034/2;
  }
  dist = dist/i;
  Serial.println("Distance: ");
  Serial.print(dist);

  return(dist); 
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
