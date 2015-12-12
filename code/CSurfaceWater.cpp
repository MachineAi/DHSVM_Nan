#include "CSurfaceWater.h"

CSurfaceWater::CSurfaceWater()
{

}
void CSurfaceWater::init(float m_PercArea, float m_Rainfall, float m_RoadArea, float m_DX, float m_DY, float m_Outflow, float m_IExcess )
{
PercArea = m_PercArea;
Rainfall = m_Rainfall;
RoadArea = m_RoadArea;
DX = m_DX;
DY = m_DY;
Outflow = m_Outflow;
IExcess = m_IExcess;
}
void CSurfaceWater::execute()
{

  /* SurfaceWater is rain falling on the hillslope +
     snowmelt on the hillslope (there is no snowmelt on the channel) +
     existing IExcess */
  /* related files: independant
  */
  SurfaceWater = (PercArea * Rainfall) +
    ((1. - (RoadArea)/(DX*DY)) * Outflow) +
    IExcess;

}
void CSurfaceWater::query(float * m_SurfaceWater)
{
    *m_SurfaceWater = SurfaceWater;
}
