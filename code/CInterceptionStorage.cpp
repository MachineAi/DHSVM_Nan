#include "cinterceptionstorage.h"
#include "math.h"
#include "data.h"
#include "constants.h"


CInterceptionStorage::CInterceptionStorage()
{

}

void CInterceptionStorage::init(int m_NMax, int m_NAct, float *m_MaxInt, float *m_Fract,
			 float *m_Int, float *m_Precip, float *m_MomentSq, float *m_Height,
			 unsigned char m_Understory, float m_Dt, float m_MS_Rainfall,
			 float m_LD_FallVelocity)
{
    NMax = m_NMax;
    NAct = m_NAct;
    MaxInt = m_MaxInt;
    Fract = m_Fract;
    Int = m_Int;
    Precip = m_Precip;
    MomentSq = m_MomentSq;
    Height = m_Height;
    Understory = m_Understory;
    Dt = m_Dt;
    MS_Rainfall = m_MS_Rainfall;
    LD_FallVelocity = m_LD_FallVelocity;
}
void CInterceptionStorage::execute()
{

  float Available;		/* Available storage */
  float Intercepted;		/* Amount of water intercepted during this
				   timestep */
  int i;			/* counter */
  float OriginalPrecip;

  OriginalPrecip = *Precip;


  /* The precipitation is multiplied by the fractional coverage, since if the
     vegetation covers only 10% of the grid cell, only 10% can be intercepted as a
     maximum */
  for (i = 0; i < NAct; i++) {
    Available = MaxInt[i] - Int[i];
    if (Available > *Precip * Fract[i])
      Intercepted = (*Precip) * Fract[i];
    else
      Intercepted = Available;
    *Precip -= Intercepted;
    Int[i] += Intercepted;
  }

  /* Find momentum squared of rainfall for use by the sediment model. */
  if(Understory)
    /* Since the understory is assumed to cover the entire grid cell, all
       momentum is associated with leaf drip, eq. 2, Wicks and Bathurst (1996) */
    *MomentSq = pow(LD_FallVelocity * WATER_DENSITY, 2) * PI/6 *
      pow(LEAF_DRIP_DIA, 3) * (*Precip)/Dt;
  else
    /* If no understory, part of the rainfall reaches the ground as direct throughfall. */
    *MomentSq = (pow(LD_FallVelocity * WATER_DENSITY, 2) * PI/6 *
		 pow(LEAF_DRIP_DIA, 3) * (*Precip)/Dt) + (1-Fract[0]) *
      MS_Rainfall;

}
void CInterceptionStorage::query()
{

}
