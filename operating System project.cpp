#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;
void firstmenu();
void methodmenu();
void preemptivemode();
void showresult();
void fcfs();
void sjf();
void priority();
void rr();

struct process *createprocess(int, int, int);
void displayprocess(struct process *);
struct process *addprocess(struct process *, int, int, int);
void sortarrivaltime(process**); 

struct process
{
    int bursttime;
    int arrivaltime;
    int priority;
    struct process *next;
};

int main(int argc, char *argv[])
{   
    
    
    /*while ((opt = getopt(argc, argv, "nt:")) != -1)
     {
        switch (opt) {
        case 'n':
            flags = 1;
            break;
        case 't':
            nsecs = atoi(optarg);
            tfnd = 1;
            break;
        default: 
            fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }*/
        
   /*cout << "argc is: " << argc << endl;
    for (int i = 0; i < argc; i++)
    {
        cout << i << "\t" << argv[i] << endl;
    }*/ 
    

    //return 0;

    string inputfilename = "input.txt";
    ifstream input(inputfilename);
    string line;
    struct process *header=NULL;


    while (getline(input, line))
    {
        replace(line.begin(), line.end(), ':', ' ');
        istringstream linestream(line);
        int bursttime, arrivaltime, priority;
        if (!(linestream >> bursttime >> arrivaltime >> priority))
        {
            break;
        }
    
        addprocess(header,bursttime, arrivaltime, priority);
        // cout << bursttime << '\t' << arrivaltime << '\t' << priority << endl;
    }

    // firstmenu();

    return 0;
}

void firstmenu()
{
    // menue for the user
    cout << "CPU Scheduler Simulator" << endl;
    cout << "1-Scheduling Method" << endl;
    cout << "2-Preemptive Mode" << endl;
    cout << "3-Show Result" << endl;
    cout << "4-End Program";
    int firstmenuanswer;
    cin >> firstmenuanswer;
    switch (firstmenuanswer)
    {
    case 1:
        methodmenu();
        break;
    case 2:
        preemptivemode();
        break;
    case 3:
        showresult();
        break;
    case 4:
        exit(1);
        break;
    default:
        cout << "please select between the menu choices (1-4)";
    }
}
void methodmenu()
{
    // methodmenue for the user
    cout << "1-None of scheduling method chosen" << endl;
    cout << "2-First Come,First Served Scheduling" << endl;
    cout << "3-Shortest-Job-First Scheduling" << endl;
    cout << "4-Priority Scheduling" << endl;
    cout << "5-Round-Robin Scheduling (You should also obtain time quantum value)" << endl;
    int methodmenuanswer;
    cin >> methodmenuanswer;
    switch (methodmenuanswer)
    {
    case 1:
        firstmenu();
        break;
    case 2:
        fcfs();
        break;
    case 3:
        sjf();
        break;
    case 4:
        priority();
        break;
    case 5:
        rr();
        break;
    default:
        cout << "please select between the mene choices (1-5)";
    }
}

void preemptivemode()
{
    firstmenu();
}
void showresult()
{
    firstmenu();
}
void fcfs()
{   

    firstmenu();
}
void sjf()
{
    firstmenu();
}
void priority()
{
    firstmenu();
}
void rr()
{
    firstmenu();
}
struct process *createprocess(int bursttime, int arrivaltime, int priority)
{
    struct process *temp;
    temp = (struct process *)malloc(sizeof(process));
    temp->bursttime = bursttime;
    temp->arrivaltime = arrivaltime;
    temp->priority = priority;
    temp->next = NULL;
    return temp;
}
struct process *insertprocess(struct process *header, int bursttime, int arrivaltime, int priority)
{
    struct process *temp = createprocess(bursttime, arrivaltime, priority);
    struct process *headertemp;
    if (header == NULL)
    {
        header = temp;
        return header;
    }
    headertemp = header;
    while (headertemp->next != NULL)
        headertemp = headertemp->next;
    headertemp->next = temp;
    return header;
}
void sortarrivaltime(process** header) 
{ 
   
    process* previous = (*header); 
    process* currunt = (*header)->next; 
  
    while (currunt != NULL) 
    { 
     
        if (currunt->arrivaltime < previous->arrivaltime) 
        { 
            previous->next = currunt->next; 
            currunt->next = (*header); 
            (*header) = currunt; 
            currunt = previous; 
        } 

        else
            previous = currunt; 
        currunt = currunt->next; 
    } 
} 
  