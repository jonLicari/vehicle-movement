/* author: jlicari, ELE800, NMK05 - Autonomous Fetching Robot
 *  Created: 10 Mar 2020
 *  txm.ino
 *  This file corresponds to the wireless transmitter controller 
 */
 
#include <VirtualWire.h>
#include <VirtualWire_Config.h>
#include "txm.h"

void setup() {
 
  // Tx initialization
  vw_set_tx_pin(tx); // Set transmit pin
  vw_setup(2000); // Bits per second
  pinMode(tx_led, OUTPUT);

  // Sensor Port Initialization
  pinMode(jx, INPUT);
  pinMode(jy, INPUT);
  pinMode(md, INPUT);
  pinMode(cntl, INPUT);

  // Set LEDs as output
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);

  select = 0; // Initialize state to 0 - manual, idle

  Serial.begin(9600);
}

void loop() {
  // Read in the state variable
  sel = analogRead(md);
  
  // Only run state check if there has been a change in ANALOG voltage
  if (sel != old && sel != 0) { // NOTE: This only works if the ANALOG signal is consistent and steady
    
    // Determine the state based on the analog voltage
    if (sel >= exp0*0.995 && sel <= exp0*1.005)
      select = 0;
    else if (sel >= exp1*0.995 && sel <= exp1*1.005)
      select = 1;
    else if (sel >= exp2*0.995 && sel <= exp2*1.005)
      select = 2;
    else if (sel >= exp3*0.995 && sel <= exp3*1.005)
      select = 3;

    old = sel;
  }

  // Read in analog inputs
  joy_x = analogRead(jx);
  joy_y = analogRead(jy);

  // Map the 10-bit ADC digital signal to an 8-bit digital signal for the DAC
  joy_x = map(joy_x, 0, 1023, 0, 255);
  joy_y = map(joy_y, 0, 1023, 0, 255);

  //rdVolt(); // Read mapped voltage of analog input and operating mode

  // Switch case for  mode selection
  switch (select) {
     case 0:
      //Serial.println("Enter Drive State");
      digitalWrite(s0, HIGH);
      digitalWrite(s1, LOW);
      digitalWrite(s2, LOW);
      digitalWrite(s3, LOW);
      break;

     case 1:
      //Serial.println("Enter Precision Movement State");
      digitalWrite(s0, LOW);
      digitalWrite(s1, HIGH);
      digitalWrite(s2, LOW);
      digitalWrite(s3, LOW);
      break;

     case 2:
      //Serial.println("Enter Robotic Arm State");
      digitalWrite(s0, LOW);
      digitalWrite(s1, LOW);
      digitalWrite(s2, HIGH);
      digitalWrite(s3, LOW);
      break;

     case 3:
      //Serial.println("Enter Demo State");
      digitalWrite(s0, LOW);
      digitalWrite(s1, LOW);
      digitalWrite(s2, LOW);
      digitalWrite(s3, HIGH);
      break;

     default: // In the event of mode select failure, vehicle maintains its position
      //Serial.println("Enter Default(Idle) State");
      digitalWrite(s0, HIGH);
      digitalWrite(s1, HIGH);
      digitalWrite(s2, HIGH);
      digitalWrite(s3, HIGH);
      break;
  } 

  send_data();
  
  // Delay to complete computation
  delay(10);  
}

void send_data() {
  digitalWrite(tx_led, HIGH); // Begin transmit
  set_data(); // Substitute for reading values
  vw_send((uint8_t *)&data, sizeof(data));
  vw_wait_tx(); // Wait until the whole message sends
  digitalWrite(tx_led, LOW);
}

void set_data() {
  data.x_dat = joy_x;
  data.y_dat = joy_y;
  data.state = select;
}

void rdVolt() { // Print the values to the serial monitor

  Serial.print("X Position = ");
  Serial.print(joy_x);

  Serial.print("\nY Position = ");
  Serial.print(joy_y);

  Serial.print("\nMode = ");
  Serial.print(select);

  Serial.print("\n\n-----------------");
}
