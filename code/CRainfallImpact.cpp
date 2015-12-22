#include "CRainfallImpact.h"
#include "constants.h"
#include "math.h"



  const float CRainfallImpact::alpha_m[4] = {2.69e-8,3.75e-8,6.12e-8,11.75e-8}; /* empirical coefficient
				for rainfall momentum after Wicks and Bathurst (1996) */
  const float CRainfallImpact::beta_m[4] = {1.6896,1.5545,1.4242,1.2821};  /* empirical coefficient
				for rainfall momentum after Wicks and Bathurst (1996) */

CRainfallImpact::CRainfallImpact()
{

}
void CRainfallImpact::init(float m_Rainfall, int m_Dt )
{
    Rainfall = m_Rainfall;
    Dt = m_Dt;
}
void CRainfallImpact::execute()
{
  /* RainFall impact */
  /* 3600 is conversion factor (number of seconds per hour) */
  /* related files: independant
  */
  float RainfallIntensity = 0;
  int MS_Index = 0;
  if (Rainfall > 0.) {
    RainfallIntensity = Rainfall * (1./MMTOM) * (3600./Dt);

    /* Momentum is later weighted with the overstory/understory fraction */
    if (RainfallIntensity < 10.)
      MS_Index = 0;
    else if (RainfallIntensity >= 10. && RainfallIntensity < 100.)
      MS_Index = floor((RainfallIntensity + 49)/50);
    else
      MS_Index = 3;

    /* Eq. 1, Wicks and Bathurst (1996) */
    MS_Rainfall = alpha_m[MS_Index] * pow(RainfallIntensity, beta_m[MS_Index]);

    /* Calculating mediam raindrop diameter after Laws and Parsons (1943) */
    Dm =  0.00124 * pow((double)RainfallIntensity, 0.182);
  }
  else {
    MS_Rainfall = 0;
    Dm = LEAF_DRIP_DIA;
  }
}
void CRainfallImpact::query(float * m_MS_Rainfall, float * m_Dm )
{
    *m_MS_Rainfall = MS_Rainfall;
    *m_Dm = Dm;
}
