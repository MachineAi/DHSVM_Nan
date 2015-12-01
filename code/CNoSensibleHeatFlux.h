#ifndef CNOSENSIBLEHEATFLUX_H
#define CNOSENSIBLEHEATFLUX_H

#include "data.h"

class CNoSensibleHeatFlux
{

public:
    CNoSensibleHeatFlux();
    void init(int Dt, PIXMET * LocalMet, float ETot, SOILPIX * LocalSoil);
    void execute();
private:
    ;;
};

#endif // CNOSENSIBLEHEATFLUX_H
