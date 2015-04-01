/* 
 * File:   main.cpp
 * Author: Michael,Marialena
 *
 * Created on February 11, 2015, 9:10 PM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>          // std::stack
#include <vector>
#include "LinkedList.h"
#include <list>
#include <sstream>     // for ostringstream
#include <time.h>

using namespace std;

LinkedList stations;

stack<int> stack_ones;
stack<int> stack_navigation;
stack<int> temporary; //remove the duplicates from stacks  

list<int> buf_available;
list<int> temp_buf_available;
list<int> max_following_tasks;
list<int> min_following_tasks;
list<int> shortest_tasks;
list<int> longest_tasks;
list<int> max_rank_tasks;
list<int> max_vns_tasks;

vector<int> tasks_completed;

vector<float> optimum_vns_vector;
vector<float> new_vns_vector;
vector<float> old_vns_vector;
vector<vector<int> > temp_depend_tasks_vns;

vector<int> optimum_heur_vector;
vector<int> new_heur_vector;
vector<int> old_heur_vector;
vector<vector<int> > temp_depend_tasks_heur;
    
string input_bench; //the name of the file that the user selects

static int count = 0;

bool available_task;       // all columns to be equal to zero
bool duplicate;
bool flag = false;    //push to stack if not duplicate  
bool already_ranked;

int input_number;   //the number of the benchmark
int temp;   
int max_processing_time; //used by longest task time algorithm
int min_processing_time; //used by shortest task time algorithm
int max_ones;         //used by most following tasks algorithm
int min_ones;         //used by least following tasks algorithm
int max_rank;         //used by ranked positional weight
int following_count;  //used by most following tasks algorithm & least following tasks algorithm
int temp_task_time;   //used by longest task time algorithm and shortest task time algorithm
int rank_tasks;       //used by ranked positional weight
int random_val;
int counter_benchm=0;
int condition_k = 0; //used by VNS algorithm
int LBc = 0; //used by ALBP2 - lower bound
int UBc = 0; //used by ALBP2 - upper bound
int midc = 0; //used by ALBP2 - binary search
int Pmax=0; //used by LBc, UBc and midc - ALBP2
int Psum=0; //used by LBc, UBc and midc - ALBP2
int random_heuristic; //used by Random algorithm
float max_float_value = 0; // used by VNS algorithm
int g=0; //used by random algorithm
int current_numb_stations = 0;
float cpu_time = 0;

string IntToString (int a)
{
    ostringstream temp;
    temp<<a;
    return temp.str();
}

void print_result_of_albp1(int problem,string algorithm,string benchmark_name,int numb_benchm,int number_of_tasks,int cycle_time,int optimum_station_numb,int numb_stations,float bd,float sx, float time){
    string problem_str;
    if (problem == 1)
        problem_str = "ALBP-1";
    else if (problem == 2)
        problem_str = "ALBP-2";
    
    string under_score = "_";
    string end_of_file = ".txt";
    
	algorithm.erase(0, 3);
    
	string outputs = "outputs/ALBP1/";
	string slash = "/";
    
    string name_file = outputs + algorithm + slash + problem_str + under_score + benchmark_name.c_str() + under_score + algorithm.c_str() + end_of_file;
    
    int absdev = abs(numb_stations-optimum_station_numb);
    float absdevperc = ((numb_stations-optimum_station_numb)/optimum_station_numb)*100;
 
    std::ostringstream ostr_time;
    ostr_time << time;
    std::string str_time = ostr_time.str();
    
    std::ostringstream ostr;
    ostr << round(absdevperc*100)/100;
    std::string str_absdevperc = ostr.str();
    
    std::ostringstream ostr_bd;
    ostr_bd << round(bd*100)/100;
    std::string str_bd = ostr_bd.str();
    
    std::ostringstream ostr_sx;
    ostr_sx << round(sx*100)/100;
    std::string str_sx = ostr_sx.str();
    
    string lines = problem_str + "         " + benchmark_name + "              " + IntToString(numb_benchm) + "          " + IntToString(number_of_tasks) + "        " + IntToString(cycle_time) + "      " + IntToString(optimum_station_numb) + "        " + IntToString(numb_stations) + "         " + IntToString(absdev) + "               " + str_absdevperc.c_str() + "         " + str_bd.c_str() + "       " + str_sx.c_str() + "         " + str_time.c_str() + "\n";

    string results = stations.printReverseRecursive_results();
    
  ofstream myfile (name_file.c_str(), std::ios_base::app);
  if (myfile.is_open())
  {
        myfile << "Problem         Benchmark           Bench#         n         c         m*         m         abs.dev         \%dev         BD         SX         CPU time(sec)\n";    myfile << lines;
    myfile << "\n";
    myfile << "Tasks allocation to stations: \n";
    myfile << results;
    
    myfile.close();
  }
  else cout << "Unable to open print_albp1 file";
}

void print_result_of_albp2(int problem,int method, string algorithm,string benchmark_name,int numb_benchm,int number_of_tasks,int cycle_time, int optimum_cycles, int optimum_station_numb,int numb_stations,float bd,float sx, float time){
    string problem_str;
    if (problem == 1)
        problem_str = "ALBP-1";
    else if (problem == 2)
        problem_str = "ALBP-2";    
     
    string under_score = "_";
    string end_of_file = ".txt";
    
	algorithm.erase(0, 3);
    
	string outputs = "outputs/ALBP2/";
	string slash = "/";
        
    int absdev = abs(cycle_time-optimum_cycles);
    float absdevperc = ((cycle_time-optimum_cycles)/optimum_cycles)*100;
    
    string algorithm_str;
    
    if (method == 1){
        algorithm_str = "Lower Bound";
    }
    else if (method == 2){
        algorithm_str = "Upper Bound";
    }
    else if (method == 3){
        algorithm_str = "Binary Search";
    }
    
    string name_file = outputs + algorithm + slash + problem_str + under_score + algorithm_str + under_score + benchmark_name.c_str() + under_score + algorithm.c_str() + end_of_file;

    std::ostringstream ostr_time;
    ostr_time << time;
    std::string str_time = ostr_time.str();
    
    std::ostringstream ostr;
    ostr << round(absdevperc*100)/100;
    std::string str_absdevperc = ostr.str();
    
    std::ostringstream ostr_bd;
    ostr_bd << round(bd*100)/100;
    std::string str_bd = ostr_bd.str();
    
    std::ostringstream ostr_sx;
    ostr_sx << round(sx*100)/100;
    std::string str_sx = ostr_sx.str();
  
    string lines = problem_str + "          " + benchmark_name + "            " + IntToString(numb_benchm) + "         " + algorithm_str + "          " + IntToString(number_of_tasks) + "      " + IntToString(cycle_time) + "      " + IntToString(optimum_cycles) + "         " + IntToString(numb_stations) + "          " + IntToString(absdev) + "              " + str_absdevperc.c_str() + "          " + str_bd.c_str() + "          " + str_sx.c_str() + "         " + str_time.c_str() + "\n";

    string results = stations.printReverseRecursive_results();    
    
  ofstream myfile2 (name_file.c_str(), std::ios_base::app);
  if (myfile2.is_open())
  {
	myfile2 << "Problem         Benchmark          Bench#         Method            n         c         c*         m         abs.dev         \%dev         BD         SX         CPU time(sec)\n";
    myfile2 << lines;
    myfile2 << "Tasks allocation to stations: \n";
    myfile2 << results;
    
    myfile2.close();
  }
  else cout << "Unable to open print_albp2 file";
}

void albp1(int algorithm, vector<int> &tasks_time, vector<vector<int> >& depend_tasks, size_t number_of_tasks, int cycle_time, int station_opt, int k) {
    condition_k = number_of_tasks; //used by VNS algorithm

    int ranking[number_of_tasks];
    float random_vns_x[number_of_tasks];

    for (int u = 0; u < number_of_tasks; u++) {
        ranking[u] = 0;
    }

    tasks_completed.clear();
    buf_available.clear();

    stations.eraseAll();

    for (int z = 0; z < number_of_tasks; z++) {

        for (int f = 0; f < number_of_tasks; f++) {
            //duplicate=false;
            available_task = true;
            for (int u = 0; u < number_of_tasks; u++) {
                if (depend_tasks[u][f] != 0) {
                    available_task = false;
                }
            }

            if (available_task == true) {
                duplicate = false;

                for (int c = 0; c < tasks_completed.size(); c++) {
                    if (f == tasks_completed[c]) {
                        duplicate = true;
                    }
                }
                temp_buf_available = buf_available;
                while (!temp_buf_available.empty()) {
                    if (f == temp_buf_available.back()) {
                        duplicate = true;
                    }
                    temp_buf_available.pop_back();
                }

                if (duplicate == false) {
                    buf_available.push_back(f);
                }
            }
        }

        temp_buf_available = buf_available;

        if (buf_available.size() == 1) {

            if (tasks_completed.empty()) {
                stations.addValue(1, cycle_time);

                //1: task number 2: task time
                stations.addToStation(buf_available.back() + 1, tasks_time[buf_available.back()]);

                tasks_completed.push_back(buf_available.back());
                buf_available.pop_back();
            } else {
                stations.addToStation(buf_available.back() + 1, tasks_time[buf_available.back()]);
                tasks_completed.push_back(buf_available.back());
                buf_available.pop_back();
            }
        } else {
            //IMPLEMENTATION OF EACH ALGORITHM
            switch (algorithm) {

                    //Longest task time
                case 0:
                    temp_buf_available.clear();
                    temp_buf_available = buf_available;

                    max_processing_time = 0; //max ones
                    temp_task_time = 0; //following count

                    while (!longest_tasks.empty()) {
                        longest_tasks.pop_back();
                    }

                    while (!temp_buf_available.empty()) {
                        temp_task_time = tasks_time[temp_buf_available.back()];

                        if (max_processing_time < temp_task_time) {
                            max_processing_time = temp_task_time;

                            while (!longest_tasks.empty()) {
                                longest_tasks.pop_back();
                            }

                            longest_tasks.push_back(temp_buf_available.back());
                        } else if (max_processing_time == temp_task_time) {
                            longest_tasks.push_back(temp_buf_available.back());
                        }

                        temp_buf_available.pop_back();
                        temp_task_time = 0;
                    }

                    if (longest_tasks.size() == 1) {

                        if (stations.isEmpty()) {
                            stations.addValue(1, cycle_time);
                        }
                        stations.addToStation(longest_tasks.back() + 1, tasks_time[longest_tasks.back()]);

                        tasks_completed.push_back(longest_tasks.back());
                        buf_available.remove(longest_tasks.back());
                    } else {
                        srand(time(0));
                        random_val = rand() % longest_tasks.size();
                        for (int y = 0; y < random_val; y++) {
                            longest_tasks.pop_front();
                        }

                        if (stations.isEmpty()) {
                            stations.addValue(1, cycle_time);
                        }
                        stations.addToStation(longest_tasks.front() + 1, tasks_time[longest_tasks.front()]);
                        tasks_completed.push_back(longest_tasks.front());
                        buf_available.remove(longest_tasks.front());
                    }
                    break;

                    //shortest task time
                case 1:
                    temp_buf_available.clear();
                    temp_buf_available = buf_available;

                    min_processing_time = 1000000000; //it represents the infinite value
                    temp_task_time = 0;

                    while (!shortest_tasks.empty()) {
                        shortest_tasks.pop_back();
                    }

                    while (!temp_buf_available.empty()) {
                        temp_task_time = tasks_time[temp_buf_available.back()];

                        if (min_processing_time > temp_task_time) {
                            min_processing_time = temp_task_time;

                            while (!shortest_tasks.empty()) {
                                shortest_tasks.pop_back();
                            }

                            shortest_tasks.push_back(temp_buf_available.back());
                        } else if (min_processing_time == temp_task_time) {
                            shortest_tasks.push_back(temp_buf_available.back());
                        }

                        temp_buf_available.pop_back();
                        temp_task_time = 0;
                    }

                    if (shortest_tasks.size() == 1) {

                        if (stations.isEmpty()) {
                            stations.addValue(1, cycle_time);
                        }
                        stations.addToStation(shortest_tasks.back() + 1, tasks_time[shortest_tasks.back()]);

                        tasks_completed.push_back(shortest_tasks.back());
                        buf_available.remove(shortest_tasks.back());
                    } else {
                        srand(time(0));
                        random_val = rand() % shortest_tasks.size();
                        for (int y = 0; y < random_val; y++) {
                            shortest_tasks.pop_front();
                        }

                        if (stations.isEmpty()) {
                            stations.addValue(1, cycle_time);
                        }
                        stations.addToStation(shortest_tasks.front() + 1, tasks_time[shortest_tasks.front()]);
                        tasks_completed.push_back(shortest_tasks.front());
                        buf_available.remove(shortest_tasks.front());
                    }
                    break;

                    //most following task
                case 2:
                    temp_buf_available = buf_available;
                    max_ones = 0;
                    following_count = 0;

                    while (!max_following_tasks.empty()) {
                        max_following_tasks.pop_back();
                    }

                    while (!temp_buf_available.empty()) {
                        for (int a = 0; a < number_of_tasks; a++) {
                            if (depend_tasks[temp_buf_available.back()][a] == 1) {
                                following_count++;
                            }
                        }
                        if (max_ones < following_count) {
                            max_ones = following_count;

                            while (!max_following_tasks.empty()) {
                                max_following_tasks.pop_back();
                            }

                            max_following_tasks.push_back(temp_buf_available.back());
                        } else if (max_ones == following_count) {
                            max_following_tasks.push_back(temp_buf_available.back());
                        }
                        temp_buf_available.pop_back();
                        following_count = 0;
                    }

                    if (max_following_tasks.size() == 1) {

                        if (stations.isEmpty()) {
                            stations.addValue(1, cycle_time);
                        }
                        stations.addToStation(max_following_tasks.back() + 1, tasks_time[max_following_tasks.back()]);

                        tasks_completed.push_back(max_following_tasks.back());
                        buf_available.remove(max_following_tasks.back());
                    } else {
                        srand(time(0));
                        random_val = rand() % max_following_tasks.size();
                        for (int y = 0; y < random_val; y++) {
                            max_following_tasks.pop_front();
                        }

                        if (stations.isEmpty()) {
                            stations.addValue(1, cycle_time);
                        }
                        stations.addToStation(max_following_tasks.front() + 1, tasks_time[max_following_tasks.front()]);
                        tasks_completed.push_back(max_following_tasks.front());
                        buf_available.remove(max_following_tasks.front());
                    }
                    break;

                    //least number of following tasks
                case 3:
                    temp_buf_available = buf_available;
                    min_ones = 100000000; // This value represents the infinite value.
                    following_count = 0;

                    while (!min_following_tasks.empty()) {
                        min_following_tasks.pop_back();
                    }

                    while (!temp_buf_available.empty()) {
                        for (int a = 0; a < number_of_tasks; a++) {
                            if (depend_tasks[temp_buf_available.back()][a] == 1) {
                                following_count++;
                            }
                        }
                        if (min_ones > following_count) {
                            min_ones = following_count;

                            while (!min_following_tasks.empty()) {
                                min_following_tasks.pop_back();
                            }

                            min_following_tasks.push_back(temp_buf_available.back());
                        } else if (min_ones == following_count) {
                            min_following_tasks.push_back(temp_buf_available.back());
                        }
                        temp_buf_available.pop_back();
                        following_count = 0;
                    }

                    if (min_following_tasks.size() == 1) {

                        if (stations.isEmpty()) {
                            stations.addValue(1, cycle_time);
                        }
                        stations.addToStation(min_following_tasks.back() + 1, tasks_time[min_following_tasks.back()]);

                        tasks_completed.push_back(min_following_tasks.back());
                        buf_available.remove(min_following_tasks.back());
                    } else {
                        srand(time(0));
                        random_val = rand() % min_following_tasks.size();
                        //Pop value until the random one task is found
                        for (int y = 0; y < random_val; y++) {
                            min_following_tasks.pop_front();
                        }

                        if (stations.isEmpty()) {
                            stations.addValue(1, cycle_time);
                        }
                        stations.addToStation(min_following_tasks.front() + 1, tasks_time[min_following_tasks.front()]);
                        tasks_completed.push_back(min_following_tasks.front());
                        buf_available.remove(min_following_tasks.front());
                    }
                    break;

                    //ranked positional weight
                case 4:
                    temp_buf_available = buf_available;
                    max_rank = 0;
                    rank_tasks = 0;

                    while (!max_rank_tasks.empty()) {
                        max_rank_tasks.pop_back();
                    }

                    while (!temp_buf_available.empty()) {
                        already_ranked = false;
                        rank_tasks = 0;
                        for (int a = 0; a < number_of_tasks; a++) {

                            if (depend_tasks[temp_buf_available.back()][a] == 1) {
                                if (ranking[temp_buf_available.back()] == 0) {
                                    rank_tasks = tasks_time[a] + rank_tasks;
                                } else {
                                    already_ranked = true;
                                }
                            }
                        }
                        if (!already_ranked) {
                            ranking[temp_buf_available.back()] = rank_tasks + tasks_time[temp_buf_available.back()];
                        }



                        if (max_rank < ranking[temp_buf_available.back()]) {
                            max_rank = ranking[temp_buf_available.back()];

                            while (!max_rank_tasks.empty()) {
                                max_rank_tasks.pop_back();
                            }

                            max_rank_tasks.push_back(temp_buf_available.back());
                        } else if (max_rank == ranking[temp_buf_available.back()]) {
                            max_rank_tasks.push_back(temp_buf_available.back());
                        }
                        //midenise to ranking[temp_buf_available.back()]
                        temp_buf_available.pop_back();
                    }

                    if (max_rank_tasks.size() == 1) {

                        if (stations.isEmpty()) {
                            stations.addValue(1, cycle_time);
                        }
                        stations.addToStation(max_rank_tasks.back() + 1, tasks_time[max_rank_tasks.back()]);

                        tasks_completed.push_back(max_rank_tasks.back());
                        buf_available.remove(max_rank_tasks.back());
                    } else {
                        srand(time(0));
                        random_val = rand() % max_rank_tasks.size();
                        for (int y = 0; y < random_val; y++) {
                            max_rank_tasks.pop_front();
                        }

                        if (stations.isEmpty()) {
                            stations.addValue(1, cycle_time);
                        }
                        stations.addToStation(max_rank_tasks.front() + 1, tasks_time[max_rank_tasks.front()]);
                        tasks_completed.push_back(max_rank_tasks.front());
                        buf_available.remove(max_rank_tasks.front());
                    }
                    //                    for(int d=0;d<number_of_tasks;d++){
                    //                        cout << "the rankiiiinnnng " << ranking[d] << endl;
                    //                    }
                    break;

                    //FCFS
                case 6:
                    temp_buf_available = buf_available;

                    if (stations.isEmpty()) {
                        stations.addValue(1, cycle_time);
                    }
                    stations.addToStation(temp_buf_available.front() + 1, tasks_time[temp_buf_available.front()]);
                    tasks_completed.push_back(temp_buf_available.front());
                    buf_available.remove(temp_buf_available.front());
                    break;

                    //Random
                case 7:
                    temp_buf_available.clear();
                    temp_buf_available = buf_available;
                    g = 0;
                    if (stations.isEmpty()) {
                        stations.addValue(1, cycle_time);
                    }
                    srand(time(0));
                    random_heuristic = rand() % (temp_buf_available.size());
                    //cout << "random_heuristic is " << random_heuristic <<endl; 

                    while (g < (random_heuristic)) {
                        //cout << "erase task: " << temp_buf_available.front() << endl;
                        temp_buf_available.pop_front();
                        g++;
                    }
                    stations.addToStation(temp_buf_available.front() + 1, tasks_time[temp_buf_available.front()]);
                    tasks_completed.push_back(temp_buf_available.front());
                    buf_available.remove(temp_buf_available.front());

                    break;
            }
        }

        // fill with zero's the row that the task completed. No other tasks depends from that row anymore   
        for (int p = 0; p < number_of_tasks; p++) {
            depend_tasks[tasks_completed.back()][p] = 0;
        }

        //        for(int d=0;d<number_of_tasks;d++){
        //            for(int a=0;a<number_of_tasks;a++){
        //                cout<<depend_tasks[d][a]<< " ";
        //            }
        //            cout<<endl;
        //        }
        //        cout << endl;

        //        while(!buf_available.empty()){
        //            cout << buf_available.front() << " " ;
        //            buf_available.pop_front();
        //        }
        //        cout << endl;
        //      //  cout << "tasks completed" <<endl;
        //    for(int r=0;r<tasks_completed.size();r++){
        //        cout << tasks_completed[r] << " ";
        //    }
        //        cout<<endl;

    }
}

int albp1_VNS(int algorithm, vector<int> &tasks_time, vector<vector<int> >& depend_tasks, size_t number_of_tasks, int cycle_time, int station_opt, int k, vector<float> &random_vns_x_vector) {
    tasks_completed.clear();
    buf_available.clear();
    stations.eraseAll();

    for (int z = 0; z < number_of_tasks; z++) {

        for (int f = 0; f < number_of_tasks; f++) {
            //duplicate=false;
            available_task = true;
            for (int u = 0; u < number_of_tasks; u++) {
                if (depend_tasks[u][f] != 0) {
                    available_task = false;
                }
            }

            if (available_task == true) {
                duplicate = false;

                for (int c = 0; c < tasks_completed.size(); c++) {
                    if (f == tasks_completed[c]) {
                        duplicate = true;
                    }
                }
                temp_buf_available = buf_available;
                while (!temp_buf_available.empty()) {
                    if (f == temp_buf_available.back()) {
                        duplicate = true;
                    }
                    temp_buf_available.pop_back();
                }

                if (duplicate == false) {
                    buf_available.push_back(f);
                }
            }
        }

        temp_buf_available = buf_available;

        if (buf_available.size() == 1) {

            if (tasks_completed.empty()) {

                stations.addValue(1, cycle_time);

                //1: task number 2: task time
                stations.addToStation(buf_available.back() + 1, tasks_time[buf_available.back()]);

                tasks_completed.push_back(buf_available.back());
                buf_available.pop_back();
            } else {
                stations.addToStation(buf_available.back() + 1, tasks_time[buf_available.back()]);
                tasks_completed.push_back(buf_available.back());
                buf_available.pop_back();
            }
        } else {
            //VNS GOES HERE
            temp_buf_available = buf_available;
            max_float_value = 0;

            while (!max_vns_tasks.empty()) {
                max_vns_tasks.pop_back();
            }

            while (!temp_buf_available.empty()) {
                if (max_float_value < random_vns_x_vector[temp_buf_available.back()]) {
                    max_float_value = random_vns_x_vector[temp_buf_available.back()];

                    while (!max_vns_tasks.empty()) {
                        max_vns_tasks.pop_back();
                    }

                    max_vns_tasks.push_back(temp_buf_available.back());
                } else if (max_float_value == random_vns_x_vector[temp_buf_available.back()]) {
                    max_vns_tasks.push_back(temp_buf_available.back());
                }

                //midenise to ranking[temp_buf_available.back()]
                temp_buf_available.pop_back();
            }

            if (max_vns_tasks.size() == 1) {
                if (stations.isEmpty()) {
                    stations.addValue(1, cycle_time);
                }
                stations.addToStation(max_vns_tasks.back() + 1, tasks_time[max_vns_tasks.back()]);

                tasks_completed.push_back(max_vns_tasks.back());
                buf_available.remove(max_vns_tasks.back());
            } else {
                srand(time(0));
                random_val = rand() % max_vns_tasks.size();
                for (int y = 0; y < random_val; y++) {
                    max_vns_tasks.pop_front();
                }

                if (stations.isEmpty()) {
                    stations.addValue(1, cycle_time);
                }

                stations.addToStation(max_vns_tasks.front() + 1, tasks_time[max_vns_tasks.front()]);
                tasks_completed.push_back(max_vns_tasks.front());
                buf_available.remove(max_vns_tasks.front());
            }
        }

        // fill with zero's the row that the task completed. No other tasks depends from that row anymore   
        for (int z = 0; z < number_of_tasks; z++) {
            depend_tasks[tasks_completed.back()][z] = 0;
        }
    }
    return stations.getStationsNumb();
}

int albp1_heuristics(vector<int> &tasks_time, vector<vector<int> >& depend_tasks, size_t number_of_tasks, int cycle_time, int station_opt, int k, vector<int> &random_heur_x_vector) {

    //    for(int u=0;u<number_of_tasks;u++){
    //        cout << random_heur_x_vector[u] << " ";
    //    }

    int ranking[number_of_tasks];

    for (int u = 0; u < number_of_tasks; u++) {
        ranking[u] = 0;
    }

    tasks_completed.clear();
    buf_available.clear();
    stations.eraseAll();

    while (tasks_completed.size() != number_of_tasks) {

        for (int f = 0; f < number_of_tasks; f++) {
            //duplicate=false;
            available_task = true;
            for (int u = 0; u < number_of_tasks; u++) {
                if (depend_tasks[u][f] != 0) {
                    available_task = false;
                }
            }

            if (available_task == true) {
                duplicate = false;

                for (int c = 0; c < tasks_completed.size(); c++) {
                    if (f == tasks_completed[c]) {
                        duplicate = true;
                    }
                }
                temp_buf_available = buf_available;
                while (!temp_buf_available.empty()) {
                    if (f == temp_buf_available.back()) {
                        duplicate = true;
                    }
                    temp_buf_available.pop_back();
                }

                if (duplicate == false) {
                    buf_available.push_back(f);
                }
            }
        }//end of available tasks for loop         

        temp_buf_available = buf_available;

        if (buf_available.size() == 1) {

            if (tasks_completed.empty()) {

                stations.addValue(1, cycle_time);

                //1: task number 2: task time
                stations.addToStation(buf_available.back() + 1, tasks_time[buf_available.back()]);

                tasks_completed.push_back(buf_available.back());
                buf_available.pop_back();
            } else {
                stations.addToStation(buf_available.back() + 1, tasks_time[buf_available.back()]);
                tasks_completed.push_back(buf_available.back());
                buf_available.pop_back();
            }
        }
        else {
            //HEURISTICS GOES HERE
            temp_buf_available = buf_available;
            max_float_value = 0;

            current_numb_stations = stations.getStationsNumb();

            //cout << "Current numb" << current_numb_stations << endl;     
            //cout << "Algorithm: " << random_heur_x_vector[current_numb_stations-1] << endl;
            //cout << "available space" << stations.available_space_last_station()<< endl;

            if (tasks_completed.size() == 0) {
                stations.addValue(1, cycle_time);
            }
            switch (random_heur_x_vector[current_numb_stations - 1]) {
                    //LTT
                case 0:
                    temp_buf_available.clear();
                    temp_buf_available = buf_available;

                    max_processing_time = 0; //max ones
                    temp_task_time = 0; //following count

                    while (!longest_tasks.empty()) {
                        longest_tasks.pop_back();
                    }

                    while (!temp_buf_available.empty()) {
                        temp_task_time = tasks_time[temp_buf_available.back()];

                        if (max_processing_time < temp_task_time) {
                            max_processing_time = temp_task_time;

                            while (!longest_tasks.empty()) {
                                longest_tasks.pop_back();
                            }

                            longest_tasks.push_back(temp_buf_available.back());
                        } else if (max_processing_time == temp_task_time) {
                            longest_tasks.push_back(temp_buf_available.back());
                        }

                        temp_buf_available.pop_back();
                        temp_task_time = 0;
                    }

                    if (longest_tasks.size() == 1) {
                        if (tasks_time[longest_tasks.back()] <= stations.available_space_last_station()) {
                            if (stations.isEmpty()) {
                                stations.addValue(1, cycle_time);
                            }
                            stations.addToStation(longest_tasks.back() + 1, tasks_time[longest_tasks.back()]);

                            tasks_completed.push_back(longest_tasks.back());
                            buf_available.remove(longest_tasks.back());
                        } else {
                            //the task doesn't fit in this station so create a new one , and check again this task from buf available (new station => new policy)
                            stations.addValue(stations.getStationsNumb() + 1, cycle_time);
                        }
                    } else {
                        srand(time(0));
                        random_val = rand() % longest_tasks.size();
                        for (int y = 0; y < random_val; y++) {
                            longest_tasks.pop_front();
                        }

                        if (tasks_time[longest_tasks.front()] <= stations.available_space_last_station()) {
                            if (stations.isEmpty()) {
                                stations.addValue(1, cycle_time);
                            }
                            stations.addToStation(longest_tasks.front() + 1, tasks_time[longest_tasks.front()]);

                            tasks_completed.push_back(longest_tasks.front());
                            buf_available.remove(longest_tasks.front());
                        } else {
                            //the task doesn't fit in this station so create a new one , and check again this task from buf available (new station => new policy)
                            stations.addValue(stations.getStationsNumb() + 1, cycle_time);
                        }
                    }
                    break;

                    //STT 
                case 1:
                    temp_buf_available.clear();
                    temp_buf_available = buf_available;

                    min_processing_time = 1000000000; //it represents the infinite value
                    temp_task_time = 0;

                    while (!shortest_tasks.empty()) {
                        shortest_tasks.pop_back();
                    }

                    while (!temp_buf_available.empty()) {
                        temp_task_time = tasks_time[temp_buf_available.back()];

                        if (min_processing_time > temp_task_time) {
                            min_processing_time = temp_task_time;

                            while (!shortest_tasks.empty()) {
                                shortest_tasks.pop_back();
                            }

                            shortest_tasks.push_back(temp_buf_available.back());
                        } else if (min_processing_time == temp_task_time) {
                            shortest_tasks.push_back(temp_buf_available.back());
                        }

                        temp_buf_available.pop_back();
                        temp_task_time = 0;
                    }

                    if (shortest_tasks.size() == 1) {
                        if (tasks_time[shortest_tasks.back()] <= stations.available_space_last_station()) {
                            if (stations.isEmpty()) {
                                stations.addValue(1, cycle_time);
                            }
                            stations.addToStation(shortest_tasks.back() + 1, tasks_time[shortest_tasks.back()]);

                            tasks_completed.push_back(shortest_tasks.back());
                            buf_available.remove(shortest_tasks.back());
                        } else {
                            //the task doesn't fit in this station so create a new one , and check again this task from buf available (new station => new policy)
                            stations.addValue(stations.getStationsNumb() + 1, cycle_time);
                        }
                    } else {
                        srand(time(0));
                        random_val = rand() % shortest_tasks.size();
                        for (int y = 0; y < random_val; y++) {
                            shortest_tasks.pop_front();
                        }

                        if (tasks_time[shortest_tasks.front()] <= stations.available_space_last_station()) {
                            if (stations.isEmpty()) {
                                stations.addValue(1, cycle_time);
                            }
                            stations.addToStation(shortest_tasks.front() + 1, tasks_time[shortest_tasks.front()]);

                            tasks_completed.push_back(shortest_tasks.front());
                            buf_available.remove(shortest_tasks.front());
                        } else {
                            //the task doesn't fit in this station so create a new one , and check again this task from buf available (new station => new policy)
                            stations.addValue(stations.getStationsNumb() + 1, cycle_time);
                        }
                    }

                    break;

                    //MFT
                case 2:
                    temp_buf_available = buf_available;
                    max_ones = 0;
                    following_count = 0;

                    while (!max_following_tasks.empty()) {
                        max_following_tasks.pop_back();
                    }

                    while (!temp_buf_available.empty()) {
                        for (int a = 0; a < number_of_tasks; a++) {
                            if (depend_tasks[temp_buf_available.back()][a] == 1) {
                                following_count++;
                            }
                        }
                        if (max_ones < following_count) {
                            max_ones = following_count;

                            while (!max_following_tasks.empty()) {
                                max_following_tasks.pop_back();
                            }

                            max_following_tasks.push_back(temp_buf_available.back());
                        } else if (max_ones == following_count) {
                            max_following_tasks.push_back(temp_buf_available.back());
                        }
                        temp_buf_available.pop_back();
                        following_count = 0;
                    }

                    if (max_following_tasks.size() == 1) {
                        if (tasks_time[max_following_tasks.back()] <= stations.available_space_last_station()) {
                            if (stations.isEmpty()) {
                                stations.addValue(1, cycle_time);
                            }
                            stations.addToStation(max_following_tasks.back() + 1, tasks_time[max_following_tasks.back()]);

                            tasks_completed.push_back(max_following_tasks.back());
                            buf_available.remove(max_following_tasks.back());
                        } else {
                            //the task doesn't fit in this station so create a new one , and check again this task from buf available (new station => new policy)
                            stations.addValue(stations.getStationsNumb() + 1, cycle_time);
                        }
                    } else {
                        srand(time(0));
                        random_val = rand() % max_following_tasks.size();
                        for (int y = 0; y < random_val; y++) {
                            max_following_tasks.pop_front();
                        }

                        if (tasks_time[max_following_tasks.front()] <= stations.available_space_last_station()) {
                            if (stations.isEmpty()) {
                                stations.addValue(1, cycle_time);
                            }
                            stations.addToStation(max_following_tasks.front() + 1, tasks_time[max_following_tasks.front()]);

                            tasks_completed.push_back(max_following_tasks.front());
                            buf_available.remove(max_following_tasks.front());
                        } else {
                            //the task doesn't fit in this station so create a new one , and check again this task from buf available (new station => new policy)
                            stations.addValue(stations.getStationsNumb() + 1, cycle_time);
                        }
                    }
                    break;

                    //LFT
                case 3:
                    temp_buf_available = buf_available;
                    min_ones = 100000000; // This value represents the infinite value.
                    following_count = 0;

                    while (!min_following_tasks.empty()) {
                        min_following_tasks.pop_back();
                    }

                    while (!temp_buf_available.empty()) {
                        for (int a = 0; a < number_of_tasks; a++) {
                            if (depend_tasks[temp_buf_available.back()][a] == 1) {
                                following_count++;
                            }
                        }
                        if (min_ones > following_count) {
                            min_ones = following_count;

                            while (!min_following_tasks.empty()) {
                                min_following_tasks.pop_back();
                            }

                            min_following_tasks.push_back(temp_buf_available.back());
                        } else if (min_ones == following_count) {
                            min_following_tasks.push_back(temp_buf_available.back());
                        }
                        temp_buf_available.pop_back();
                        following_count = 0;
                    }

                    if (min_following_tasks.size() == 1) {
                        if (tasks_time[min_following_tasks.back()] <= stations.available_space_last_station()) {
                            if (stations.isEmpty()) {
                                stations.addValue(1, cycle_time);
                            }
                            stations.addToStation(min_following_tasks.back() + 1, tasks_time[min_following_tasks.back()]);

                            tasks_completed.push_back(min_following_tasks.back());
                            buf_available.remove(min_following_tasks.back());
                        } else {
                            //the task doesn't fit in this station so create a new one , and check again this task from buf available (new station => new policy)
                            stations.addValue(stations.getStationsNumb() + 1, cycle_time);
                        }
                    } else {
                        srand(time(0));
                        random_val = rand() % min_following_tasks.size();
                        //Pop value until the random one task is found
                        for (int y = 0; y < random_val; y++) {
                            min_following_tasks.pop_front();
                        }

                        if (tasks_time[min_following_tasks.front()] <= stations.available_space_last_station()) {
                            if (stations.isEmpty()) {
                                stations.addValue(1, cycle_time);
                            }
                            stations.addToStation(min_following_tasks.front() + 1, tasks_time[min_following_tasks.front()]);

                            tasks_completed.push_back(min_following_tasks.front());
                            buf_available.remove(min_following_tasks.front());
                        } else {
                            //the task doesn't fit in this station so create a new one , and check again this task from buf available (new station => new policy)
                            stations.addValue(stations.getStationsNumb() + 1, cycle_time);
                        }
                    }
                    break;

                    //RPW
                case 4:
                    temp_buf_available = buf_available;
                    max_rank = 0;
                    rank_tasks = 0;

                    while (!max_rank_tasks.empty()) {
                        max_rank_tasks.pop_back();
                    }

                    while (!temp_buf_available.empty()) {
                        already_ranked = false;
                        rank_tasks = 0;
                        for (int a = 0; a < number_of_tasks; a++) {

                            if (depend_tasks[temp_buf_available.back()][a] == 1) {
                                if (ranking[temp_buf_available.back()] == 0) {
                                    rank_tasks = tasks_time[a] + rank_tasks;
                                } else {
                                    already_ranked = true;
                                }
                            }
                        }
                        if (!already_ranked) {
                            ranking[temp_buf_available.back()] = rank_tasks + tasks_time[temp_buf_available.back()];
                        }

                        if (max_rank < ranking[temp_buf_available.back()]) {
                            max_rank = ranking[temp_buf_available.back()];

                            while (!max_rank_tasks.empty()) {
                                max_rank_tasks.pop_back();
                            }

                            max_rank_tasks.push_back(temp_buf_available.back());
                        } else if (max_rank == ranking[temp_buf_available.back()]) {
                            max_rank_tasks.push_back(temp_buf_available.back());
                        }
                        //midenise to ranking[temp_buf_available.back()]
                        temp_buf_available.pop_back();
                    }

                    if (max_rank_tasks.size() == 1) {
                        if (tasks_time[max_rank_tasks.back()] <= stations.available_space_last_station()) {
                            if (stations.isEmpty()) {
                                stations.addValue(1, cycle_time);
                            }
                            stations.addToStation(max_rank_tasks.back() + 1, tasks_time[max_rank_tasks.back()]);

                            tasks_completed.push_back(max_rank_tasks.back());
                            buf_available.remove(max_rank_tasks.back());
                        } else {
                            //the task doesn't fit in this station so create a new one , and check again this task from buf available (new station => new policy)
                            stations.addValue(stations.getStationsNumb() + 1, cycle_time);
                        }
                    } else {
                        srand(time(0));
                        random_val = rand() % max_rank_tasks.size();
                        for (int y = 0; y < random_val; y++) {
                            max_rank_tasks.pop_front();
                        }

                        if (tasks_time[max_rank_tasks.front()] <= stations.available_space_last_station()) {
                            if (stations.isEmpty()) {
                                stations.addValue(1, cycle_time);
                            }
                            stations.addToStation(max_rank_tasks.front() + 1, tasks_time[max_rank_tasks.front()]);

                            tasks_completed.push_back(max_rank_tasks.front());
                            buf_available.remove(max_rank_tasks.front());
                        } else {
                            //the task doesn't fit in this station so create a new one , and check again this task from buf available (new station => new policy)
                            stations.addValue(stations.getStationsNumb() + 1, cycle_time);
                        }
                    }
                    //                    for(int d=0;d<number_of_tasks;d++){
                    //                        cout << "the rankiiiinnnng " << ranking[d] << endl;
                    //                    }                    
                    break;

                    //FCFS
                case 5:
                    temp_buf_available = buf_available;

                    if (tasks_time[temp_buf_available.front()] <= stations.available_space_last_station()) {
                        if (stations.isEmpty()) {
                            stations.addValue(1, cycle_time);
                        }
                        stations.addToStation(temp_buf_available.front() + 1, tasks_time[temp_buf_available.front()]);

                        tasks_completed.push_back(temp_buf_available.front());
                        buf_available.remove(temp_buf_available.front());
                    } else {
                        //the task doesn't fit in this station so create a new one , and check again this task from buf available (new station => new policy)
                        stations.addValue(stations.getStationsNumb() + 1, cycle_time);
                    }
                    break;

                    //RANDOM
                case 6:
                    temp_buf_available.clear();
                    temp_buf_available = buf_available;
                    g = 0;
                    srand(time(0));
                    random_heuristic = rand() % (temp_buf_available.size());
                    //cout << "random_heuristic is " << random_heuristic <<endl; 

                    while (g < (random_heuristic)) {
                        //cout << "erase task: " << temp_buf_available.front() << endl;
                        temp_buf_available.pop_front();
                        g++;
                    }

                    if (tasks_time[temp_buf_available.front()] <= stations.available_space_last_station()) {
                        if (stations.isEmpty()) {
                            stations.addValue(1, cycle_time);
                        }
                        stations.addToStation(temp_buf_available.front() + 1, tasks_time[temp_buf_available.front()]);

                        tasks_completed.push_back(temp_buf_available.front());
                        buf_available.remove(temp_buf_available.front());
                    } else {
                        //the task doesn't fit in this station so create a new one , and check again this task from buf available (new station => new policy)
                        stations.addValue(stations.getStationsNumb() + 1, cycle_time);
                    }

                    break;
            }
        }

        // fill with zero's the row that the task completed. No other tasks depends from that row anymore   
        for (int z = 0; z < number_of_tasks; z++) {
            depend_tasks[tasks_completed.back()][z] = 0;
        }

        //cout << "Task completed size: " << tasks_completed.size() << endl; 
    }

    return stations.getStationsNumb();
}

void VNS(int algorithm, vector<int> &tasks_time, vector<vector<int> >& depend_tasks, size_t number_of_tasks, int cycle_time, int station_opt, int k) {
    float random_vns_x[number_of_tasks];
    int old_number_of_stations_sol = 0;
    int new_number_of_stations_sol = 0;
    int optimum_number_of_stations_sol = 0;

    condition_k = number_of_tasks; //used by VNS algorithm
    int m = 0; //represents the k in inner condition

    int rand1_exch; //used to swap values in array
    int rand2_exch; //used to swap values in array

    float temp_exchange;
    float temp_shift;

    //cout << "OLD ARRAY" << endl;
    srand(time(0));
    for (int i = 0; i < number_of_tasks; i++) {
        random_vns_x[i] = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);
        //cout << random_vns_x[i] << " ";
    }
    //    cout << endl;

    vector<float> random_vns_x_vector(random_vns_x, random_vns_x + number_of_tasks);

    old_vns_vector = random_vns_x_vector;
    optimum_vns_vector = old_vns_vector;

    for (int j = 0; j < 5; j++) {
        temp_depend_tasks_vns = depend_tasks;

        //find the solution
        old_number_of_stations_sol = albp1_VNS(algorithm, tasks_time, temp_depend_tasks_vns, number_of_tasks, cycle_time, station_opt, k, old_vns_vector);

        m = 0;

        while (m < condition_k) {
            new_vns_vector = old_vns_vector;

            if (m % 2 == 0) //shift
            {
                for (int i = 0; i < m; i++) {
                    temp_shift = new_vns_vector.front();
                    new_vns_vector.erase(new_vns_vector.begin());
                    new_vns_vector.push_back(temp_shift);
                }
            } else //exchange
            {
                srand(time(0));
                for (int i = 0; i < m; i++) {
                    rand1_exch = rand() % number_of_tasks;
                    rand2_exch = rand() % number_of_tasks;

                    temp_exchange = new_vns_vector[rand1_exch];
                    new_vns_vector[rand1_exch] = new_vns_vector[rand2_exch];
                    new_vns_vector[rand2_exch] = temp_exchange;
                }
            }

            temp_depend_tasks_vns = depend_tasks;

            new_number_of_stations_sol = albp1_VNS(algorithm, tasks_time, temp_depend_tasks_vns, number_of_tasks, cycle_time, station_opt, k, new_vns_vector);

            if (new_number_of_stations_sol < old_number_of_stations_sol) {
                old_number_of_stations_sol = new_number_of_stations_sol;
                optimum_vns_vector = new_vns_vector;
            } else {
                m++;
            }
        }
    }
    temp_depend_tasks_vns = depend_tasks;
    optimum_number_of_stations_sol = albp1_VNS(algorithm, tasks_time, temp_depend_tasks_vns, number_of_tasks, cycle_time, station_opt, k, optimum_vns_vector);

    //    cout << "NEW ARRAY" << endl;
    //    for (int i=0; i<number_of_tasks; i++){
    //        cout << optimum_vns_vector[i] << " ";
    //    }
    //    cout << endl;   
}

void Heuristics(int algorithm, vector<int> &tasks_time, vector<vector<int> >& depend_tasks, size_t number_of_tasks, int cycle_time, int station_opt, int k) {
    int random_heuristics_x[number_of_tasks];
    int old_number_of_stations_sol = 0;
    int new_number_of_stations_sol = 0;
    int optimum_number_of_stations_sol = 0;

    condition_k = number_of_tasks; //used by VNS algorithm
    int m = 0; //represents the k in inner condition

    int rand1_exch; //used to swap values in array
    int rand2_exch; //used to swap values in array

    float temp_exchange;
    float temp_shift;

    //cout << "OLD ARRAY" << endl;
    srand(time(0));
    for (int i = 0; i < number_of_tasks; i++) {

        random_heuristics_x[i] = rand() % 7;
        //cout << random_heuristics_x[i] << " ";
    }
    //    cout << endl;

    vector<int> random_heuristics_x_vector(random_heuristics_x, random_heuristics_x + number_of_tasks);

    old_heur_vector = random_heuristics_x_vector;
    optimum_heur_vector = old_heur_vector;

    for (int j = 0; j < 1; j++) {
        temp_depend_tasks_heur = depend_tasks;
        //find the solution
        old_number_of_stations_sol = albp1_heuristics(tasks_time, temp_depend_tasks_heur, number_of_tasks, cycle_time, station_opt, k, old_heur_vector);

        m = 0;
    }

    temp_depend_tasks_heur = depend_tasks;
    optimum_number_of_stations_sol = albp1_heuristics(tasks_time, temp_depend_tasks_heur, number_of_tasks, cycle_time, station_opt, k, optimum_heur_vector);
    
    for (int j = 0; j < 5; j++) {
        temp_depend_tasks_heur = depend_tasks;

        //find the solution
        old_number_of_stations_sol = albp1_heuristics(tasks_time, temp_depend_tasks_heur, number_of_tasks, cycle_time, station_opt, k, old_heur_vector);

        m = 0;
        
        while (m < condition_k) {
            new_heur_vector = old_heur_vector;

            if (m % 2 == 0) //shift
            {
                for (int i = 0; i < m; i++) {
                    temp_shift = new_heur_vector.front();
                    new_heur_vector.erase(new_heur_vector.begin());
                    new_heur_vector.push_back(temp_shift);
                }
            } else //exchange
            {
                for (int i = 0; i < m; i++) {
                    rand1_exch = rand() % number_of_tasks;
                    rand2_exch = rand() % number_of_tasks;

                    temp_exchange = new_heur_vector[rand1_exch];
                    new_heur_vector[rand1_exch] = new_heur_vector[rand2_exch];
                    new_heur_vector[rand2_exch] = temp_exchange;
                }
            }

            temp_depend_tasks_heur = depend_tasks;

            new_number_of_stations_sol = albp1_heuristics(tasks_time, temp_depend_tasks_heur, number_of_tasks, cycle_time, station_opt, k, new_heur_vector);

            if (new_number_of_stations_sol < old_number_of_stations_sol) {
                old_number_of_stations_sol = new_number_of_stations_sol;
                optimum_heur_vector = new_heur_vector;
            } else {
                m++;
            }
        }
    }
    temp_depend_tasks_heur = depend_tasks;
    optimum_number_of_stations_sol = albp1_heuristics(tasks_time, temp_depend_tasks_heur, number_of_tasks, cycle_time, station_opt, k, optimum_heur_vector);


    //    cout << "NEW ARRAY" << endl;
    //    for (int i=0; i<number_of_tasks; i++){
    //        cout << optimum_heur_vector[i] << " ";
    //    }
    //    cout << endl;
}

void functionDepend(vector<vector<int> >* depend_tasks, size_t number_of_tasks, int k) {
    flag = false;
    for (int f = 0; f < number_of_tasks; f++) {
        if (depend_tasks->at(k)[f] == 1) {
            //Check for duplicates before insertion
            temporary = stack_ones;
            while (!temporary.empty()) {
                if (temporary.top() == f) {
                    flag = true;
                    //cout<<"we are here"<<endl;
                    break;
                }
                temporary.pop();
            }
            if (flag == false) {
                stack_ones.push(f);
                stack_navigation.push(f);
            }
        }
    }
    if (!stack_navigation.empty()) {
        temp = stack_navigation.top();
        stack_navigation.pop();

        functionDepend(depend_tasks, number_of_tasks, temp);
        return;
    }
}

int main() {
    vector<int> temp_tasks_time_vector;

    vector<vector<int> > temp_depend_tasks_vector;
    vector<vector<int> > temp_temp_depend_tasks_vector;

    string c; //cycle time
    string m; //number of sttions            
    string line; //dependencies file - just get the line
    string line2; //datasets-01 - just get the line
    string line3; //datasets-01  - get cycle time and number of stations
    string delimiter_eof = "-1,-1";
    size_t pos = 0; //position of space character
    size_t pos2 = line.find('\n');
    size_t pos3 = line.find(' ');
    string first_depend; // left dependency  array[x][]
    string second_depend; // right dependency array[][x]
    string delimiter = ",";

    int numb_tasks;
    int number_of_benchmarks = 0;
    int number_of_algorithms = 8;
    int choice_of_albp;
    int choice_of_albp2; //method of the albp2 problem
    int input_algorithm; //the number of the algorithm
    int size_of_benchmarks_array = 25;

    //The file names of the benchmarks
    string benchmarks[] = {"inputs/ARC83.IN2", "inputs/ARC111.IN2", "inputs/BARTHOL2.IN2", "inputs/BARTHOLD.IN2",
        "inputs/BOWMAN8.IN2", "inputs/BUXEY.IN2", "inputs/GUNTHER.IN2", "inputs/HAHN.IN2", "inputs/HESKIA.IN2", "inputs/JACKSON.IN2", "inputs/JAESCHKE.IN2"
        , "inputs/KILBRID.IN2", "inputs/LUTZ1.IN2", "inputs/LUTZ2.IN2", "inputs/LUTZ3.IN2", "inputs/MANSOOR.IN2", "inputs/MERTENS.IN2", "inputs/MITCHELL.IN2",
        "inputs/MUKHERJE.IN2", "inputs/ROSZIEG.IN2", "inputs/SAWYER30.IN2", "inputs/SCHOLL.IN2", "inputs/TONGE70.IN2", "inputs/WARNECKE.IN2", "inputs/WEE-MAG.IN2"};

    string algorithms[] = {"0. Longest task time", "1. Shortest task time", "2. Most following tasks", "3. Least number of following tasks", "4. Ranked positional weight", "5. VNS technique", "6. Heuristics", "7. Heuristics"};

    cout << "Files: \n" << endl;
  
  //we do this to print all of the benchmarks and erase the "inputs/" 
  for(int z=0;z<size_of_benchmarks_array;z++){
      cout << z << ". " << benchmarks[z].substr(7,'2') << "  "; 
  }
  
  cout << endl;
  cout << endl;
  
  cout << "Please give the name of the benchmark:" << endl;
  cin >> input_number;
  
  cout << endl;
  
  cout << "Choose the problem to be solved:        1. ALBP-1       2.ALBP-2" << endl;
  cin >> choice_of_albp;
  cout << endl;
  
  if(choice_of_albp==2){
      cout << endl;
      cout <<"Choose the method to solve the problem: 1.Lower-Bound  2.Upper-Bound  3.Binary Search"<<endl;  
      cin >> choice_of_albp2;
      cout << endl;
  }
  for (int j=0; j<number_of_algorithms-1; j++)
    cout << algorithms[j] << endl;
  
  cout << endl;
  cout << "Please give the number of the algorithm:" << endl;
  cin >> input_algorithm;
    
  //erase 'inputs/' from benchmarks name 
  input_bench = benchmarks[input_number];
  input_bench.erase(0, 7);
  
  ifstream myfile (benchmarks[input_number].c_str());
    if (myfile.is_open())
    {
     //get the number of Tasks and save them in an array
      getline (myfile,line);
      cout << "Number of TASKS : " << line << endl;
      numb_tasks = atoi(line.c_str());

      //create the array for Tasks durations 
      int tasks_time[numb_tasks];
      int depend_tasks[numb_tasks][numb_tasks];

      //initialization of dependencies table
      for (int h = 0; h < numb_tasks; h++)
              for (int j = 0; j < numb_tasks; j++)
                  depend_tasks[h][j] = 0;

      //get the tasks times and save them
      for(int i=0;i<numb_tasks;i++){
          getline (myfile,line);
          tasks_time[i] = atoi(line.c_str());
          //cout<<tasks_time[i]<<endl;
      }

      //read the dependencies of the tasks(graph) 
      while(getline(myfile, line)){
        //stop reading from the file when the delimiter is found
        if(line.find(delimiter_eof) != string::npos){
            break;
        }

      //split the pairs of dependencies and fill the 2D array with 1's in case of a dependency -> ex 2,3 => array[1][2]=1 starting
      //from 0 not 1 thats why we subtract 1. 
      while ((pos = line.find(delimiter)) != std::string::npos) {
          first_depend = line.substr(0, pos);
          //cout << first_depend << endl;
          line.erase(0, pos + delimiter.length());
          second_depend = line.substr(0, pos2);
          //cout << second_depend << endl;
          depend_tasks[atoi(first_depend.c_str())-1][atoi(second_depend.c_str())-1]=1;
      } 
    }
      
      vector<int> tasks_time_vector(tasks_time,tasks_time  + numb_tasks);
      temp_tasks_time_vector=tasks_time_vector;
      vector<vector<int> > depend_tasks_vector(numb_tasks, vector<int>(numb_tasks));
      
      for (int i = 0; i < numb_tasks; ++i)
      {
          depend_tasks_vector[i].assign(depend_tasks[i], depend_tasks[i] + numb_tasks);
      }

      for(int k=0;k<numb_tasks;k++){
          functionDepend(&depend_tasks_vector,numb_tasks,k);

          while (!stack_navigation.empty()){
              stack_navigation.pop();
          }
          while (!stack_ones.empty()){
              depend_tasks[k][stack_ones.top()]=1;
              stack_ones.pop();
          }
      }

      //print the dependencies for evaluation    
      for (int s = 0; s < numb_tasks; s++){
              for (int q = 0; q < numb_tasks; q++){
                  cout<<depend_tasks[s][q]; 
              }      
              cout<<endl;    
      }
      
      for (int i = 0; i < numb_tasks; ++i)
      {
          depend_tasks_vector[i].assign(depend_tasks[i], depend_tasks[i] + numb_tasks);
      }
      temp_depend_tasks_vector=depend_tasks_vector;
      myfile.close();
    }
    else cout << "Unable to open dependencies file"; 
  
if(choice_of_albp==1){    

    ifstream myfile2 ("inputs/dataset-1.txt");
    if (myfile2.is_open())
    {
        //read cycle times and optimum No.of stations
        while(getline(myfile2, line2)){
            //when the name of the benchmark is found, save cycle time and No.of stations in arrays
            if(line2.find(input_bench) != string::npos){
                number_of_benchmarks = number_of_benchmarks + 1;
            }    
        }
    
      myfile2.close();
    }
    else cout << "Unable to open dataset1 (a) file"; 
        
    int cycle_times[number_of_benchmarks];
    int optimum_stations[number_of_benchmarks];
    int x=0;
    
    ifstream myfile3 ("inputs/dataset-1.txt");
    if (myfile3.is_open())
    {
        //read cycle times and optimum No.of stations
        while(getline(myfile3, line3)){
            //when the name of the benchmark is found, save cycle time and No.of stations in arrays
            if(line3.find(input_bench) != string::npos){
                    //tokenize line to cycle time and optimum stations number.
                        
                    //erase from line the first word that represents the benchmark name and the space
                    line3.erase(0, line3.find(" ")+1);

                    //stations number is equal to the number after space and before new line                    
                    m = line3.substr(line3.find(" ")+1, pos2); // token is "scott"
                    //erase the station number from line => the only thing remaining is the cycle time
                    c = line3.erase(line3.find(" ")+1, pos2);
                                               
                    //cycle time = characters after the first space and before the second one       
                    cycle_times[x] = atoi(c.c_str());

                    //stations number = characters after the second space and before the new line (pos2)                       
                    optimum_stations[x] = atoi(m.c_str());
                        
                    x++;
            }    
        }
      myfile3.close();
    }
    else cout << "Unable to open dataset1 file"; 
    
//        for (int i=0; i<number_of_benchmarks; i++){
//            cout << "m " << optimum_stations[i] << endl;
//            cout << "c " << cycle_times[i] << endl;
//        }
    
    cout << endl;
    cout << endl;  
    
    for (int i=0; i<number_of_benchmarks; i++){
           cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;

           cout << "Running benchmark with Number: " << i+1 << endl;
           
           temp_temp_depend_tasks_vector = temp_depend_tasks_vector;    
           
           const clock_t begin_time = clock();
           
           if(input_algorithm==5){
                VNS(input_algorithm,temp_tasks_time_vector,temp_temp_depend_tasks_vector,numb_tasks,cycle_times[i],optimum_stations[i],i); 
           }
           else if(input_algorithm==6){
                Heuristics(input_algorithm,temp_tasks_time_vector,temp_temp_depend_tasks_vector,numb_tasks,cycle_times[i],optimum_stations[i],i); 
           }
           else{
                albp1(input_algorithm,temp_tasks_time_vector,temp_temp_depend_tasks_vector,numb_tasks,cycle_times[i],optimum_stations[i],i);   
           }
           
           cpu_time = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
           print_result_of_albp1(choice_of_albp,algorithms[input_algorithm],input_bench,i,numb_tasks,cycle_times[i],optimum_stations[i],stations.getStationsNumb(),stations.BD(),stations.SX(),cpu_time);

//          stations.printReverse();
//           cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
    }
}//end of albp1 problem
else{
    //albp2 goes here!!!!!!!!!!!!!!!!
    ifstream myfile4 ("inputs/dataset-2.txt");
    if (myfile4.is_open())
    {
        //read cycle times and optimum No.of stations
        while(getline(myfile4, line2)){
            //when the name of the benchmark is found, save cycle time and No.of stations in arrays
            if(line2.find(input_bench) != string::npos){
                //cout << line2 <<endl;
                number_of_benchmarks = number_of_benchmarks + 1;
            }    
        }
    
      myfile4.close();
    }
    else cout << "Unable to open dataset2 (a) file"; 
        
    int station_numbs[number_of_benchmarks];
    int optimum_cycles[number_of_benchmarks];
    int x=0;
    
    ifstream myfile5 ("inputs/dataset-2.txt");
    if (myfile5.is_open())
    {
        //read cycle times and optimum No.of stations
        while(getline(myfile5, line3)){
            //when the name of the benchmark is found, save cycle time and No.of stations in arrays
            if(line3.find(input_bench) != string::npos){
                    //tokenize line to cycle time and optimum stations number.
                        
                    //erase from line the first word that represents the benchmark name and the space
                    line3.erase(0, line3.find(" ")+1);

                    //stations number is equal to the number after space and before new line                    
                    m = line3.substr(line3.find(" ")+1, pos2); // token is "scott"
                    //erase the station number from line => the only thing remaining is the cycle time
                    c = line3.erase(line3.find(" ")+1, pos2);
                                               
                    //cycle time = characters after the first space and before the second one       
                    station_numbs[x] = atoi(c.c_str());

                    //stations number = characters after the second space and before the new line (pos2)                       
                    optimum_cycles[x] = atoi(m.c_str());
                        
                    x++;
            }    
        }   
      myfile5.close();
    }
    else cout << "Unable to open datasets2 file"; 
    
//        for (int i=0; i<number_of_benchmarks; i++){
//            cout << "c* " << optimum_cycles[i] << endl;
//            cout << "m " << station_numbs[i] << endl;
//        }
    
    for (int i=0; i<numb_tasks;i++){
        if (temp_tasks_time_vector[i] > Pmax){
            Pmax = temp_tasks_time_vector[i];
        }
        Psum = Psum + temp_tasks_time_vector[i];
    }
    cout << endl;
    
    //execution of the albp2 problem for each benchmark
    for (int i=0; i<number_of_benchmarks; i++){
           cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;

           cout << "Running benchmark with Number: " << i+1 << endl;
           
           const clock_t begin_time = clock();
           
           //calculate LBc - pick as value of LBc the max value of Pmax and Psum   
            if (Pmax > (Psum/(station_numbs[i]))){     
                LBc = Pmax;
            }
            else{
                LBc = Psum/(station_numbs[i]);
            }
           
           //calculate UBc - pick as value of UBc the max value of Pmax and 2*Psum   
            if (Pmax > ((2*Psum)/(station_numbs[i]))){     
                UBc = Pmax;
            }
            else{
                UBc = 2*(Psum/(station_numbs[i]));
            }
           
           //calculate MIDC - pick as value the average of UBc and LBc   
           midc = (UBc + LBc)/2;

           //NOTE: LBc, UBc and MIDC are all floored values !!!!

            temp_temp_depend_tasks_vector = temp_depend_tasks_vector;           

            switch (choice_of_albp2){

                //Lower Bound
                case 1:
                    //check if you found the given number of stations
                    do{    
					   temp_temp_depend_tasks_vector = temp_depend_tasks_vector;
                   
                        if(input_algorithm==5){
                             VNS(input_algorithm,temp_tasks_time_vector,temp_temp_depend_tasks_vector,numb_tasks,LBc,station_numbs[i],i); 
                        }
                        else if(input_algorithm==6){
                             Heuristics(input_algorithm,temp_tasks_time_vector,temp_temp_depend_tasks_vector,numb_tasks,LBc,station_numbs[i],i); 
                        }
                        else{
                             albp1(input_algorithm,temp_tasks_time_vector,temp_temp_depend_tasks_vector,numb_tasks,LBc,station_numbs[i],i);
                        }                   
                        LBc = LBc + 1;
                        
                        //cout << "stations numb: " << stations.getStationsNumb() << endl;
                        //cout << "given stations numb: " << station_numbs[i] << endl;
                    }while (stations.getStationsNumb() != station_numbs[i]);
                    
                    cpu_time = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
                    print_result_of_albp2(choice_of_albp,choice_of_albp2,algorithms[input_algorithm],input_bench,i,numb_tasks,LBc,optimum_cycles[i],station_numbs[i],stations.getStationsNumb(),stations.BD(),stations.SX(),cpu_time);
                    
                    break;

                //Upper Bound
                case 2:
                    do{  
					    temp_temp_depend_tasks_vector = temp_depend_tasks_vector;
                     
                        if(input_algorithm==5){
                             VNS(input_algorithm,temp_tasks_time_vector,temp_temp_depend_tasks_vector,numb_tasks,UBc,station_numbs[i],i); 
                        }
                        else if(input_algorithm==6){
                             Heuristics(input_algorithm,temp_tasks_time_vector,temp_temp_depend_tasks_vector,numb_tasks,UBc,station_numbs[i],i); 
                        }
                        else{
                             albp1(input_algorithm,temp_tasks_time_vector,temp_temp_depend_tasks_vector,numb_tasks,UBc,station_numbs[i],i);
                        }               
                        UBc = UBc - 1;
                        
                        //cout << "stations numb: " << stations.getStationsNumb() << endl;
                        //cout << "given stations numb: " << station_numbs[i] << endl;
                    }while (stations.getStationsNumb() != station_numbs[i]);
                    
                    cpu_time = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
                    print_result_of_albp2(choice_of_albp,choice_of_albp2,algorithms[input_algorithm],input_bench,i,numb_tasks,UBc,optimum_cycles[i],station_numbs[i],stations.getStationsNumb(),stations.BD(),stations.SX(),cpu_time);
                    
                    break;

                //Binary Search
                case 3:
                    do{   
					    temp_temp_depend_tasks_vector = temp_depend_tasks_vector;
                    
                        if(input_algorithm==5){
                             VNS(input_algorithm,temp_tasks_time_vector,temp_temp_depend_tasks_vector,numb_tasks,midc,station_numbs[i],i); 
                        }
                        else if(input_algorithm==6){
                             Heuristics(input_algorithm,temp_tasks_time_vector,temp_temp_depend_tasks_vector,numb_tasks,midc,station_numbs[i],i); 
                        }
                        else{
                             albp1(input_algorithm,temp_tasks_time_vector,temp_temp_depend_tasks_vector,numb_tasks,midc,station_numbs[i],i);
                        }               
                        
                        if(stations.getStationsNumb() < station_numbs[i]){
                            //calculate MIDC - [LBc...midc-1]
                            midc = (LBc + (midc-1))/2; 
                        }
                        else if(stations.getStationsNumb() > station_numbs[i]){
                            //calculate MIDC - - [midc+1...UBc]  
                            midc = ((midc+1) + UBc)/2;
                        }
                        else{
                            //do nothing
                        }  
                        //cout << "stations numb: " << stations.getStationsNumb() << endl;
                        //cout << "given stations numb: " << station_numbs[i] << endl;
                    }while(stations.getStationsNumb() != station_numbs[i]);
                    
                    cpu_time = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
                    print_result_of_albp2(choice_of_albp,choice_of_albp2,algorithms[input_algorithm],input_bench,i,numb_tasks,midc,optimum_cycles[i],station_numbs[i],stations.getStationsNumb(),stations.BD(),stations.SX(),cpu_time);
                    
                    break;
            }
//           stations.printReverse();   
//           cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
        }
}
  return 0;
}