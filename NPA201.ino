/*
Copyright (c) 2016 Amphenol Advanced Sensors
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "Wire.h"


#define I2CAddress 0x27
  long aa, bb, cc, dd, ee;
  float temperature = 0.00;
  float pressure = 0.00;


void setup() {
    pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);        //switches sensor on
  Serial.begin(115200);  // start serial for output
  Wire.begin();
  Serial.println("Amphenol Advanced Sensors");
  Serial.println("Arduino NPA-201 Monitor");
}


void NPA201ReadData()
{


  // Initiate Comms to device, initiate measure and read 5 bytes of data
  Wire.beginTransmission(I2CAddress);
  Wire.write(0xAC);  Wire.write(0);
  Wire.endTransmission();
  delay(21);
  Wire.requestFrom(I2CAddress, 5);
  aa = Wire.read();
  bb = Wire.read();
  cc = Wire.read();
  dd = Wire.read();
  ee = Wire.read();

  //Pressure Value = BridgeData/65535 * (1260-260) + 260 (hPa)
  //Temperature_Value = TempData/65535 * (85+40) - 40   (°C)
  // additional calculations to make values reasonable based on accuracy

  pressure =  (float) ((bb << 8) | cc) / 65535 * 1000 + 260;
  pressure = round(pressure);

  temperature =  (float) ((dd << 8) | ee) / 65535 * 125 - 40;
  temperature = round(temperature * 10);
  temperature = temperature / 10;
  
}

void loop() {
  NPA201ReadData();
  
  Serial.print(int (pressure)); Serial.print("  "); Serial.print("hPa"); Serial.print("  ");
  Serial.print(temperature); Serial.print("  "); Serial.write(176); Serial.println("C");

  delay(3000);
}
