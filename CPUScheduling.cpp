#include <typeinfo>
using namespace std;
#include "CPUScheduling.h"

//PCBs
PCB::PCB(){
	
}

//RNR
RecurringPCB::RecurringPCB(){
	
}

NonRecurringPCB::NonRecurringPCB(){
	
}

RecurringPCB::RecurringPCB(int burstTime, int sleepTime, string type):PCB(burstTime, type){
	this->sleepTime = sleepTime;
}

NonRecurringPCB::NonRecurringPCB(int burstTime, int timeToRemain, string type):PCB(burstTime, type){
	this->timeToRemain = timeToRemain;
}


//Queues:
priorityQ::priorityQ(string r,int bt, int time):RecurringPCB(bt, time, "PriorityQ"), NonRecurringPCB(bt, time, "PriorityQNR"){
	this->burstTime = bt;
	
	//random priority of 1 to 3.
	this->priority = 1 + rand() % ( 3 - 1  + 1);
	if(r=="r"){
		RecurringPCB(bt, time, "PriorityQ");
		this->setSleepTime(time);
	}
	else{
		NonRecurringPCB(bt,time, "PriorityQNR");
		this->setTimeToRemain(time);
	}
	
}

SJF::SJF(string r,int bt, int time):RecurringPCB(bt, time, "SJF"), NonRecurringPCB(bt, time, "SJFNR"){
	this->burstTime = bt;
	if(r=="r"){
		RecurringPCB(bt, time, "SJF");
		this->setSleepTime(time);
	}
	else{
		NonRecurringPCB(bt,time, "SJFNR");
		this->setTimeToRemain(time);
	}
	
}


FCFS::FCFS(string r,int bt, int time):RecurringPCB(bt, time, "FCFS"), NonRecurringPCB(bt, time, "FCFSNR"){
	this->burstTime = bt;
	if(r=="r"){
		RecurringPCB(bt, time, "FCFS");
		this->setSleepTime(time);
	}
	else{
		NonRecurringPCB(bt,time, "FCFSNR");
		this->setTimeToRemain(time);
	}
	
}


//scheduler
Scheduler::Scheduler(int jobs, int rs1, int rs2, int stv1, int stv2){
	this->n_jobs = jobs;
	this->rs1 = rs1;
	this->rs2 = rs2;
	this->stv1 = stv1;
	this->stv2 = stv2;
	
	cpus = new int[nCPUs];
}

void Scheduler::addProcess(string type, string r, int bt){
	int starve = stv1 + rand() % ( stv2 - stv1  + 1);
	int sleep = rs1 + rand() % ( rs2 - rs1  + 1);
	if(type=="Priority"){
		if(r=="r"){
			priorityQueue.emplace_back(r,bt, sleep);
		}
		else{
			priorityQueueNR.emplace_back(r,bt,starve);
		}
	}
	else if(type=="SJF"){
		if(r=="r"){
			sjfQueue.emplace_back(r,bt, sleep);
		}
		else{
			sjfQueueNR.emplace_back(r,bt, sleep);
		}
	}
	else{
		if(r=="r"){
			fcfsQueue.emplace_back(r,bt, sleep);
		}
		else{
			fcfsQueueNR.emplace_back(r,bt, sleep);
		}
	}
}

void Scheduler::status(){
	vector<priorityQ>::iterator it;
	cout<<"BURST TIMES"<<endl;
	cout<<"************"<<endl;
	cout<<"Priority Queue Jobs: "<<endl;
	for(it=priorityQueue.begin(); it!=priorityQueue.end(); it++){
		cout<<it->getBurstTime()<<endl;
	}
	
	cout<<"Priority Queue NR Jobs: "<<endl;
	for(it=priorityQueueNR.begin(); it!=priorityQueueNR.end(); it++){
		cout<<it->getBurstTime()<<endl;
	}
	
	vector<SJF>::iterator it2;
	cout<<"SJF Queue Jobs: "<<endl;
	for(it2=sjfQueue.begin(); it2!=sjfQueue.end(); it2++){
		cout<<it2->getBurstTime()<<endl;
	}
	
	cout<<"SJF Queue NR Jobs: "<<endl;
	for(it2=sjfQueueNR.begin(); it2!=sjfQueueNR.end(); it2++){
		cout<<it2->getBurstTime()<<endl;
	}
	
	vector<FCFS>::iterator it3;
	cout<<"FCFS Queue Jobs: "<<endl;
	for(it3=fcfsQueue.begin(); it3!=fcfsQueue.end(); it3++){
		cout<<it3->getBurstTime()<<endl;
	}
	
	cout<<"FCFS Queue NR Jobs: "<<endl;
	for(it3=fcfsQueueNR.begin(); it3!=fcfsQueueNR.end(); it3++){
		cout<<it3->getBurstTime()<<endl;
	}
	
}

int Scheduler::checkCPUs(){
	int i;
	for(i=0;i<nCPUs;i++){
		//0 available 1 not available.
		if(cpus[i]==0){
			return i;
		}
	}
	if(i==nCPUs){
		return -1;
	}
	return -1;
}

void Scheduler::setCPUFree(){
	for(int i=0;i<nCPUs;i++){
		if(cpus[i]==1){
			cpus[i]=0;
			return;
		}
	}
}

void Scheduler::checkCompletion(string q){
	if(currentBurst==0){
		cout<<"Job #"<<jobIndex<<"completed"<<endl;
		jobsCompleted++;
		if(q=="pq"){
			priorityQueue.erase(priorityQueue.begin()+ind);
		}
		else if(q=="pnrq"){
			priorityQueueNR.erase(priorityQueueNR.begin()+ind);
			if(priorityQueueNR.size()==0){
				cout<<"Priority Non Recursive Queue empty!"<<endl;
				cout<<"Exiting from the system!"<<endl;
				nrJobsOngoing = false;
				priorityNRcomplete = true;
			}
		}
		else if(q=="sjfq"){
			sjfQueue.erase(sjfQueue.begin()+ind);
		}
		else if(q=="sjfnrq"){
			sjfQueueNR.erase(sjfQueueNR.begin()+ind);
			if(sjfQueueNR.size()==0){
				cout<<"SJF Non Recursive Queue empty!"<<endl;
				cout<<"Exiting from the system!"<<endl;
				nrJobsOngoing = false;
				sjfNRcomplete = true;
			}
		}
		else if(q=="fcfsq"){
			fcfsQueue.erase(fcfsQueue.begin());
		}
		else if(q=="fcfsnrq"){
			fcfsQueueNR.erase(fcfsQueueNR.begin());
			if(fcfsQueueNR.size()==0){
				cout<<"FCFS Non Recursive Queue empty!"<<endl;
				cout<<"Exiting from the system!"<<endl;
				nrJobsOngoing = false;
				fcfsNRcomplete = true;
			}
		}
		//cout<<"Size changed to: "<<priorityQueue.size()<<endl;
		setCPUFree();
		
		if(jobsCompleted==n_jobs){
			return;
		}
	}
	else{
		cout<<"All CPUs busy"<<endl;
	}
}

void Scheduler::schedule(){
	int allocatedCPUs = 0;
	timeUnit = 0;
	for(int i=0;i<nCPUs;i++){
		cpus[i] = 0;
	}
	TimingWheel t;
	jobsCompleted = t.getJobs();
	while(jobsCompleted<n_jobs){
		
		//complete priority queue.
		timeUnit = t.getTimeUnit();
		//cout<<"At time unit: "<<timeUnit<<endl;

		
		//place partitions in timing wheel
		//assuming at t=0, all cpus are available.
		jobsCompleted = t.getJobs();
		if(jobsCompleted>=n_jobs){
			break;
		}
		
		nCPUs = t.getNCPUs();
		for(int i=0; i<nCPUs; i++){
			if(allocatedCPUs == nCPUs){
				break;
			}
			if(priorityQueue.size()>0 || priorityQueueNR.size()>0){
				//n_jobs++;
				if(priorityQueue.size()>0){
					int ind = priorityJobs();
					//priorityQueue[ind].setBurstTime(priorityQueue[ind].getBurstTime());
					if(t.insert(priorityQueue[ind].getBurstTime(),i,&priorityQueue[ind])){
						priorityQueue.erase(priorityQueue.begin()+ind);
						//nCPUs--;
						allocatedCPUs++;
						//i++;
					}
	
					continue;
					
				}
				if(priorityQueueNR.size()>0){
					int ind = priorityNRJobs();
					if(t.insert(priorityQueueNR[ind].getBurstTime(),i,&priorityQueueNR[ind])){
						priorityQueueNR.erase(priorityQueueNR.begin()+ind);
						//nCPUs--;
						allocatedCPUs++;
						//i++;
					}

					continue;
				}
				
			}
			if(priorityQueue.size()==0 && priorityQueueNR.size()==0){
				if(sjfQueue.size()>0 || sjfQueueNR.size()>0){
					//n_jobs++;
					if(sjfQueue.size()>0){
						int ind = sjfJobs();
						if(t.insert(sjfQueue[ind].getBurstTime(),i,&sjfQueue[ind])){
							sjfQueue.erase(sjfQueue.begin()+ind);
							//nCPUs--;
							allocatedCPUs++;
							//i++;
						}

						continue;
					}
					if(sjfQueueNR.size()>0){
						int ind = sjfNRJobs();
						if(t.insert(sjfQueueNR[ind].getBurstTime(),i,&sjfQueueNR[ind])){
							sjfQueueNR.erase(sjfQueueNR.begin()+ind);
							//nCPUs--;
							allocatedCPUs++;
							//i++;
						}

						continue;
					}
				}
			}
			
			if(priorityQueue.size()==0 && priorityQueueNR.size()==0 && sjfQueue.size()==0 && sjfQueueNR.size()==0){
				if(sjfQueue.size()==0 && sjfQueueNR.size()==0){
					//n_jobs++;
					if(fcfsQueue.size()>0 || fcfsQueueNR.size()>0){
						if(fcfsQueue.size()>0){
							cout<<"Allocating First in Queue: "<<fcfsQueue[0].getBurstTime()<<" to available CPU"<<endl;
							if(t.insert(fcfsQueue[0].getBurstTime(),i,&fcfsQueue[0])){
								fcfsQueue.erase(fcfsQueue.begin());
								//nCPUs--;
								allocatedCPUs++;
								//i++;
							}

							continue;
						}
						if(fcfsQueueNR.size()>0){
							cout<<"Allocating First in Queue: "<<fcfsQueueNR[0].getBurstTime()<<" to available CPU"<<endl;
							if(t.insert(fcfsQueueNR[0].getBurstTime(),i,&fcfsQueueNR[0])){
								fcfsQueueNR.erase(fcfsQueueNR.begin());
								//nCPUs--;
								allocatedCPUs++;
								//i++;
							}

							continue;
						}
					}
				}
				
			}
			
			
		}
		if(jobsCompleted==0 && t.getNCPUs()==4){
			t.setNCPUs(0);
			t.schedule();
			allocatedCPUs = 0;
		}
		else if(t.getNCPUs()==allocatedCPUs){
			t.setNCPUs(0);
			t.schedule();
			allocatedCPUs = 0;
		}
		else if(fcfsQueue.size()==0 && fcfsQueueNR.size()==0){
			t.setNCPUs(0);
			t.schedule();
			allocatedCPUs = 0;
		}
	} //while loop ends
	
	cout<<"All jobs completed execution!"<<endl;
}

bool TimingWheel::insert(int burst_time, int CPU_num, PCB* p1){
	
	if(nCPUs>0){
		if(slot[(current_slot+burst_time)%11] == NULL){
			slot[(current_slot+burst_time)%11] = new Partition(p1, CPU_num, NULL);
		}
		else{
			Partition *temp = slot[(current_slot+burst_time)%11];
			while(temp->getNext()!=NULL){
				temp = temp->getNext();
			}
			temp->setNext(new Partition(p1, CPU_num, NULL));
		}
		//nCPUs--;
		
		return true;
	}
	else{
		return false;
	}
	
}

void TimingWheel::schedule(){
	int i;
	if(timeUnit==0){
		current_slot = 0;
	}
	for(i=current_slot; i<11+current_slot;i++){
		if(slot[i%11]==NULL){
			continue;
		}
		
		else{
			timeUnit = prevTimeUnit+slot[i%11]->getBurst();
			cout<<"At time unit"<<timeUnit<<":"<<endl;
			cout<<slot[i%11]->getType()<<"Job with burst time "<<slot[i%11]->getBurst()<<" has completed execution"<<endl;
			nCPUs += 1;
			jobs += 1;
			cout<<"CPU is available"<<endl;
			if(slot[i%11]->getNext() == NULL){
				clear_curr_slot(i%11);
				current_slot = i%11;
				prevTimeUnit = timeUnit;
				return;
			}
			else{
				slot[i%11] = slot[i%11]->getNext();
				i -= 1;
				current_slot = (i-1)%11;
				prevTimeUnit = timeUnit;
				return;
			}
			break;
		}
	}
	
}


//priority
int Scheduler::priorityJobs(){
	if(cpuAvailable != -1){
		//cout<<"CPU # "<<cpuAvailable+1<<" is available"<<endl;
		
		//cout<<"Checking for priority Queue: "<<endl;
		if(priorityQueue.size()>=1){
			//get the highest priority.
			int highestPriority = 0;
			
			//searches for highest priority.
			for(int i=0;i<priorityQueue.size();i++){
				if(priorityQueue[i].getPriority()>highestPriority){
					highestPriority = priorityQueue[i].getPriority();
					ind = i;
					currentBurst = priorityQueue[i].getBurstTime();
				}
			}
			
			//priority received.
			//cout<<"allocating job # "<<++jobIndex<<" in priority queue to CPU"<<endl;
			cout<<"Allocating Priority: "<<highestPriority<<"| Burst Time: "<<currentBurst<<" to available CPU"<<endl;
			return ind;
		}
		else{
			cout<<"Priority Queue empty!"<<endl;
			if(!priorityNRcomplete){
				priorityNRJobs();
			}
		}
	}
	else{
		if(!nrJobsOngoing){
			currentBurst--;
			checkCompletion("pq");
		}
		else{
			priorityNRJobs();
		}
	}
	return 0;
}

int Scheduler::priorityNRJobs(){
	if(cpuAvailable != -1){
		//cout<<"CPU # "<<cpuAvailable+1<<" is available"<<endl;
		
		//cout<<"Checking for priority Non Recursive Queue: "<<endl;
		if(priorityQueueNR.size()>=1){
			//get the highest priority.
			int highestPriority = 0;
			
			//searches for highest priority.
			for(int i=0;i<priorityQueueNR.size();i++){
				if(priorityQueueNR[i].getPriority()>highestPriority){
					highestPriority = priorityQueueNR[i].getPriority();
					ind = i;
					currentBurst = priorityQueueNR[i].getBurstTime();
				}
			}
			
			//priority received.
			//cout<<"allocating job # "<<++jobIndex<<" in priority Non Recursive queue to CPU"<<endl;
			cout<<"Allocating Priority: "<<highestPriority<<"| Burst Time: "<<currentBurst<<" to available CPU"<<endl;
			return ind;
			cpus[cpuAvailable] = 1;
			nrJobsOngoing = true;
		}
		else{
			cout<<"Priority Non Recursive Queue empty!"<<endl;
			cout<<"Exiting from the system!"<<endl;
			nrJobsOngoing = false;
			priorityNRcomplete = true;
		}
		
	}
	else{
		currentBurst--;
		checkCompletion("pnrq");
	}
	return 0;
}

//SJF
int Scheduler::sjfJobs(){
	if(cpuAvailable != -1){
		//cout<<"CPU # "<<cpuAvailable+1<<" is available"<<endl;
		
		//cout<<"Checking for SJF Queue: "<<endl;
		if(sjfQueue.size()>=1){
			//get the highest priority.
			currentBurst = 999999;
			
			//searches for highest priority.
			for(int i=0;i<sjfQueue.size();i++){
				if(sjfQueue[i].getBurstTime()<currentBurst){
					ind = i;
					currentBurst = sjfQueue[i].getBurstTime();
				}
			}
			
			//shortest job received
			//cout<<"allocating job # "<<++jobIndex<<" in SJF queue to CPU"<<endl;
			cout<<"Allocating Shortest Burst Time: "<<currentBurst<<" to available CPU"<<endl;
			return ind;
			cpus[cpuAvailable] = 1;
		}
		else{
			cout<<"SJF Queue empty!"<<endl;
			if(!sjfNRcomplete){
				sjfNRJobs();
			}
		}
	}
	else{
		if(!nrJobsOngoing){
			currentBurst--;
			checkCompletion("sjfq");
		}
		else{
			sjfNRJobs();
		}
	}
	return 0;
}

int Scheduler::sjfNRJobs(){
	if(cpuAvailable != -1){
		//cout<<"CPU # "<<cpuAvailable+1<<" is available"<<endl;
		
		//cout<<"Checking for SJF Non Recursive Queue: "<<endl;
		if(sjfQueueNR.size()>=1){
			//get the highest priority.
			currentBurst = 999999;
			
			//searches for highest priority.
			for(int i=0;i<sjfQueueNR.size();i++){
				if(sjfQueueNR[i].getBurstTime()<currentBurst){
					ind = i;
					currentBurst = sjfQueueNR[i].getBurstTime();
				}
			}
			
			//priority received.
			//cout<<"allocating job # "<<++jobIndex<<" in priority Non Recursive queue to CPU"<<endl;
			cout<<"Allocating Shortest Burst Time: "<<currentBurst<<" to available CPU"<<endl;
			return ind;
			cpus[cpuAvailable] = 1;
			nrJobsOngoing = true;
		}
		else{
			cout<<"SJF Non Recursive Queue empty!"<<endl;
			cout<<"Exiting from the system!"<<endl;
			nrJobsOngoing = false;
			sjfNRcomplete = true;
		}
		
	}
	else{
		currentBurst--;
		checkCompletion("sjfnrq");
	}
	return 0;
}


//FCFS
void Scheduler::fcfsJobs(){
	if(cpuAvailable != -1){
		cout<<"CPU # "<<cpuAvailable+1<<" is available"<<endl;
		
		cout<<"Checking for FCFS Queue: "<<endl;
		if(fcfsQueue.size()>=1){
			
			ind = 0;
			currentBurst = fcfsQueue[0].getBurstTime();
			
			//priority received.
			cout<<"allocating job # "<<++jobIndex<<" in FCFS queue to CPU"<<endl;
			cout<<"Burst Time: "<<currentBurst<<endl;
			cpus[cpuAvailable] = 1;
		}
		else{
			cout<<"FCFS Queue empty!"<<endl;
			if(!fcfsNRcomplete){
				fcfsNRJobs();
			}
		}
	}
	else{
		if(!nrJobsOngoing){
			currentBurst--;
			checkCompletion("fcfsq");
		}
		else{
			fcfsNRJobs();
		}
	}
	
}

void Scheduler::fcfsNRJobs(){
	if(cpuAvailable != -1){
		cout<<"CPU # "<<cpuAvailable+1<<" is available"<<endl;
		
		cout<<"Checking for FCFS Non Recursive Queue: "<<endl;
		if(fcfsQueueNR.size()>=1){
			for(int i=0;i<fcfsQueueNR.size();i++){
				ind = i;
				currentBurst = fcfsQueueNR[i].getBurstTime();
			}
			
			
			cout<<"allocating job # "<<++jobIndex<<" in priority Non Recursive queue to CPU"<<endl;
			cout<<"Burst Time: "<<currentBurst<<endl;
			cpus[cpuAvailable] = 1;
			nrJobsOngoing = true;
		}
		else{
			cout<<"FCFS Non Recursive Queue empty!"<<endl;
			cout<<"Exiting from the system!"<<endl;
			nrJobsOngoing = false;
			fcfsNRcomplete = true;
		}
		
	}
	else{
		currentBurst--;
		checkCompletion("fcfsnrq");
	}
}