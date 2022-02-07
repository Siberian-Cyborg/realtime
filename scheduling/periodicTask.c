#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "errno.h"
#include <stdlib.h>
#include <fcntl.h>  /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <sched.h>
#include <string.h>


extern int errno;
float period_ = 5;
float CPUload_ = 10;
timer_t timer;


//cather is only called if we missed a signal, meaning we were still working on the load
void catcher(int sig){
	printf("Missed Deadline!\n");
}

// the timer will periodically send a sigalam
void init_timer(long period){
	
	
	struct itimerspec timer_spec;
	
	// Create a timer.
	timer_create(CLOCK_REALTIME, NULL, &timer); //NULL means it will send SIGALRM
	
	// Specify the timer's period.
	timer_spec.it_value.tv_sec= 0;	/* Start immediately if here is a 0*/
	timer_spec.it_value.tv_nsec= 1; /* Start immediately if here is a 1*/
	timer_spec.it_interval.tv_sec= 0;
	timer_spec.it_interval.tv_nsec= period*1000000; //if period is 500 that means its 0,5 seconds
		
	// Setup the timer's period.
	timer_settime(timer, 0, &timer_spec, NULL);
}


void genericLoad(){
	//sleep(2);
	for(int i =0; i<CPUload_*1000; i++){
		//just a generic laod
	}	
}

int main(int argc, char *argv[])
{
	//parsing command line input
	period_ = atof(argv[1]);
	float priority = atof(argv[2]);
	CPUload_ = atof(argv[3]);
	int sched = SCHED_FIFO;
	if(argc == 5){
		if(!strcmp(argv[4], "FIFO")){
			sched = SCHED_FIFO;
			printf("FIFO scheduling is used\n");
		}else if (!strcmp(argv[4], "RR")){
			sched = SCHED_RR;
			printf("RR scheduling is used\n");
		}
	}else{
		printf("default FIFO scheduling is used\n");
	}

	
	int result = 1;
	struct timespec tstart={0,0}, tend={0,0};
	struct itimerspec currentTime={0,0};
	sigset_t waitset;
	int signum;
	sigemptyset(&waitset);
	sigaddset(&waitset, SIGALRM); // add sigalarm to the waitset

	//sets schedule
	struct sched_param spar;
	spar.sched_priority = priority;
	sched_setscheduler(getpid(), sched, &spar);

	// link the catcher function to sigalarm
	// if sigwait is not blocking, catcher will be called upon arrival of a sigalarm
	signal(SIGALRM, catcher); 

	//creates periodic signal
	init_timer(period_);

	while(1){
		//sigwait returns 0 if it recieves a signal defined in waitset
		while(result){
			result = sigwait(&waitset, &signum);
		}
		//this code is only executed if we recieved a sigalam
		//while this code is executed, sigwait is not waiting for sigalarm
		//instead sigalarm is handeled by the catcher function
		timer_gettime(timer, &currentTime);
		//clock_gettime(CLOCK_MONOTONIC_RAW, &tstart);

		genericLoad();

		//clock_gettime(CLOCK_MONOTONIC_RAW, &tend);
		//double time_diff = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
		double ttt = currentTime.it_interval.tv_nsec;
		double time_diff = (ttt / 1000000000) - (currentTime.it_value.tv_sec + 1.0e-9*currentTime.it_value.tv_nsec); // currentTime.it_value the time remaining until the next timer expiration
		if(time_diff*1000<period_)
			printf("the comupation took %.5f seconds\n", time_diff);

		result = 1;
	}
	return EXIT_SUCCESS;
}


