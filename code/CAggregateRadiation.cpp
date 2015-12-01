#include "CAggregateRadiation.h"

#include <stdio.h>
#include <stdlib.h>
#include "settings.h"
#include "massenergy.h"
CAggregateRadiation::CAggregateRadiation()
{

}
void CAggregateRadiation::init(int MaxVegLayers, int NVegL, PIXRAD * Rad,
			PIXRAD * TotalRad)
{

  int i;			/* counter */

  /* aggregate radiation data */
  for (i = 0; i < NVegL; i++) {
    TotalRad->NetShort[i] += Rad->NetShort[i];
    TotalRad->LongIn[i] += Rad->LongIn[i];
    TotalRad->LongOut[i] += Rad->LongOut[i];
  }
  TotalRad->NetShort[MaxVegLayers] += Rad->NetShort[NVegL];
  TotalRad->LongIn[MaxVegLayers] += Rad->LongIn[NVegL];
  TotalRad->LongOut[MaxVegLayers] += Rad->LongOut[NVegL];
  TotalRad->PixelNetShort += Rad->PixelNetShort;
  TotalRad->PixelLongIn += Rad->PixelLongIn;
  TotalRad->PixelLongOut += Rad->PixelLongOut;

}
void CAggregateRadiation::execute()
{

}
