//Wojciech Rosciszewski Wojtanowski
#include "SeedGenerator.h"

SeedGenerator::SeedGenerator(int start_value)
{
	//We assign a start value to our class variable
	start_value_ = start_value;
}

vector<int> SeedGenerator::GenerateSeeds(int number_of_simulations)
{
	int counter = 0;
	//Here I used the number of trucks * amount of simulations * 100000 and we will produce number of seeds
	for (int i = 0; i < 12 * number_of_simulations * 100000; i++)
	{
		counter++;
		//This is the normal generator
		int h = start_value_ / kQ;
		start_value_ = kA * (start_value_ - kQ * h) - kR * h;
		if (start_value_ < 0)start_value_ = start_value_ + static_cast<int>(kM);
		if (counter >= 100000)
		{
			//The start value is saved into our generated seed.
			seeds_generated_.push_back(start_value_);
			counter = 0;
		}
	}
	return seeds_generated_;
}
