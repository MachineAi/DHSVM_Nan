#include "CNoSensibleHeatFlux.h"
#include "constants.h"
CNoSensibleHeatFlux::CNoSensibleHeatFlux()
{
;
}
void CNoSensibleHeatFlux::init(int Dt, PIXMET * LocalMet, float ETot, SOILPIX * LocalSoil)
{

  LocalSoil->TSurf = 0.0;

  LocalSoil->Ra = 0.0;

  LocalSoil->Qnet = 0.0;

  LocalSoil->Qs = 0.0;

  LocalSoil->Qe = -(LocalMet->Lv * ETot) / Dt * WATER_DENSITY;

  LocalSoil->Qg = 0.0;

  LocalSoil->Qst = 0.0;

  LocalSoil->Qrest = 0.0;
}

void CNoSensibleHeatFlux::execute()
{
;
}
