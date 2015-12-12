#include "CLeafDripImpact.h"

CLeafDripImpact::CLeafDripImpact()
{
}
void CLeafDripImpact::init(VEGTABLE *tmpVType, float* tmpCanopyHeight, float * tmpFallVelocity )
{
    VType = tmpVType;
    CanopyHeight = tmpCanopyHeight;
    FallVelocity = tmpFallVelocity;
}
void CLeafDripImpact::execute()
{
  /* Leaf drip impact*/
  /* Find corresponding fall velocity for overstory and understory heights
     by weighting scheme */
  /* related files: independent module
  */
   int i;
  if (VType->OverStory){
    /* staring at 1 assumes the overstory height > 0.5 m */
    for (i = 1; i <= 17; i++ ) {
      if (VType->Height[0] < CanopyHeight[i]) {
        LD_FallVelocity = ((VType->Height[0] - CanopyHeight[i-1])
			   *FallVelocity[i] +
			   (CanopyHeight[i] - VType->Height[0])*FallVelocity[i-1]) /
	  (CanopyHeight[i] - CanopyHeight[i-1]);
      }
    }
    if (VType->UnderStory) {
      /* ending at 16 assumes the understory height < 16 m */
      for (i = 0; i <= 16; i++) {
	if (VType->Height[1] < CanopyHeight[i]) {
	  LD_FallVelocity = ((VType->Height[1] - CanopyHeight[i])*FallVelocity[i] +
			     (CanopyHeight[i+1] - VType->Height[1])*FallVelocity[i-1]) /
	    (CanopyHeight[i+1] - CanopyHeight[i]);
	}
      }
    }
  }
  else if (VType->UnderStory) {
    /* ending at 16 assumes the understory height < 16 m */
    for (i = 0; i <= 16; i++) {
      if (VType->Height[0] < CanopyHeight[i]) {
	LD_FallVelocity = ((VType->Height[0] - CanopyHeight[i])*FallVelocity[i] +
			   (CanopyHeight[i+1] - VType->Height[0])*FallVelocity[i-1]) /
	  (CanopyHeight[i+1] - CanopyHeight[i]);
      }
    }
  }
  else LD_FallVelocity = 0;
}

void CLeafDripImpact::query(float* m_LD_FallVelocity )
{
    *m_LD_FallVelocity = LD_FallVelocity;
}
