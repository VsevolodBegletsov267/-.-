#include <iarduino_MultiServo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);  

iarduino_MultiServo MSS0;
iarduino_MultiServo MSS1;
iarduino_MultiServo MSS2;
iarduino_MultiServo MSS3;

byte recieved_data[4]; 

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; 

void setup() {
  Serial.begin(9600);     

  radio.begin();
  radio.setAutoAck(1);        
  radio.setRetries(0, 15);    
  radio.enableAckPayload();   
  radio.setPayloadSize(32);

  radio.openReadingPipe(1, address[0]);    
  radio.setChannel(0x60);  

  radio.setPALevel (RF24_PA_MAX);  
  radio.setDataRate (RF24_250KBPS); 

  radio.powerUp();        
  radio.startListening();   

  MSS0.servoSet(15,SERVO_SG90);
  MSS1.servoSet(4,SERVO_SG90);
  MSS2.servoSet(8,SERVO_SG90);
  MSS3.servoSet(12,SERVO_SG90);
  MSS0.begin();
  MSS1.begin();
  MSS2.begin();
  MSS3.begin();
}

void loop() {
  byte pipeNo;
  while ( radio.available(&pipeNo))
    radio.read(&recieved_data, sizeof(recieved_data));

    MSS0.servoWrite(0,recieved_data[0]);
    MSS1.servoWrite(4,recieved_data[1]);
    MSS2.servoWrite(8,recieved_data[2]);
    MSS3.servoWrite(12,recieved_data[3]);
    delay(15);
  }
