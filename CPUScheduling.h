#pragma once
#include <string>
#include <vector>
#include <iostream>
using namespace std;
class PCB {
private:
	int n_processors;
	string id;
	int arrivalTime;
	int startTime;
	int endTime;
	int burstTime=-1;
	string type;
public:
	PCB();
	PCB(int burst, string type){this->burstTime = burst; this->type = type;};
	int getArrivalTime();
	virtual int getBurstTime(){return this->burstTime;};
	void setBurstTime(int bt){this->burstTime = bt;};
	int getStartTime();
	void setStartTime(int);
	int getEndTime();
	void setEndTime(int);
	string getId();
	string getType(){return type;};
};

class RecurringPCB:virtual public PCB{
private:
	int sleepTime;
public:
	RecurringPCB();
	RecurringPCB(int bt, int sleepTime, string type);
	int getSleepTime(){return sleepTime;};
	void setSleepTime(int t){sleepTime = t;};
	virtual int getBurstTime(){return this->getBurstTime();};
	
};

class NonRecurringPCB:virtual public PCB{
private:
	int timeToRemain;
public:
	NonRecurringPCB();
	NonRecurringPCB(int bt, int timeToRemain, string type);
	int getTimeToRemain(){return timeToRemain;};
	void setTimeToRemain(int t){timeToRemain = t;};
	virtual int getBurstTime(){return this->getBurstTime();};
};

class FCFS:public RecurringPCB, public NonRecurringPCB{
private:
	bool isStarving;
	int burstTime;
public:
	void upgradeJob();
	int getBurstTime(){return burstTime;};
	FCFS(string r, int bt, int time);
};

class SJF:public RecurringPCB, public NonRecurringPCB{
private:
	bool isEmpty;
	bool isStarving;
	int burstTime;
public:
	void upgradeJob();
	int getBurstTime(){return burstTime;};
	SJF(string r, int bt, int time);
};

class priorityQ:public RecurringPCB, public NonRecurringPCB{
private:
	int burstTime;
	bool isEmpty;
	int priority;
public:
	priorityQ(string r, int bt, int time);
	int getBurstTime(){return burstTime;};
	int getPriority(){return priority;};
};


class Partition { 
	
	PCB* pb;  
	int CPU_num; 
	int burst;
	Partition *nextp; 
	
public: 
	
	// all needed accessor functions 
	Partition(PCB* pb1, int CPU_num, Partition* nextp){
		//this->pb->setBurstTime(pb->getBurstTime());
		this->pb = pb1;
		pb->setBurstTime(pb1->getBurstTime());
		this->CPU_num = CPU_num;
		this->nextp = nextp;
		
	}
	int getCPU_num(){
		return this->CPU_num;
	}
	void setCPU_num(int CPU_num){
		this->CPU_num = CPU_num;
	}
	int getBurst(){
		this->burst = pb->getBurstTime();
		return this->burst;
	}
	string getType(){
		return this->pb->getType();
	}
	
	void setNext(Partition* nextp){
		this->nextp = nextp;
	}
	
	Partition* getNext(){
		return this->nextp;
	}
	
}; 

class TimingWheel { 
	
	Partition * slot[10 + 1];   
	int current_slot; 
	int nCPUs = 4;
	int timeUnit = 0;
	int prevTimeUnit = 0;
	int jobs = 0;
	
public: 
	bool insert(int burst_time, int CPU_num, PCB* p1); 
	void schedule(); 
	void clear_curr_slot(int i){
		slot[i] = NULL;
	}; 
	//make all slots NULL at the beginning
	TimingWheel(){
		current_slot = 0;
		for(int i=0;i<11;i++){
			slot[i] = NULL;
		}
	}
	
	int getNCPUs(){
		return nCPUs;
	}
	void setNCPUs(int n){
		this->nCPUs = n;
	}
	
	int getTimeUnit(){
		return timeUnit;
	}
	
	int getJobs(){
		return jobs;
	}
	
	void incrementJobs(){
		this->jobs++;
	}
}; 


class Scheduler{
private:
	//core
	int timeUnit=0;
	int jobsCompleted=0;
	int n_jobs;
	int rs1,rs2,stv1,stv2;
	int currentBurst;
	int ind = 0; //to keep track of index.
	int nCPUs;
	//pseudo index
	int jobIndex=0; //to display.

	
	//queues for jobs
	vector<priorityQ> priorityQueue;
	vector<priorityQ> priorityQueueNR;
	vector<SJF> sjfQueue;
	vector<SJF> sjfQueueNR;
	vector<FCFS> fcfsQueue;
	vector<FCFS> fcfsQueueNR;
	
	vector<TimingWheel> timingwheel;
	
	//cpu related
	
	int* cpus;
	int cpuAvailable = 0;
	
	//checks
	bool priorityNRcomplete = false;
	bool sjfNRcomplete = false;
	bool fcfsNRcomplete = false;
	bool nrJobsOngoing = false;
public:
	Scheduler(int jobs, int rs1, int rs2, int stv1, int stv2);
	void addProcess(string type, string r, int bt);
	void status();
	int getTimeUnit(){return timeUnit;};
	void schedule();
	
	int checkCPUs();
	void setCPUFree(); //sets the first found cpu free.
	
	void checkCompletion(string q);
	
	
	//algorithms
	int priorityJobs();
	int priorityNRJobs();
	int sjfJobs();
	int sjfNRJobs();
	void fcfsJobs();
	void fcfsNRJobs();
};

 