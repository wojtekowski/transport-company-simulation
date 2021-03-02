//Wojciech Rosciszewski Wojtanowski
#pragma once
#include <queue>
#include <vector>
using namespace std;
#include "Truck.h"
#include "CargoBatch.h"
class Depot
{
private:
	const int kM = 2;
	queue<Truck*> queue_truck_;
	vector<Truck*> platforms_;
	vector<CargoBatch*> cargo_queue_; // vector but use like queue
	double time_generate_next_cargo_;
	bool custom_generate_time_;
	double custom_time_;

public:
	Depot(bool condition, double time);
	~Depot();
	int GetTruckQueueSize();
	bool GetIsFreeSpacePlatforms();
	void AddTruckToQueue(Truck* truck);
	int AddTruckFromQueueToPlatform();
	void DeleteTruckFromPlatform(int id);
	bool CheckIsAnyCargoToLoad();
	double GetSizeCargoWithId(int id);
	CargoBatch* GetCargoToLoadWithId(int id);
	double GetTimeGenerateCargo();
	void SetTimeGenerateCargo(double time);
	void AddCargo(CargoBatch* cargo);
	int GetQueueSize();
};

