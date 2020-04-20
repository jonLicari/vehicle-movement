#include <VirtualWire.h>
#include <VirtualWire_Config.h> 

// Rx Declarations
const int rx = 12;
const int rx_led = 13;
int joy_x, joy_y, select;
char dat[5];

struct Data_Packet {
  int x_dat;  // Joystick x-axis
  int y_dat;  // Joystick y-axis
  int state;  // Op mode
  int grp_s;  // Manual grip confirm
  int img_s;  // Manual image detect
};
Data_Packet data;

void setup() {
  select = 0;
  joy_x = 0;
  joy_y = 0;
  
  // Rx Initialization
  vw_set_rx_pin(rx);  // Set receive pin
  vw_setup(2000);
  vw_rx_start();  // Start running the receiver PLL 
  pinMode(rx_led, OUTPUT);
    
  Serial.begin(9600);
}

void loop() {
  get_data();
  //select = 1;
  //joy_x = 3;
  //joy_y = 5;
  //Serial.write(select);
  //Serial.write(joy_x);
  //Serial.write(joy_y);
  //delay(1000);
}

void get_data() {
  uint8_t buf[VW_MAX_MESSAGE_LEN]; // Array where message is copied
  uint8_t buflen = VW_MAX_MESSAGE_LEN; 
  // ^^^ should have arrays max size upon input,return#of bytes actually copied
  // ^^^ function itself returns true if the message was verified correct, 
  // ^^^ or false if a message was received but appears to have been corrupted.

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      digitalWrite(13, true); // Flash a light to show received good message
      // Message with a good checksum received, dump it.
      Serial.print("Got: ");

      data.x_dat = buf[0];
      data.y_dat = buf[2];
      data.state = buf[4];
      // Test Block
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
