#include <iostream>
#include <iomanip>
#include <thread>
#include <signal.h>
#include <atomic>
#include "Gear.h"
#include "EngineCoolantTemperature.h"
#include "EngineSpeed.h"
#include "FuelConsumption.h"
#include "VehicleSpeed.h"

using namespace std;

int current_time = 0;
int global_timer_period = 1;
atomic<bool> executing(true);
mutex mtx;

int current_fuel_consumption_time = 0;
int fuel_consumption_period = 1;

int current_engine_speed_time = 0;
int engine_speed_period = 1;

int current_engine_coolant_temperature_time = 0;
int engine_coolant_temperature_period = 1;

int current_gear_time = 0;
int gear_period = 1;

int current_vehicle_speed_time = 0;
int vehicle_speed_period = 1;

Gear gear;
EngineCoolantTemperature engine_coolant_temperature;
EngineSpeed engine_speed;
FuelConsumption fuel_consumption;
VehicleSpeed vehicle_speed;

void start_global_timer(function<void(void)> func, unsigned int interval)
{
    thread([func, interval]() {
        while (executing.load())
        {
            func();
            this_thread::sleep_for(chrono::seconds(interval));
        }
    }).detach();
}

void start_fuel_consumption_timer(function<void(void)> func, unsigned int interval)
{
    thread([func, interval]() {
        while (executing.load())
        {
            func();
            this_thread::sleep_for(chrono::seconds(interval));
        }
    }).detach();
}

void start_engine_speed_timer(function<void(void)> func, unsigned int interval)
{
    thread([func, interval]() {
        while (executing.load())
        {
            func();
            this_thread::sleep_for(chrono::seconds(interval));
        }
    }).detach();
}

void start_engine_coolant_timer(function<void(void)> func, unsigned int interval)
{
    thread([func, interval]() {
        while (executing.load())
        {
            func();
            this_thread::sleep_for(chrono::seconds(interval));
        }
    }).detach();
}

void start_gear_timer(function<void(void)> func, unsigned int interval)
{
    thread([func, interval]() {
        while (executing.load())
        {
            func();
            this_thread::sleep_for(chrono::seconds(interval));
        }
    }).detach();
}

void start_vehicle_speed_timer(function<void(void)> func, unsigned int interval)
{
    thread([func, interval]() {
        while (executing.load())
        {
            func();
            this_thread::sleep_for(chrono::seconds(interval));
        }
    }).detach();
}

void state_consumer()
{
	cout << "Time: " << current_time
	        << setw(20) << "\tFuel Consumption: " << fuel_consumption.values[current_fuel_consumption_time]
	        << setw(20) << "\tEngine Speed: " << engine_speed.values[current_engine_speed_time]
	        << setw(20) << "\tEngine Coolant Temperature: " << engine_coolant_temperature.values[current_engine_coolant_temperature_time]
	        << setw(20) << "\tGear: " << gear.values[current_gear_time]
	        << setw(20) << "\tVehicle Speed: " << vehicle_speed.values[current_vehicle_speed_time] << endl;

   current_time++;
}

void current_fuel_consumption_time_producer()
{
	mtx.lock();
	current_fuel_consumption_time = current_time;
	mtx.unlock();
}

void current_engine_speed_time_producer()
{
	mtx.lock();
	current_engine_speed_time = current_time;
	mtx.unlock();
}

void current_engine_coolant_temperature_time_producer()
{
	mtx.lock();
	current_engine_coolant_temperature_time = current_time;
	mtx.unlock();
}

void current_gear_time_producer()
{
	mtx.lock();
	current_gear_time = current_time;
	mtx.unlock();
}

void current_vehicle_speed_time_producer()
{
	mtx.lock();
	current_vehicle_speed_time = current_time;
	mtx.unlock();
}

void start_timers()
{
	executing = true;
	start_global_timer(state_consumer, global_timer_period);
	start_fuel_consumption_timer(current_fuel_consumption_time_producer, fuel_consumption_period);
	start_engine_speed_timer(current_engine_speed_time_producer, engine_speed_period);
	start_engine_coolant_timer(current_engine_coolant_temperature_time_producer, engine_coolant_temperature_period);
	start_gear_timer(current_gear_time_producer, gear_period);
	start_vehicle_speed_timer(current_vehicle_speed_time_producer, vehicle_speed_period);
}

void user_keyboard_input()
{
	while(cin.get() == 'u')
	{
		executing = false;
		cout << "\n\nCurrent Periods: \n\t Fuel Consumption Period:\t\t" << fuel_consumption_period
				<< "s\n\t Engine Speed Period:\t\t\t" << engine_speed_period
				<< "s\n\t Engine Coolant Temperature Period:\t" << engine_coolant_temperature_period
				<< "s\n\t Current Gear Period:\t\t\t" << gear_period
				<< "s\n\t Vehicle Speed Period:\t\t\t" << vehicle_speed_period << "s";


		int choice;
		cout << "\n\nChange Period\nEnter number choice for variable:"
				<< "\n\t(1) Fuel Consumption\n\t(2) Engine Speed (RPM)"
				<< "\n\t(3) Engine Coolant Temperature\n\t(4) Current Gear"
				<< "\n\t(5) Vehicle Speed\n\t(6) to resume execution\n\t\tChoice: ";
		cin >> choice;

		int new_period;
		switch (choice)
		{
			case 1:
				cout << "\nNew Fuel Consumption Period: ";
				cin >> new_period;
				if(!cin.fail())
				{
					fuel_consumption_period = new_period;
				}
				else
					cout << "ERROR: Not an integer. Only integer values can be entered." << endl;
				break;
			case 2:
				cout << "\nNew Engine Speed (RPM) Period: ";
				cin >> new_period;
				if(!cin.fail())
				{
					engine_speed_period = new_period;
				}
				else
					cout << "ERROR: Not an integer. Only integer values can be entered." << endl;
				break;
			case 3:
				cout << "\nNew Engine Coolant Temperature Period: ";
				cin >> new_period;
				if(!cin.fail())
				{
					engine_coolant_temperature_period = new_period;
				}
				else
					cout << "ERROR: Not an integer. Only integer values can be entered." << endl;
				break;
			case 4:
				cout << "\nNew Current Gear Period: ";
				cin >> new_period;
				if(!cin.fail())
				{
					gear_period = new_period;
				}
				else
					cout << "ERROR: Not an integer. Only integer values can be entered." << endl;
				break;
			case 5:
				cout << "\nNew Vehicle Speed Period: ";
				cin >> new_period;
				if(!cin.fail())
				{
					vehicle_speed_period = new_period;
				}
				else
					cout << "ERROR: Not an integer. Only integer values can be entered." << endl;
				break;
			case 6:
				break;
			default:
				cout << "\nInvalid input. Enter 'u' to try again." << endl;
		}
		start_timers();
	}
}


int main()
{
	cout << "Welcome to the Real-Time Vehicle Monitoring System!"
			<< "\nTo update sampling periods for different variables, enter 'u' to see the menu."
			<< "\n\nEnter any key to start the application..." << endl;

	while(!cin.get());

	start_timers();

    while(true)
    {
    	thread user_input(&user_keyboard_input);
    	user_input.join();
    }
}
