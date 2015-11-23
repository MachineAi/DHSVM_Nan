#ifndef CSNOWINTERCEPTION_H
#define CSNOWINTERCEPTION_H

#include "data.h"

class CSnowInterception
{
public:
    CSnowInterception();

    void init(int y, int x, int Dt, float F, float LAI,
		      float MaxInt, float MaxSnowIntCap, float MDRatio,
		      float SnowIntEff, float Ra, float AirDens, float EactAir,
		      float Lv, PIXRAD * LocalRad, float Press, float Tair,
		      float Vpd, float Wind, float *RainFall, float *SnowFall,
		      float *IntRain, float *IntSnow, float *TempIntStorage,
		      float *VaporMassFlux, float *Tcanopy, float *MeltEnergy,
		      float *MomentSq, float *Height, unsigned char Understory,
		      float MS_Rainfall, float LD_FallVelocity);
    void execut();
    ;
private:
    int m_y;
    int m_x;
    int m_Dt;
    float m_F;
    float m_LAI;

    float m_MaxInt;
    float m_MaxSnowIntCap;
    float m_MDRatio;

    float m_SnowIntEff;
    float m_Ra;
    float m_AirDens;
    float m_EactAir;

    float m_Lv;
    PIXRAD * m_LocalRad;
    float m_Press;
    float m_Tair;

    float m_Vpd;
    float m_Wind;
    float * m_RainFall;
    float * m_SnowFall;

    float * m_IntRain;
    float * m_IntSnow;
    float * m_TempIntStorage;

    float * m_VaporMassFlux;
    float * m_Tcanopy;
    float * m_MeltEnergy;

    float * m_MomentSq;
    float * m_Height;
    unsigned char m_Understory;

    float m_MS_Rainfall;
    float m_LD_FallVelocity;

};

#endif //CSNOWINTERCEPTION_H
