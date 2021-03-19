#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <sss.h>
#include <string.h>

void writeToRFID(char *source);
void readFromRFID(char *destination);
void addDelay(int seconds);
void exitUno();
void printToSerial(char *data);

#define SS_PIN 10
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
int option = 2;

void setup()
{
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
}

void loop()
{
  // char share1[15] = "_ZeZdY";
  // char share2[15] = "PKVKUJ";
  char scanned[15] = "shit";
  char secret[15] = "manoj";
  
  // int secret_length = strlen(share1);

  if (option == 1)
  {
    Serial.print("\nPlace RFID in 5 Secs \n");
    delay(1000);
    writeToRFID(secret);
    option = 0;
  }
  if (option == 2)
  {
    Serial.print("\nPlace RFID in 5 Secs \n");
    delay(1000);
    readFromRFID(scanned);
    Serial.print("\n");
    printToSerial(scanned);
    option = 0;
  }
}

void printToSerial(char *data)
{
  for (int i = 0; i < 6; i++)
  {
    Serial.write(data[i]);
  }
}

void addDelay(int seconds)
{
  Serial.print("\n");
  for (int i = seconds; i > 0; i--)
  {
    Serial.print(i);
    Serial.print("..");
    delay(1000);
  }
  Serial.print("\n");
}

void exitUno()
{
  Serial.print("\nTERMINATED\n");
  exit(0);
}

/* Can Write Data to Block 4 (max of length 18)  */
void writeToRFID(char *source)
{
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //----------------------------------------------

  byte buffer[20];
  byte block;
  MFRC522::StatusCode status;
  byte len = strlen(source);
  for (byte i = 0; i < 16; i++)
  {
    buffer[i] = (byte)source[i];
  }

  for (byte i = len; i < 16; i++)
    buffer[i] = ' '; // pad with spaces

  block = 4;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Write block
  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
    Serial.println(F("** Write success **"));

  Serial.println(" ");

  delay(1000);
  mfrc522.PICC_HaltA();      // Halt PICC
  mfrc522.PCD_StopCrypto1(); // Stop encryption on PCD
}

/* Can Read Data Writen in Block 4 (max of length 18)  */
void readFromRFID(char *destination)
{
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;

  //some variables we need
  byte block;
  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //-------------------------------------------

  byte buffer1[18];

  block = 4;
  len = 18;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  Serial.print(F("**Reading.."));

  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer1[i] != 32) //32 is space
    {
      Serial.write(buffer1[i]);
    }
  }
  
  Serial.print(F("Done**"));

  delay(1000); //change value if you want to read cards faster

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}