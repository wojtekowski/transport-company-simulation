//Wojciech Rosciszewski Wojtanowski
#pragma once
#include <queue>
#include <vector>
using namespace std;
#include "Truck.h"
#include "CargoBatch.h"
class Headquater
{
private:
	const int kM = 3;
	queue<Truck*> queue_truck_;
	vector<Truck*> platforms_;
	vector<CargoBatch*> cargo_queue_; // vector but use like queue
	double time_generate_next_cargo_;
	bool custom_generate_time_;
	double custom_time_;
	//table 8 elements, 0-7
	int cargo_[8]{ 0 };
public:
	Headquater(bool condition, double time);
	~Headquater();
	bool GetTruckQueueSize();
	bool GetIsFreeSpacePlatforms();
	void AddTruckToQueue(Truck* truck);
	int AddTruckFromQueueToPlatform();
	void DeleteTruckFromPlatform(int id);
	bool CheckIsAnyCargoToLoad();
	int GetInfoAboutWhichCargoAreTheMost();
	double GetSizeCargoWithId(int id);
	CargoBatch* GetCargoToLoadWithId(int id);
	double GetTimeGenerateCargo();
	void SetTimeGenerateCargo(double time);
	void AddCargo(CargoBatch* cargo);
	int GetQueueSize();

};

