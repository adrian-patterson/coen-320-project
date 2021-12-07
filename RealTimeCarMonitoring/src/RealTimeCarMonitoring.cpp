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

/* Global Variables: Current time, global timer period, a mutex lock, and  an atomic boolean */
int t = 0;
int global_timer_period = 1;
atomic<bool> executing(true);
mutex mtx;

/* Values for current time and sampling period for each variable */
float fuel_consumption_value= 0;
int fuel_consumption_period = 5;

int engine_speed_value= 0;
int engine_speed_period = 5;

int engine_coolant_temperature_value= 0;
int engine_coolant_temperature_period = 5;

int gear_value= 0;
int gear_period = 5;

int vehicle_speed_value= 0;
int vehicle_speed_period = 5;

/* Importing arrays of data as objects */
Gear gear;
EngineCoolantTemperature engine_coolant_temperature;
EngineSpeed engine_speed;
FuelConsumption fuel_consumption;
VehicleSpeed vehicle_speed;

/* Function to generate a timer to run periodically */
void start_timer(function<void(void)> producer_function, unsigned int period)
{
    thread([producer_function, period]()
	{
        while (executing.load())
        {
        	producer_function();
            this_thread::sleep_for(chrono::seconds(period));
        }
    }).detach();
}

/* State Consumer: Consumes all data produced about the 5 variables and displays it */
void state_consumer()
{
	cout << "Time: " << t
	        << setw(20) << "\tFuel Consumption: " << setprecision(1) << fixed << fuel_consumption_value
	        << setw(20) << "\tEngine Speed: " << engine_speed_value
	        << setw(20) << "\tEngine Coolant Temperature: " << engine_coolant_temperature_value
	        << setw(20) << "\tGear: " << gear_value
	        << setw(20) << "\tVehicle Speed: " << vehicle_speed_value << endl;

   t++;
}

/* Variable producer functions: Called back periodically. A mutex locks the variable so they may be updated for the consumer */
void fuel_consumption_value_producer()
{
	mtx.lock();
	fuel_consumption_value = fuel_consumption.values[t];
	mtx.unlock();
}

void engine_speed_value_producer()
{
	mtx.lock();
	engine_speed_value = engine_speed.values[t];
	mtx.unlock();
}

void engine_coolant_temperature_value_producer()
{
	mtx.lock();
	engine_coolant_temperature_value = engine_coolant_temperature.values[t];
	mtx.unlock();
}

void gear_value_producer()
{
	mtx.lock();
	gear_value = gear.values[t];
	mtx.unlock();
}

void vehicle_speed_value_producer()
{
	mtx.lock();
	vehicle_speed_value = vehicle_speed.values[t];
	mtx.unlock();
}

/* Function to initialize and start all timers */
void start_timers()
{
	executing = true;
	start_timer(fuel_consumption_value_producer, fuel_consumption_period);
	start_timer(engine_speed_value_producer, engine_speed_period);
	start_timer(engine_coolant_temperature_value_producer, engine_coolant_temperature_period);
	start_timer(gear_value_producer, gear_period);
	start_timer(vehicle_speed_value_producer, vehicle_speed_period);
	start_timer(state_consumer, global_timer_period);
}

/* Function to implement user interface. Allows a user to change the period */
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


		string choice;
		cin.ignore();
		cout << "\n\nChange Period\nEnter number choice for variable:"
				<< "\n\tFuel Consumption\n\tEngine Speed"
				<< "\n\tEngine Coolant Temperature\n\tCurrent Gear"
				<< "\n\tVehicle Speed\n\tEnter to resume execution\n\t\tChoice: ";
		getline(cin, choice);
		if(!choice.empty())
		{
			int new_period;

			if(choice == "fuel consumption" || choice == "Fuel Consumption")
			{
				cout << "\t\tNew Fuel Consumption Period: ";
				cin >> new_period;
				if(!cin.fail())
				{
					fuel_consumption_period = new_period;
				}
				else
					cout << "ERROR: Not an integer. Only integer values can be entered." << endl;
			}
			else if(choice == "engine speed" || choice == "Engine Speed")
			{
				cout << "\t\tNew Engine Speed (RPM) Period: ";
				cin >> new_period;
				if(!cin.fail())
				{
					engine_speed_period = new_period;
				}
				else
					cout << "ERROR: Not an integer. Only integer values can be entered." << endl;
			}
			else if(choice == "engine coolant temperature" || choice == "Engine Coolant Temperature")
			{
				cout << "\t\tNew Engine Coolant Temperature Period: ";
				cin >> new_period;
				if(!cin.fail())
				{
					engine_coolant_temperature_period = new_period;
				}
				else
					cout << "ERROR: Not an integer. Only integer values can be entered." << endl;
			}
			else if(choice == "current gear" || choice == "Current Gear")
			{
				cout << "\t\tNew Current Gear Period: ";
				cin >> new_period;
				if(!cin.fail())
				{
					gear_period = new_period;
				}
				else
					cout << "ERROR: Not an integer. Only integer values can be entered." << endl;
			}
			else if(choice == "vehicle speed" || choice == "Vehicle Speed")
			{
					cout << "\t\tNew Vehicle Speed Period: ";
					cin >> new_period;
					if(!cin.fail())
					{
						vehicle_speed_period = new_period;
					}
					else
						cout << "ERROR: Not an integer. Only integer values can be entered." << endl;
			}
			else if(choice == "")
			{
				break;
			}
			else
			{
					cout << "\nInvalid input. Enter 'u' to try again.\n" << endl;
			}
		}

		cout << endl;
		start_timers();
	}
}


/* Main function. Starts timers and loops waiting for user input */
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
