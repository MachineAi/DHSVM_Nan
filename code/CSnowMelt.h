#ifndef CSNOWMELT_H
#define CSNOWMELT_H

#include <stdarg.h>
class CSnowMelt
{
public:
    CSnowMelt();
    void init(int y, int x, int Dt, float Z, float Displacement, float Z0,
	       float BaseRa, float AirDens, float EactAir, float Lv,
	       float ShortRad, float LongRadIn, float Press, float RainFall,
	       float SnowFall, float Tair, float Vpd, float Wind,
	       float *PackWater, float *SurfWater, float *Swq,
	       float *VaporMassFlux, float *TPack, float *TSurf,
	       float *MeltEnergy, float * sm);
    void execute();
private:
    float SnowPackEnergyBalance(float TSurf, va_list ap);

/*****************************************************************************
  Function name: CalcSnowPackEnergyBalance()

  Purpose      : Dummy function to make a direct call to
                 SnowEnergyBalance() possible.

  Required     :
    float TSurf - SnowPack surface temperature (C)
    other arguments required by SnowPackEnergyBalance()

  Returns      :
    float Qnet - Net energy exchange at the SnowPack snow surface (W/m^2)

  Modifies     : none

  Comments     : function is local to this module
*****************************************************************************/
    float CalcSnowPackEnergyBalance(float Tsurf, ...)
    {
      va_list ap;			/* Used in traversing variable argument list
                     */
      float Qnet;			/* Net energy exchange at the SnowPack snow
                       surface (W/m^2) */

      va_start(ap, Tsurf);
      Qnet = SnowPackEnergyBalance(Tsurf, ap);
      va_end(ap);

      return Qnet;
    }
};


#endif //CSNOWMELT_H
