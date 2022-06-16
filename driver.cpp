#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
using namespace std;

#include "CPUScheduling.h"

int main(int argc, char *argv[]) {  
	ifstream in;
	string line;
	srand(time(NULL));
	in.open("input.txt");
	getline(in, line);
	int n_processes = stoi(line);
	
	//recurring sleep range
	getline(in, line);
	stringstream ss(line);
	string identifier;
	int rs1, rs2;
	ss>>identifier;
	ss>>rs1;
	ss>>rs2;
	
	//starvation threshold
	getline(in, line);
	stringstream ss2(line);
	ss2>>identifier;
	int stv1, stv2;
	ss2>>stv1;
	ss2>>stv2;

	string type;
	vector<int> prob;
	string rtype;
	vector<int> range1,range2;
	string burst;
	vector<int> brange1, brange2;
	while(getline(in, line)){
		int p1;
		int r1,r2;
		int br1, br2;
		stringstream ss3(line);
		
		ss3>>type;
		ss3>>p1;
		prob.push_back(p1);
		ss3>>rtype;
		ss3>>r1;
		
		range1.push_back(r1);
		ss3>>rtype;
		ss3>>r2;
		
		range2.push_back(r2);
		
		ss3>>burst;
		
		ss3>>br1;
		ss3>>br2;
		
		brange1.push_back(br1);
		brange2.push_back(br2);
	}
	
	//file read complete.
	
	//create objects.
	
	Scheduler s(n_processes,rs1, rs2, stv1, stv2);
	for(int i=0; i<n_processes;i++){
		int rnr = 1 + rand() % ( 100 - 1  + 1);
		int k = 1 + rand() % ( 100 - 1  + 1);
		if (k<=prob[0]){
			//determine recursive or non recursive.
			int bt = brange1[0] + rand() % ( brange2[0] - brange1[0] + 1);
			if(rnr<=range1[0]){
				//add a process.
				s.addProcess("Priority","r",bt);
			}
			//similarly for others
			//will be completed after all the functions are completed.
			else{
				//priority non recursive
				s.addProcess("Priority","nr",bt);
			}
		}
		else if (k<=prob[1]){
			int bt = brange1[1] + rand() % ( brange2[1] - brange1[1]  + 1);
			if(rnr<=range1[1]){
				s.addProcess("SJF","r",bt);
			}
			else{
				s.addProcess("SJF","nr", bt);
			}
		}
		else{
			int bt = brange1[2] + rand() % ( brange2[2] - brange1[2] ) + 1;
			if(rnr<=range1[2]){
				s.addProcess("FCFS","r",bt);
			}
			else{
				s.addProcess("FCFS","nr", bt);
			}
		}
	}
	
	//check the status:
	s.status();
	
	//begin scheduling
	s.schedule();
}