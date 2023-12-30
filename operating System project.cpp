#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

void firstmenu(struct process *, int);
int *methodmenu(string *, bool, struct process *, int);
void preemptivemode(bool *, string, struct process *);
void showresult(int *, int);

int *fcfs(bool, struct process *, int);
int *fcfsnonepreemptive(struct process *, int);

int *sjfnonpreemptive(bool, struct process *, int);
int *sjfpreemptive(bool, struct process *, int);

void priority();
void rr();

struct process *createprocess(int, int, int);
struct process *insertprocess(struct process *, int, int, int);
void displayprocess(struct process *);

void arrivaltimesort(struct process **);
void bursttimesort(struct process **);
struct process *swap(struct process *, struct process *);

struct process
{
    int bursttime;
    int arrivaltime;
    int priority;
    struct process *next;
};

int main(int argc, char *argv[])
{
    char opt;
    bool fselected, oselected;
    string inputfile, outputfile;
    ifstream inputfilename;
    ofstream outputfilename;

    while ((opt = getopt(argc, argv, "f:o:")) != -1)
    {
        switch (opt)
        {
        case 'f':
            inputfile = optarg;
            inputfilename = ifstream(inputfile);
            fselected = true;
            break;
        case 'o':
            outputfile = optarg;
            outputfilename = ofstream(outputfile);
            oselected = true;

            break;
        default:
            cout << "wrong argument entered!!";
        }
    }

    if (!fselected || !oselected)
    {
        cout << "you have to enter arguments f and o to continue" << endl;
        exit(1);
    }

    string line;
    struct process *header = NULL;

    while (getline(inputfilename, line))
    {
        replace(line.begin(), line.end(), ':', ' ');
        istringstream linestream(line);
        int bursttime, arrivaltime, priority;
        if (!(linestream >> bursttime >> arrivaltime >> priority))
        {
            break;
        }

        header = insertprocess(header, bursttime, arrivaltime, priority);
    }

    struct process *temp = header;
    int count = 0;
    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }
    /*just for test and should be cleared
    cout << "this is the normal list" << endl;
    displayprocess(header);
    arrivaltimesort(&header);
    cout << "this is the sorted linked list " << endl;
    displayprocess(header);
        cout << "this is the normal list" << endl;
    displayprocess(header);
    bursttimesort(&header);
    cout << "this is the sorted linked list " << endl;
    displayprocess(header);*/

    firstmenu(header, count);

    return 0;
}

void firstmenu(struct process *header, int count)
{
    string selectedmethod = "1-None of scheduling method chosen";
    bool preemptivem = false;
    while (true)
    {
        // menue for the user
        cout << endl
             << "****CPU Scheduler Simulator****" << endl;
        cout << "1-Scheduling Method" << endl;
        cout << "2-Preemptive Mode" << endl;
        cout << "3-Show Result" << endl;
        cout << "4-End Program" << endl
             << endl;
        int firstmenuanswer;
        cin >> firstmenuanswer;
        cout << endl;
        int *waitingtime;
        switch (firstmenuanswer)
        {
        case 1:
            waitingtime = methodmenu(&selectedmethod, preemptivem, header, count);
            break;
        case 2:
            preemptivemode(&preemptivem, selectedmethod, header);
            break;
        case 3:
            if (selectedmethod == "1-None of scheduling method chosen")
            {
                cout << "please first select a scheduling method " << endl;
                break;
            }
            showresult(waitingtime, count);
            break;
        case 4:

            exit(1);

            break;
        default:
            cout << "please select between the menu choices (1-4)";
        }
    }
}

int *methodmenu(string *selectedmethod, bool preemptivem, struct process *header, int count)
{
    // methodmenue for the user

    cout << endl
         << "****select one of the scheduling method that you want to test****" << endl;
    cout << "1-None of scheduling method chosen" << endl;
    cout << "2-First Come,First Served Scheduling" << endl;
    cout << "3-Shortest-Job-First Scheduling" << endl;
    cout << "4-Priority Scheduling" << endl;
    cout << "5-Round-Robin Scheduling (You should also obtain time quantum value)" << endl
         << endl;
    int methodmenuanswer;
    cin >> methodmenuanswer;
    cout << endl;
    switch (methodmenuanswer)
    {
    case 1:
        *selectedmethod = "1-None of scheduling method chosen";
        break;
    case 2:
        *selectedmethod = "2-First Come,First Served Scheduling";
        return fcfs(preemptivem, header, count);
        break;
    case 3:
        *selectedmethod = "3-Shortest-Job-First Scheduling";
        if (preemptivem)
        {
            return sjfpreemptive(preemptivem, header, count);
        }
        else
        {
            return sjfnonpreemptive(preemptivem, header, count);
        }

        break;
    case 4:
        *selectedmethod = "4-Priority Scheduling";
        break;
    case 5:
        *selectedmethod = "5-Round-Robin Scheduling (You should also obtain time quantum value)";
        break;
    default:
        cout << "please select between the mene choices (1-5)";
    }
    return NULL;
}

void preemptivemode(bool *preemptivem, string selectedmethod, struct process *header)
{
    if (selectedmethod == "1-None of scheduling method chosen")
    {
        cout << "please first select a scheduling method " << endl;
        return;
    }
    else
    {
        cout << endl;
        cout << "***please select the mode you want*** " << endl;
        cout << "1-preemptive mode(on)" << endl;
        cout << "2-nonepreemptive mode (off)" << endl;
        cout << "3-cancel" << endl
             << endl;
    }
    int preemptivemodeanswer;
    cin >> preemptivemodeanswer;
    cout << endl;
    switch (preemptivemodeanswer)
    {
    case 1:
        *preemptivem = true;

        break;

    case 2:
        *preemptivem = false;
        break;
    case 3:
        break;

    default:
        cout << "please select from the numbers (1-3)";
        break;
    }
}
void showresult(int *waitingtime, int count)
{
    float sum = 0;
    cout << "process waiting times:" << endl;
    for (int i = 0; i < count; i++)
    {
        cout << "p" << i + 1 << ": " << waitingtime[i] << "ms" << endl;
        sum += waitingtime[i];
    }
    float averagewaitingtime = sum / count;
    cout << "average waiting time:  " << averagewaitingtime << "ms" << endl;
}
int *fcfs(bool preemptivem, struct process *header, int count)
{
    if (preemptivem)
    {
        cout << "first come first served is not supporting preemptive mode";
        return NULL;
    }
    else
    {
        return fcfsnonepreemptive(header, count);
    }
}
int *sjfnonpreemptive(bool preemptivem, process *header, int count)
{

    bursttimesort(&header);
    displayprocess(header);
    return NULL;
}
int *sjfpreemptive(bool preemptivem, process *header, int count)
{
}
void priority()
{
}
void rr()
{
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
void displayprocess(struct process *header)
{
    if (header == NULL)
        cout << "List is empty" << endl;
    struct process *temp = header;
    while (temp != NULL)
    {
        cout << temp->bursttime << " , " << temp->arrivaltime << " , " << temp->priority << " --> ";
        temp = temp->next;
    }
    cout << endl;
}
struct process *swap(struct process *ptr1, struct process *ptr2)
{
    struct process *tmp = ptr2->next;
    ptr2->next = ptr1;
    ptr1->next = tmp;
    return ptr2;
}

void arrivaltimesort(struct process **header)
{
    int count = 0;
    struct process *temp = *header;
    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }
    struct process **h;
    int i, j, swapped;

    for (i = 0; i <= count; i++)
    {

        h = header;
        swapped = 0;

        for (j = 0; j < count - i - 1; j++)
        {

            struct process *p1 = *h;
            struct process *p2 = p1->next;

            if (p1->arrivaltime > p2->arrivaltime)
            {

                *h = swap(p1, p2);
                swapped = 1;
            }

            h = &(*h)->next;
        }
        if (swapped == 0)
            break;
    }
}
void bursttimesort(struct process **header)
{
    int count = 0;
    struct process *temp = *header;
    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }
    struct process **h;
    int i, j, swapped;

    for (i = 0; i <= count; i++)
    {

        h = header;
        swapped = 0;

        for (j = 0; j < count - i - 1; j++)
        {

            struct process *p1 = *h;
            struct process *p2 = p1->next;

            if (p1->bursttime > p2->bursttime)
            {

                *h = swap(p1, p2);
                swapped = 1;
            }

            h = &(*h)->next;
        }
        if (swapped == 0)
            break;
    }
}
int *fcfsnonepreemptive(struct process *header, int count)
{
    arrivaltimesort(&header);
    struct process *temp = header;

    int startingtime = 0;

    int *waitingtime = (int *)malloc(count * sizeof(int));
    int counter = 0;
    while (temp != NULL)
    {
        if (startingtime < temp->arrivaltime)
        {
            startingtime = temp->arrivaltime;
        }
        waitingtime[counter++] = startingtime - temp->arrivaltime;
        // cout<<"this is the waiting time of procces "<<counter<<waitingtime[counter-1]<<endl;
        startingtime += temp->bursttime;
        temp = temp->next;
    }
    return waitingtime;
}