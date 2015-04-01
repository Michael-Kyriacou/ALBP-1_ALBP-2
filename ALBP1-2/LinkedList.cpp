/* 
 * File:   LinkedList.cpp
 * Author: Michael
 * 
 * Created on March 7, 2015, 7:26 PM
 */


#include "LinkedList.h"

using namespace std;

LinkedList::LinkedList() {
    head = NULL; // set head to NULL
    //head->ST = 0;
}
int LinkedList::available_space_last_station(){
    int available_space=0;
    Node *temp = head;
    
    available_space = temp->cycle_time - temp->ST;
    
    return available_space;
}

void LinkedList::addToStation(int task, int task_time){
        if (task_time <= head->temp_cycle_time){
            head->station.push(task);
            head->temp_cycle_time = head->temp_cycle_time - task_time;
            head->ST = head->ST + task_time;
        }
        else {
            addValue((head->m_number)+1 , head->cycle_time);
            addToStation(task, task_time);
        }
    }
float LinkedList::BD(){
    Node *temp = head;
    float bd_result = 0;
    
    while (temp != NULL) {
//        cout << "the time of the station is " << temp->ST << " ";
        bd_result = bd_result + (temp->cycle_time - temp->ST);
        temp = temp->next;
    }
    
    return bd_result;        
}
float LinkedList::SX(){
    Node *temp = head;
    float sx_result = 0;
    
    while (temp != NULL) {
        sx_result = sx_result + pow((temp->cycle_time - temp->ST),2.0);

        temp = temp->next;
    }
    sx_result = sqrt(sx_result);
    return sx_result;        
}

// This prepends a new value at the beginning of the list
void LinkedList::addValue(int val, int cycle){
        Node *n = new Node();   // create new Node
        n->m_number = val;             // set value
        n->next = head;         // make the node point to the next node.
                                //  If the list is empty, this is NULL, so the end of the list --> OK
        n->ST = 0;
        n->cycle_time = cycle;
        n->temp_cycle_time = cycle;
        head = n;               // last but not least, make the head point at the new node.
    }
    
void LinkedList::printList(){
        Node* tmp = head;
          cout << tmp->m_number << endl;

        while(tmp->next != NULL) {
          tmp = tmp->next;
          cout << tmp->m_number << endl;
        }
    }

void LinkedList::printReverse()
    {
        printReverseRecursive(head);
    }

void LinkedList::printReverseRecursive(Node *n)
    {        
        if(n) {
            printReverseRecursive(n->next);
            cout << "Station Number" << endl;
            cout << n->m_number << endl;
            cout << "Stack inside of the node" << endl;
            n->temp = n->station;
            
            while (!n->temp.empty()) {
                cout << n->temp.top() << '\n';
                n->temp.pop();
            }
            cout << endl;
        }
    }

string LinkedList::IntToString (int a)
{
    ostringstream temp;
    temp<<a;
    return temp.str();
}

string* LinkedList::getStations() {
    Node* n = head;
    //string* stations_tasks = new string[getStationsNumb()];
    
    int size = 0;
    size = LinkedList::getStationsNumb();
    
    cout << size;
    string *stations_tasks = new string[size]; //right
    //string stations_tasks[size];
    
    //cout << "hi"<<endl;
    for (int i=0; i< size; i++){
        
        n->temp = n->station;
            
        while (!n->temp.empty()) {
            stations_tasks[i].append(LinkedList::IntToString(n->temp.top())); 
            //stations_tasks[i].append("\n");
            //cout << stations_tasks[i] << endl;
            n->temp.pop();
        }
        n = n->next;
    }

    return stations_tasks;
}

string LinkedList::printReverseRecursive_results()
    {      
        Node *n = head;
        string buffer = "";
        
        while (n != NULL){
            buffer.append("Station ");
            buffer.append(IntToString(n->m_number)+" : ");
            buffer.append("Tasks : ");

            n->temp = n->station;
            
            while (!n->temp.empty()) {   
                buffer.append(IntToString(n->temp.top())+" ");
                n->temp.pop();
            }
            
            n = n->next;           
            buffer.append("\n");
        }     
        buffer.append("\n");
        
        //cout << buffer;
        return buffer;
}

bool LinkedList::isEmpty(){
    if (head == NULL)
        return true;
    else
        return false;
}

void LinkedList::eraseAll(){
    Node *temp = head;
    
    /* Traverse the list and delete the node one by one from the head */
    while (temp != NULL) {
        /* take out the head node */
        head = head->next;
        delete temp;
        /* update the head node */
        temp = head;
    }
    /* Reset the head and tail node */
    head = NULL;
}


int LinkedList::getStationsNumb(){
    Node *temp = head;
    int count_stations = 0;
    
    while (temp != NULL) {
        temp = temp->next;
        
        count_stations++;
    }
    
    return count_stations;
}
    
    // returns the first element in the list and deletes the Node.
    // caution, no error-checking here!
int LinkedList::popValue(){
        Node *n = head;
        int ret = n->m_number;

        head = head->next;
        delete n;
        return ret;
    }


LinkedList::LinkedList(const LinkedList& orig) {
}

LinkedList::~LinkedList() {
}

