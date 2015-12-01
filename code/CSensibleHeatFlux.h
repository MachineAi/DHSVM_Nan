#ifndef CSENSIBLEHEATFLUX_H
#define CSENSIBLEHEATFLUX_H


#include "data.h"

class CSensibleHeatFlux
{
public:
    CSensibleHeatFlux();
    void init(int y, int x, int Dt, float Ra, float ZRef,
		      float Displacement, float Z0, PIXMET * LocalMet,
		      float NetShort, float LongIn, float ETot,
		      int NSoilLayers, float *SoilDepth, SOILTABLE * SoilType,
		      float MeltEnergy, SOILPIX * LocalSoil);
    void execute();
private:
    ;

};


#endif // CSENSIBLEHEATFLUX_H
