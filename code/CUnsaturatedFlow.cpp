#include "CUnsaturatedFlow.h"


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "settings.h"
#include "functions.h"
#include "soilmoisture.h"

CUnsaturatedFlow::CUnsaturatedFlow()
{

}
void CUnsaturatedFlow::init(int Dt, float DX, float DY, float Infiltration,
		     float RoadbedInfiltration, float SatFlow, int NSoilLayers,
		     float TotalDepth, float Area, float *RootDepth, float *Ks,
		     float *PoreDist, float *Porosity, float *FCap,
		     float *Perc, float *PercArea, float *Adjust,
		     int CutBankZone, float BankHeight, float *TableDepth,
		     float *Runoff, float *Moist, int RoadRouteOption,
		     int InfiltOption, float *RoadIExcess)
{
  float DeepDrainage;		/* amount of drainage from the lowest root
				   zone to the layer below it (m) */
  float DeepLayerDepth;		/* depth of the layer below the deepest
				   root layer */
  float Drainage;		/* amount of water drained from each soil
				   layer during the current timestep */
  float Exponent;		/* Brooks-Corey exponent */
  float FieldCapacity;		/* amount of water in soil at field capacity
				   (m) */
  float MaxSoilWater;		/* maximum allowable amount of soil moiture
				   in each layer (m) */
  float SoilWater;		/* amount of water in each soil layer (m) */
  int i;			/* counter */

  DeepLayerDepth = TotalDepth;
  for (i = 0; i < NSoilLayers; i++)
    DeepLayerDepth -= RootDepth[i];

  /* first take care of infiltration through the roadbed/channel, then through the
     remaining surface */
  if (*TableDepth <= BankHeight) { /* watertable above road/channel surface */
    if(RoadRouteOption)
      *RoadIExcess += RoadbedInfiltration;
    else
      *Runoff += RoadbedInfiltration;
  }

  else {
    if (CutBankZone == NSoilLayers) {
      Moist[NSoilLayers] += RoadbedInfiltration /
	(DeepLayerDepth * Adjust[NSoilLayers]);
    }
    else if (CutBankZone >= 0){
      Moist[CutBankZone] += RoadbedInfiltration /
	(RootDepth[CutBankZone] * Adjust[CutBankZone]);
    }
  }
  if (*TableDepth <= 0) { /* watertable above surface */
    *Runoff += Infiltration;
    if(InfiltOption == DYNAMIC) Infiltration = 0.;
  }
  else {
    Moist[0] += Infiltration / (RootDepth[0] * Adjust[0]);
  }


  for (i = 0; i < NSoilLayers; i++) {

    /* No movement if soil moisture is below field capacity */
    if (Moist[i] > FCap[i]) {
      Exponent = 2.0 / PoreDist[i] + 3.0;

      if (Moist[i] > Porosity[i])
	/* this can happen because the moisture content can exceed the
	   porosity the way the algorithm is implemented */
	Drainage = Ks[i];
      else
	Drainage =
	  Ks[i] * pow((double) (Moist[i] / Porosity[i]), (double) Exponent);

      Drainage *= Dt;
      Perc[i] = 0.5 * (Perc[i] + Drainage) * PercArea[i];

      MaxSoilWater = RootDepth[i] * Porosity[i] * Adjust[i];
      SoilWater = RootDepth[i] * Moist[i] * Adjust[i];
      FieldCapacity = RootDepth[i] * FCap[i] * Adjust[i];

      /* No unsaturated flow if the moisture content drops below field
	 capacity */

      if ((SoilWater - Perc[i]) < FieldCapacity)
	Perc[i] = SoilWater - FieldCapacity;

      /* WORK IN PROGRESS */
      /* If the moisture content is greater than the porosity add the
	 additional soil moisture to the percolation */

      SoilWater -= Perc[i];
      if (SoilWater > MaxSoilWater)
	Perc[i] += SoilWater - MaxSoilWater;

      /* Adjust the moisture content in the current layer, and the layer
	 immediately below it */

      Moist[i] -= Perc[i] / (RootDepth[i] * Adjust[i]);
      if (i < (NSoilLayers - 1))
	Moist[i + 1] += Perc[i] / (RootDepth[i + 1] * Adjust[i + 1]);
    }
    else
      Perc[i] = 0.0;

    /* convert back to straight 1-d flux */
    Perc[i] /= PercArea[i];
  }

  DeepDrainage = (Perc[NSoilLayers - 1] * PercArea[NSoilLayers - 1]) + SatFlow;

  Moist[NSoilLayers] += DeepDrainage / (DeepLayerDepth * Adjust[NSoilLayers]);

  /* added 8/16/2000 by Pascal Storck */
  /* this following statement will force a trap of out of bounds
     soil moisture in the lowest layer in the mass balance calculation */
  if (Moist[NSoilLayers] < FCap[NSoilLayers - 1]) {
    /*    Moist[NSoilLayers] = FCap[NSoilLayers - 1]; */
   /*  printf("Warning: Deep layer soil moisture is less than field capacity.\n"); */

  /*   if (Moist[NSoilLayers] < 0. ) */
/*       printf("Warning: Deep layer soil moisture is negative.\n"); */
  }

  /* Calculate the depth of the water table based on the soil moisture
     profile and adjust the soil moisture profile, to assure that the soil
     moisture is never more than the maximum allowed soil moisture amount,
     i.e. the porosity.  A negative water table depth means that the water is
     ponding on the surface.  This amount of water becomes surface Runoff */

  *TableDepth = WaterTableDepth(NSoilLayers, TotalDepth, RootDepth, Porosity,
				FCap, Adjust, Moist);

  if (*TableDepth < 0.0) {
    *Runoff += -(*TableDepth);

    if(InfiltOption == DYNAMIC){
      if (Infiltration > -(*TableDepth))
	Infiltration += *TableDepth;
      else
	Infiltration = 0.;
    }

    *TableDepth = 0.0;
  }
}


void CUnsaturatedFlow::execute()
{

}
