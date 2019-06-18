# 8-Bit Calculator
A hardware based 8-Bit Calculator all done on breadboard. All the code below is to program the ROM chip in this project as well as documentation for chips used

Project by: Basem Khan

## 555 Timer:
  * Follow the diagram in the documentation for an astable Clock.
  * I used a variable resistor so the clock speed could be changed easily
  
## JK-FlipFlop:
  * Used to Create a 2-bit counter-- 00 01 10 11
  * Attach J and K to HIGH on both FF to set them to toggle
  * Attach the 555 timer CLK to one of the FF's CLK
  * Attach the output of the first FF to the CLK of the second
 
## Mux/Demux:
  * Used to run the displays... Turn one of them on at a time
  * Connect COM to GND
  * A and B to Outputs of the JK flip flops
  * Channel 0-3 to coorisponding display outputs with a resistor inbetween the IC and 7Segment Display

## EEPROM:
 * Connect WE HIGH, OE LOW, CE LOW : this enables output for the display
 * The Data Lines attach to display as specified in the 7segment arduino code under the AT28C256 folder
 * Address 0-7 are assumed to be the number attempting to be displayed
 * Address 8 and 9 are connected to JK the flip flips outputs
 * Address 10: Switches between Hex and Decimal Mode
 * Address 11: not programmed yet, (Negative Flag)
 * Address 12-14: Not Used
