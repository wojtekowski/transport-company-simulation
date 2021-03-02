//Wojciech Rosciszewski Wojtanowski
#pragma once
#include "CargoBatch.h"
#include <queue>
using namespace std;
class Truck
{
private:
	int id_;
	int current_localization_;
	int destination_ = 8; // 0-7 regional depots 8- hq
	const double kMaxCapacity = 12;
	double current_cargo_size_ = 0;
	double time_event_finish_time_;
	int type_of_event_;
	queue<CargoBatch*> cargo_truck_;
	bool ready_to_load_;
	bool drive_after_loading_;
	bool ready_to_go_;

public:
	Truck(int id);
	~Truck();
	void SetCurrentLocalization();
	int GetLocalization();
	void SetDestination(int destination);
	void SetNewTimeEvent(double time, int type);
	double GetTimeFinishTimeEvent();
	int GetTypeOfEvent();
	void SetReadyToLoad(bool condition);
	bool GetReadyToLoad();
	void SetReadyToGo(bool condition);
	bool GetReadyToGo();
	bool CheckSpaceForNextCargo(double size);
	void AddCargo(CargoBatch* cargo);
	int GetID();
	bool GetInfoAboutCargo();
	bool GetInfoAboutCargoQueue();
	CargoBatch* GetFirstCargo();
	double GetSizeCargo();
	void FinishUnloading();
	void SetDriveAfterLoading(bool condition);
	bool GetDriveAfterLoading();
	double GetProbability();
	int GetIdLoaded();

};

