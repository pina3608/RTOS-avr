/*
 * Final project
 *
 * Author: Santiago Pina Ros
 *
 */

#include <avr/pgmspace.h>
#include <SPI.h>


#include "bma180.h"

#define READ                0x80
#define CSB                 8
#define start()             digitalWrite(CSB, LOW)
#define stop()              digitalWrite(CSB, HIGH);


void BMA180::init(){
    pinMode (address, OUTPUT); // set the slaveSelectPin as an output
    SPI.begin();
    SPI.setDataMode(SPI_MODE3);
    softReset();
    enableWrite();
    disableI2C();
    setFilter(F10HZ);
    setGSensitivty(G2);
    disableWrite();
}

BMA180::BMA180(uint8_t a){
    address=a; 
    init();
}

BMA180::BMA180(){
    address=CSB;
    init();
    
}

void BMA180::setAddress(int adr){
  
  address=(uint8_t) adr;
}


void BMA180::readAccel(){
    
    start();
    SPI.transfer(0x02|READ);
    
    x = SPI.transfer(0xFF);
    x |= SPI.transfer(0xFF) << 8;
    x >>= 2;
    
    y = SPI.transfer(0xFF);
    y |= SPI.transfer(0xFF) << 8;
    y >>= 2;
    
    z = SPI.transfer(0xFF);
    z |= SPI.transfer(0xFF) << 8;
    z >>= 2;
    
    temp = SPI.transfer(0xFF);
    
    stop();
}

int BMA180::getTemp(){
  return temp;
}

float BMA180::getGSense(){
    switch(gSense)
    {
        case G1: return 1.0;
        case G15: return 1.5;
        case G2: return 2.0;
        case G3: return 3.0;
        case G4: return 4.0;
        case G8: return 8.0;
        case G16: return 16.0;
    }
}

float BMA180::getXValFloat(){
    
    return (float)x/8191.0*getGSense();
}

float BMA180::getYValFloat(){
    
    return (float)y/8191.0*getGSense();
}

float BMA180::getZValFloat(){
    
    return (float)z/8191.0*getGSense();
}

int BMA180::getRegValue(int regAdr){
    
    int val;
    start();
    SPI.transfer(regAdr|READ);
    val = SPI.transfer(0xFF);
    return val;
}

void BMA180::setRegValue(int regAdr, int val, int maskPreserve){
    
    int preserve = getRegValue(regAdr);
    int orgval = preserve & maskPreserve;
    //val = val & (~maskPreserve);
    
    delay(1);
    
    start();
    SPI.transfer(regAdr);
    SPI.transfer(orgval|val);
    stop();
    
    delay(20);
}

void BMA180::setGSensitivty(GSENSITIVITY maxg){ //1, 1.5 2 3 4 8 16
    
    setRegValue(0x35,maxg<<1,0xF1);
    gSense = maxg;
}

void BMA180::setFilter(FILTER f){

    setRegValue(0x20,f<<4,0x0F);
}

void BMA180::setISRMode(){ // you must provide a ISR function on the pin selected

    setRegValue(0x21,2,0xFD);
}

void BMA180::softReset(){ // all values will be default

    setRegValue(0x10,0xB6,0);
    delay(100);
}

void BMA180::setSMPSkip(){
    
    setRegValue(0x35, 1, 0xFE);
}

void BMA180::getIDs(int *id, int *version){
    
    start();
    SPI.transfer(0x0|READ);
    
    *id = SPI.transfer(0xFF);
    *version = SPI.transfer(0xFF);
    stop();
    
}


void BMA180::enableWrite(){
  
    setRegValue(0x0D,0x10,~0x10);
    delay(10);
}


void BMA180::disableWrite(){
  
    setRegValue(0x0D,0x0,~0x10);
    delay(10);
}
   
void BMA180::disableI2C(){
    
    setRegValue(0x27,0x01,~0x01);
}

