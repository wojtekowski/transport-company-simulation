//Wojciech Rosciszewski Wojtanowski
#include "Truck.h"
#include <iostream>
using namespace std;

//Truck Constructor
Truck::Truck(int id)
{
	id_ = id;
	current_localization_ = 8; // On start add all truck to hq
	time_event_finish_time_ = 0;// arrive to hq in time 0;
	type_of_event_ = 0; // 0 - Arrive to hq
	ready_to_load_ = false;
	drive_after_loading_ = false;
}
//Truck Destructor
Truck::~Truck()
{
	//remove all truck cargo
	while (!cargo_truck_.empty())
	{
		CargoBatch* temp;
		temp = cargo_truck_.front();
		cargo_truck_.pop();
		delete temp;
	}
}

//Setting the Location of truck where it was going (when truck is arriving at its destination and then we assign as localization)
void Truck::SetCurrentLocalization()
{
	current_localization_ = destination_;
}

//Returning the current location of truck
int Truck::GetLocalization()
{
	return current_localization_;
}

//Where is the truck headed next
void Truck::SetDestination(int destination)
{
	destination_ = destination;
}

//Setting of new time event
void Truck::SetNewTimeEvent(double time, int type)
{
	time_event_finish_time_ = time;
	type_of_event_ = type;
}

//Method to take the end of an event
double Truck::GetTimeFinishTimeEvent()
{
	return time_event_finish_time_;
}

//Method to get the type of event (-1, 0, 1, 2) etc
int Truck::GetTypeOfEvent()
{
	return type_of_event_;
}

//First time arriving at Depot from HQ truck is empty!
void Truck::SetReadyToLoad(bool condition)
{
	ready_to_load_ = condition;
}

//get the value is the truck is ready to load
bool Truck::GetReadyToLoad()
{
	return ready_to_load_;
}

//true/false if truck is ready to leave and travel
void Truck::SetReadyToGo(bool condition)
{
	ready_to_go_ = condition;
}

//get if truck is ready for departure or not
bool Truck::GetReadyToGo()
{
	return ready_to_go_;
}

//Check if next cargo will fit into the truck
bool Truck::CheckSpaceForNextCargo(double size)
{
	if (kMaxCapacity >= current_cargo_size_ + size)
	{
		return true;
	}
	else return false;
}

//Adding another cargo to our truck
void Truck::AddCargo(CargoBatch* cargo)
{
	current_cargo_size_ += cargo->GetSize();
	cargo_truck_.push(cargo);
}

//Return the id of that truck
int Truck::GetID()
{
	return id_;
}

//Get the information if cargo is available (used at start of simulation)
bool Truck::GetInfoAboutCargo()
{
	if (current_cargo_size_ == 0)
	{
		return true;
	}
	else return false;
}

//Do we have a cargo in truck? So we check the cargo_truck_ queue
bool Truck::GetInfoAboutCargoQueue()
{
	return cargo_truck_.empty();
}

//Here we return the first cargo in queue
CargoBatch* Truck::GetFirstCargo()
{
	CargoBatch* temp = cargo_truck_.front();
	cargo_truck_.pop();
	return temp;
}

//Return the size of the current cargo
double Truck::GetSizeCargo()
{
	return current_cargo_size_;
}

//Setting unloading of truck, truck empty cargo 0
void Truck::FinishUnloading()
{
	current_cargo_size_ = 0;
}

//Truck can leave right after loading, used if next load is too large and won't fit in truck
void Truck::SetDriveAfterLoading(bool condition)
{
	drive_after_loading_ = condition;
}

//Get condition if Truck can leave
bool Truck::GetDriveAfterLoading()
{
	return drive_after_loading_;
}

//Get the probability that the truck will leave so size of cargo we currently have / maximal capacity of truck which is 12
double Truck::GetProbability()
{
	return current_cargo_size_ / kMaxCapacity;
}

//ID of load that is found in the queue
int Truck::GetIdLoaded()
{
	return cargo_truck_.front()->GetId();
}
