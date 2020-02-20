/* author: jlicari, ELE 800, Ryerson University
 *  Created: 8 February 2020
 *  mtr.ino
 *  Vehicle mode selection, sensor assisted navigation, wireless control
 */

#include <VirtualWire.h>
#include <Servo.h>
#include "mtr.h"
#include "arm.h"
#include "manual.h"

void setup() {
  Servo sx, sy;
  
  // Variable Initialization
  state = 3; // Default demo state
  img = false;
  grp = false;
  sx.attach(x);
  sy.attach(y);
  posx = 0; // Default sx position to 0
  posy = 0; // Default sy position to 0
  
  // Sensor Port Initialization
  pinMode(jx, INPUT);
  pinMode(jy, INPUT);
  pinMode(md, INPUT);
  pinMode(im, INPUT_PULLUP);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(grp, INPUT);

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
  attachInterrupt(digitalPinToInterrupt(im), detect, RISING);
  attachInterrupt(digitalPinToInterrupt(grp), grip_detect, CHANGE);

  // Initialize Serial monitor
  Serial.begin(250000);
}

void loop() {
  if (analogRead(cntl) != 1) { // auto
    demo();
  }
  else {
    manual();
  }
  //send_data(); // test serial connect before wireless
  // delay(10); Remove unless required for performance
}

void detect() {
  img = !img;
}

void grip_detect() {
  grip_cnfm = !grip_cnfm;
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

int measure() {
  int i;
  for (i = 0; i < 2; i++) {
    // Clear trig
    digitalWrite(trig, LOW);
    delayMicroseconds(2);

    // Set trig HIGH for 10 ms
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    // Read echo and return travel time in ms
    dur = pulseIn(echo, HIGH);

    // Calculate distance
    dist += dur*0.034/2;
  }
  dist = dist/i;
  Serial.println("Distance: ");
  Serial.print(dist);

  return(dist); 
}

void deliver(unsigned long time) {
  // 180 degree turn
  analogWrite(ml, 255);
  analogWrite(mr, 200);
  delay(2000);
  
  // Return
  analogWrite(ml, 200);
  analogWrite(mr, 200);
  delay(time); 
} 

void demo() {
  int spd, d;
  bgn = millis();
    
  while(img != false) {
    Serial.println("true loop ");

    d = measure();
    Serial.println("d = ");
    Serial.println(d);
    
    if (d > 20) { // Distance > 20cm from object
      spd = 255;
      analogWrite(ml, spd);
      analogWrite(mr, spd);
    }
    else if (d > 8 && d < 20) { // Slow: 20cm > Distance > 8cm
      spd = 175;
      analogWrite(ml, spd);
      analogWrite(mr, spd);
    }
    else { // Stop: Distance <= 7cm
      tot_time = millis() - bgn;
      analogWrite(ml, 0);
      analogWrite(mr, 0);
      grab(); // See arm.h
      deliver(tot_time); // Delivers object to requester
    }
  }
  
  // object not detected
  if (img != true) {
    Serial.println("false");
    analogWrite(ml, 140);
    analogWrite(mr, 50);
  }
}
