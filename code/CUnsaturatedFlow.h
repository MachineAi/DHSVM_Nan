#ifndef CUNSATURATEDFLOW_H
#define CUNSATURATEDFLOW_H

class CUnsaturatedFlow
{
public:
    CUnsaturatedFlow();
    void init(int Dt, float DX, float DY, float Infiltration,
		     float RoadbedInfiltration, float SatFlow, int NSoilLayers,
		     float TotalDepth, float Area, float *RootDepth, float *Ks,
		     float *PoreDist, float *Porosity, float *FCap,
		     float *Perc, float *PercArea, float *Adjust,
		     int CutBankZone, float BankHeight, float *TableDepth,
		     float *Runoff, float *Moist, int RoadRouteOption,
		     int InfiltOption, float *RoadIExcess);
    void execute();
private:

};

#endif // CUNSATURATEDFLOW_H
