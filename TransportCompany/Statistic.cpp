//Wojciech Rosciszewski Wojtanowski
#include "Statistic.h"
using namespace std;
#include <iostream>
#include <fstream>
Statistic::Statistic(bool initial_phase_graph)
{
	for (int i = 0; i < 8; i++)
	{
		queue_size_depot_.push_back(0);
		amount_save_size_depot_.push_back(0);
	}
	initial_phase_graph_ = initial_phase_graph;
}

void Statistic::SaveQueuingTimeCargo(double time)
{
	//This is the sum
	sum_queuing_time_cargo_ += time;
	//This is the counter
	amount_save_times_queuint_cargo++;
}

void Statistic::SaveSizeQueueHQ(unsigned size)
{
	queue_size_hq_ += (double)size;
	amount_save_size_hq_++;
}

void Statistic::SaveSizeQueueDepot(unsigned size, int id_depot)
{
	queue_size_depot_[id_depot] += (double)size;
	amount_save_size_depot_[id_depot]++;
}

void Statistic::SaveTransportTime(double time)
{
	sum_transport_time_cargo_ += time;
	amount_save_transport_time_cargo_++;
}

void Statistic::SaveUtilization(double time, double simulation_time)
{
	sum_time_of_utilization_ += time;
	if (initial_phase_graph_)
	{
		ofstream outfile;

		outfile.open("InitialPhaseGraph.txt", ios_base::app);//std::ios_base::app
		outfile << simulation_time << " " << (sum_time_of_utilization_ / 12) / simulation_time << endl;
		outfile.close();
	}
}
//All of our taken times
void Statistic::PrintStatistic(double time)
{
	//I wanted to save my statistics into a seperate table
	ofstream outfile;
	outfile.open("Statistic.txt", ios_base::app);//std::ios_base::app
	//Average queueing time for cargo = Total time of queuing time of cargo / amount of saved times
	cout << "Average queuing time for cargo: " << sum_queuing_time_cargo_ / amount_save_times_queuint_cargo << endl;
	outfile << "Average queuing time for cargo: " << sum_queuing_time_cargo_ / amount_save_times_queuint_cargo << endl;
	cout << "Average number of queued cargo units in Headquarter: " << queue_size_hq_ / amount_save_size_hq_ << endl;
	outfile << "Average number of queued cargo units in Headquarter: " << queue_size_hq_ / amount_save_size_hq_ << endl;
	cout << "Average number of queued cargo units in Depots: " << endl;
	outfile << "Average number of queued cargo units in Depots: " << endl;
	for (int i = 0; i < 8; i++)
	{
		//average number of queued cargo is the size of queue / number of saved times
		cout << i << " : " << queue_size_depot_[i] / amount_save_size_depot_[i] << endl;
		outfile << i << " : " << queue_size_depot_[i] / amount_save_size_depot_[i] << endl;
	}
	cout << "The average transport time for a cargo batch: " << sum_transport_time_cargo_ / amount_save_transport_time_cargo_ << endl;
	outfile << "The average transport time for a cargo batch: " << sum_transport_time_cargo_ / amount_save_transport_time_cargo_ << endl;
	//average utili. (sum of utilization / amount of trucks (12 trucks) / amount of time
	cout << "Average utilization of trucks: " << (sum_time_of_utilization_ / 12) / time << endl;
	outfile << "Average utilization of trucks: " << (sum_time_of_utilization_ / 12) / time << endl;
	outfile.close();
}

void Statistic::ResetStatistic()
{
	//I needed to reset the statistics so zero them
	for (int i = 0; i < 8; i++)
	{
		queue_size_depot_[i] = 0;
		amount_save_size_depot_[i] = 0;
	}
	sum_queuing_time_cargo_ = 0;
	amount_save_times_queuint_cargo = 0;
	queue_size_hq_ = 0;
	amount_save_size_hq_ = 0;
	sum_transport_time_cargo_ = 0;
	amount_save_transport_time_cargo_ = 0;
	sum_time_of_utilization_ = 0;
}
