// This is the main.cpp file, so from this we will start and end the simulation of the program.
// _    _       _      _           _    ______               _                          _    _ 
//| |  | |     (_)    (_)         | |   | ___ \             (_)                        | |  (_)
//| |  | | ___  _  ___ _  ___  ___| |__ | |_/ /___  ___  ___ _ ___ _________      _____| | ___ 
//| |/\| |/ _ \| |/ __| |/ _ \/ __| '_ \|    // _ \/ __|/ __| / __|_  / _ \ \ /\ / / __| |/ / |
//\  /\  / (_) | | (__| |  __/ (__| | | | |\ \ (_) \__ \ (__| \__ \/ /  __/\ V  V /\__ \   <| |
// \/  \/ \___/| |\___|_|\___|\___|_| |_\_| \_\___/|___/\___|_|___/___\___| \_/\_/ |___/_|\_\_|
//            _/ |                                                                             
//           |__/                                                                              
// 2020 E&T 140062

//Basics
#include <iostream>
#include <vector>
using namespace std;
//Here we include our header
#include "Simulation.h"

//ask if i can add this project into my git

int main()
{
	cout << "Written by Wojciech Rościszewski Wojtanowski 2021.";
	cout << endl;
	//USER INPUT: Here we specify the duration of our simulation
	double duration;
	cout << "Specify the duration of the simulation (e.g. 5000) ";
	cin >> duration;
	cout << endl;

	//USER INPUT: Here we set the initial phase duration (Time after statistics stats are reset)
	double initial_phase;
	cout << "Specify the duration of the initial time (e.g. 0)";
	cin >> initial_phase;
	cout << endl;

	//USER INPUT: Here we just set if the user wants to provide different generation times
	bool custom_cargo;
	cout << "Do you want to use user specified load generation times? (1 = yes, 0 = no) ";
	cin >> custom_cargo;
	cout << endl;
	double custom_cargo_hq = -1;
	double custom_cargo_depot = -1;

	//USER INPUT: IF user selected 1 then user must provide the duration of loads
	if (custom_cargo)
	{
		cout << "Specify how often the load is to be generated in HQ Location ";
		cin >> custom_cargo_hq;
		cout << endl;
		cout << "Specify how often the load is to be generated in Depot Location ";
		cin >> custom_cargo_depot;
		cout << endl;
	}

	//USER INPUT: Here we set if the user wants updates on screen on how the simulation is going
	bool validation;
	cout << "Do you want to display simulation run (step-by-step) (1 = yes, 0 = no) ";
	cin >> validation;
	cout << endl;

	//USER INPUT: Does the user want to generate a initial phase graph. I really suggest to use for one simulation as it take a very long time on my computer
	bool initial_phase_graph;
	cout << "Do you want to create a graph of the initial phase ? (1 = yes, 0 = no) ";
	cin >> initial_phase_graph;
	cout << endl;

	//USER INPUT : User provides number of simulations. I guess we can use either 1 or 10.
	int number_of_simulations;
	cout << "Enter the number of simulations ";
	cin >> number_of_simulations;
	cout << endl;

	//This is our Random Generator, it links to the class which has constants.
	SeedGenerator* seed_generator = new SeedGenerator(992838);
	//Here the make a new vector, seed_generator vector added to seed for our simulation new vector
	vector<int> seeds_for_simulations = seed_generator->GenerateSeeds(number_of_simulations);
	//Here we have a seed for a single simulation
	vector<int> seeds_for_one_simulation;

	//This is our loop where we make the simulations happen 
	for (int i = 0; i < number_of_simulations; i++)
	{
		//Previously my seeds were not cleared and caused issues, we must clear the seeds from previous simulation
		seeds_for_one_simulation.clear();
		//For 12 trucks (each)
		for (int i = 0; i < 12; i++)
		{
			//Here we take the first seed from all seeds and added to one simulation
			seeds_for_one_simulation.push_back(seeds_for_simulations[0]);
			//Here we delete so we can take another seed yes, so we don't mix things up
			seeds_for_simulations.erase(seeds_for_simulations.begin());
		}
		//This is our main Simulation class, we call our constructor using NEW simulation and is the first function started.
		Simulation* simulation = new Simulation(
			initial_phase,
			duration,
			custom_cargo,
			custom_cargo_hq,
			custom_cargo_depot,
			seeds_for_one_simulation,
			validation,
			initial_phase_graph);

		//We start our simulation here
		simulation->StartSimulation();
		//We remove class so that the old simulation will not cover anything as it could mix up results
		delete simulation;
	}
	return 0;
}