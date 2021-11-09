#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <semaphore.h>

struct CAR_VALUES
{
    double fuelConsumption;
    double engineSpeed;
    double engineCoolantTemperature;
    double currentGear;
    double vehicleSpeed;
}VALUES;

sem_t updateInterupt;
sem_t printMutex;
sem_t structAccess;
sem_t valueMutex;
sem_t sigWait;


sem_t fuelFlag;
sem_t engineSpeedFlag;
sem_t engineCoolFlag;
sem_t currentGearFlag;
sem_t speedFlag;


struct arg_struct {
    sigset_t *sigst;
    uint64_t offset;
    int period;
    int signalID;
};


struct producer_struct {
    sem_t* flag;
    int id;
};

sigset_t sigset_Fuel;
sigset_t sigset_Engine;
sigset_t sigset;
FILE* stream;

void *fuelConsumption(void *);
void *engineSpeed(void *);
void *engineCoolantTemperature(void *);
void *currentGear(void *);
void *vehicleSpeed(void *);

void *consumerThread(void *);
void *signalHandler(void *);
void *producerFunction(void *arguments);
void initalizeTimers();
void initalizeProducers();
int start_periodic_timer(void *arguments, int flag);

int main (int argc, char *argv[]) {
    pthread_t consumerpThread, timerHandlerThread;
    stream = fopen("dataset.csv", "r");
    VALUES.fuelConsumption = 0.0;
    VALUES.engineSpeed = 0.0;
    VALUES.engineCoolantTemperature =0.0;
    VALUES.currentGear = 0.0;
    VALUES.vehicleSpeed = 0.0;


    sem_init(&structAccess, 0, 1);
    sem_init(&valueMutex, 0, 1);
    sem_init(&updateInterupt, 0, 0);
    sem_init(&printMutex,0,1);
    sem_init(&sigWait,0,1);


    sem_init(&fuelFlag,0,1);
    sem_init(&engineCoolFlag,0,1);
    sem_init(& engineSpeedFlag,0,1);
    sem_init(&currentGearFlag,0,1);
    sem_init(& speedFlag,0,1);

    sigemptyset(&sigst); // initialize a signal set

    initalizeTimers();
    initalizeProducers();

    pthread_create(&timerHandlerThread, NULL, &signalHandler, NULL);

    printf("Creating the consumer producer threads.\n");
    pthread_create(&consumerpThread,NULL, &consumerThread ,NULL);

    pthread_join(&consumerpThread,NULL);
    pthread_join(&timerHandlerThread, NULL);

    pause();

    return EXIT_SUCCESS;
}

void initalizeProducers(){}
void initalizeTimers(){}
void* producerFunction(){}
void *consumerThread(){}
void *signalHandler(){}
const char* getfield(){}
int start_periodic_timer(){}


