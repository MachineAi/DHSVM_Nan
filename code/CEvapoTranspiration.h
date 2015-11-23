#ifndef CEVAPOTRANSPIRATION_H
#define CEVAPOTRANSPIRATION_H

#include "data.h"

class CEvapoTranspiration
{
public:
    CEvapoTranspiration();
    void init(int Layer, int Dt, PIXMET * Met, float NetRad,
			float Rp, VEGTABLE * VType, SOILTABLE * SType,
			float MoistureFlux, SOILPIX * LocalSoil, float *Int,
			EVAPPIX * LocalEvap, float *Adjust, float Ra);
    void execute();
private:


};

#endif // CEVAPOTRANSPIRATION_H
