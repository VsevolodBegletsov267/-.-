/*   Данный скетч делает следующее: передатчик (TX) отправляет массив
     данных, который генерируется согласно показаниям с кнопки и с
     двух потенциомтеров. Приёмник (RX) получает массив, и записывает
     данные на реле, сервомашинку и генерирует ШИМ сигнал на транзистор.
    by AlexGyver 2016
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

byte potent0 = 0;
byte potent1 = 1;
byte potent2 = 2;
byte potent3 = 3;
byte potent4 = 4;

byte transmit_data[5]; 
byte latest_data[5];    
boolean flag;           

void setup() {
  Serial.begin(9600);   //открываем порт для связи с ПК

  radio.begin();           
  radio.setAutoAck(1);     
  radio.setRetries(0, 15);    
  radio.enableAckPayload(); 
  radio.setPayloadSize(32);   
  
  radio.openWritingPipe(address[0]);  
  radio.setChannel(0x60);     

  radio.setPALevel (RF24_PA_MAX);  
  radio.setDataRate (RF24_250KBPS); 

  radio.powerUp();       
  radio.stopListening();  
}

void loop() {
  transmit_data[0] = map(analogRead(potent0), 0, 1023, 0, 180);
  transmit_data[1] = map(analogRead(potent1), 0, 1023, 0, 180);
  transmit_data[2] = map(analogRead(potent2), 0, 1023, 0, 180);  
  transmit_data[3] = map(analogRead(potent3), 0, 1023, 0, 180);
  transmit_data[4] = map(analogRead(potent4), 0, 1023, 0, 180);

  for (int i = 0; i < 4; i++) {               
    if (transmit_data[i] != latest_data[i]) { 
      flag = 1;                               
      latest_data[i] = transmit_data[i];      
    }
  }

  if (flag == 1) {
    radio.powerUp();  
    radio.write(&transmit_data, sizeof(transmit_data));
    flag = 0;          
    radio.powerDown();  
  }
}
