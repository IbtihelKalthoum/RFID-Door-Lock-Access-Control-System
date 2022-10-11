#include <SPI.h>
#include <MFRC522.h>

#define SS_1_PIN 21
#define RST_PIN 22
#define LEDv 12
#define LEDr 32
 
MFRC522 rfid(SS_1_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 
// Init array that will store new NUID 
byte nuidPICC[3];
void setup() { 
  
  Serial.begin(9600);
  Serial.println("bonjour");
  // Init LEDs 
  pinMode(LEDv, OUTPUT);
  pinMode(LEDr, OUTPUT);  
  digitalWrite(LEDv, LOW);
  digitalWrite(LEDr, LOW);  
}


void loop() {
RFID();
 }
//===========================================
void RFID()
{
 //Serial.println("pirmais");
    SPI.begin(); // Init SPI bus
 rfid.PCD_Init(); // Init MFRC522 
 for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  } 
  // Look for new 1 cards
  if ( ! rfid.PICC_IsNewCardPresent())
  return;
    // Verify if the NUID has been readed
  if (  !rfid.PICC_ReadCardSerial())
  return;
 // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
      }
    
  /*  Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);*/

    Serial.println();
  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
   String content= "";
  byte letter;
  for (byte i = 0; i < rfid.uid.size; i++) 
  {
     Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(rfid.uid.uidByte[i], HEX);
     content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(rfid.uid.uidByte[i], HEX));
  }
  Serial.println();
   content.toUpperCase();
  Serial.println("Card : "+content);

 if (content.substring(1) == "B3 B1 64 11") 
  {
    Serial.println("Authorized access");
    Serial.println();
    digitalWrite(LEDv, HIGH);
    delay(30000); 
      digitalWrite(LEDv, LOW);
    delay(30000);}
  if(content.substring(1) == "")
  {Serial.println("No card");}
 
 if (content.substring(1) != "B3 B1 64 11")  {
    Serial.println("Access denied");
    digitalWrite(LEDr, HIGH);
      delay(20000); 
      digitalWrite(LEDr, LOW);
            delay(20000);      
  }
  SPI.end();
}
/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
