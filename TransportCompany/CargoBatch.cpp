//Wojciech Rosciszewski Wojtanowski
#include "CargoBatch.h"

//Cargo Constructor.
CargoBatch::CargoBatch(double size, int id, double generation_time)
{
  if (size < 0)
  {
    size_ = 0.1;
  }
  else if (size > 10)
  {
    size_ = 10;
  }
  else
  {
    size_ = size;
  }

  id_ = id;
  generation_time_ = generation_time;
}

//Get ID of Cargo batch.
int CargoBatch::GetId()
{
  return id_;
}

//In this module we'll get the specific size of the cargo batch and display it (according to our parameters of course)
//Size should be: following normal distribution with mean ur and variance or2 (between units 0.1 and 10).

//Get size of Cargo.
double CargoBatch::GetSize()
{
  return size_;
}

//In this module we'll get the set of time data of how long a piece of Cargo has been in a queue after generation process
//here we pull data e.g. how long the batch was in queue?

//Get queueing time of Cargo.
double CargoBatch::GetQueuingTime(double time)
{
  return time - generation_time_;
}

//In this module we'll get the time the cargo batch has been transported (travel time)

//Get transport time of Cargo.
double CargoBatch::GetTransportTime(double time)
{
  return time - generation_time_;
}
