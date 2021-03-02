//Wojciech Rosciszewski Wojtanowski
#pragma once

#include <vector>
using namespace std;
#include<iostream>
#include <random>
#include "SeedGenerator.h"
#include "Statistic.h"
#include "Headquater.h"
#include "Truck.h"
#include "Depot.h"
class Simulation
{
private:
	double simulation_time_ = 0;
	double initial_phase_time_ = 0;
	double max_simulation_time_ = 0;
	bool custom_time_generate_cargo_ = false;
	double time_generate_cargo_hq_ = 0;
	double time_generate_cargo_depot_ = 0;
	//Here I store the seed of vectors
	vector<int> seeds_;
	//step by step mode
	bool validation_;
	bool initial_phase_;
	//Here we save statisics that are presented at the end of simulation
	Statistic* statistic_;
	bool NO_EVENTTRIGGERED = true;
	//Here I store all of the trucks
	vector<Truck*> truck_vector_;
	const int kTruck = 12;
	//Here I store all of the Depot locations
	vector<Depot*> depot_vector_;
	const int kDepot = 8;
	//Here I store just our Headquarter
	Headquater* headquater_;
	int start_value_;
	//There are constants which I've used when implementing the seed random generator in our simulation
	const double kM = 2147483647.0;
	static const int kA = 16807;
	static const int kQ = 127773;
	static const int kR = 2836;

public:
	//Constructor
	Simulation(double initial_phase_time, double max_simulation_time, bool custom_time_generate_cargo, double time_hq, double time_depot, vector<int> seeds, bool validation, bool initial_phase_graph);
	//Destructor
	~Simulation();
	void StartSimulation();
	bool CheckTimeEventArrivalTruck(double time);
	bool CheckConditionalEventStartOfService();
	bool CheckTimeEventEndOfUnloading(double time);
	bool CheckConditionalStartLoadingCargo();
	bool CheckTimeEventEndOfLoading(double time);
	bool CheckConditionalTruckDepartue();
	bool CheckTimeEventGenerateCargo(double time);
	void SetNewSystemTime();
	double UniformGenerator(int id);
	int UniformGeneratorIntercal(int max, int min, int id);
};

