#ifndef CSOILEVAPORATION_H
#define CSOILEVAPORATION_H


#include <stdio.h>
#include <stdlib.h>
#include "settings.h"
#include "DHSVMerror.h"
#include "massenergy.h"
#include "constants.h"
#include "settings.h"

class CSoilEvaporation
{
public:
    CSoilEvaporation();
    void init(int Dt, float Temp, float Slope, float Gamma, float Lv,
		      float AirDens, float Vpd, float NetRad, float RaSoil,
		      float Transpiration, float Porosity, float Ks,
		      float Press, float m, float RootDepth,
		      float *MoistContent, float Adjust, float * SE);
    void execute();
private:

};

#endif // CSOILEVAPORATION_H
