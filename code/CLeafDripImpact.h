#ifndef CLEAFDRIPIMPACT_H
#define CLEAFDRIPIMPACT_H

#include "data.h"

class CLeafDripImpact
{
public:
    CLeafDripImpact();
    void init(VEGTABLE *VType, float* CanopyHeight, float * FallVelocity);
    void execute();
    void query(float* LD_FallVelocity );
private:
   float LD_FallVelocity;
   VEGTABLE * VType;
   float * CanopyHeight;
   float * FallVelocity;

};

#endif // CLEAFDRIPIMPACT_H
