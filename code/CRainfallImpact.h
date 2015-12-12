#ifndef CRAINFALLIMPACT_H
#define CRAINFALLIMPACT_H

#include "data.h"

class CRainfallImpact
{
public:
    CRainfallImpact();
    void init( float m_Rainfall, int m_Dt );
    void execute();
    void query( float * m_MS_Rainfall, float * m_Dm );
private:
  static const float alpha[4];
  static const float beta[4];
  float MS_Rainfall;
  float Dm;
  float Rainfall;
  int Dt;
};
  const float CRainfallImpact::alpha[4]={2.69e-8,3.75e-8,6.12e-8,11.75e-8}; /* empirical coefficient
				for rainfall momentum after Wicks and Bathurst (1996) */
  const float CRainfallImpact::beta[4]={1.6896,1.5545,1.4242,1.2821};  /* empirical coefficient
				for rainfall momentum after Wicks and Bathurst (1996) */

#endif // CRAINFALLIMPACT_H
