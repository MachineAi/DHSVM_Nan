#include "CSnowMelt.h"


#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "brent.h"
#include "constants.h"
#include "settings.h"
#include "massenergy.h"
#include "functions.h"
#include "snow.h"

CSnowMelt::CSnowMelt()
{


}

void CSnowMelt::init(int y, int x, int Dt, float Z, float Displacement, float Z0,
	       float BaseRa, float AirDens, float EactAir, float Lv,
	       float ShortRad, float LongRadIn, float Press, float RainFall,
	       float SnowFall, float Tair, float Vpd, float Wind,
	       float *PackWater, float *SurfWater, float *Swq,
	       float *VaporMassFlux, float *TPack, float *TSurf,
	       float *MeltEnergy, float * sm)
{
  float DeltaPackCC;		/* Change in cold content of the pack */
  float DeltaPackSwq;		/* Change in snow water equivalent of the
				   pack (m) */
  float Ice;			/* Ice content of snow pack (m) */
  float InitialSwq;		/* Initial snow water equivalent (m) */
  float MassBalanceError;	/* Mass balance error (m) */
  float MaxLiquidWater;		/* Maximum liquid water content of pack (m) */
  float OldTSurf;		/* Old snow surface temperature (C) */
  float Outflow;		/* Amount water flowing out of the snow pack
				   during the time interval (m) */
  float PackCC;			/* Cold content of snow pack (J) */
  float PackSwq;		/* Snow pack snow water equivalent (m) */
  float Qnet;			/* Net energy exchange at the surface (W/m2) */
  float RefreezeEnergy;		/* refreeze energy (W/m2) */
  float RefrozenWater;		/* Amount of refrozen water (m) */
  float SnowFallCC;		/* Cold content of new snowfall (J) */
  float SnowMelt;		/* Amount of snow melt during time interval
				   (m water equivalent) */
  float SurfaceCC;		/* Cold content of snow pack (J) */
  float SurfaceSwq;		/* Surface layer snow water equivalent (m) */

  InitialSwq = *Swq;
  OldTSurf = *TSurf;

  /* Initialize snowpack variables */

  Ice = *Swq - *PackWater - *SurfWater;

  /* Reconstruct snow pack */
  if (Ice > MAX_SURFACE_SWE)
    SurfaceSwq = MAX_SURFACE_SWE;
  else
    SurfaceSwq = Ice;
  PackSwq = Ice - SurfaceSwq;

  /* Calculate cold contents */
  SurfaceCC = CH_ICE * SurfaceSwq * *TSurf;
  PackCC = CH_ICE * PackSwq * *TPack;
  if (Tair > 0.0)
    SnowFallCC = 0.0;
  else
    SnowFallCC = CH_ICE * SnowFall * Tair;

  /* Distribute fresh snowfall */
  if (SnowFall > (MAX_SURFACE_SWE - SurfaceSwq)) {
    DeltaPackSwq = SurfaceSwq + SnowFall - MAX_SURFACE_SWE;
    if (DeltaPackSwq > SurfaceSwq)
      DeltaPackCC = SurfaceCC + (SnowFall - MAX_SURFACE_SWE) / SnowFall *
	SnowFallCC;
    else
      DeltaPackCC = DeltaPackSwq / SurfaceSwq * SurfaceCC;
    SurfaceSwq = MAX_SURFACE_SWE;
    SurfaceCC += SnowFallCC - DeltaPackCC;
    PackSwq += DeltaPackSwq;
    PackCC += DeltaPackCC;
  }
  else {
    SurfaceSwq += SnowFall;
    SurfaceCC += SnowFallCC;
  }
  if (SurfaceSwq > 0.0)
    *TSurf = SurfaceCC / (CH_ICE * SurfaceSwq);
  else
    *TSurf = 0.0;
  if (PackSwq > 0.0)
    *TPack = PackCC / (CH_ICE * PackSwq);
  else
    *TPack = 0.0;

  /* Adjust ice and *SurfWater */
  Ice += SnowFall;
  *SurfWater += RainFall;

  /* Calculate the surface energy balance for snow_temp = 0.0 */

  Qnet = CalcSnowPackEnergyBalance((float) 0.0, Dt, BaseRa, Z, Displacement,
				   Z0, Wind, ShortRad, LongRadIn, AirDens,
				   Lv, Tair, Press, Vpd, EactAir, RainFall,
				   SurfaceSwq, *SurfWater, OldTSurf,
				   &RefreezeEnergy, VaporMassFlux);

  /* If Qnet == 0.0, then set the surface temperature to 0.0 */
  if (fequal(Qnet, 0.0)) {
    *TSurf = 0.0;
    if (RefreezeEnergy >= 0.0) {
      RefrozenWater = RefreezeEnergy / (LF * WATER_DENSITY) * Dt;
      if (RefrozenWater > *SurfWater) {
	RefrozenWater = *SurfWater;
	RefreezeEnergy = (RefrozenWater * LF * WATER_DENSITY) / Dt;
      }
      *MeltEnergy += RefreezeEnergy;
      SurfaceSwq += RefrozenWater;
      Ice += RefrozenWater;
      *SurfWater -= RefrozenWater;
      assert(*SurfWater >= 0.0);
      SnowMelt = 0.0;

    }
    else {

      /* Calculate snow melt */
      SnowMelt = fabs(RefreezeEnergy) / (LF * WATER_DENSITY) * Dt;
      *MeltEnergy += RefreezeEnergy;
    }

    /* Convert vapor mass flux to a depth per timestep and adjust *SurfWater */
    *VaporMassFlux *= Dt;

    if (*SurfWater < -(*VaporMassFlux)) {
      *VaporMassFlux = -(*SurfWater);
      *SurfWater = 0.0;
    }
    else
      *SurfWater += *VaporMassFlux;

    /* If SnowMelt < Ice, there was incomplete melting of the pack */

    if (SnowMelt < Ice) {
      if (SnowMelt <= PackSwq) {
	*SurfWater += SnowMelt;
	PackSwq -= SnowMelt;
	Ice -= SnowMelt;
      }
      else {
	*SurfWater += SnowMelt + *PackWater;
	*PackWater = 0.0;
	PackSwq = 0.0;
	Ice -= SnowMelt;
	SurfaceSwq = Ice;
      }
    }

    /* Else, SnowMelt > Ice and there was complete melting of the pack */
    else {
      SnowMelt = Ice;
      *SurfWater += Ice;
      SurfaceSwq = 0.0;
      *TSurf = 0.0;
      PackSwq = 0.0;
      *TPack = 0.0;
      Ice = 0.0;
    }
  }

  /* Else, SnowPackEnergyBalance(T=0.0) <= 0.0 */
  else {
    /* Calculate surface layer temperature using "Brent method" */

//
//
//
//
//    *TSurf = RootBrent(y, x, (float) (*TSurf - DELTAT), (float) 0.0,
//		       SnowPackEnergyBalance, Dt, BaseRa, Z, Displacement,
//		       Z0, Wind, ShortRad, LongRadIn, AirDens, Lv, Tair,
//		       Press, Vpd, EactAir, RainFall, SurfaceSwq, *SurfWater,
//		       OldTSurf, &RefreezeEnergy, VaporMassFlux);
//
//
//
//
//
//
    /* since we iterated, the surface layer is below freezing and no snowmelt
     */

    SnowMelt = 0.0;

    /* Since updated snow_temp < 0.0, all of the liquid water in the surface
       layer has been frozen */

    SurfaceSwq += *SurfWater;
    Ice += *SurfWater;
    *SurfWater = 0.0;
    *MeltEnergy += (*SurfWater * LF * WATER_DENSITY) / Dt;

    /* Convert mass flux to a depth per timestep and adjust SurfaceSwq */

    *VaporMassFlux *= Dt;

    if (SurfaceSwq < -(*VaporMassFlux)) {
      *VaporMassFlux = -SurfaceSwq;
      SurfaceSwq = 0.0;
      Ice = PackSwq;
    }
    else {
      SurfaceSwq += *VaporMassFlux;
      Ice += *VaporMassFlux;
    }
  }

  /* Done with iteration etc, now Update the liquid water content of the
     surface layer */

  MaxLiquidWater = LIQUID_WATER_CAPACITY * SurfaceSwq;
  if (*SurfWater > MaxLiquidWater) {
    Outflow = *SurfWater - MaxLiquidWater;
    *SurfWater = MaxLiquidWater;
  }
  else
    Outflow = 0.0;

  /* Refreeze liquid water in the pack.
     variable 'RefreezeEnergy' is the heat released to the snow pack
     if all liquid water were refrozen.
     if RefreezeEnergy < PackCC then all water IS refrozen
     PackCC always <=0.0

     WORK IN PROGRESS: This energy is NOT added to MeltEnergy, since this does
     not involve energy transported to the pixel.  Instead heat from the snow
     pack is used to refreeze water */

  *PackWater += Outflow;	/* add surface layer outflow to pack liquid water */
  RefreezeEnergy = *PackWater * LF * WATER_DENSITY;

  /* calculate energy released to freeze */

  if (PackCC < -RefreezeEnergy) {	/* cold content not fully depleted */
    PackSwq += *PackWater;	/* refreeze all water and update */
    Ice += *PackWater;
    *PackWater = 0.0;
    if (PackSwq > 0.0)
      *TPack = (PackCC + RefreezeEnergy) / (CH_ICE * PackSwq);
    else
      *TPack = 0.0;
  }
  else {
    /* cold content has been either exactly satisfied or exceeded. If
       PackCC = refreeze then pack is ripe and all pack water is
       refrozen, else if energy released in refreezing exceeds PackCC
       then exactly the right amount of water is refrozen to satify PackCC.
       The refrozen water is added to PackSwq and Ice */

    *TPack = 0.0;
    DeltaPackSwq = -PackCC / (LF * WATER_DENSITY);
    *PackWater -= DeltaPackSwq;
    PackSwq += DeltaPackSwq;
    Ice += DeltaPackSwq;
  }

  /* Update the liquid water content of the pack */

  MaxLiquidWater = LIQUID_WATER_CAPACITY * PackSwq;
  if (*PackWater > MaxLiquidWater) {
    Outflow = *PackWater - MaxLiquidWater;
    *PackWater = MaxLiquidWater;
  }
  else
    Outflow = 0.0;

  /* Update snow properties */

  Ice = PackSwq + SurfaceSwq;

  if (Ice > MAX_SURFACE_SWE) {
    SurfaceCC = CH_ICE * *TSurf * SurfaceSwq;
    PackCC = CH_ICE * *TPack * PackSwq;
    if (SurfaceSwq > MAX_SURFACE_SWE) {
      PackCC += SurfaceCC * (SurfaceSwq - MAX_SURFACE_SWE) / SurfaceSwq;
      SurfaceCC -= SurfaceCC * (SurfaceSwq - MAX_SURFACE_SWE) / SurfaceSwq;
      PackSwq += SurfaceSwq - MAX_SURFACE_SWE;
      SurfaceSwq -= SurfaceSwq - MAX_SURFACE_SWE;
    }
    else if (SurfaceSwq < MAX_SURFACE_SWE) {
      PackCC -= PackCC * (MAX_SURFACE_SWE - SurfaceSwq) / PackSwq;
      SurfaceCC += PackCC * (MAX_SURFACE_SWE - SurfaceSwq) / PackSwq;
      PackSwq -= MAX_SURFACE_SWE - SurfaceSwq;
      SurfaceSwq += MAX_SURFACE_SWE - SurfaceSwq;
    }
    *TPack = PackCC / (CH_ICE * PackSwq);
    *TSurf = SurfaceCC / (CH_ICE * SurfaceSwq);
  }
  else {
    PackSwq = 0.0;
    PackCC = 0.0;
    *TPack = 0.0;
  }

  *Swq = Ice + *PackWater + *SurfWater;

  if (fequal(*Swq, 0.0)) {
    *TSurf = 0.0;
    *TPack = 0.0;
  }

  /* Mass balance test */

  MassBalanceError = (InitialSwq - *Swq) + (RainFall + SnowFall) - Outflow +
    *VaporMassFlux;

  //return (Outflow);
  *sm = Outflow;
}

void CSnowMelt::execute()
{

}

/*****************************************************************************
  Function name: SnowPackEnergyBalance()

  Purpose      : Calculate the surface energy balance for the snow pack

  Required     :
    float TSurf           - new estimate of effective surface temperature
    va_list ap            - Argument list initialized by va_start().  For
                            elements of list and order, see beginning of
                            routine

  Returns      :
    float RestTerm        - Rest term in the energy balance

  Modifies     :
    float *RefreezeEnergy - Refreeze energy (W/m2)
    float *VaporMassFlux  - Mass flux of water vapor to or from the
                            intercepted snow

  Comments     :
    Reference:  Bras, R. A., Hydrology, an introduction to hydrologic
                science, Addisson Wesley, Inc., Reading, etc., 1990.
*****************************************************************************/
float CSnowMelt::SnowPackEnergyBalance(float TSurf, va_list ap)
{
  /* start of list of arguments in variable argument list */

  int Dt;			/* Model time step (hours) */
  float Ra;			/* Aerodynamic resistance (s/m) */
  float Z;			/* Reference height (m) */
  float Displacement;		/* Displacement height (m) */
  float Z0;			/* Roughness length (m) */
  float Wind;			/* Wind speed (m/s) */
  float ShortRad;		/* Net incident shortwave radiation (W/m2) */
  float LongRadIn;		/* Incoming longwave radiation (W/m2) */
  float AirDens;		/* Density of air (kg/m3) */
  float Lv;			/* Latent heat of vaporization (J/kg3) */
  float Tair;			/* Air temperature (C) */
  float Press;			/* Air pressure (Pa) */
  float Vpd;			/* Vapor pressure deficit (Pa) */
  float EactAir;		/* Actual vapor pressure of air (Pa) */
  float Rain;			/* Rain fall (m/timestep) */
  float SweSurfaceLayer;	/* Snow water equivalent in surface layer (m)
				 */
  float SurfaceLiquidWater;	/* Liquid water in the surface layer (m) */
  float OldTSurf;		/* Surface temperature during previous time
				   step */
  float *RefreezeEnergy;	/* Refreeze energy (W/m2) */
  float *VaporMassFlux;		/* Mass flux of water vapor to or from the
				   intercepted snow */

  /* end of list of arguments in variable argument list */

  float AdvectedEnergy;		/* Energy advected by precipitation (W/m2) */
  float DeltaColdContent;	/* Change in cold content (W/m2) */
  float EsSnow;			/* saturated vapor pressure in the snow pack
				   (Pa)  */
  float LatentHeat;		/* Latent heat exchange at surface (W/m2) */
  float LongRadOut;		/* long wave radiation emitted by surface
				   (W/m2) */
  float Ls;			/* Latent heat of sublimation (J/kg) */
  float NetRad;			/* Net radiation exchange at surface (W/m2) */
  float RestTerm;		/* Rest term in surface energy balance
				   (W/m2) */
  float SensibleHeat;		/* Sensible heat exchange at surface (W/m2) */
  float TMean;			/* Mean temperature during interval (C) */
  double Tmp;			/* temporary variable */

  /* Assign the elements of the array to the appropriate variables.  The list
     is traversed as if the elements are doubles, because:

     In the variable-length part of variable-length argument lists, the old
     ``default argument promotions'' apply: arguments of type float are
     always promoted (widened) to type double, and types char and short int
     are promoted to int. Therefore, it is never correct to invoke
     va_arg(argp, float); instead you should always use va_arg(argp,
     double).

     (quoted from the comp.lang.c FAQ list)
   */
  Dt = va_arg(ap, int);
  Ra = (float) va_arg(ap, double);
  Z = (float) va_arg(ap, double);
  Displacement = (float) va_arg(ap, double);
  Z0 = (float) va_arg(ap, double);
  Wind = (float) va_arg(ap, double);
  ShortRad = (float) va_arg(ap, double);
  LongRadIn = (float) va_arg(ap, double);
  AirDens = (float) va_arg(ap, double);
  Lv = (float) va_arg(ap, double);
  Tair = (float) va_arg(ap, double);
  Press = (float) va_arg(ap, double);
  Vpd = (float) va_arg(ap, double);
  EactAir = (float) va_arg(ap, double);
  Rain = (float) va_arg(ap, double);
  SweSurfaceLayer = (float) va_arg(ap, double);
  SurfaceLiquidWater = (float) va_arg(ap, double);
  OldTSurf = (float) va_arg(ap, double);
  RefreezeEnergy = (float *) va_arg(ap, double *);
  VaporMassFlux = (float *) va_arg(ap, double *);

  /* Calculate active temp for energy balance as average of old and new  */

  TMean = 0.5 * (OldTSurf + TSurf);

  /* Correct aerodynamic conductance for stable conditions
     Note: If air temp >> snow temp then aero_cond -> 0 (i.e. very stable)
     velocity (vel_2m) is expected to be in m/sec */

  /* Apply the stability correction to the aerodynamic resistance
     NOTE: In the old code 2m was passed instead of Z-Displacement.  I (bart)
     think that it is more correct to calculate ALL fluxes at the same
     reference level */

  if (Wind > 0.0)
    Ra /= StabilityCorrection(2.0f, 0.f, TMean, Tair, Wind, Z0);
  else
    Ra = DHSVM_HUGE;

  /* Calculate longwave exchange and net radiation */

  Tmp = TMean + 273.15;
  LongRadOut = STEFAN * (Tmp * Tmp * Tmp * Tmp);
  NetRad = ShortRad + LongRadIn - LongRadOut;

  /* Calculate the sensible heat flux */

  SensibleHeat = AirDens * CP * (Tair - TMean) / Ra;

  /* Calculate the mass flux of ice to or from the surface layer */

  /* Calculate the saturated vapor pressure in the snow pack,
     (Equation 3.32, Bras 1990) */

  EsSnow = SatVaporPressure(TMean);

  *VaporMassFlux = AirDens * (EPS / Press) * (EactAir - EsSnow) / Ra;
  *VaporMassFlux /= WATER_DENSITY;
  if (fequal(Vpd, 0.0) && *VaporMassFlux < 0.0)
    *VaporMassFlux = 0.0;

  /* Calculate latent heat flux */

  if (TMean >= 0.0) {
    /* Melt conditions: use latent heat of vaporization */
    LatentHeat = Lv * *VaporMassFlux * WATER_DENSITY;
  }
  else {
    /* Accumulation: use latent heat of sublimation (Eq. 3.19, Bras 1990 */
    Ls = (677. - 0.07 * TMean) * JOULESPCAL * GRAMSPKG;
    LatentHeat = Ls * *VaporMassFlux * WATER_DENSITY;
  }

  /* Calculate advected heat flux from rain
     WORK IN PROGRESS:  Should the following read (Tair - Tsurf) ?? */

  AdvectedEnergy = (CH_WATER * Tair * Rain) / Dt;

  /* Calculate change in cold content */

  DeltaColdContent = CH_ICE * SweSurfaceLayer * (TSurf - OldTSurf) / Dt;

  /* Calculate net energy exchange at the snow surface */

  RestTerm = NetRad + SensibleHeat + LatentHeat + AdvectedEnergy -
    DeltaColdContent;

  *RefreezeEnergy = (SurfaceLiquidWater * LF * WATER_DENSITY) / Dt;

  if (fequal(TSurf, 0.0) && RestTerm > -(*RefreezeEnergy)) {
    *RefreezeEnergy = -RestTerm;	/* available energy input over cold content
					   used to melt, i.e. Qrf is negative value
					   (energy out of pack) */
    RestTerm = 0.0;
  }
  else {
    RestTerm += *RefreezeEnergy;	/* add this positive value to the pack */
  }

  return RestTerm;
}
