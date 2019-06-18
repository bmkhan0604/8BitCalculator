# 8-Bit Calculator
A hardware based 8-Bit Calculator all done on breadboard. All the code below is to program the ROM chip in this project as well as documentation for chips used

Project by: Basem Khan

## 555 Timer:
  *Follow the diagram in the documentation for an astable Clock.
  *I used a variable resistor so the clock speed could be changed easily
  
## JK-FlipFlop:
  *Used to Create a 2-bit counter-- 00 01 10 11
  *Attach J and K to HIGH on both FF to set them to toggle
  *Attach the 555 timer CLK to one of the FF's CLK
  *Attach the output of the first FF to the CLK of the second
 
## Mux/Demux:
  *Used to run the displays... Turn one of them on at a time
  
