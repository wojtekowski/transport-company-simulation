//Wojciech Rosciszewski Wojtanowski
#pragma once
class CargoBatch
{
private:
	double size_;
	int id_;
	double generation_time_;
public:
	CargoBatch(double size, int id, double generation_time);
	int GetId();
	double GetSize();
	double GetQueuingTime(double time);
	double GetTransportTime(double time);
};

