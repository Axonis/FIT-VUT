/**
 * File:         proj2.c
 * Date:         16.04.2016
 * Author:       Jozef Urbanovský
 * E-mail:       xurban66@stud.fit.vutbr.cz
 * Project:      IOS 2. - Semaphores
 * Description:  Program solves roller coaster synchronization problem with use of semaphores.
 * Version:      1.17 finalizing
 **/
/*************************************************/

/* Used libs */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <semaphore.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <wait.h>
#include <time.h>
#include <stdbool.h>
/*************************************************/
/* Global consts */
#define E_OK 0
#define EXIT_SYSTEM_CALL 2
#define E_PARAM 10
#define E_FILE 20
#define E_SEM 30
#define E_DEL_SEM 35
#define E_SHM 40
#define E_DEL_SHM 45
#define E_FORK 50
/*************************************************/

/* Global variables */
/* (Not optimal solution in term of memory management,
 * used as a simplification of functions calling)*/

int     *passenger_id = NULL,
        *process_id = NULL,
        *car_id = NULL,
        *onboard = NULL,
        *pass_pid = NULL;

/* Shared memory */
int     shm_passenger_id = 0,
        shm_process_id = 0,
        shm_car_id = 0,
        shm_onboard = 0,
        order_id = 0,
        shm_pass_pid = 0;

/* Parameters */
struct param{
    long    max_passenger,
            max_capacity,
            max_process_time,
            max_run_time;
} p;
/* Enum for print */
enum write_id {
    car_started,
    car_load,
    car_run,
    car_unload,
    car_finished,
    passenger_started,
    passenger_board,
    passenger_board_order,
    passenger_board_last,
    passenger_unboard,
    passenger_unboard_order,
    passenger_unboard_last,
    passenger_finished,
};


/* Process ids */
int     car_pid,
        passenger_pid = -1,
        generator_pid;
int     p_time,
        c_time;

/* Semaphores */
sem_t   *xurban66_board,
        *xurban66_unboard,
        *xurban66_passenger_mutex,
        *xurban66_write_mutex,
        *xurban66_car_mutex,
        *xurban66_load,
        *xurban66_unload,
        *xurban66_finish;

/* Output file */
FILE    *output = NULL;
bool    process_time = true,
        run_time = true;

/*************************************************/
/* Function prototypes */
void parameters(int argc, char *argv[]);
void error_log(int error_number);
void help(int argc, char *argv[]);
void file(void);
void process_car(void);
void process_passenger(void);
void write_to_file(enum write_id id);
void load_resources(void);
void free_resources(void);
void terminate();

/******************* MAIN *************************/
int main (int argc, char *argv[])
{
    help(argc,argv);
    parameters(argc, argv);
    file();

    srand(time(0)); /* For RNG */

    if (process_time)
        p_time = (int)(random() % (p.max_process_time + 1));
    if (run_time)
        c_time = (int)(random() % (p.max_run_time + 1));

    signal(SIGINT, (__sighandler_t)terminate); /* Handler for captured signals */
    signal(SIGKILL, (__sighandler_t)terminate);

    load_resources();

    if ((generator_pid = fork()) == 0) { /* Main process that generates max_passenger processes */
        for (int i = 0; i < p.max_passenger; i++) {
            if (process_time)
                usleep((__useconds_t)p_time * 1000); /* Random delay between forking passengers in the interval of <0,max_process_time> */
            if ((passenger_pid = fork()) == 0) {
                break;
            }
        }
        if (passenger_pid != 0) {
            for (int i = 0; i < p.max_passenger; i++)
                waitpid(pass_pid[i], NULL, 0); /* If there are still PIDs in parent, he has to wait for them */
            sem_wait(xurban66_finish);
            exit(EXIT_SUCCESS);
        }

        if (passenger_pid < 0) { /* If forks fails program is terminated */
            error_log(E_FORK);
        }
    }

    if (passenger_pid == 0)
        process_passenger();

    if ((car_pid = fork()) == 0)
        process_car();

    waitpid(car_pid, NULL, 0);
    waitpid(generator_pid, NULL, 0);
    free_resources();


    return EXIT_SUCCESS;
}


/*************************************************/
/**
 * Function iterates over max_passenger/max_capacity times,
 * loading and boarding max_capacity passengers each ride.
 * Car_mutex ensures that passengers wait up until
 * every passenger finishes its critical section, then car
 * begins to unload and unboard passengers. After rides,
 * car has to wait for other processes to finish.
**/
/******************* PROCESS_CAR *************************/
void process_car(void) {

    *car_id = 1;
    write_to_file(car_started);
    for (int i = 0; i < p.max_passenger/p.max_capacity; i++) { /* Car is going to ride exact number of times */
        write_to_file(car_load);

        sem_wait(xurban66_car_mutex); /* Car starts to load processes, up until max capacity */

        for (int i = 0; i < p.max_capacity; i++)
            sem_post(xurban66_load);

        sem_post(xurban66_car_mutex);
        sem_wait(xurban66_board); /* Car is waiting up until all passengers are aboard */

        sem_wait(xurban66_car_mutex);
        write_to_file(car_run); /* When car is full, car operation run begins */
        sem_post(xurban66_car_mutex);

        if (run_time)
            usleep((__useconds_t)c_time * 1000); /* Random delay in the interval of <0,max_run_time>*/

        write_to_file(car_unload); /* After ride, car beings unload */

        for (int i = 0; i < p.max_capacity; i++)
            sem_post(xurban66_unload);

        sem_wait(xurban66_unboard); /* Car has to unload all passengers before going on another ride */
    }

    sem_post(xurban66_finish);
    // After all rides are over, car is waiting for processes to finish
    waitpid(generator_pid, NULL, 0);
    write_to_file(car_finished);
    free_resources();
    exit(EXIT_SUCCESS);
}


/***************************************************************/
/**
 * Function has to store each passenger pid to shared array of int.
 * Order_id is used as a local id for passengers on a same ride.
 * Passengers are waiting in queue, before being loaded to car.
 * After loading, they start to board a car up until they fill
 * whole car. Ride starts,each process does his critical section
 * and waits for others to unload and unboard.
**/
/******************* PROCESS_PASSENGER *************************/
void process_passenger(void) {

    sem_wait(xurban66_passenger_mutex);
    pass_pid[*passenger_id] = getpid(); /* Stores passenger pids to shared array of int */

    (*passenger_id)++; /* Each passenger has to have unique id */
    order_id = *passenger_id; /* For each ride there is unique order of passengers */

    write_to_file(passenger_started);
    sem_post(xurban66_passenger_mutex);

    sem_wait(xurban66_load); /* Passenger is waiting in queue for car signal to start loading */
    sem_wait(xurban66_passenger_mutex);/* If there is still free space in car, another passenger can aboard it */

    if (*onboard <= p.max_capacity) {
        write_to_file(passenger_board);
        (*onboard)++; /* Passengers are boarding car */
        if (*onboard == p.max_capacity) { /* Up until last passenger boards */
            write_to_file(passenger_board_last);
            sem_post(xurban66_board); /* It signalizes that boarding is over */
        } else {
            write_to_file(passenger_board_order); /* If there are process in queue waiting to board, they are sorted ascending */
        }
        sem_post(xurban66_passenger_mutex);
        sem_wait(xurban66_unload);
    }

    sem_wait(xurban66_passenger_mutex); /* After the ride, operation unboard begins */
    if (*onboard > 0) {
        write_to_file(passenger_unboard);
        if (*onboard == 1) { /* If there is last passenger aboard he signalizes it */
            write_to_file(passenger_unboard_last);
            sem_post(xurban66_unboard);
        } else {
            write_to_file(passenger_unboard_order); /* Otherwise passengers are leaving car in same order */
        }
        (*onboard)--; /* Passengers are leaving car */
    }
    sem_post(xurban66_passenger_mutex);
    /* Each passenger after ride has to wait for others to finish */
    sem_wait(xurban66_finish);

    write_to_file(passenger_finished);
    sem_post(xurban66_finish);
    free_resources();
    exit(EXIT_SUCCESS);
}



/***********************************************************/
/**
 * Function prints out to a file statement depending on enum id
**/
/******************* WRITE_TO_FILE *************************/
void write_to_file(enum write_id id) {
    int tmp = (int)p.max_capacity - *onboard + 1;

    sem_wait(xurban66_write_mutex);
    (*process_id)++;
    switch (id) {
        case car_started: fprintf(output, "%d: C %d: started\n", *process_id, *car_id); break;
        case car_load: fprintf(output, "%d: C %d: load\n", *process_id, *car_id); break;
        case car_run: fprintf(output, "%d: C %d: run\n", *process_id, *car_id); break;
        case car_unload: fprintf(output, "%d: C %d: unload\n", *process_id, *car_id); break;
        case car_finished: fprintf(output, "%d: C %d: finished\n", *process_id, *car_id); break;
        case passenger_started: fprintf(output, "%d: P %d: started\n", *process_id, order_id); break;
        case passenger_board: fprintf(output, "%d: P %d: board\n", *process_id, order_id); break;
        case passenger_board_order: fprintf(output, "%d: P %d: board order %d\n", *process_id, order_id, *onboard); break;
        case passenger_board_last: fprintf(output, "%d: P %d: board last\n", *process_id, order_id); break;
        case passenger_unboard: fprintf(output, "%d: P %d: unboard\n", *process_id, order_id); break;
        case passenger_unboard_order: fprintf(output, "%d: P %d: unboard order %d\n", *process_id, order_id, tmp); break;
        case passenger_unboard_last: fprintf(output, "%d: P %d: unboard last\n", *process_id, order_id); break;
        case passenger_finished: fprintf(output, "%d: P %d: finished\n", *process_id, order_id); break;
    }
    tmp = 0;
    fflush(output);
    sem_post(xurban66_write_mutex);

}


/************************************************************/
/**
 * Function allocates memory for semaphores and shared memory, if system call fails error number is set accordingly
 * and initialization is aborted.
**/
/******************* LOAD_RESOURCES *************************/
void load_resources(void) {
    int error = E_OK;
    // -------------------- Init of semaphores -----------------------

    if((xurban66_board = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED ||
       (xurban66_write_mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED ||
       (xurban66_car_mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED ||
       (xurban66_passenger_mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED ||
       (xurban66_load = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED ||
       (xurban66_unload = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED ||
       (xurban66_finish = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED ||
       (xurban66_unboard = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED ||
       sem_init(xurban66_board, 1, 0) == -1 ||
       sem_init(xurban66_write_mutex, 1, 1) == -1 ||
       sem_init(xurban66_car_mutex, 1, 1) == -1 ||
       sem_init(xurban66_passenger_mutex, 1, 1) == -1 ||
       sem_init(xurban66_load, 1, 0) == -1 ||
       sem_init(xurban66_unload, 1, 0) == -1 ||
       sem_init(xurban66_finish, 1, 0) == -1 ||
       sem_init(xurban66_unboard, 1, 0) == -1)
        error = E_SEM;

        // -------------------- Init of shared memory -----------------------

    else if((shm_passenger_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||
            (shm_process_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||
            (shm_car_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||
            (shm_onboard = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||
            (shm_pass_pid = shmget(IPC_PRIVATE, sizeof(int)*p.max_passenger, IPC_CREAT | IPC_EXCL | 0666)) == -1 ||
            (passenger_id = (int *) shmat(shm_passenger_id, NULL, 0)) == NULL ||
            (process_id = (int *) shmat(shm_process_id, NULL, 0)) == NULL ||
            (car_id = (int *) shmat(shm_car_id, NULL, 0)) == NULL ||
            (onboard = (int *) shmat(shm_onboard, NULL, 0)) == NULL ||
            (pass_pid = (int *) shmat(shm_pass_pid, NULL, 0)) == NULL)
        error = E_SHM;
    // -------------------- End of init -----------------

    if (error != E_OK) {
        if (error == E_SEM)
            error_log(E_SEM);
        else
            error_log(E_SHM);
    }
}


/************************************************************/
/**
 * Function destroys semaphores and frees allocated shared memory, if system call fails,
 * error number is set accordingly and program is killed
**/
/******************* FREE_RESOURCES *************************/
void free_resources() {
    int error = E_OK;

    fclose(output);

    if(sem_destroy(xurban66_board) == -1 ||
       sem_destroy(xurban66_write_mutex) == -1 ||
       sem_destroy(xurban66_car_mutex) == -1 ||
       sem_destroy(xurban66_passenger_mutex) == -1 ||
       sem_destroy(xurban66_load) == -1 ||
       sem_destroy(xurban66_unload) == -1 ||
       sem_destroy(xurban66_finish) == -1 ||
       sem_destroy(xurban66_unboard) == -1) {
        error = E_DEL_SEM;
    }

    if(error == E_OK)
    {
        if(shmctl(shm_passenger_id, IPC_RMID, NULL) == -1 ||
           shmctl(shm_process_id, IPC_RMID, NULL) == -1 ||
           shmctl(shm_car_id, IPC_RMID, NULL) == -1 ||
           shmctl(shm_onboard, IPC_RMID, NULL) == -1 ||
           shmctl(shm_pass_pid, IPC_RMID, NULL) == -1 ||
           shmdt(passenger_id) == -1 ||
           shmdt(process_id) == -1 ||
           shmdt(car_id) == -1 ||
           shmdt(onboard) == -1 ||
           shmdt(pass_pid) == -1) {
            error = E_DEL_SHM;
        }
    }

    if(error != E_OK)
    {
        if(error == E_DEL_SEM)
            error_log(E_DEL_SEM);
        else
            error_log(E_DEL_SHM);
    }
}

/*******************************************************/
/**
 * Function that safely terminates program
**/
/******************* TERMINATE *************************/
void terminate()
{
    free_resources();
    kill(generator_pid, SIGTERM);
    kill(car_pid, SIGTERM);
    exit(EXIT_FAILURE);
}

/**************************************************/
/**
 * Function that prepares output file for write
**/
/******************* FILE *************************/
void file (void){
    if ((output = fopen("proj2.out", "w")) == NULL)
        error_log(E_FILE);
    else
        setbuf(output, NULL);
}

/*********************************************************/
/**
* Function for processing parameters from input. Valid input is
* P - number of processes representing passenger, P>0
* C - capacity of car, C > 0, P > C, P has to be multiple of C.
* PT - maximum duration (in ms), in which new process for passenger is generated, PT >= 0 && PT < 5001.
* RT - maximum duration (in ms) of ride, RT >= 0 && RT < 5001.
* Each parameter is integer value.
**/
/******************* PARAMETERS *************************/
void parameters (int argc, char *argv[])
{

    char *error = NULL;
    int e_num = E_OK;

    if (argc != 5) /* Number of parameters = 5 (4 parameters + 1 filename) */
        error_log(E_PARAM);

    else {
        if (isdigit(*argv[1]))
            p.max_passenger=strtol(argv[1], &error, 10);
        if (p.max_passenger <= 0)
            e_num = E_PARAM;

        if (isdigit(*argv[2]))
            p.max_capacity=strtol(argv[2], &error, 10);
        if (p.max_capacity <= 0 || p.max_passenger < p.max_capacity || (p.max_passenger % p.max_capacity != 0))
            e_num = E_PARAM;

        if (isdigit(*argv[3]))
            p.max_process_time=strtol(argv[3], &error, 10);
        if (p.max_process_time < 0 || p.max_process_time > 5001)
            e_num = E_PARAM;
        if (p.max_process_time == 0)
            process_time = false;

        if (isdigit(*argv[4]))
            p.max_run_time=strtol(argv[4], &error, 10);
        if (p.max_run_time < 0 || p.max_run_time > 5001)
            e_num = E_PARAM;
        if (p.max_run_time == 0)
            run_time = false;
    }
    if (strcmp(error, "")) /* If there was a problem with converting input values to integers program is terminated. */
        error_log(E_PARAM);
    if (e_num != E_OK) /* If false arguments are found e_num is set to != 0*/
        error_log(E_PARAM);
}

/*******************************************************/
/**
* Function prints our error log depending on error_number and exits
* program with error. Before calling, allocated memory has to be freed.
**/
/******************* ERROR_LOG *************************/

void error_log (int error_number)
{
    switch (error_number){
        case E_PARAM:
            fprintf(stderr, "Invalid arguments\n"); break;

        case E_FILE:
            fprintf(stderr, "Failed to open file for write\n"); break;

        case E_SEM:
            fprintf(stderr, "Failed to create semaphores\n"); break;

        case E_DEL_SEM:
            fprintf(stderr, "Failed to destroy semaphores\n"); break;

        case E_SHM:
            fprintf(stderr, "Failed to allocate shared memory\n"); break;

        case E_DEL_SHM:
            fprintf(stderr, "Failed to free allocated shared memory\n"); break;

        case E_FORK:
            fprintf(stderr, "Failed to fork process\n"); break;


        default: fprintf(stderr, "Unexpected Error\n");
    }

    fprintf(stderr,"For help start a program with parameter \"-h\"\n");
    if (error_number == E_PARAM)
        exit(EXIT_FAILURE);
    else{
        free_resources();
        exit(EXIT_SYSTEM_CALL);
    }

}

/**************************************************/
/**
* Function prints out manual to program
**/
/******************* HELP *************************/

void help(int argc, char *argv[])
{
    if (argc == 2 && (strcmp("-h", argv[1])==0)) {
        printf("Author: Jozef Urbanovský\n"
                       "Program: solution of synchronization "
                       "problem called : \"Roller Coaster\" with semaphores.\n"
                       "How to execute program: ./proj2 P C PT RT\n"
                       "P - number of processes representing passenger, P>0\n"
                       "C - capacity of car, C > 0, P > C, P has to be multiple of C\n"
                       "PT - maximum duration (in ms), in which new process for passenger is generated, PT >= 0 && PT < 5001.\n"
                       "RT - maximum duration (in ms) of ride, RT >= 0 && RT < 5001.\n"
                       "Example: ./proj2 4 2 0 0\n"
                       "Program executes with 4 passengers, car capacity of 2, without any delay\n");
        exit(EXIT_SUCCESS);
    }
}