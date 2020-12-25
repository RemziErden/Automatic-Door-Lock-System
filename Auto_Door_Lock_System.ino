/*
 Project Name  : AUTOMATIC DOOR LOCK SYSTEM WITH RFID CARD READER
 Revision Date : 20.12.2020
 Author        : Remzi Erden
 // ---------------------------------------------------------------------------------------------------------------------------------------------
 Purpose       : To implement an automatic door lock system, which is critical in protecting your home and workplace against risks such as theft, 
                 and to ensure that only people with access can open the door. 
 // ---------------------------------------------------------------------------------------------------------------------------------------------
 System Input  : 
                 1)RFID Card Reader: To have the card read.
 System Output :
                 1)Servo Motor     : To make the door open.
                 2)LCD Display     : To make the system more understandable.
 // ---------------------------------------------------------------------------------------------------------------------------------------------
 System Work   : The card reading process is performed by the RFID card reader, if the card with the defined ID is scanned, the servo motor operates
                 and the door opens and the green LED lights up at the same time, when a card that does not have the defined ID is scanned, the red
                 LED lights up and the door doesn't open.
 // ---------------------------------------------------------------------------------------------------------------------------------------------                               
 */

 // DEFINITIONS
#include <Servo.h>                   // Servo motor library added.
#include <MFRC522.h>                 // Libraries required for the RFID module has been added.
#include <SPI.h>                     
#include <Wire.h>                    // Libraries required for the LCD display were added.
#include <LiquidCrystal_I2C.h>       

LiquidCrystal_I2C ekran(0x27,16,2);  // Screen type and size are defined. 
MFRC522 rfid(10,9);                  // RFID module reset (9) sda (10) connection has been defined.
Servo motor;                         // The servo motor is named.

int period = 3000;                   // Duration is defined.  
 
byte ID[4]={58,208,152,25};          // Valid card ID defined.
// ---------------------------------------------------------------------------------------------------------------------------------------------

// SETUP CODE AREA
void setup() 
{
  ekran.init();                     // The screen has been included.
  ekran.backlight();                // The screen backlight is turned on.
  ekran.setCursor(2,0);             // Display row and column number has been defined. 
  ekran.print("Please Scan");       // Text has been written in the defined row and column on the screen.
  ekran.setCursor(3,1);             // Display row and column number has been defined.
  ekran.print("Your Card");         // Text has been written in the defined row and column on the screen.
  Serial.begin(9600);               // Serial communication started.
  SPI.begin();                      // SPI communication started.
  rfid.PCD_Init();                  // RFID is started.
  motor.attach(8);                  // The pin to which the servo motor is connected has been defined.
  motor.write(0);                   // The motor was started at 0 degrees.
}
//---------------------------------------------------------------------------------------------------------------------------------------------

// MAIN CODE AREA
void loop() 
{
  if(rfid.PICC_IsNewCardPresent())              // Has the card been scanned?
  {
  
    if(rfid.PICC_ReadCardSerial())              // Scanning the card.   
    {
    
      Serial.print("Valid card ID: ");          // The ID of the card that was scanned was printed on the serial screen.
      Serial.print(rfid.uid.uidByte[0]);
      Serial.print(",");
      Serial.print(rfid.uid.uidByte[1]);
      Serial.print(",");
      Serial.print(rfid.uid.uidByte[2]);
      Serial.print(",");
      Serial.println(rfid.uid.uidByte[3]);
     
      if(rfid.uid.uidByte[0]==ID[0]&&          // The ID of the card read is compared with the ID registered in the system.
      rfid.uid.uidByte[1]==ID[1]&&
      rfid.uid.uidByte[2]==ID[2]&&
      rfid.uid.uidByte[3]==ID[3])
      {  
      Serial.println("Door opened");          // If the scanned card is registered in the system, respectively
      motor.write(90);                        // Door opened.
      digitalWrite(2, HIGH);                  // The green led lights up.          
      ekran.clear();                          // The writings on the screen have been deleted.
      ekran.setCursor(4,0);                   // Display row and column number has been defined.
      ekran.print("WELCOME");                 // Text was written on the screen.
      delay(period);                          // The open time of the door has been specified.
      digitalWrite(2, LOW);                   // The led is off.
      motor.write(0);                         // The door is closed.
      setup();                                // Returned to void setup function, to reset the purpose screen information.
      }
      
    
      else                                    // If the scanned card ID is not registered in the system, respectively
      {
      Serial.println("Undefined card");       // Undefined card is printed on serial screen.
      digitalWrite(3, HIGH);                  // The red led lights up.
      ekran.clear();                          // The writings on the screen have been deleted. 
      ekran.setCursor(2,0);                   // Display row and column number has been defined.
      ekran.print("Invalid Card");            // It is written that the card is invalid on the lcd screen.
      delay(1000);                            // The time has been added for the next post.
      ekran.setCursor(3,1);                   // Display row and column number has been defined.
      ekran.print("Try Again");               // Text was written on the screen.
      delay(period);                          // The duration of the text on the screen has been determined.
      setup();                                // Returned to void setup function, to reset the purpose screen information.
      digitalWrite(3, LOW);                   // The led is off.
      }
    } 
  }
    rfid.PICC_HaltA();                        // RFID communication was terminated.
  }
 // ------------------------------------------------------------------------------------------------------------------------------------------ 
