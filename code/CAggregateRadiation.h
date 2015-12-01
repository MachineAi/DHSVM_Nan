#ifndef CAGGREGATERADIATION_H
#define CAGGREGATERADIATION_H

#include "data.h"
class CAggregateRadiation
{
public:
    CAggregateRadiation();
    void init(int MaxVegLayers, int NVegL, PIXRAD * Rad,
			PIXRAD * TotalRad);
    void execute();
private:
    ;

};

#endif // CAGGREGATERADIATION_H
