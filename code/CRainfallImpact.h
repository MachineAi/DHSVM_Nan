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
  static const float alpha_m[4];
  static const float beta_m[4];
  float MS_Rainfall;
  float Dm;
  float Rainfall;
  int Dt;
};

#endif // CRAINFALLIMPACT_H
