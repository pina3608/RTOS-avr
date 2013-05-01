
#include <SPI.h>
#include "bma180.h"
#define DEBUGOFF

BMA180 *bma180;


void setup(){
  
  bma180 = new BMA180();
  Serial.begin(9600);
  delay(2000);

}

void loop(){

  bma180->readAccel(); 
  printAccel();
  delay(3000);
}


void printAccel(){
  
  Serial.print("X = ");
  Serial.print(bma180->getXValFloat());
  Serial.print("  Y = ");
  Serial.print(bma180->getYValFloat());
  Serial.print("  Z = ");
  Serial.println(bma180->getZValFloat());
  Serial.print("Temperature(C) = ");
  Serial.println(map((int8_t)bma180->getTemp(),-128,127,-400,875)/10.0,1);

}



