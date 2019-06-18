// Designed using Arduino Mega'
// 28 Lead Cerdip/PDIP/Flatpack/SOIC 
// Low/High refers to the MC's output state, while On/Off refers to the pin state with invertesion taken into consideration

// Writing: Chip Enable(ON/Off), Output Enable(OFF), Write Enable(ON/OFF)
//      To write set OE to the above state. A low pulse on CE and WE, with data latching on the first rising edge. The length of the pulse twc:10ms or less   
// Software Data Protection: 
//      Data in format IO7-IO0; Address A14-A0  - 0xAA to 0x5555 is B'1010 1010 to B'0101 0101 0101 0101
//      Enable Algorithm: 0xAA to 0x5555; 0x55 to 0x2AAA; 0xA0 to 0x5555; Writes Enabled
//      Disable Algorithm: 0xAA to 0x5555; 0x55 to 0x2AAA; 0x80 to 0x5555; 0xAA to 5555; Ox55 to 0x2AAA; Ox20 to 0x5555

// Pin# , Name - Function and use                                                                   Arduino Pin Connection
// 28 Vcc - 5V with %5 error; if below 3.8 chip disables                                                       5V`
// 27 WE  - Write Enable: Inverted Input, Low Pulse(ON) with OE High(OFF)                                      2
// 26 A13 - Address bit 13, B'00X0 0000 0000 0000; used to read/write                                          48
// 25 A8  - Address bit 08, B'0000 000X 0000 0000; used to read/write                                          38
// 24 A9  - Address bit 09, B'0000 00X0 0000 0000; used to read/write                                          40
// 23 A11 - Address bit 11, B'0000 X000 0000 0000; used to read/write                                          44
// 22 OE  - Output Enable: Inverted Input, To Write needs to be High(OFF)                                      3
// 21 A10 - Address bit 10, B'0000 0X00 0000 0000; used to read/write                                          42
// 20 CE  - Chip Enable: Inverted Input, Always Low(ON) regardless of R/W                                      4
// 19 IO7 - Input/Output: B'X000 0000; used to enter data on Write, or output on Read                          37
// 18 IO6 - Input/Output: B'0X00 0000; used to enter data on Write, or output on Read                          35
// 17 IO5 - Input/Output: B'00X0 0000; used to enter data on Write, or output on Read                          33
// 16 IO4 - Input/Output: B'000X 0000; used to enter data on Write, or output on Read                          31
// 15 IO3 - Input/Output: B'0000 X000; used to enter data on Write, or output on Read                          29
// 14 GND - Ground Connection                                                                                  GND
// 13 IO2 - Input/Output: B'0000 0X00; used to enter data on Write, or output on Read                          27 
// 12 IO1 - Input/Output: B'0000 00X0; used to enter data on Write, or output on Read                          25
// 11 IO0 - Input/Output: B'0000 000X; used to enter data on Write, or output on Read                          23
// 10 A0  - Address bit 00, B'0000 0000 0000 000X; used to read/write                                          22
// 09 A1  - Address bit 01, B'0000 0000 0000 00X0; used to read/write                                          24
// 08 A2  - Address bit 02, B'0000 0000 0000 0X00; used to read/write                                          26
// 07 A3  - Address bit 03, B'0000 0000 0000 X000; used to read/write                                          28
// 06 A4  - Address bit 04, B'0000 0000 000X 0000; used to read/write                                          30
// 05 A5  - Address bit 05, B'0000 0000 00X0 0000; used to read/write                                          32
// 04 A6  - Address bit 06, B'0000 0000 0X00 0000; used to read/write                                          34
// 03 A7  - Address bit 07, B'0000 0000 X000 0000; used to read/write                                          36
// 02 A12 - Address bit 12, B'000X 0000 0000 0000; used to read/write                                          46
// 01 A14 - Address bit 14, B'0X00 0000 0000 0000; used to read/write                                          50

//Data and Address Pins are spaced 2 pins from each other
#define IO7 37
#define IO0 23
#define A14 50
#define A0 22

#define WE 2
#define OE 3
#define CE 4
// Used to clarify inverted inputs on OE and WE; CE is always grounded
#define OFF 1   
#define ON 0

void Read(uint16_t Address);
void Write(uint8_t Data,uint16_t Address);
void Unlock();

void setup() {
  Serial.begin(9600);
// Set all pins to OUTPUT
  for(int i=2; i<=53; i++)
  {
    pinMode(i,OUTPUT);
  }
// All Chip Inputs OFF except CE, which is always ON
  digitalWrite(WE,OFF);
  digitalWrite(OE,OFF);
  digitalWrite(CE,OFF);
  delay(1000);
// Test Code
   //Lock();
//Serial.println("\n\t::Writing::");
  //Write(0xBB,0x7FFE); // Should Read Last Address of the EEPROM
  
  Serial.println("\n\t::Reading::");
  //for(uint16_t j=0x0; j<0x8000;j++)
  // Read(j);
  Read(0x7FFF);
}

void loop() {
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
  delay(1000);
}

void Unlock()   // Maybe Works
{
    // Set IO pins are OUTPUT
  for(int i = IO0; i<=IO7; i+=2)
    pinMode(i,OUTPUT);
  Serial.println("  ::Unlocking EEPROM::");
  WritePage(0xAA,0x5555);
  WritePage(0x55,0x2AAA);
  WritePage(0x80,0x5555);
  WritePage(0xAA,0x5555);
  WritePage(0x55,0x2AAA);
  WritePage(0x20,0x5555);
  //WritePage(0xAA,0x7FFF);
  Serial.println("Done Hopefully");
}

void Lock()   // Maybe Works
{
    // Set IO pins are OUTPUT
  for(int i = IO0; i<=IO7; i+=2)
    pinMode(i,OUTPUT);
  Serial.println("  ::Unlocking EEPROM::");
  WritePage(0xAA,0x5555);
  WritePage(0x55,0x2AAA);
  WritePage(0xA0,0x5555);
  //WritePage(0xAA,0x7FFF);
  Serial.println("Done Hopefully");
}

void WritePage(uint8_t Data,uint16_t Address)   // maybe Works
{
  AddressOut(Address);
  digitalWrite(CE,ON);
  digitalWrite(WE,ON);
    DataOut(Data);
  digitalWrite(CE,OFF); // Data Latched hopefully
  digitalWrite(WE,OFF); 
}

void Read(uint16_t Address)       // seems to work with small errors here and there
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
  if(Data != 0xFF)
  {
  Serial.print("Address: ");
  Serial.print( Address,HEX);
  Serial.print("\tData: ");
  Serial.println(Data,HEX); 
  }
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
  Serial.print("Address: ");
  Serial.print( Address,HEX);
  Serial.print("\tData: ");
  Serial.println(Data,HEX); 
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
