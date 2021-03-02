//Wojciech Rosciszewski Wojtanowski
#include "Headquater.h"
#include <iostream>
using namespace std;

//HQ Constructor
Headquater::Headquater(bool condition, double time)
{
	custom_generate_time_ = condition;
	custom_time_ = time;
}

//HQ Destructor
Headquater::~Headquater()
{
	//delete our elements made in our class
	while (!cargo_queue_.empty())
	{
		CargoBatch* temp;
		temp = cargo_queue_[0];
		cargo_queue_.erase(cargo_queue_.begin());
		delete temp;
	}

}

//Return the size value of the truck queue
bool Headquater::GetTruckQueueSize()
{
	//return queue_truck_.size();
	if (queue_truck_.size() > 0)return true;
	else return false;
}

//Here we conditionally check if we have free space in platforms
bool Headquater::GetIsFreeSpacePlatforms()
{
	//The way this works is that if km is 3 size of our platforms and if it is larger then return true as we have space
	if (kM > platforms_.size())
	{
		return true;
	}
	//no space!
	else return false;
}

//Add Truck to queue.
void Headquater::AddTruckToQueue(Truck* truck)
{
	//Push Truck object back into queue.
	queue_truck_.push(truck);
}
//Add first truck in queue to platform
int Headquater::AddTruckFromQueueToPlatform()
{
	//Take first element from queue_truck_
	Truck* temp = queue_truck_.front();
	int id = temp->GetID();
	//Remove truck from queue.
	queue_truck_.pop();
	//Take the temporary variable and push to back of queue at platforms
	platforms_.push_back(temp);
	return id;
}

//Remove truck from the platform
void Headquater::DeleteTruckFromPlatform(int id)
{
	for (int i = 0; i < platforms_.size(); i++)
	{
		if (platforms_[i]->GetID() == id)
		{
			platforms_.erase(platforms_.begin() + i);
		}
	}
}

//Check if we have some cargo to load at platform
bool Headquater::CheckIsAnyCargoToLoad()
{
	if (cargo_queue_.size() > 0) return true;
	else return false;
}

int Headquater::GetInfoAboutWhichCargoAreTheMost()
{ 
	for (int i = 0; i < cargo_queue_.size(); i++)
	{
		//Here I did that if index is the same as ID then we increment
		cargo_[cargo_queue_[i]->GetId()]++;
	}
	int max = 0;
	int id;
	int counter = 0;
	//My vector id
	vector<int> id_v;
	for (int i = 0; i < 8; i++)
	{
		//First I look for max, so which id from our cargobatchs is largest
		if (max <= cargo_[i])
		{
			max = cargo_[i];
			id = i;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		//Check if we have the an ID fo cargo equal to our max 
		if (max == cargo_[i])
		{
			//if yes, we place it at the back of the vector
			id_v.push_back(i);
		}
	}
	//from 0 to the size of vector, rand is random so evenly spread
	id = id_v[rand() % id_v.size()];
	//for()
	//Here I return the ID
	for (int i = 0; i < 8; i++)
	{
		cargo_[i] = 0;
	}
	id_v.clear();
	return id;
}

//Return the size of cargo edit: with ID!
double Headquater::GetSizeCargoWithId(int id)
{
	for (int i = 0; i < cargo_queue_.size(); i++)
	{
		if (cargo_queue_[i]->GetId() == id)
		{
			return cargo_queue_[i]->GetSize();
		}
	}
	return -1;
}

//
CargoBatch* Headquater::GetCargoToLoadWithId(int id)
{
	for (int i = 0; i < cargo_queue_.size(); i++)
	{
		if (cargo_queue_[i]->GetId() == id)
		{
			CargoBatch* temp = cargo_queue_[i];
			cargo_queue_.erase(cargo_queue_.begin() + i);
			return temp;
		}
	}
}

//Return time
double Headquater::GetTimeGenerateCargo()
{
	return time_generate_next_cargo_;
}

//
void Headquater::SetTimeGenerateCargo(double time)
{
	//if negative then we read the custom time if positive we use standard time
	if (time >= 0)
	{
		time_generate_next_cargo_ = time;
	}
	else
	{
		time_generate_next_cargo_ += custom_time_;
	}
}

//Get next Cargo Batch
void Headquater::AddCargo(CargoBatch* cargo)
{
	cargo_queue_.push_back(cargo);
}

//
int Headquater::GetQueueSize()
{
	return cargo_queue_.size();
}


