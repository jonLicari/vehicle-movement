/* author: jlicari, ELE 800, Ryerson University
 *  Created: 8 February 2020
 *  mtr.ino
 *  Vehicle mode selection, sensor assisted navigation, wireless control
 */
#include <Servo.h>
#include "mtr.h"
#include "manual.h"

Servo sx, sy;

void setup() {
  // Servo Initialization
  sx.attach(11);
  sy.attach(12);
  posx = 0; // Default sx position to 0
  posy = 0; // Default sy position to 0
  sx.write(posx);
  sy.write(posy);    
  
  // Variable Initialization
  img = false;
  grp = false;

  // Sensor Port Initialization
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
  
  // Interrupt 
  attachInterrupt(digitalPinToInterrupt(im), detect, RISING);
  attachInterrupt(digitalPinToInterrupt(grp), grip_detect, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(face), face_detect, RISING);

  // Initialize Serial monitor
  Serial.begin(115200);
}

void loop() {

  //get_data(); // Start receiving the wireless data
  if (Serial.available()) {
    delay(1000);
    while(Serial.available()) {
      select = Serial.read();
      joy_x = Serial.read();
      joy_y = Serial.read();
    }
  }
  Serial.print("\nSelect = ");
  Serial.print(select);
  Serial.print("\nX = ");
  Serial.print(joy_x);
  Serial.print("\nY = ");
  Serial.print(joy_y);
  delay(1000);
  
   // Switch case for flight mode selection
  switch (select) {
     case 0:
      drive();
      //Serial.println("Enter Drive State");
      break;

     case 1:
      slow();
      //Serial.println("Enter Precision Movement State");
      break;

     case 2:
      manual_grab();
      //Serial.println("Enter Robotic Arm State");
      break;

     case 3:
      demo();
      //Serial.println("Enter Demo State");
      break;

     default: // In the event of mode select failure, vehicle maintains its position
      idle();
      //Serial.println("Enter Default(Idle) State");
      break;
  }  
}

void detect() {
  img = !img;
}

void grip_detect() {
  grp = !grp;
  // Send the pi grip detect signal
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

void grab() {
  // sx operation
  while(grp != true) { // or digitalRead(pin12) != HIGH
    sx.write(posx);
    posx++;
  }
  
  // sy operation
  for (posy = 0; posy < ymax; posy++) {
    sy.write(posy);
  }
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
