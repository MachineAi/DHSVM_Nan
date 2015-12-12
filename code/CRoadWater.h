#ifndef CROADWATER_H
#define CROADWATER_H

class CRoadWater
{
public:
    CRoadWater();
    void init(float m_RoadArea, float m_DX, float m_DY, float m_Rainfall, float m_Outflow, float m_IExcess);
    void execute();
    void query(float * m_RoadWater);
private:
    float RoadWater;
    float RoadArea;
    float DX;
    float DY;
    float Rainfall;
    float Outflow;
    float IExcess;

};

#endif // CROADWATER_H
