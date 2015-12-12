#ifndef CSURFACEWATER_H
#define CSURFACEWATER_H

class CSurfaceWater
{
public:
    CSurfaceWater();
    void init(float m_PercArea, float m_Rainfall, float m_RoadArea, float m_DX, float m_DY, float m_Outflow, float m_IExcess );
    void execute();
    void query(float * m_SurfaceWater);
private:
float PercArea;
float Rainfall;
float RoadArea;
float DX;
float DY;
float Outflow;
float IExcess;
float SurfaceWater;
};

#endif // CSURFACEWATER_H
