//Wojciech Rosciszewski Wojtanowski
#include "Simulation.h"

Simulation::Simulation(double initial_phase_time, double max_simulation_time, bool custom_time_generate_cargo, double time_hq, double time_depot, vector<int> seeds, bool validation, bool initial_phase_graph)
{
	cout << "Start simulation" << endl;
	//Here I set all of our parameters to our class variables
	initial_phase_time_ = initial_phase_time;
	max_simulation_time_ = max_simulation_time;
	//This is if we want to use our own times
	custom_time_generate_cargo_ = custom_time_generate_cargo;
	seeds_ = seeds;
	//Validation is step by step mode
	validation_ = validation;
	//This true/false state is used to validate or reassure that the stats are reset once.
	initial_phase_ = true;
	time_generate_cargo_hq_ = time_hq;
	time_generate_cargo_depot_ = time_depot;
	//Here I "construct" new trucks
	for (int i = 0; i < kTruck; i++)
	{
		//We add it to a vector, i is the truck number
		Truck* temp = new Truck(i);
		truck_vector_.push_back(temp);
	}
	for (int i = 0; i < kDepot; i++)
	{
		//Here I construct the depots but the user can provide a custom time of generation or we just use the normal time to generate
		Depot* temp = new Depot(custom_time_generate_cargo_, time_generate_cargo_depot_);					//dodac jezeli user chce miec swoje czasy ladunku
		depot_vector_.push_back(temp);
	}
	//The same as for vector but I have one headquarter so no need for vector right
	headquater_ = new Headquater(custom_time_generate_cargo_, time_generate_cargo_hq_);
	//Here is my constructor to make the inital phase graph
	statistic_ = new Statistic(initial_phase_graph);
}

//Destructor of our simulation
Simulation::~Simulation()
{
	//Here I remove vectors etc. for next simulation so we don't mix up data (all elements we make in our class)
	delete headquater_;
	for (int i = 0; i < kDepot; i++)
	{
		delete depot_vector_[i];
	}
	for (int i = 0; i < kTruck; i++)
	{
		delete truck_vector_[i];
	}
	delete statistic_;
}

void Simulation::StartSimulation()
{
	while (simulation_time_ < max_simulation_time_)
	{
		if (validation_)
		{
			//So step by step mode we see the simulation time
			cout << "Simulation Time: " << simulation_time_ << endl;
			cin.get();
		}
		//Ideas: set a variable to indicate or just sets true/false states of our class variable
		//we have no event
		NO_EVENTTRIGGERED = true;
		//Here I conditionally check IF the initial phase is marked as true and the simulation time is smaller than or equal to the initial phase then we must reset the statistics
		if (initial_phase_ == true && initial_phase_time_ >= simulation_time_)
		{
			
			initial_phase_ = false;
			statistic_->ResetStatistic();
			if (validation_)
			{
				cout << "Statistics have been reset" << endl;
			}
		}
		if (CheckTimeEventArrivalTruck(simulation_time_)) //arrival truck to:
		{
			//I used a for loop to check all trucks
			for (int i = 0; i < kTruck; i++)
			{
				//Check here if we have 0 
				if (truck_vector_[i]->GetTypeOfEvent() == 0)
				{
					if (truck_vector_[i]->GetTimeFinishTimeEvent() == simulation_time_)
					{
						//I implemented that if a truck is at depot, being loaded, then the destination is set to it to HQ, we have 1 HQ.
						//So truck has arrived as its loaded so we set the next destination
						//We set current localisation to the localisation where it was going right, so its in a cycle as I imagined
						truck_vector_[i]->SetCurrentLocalization();
						NO_EVENTTRIGGERED = false;
						//I set that 0-7 is depot and hq is 8
						//We check on our truck what is its location
						if (truck_vector_[i]->GetLocalization() == 8)//Headquater = 8!
						{
							//this is for step by step mode
							if (validation_)
							{
								cout << "Truck with id: " << i << " has arrived to HQ with time: " << simulation_time_ << endl;
							}
							//Adding the truck to the HQ truck queue vector and leave it there
							headquater_->AddTruckToQueue(truck_vector_[i]);
							//I set -1 as time event to indicate that it has no time event it just stays in the queue and isn't affected, its just waiting to be conditionally popped from queue
							//where -1 is the time and -1 is type of event
							truck_vector_[i]->SetNewTimeEvent(-1, -1);
						}
						else //Depot = 0/7
						{
							//this is for step by step mode
							if (validation_)
							{
								cout << "Truck with id: " << i << " has arrived to depot with id: " << truck_vector_[i]->GetLocalization() << " with time: " << simulation_time_ << endl;
							}
							//This was hard to do, so we in fact have the same really as above but we have a vector so we needed to know WHICH depot we have yes
							//We take the truck id and we then get its localization from 0 to 7, then we know where it is and we just add that truck to our queue 
							depot_vector_[truck_vector_[i]->GetLocalization()]->AddTruckToQueue(truck_vector_[i]);
							//the same here
							truck_vector_[i]->SetNewTimeEvent(-1, -1);
						}
					}
				}
			}
		}
		//This is our conditional start of service (of truck)
		if (CheckConditionalEventStartOfService()) 
		{
			NO_EVENTTRIGGERED = false;
			
			if (headquater_->GetTruckQueueSize() && headquater_->GetIsFreeSpacePlatforms())
			{
				int id = headquater_->AddTruckFromQueueToPlatform();
				//we have id so we're working on that truck
				if (truck_vector_[id]->GetInfoAboutCargo())
				{
					//step by step mode
					if (validation_)
					{
						cout << "Truck with id: " << id << " has been added to the platform in hq - truck is empty: " << endl;
					}
					//Trucks at beginning arrive at depots without load, so they are ready to be loaded, omitts unloading stage
					truck_vector_[id]->SetReadyToLoad(true);
				}
				else
					//IF truck is actually loaded then:
					//we calculate time, so we have new event
				{
					//0.55 is Tu we multiply by the size of cargo and to this we add the simulation time because
					//here I want to make sure that the time is in fact larger than simulation time

					double time = simulation_time_ + truck_vector_[id]->GetSizeCargo() * 0.55;
					if (validation_)
					{
						cout << "Truck with id: " << id << " has been added to the platform in hq - finish unloading: " << time << endl;
					}
					//New time event, time generated and ID is set to as 1 so this is the END of loading.
					truck_vector_[id]->SetNewTimeEvent(time, 1);
					//and here for the statistics I save the mean time of USE of trucks, so how long a truck has been used. So time - simulation time will be the time of this event
					//I also added simulation_time_ and this is needed for initial phase
					//But this is uncessary if we dont select this option when running simulation
					statistic_->SaveUtilization(time - simulation_time_, simulation_time_);
				}
			}
			//I used a for loop to check all depots
			for (int j = 0; j < kDepot; j++)
			{
				//Goes through the depots, so 
				if (depot_vector_[j]->GetTruckQueueSize() > 0 && depot_vector_[j]->GetIsFreeSpacePlatforms())
				{
					//add truck from queue to platform and we get the id
					int id = depot_vector_[j]->AddTruckFromQueueToPlatform();
					//check if we have actually any cargo conditional checks
					if (truck_vector_[id]->GetInfoAboutCargo())
					{
						//for step by step mode (actually this doesnt occur, but added only for debugging)
						if (validation_)
						{
							cout << "Truck with id: " << id << " has been added to the platform to depot with id: " << truck_vector_[id]->GetLocalization() << " the truck is empty " << endl;
						}
						//so we have no cargo and truck is in condition that it is ready to load, its empty! :D
						truck_vector_[id]->SetReadyToLoad(true);
					}
					else
					{
						//Here we do the same, if we have cargo then we do as before. We use the 0.55 Tu to get the time of unloading at depot
						double time = simulation_time_ + truck_vector_[id]->GetSizeCargo() * 0.55;
						//this is for step by step mode
						if (validation_)
						{
							cout << "Truck with id: " << id << " has been added to the platform in depot with id: " << j << "  finish unloading: " << time << endl;
						}
						//New time event here, time generated and ID is set to as 1 so this is the END of loading.
						truck_vector_[id]->SetNewTimeEvent(time, 1);
						statistic_->SaveUtilization(time - simulation_time_, simulation_time_);
					}
				}
			}
		}
		//This is the activity to run at ending of our unloading stage.
		if (CheckTimeEventEndOfUnloading(simulation_time_))
		{
			//For each time event, false set to know if some event occurs we must go through the loop again and check if meanwhile some event didn't occur 
			NO_EVENTTRIGGERED = false;
			for (int i = 0; i < kTruck; i++)
			{
				//1 so it's finished. actually the same conditions as in the CheckTimeEventEndOfUnloading but just to be sure i dont want any anomalies.
				if (truck_vector_[i]->GetTypeOfEvent() == 1)
				{
					if (truck_vector_[i]->GetTimeFinishTimeEvent() == simulation_time_)
					{
						//At this activity stage the truck has finished unloading so we set -1 and -1 as no time event as next event is loading truck
						truck_vector_[i]->SetNewTimeEvent(-1, -1);
						//I get information if we have any cargo and will refresh (while loop) until we actually have a load
						while (!(truck_vector_[i]->GetInfoAboutCargoQueue()))
							//here i save the transfport time it took to transport 
						{
							//1. this time in fact is saved to the sum of cargo transport
							//2. We then get the first cargo of the unloaded truck 
							//3. we get the time of transport of that cargo (so the generation time of cargobatch < transport time)
							statistic_->SaveTransportTime(truck_vector_[i]->GetFirstCargo()->GetTransportTime(simulation_time_));
						}
						//then we finish unloading and setting current cargo size to 0.
						truck_vector_[i]->FinishUnloading();
						//set that the truck is ready to load
						truck_vector_[i]->SetReadyToLoad(true);
						//this is for step by step mode
						if (validation_)
						{
							cout << "Truck with ID: " << i << " finished unloading " << endl;
						}
					}
				}
			}
		}
		//Activity to check if we can actually start loading the truck
		if (CheckConditionalStartLoadingCargo())
		{
			//again, no event has been triggered set to false.
			NO_EVENTTRIGGERED = false;
			//Go through all trucks
			for (int i = 0; i < kTruck; i++)
			{
				//we get value if truck is ready to load
				if (truck_vector_[i]->GetReadyToLoad())
				{
					//if truck is at HQ
					if (truck_vector_[i]->GetLocalization() == 8)
					{
						//we now check if there is any cargo to load at HQ
						if (headquater_->CheckIsAnyCargoToLoad())
						{
							//We get the cargo that we have the most 
							int id = headquater_->GetInfoAboutWhichCargoAreTheMost();
							//Here I set destination
							truck_vector_[i]->SetDestination(id);
							//TO count the time loaded i added this variable
							double capacity_loaded_cargo = 0;
							//while loop, we take the cargo size id, if we dont have a cargobatch we're looking for value is -1 and while loop carries on looping
							//until we run out of space in truck or no more cargo batch
							while (headquater_->GetSizeCargoWithId(id) > 0)
							{
								//we check if we have space in truck, and here I take the size of the cargo from HQ
								if (truck_vector_[i]->CheckSpaceForNextCargo(headquater_->GetSizeCargoWithId(id)))
								{
									//this is our variable i created above, so this is the cargo that is loaded to truck
									capacity_loaded_cargo += headquater_->GetSizeCargoWithId(id);
									//Take our cargo batch from HQ
									CargoBatch* cargo = headquater_->GetCargoToLoadWithId(id);
									//this is the same as transport we save statistics, we add time to sum and increment counter, we return time - generation time of cargo 
									statistic_->SaveQueuingTimeCargo(cargo->GetQueuingTime(simulation_time_));
									//Here I just assign that cargo we took and load to truck
									truck_vector_[i]->AddCargo(cargo);
								}
								else //if we have no more cargo batch or space in truck
								{
									//truck is ready to go right after loading
									truck_vector_[i]->SetDriveAfterLoading(true);
									//we dont check any distributions so do break and exit from while loop
									break;
								}
							}
							//Truck is loaded so no need to load again
							truck_vector_[i]->SetReadyToLoad(false);
							//This is where I count the time of loading, and this is the variable we used
							double time = 0.65 * capacity_loaded_cargo;
							//this is for step by step mode
							if (validation_)
							{
								cout << "Truck with ID: " << i << " completes loading cargo in headquarter with time: " << time << " loading: " << capacity_loaded_cargo << endl;
							}
							//NEW TIME EVENT, id 2 is the end of truck loading
							truck_vector_[i]->SetNewTimeEvent(time + simulation_time_, 2);
							//and save the time stamp of our loading into statistics
							statistic_->SaveUtilization(time, simulation_time_);
						}
					}
					else
					{
						//this is the same but just for depots
						if (depot_vector_[truck_vector_[i]->GetLocalization()]->CheckIsAnyCargoToLoad())
						{
							//we're in depot so we know we go back to HQ
							truck_vector_[i]->SetDestination(8);
							//again this is the variable to count time of load
							double capacity_loaded_cargo = 0;
							//This was a hassle for me, so Dr I got the vector of our depot where our truck is right, and then i just take the cargo batches
							while (depot_vector_[truck_vector_[i]->GetLocalization()]->GetSizeCargoWithId(8) > 0)
							{
								//we check if we have space in truck, and here I take the size of the cargo from HQ
								if (truck_vector_[i]->CheckSpaceForNextCargo(depot_vector_[truck_vector_[i]->GetLocalization()]->GetSizeCargoWithId(8)))
								{
									//this is our variable i created before, so this is the cargo that is loaded to truck
									capacity_loaded_cargo += depot_vector_[truck_vector_[i]->GetLocalization()]->GetSizeCargoWithId(8);
									//Take our cargo batch from depot
									CargoBatch* cargo = depot_vector_[truck_vector_[i]->GetLocalization()]->GetCargoToLoadWithId(8);
									//this is the same as transport we save statistics, we add time to sum and increment counter, we return time - generation time of cargo
									statistic_->SaveQueuingTimeCargo(cargo->GetQueuingTime(simulation_time_));
									//Here I just assign that cargo we took and load to truck
									truck_vector_[i]->AddCargo(cargo);
								}
								else //if we have no more cargo batch or space in truck
								{
									//truck is ready to go right after loading
									truck_vector_[i]->SetDriveAfterLoading(true);
									//we dont check any distributions so do break and exit from while loop
									break;
								}
							}
							//Truck is loaded so no need to load again
							truck_vector_[i]->SetReadyToLoad(false);
							//This is where I count the time of loading, and this is the variable we used
							double time = 0.65 * capacity_loaded_cargo;
							//this is for step by step mode
							if (validation_)
							{
								cout << "Truck with id: " << i << " completes loading cargo in depot with id: " << truck_vector_[i]->GetLocalization() << " with time: " << time << endl;
							}
							//NEW TIME EVENT, id 2 is the end of truck loading
							truck_vector_[i]->SetNewTimeEvent(time + simulation_time_, 2);
							//and save the time stamp of our loading into statistics
							statistic_->SaveUtilization(time, simulation_time_);
						}
					}

				}
			}
		}
		//All of this was difficult but this was hard, not very intuitonal for me
		//So this is the time event check of our end, here if we get true...
		if (CheckTimeEventEndOfLoading(simulation_time_))
		{
			//event triggered
			NO_EVENTTRIGGERED = false;
			//we go through all of our trucks
			for (int i = 0; i < kTruck; i++)
			{
				//if the truck has been loaded (so time event 2)
				if (truck_vector_[i]->GetTypeOfEvent() == 2)
				{
					//if system time is ok
					if (truck_vector_[i]->GetTimeFinishTimeEvent() == simulation_time_)
					{
						//does the truck need to drive after loading?
						if (truck_vector_[i]->GetDriveAfterLoading())
						{
							//this is for step by step mode
							if (validation_)
							{
								cout << "Truck with id: " << i << " completes loading cargo  and it's ready to go " << endl;
							}
							//we set that  truck cannot drive after loading to avoid collisions 
							truck_vector_[i]->SetDriveAfterLoading(false);
							//truck is ready to gooooo
							truck_vector_[i]->SetReadyToGo(true);
						}
						else
							//if the truck doesn't have the get drive after loading as true
						{//
							//I check if localization is HQ
							if (truck_vector_[i]->GetLocalization() == 8)
							{
								//if we have HQ then here I check if the next cargo loaded into the truck will not overflow the truck limits
								//if it overflows then the truck can leave (as per project requirement)
								if (truck_vector_[i]->CheckSpaceForNextCargo(headquater_->GetSizeCargoWithId(truck_vector_[i]->GetIdLoaded())))
								{
									//Here I just calculate the probability of truck leaving (here I use ID (i) for seed so the seed is chainging and generates next) 
									//Uniform 0 - 1 
									double temp = UniformGenerator(i);
									//if our generated value is smaller than the probability of truck leaving (cargo size loaded / max capacity) and truck can leave
									if (temp < truck_vector_[i]->GetProbability())
									{
										//this is for step by step mode
										if (validation_)
										{
											cout << "Truck with id: " << i << " completes loading cargo  and it's ready to go (probability: " << truck_vector_[i]->GetProbability() << " )" << " gen: " << temp << endl;
										}
										//truck leaves so we end time event, not existent 
										truck_vector_[i]->SetNewTimeEvent(-1, -1);
										//truck is ready to go
										truck_vector_[i]->SetReadyToGo(true);
									}
									else
									{
										//if value is smaller then we check every 2 time new event, truck loaded (so time event 2)
										//checked until the truck cannot leave until matches the distribution probability
										truck_vector_[i]->SetNewTimeEvent(simulation_time_ + 2, 2);
										//this is for step by step mode
										if (validation_)
										{
											cout << "Truck with id: " << i << " completes loading cargo  and it's NOT ready to go (probability: " << truck_vector_[i]->GetProbability() << " )" << " gen: " << temp << endl;
										}
									}
								}
								else
								{
									//This is where the truck actually matches the criteria and can leave
									//this is for step by step mode
									if (validation_)
									{
										cout << "Truck with id: " << i << " completes loading cargo  and it's ready to go " << endl;
									}
									//At this activity stage the truck has finished loading so we set -1 and -1 as no time event
									truck_vector_[i]->SetNewTimeEvent(-1, -1);
									truck_vector_[i]->SetReadyToGo(true);
								}
							}

							else
							{
								//here is the same as above but for depot vector
								if (truck_vector_[i]->CheckSpaceForNextCargo(depot_vector_[truck_vector_[i]->GetLocalization()]->GetSizeCargoWithId(truck_vector_[i]->GetIdLoaded())))
								{
									double temp = UniformGenerator(i);
									if (temp < truck_vector_[i]->GetProbability())
									{
										truck_vector_[i]->SetNewTimeEvent(-1, -1);
										truck_vector_[i]->SetReadyToGo(true);
										//this is for step by step mode
										if (validation_)
										{
											cout << "Truck with id: " << i << " completes loading cargo  and it's ready to go (probability: " << truck_vector_[i]->GetProbability() << " )" << " gen: " << temp << endl;
										}
									}
									else
									{
										truck_vector_[i]->SetNewTimeEvent(simulation_time_ + 2, 2);
										//this is for step by step mode
										if (validation_)
										{
											cout << "Truck with id: " << i << " completes loading cargo  and it's not ready to go (probability: " << truck_vector_[i]->GetProbability() << " )" << " gen: " << temp << endl;
										}
									}
								}
								else
								{
									truck_vector_[i]->SetNewTimeEvent(-1, -1);
									truck_vector_[i]->SetReadyToGo(true);
									//this is for step by step mode
									if (validation_)
									{
										cout << "Truck with id: " << i << " completes loading cargo  and it's ready to go " << endl;
									}
								}
							}
						}
					}
				}
			}
		}
		//Truck departure from a location
		//if in some truck we will have ready to go set as true
		if (CheckConditionalTruckDepartue())
		{
			NO_EVENTTRIGGERED = false;
			for (int i = 0; i < kTruck; i++)
			{
				if (truck_vector_[i]->GetReadyToGo())
				{
					//we initialise all values
					double time = 0;
					normal_distribution<double> distribution_traveling_time(22.0, 1.8);
					default_random_engine generator_traveling_time(seeds_[i]);
					UniformGenerator(i); //We want to change seed as the default_random_engine would generate the same values
					//generate time
					time = distribution_traveling_time(generator_traveling_time);
					//and save the time of use and to our statistics
					statistic_->SaveUtilization(time, simulation_time_);
					//Here we decide where the truck is, if truck is at HQ:
					if (truck_vector_[i]->GetLocalization() == 8)
					{
						//Then we remove from platform as its loaded (for each truck, i)
						headquater_->DeleteTruckFromPlatform(i);
						//this is for step by step mode
						if (validation_)
						{
							cout << "Truck with ID: " << i << " leaves HQ " << endl;
						}
					}
					else
						//this is the same for depot location if truck is at depot: , get location and delete it
					{
						depot_vector_[truck_vector_[i]->GetLocalization()]->DeleteTruckFromPlatform(i);
						//this is for step by step mode
						if (validation_)
						{
							cout << "Truck with ID: " << i << " leaves depot with ID: " << i << endl;
						}
					}
					//True left so we set as false as it has left
					truck_vector_[i]->SetReadyToGo(false);
					//new time event, 0 id is the first time event in my case. So it is the arrival at HQ/Depot and just loops
					truck_vector_[i]->SetNewTimeEvent(simulation_time_ + time, 0);
					//this is for step by step mode
					if (validation_)
					{
						cout << "Truck with id: " << i << " finish : " << simulation_time_ + time << endl;
					}
				}
			}
		}
		//Here is activity of checking the time event of cargo batch generation
		//if at HQ our carbo batch generation time is equal to the system time, if yes ten true, if no then we go through depots
		if (CheckTimeEventGenerateCargo(simulation_time_))
		{
			NO_EVENTTRIGGERED = false;
			//here we check if it occurs at HQ
			if (headquater_->GetTimeGenerateCargo() == simulation_time_)
			{
				//this is for step by step mode
				if (validation_)
				{
					cout << "A cargo was generated in hq " << endl;
				}
				//seed 8 as HQ is 8, same as for trucks right
				default_random_engine generator_cargo_weight(seeds_[8]);
				//change seed 
				UniformGenerator(8);
				//This is done as in the project task description, my chosen values 2.5 and 0.8
				normal_distribution<double> distribution_hq_weight(2.5, 0.8);
				double size = distribution_hq_weight(generator_cargo_weight);
				//here we calculate the cargo batch ID, in HQ we have id 0-7, and 8 is the seed from 8, 8 not i no need to do this.
				int id = UniformGeneratorIntercal(8, 0, 8);
				//call cargo batch object with generated size according to our distribution, its unique ID
				//if size is smaller than 0 then we take minimal case (0.1)
				//if size is larger than 10 then we take maximal case (10.0)
				//i did this as i had an issue that the generator generated negative values and over 10
				CargoBatch* cargo = new CargoBatch(size, id, simulation_time_);
				//and we just add the cargo from HQ localization to truck
				headquater_->AddCargo(cargo);
				//I save the size of our hq queue to stats and amount of saves
				statistic_->SaveSizeQueueHQ(headquater_->GetQueueSize());
				//if the user didn't give their own parameters of cargo generation
				if (!custom_time_generate_cargo_)
				{
					//we make new generator with time 3.1 as average see lambda below
					default_random_engine generator_cargo_time(seeds_[8]);
					UniformGenerator(8);
					exponential_distribution<double> distribution_hq_cargo_time(0.3); // lambda = 1/ E[x] -> 1/3.1= 0.3
					//saving time
					headquater_->SetTimeGenerateCargo(distribution_hq_cargo_time(generator_cargo_time) + simulation_time_);
				}
				else
					//if the user gave their own parameters of cargo generation
				{
					//this is if the user wants custom time yes
					headquater_->SetTimeGenerateCargo(-1);
				}
			}
			//for all depots
			for (int i = 0; i < kDepot; i++)
			{
				if (depot_vector_[i]->GetTimeGenerateCargo() == simulation_time_)
				{
					//this is for step by step mode
					if (validation_)
					{
						cout << "A cargo was generated in depot with id: " << i << endl;
					}
					//new generator
					default_random_engine generator_cargo_weight(seeds_[i]);
					UniformGenerator(i);
					normal_distribution<double> distribution_depot_weight(2.5, 0.8);
					double size = distribution_depot_weight(generator_cargo_weight);
					//hq id as the trucks from depots just go back right
					int id = 8;
					//new cargo batch
					CargoBatch* cargo = new CargoBatch(size, id, simulation_time_);
					//add cargo from vector to truck
					depot_vector_[i]->AddCargo(cargo);
					//again we save statistics of queue size into depot vector for stats
					statistic_->SaveSizeQueueDepot(depot_vector_[i]->GetQueueSize(), i);
					//again, this part is for when we want to use our generated distributions according to lambda
					if (!custom_time_generate_cargo_)
					{

						default_random_engine generator_cargo_time(seeds_[i]);
						UniformGenerator(i);
						exponential_distribution<double> distribution_hq_cargo_time(0.098); // lambda = 1/ E[x] -> 1/10.2= 0.098
						depot_vector_[i]->SetTimeGenerateCargo(distribution_hq_cargo_time(generator_cargo_time) + simulation_time_);
					}
					//this is where the users sets their own distributions
					else
					{
						depot_vector_[i]->SetTimeGenerateCargo(-1);
					}
				}
			}
		}
		//This is how i looped the time of the simulation, i *2
		if (!NO_EVENTTRIGGERED)
		{
			SetNewSystemTime();
		}

	}
	statistic_->PrintStatistic(simulation_time_);

}



bool Simulation::CheckTimeEventArrivalTruck(double time)
{
	//time event with id 0
	for (int i = 0; i < kTruck; i++)
	{
		//arrival is 0
		if (truck_vector_[i]->GetTypeOfEvent() == 0)
		{
			if (truck_vector_[i]->GetTimeFinishTimeEvent() == time)
			{
				return true;
			}
		}
	}
	return false;
}

bool Simulation::CheckConditionalEventStartOfService()
{
	////we check if at headquarters the size of HQ and then we check if we have any free platforms
	if (headquater_->GetTruckQueueSize() && headquater_->GetIsFreeSpacePlatforms())
	{
		//true as the conditonal can be done
		return true;
	}
	for (int i = 0; i < kDepot; i++)
	{
		//we check if we have space and free platforms (the same as for HQ)
		if (depot_vector_[i]->GetTruckQueueSize() > 0 && depot_vector_[i]->GetIsFreeSpacePlatforms())
		{
			return true;
		}
	}
	return false;
}

bool Simulation::CheckTimeEventEndOfUnloading(double time)
{
	//We go through the entire truck vector
	for (int i = 0; i < kTruck; i++)
	{
		//This is our ID1 for our time event so here i set the ending of our unloading activity
		if (truck_vector_[i]->GetTypeOfEvent() == 1)
		{
			//Here I just compare if the time we have unloading is the same as the system time
			if (truck_vector_[i]->GetTimeFinishTimeEvent() == time)
			{
				return true;
			}
		}
	}
	return false;
}

bool Simulation::CheckConditionalStartLoadingCargo()
{
	for (int i = 0; i < kTruck; i++)
	{
		//Get condition if truck is ready to load
		if (truck_vector_[i]->GetReadyToLoad())
		{
			//Get localization of truck, 8 is HQ.
			if (truck_vector_[i]->GetLocalization() == 8)
			{
				//Do we have cargo to load? if true
				if (headquater_->CheckIsAnyCargoToLoad())
				{
					return true;
				}
			}
			
			else
			{
				if (depot_vector_[truck_vector_[i]->GetLocalization()]->CheckIsAnyCargoToLoad())
				{
					return true;
				}
			}

		}
	}
	return false;
}

bool Simulation::CheckTimeEventEndOfLoading(double time)
{
	for (int i = 0; i < kTruck; i++)
	{
		if (truck_vector_[i]->GetTypeOfEvent() == 2)
		{
			if (truck_vector_[i]->GetTimeFinishTimeEvent() == time)
			{
				return true;
			}
		}
	}
	return false;
}

bool Simulation::CheckConditionalTruckDepartue()
{
	for (int i = 0; i < kTruck; i++)
	{
		if (truck_vector_[i]->GetReadyToGo())
		{
			return true;
		}
	}
	return false;

}

bool Simulation::CheckTimeEventGenerateCargo(double time)
{
	if (headquater_->GetTimeGenerateCargo() == time)return true;
	for (int i = 0; i < kDepot; i++)
	{
		if (depot_vector_[i]->GetTimeGenerateCargo() == time)
		{
			return true;
		}
	}
	return false;
}

void Simulation::SetNewSystemTime()
{
	//goal is to find the minimum of time
	double temp_time_ = max_simulation_time_ * 2; //int_max but i used *2 just to be sure condition is ok
	//go through all trucks
	for (int i = 0; i < kTruck; i++)
	{
		//go through if time is larger than -1 because if not we would have collision as I needed to check here if there is some time event planned
		if (truck_vector_[i]->GetTimeFinishTimeEvent() > -1)
		{
			if (truck_vector_[i]->GetTimeFinishTimeEvent() < temp_time_)
			{
				temp_time_ = truck_vector_[i]->GetTimeFinishTimeEvent();
			}
		}
	}
	for (int i = 0; i < kDepot; i++)
	{
		if (depot_vector_[i]->GetTimeGenerateCargo() > -1)
		{
			//time of cargo batch generated
			if (depot_vector_[i]->GetTimeGenerateCargo() < temp_time_)
			{
				temp_time_ = depot_vector_[i]->GetTimeGenerateCargo();
			}
		}
	}
	if (headquater_->GetTimeGenerateCargo() > -1)
	{
		if (headquater_->GetTimeGenerateCargo() < temp_time_)
		{
			temp_time_ = headquater_->GetTimeGenerateCargo();
		}
	}
	//I came up with an idea to prewencyjnie /preventively? make sure that if the simulation time is actually larger than our temporary time as that would mean that the time is going backwards
	if (simulation_time_ > temp_time_)
	{
		cout << "ERROR" << endl;
		cin.get();
	}
	else
	{
		//temp time or our minimal time is written to the simulation time
		simulation_time_ = temp_time_;
	}
}

double Simulation::UniformGenerator(int id)
{
	int h = seeds_[id] / kQ;
	seeds_[id] = kA * (seeds_[id] - kQ * h) - kR * h;
	if (seeds_[id] < 0)seeds_[id] = seeds_[id] + static_cast<int>(kM);
	return seeds_[id] / kM;
}

int Simulation::UniformGeneratorIntercal(int max, int min, int id)
{
	return (int)(UniformGenerator(id) * (max - min) + min);
}
