#define IO7 37
#define IO0 23
#define A14 50
#define A0 22

#define WE 2
#define OE 3
#define CE 4

#define OFF 1   
#define ON 0

uint8_t Read(uint16_t);
void Write(uint8_t,uint16_t);

// Data bits sequenced g-a... no DP
uint8_t Decimal[] ={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67};
uint8_t Hex[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
int x,base=16;
//EEPROM ADDRESS Input/Output LAYOUT
//        INPUT       |          OUTPUT
// 000 0000 XXXX XXXX | Single's Digit in Decimal                                       DONE
// 000 0001 XXXX XXXX |   Ten's Digit in Decimal                                        DONE
// 000 0010 XXXX XXXX | Hundreds Digit in Decimal                                       DONE
// 000 0011 XXXX XXXX |          Unused - All 0's                                       N/A
// 000 0100 XXXX XXXX |   Single's Digit in Hex                                         DONE
// 000 0101 XXXX XXXX |     Ten's Digit in Hex                                          DONE
// 000 0110 XXXX XXXX |   Hundreds Digit in Hex ?? No hundreds place                    N/A
// 000 0111 XXXX XXXX |          Unused - All 0's                                       N/A
// 000 1000 XXXX XXXX | Single's Digit in Decimal (Negative) ---- All inputs are assumed to be negative numbers(2's comp)
// 000 1001 XXXX XXXX |   Ten's Digit in Decimal  (Negative) 
// 000 1010 XXXX XXXX | Hundreds Digit in Decimal (Negative) 
// 000 1011 XXXX XXXX |       Negative Sign       (Negative) 

void setup() {
  Serial.begin(9600);
  for(int i=2; i<=53; i++)
    pinMode(i,OUTPUT);
  digitalWrite(WE,OFF);
  digitalWrite(OE,OFF);
  digitalWrite(CE,OFF);
  delay(1000);
// ----------------------------------------Code-------------------------------------------------------
  //------------READING----------------
  Serial.println("\n\tReading");
  ReadEEPROM();
}
void loop() {
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
  delay(1000);
}

//-------------------------------------------Functions---------------------------------------------

void ReadEEPROM()
{
  base=10;
  x=0;
  Serial.println("\nOnes Place:");
  PageRead(0x00,0x100);// Ones'place
  Serial.println();
  x=6;
  Serial.println("\nTens Place:");
  PageRead(0x100,0x200);// Tens'place
  Serial.println();
  x=2;
  Serial.println("\nHundreds Place:");
  PageRead(0x200,0x300);// Hundredss'place
  Serial.println();
  x=8;
  Serial.println("\nThousands Place/Unused/:");
  PageRead(0x300,0x400);// 
  Serial.println();
  Serial.println("\n\nHEX");
  base=16;
  x=0;
  Serial.println("\nOnes Place:");
  PageRead(0x400,0x500);// Ones'place
  Serial.println("\nTens Place:");
  PageRead(0x500,0x600);// Tens'place
  Serial.println("\nHundreds Place/Unused/:");
  PageRead(0x600,0x700);// Ones'place
  Serial.println("\nThousands Place/Unused/:");
  PageRead(0x700,0x800);// Tens'place
}

void PageRead(uint16_t Address1,uint16_t Address2)
{
  for(uint16_t Address=Address1; Address<Address2; Address++)
  {
    delay(10);
    Serial.print(Read(Address),HEX);
    if((Address-x)%base == base-1)
      Serial.println();
    else
      Serial.print("\t");
  }
}


uint8_t Read(uint16_t Address)       // seems to work with small errors here and there
{
  DataOut(0x00);
  AddressOut(Address);
  for(int i = IO0; i<=IO7; i+=2)
    pinMode(i,INPUT);
  digitalWrite(OE,ON);
  digitalWrite(CE,ON);
  uint8_t Data = 0x00;
  for(int i = IO7; i>=IO0; i-=2)
  {
    Data = Data << 1;
    Data = Data | digitalRead(i);
  }
  digitalWrite(OE,OFF);
  digitalWrite(CE,OFF);
  //Serial.print("Address: ");
  //Serial.print( Address,HEX);
  //Serial.print("\tData: ");
  //Serial.println(Data,HEX); 
  return Data;
}

void Write(uint8_t Data,uint16_t Address)     // Seems to work
{
  // Set IO pins are OUTPUT
  for(int i = IO0; i<=IO7; i+=2)
    pinMode(i,OUTPUT);
  AddressOut(Address);
  DataOut(Data);
  digitalWrite(CE,ON);
  digitalWrite(WE,ON);
  delay(1);
  digitalWrite(WE,OFF); // Data Latched hopefully
  digitalWrite(CE,OFF);
  //Serial.print("Address: ");
  //Serial.print( Address,HEX);
  //Serial.print("\tData: ");
  //Serial.println(Data,HEX); 
}

void DataOut(uint8_t Data)  // Tested and Functional
{
  uint8_t temp = Data;
  bool value;
  for(int i=IO0; i<=IO7; i+=2)
  {
    value = temp & 0x01;
    digitalWrite(i,value);
    /*
    Serial.print("temp: ");
    Serial.println(temp,BIN);
    Serial.print("Value: ");
    Serial.println(value);
    */
    temp = temp >> 1;
  }
}

void AddressOut(uint16_t Address)  // Tested and Functional
{
  uint16_t temp = Address;
  uint8_t value;
  for(int i=A0; i<=A14; i+=2)
  {
    value = temp & 0x01;
    digitalWrite(i,value);
    /*
    Serial.print("temp: ");
    Serial.println(temp,BIN);
    Serial.print("Value: ");
    Serial.println(value);
    */
    temp = temp >> 1;
  }
}
//----------------------------------------------------------Functions for each digit/Condition mentioned above-------------------------
void OnesDecimal()
{
  x=0;
  base=10;
  int Data;
  for(uint16_t Address=0; Address<256; Address++)   // 000 0000 XXXX XXXX | Single's Digit in Decimal 
  {
      Data = Address%10;
      Write(Decimal[Data],Address);
      delay(10);
  }
}

void TensDecimal()
{
  x=6;
  base=10;
  int Data;
  uint16_t Address;
  for(uint16_t Number=0; Number<256; Number++)   // 000 0001 XXXX XXXX |  Ten's Digiit in Decimal 
  {
     Data = Number/10;
     Data = Data%10;
     Address = Number | 0x100;
     Write(Decimal[Data],Address );   
     delay(10); 
  }
}

void HundredDecimal()
{
  x=2;
  base=10;
  int Data;
  uint16_t Address;
  for(uint16_t Number=0; Number<256; Number++)   // 000 0010 XXXX XXXX | Hundreds Digit in Decimal
  {
     Data = Number/100;
     Data = Data%10;
     Address = Number | 0x200;
     Write(Decimal[Data],Address ); 
     //Serial.println(Data,HEX);  
     delay(10); 
  }
}

void ThousandsDecimal()
{
 x=8;
  base=10;
  int Data;
  uint16_t Address;
  for(uint16_t Number=0; Number<256; Number++)   // 000 0011 XXXX XXXX |    Thousands Spot 10s(Always 0 for 8bit)
  {
    Data = 0;
    Address = Number | 0x300;
    //Write(0x00,Address); 
    delay(10);
  }
}

void OnesHex()
{
  x=0;
    base=16;
    int Data;
    uint16_t Address;
    for(uint16_t Number=0; Number<256; Number++)   // 000 0100 XXXX XXXX |   Single's Digit in Hex
    {
       Data = Number%16;
       Address = Number | 0x400;
       Write(Hex[Data],Address ); 
       //Serial.println(Data,HEX);  
       delay(10); 
    }
}

void TensHex()
{
  x=0;
  base=16;
  int Data;
  uint16_t Address;
  for(uint16_t Number=0; Number<256; Number++)   // 000 0101 XXXX XXXX |    Ten's Digiit in Hex
  {
    Data = Number/16;
    Data = Data%16;
    Address = Number | 0x500;
    //Write(Hex[Data],Address ); 
    //Serial.println(Address,BIN);  
    delay(10); 
  }
}

void HundredsHex()
{
  x=0;
  base=16;
  int Data;
  uint16_t Address;
  for(uint16_t Number=0; Number<256; Number++)   // 000 0011 XXXX XXXX |    Thousands Spot 10s(Always 0 for 8bit)
  {
    Data = 0;
    Address = Number | 0x600;
    Write(0x00,Address); 
    delay(10);
  }
}

void ThousandsHex()
{
  x=0;
  base=16;
  int Data;
  uint16_t Address;
  for(uint16_t Number=0; Number<256; Number++)   // 000 0011 XXXX XXXX |    Thousands Spot 10s(Always 0 for 8bit)
  {
    Data = 0;
    Address = Number | 0x700;
    Write(0x00,Address); 
    delay(10);
  }
}
