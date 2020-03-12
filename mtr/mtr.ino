/* author: jlicari, ELE 800, Ryerson University
 *  Created: 8 February 2020
 *  mtr.ino
 *  Vehicle mode selection, sensor assisted navigation, wireless control
 */
#include <Servo.h>
#include <VirtualWire.h>
#include <VirtualWire_Config.h> 
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
  
  // Rx Initialization
  vw_set_rx_pin(rx);  // Set receive pin
  vw_setup(2000);
  vw_rx_start();  // Start running the receiver PLL 
  pinMode(rx_led, OUTPUT);
  
  // Interrupt 
  attachInterrupt(digitalPinToInterrupt(im), detect, RISING);
  attachInterrupt(digitalPinToInterrupt(grp), grip_detect, CHANGE);

  // Initialize Serial monitor
  Serial.begin(250000);
}

void loop() {

  get_data(); // Start receiving the wireless data

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

void get_data() {
  uint8_t buf[VW_MAX_MESSAGE_LEN]; // Array where message is copied
  uint8_t buflen = VW_MAX_MESSAGE_LEN; 
  // ^^^ should have arrays max size upon input,return#of bytes actually copied
  // ^^^ function itself returns true if the message was verified correct, 
  // ^^^ or false if a message was received but appears to have been corrupted.

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      //int i;

      digitalWrite(13, true); // Flash a light to show received good message
      // Message with a good checksum received, dump it.
      Serial.print("Got: ");
  
      //for (i = 0; i < buflen; i++)
      //{
       //   Serial.print(buf[i], HEX);
         // Serial.print(" ");
      //}
      data.x_dat = buf[0];
      data.y_dat = buf[2];
      data.state = buf[4];
      Serial.print("X Pos = ");
      Serial.println(data.x_dat);
      Serial.print("Y Pos = ");
      Serial.println(data.y_dat);
      Serial.print("Mode = ");
      Serial.println(data.state);
      
      digitalWrite(13, false);
    }

    set_data();
}

void set_data() {
  
  select = data.state;
  joy_x = data.x_dat;
  joy_y = data.y_dat;
}

void detect() {
  img = !img;
}

void grip_detect() {
  grp = !grp;
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
