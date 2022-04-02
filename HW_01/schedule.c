/* schedule.c
 * This file contains the primary logic for the 
 * scheduler.
 */
#include "privatestructs.h"
#include "schedule.h"
#include "macros.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define NEWTASKSLICE (NS_TO_JIFFIES(100000000))
#define a 0.5

/* Local Globals
 * rq - This is a pointer to the runqueue that the scheduler uses.
 * current - A pointer to the current running task.
 */
struct runqueue *rq;
struct task_struct *current;

/* External Globals
 * jiffies - A discrete unit of time used for scheduling.
 *			 There are HZ jiffies in a second, (HZ is 
 *			 declared in macros.h), and is usually
 *			 1 or 10 milliseconds.
 */
extern long long jiffies;
extern struct task_struct *idle;

/*-----------------Initilization/Shutdown Code-------------------*/
/* This code is not used by the scheduler, but by the virtual machine
 * to setup and destroy the scheduler cleanly.
 */
 
 /* initscheduler
  * Sets up and allocates memory for the scheduler, as well
  * as sets initial values. This function should also
  * set the initial effective priority for the "seed" task 
  * and enqueu it in the scheduler.
  * INPUT:
  * newrq - A pointer to an allocated rq to assign to your
  *			local rq.
  * seedTask - A pointer to a task to seed the scheduler and start
  * the simulation.
  */
void initschedule(struct runqueue *newrq, struct task_struct *seedTask)
{
	seedTask->next = seedTask->prev = seedTask;
	newrq->head = seedTask;
	newrq->nr_running++;

	//Initialization of init task
	seedTask->Burst = 0;
	seedTask->Exp_Burst = 0;
	seedTask->entering_cpu = 0;
	seedTask->entering_rq = 0;
	seedTask->goodness = 0;
	seedTask->standard_goodness = 0;
	seedTask->waiting_inRQ = 0;
}

/* killschedule
 * This function should free any memory that 
 * was allocated when setting up the runqueu.
 * It SHOULD NOT free the runqueue itself.
 */
void killschedule()
//We didnt allocate memory, so killschedule wont free something.
{
	return;
}


/* print_rq
 * This function runs the rq list and
 * prints all the tasks(name).
 */
void print_rq () {
	struct task_struct *curr;
	
	printf("Rq: \n");
	curr = rq->head;
	if (curr) {
		printf("%s", curr->thread_info->processName);
	}
	while(curr->next != rq->head) {
		curr = curr->next;
		printf(", %s", curr->thread_info->processName);
	};
	printf("\n");
}

/*-------------Scheduler Code Goes Below------------*/
/* This is the beginning of the actual scheduling logic */

/* schedule
 * Gets the next task in the queue
 */
void schedule()
{
	//static struct task_struct *nxt = NULL;
	struct task_struct *next_p;
	unsigned long long tmp_Burst, tmp_Exp_Burst;
	
	
	printf("In schedule\n");
	print_rq();
	
	if (current->need_reschedule ==  1) {
		current->time_slice = 100;
		current->need_reschedule = 0;
	}



	if (rq->nr_running == 1) { //If we wad only init.
		context_switch(rq->head);
		//nxt = rq->head->next;
	}	
	else {
		tmp_Burst = current->Burst;
		tmp_Exp_Burst = current->Exp_Burst;
		current->Burst = sched_clock() - current->entering_cpu;
		current->Exp_Burst = (current->Burst + a * (current->Exp_Burst)) / (1 + a);
		
		//1. minimum_Exp_Burst method. Uncomment for usage
		//next_p = minimum_Exp_Burst();

		//2. Standard Goodness Method. Uncomment for usage
		//next_p = min_standard_Goodness();

		//3. Goodness Method. Uncomment for usage
		next_p = min_Goodness();
		
		//Each time we print the the next_p pointer, which is 
		//a pointer to the minimum_task (== task which gets the biggest priority
		
		if (next_p != current) {
			//Current entering in RQ list
			current->entering_rq = sched_clock();  
			next_p->entering_cpu = sched_clock();
			next_p->waiting_inRQ = 0;
			context_switch(next_p);
		} else {
			//If next_p == current, saving the old values that we calculated before
			current->Burst = tmp_Burst;
			current->Exp_Burst = tmp_Exp_Burst;
			context_switch(current);
		}
	}
}


/* minimum_Exp_Burst: This function
 * runs the rq list , and finds the task
 * with minimum Exp_Burst. 
 * Return value: the task with minimum Exp_Burst.
 */
struct task_struct *minimum_Exp_Burst(void) {

	unsigned long long min_time;
	struct task_struct *curr, *min_p;

	min_time = current->Exp_Burst;
	min_p = rq->head->next;

	for (curr = rq->head->next; curr != rq->head; curr = curr->next) {
		if (min_time > curr->Exp_Burst) {
			min_time = curr->Exp_Burst;
			min_p = curr;
		}
	}
	return(min_p);
}


/* maximum_waiting_rq: This function
 * runs the rq list, calculates and saves the waiting_inRQ time 
 * and then, finds the task with maximum one.
 * Return value: the struct with the maximum waiting_inRQ time.
 */
struct task_struct *maximum_waiting_rq(void) {

	unsigned long long max_time_inRQ;
	struct task_struct *curr, *max_p;

	max_p = rq->head->next;
	max_time_inRQ = max_p->waiting_inRQ;


	for (curr = rq->head->next; curr != rq->head; curr = curr->next) {
		if (curr != current) { //Beacuse current is the task which is running, it is not in rq.

			curr->waiting_inRQ = sched_clock() - curr->entering_rq;
			if (max_time_inRQ < curr->waiting_inRQ) {
				max_time_inRQ = curr->waiting_inRQ;
				max_p = curr;
			}
		}
	}

	return(max_p);
}


/* min_Goodness: This function calculates for each task
 * the goodness, then finds the one with minimum goodness.
 * Return value: the struct with minimum goodness
 */
struct task_struct *min_Goodness(void) {

	struct task_struct * min_Exp_Burst, *max_waiting_rq, *curr, *min_goodness_task;
	double min_goodness_value;

	min_Exp_Burst = minimum_Exp_Burst(); //Get min_Exp_Burst, necessary for goodness's calculation
	max_waiting_rq = maximum_waiting_rq(); //same

	curr = rq->head->next;
	min_goodness_task = curr;
	
	//Type cast needed: goodness is double, and the other are unsigned long long. 
	//Without type casting, we get wrong result.
	curr->goodness = ((1 + (double)curr->Exp_Burst) / (1 + (double)min_Exp_Burst->Exp_Burst)) * (((double)max_waiting_rq->waiting_inRQ + 1) / (1 + (double)curr->waiting_inRQ));
	min_goodness_value = curr->goodness;

	printf("P: %s, Goodness: %lf\n",min_goodness_task->thread_info->processName,min_goodness_value);
	for (curr = curr->next; curr != rq->head; curr = curr->next) {
		curr->goodness = ((1 + (double)curr->Exp_Burst) / (1 + (double)min_Exp_Burst->Exp_Burst)) * (((double)max_waiting_rq->waiting_inRQ + 1) / (1 + (double)curr->waiting_inRQ));
		printf("P: %s, Goodness: %lf\n",curr->thread_info->processName,curr->goodness);
		if (min_goodness_value > curr->goodness) {
			min_goodness_value = curr->goodness;
			min_goodness_task = curr;
		}
	}
	printf("MIN (P): %s, Goodness: %lf\n", min_goodness_task->thread_info->processName, min_goodness_task->goodness);
	return(min_goodness_task);

}

/* min_standard_Goodness: Same method, just like min_Goodness */
struct task_struct *min_standard_Goodness(void) {

	struct task_struct *min_Exp_Burst, *curr, *min_standard_goodness_task;
	double min_standard_goodness_value;

	min_Exp_Burst = minimum_Exp_Burst(); //Necessary for standard_Goodness's calculation
	
	curr = rq->head->next;
	min_standard_goodness_task = curr;
	curr->standard_goodness = ((1 + (double)curr->Exp_Burst) / (1 + (double)min_Exp_Burst->Exp_Burst));
	min_standard_goodness_value = curr->standard_goodness;

	printf("P: %s, S_Goodness: %lf\n", min_standard_goodness_task->thread_info->processName, min_standard_goodness_value);
	for (curr = curr->next; curr != rq->head; curr = curr->next) {
		curr->standard_goodness = ((1 + (double)curr->Exp_Burst) / (1 + (double)min_Exp_Burst->Exp_Burst));
		printf("P: %s, S_Goodness: %lf\n", curr->thread_info->processName, curr->standard_goodness);
		if (min_standard_goodness_value > curr->standard_goodness) {
			min_standard_goodness_value = curr->standard_goodness;
			min_standard_goodness_task = curr;
		}
	}
	printf("MIN (P): %s, S_Goodness: %lf\n", min_standard_goodness_task->thread_info->processName, min_standard_goodness_task->standard_goodness);
	return(min_standard_goodness_task);

}


/* sched_fork
 * Sets up schedule info for a newly forked task
 */
void sched_fork(struct task_struct *p)
{
	p->time_slice = 0;
	p->Exp_Burst = 0;
	p->Burst = 0;
	p->goodness = 0;
	p->standard_goodness = 0;
	p->entering_cpu = 0;
	p->entering_rq = 0;
	p->waiting_inRQ = 0;
}

/* scheduler_tick
 * Updates information and priority
 * for the task that is currently running.
 */
void scheduler_tick(struct task_struct *p)
{

	if (rq->nr_running == 1) {
		schedule();
	}
	else {
		p->time_slice--;
		if (p->time_slice <= 0) {
			p->need_reschedule = 1;
		}
	schedule();
	}
}

/* wake_up_new_task
 * Prepares information for a task
 * that is waking up for the first time
 * (being created).
 */
void wake_up_new_task(struct task_struct *p)
{	
	p->next = rq->head->next;
	p->prev = rq->head;
	p->next->prev = p;
	p->prev->next = p;
	p->entering_rq = sched_clock();
	rq->nr_running++;
}

/* activate_task
 * Activates a task that is being woken-up
 * from sleeping.
 */
void activate_task(struct task_struct *p)
{
	p->next = rq->head->next;
	p->prev = rq->head;
	p->next->prev = p;
	p->prev->next = p;
	p->entering_rq = sched_clock();
	rq->nr_running++;
}

/* deactivate_task
 * Removes a running task from the scheduler to
 * put it to sleep.
 */
void deactivate_task(struct task_struct *p)
{
	p->prev->next = p->next;
	p->next->prev = p->prev;
	p->next = p->prev = NULL; /* Make sure to set them to NULL *
							   * next is checked in cpu.c      */

	rq->nr_running--;
}
