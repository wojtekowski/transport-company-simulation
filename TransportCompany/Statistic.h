//Wojciech Rosciszewski Wojtanowski
#pragma once
#include <vector>
using namespace std;
class Statistic
{
private:
	//for Average queuing cargo
	double sum_queuing_time_cargo_ = 0;
	double amount_save_times_queuint_cargo = 0;
	//for Average number queude cargo units in HQ
	double queue_size_hq_ = 0;
	double amount_save_size_hq_ = 0;
	//for Average number queue cargo units in Depot's
	vector<double> queue_size_depot_;
	vector<double> amount_save_size_depot_;
	//for Average transport time
	double sum_transport_time_cargo_ = 0;
	double amount_save_transport_time_cargo_ = 0;
	//for Average utilization of trucks
	double sum_time_of_utilization_ = 0;
	bool initial_phase_graph_;
public:
	Statistic(bool initial_phase_graph);
	~Statistic() = default;
	void SaveQueuingTimeCargo(double time);
	void SaveSizeQueueHQ(unsigned size);
	void SaveSizeQueueDepot(unsigned size, int id_depot);
	void SaveTransportTime(double time);
	void SaveUtilization(double time, double simulation_time);
	void PrintStatistic(double time);
	void ResetStatistic();
};

