#include <iostream>
#include <array>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <queue>
#include <algorithm>
#include <vector>
using namespace std;


void random(int *arry,int percent[],int size) {
	int temp_time = 0;
	int numofBT = 0;
	int channel = 0;
	int numofPC_virtual[3] = { 7,11,6};
	int numofPC_begin[3] = { 2,20,35 };
	

	for (int i = 0; i < 3; i++) {
		numofBT= (size * percent[i]) / 100;
		
		while (numofBT != 0) {
			temp_time =( rand() % numofPC_virtual[i]) + numofPC_begin[i];
		
			while (true) {
				channel = rand() % size;
				if(arry[channel]==0) {
					arry[channel] = temp_time;
					break;
				}
			}
			numofBT--;
		}
	}
}

int Each_Waitingtime(int* arry, int size, int max) {
	int total_waiting_time = 0;
	if (max == 0) {
		return 0;
	}
	
		for (int i = 0; i < max; i++) {
				total_waiting_time = total_waiting_time + arry[i];
			}
		return total_waiting_time;	
}

int avg_waiting(int* arry, int size, vector<int>seq_process) {
	int total_waiting_time = 0;
	int hold = 0;
	for (int i = 0; i < size; i++) {
		hold= Each_Waitingtime(arry, size, i);
		total_waiting_time = total_waiting_time + hold;
	}
	return (total_waiting_time /size);
}

int RREach_Waitingtime(int* arry, int size, int processI, vector<int>seq_process) {
	int gab = 0;
	//cout << "process--->" << processI << endl;
	for (int l = 0; l < size; l++) {
		
		if (seq_process[l] == processI && l>0) {
			int b = l-1;
			while (b > 0 && seq_process[l] != seq_process[b]) {	
				b--;
			}
			gab = gab + (Each_Waitingtime(arry, size, l) - Each_Waitingtime(arry, size, b + 1));	
		}
		else {
			gab = gab + 0;
		}
		
	}

	return gab;
}

int RRavg_waiting(int* arry,int oldsize,int size, vector<int>seq_process) {
	int total_waiting_time = 0;
	int hold = 0;

	for (int i = 0; i < oldsize; i++) {
		hold = RREach_Waitingtime(arry, size, i, seq_process);
		total_waiting_time = total_waiting_time + hold;
	}
	return (total_waiting_time / size);
}

int avg_turn_around(int* arry, int size,vector<int>seq_process) {
	int total_turn_at = 0;
	int i = 0;
	int status = 0;
	vector<int> check;
	for (int i = seq_process.size() - 1;i>=0; i--) {
		if (i == seq_process.size() - 1) {
			check.push_back(seq_process[i]);
			total_turn_at = total_turn_at + Each_Waitingtime(arry, size, i)+arry[i];
		}
		else {
			for (int j = 0; j < check.size(); j++) {
				if (check[j] == seq_process[i]) {
					status = 1;//have
					break;
				}			
			}

			if (status == 0) {//no have
				check.push_back(seq_process[i]);
				total_turn_at = total_turn_at + Each_Waitingtime(arry, size, i+1);
			}
		}	
		status = 0;
	}
	//total_turn_at = total_turn_at +Each_Waitingtime(arry,size,i+1);
	return (total_turn_at/size);
}

void fcfs(int *arry,int size) {
	vector<int>seq_process;
	for (int i = 0; i < size; i++) {
		seq_process.push_back(i);
	}
	cout << "Average Waiting time of fcfs algorithm    :" << avg_waiting(arry, size, seq_process) << endl;
	cout << "Average Turn-Around time of fcfs algorithm:" << avg_turn_around(arry, size, seq_process) << endl;

}

void sjf_withoutAT(int arry[], int size) {
	vector<int>seq_process;
	int* arry_virtual = new int[size];
	for (int i = 0; i < size; i++) {
		arry_virtual[i] = arry[i];
		seq_process.push_back(i);
	}
	sort(arry_virtual,arry_virtual+size);
	cout <<"Average Waiting time of SJF algorithm     :" << avg_waiting(arry_virtual, size,seq_process)<<endl;
	cout <<"Average Turn-Around time of SJF algorithm :" << avg_turn_around(arry_virtual, size, seq_process) << endl;

	delete [] arry_virtual;
}

void chop(int size, int q,vector<int> &bursttime,vector<int> &Q_process) {
	
	int status = 0;
	int i = 0;
	while (i < bursttime.size()) {
		if (bursttime[i] > q)break;
		else {
			i++;
		}
	}
	
	while (i < bursttime.size()-1) {
			if (bursttime[i] > q) {
				bursttime.push_back(bursttime[i] - q);
				bursttime[i] = q;
				Q_process.push_back(Q_process[i]);
			}
		
		i= 0;
		while (i < bursttime.size()) {
			if (bursttime[i] > q)break;
			else {
				i++;
			}
		}
	}
}

void round_robin(int arry[], int size,int q) {
	int* arry_virtual;
	vector<int> temp;
	vector<int> queueprocess;
	int hold = 0;
	for (int i = 0; i < size; i++) {
		queueprocess.push_back(i);
		temp.push_back(arry[i]);
	}
	
	chop(size,q,temp, queueprocess);
	arry_virtual = new int[temp.size()+1];
	
	for (int i = 0; i < temp.size(); i++) {
		arry_virtual[i] = temp[i];
	}
	
	cout <<"Average Waiting time of RR algorithm       :" << RRavg_waiting(arry_virtual,size,temp.size(),queueprocess) << endl;
	cout <<"Average Turn-Around time of fcfs algorithm :" << avg_turn_around(arry_virtual, temp.size(), queueprocess) << endl;
	delete[] arry_virtual;
}

void show(int* arry, int size) {
	int hold = 0;

	for (int i = 1; i <= size; i++) {
		hold = arry[i - 1];
		if (hold < 10) { cout << " "; }
		if (hold < 100) { cout <<" "; }
		cout << hold<<" ";
		if (i % 10 == 0)cout << endl;
	}
}


int main() {
	srand(time(0));
	int percent_assum1[3] = { 70,20,10 };
	int percent_assum2[3] = { 50,30,20 };
	int percent_assum3[3] = { 40,40,20 };
	
	int assum_one[60] = {0};
	int assum_two[40] = {0};
	int assum_three[20] = {0};
	int assum_test[10] = { 0 };
	int size_one = 60;
	int size_two = 40;
	int size_three = 20;
	int q = 5;

	random(assum_one,percent_assum1,size_one);
	random(assum_two, percent_assum2,size_two);
	random(assum_three, percent_assum3,size_three);
	
	show(assum_one, size_one);
	cout << "---------------------------" << endl;
	fcfs(assum_one, size_one);
	fcfs(assum_two, size_two);
	fcfs(assum_three, size_three);
	cout << "---------------------------" << endl;
	sjf_withoutAT(assum_one, size_one);
	sjf_withoutAT(assum_two, size_two);
	sjf_withoutAT(assum_three, size_three);
	cout << "---------------------------" << endl;
	q = 8;
	cout << q << endl;
	round_robin(assum_one, size_one, q);
	round_robin(assum_two, size_two, q);
	round_robin(assum_three, size_three, q);
	cout << "---------------------------" << endl;

	q = 15;
	cout << q << endl;
	round_robin(assum_one, size_one, q);
	round_robin(assum_two, size_two, q);
	round_robin(assum_three, size_three, q);
	cout << "---------------------------" << endl;

	q = 20;
	cout << q << endl;
	round_robin(assum_one, size_one, q);
	round_robin(assum_two, size_two, q);
	round_robin(assum_three, size_three, q);
	cout << "---------------------------" << endl;
	q = 30;
	cout << q << endl;
	round_robin(assum_one, size_one, q);
	round_robin(assum_two, size_two, q);
	round_robin(assum_three, size_three, q);
	cout << "---------------------------" << endl;
	q = 40;
	cout << q << endl;
	round_robin(assum_one, size_one, q);
	round_robin(assum_two, size_two, q);
	round_robin(assum_three, size_three, q);
	/*q = 20;
	cout << q << endl;
	round_robin(assum_one, size_one, q);
	round_robin(assum_two, size_two, q);
	round_robin(assum_three, size_three, q);
	q = 30;
	cout << q << endl;
	round_robin(assum_one, size_one, q);
	round_robin(assum_two, size_two, q);
	round_robin(assum_three, size_three, q);
	q = 40;
	cout << q << endl;
	round_robin(assum_one, size_one, q);
	round_robin(assum_two, size_two, q);
	round_robin(assum_three, size_three, q);
	*/
	return 0;

}