/* 
 * File:   LinkedList.h
 * Author: Michael
 *
 * Created on March 7, 2015, 7:26 PM
 */

#ifndef LINKEDLIST_H
#define	LINKEDLIST_H

#include <stack>          // std::stack
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>          // std::stack
#include <vector>
#include <math.h>       /* pow */
#include <sstream>

using namespace std;

class LinkedList {
   struct Node {
        int m_number;
        int cycle_time;
        int temp_cycle_time;
        int ST; //
        Node *next;
        stack<int> station; 
        stack<int> temp;
    };
    
public:
    LinkedList();
    string IntToString(int a);
    void addToStation(int task, int task_time);
    void addValue(int val, int cycle);
    bool isEmpty();
    float BD();
    float SX();
    void eraseAll();
    void printList();
    void printReverse();
    int available_space_last_station();
    string* getStations();
    void printReverseRecursive(Node *n);
    string printReverseRecursive_results();
    int getStationsNumb();
    int popValue();
    LinkedList(const LinkedList& orig);
    virtual ~LinkedList();
private:
    Node *head; // this is the private member variable. It is just a pointer to the first Node
};

#endif	/* LINKEDLIST_H */

