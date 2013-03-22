/*  
   Card reader decoder
   
   The card reader used is the HID-RP40-CGNNN.
   The RP40 comes in two flavours, one uses the WEIGAND protocol the other uses Clock and Data. 
   The HID-RP40-CGNNN uses clock and data and the code in this sketch has been written with that in mind.
   For more information on how to read the WEIGAND format go to this website:
   http://www.pagemac.com/azure/arduino_wiegand.php
   
   This sketch has only been tested with HID iClass DH cards
   
   For more information related to the different formats HID readers provide 
   data download this pdf:
   http://rfid-reader.googlecode.com/files/Open%20Format.pdf
   
   Format from raw bit stream:
   
   <25 leading zeros>  xx yyyy yyyy yyyy zzzz zzzz zzzz zzzz zzzz aaaa<trailing zeros>
   
   x: start
   y: Facility Number
   z: Card ID
   a: Parity Check stuff
   
   In this sketch only the y and z bits are being decoded and the rest are being ignored.
   
   created 22 Mar 2013
   by Surya Mattu and Luis Daniel
   
*/

//These pins are labelled on the card reader.
int clockPin = 3;
int dataPin = 2;

int facilityNumber = 0;
long idNumber = 0;
volatile int count = 0;
volatile byte data[300];

void setup()
{
  pinMode(dataPin, INPUT);
  pinMode(clockPin, INPUT);
  attachInterrupt(1, readBit, RISING);
  Serial.begin(9600);
  for (int a = 0; a < 200; a++)
  {
    data[a] = 0;
  }
  interrupts();

}
void loop()
{

  //TODO: Make this less arbitrary. 
  //211 seems to be the number of bits that come in per tap.
  if (count >= 211)
  {
    Serial.println("READ COMPLETE!");

    //decode the facility number    
    for (int i = 27; i <39; i++)
    {
      facilityNumber <<= 1;
      facilityNumber |= data[i];
    }

    //decode the id number 
    for (int j = 39; j <59; j++)
    {
      idNumber <<= 1;
      idNumber |= data[j];
    }

    //send this data via serial
    Serial.println(facilityNumber);
    Serial.println(idNumber);

    clearData();

    Serial.println("here");
    Serial.println(count);
  }

}

void clearData(){
  count = 0;
  idNumber = 0;
  facilityNumber= 0;

  for (int a = 0; a < 200; a++)
  {
    data[a] = 0;
  }
}

void readBit()
{

  Serial.print(".");
  if (digitalRead(dataPin) == HIGH)
  {
    data[count] = 0;
  }
  else
  {
    data[count] = 1;
  }
  count++;
}







