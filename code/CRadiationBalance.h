#ifndef CRADIATIONBALANCE_H
#define CRADIATIONBALANCE_H

#include "data.h"

class CRadiationBalance
{
public:
	CRadiationBalance();
	//CRadiationBalance(QLibrary * lib);

//public slots:
    void execute();
    void init(OPTIONSTRUCT *Options, int HeatFluxOption, int CanopyRadAttOption,
		      float SineSolarAltitude, float VIC_Rs, float Rs,
		      float Rsb, float Rsd, float Ld, float Tair,
		      float Tcanopy, float Tsoil, float SoilAlbedo,
		      VEGTABLE *VType, SNOWPIX *LocalSnow, PIXRAD *LocalRad);

private:
    OPTIONSTRUCT *m_Options;
    int m_HeatFluxOption;
    int m_CanopyRadAttOption;
    float m_SineSolarAltitude;
    float m_VIC_Rs;
    float m_Rs;
    float m_Rsb;
    float m_Rsd;
    float m_Ld;
    float m_Tair;
    float m_Tcanopy;
    float m_Tsoil;
    float m_SoilAlbedo;
    VEGTABLE *m_VType;
    SNOWPIX *m_LocalSnow;
    PIXRAD *m_LocalRad;


    void execute(OPTIONSTRUCT *Options, int HeatFluxOption, int CanopyRadAttOption,
		      float SineSolarAltitude, float VIC_Rs, float Rs,
		      float Rsb, float Rsd, float Ld, float Tair,
		      float Tcanopy, float Tsoil, float SoilAlbedo,
		      VEGTABLE *VType, SNOWPIX *LocalSnow, PIXRAD *LocalRad);
    void LongwaveBalance(OPTIONSTRUCT *Options, unsigned char OverStory,
                         float F,float Ld, float Tcanopy, float Tsurf, PIXRAD * LocalRad);
    void ShortwaveBalance(OPTIONSTRUCT *Options, unsigned char OverStory,
                          float F, float Rs, float Rsb, float Rsd, float Tau, float *Albedo, PIXRAD * LocalRad);

};
#endif //CRADIATIONBALANCE_H
