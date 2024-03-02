#include <SPI.h>
#include "DHT.h" 
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> // wire kütüphanesi çağrıldı
LiquidCrystal_I2C lcd(0x27,16,2); 
#define RST_PIN 9
#define SS_PIN 10
#define ONE_WIRE_BUS 6
//Herhangi bir OneWire cihazıyla iletişim kurmak için bir oneWire örneği kuruyoruz.


int DHTPIN =7  ;// DHT sensörün bağlı olduğu pini tanımla
DHT dht(DHTPIN, DHT11);  // dht adında sensör nesnesi tanımla. 
byte readCard[4];
String MasterTag = "F974FD3";  // REPLACE this Tag ID with your Tag ID!!!
String tagID = "";
float santigrat;
MFRC522 mfrc522(SS_PIN, RST_PIN);
bool fan = true;

void setup() 
{
  // Initiating
  SPI.begin(); // SPI bus
  mfrc522.PCD_Init(); // MFRC522
  lcd.begin(); // LCD screen
  pinMode(2,OUTPUT);
  dht.begin();  //dht sensörü başlat
  lcd.clear();
  lcd.print("Fiat Linea Hazir");
  lcd.setCursor(0, 1);
  lcd.print("Karti taratin >>");
  digitalWrite(2,HIGH);  // Röleyi açık konuma getir
}

void loop() 
{
  
  //Wait until new tag is available
  while (getID()) 
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    
    if (tagID == MasterTag) 
    {
      
      lcd.print("Giris Basarili!");
      delay(500);
      while(true){
        
        santigrat = dht.readTemperature();  //Sıcaklık bilgisini oku
        lcd.clear();
        lcd.print("Level:");
        lcd.setCursor(6, 0);
        lcd.print(analogRead(A0));
        lcd.setCursor(0, 1);
        lcd.print("Fan:");
        if(fan==false){
          lcd.print("Off");        
        }
        else{
          lcd.print("On");
        }
        lcd.setCursor(7, 1);
        
        lcd.print("Tmp");
        lcd.print(santigrat);
        lcd.print("\337C");
        if(santigrat>24){
          fan = true;
          digitalWrite(2,LOW);  // Röleyi açık konuma getir
        }
        else{
          fan = false;
          digitalWrite(2,HIGH);  // Röleyi açık konuma getir
        }
        delay(1000);
      }
    }
    else
    {
      lcd.print(" Kart Reddedildi !");
    }
    
      lcd.setCursor(0, 1);
      lcd.print(" ID : ");
      lcd.print(tagID);
      
    delay(2000);

    lcd.clear();
    lcd.print("Fiat Linea Hazir ");
    lcd.setCursor(0, 1);
    lcd.print("Kart taratin >>");
  }
}

//Read new tag if available
boolean getID() 
{
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
  return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
  return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) { // The MIFARE PICCs that we use have 4 byte UID
  //readCard[i] = mfrc522.uid.uidByte[i];
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}
