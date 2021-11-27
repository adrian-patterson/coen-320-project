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
int engine_coolant_temperature_period = 0;

int current_gear_time = 0;
int gear_period = 0;

int current_vehicle_speed_time = 0;
int vehicle_speed_period = 0;

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
        while (executing)
        {
            func();
            this_thread::sleep_for(chrono::seconds(interval));
        }
    }).detach();
}

void start_engine_coolant_timer(function<void(void)> func, unsigned int interval)
{
    thread([func, interval]() {
        while (executing)
        {
            func();
            this_thread::sleep_for(chrono::seconds(interval));
        }
    }).detach();
}

void start_gear_timer(function<void(void)> func, unsigned int interval)
{
    thread([func, interval]() {
        while (executing)
        {
            func();
            this_thread::sleep_for(chrono::seconds(interval));
        }
    }).detach();
}

void start_vehicle_speed_timer(function<void(void)> func, unsigned int interval)
{
    thread([func, interval]() {
        while (executing)
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
		string choice;
		executing = false;
		cout << "Enter choice for changing period: " << endl;
		cin >> choice;
		start_timers();
	}
}

int main()
{
	start_timers();

    while(true)
    {
    	thread user_input(&user_keyboard_input);
    	user_input.join();
    }
}
