#include <iostream>
#include <iomanip>
#include <thread>
#include "Gear.h"
#include "EngineCoolantTemperature.h"
#include "EngineSpeed.h"
#include "FuelConsumption.h"
#include "VehicleSpeed.h"

using namespace std;

int current_time = 0;
Gear gear;
EngineCoolantTemperature engine_coolant_temperature;
EngineSpeed engine_speed;
FuelConsumption fuel_consumption;
VehicleSpeed vehicle_speed;

void create_timer(function<void(void)> func, unsigned int interval)
{
    thread([func, interval]() {
        while (true)
        {
            func();
            this_thread::sleep_for(chrono::milliseconds(interval));
        }
    }).detach();
}


void print_state()
{
   cout << "Time: " << current_time
	    << right << setw(30) << "Fuel Consumption: " << fuel_consumption.values[current_time]
	    << right << setw(30) << "Engine Speed: " << engine_speed.values[current_time]
		<< right << setw(50) << "Engine Coolant Temperature: " << engine_coolant_temperature.values[current_time]
        << right << setw(30) << "Gear: " << gear.values[current_time]
        << right << setw(30) << "Vehicle Speed: " << vehicle_speed.values[current_time] << endl;

   current_time++;
}

int main()
{
    create_timer(print_state, 100);

    while(true);
}
