#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;
//menue functions 
void firstmenu(struct process *, int, string);
void methodmenu(string *, bool, struct process **, int);

void preemptivemode(bool *, string, struct process *);
void showresult(struct process **, int, string);
//first come first served functions
void fcfs(bool, struct process **, int);
void fcfsnonpreemptive(struct process **, int);
//shortest job first functions 
void sjfnonpreemptive(bool, struct process **, int);
void sjfpreemptive(bool, struct process **, int);
//priority scheduling functions 
void prioritynonpreemptive(bool, struct process **, int);
void prioritypreemptive(bool, struct process **, int);
//roundrobbin functions
void rr();
//linked list functions
struct process *createprocess(int, int, int);
struct process *insertprocess(struct process *, int, int, int);
void displayprocess(struct process *);
//sort functions 
void pidsort(struct process **);
void arrivaltimesort(struct process **);
void bursttimesort(struct process **);
void prioritysort(struct process **);
struct process *swap(struct process *, struct process *);
//linked list data structure
struct process
{
    int processid;
    int bursttime;
    int arrivaltime;
    int priority;
    int waitingtime;
    bool isdone;
    struct process *next;
};
//main function
int main(int argc, char *argv[])
{
    char opt;
    bool fselected, oselected;
    string inputfilename, outputfilename;
    ifstream inputfile;
//getting arguments
    while ((opt = getopt(argc, argv, "f:o:")) != -1)
    {
        switch (opt)
        {
        case 'f':
            inputfilename = optarg;
            inputfile = ifstream(inputfilename);
            fselected = true;
            break;
        case 'o':
            outputfilename = optarg;
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
//read the file and insert it into the linked list 
    while (getline(inputfile, line))
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
    //count the number of the processes inside the linked list 
    while (temp != NULL)
    {
        count++;
        temp->processid = count;
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

    firstmenu(header, count, outputfilename);

    return 0;
}

void firstmenu(struct process *header, int count, string outputfilename)
{
    string selectedmethod = "1-None of scheduling method chosen";
    bool preemptivem = 0;
    while (true)
    {
        // menue for the user
        cout << endl
             << "*CPU Scheduler Simulator*" << endl;
        cout << "1-Scheduling Method" << endl;
        cout << "2-Preemptive Mode" << endl;
        cout << "3-Show Result" << endl;
        cout << "4-End Program" << endl
             << endl;
        int firstmenuanswer;
        cout << "your choice"
             << ">>";
        cin >> firstmenuanswer;
        cout << endl;

        switch (firstmenuanswer)
        {
        case 1:
            methodmenu(&selectedmethod, preemptivem, &header, count);
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
            showresult(&header, count, outputfilename);
            break;
        case 4:
            /*fcfsnonpreemptive(header,count);
            showresult(&header, count, outputfile);
             sjfnonpreemptive(preemptivem,header,count);
            showresult(&header, count, outputfile);
             sjfpreemptive(preemptivem,header,count);
            showresult(&header, count, outputfile);
             prioritynonpreemptive(preemptivem,header,count);
            showresult(&header, count, outputfile);
             prioritypreemptive(preemptivem,header,count);
            showresult(&header, count, outputfile);
             rr();
            showresult(&header, count, outputfile);
 */
            exit(1);

            break;
        default:
            cout << "please select between the menu choices (1-4)" << endl;
            exit(1);
        }
    }
}
void methodmenu(string *selectedmethod, bool preemptivem, struct process **header, int count)
{
    // methodmenue for the user

    cout << endl
         << "*select one of the scheduling method that you want to test*" << endl;
    cout << "1-None of scheduling method chosen" << endl;
    cout << "2-First Come,First Served Scheduling" << endl;
    cout << "3-Shortest-Job-First Scheduling" << endl;
    cout << "4-Priority Scheduling" << endl;
    cout << "5-Round-Robin Scheduling (You should also obtain time quantum value)" << endl
         << endl;
    int methodmenuanswer;
    cout << "your choice"
         << ">>";
    cin >> methodmenuanswer;
    cout << endl;
    switch (methodmenuanswer)
    {
    case 1:
        *selectedmethod = "1-None of scheduling method chosen";
        break;
    case 2:
        *selectedmethod = "2-First Come,First Served Scheduling";
        fcfs(preemptivem, header, count);
        break;
    case 3:
        *selectedmethod = "3-Shortest-Job-First Scheduling";
        if (preemptivem)
        {
            sjfpreemptive(preemptivem, header, count);
        }
        else
        {
            sjfnonpreemptive(preemptivem, header, count);
        }

        break;
    case 4:
        *selectedmethod = "4-Priority Scheduling";
        if (preemptivem)
        {
            prioritypreemptive(preemptivem, header, count);
        }
        else
        {
            prioritynonpreemptive(preemptivem, header, count);
        }
        break;
    case 5:
        *selectedmethod = "5-Round-Robin Scheduling (You should also obtain time quantum value)";
        break;
    default:
        cout << "please select between the mene choices (1-5)" << endl;
        exit(1);
    }
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
        cout << "**please select the mode you want** " << endl;
        cout << "1-preemptive mode(on)" << endl;
        cout << "2-nonepreemptive mode (off)" << endl;
        cout << "3-cancel" << endl
             << endl;
    }
    int preemptivemodeanswer;
    cout << "your choice"
         << ">>";

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
        cout << "please select from the numbers (1-3)" << endl;
        exit(1);
    }
}
void showresult(struct process **header, int count, string outputfilename)
{
    ofstream outputfile(outputfilename);
    pidsort(header);
    float sum = 0;
    struct process *temp = *header;
    // ofstream f(outputfilename);
    cout << "process waiting times:" << endl;
    while (temp != NULL)
    {
        cout << "p" << temp->processid << " : " << temp->waitingtime << "ms" << endl;
        outputfile << "p" << temp->processid << " : " << temp->waitingtime << "ms" << endl;
        sum += temp->waitingtime;
        temp = temp->next;
    }
    float averagewaitingtime = sum / count;
    cout << "average waiting time:  " << averagewaitingtime << "ms" << endl;
    outputfile << "average waiting time:  " << averagewaitingtime << "ms" << endl;
}

void fcfs(bool preemptivem, struct process **header, int count)
{
    if (preemptivem)
    {
        cout << "first come first served is not supporting preemptive mode";
    }
    else
    {
        fcfsnonpreemptive(header, count);
    }
}
void fcfsnonpreemptive(struct process **header, int count)
{
    arrivaltimesort(header);

    int startingtime = 0;
    struct process *temp = *header;

    int counter = 0;
    while (temp != NULL)
    {   //check if the process is arrived or not 
        if (startingtime < temp->arrivaltime)
        {
            startingtime = temp->arrivaltime;
        }
        temp->waitingtime = startingtime - temp->arrivaltime;
        // cout<<"this is the waiting time of procces "<<counter<<waitingtime[counter-1]<<endl;
        startingtime += temp->bursttime;
        temp = temp->next;
        counter++;
    }
}

void sjfnonpreemptive(bool preemptivem, process **header, int count)
{
    bursttimesort(header);
    int startingtime = 0;
    struct process *temp = *header;

    for (int counter = 0; counter < count; counter++)
    {   //check if the process is arrived or not 
        if (startingtime < temp->arrivaltime)
        {
            startingtime = temp->arrivaltime;
        }

        temp->waitingtime = startingtime - temp->arrivaltime;
        startingtime += temp->bursttime;

        temp = temp->next;
    }
}
void sjfpreemptive(bool preemptivem, process **header, int count)
{
    
}
void prioritynonpreemptive(bool preemptivem, struct process **header, int count)
{
    prioritysort(header);
    struct process *temp = *header;
    int startingtime = 0;
    while (temp != NULL)
    {
        temp->isdone = false;
        temp = temp->next;
    }

    int numberofdoneprocess = 0;
    struct process *selectedprocess;
    while (numberofdoneprocess != count)
    {
        selectedprocess = NULL;
        temp = *header;
        while (temp != NULL)
        {
            if (selectedprocess == NULL)
            {   //check if the process is executed before
                if (temp->isdone == true)
                {
                    temp = temp->next;
                }//check if the process is arrived or not 
                else if (temp->arrivaltime > startingtime)
                {
                    temp = temp->next;
                }
                else
                    selectedprocess = temp;
            }
            else
            {//check if the process is executed before
                if (temp->isdone == true)
                {
                    temp = temp->next;
                }
                //check if the process is arrived or not
                else if (temp->arrivaltime >= startingtime)
                {
                    temp = temp->next;
                }
                //check the priorities 
                else if (temp->priority >= selectedprocess->priority)
                {
                    temp = temp->next;
                }
                else if ((temp->priority == selectedprocess->priority) && (selectedprocess->arrivaltime <= temp->arrivaltime))
                {
                    temp = temp->next;
                }
                else
                    selectedprocess = temp;
            }
        }

        if (selectedprocess == NULL)
        {
            startingtime++;
        }
        else
        {
            selectedprocess->isdone = true;
            startingtime += selectedprocess->bursttime;
            selectedprocess->waitingtime = startingtime - selectedprocess->arrivaltime - selectedprocess->bursttime;
            numberofdoneprocess++;
        }
    }
}
void prioritypreemptive(bool preemptivem, struct process **header, int count)
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

void pidsort(struct process **header)
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

            if (p1->processid > p2->processid)
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

void prioritysort(struct process **header)
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

            if (p1->priority > p2->priority)
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
