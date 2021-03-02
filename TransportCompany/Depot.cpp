//Wojciech Rosciszewski Wojtanowski
#include "Depot.h"

//Depot Constructor
Depot::Depot(bool condition, double time)
{
	//saving our times
	custom_generate_time_ = condition;
	custom_time_ = time;
}

//Depot Destructor
Depot::~Depot()
{
	//while loop when removing I clear queue of all cargo batchs
	while (!cargo_queue_.empty())
	{
		CargoBatch* temp;
		temp = cargo_queue_[0];
		cargo_queue_.erase(cargo_queue_.begin());
		delete temp;
	}

}

//Return the size value of the truck queue
int Depot::GetTruckQueueSize()
{
	//return queue_truck_.size();
	if (queue_truck_.size() > 0) return true;
	else return false;
}

//Here we conditionally check if we have free space at platforms
bool Depot::GetIsFreeSpacePlatforms()
{
	//The way this works is that if km is 2 size of our platforms and if it is larger then return true as we have space
	if (kM > platforms_.size())
	{
		return true;
	}
	else return false;
}

//Add Truck to queue.
void Depot::AddTruckToQueue(Truck* truck)
{
	//Push Truck object back into queue.
	queue_truck_.push(truck);
}

//Add first truck in queue to platform
int Depot::AddTruckFromQueueToPlatform()
{
	//Take first element from queue_truck_
	Truck* temp = queue_truck_.front();
	//Remove truck from queue.
	queue_truck_.pop();
	//Take the temporary variable and push to back of queue at platforms
	platforms_.push_back(temp);
	return temp->GetID();
}

//Remove truck from the platform according to ID
void Depot::DeleteTruckFromPlatform(int id)
{
	for (int i = 0; i < platforms_.size(); i++)
	{
		if (platforms_[i]->GetID() == id)
		{
			platforms_.erase(platforms_.begin() + i);
		}
	}
}

//Check if we have some cargo to load at platform so queue must be bigger than 0
bool Depot::CheckIsAnyCargoToLoad()
{
	if (cargo_queue_.size() > 0) return true;
	else return false;
}

//Return the size of cargo with ID!
double Depot::GetSizeCargoWithId(int id)
{
	for (int i = 0; i < cargo_queue_.size(); i++)
	{
		if (cargo_queue_[i]->GetId() == id)
		{
			//if it has ID we return its size
			return cargo_queue_[i]->GetSize();
		}
	}
	//if no cargo
	return -1;
}
//Return the cargo that we need to load into truck edit: with ID!
CargoBatch* Depot::GetCargoToLoadWithId(int id)
{
	for (int i = 0; i < cargo_queue_.size(); i++)
	{
		if (cargo_queue_[i]->GetId() == id)
		{
			CargoBatch* temp = cargo_queue_[i];
			//remove queue and return the cargo 
			cargo_queue_.erase(cargo_queue_.begin() + i);
			return temp;
		}
	}
}

//Return time of time generated of next cargo
double Depot::GetTimeGenerateCargo()
{
	return time_generate_next_cargo_;
}

//setting time to generate the next cargo 
void Depot::SetTimeGenerateCargo(double time)
{
	if (time >= 0)
	{
		time_generate_next_cargo_ = time;
	}
	else
	{
		time_generate_next_cargo_ += custom_time_;
	}
}

//Get next Cargo Batch add to queue
void Depot::AddCargo(CargoBatch* cargo)
{
	cargo_queue_.push_back(cargo);
}

//return the size of our queue
int Depot::GetQueueSize()
{
	return cargo_queue_.size();
}