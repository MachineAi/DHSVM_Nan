#include "CRoadWater.h"

CRoadWater::CRoadWater()
{

}
void CRoadWater::init(float m_RoadArea, float m_DX, float m_DY, float m_Rainfall, float m_Outflow, float m_IExcess)
{
    RoadArea = m_RoadArea;
    DX = m_DX;
    DY = m_DY;
    Rainfall = m_Rainfall;
    Outflow = m_Outflow;
    IExcess = m_IExcess;
}
void CRoadWater::execute()
{
  /* RoadWater is rain falling on the road surface +
     snowmelt on the road surface + existing Road IExcess
     (Existing road IExcess = 0). WORK IN PROGRESS*/
  /* related files: independant
  */
  RoadWater = (RoadArea/(DX*DY) *
	       (Rainfall + Outflow)) + IExcess;
}
void CRoadWater::query(float * m_RoadWater)
{
    *m_RoadWater = RoadWater;
}
