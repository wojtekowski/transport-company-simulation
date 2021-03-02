//Wojciech Rosciszewski Wojtanowski
#pragma once
using namespace std;
#include <vector>
class SeedGenerator
{
private:
	int start_value_;
	const double kM = 2147483647.0;
	static const int kA = 16807;
	static const int kQ = 127773;
	static const int kR = 2836;
	vector<int> seeds_generated_;
public:
	//This is our constructor with out start value
	SeedGenerator(int start_value);
	//Destructor 
	~SeedGenerator() = default;
	vector<int> GenerateSeeds(int number_of_simulations);
};

