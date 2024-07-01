 #include <Wire.h>
 #include <Adafruit_BMP085.h>
 #include "SevSeg.h"

 Adafruit_BMP085 bmp;
 SevSeg sevseg;                                     //Call all library files and mark their usage.

byte numDigits = 6;                                 // To display the full pressure on the 4-bit 7-segment displays, set the digit count to 6.
byte digitPins[] = {2, 3, 4, 5};                    //Connect the common anode of the seven-segment display to the Arduino pin.
byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13 }; //The pins corresponding to the seven-segment display segments a, b, c, d, e, f, g, and dp
bool resistorsOnSegments = false;                   //  Resistors are connected to the digital pins.
byte hardwareConfig = COMMON_ANODE;                 // A common anode 7-segment display is being used.
bool updateWithDelays = false;                      // Default 'false' is Recommended
bool leadingZeros = false;                          // No need to keep leading zeros.
bool disableDecPoint = false;                       //Utilize the decimal point functionality.

int i;
volatile bool enterInterrupt = false;               //Set variable parameters.



void setup() {
//Initialize the seven-segment display.
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint); //Add the previously set parameters.
  sevseg.setBrightness(6);                          //Set the brightness of the seven-segment display.
 if (!bmp.begin()) {                                
    while (1) {}                                    //begin bmp180
  }
}

   
void loop() {

  if (bmp.readTemperature() >= 50 && bmp.readPressure() >= 105000){
  //Enter the interrupt if the current temperature and pressure are greater than the set values of 50 and 105000, respectively.
    enterInterrupt = true;
    } else{
  //Otherwise, enter the normal detection loop.
    enterInterrupt = false;
    }

  if (!enterInterrupt){
  //Non-interrupt normal display loop.
    i=bmp.readTemperature();
    i=i*100;                                         //Extract the current temperature value and multiply it by 100 to display it correctly on the seven-segment display.
    sevseg.setNumber(i);                             //Use the seven-segment display to show the data.
  for(int k=0;k<32000;k++){
    sevseg.refreshDisplay();                         //Rapidly refresh the 7-segment display multiple times to ensure proper display on Proteus.
    }
  
    sevseg.setNumber(bmp.readPressure());            //Extract and display the current pressure value.
  for(int k=0;k<32000;k++){
    sevseg.refreshDisplay();                         //refresh the 7-segment display
    }
  }
    
  if (enterInterrupt){
  //Interrupt loop content.
    sevseg.setNumber(5000,2);                        //Display the set maximum alarm temperature, indicating it with a decimal point at the end.
  for(int k=0;k<32000;k++){
    sevseg.refreshDisplay();                         //refresh the 7-segment display
    }
     
   sevseg.setNumber(105000,2);                       //Display the set maximum alarm pressure
  for(int k=0;k<32000;k++){
    sevseg.refreshDisplay();                         //refresh the 7-segment display
    }
     
  if (bmp.readTemperature() < 50 || bmp.readPressure() < 105000){
  //Exit the interrupt loop if either the temperature or pressure falls below the set values.
    enterInterrupt = false;
    }
  }   
     
}
     

  
