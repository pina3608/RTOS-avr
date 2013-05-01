
#ifndef _BMA180_h
#define _BMA180_h


class BMA180
{
  public:
    typedef enum {F10HZ=0,F20HZ=1,F40HZ, F75HZ,F15HZ0,F300HZ,F600HZ,F1200HZ,HIGHPASS,BANDPASS} FILTER;
    typedef enum {G1=0,G15=1,G2,G3,G4,G8,G16}GSENSITIVITY;
private:
    uint8_t address;
    GSENSITIVITY gSense;
    int x,y,z;
    int temp;
    void init();
  public:

    BMA180(uint8_t a);    
    BMA180();
    
    void setAddress(int val);
    void readAccel();
    
    int getTemp();
    float getGSense();
    float getXValFloat();
    float getYValFloat();
    float getZValFloat();
    void setRegValue(int regAdr, int val, int maskPreserve);
    int getRegValue(int regAdr);
    void setGSensitivty(GSENSITIVITY maxg);
    void setFilter(FILTER f);
    void setISRMode();
    void softReset();
    void setSMPSkip();
    void getIDs(int *id, int *version);
    void enableWrite();
    void disableWrite();
    void disableI2C();
};


#endif //_BMA180_h
