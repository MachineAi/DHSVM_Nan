#ifndef CINTERCEPTIONSTORAGE_H
#define CINTERCEPTIONSTORAGE_H

class CInterceptionStorage
{
public:
    CInterceptionStorage();
    void init(int NMax, int NAct, float *MaxInt, float *Fract,
			 float *Int, float *Precip, float *MomentSq, float *Height,
			 unsigned char Understory, float Dt, float MS_Rainfall,
			 float LD_FallVelocity);
    void execute();
    void query();
private:
    int NMax;
    int NAct;
    float *MaxInt;
    float *Fract;
    float *Int;
    float *Precip;
    float *MomentSq;
    float *Height;
    unsigned char Understory;
    float Dt;
    float MS_Rainfall;
    float LD_FallVelocity;
};

#endif // CINTERCEPTIONSTORAGE_H
