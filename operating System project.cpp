#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;
// menue functions
void firstmenu(struct process *, int, ofstream *);
string methodmenu(string, bool, struct process **, int);
void preemptivemode(bool *, string, struct process **, int);
void showresult(struct process **, int, ofstream *, string);

// first come first served functions
void fcfs(struct process **, int);
void fcfsnonpreemptive(struct process **, int);
// shortest job first functions
void sjfnonpreemptive(struct process **, int);
void sjfpreemptive(struct process **, int);
// priority scheduling functions
void prioritynonpreemptive(struct process **, int);
void prioritypreemptive(struct process **, int);
// roundrobbin functions
void rrpreemptive(struct process **, int);
void rrnonpreemptive(struct process **, int);
// linked list functions
struct process *createprocess(int, int, int);
struct process *insertprocess(struct process *, int, int, int);
void displayprocess(struct process *);
// queue functions
struct process *insertreadyqueue(struct process *, int, int, int, int, int, int, bool);
struct process *deletereadyqueue(struct process *);
// sort functions
void pidsort(struct process **);
void arrivaltimesort(struct process **);
void bursttimesort(struct process **);
void prioritysort(struct process **);
struct process *swap(struct process *, struct process *);
// linked list data structure
struct process
{
    int processid;
    int bursttime;
    int arrivaltime;
    int priority;
    int waitingtime;
    int remainingtime;
    bool isdone;
    struct process *next;
};
// main function
int main(int argc, char *argv[])
{
    char opt;
    bool fselected, oselected;
    string inputfilename, outputfilename;
    ifstream inputfile;
    // getting arguments
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
    // read the file and insert it into the linked list
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
    // count the number of the processes inside the linked list
    while (temp != NULL)
    {
        count++;
        temp->processid = count;
        temp = temp->next;
    }

    // just for test and should be cleared
    cout << "this is the normal list" << endl;
    displayprocess(header);
    arrivaltimesort(&header);
    cout << "this is the sorted linked list " << endl;
    displayprocess(header);
    /*  cout << "this is the normal list" << endl;
  displayprocess(header);
  bursttimesort(&header);
  cout << "this is the sorted linked list " << endl;
  displayprocess(header);*/

    ofstream outputfile(outputfilename);
    firstmenu(header, count, &outputfile);
    outputfile.close();

    return 0;
}
// menu functions
void firstmenu(struct process *header, int count, ofstream *outputfile)
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
            selectedmethod = methodmenu(selectedmethod, preemptivem, &header, count);
            break;
        case 2:
            preemptivemode(&preemptivem, selectedmethod, &header, count);
            break;
        case 3:
            if (selectedmethod == "1-None of scheduling method chosen")
            {
                cout << "please first select a scheduling method " << endl;
                break;
            }
            showresult(&header, count, outputfile, selectedmethod);
            break;
        case 4:
            selectedmethod = "First Come,First Served Scheduling-NonPreemptive";
            fcfsnonpreemptive(&header, count);
            showresult(&header, count, outputfile, selectedmethod);
            selectedmethod = "Shortest-Job-First Scheduling-Preemptive ";

            sjfnonpreemptive(&header, count);
            showresult(&header, count, outputfile, selectedmethod);
            selectedmethod = "Shortest-Job-First Scheduling-NonPreemptive ";

            sjfpreemptive(&header, count);
            showresult(&header, count, outputfile, selectedmethod);
            selectedmethod = "Shortest-Job-First Scheduling-NonPreemptive ";

            prioritynonpreemptive(&header, count);
            showresult(&header, count, outputfile, selectedmethod);
            selectedmethod = "Priority Scheduling-NonPreemptive";

            prioritypreemptive(&header, count);
            showresult(&header, count, outputfile, selectedmethod);
            /*rr();
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
string methodmenu(string selectedmethod, bool preemptivem, struct process **header, int count)
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
        selectedmethod = "None of scheduling method chosen";
        break;
    case 2:
        selectedmethod = "First Come,First Served Scheduling-NonPreemptive";

        if (preemptivem)
        {
            fcfs(header, count);
        }
        else
        {
            fcfsnonpreemptive(header, count);
        }

        break;
    case 3:
        if (preemptivem)
        {
            selectedmethod = "Shortest-Job-First Scheduling-Preemptive ";

            sjfpreemptive(header, count);
        }
        else
        {
            selectedmethod = "Shortest-Job-First Scheduling-NonPreemptive ";

            sjfnonpreemptive(header, count);
        }

        break;
    case 4:
        if (preemptivem)
        {
            selectedmethod = "Priority Scheduling-Preemptive";

            prioritypreemptive(header, count);
        }
        else
        {
            selectedmethod = "Priority Scheduling-NonPreemptive";
            prioritynonpreemptive(header, count);
        }
        break;
    case 5:
        selectedmethod = "Round-Robin Scheduling-Preemptive mode";
        if (preemptivem)
        {
            rrpreemptive(header, count);
        }
        else
        {
            rrnonpreemptive(header, count);
        }
        break;
    default:
        cout << "please select between the mene choices (1-5)" << endl;
        exit(1);
    }
    return selectedmethod;
}
void preemptivemode(bool *preemptivem, string selectedmethod, struct process **header, int count)
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

    if (selectedmethod == "2-First Come,First Served Scheduling")
    {
        fcfs(header, count);
    }
    else if (selectedmethod == "3-Shortest-Job-First Scheduling")
    {
        if (*preemptivem)
        {
            sjfpreemptive(header, count);
        }
        else
        {
            sjfnonpreemptive(header, count);
        }
    }
    else if (selectedmethod == "4-Priority Scheduling")
    {
        if (*preemptivem)
        {
            prioritypreemptive(header, count);
        }
        else
        {
            prioritynonpreemptive(header, count);
        }
    }
    else if (selectedmethod == "5-Round-Robin Scheduling (You should also obtain time quantum value)")
    {
        if (*preemptivem)
        {
            rrpreemptive(header, count);
        }
        else
        {
            rrnonpreemptive(header, count);
        }
    }
}
void showresult(struct process **header, int count, ofstream *outputfile, string selectedmethod)
{
    pidsort(header);
    float sum = 0;
    struct process *temp = *header;
    // ofstream f(outputfilename);
    cout << "scheduling method:" << selectedmethod << endl
         << "process waiting times:" << endl;
    *outputfile << "scheduling method:" << selectedmethod << endl
                << "process waiting times:" << endl;

    while (temp != NULL)
    {
        cout << "p" << temp->processid << " : " << temp->waitingtime << "ms" << endl;
        *outputfile << "p" << temp->processid << " : " << temp->waitingtime << "ms" << endl;
        sum += temp->waitingtime;
        temp = temp->next;
    }
    float averagewaitingtime = sum / count;
    cout << "average waiting time:  " << averagewaitingtime << "ms" << endl<<endl;
    *outputfile << "average waiting time:  " << averagewaitingtime << "ms" << endl<<endl;
}
// first come first served functions
void fcfs(struct process **header, int count)
{
    pidsort(header);
    arrivaltimesort(header);
    int startingtime = 0;
    struct process *temp = *header;

    while (temp != NULL)
    {
        // check if the process is arrived or not
        if (startingtime < temp->arrivaltime)
        {
            startingtime = temp->arrivaltime;
        }
        temp->waitingtime = startingtime - temp->arrivaltime;
        // cout<<"this is the waiting time of procces "<<counter<<waitingtime[counter-1]<<endl;
        startingtime += temp->bursttime;
        temp = temp->next;
    }
}
void fcfsnonpreemptive(struct process **header, int count)
{
    pidsort(header);
    arrivaltimesort(header);
    int startingtime = 0;
    struct process *temp = *header;

    while (temp != NULL)
    {
        // check if the process is arrived or not
        if (startingtime < temp->arrivaltime)
        {
            startingtime = temp->arrivaltime;
        }
        temp->waitingtime = startingtime - temp->arrivaltime;
        // cout<<"this is the waiting time of procces "<<counter<<waitingtime[counter-1]<<endl;
        startingtime += temp->bursttime;
        temp = temp->next;
    }
}
// shortest job first functions
void sjfnonpreemptive(process **header, int count)
{
    pidsort(header);
    bursttimesort(header);
    struct process *temp = *header;
    struct process *selectedprocess;
    int startingtime = 0;
    int numberofdoneprocess = 0;

    for (int i = 0; i < count; i++)
    {
        temp->isdone = false;
        temp = temp->next;
    }

    while (numberofdoneprocess < count)
    {
        temp = *header;
        selectedprocess = NULL;

        while (temp != NULL)
        {
            if (temp->isdone != true && startingtime >= temp->arrivaltime)
            {
                if (selectedprocess == NULL || (selectedprocess->arrivaltime > temp->arrivaltime && selectedprocess->bursttime > temp->bursttime))
                {
                    selectedprocess = temp;
                }
            }
            temp = temp->next;
        }

        if (selectedprocess != NULL)
        {
            startingtime += selectedprocess->bursttime;
            selectedprocess->isdone = true;
            selectedprocess->waitingtime = startingtime - selectedprocess->arrivaltime - selectedprocess->bursttime;
            numberofdoneprocess++;
        }
        else if (selectedprocess == NULL)
        {
            // if no process arrived increase the starting time till reach an arrived process
            startingtime++;
        }
    }
}
void sjfpreemptive(process **header, int count)
{
    pidsort(header);
    bursttimesort(header);

    struct process *temp = *header;

    for (int i = 0; i < count; i++)
    {
        temp->remainingtime = temp->bursttime;
        temp = temp->next;
    }

    int startingtime = 0;

    int numberofdoneprocess = 0;
    struct process *selectedprocess;

    while (numberofdoneprocess < count)
    {
        temp = *header;
        selectedprocess = NULL;
        while (selectedprocess == NULL)
        {
            while (temp != NULL)
            {
                if (startingtime >= temp->arrivaltime)
                {
                    if (temp->remainingtime > 0)
                    {
                        if (selectedprocess == NULL)
                        {
                            selectedprocess = temp;
                        }
                        else
                        {
                            if (selectedprocess->remainingtime == temp->remainingtime && selectedprocess->arrivaltime > temp->arrivaltime)
                            {
                                selectedprocess = temp;
                            }
                            else if (selectedprocess->remainingtime > temp->remainingtime)
                            {
                                selectedprocess = temp;
                            }
                        }
                    }
                }
                temp = temp->next;
            }
            startingtime++;
        }
        selectedprocess->remainingtime--;
        if (selectedprocess->remainingtime == 0)
        {
            selectedprocess->waitingtime = startingtime - selectedprocess->arrivaltime - selectedprocess->bursttime;
            numberofdoneprocess++;
        }
    }
}
// priority scheduling functions
void prioritynonpreemptive(struct process **header, int count)
{
    pidsort(header);
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
            { // check if the process is executed before
                if (temp->isdone == true)
                {
                    temp = temp->next;
                } // check if the process is arrived or not
                else if (temp->arrivaltime > startingtime)
                {
                    temp = temp->next;
                }
                else
                    selectedprocess = temp;
            }
            else
            { // check if the process is executed before
                if (temp->isdone == true)
                {
                    temp = temp->next;
                }
                // check if the process is arrived or not
                else if (temp->arrivaltime >= startingtime)
                {
                    temp = temp->next;
                }
                // check the priorities
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
void prioritypreemptive(struct process **header, int count)
{
    pidsort(header);
    prioritysort(header);

    struct process *temp = *header;

    for (int i = 0; i < count; i++)
    {
        temp->remainingtime = temp->bursttime;
        temp = temp->next;
    }

    int startingtime = 0;

    int numberofdoneprocess = 0;
    struct process *selectedprocess;

    while (numberofdoneprocess < count)
    {
        temp = *header;
        selectedprocess = NULL;
        while (selectedprocess == NULL)
        {
            while (temp != NULL)
            {
                if (startingtime >= temp->arrivaltime)
                {
                    if (temp->remainingtime > 0)
                    {
                        if (selectedprocess == NULL)
                        {
                            selectedprocess = temp;
                        }
                        else
                        {
                            if (selectedprocess->priority == temp->priority && selectedprocess->arrivaltime > temp->arrivaltime)
                            {
                                selectedprocess = temp;
                            }
                            if (selectedprocess->priority > temp->priority)
                            {
                                selectedprocess = temp;
                            }
                        }
                    }
                }
                temp = temp->next;
            }
            startingtime++;
        }
        selectedprocess->remainingtime--;
        if (selectedprocess->remainingtime == 0)
        {
            selectedprocess->waitingtime = startingtime - selectedprocess->arrivaltime - selectedprocess->bursttime;
            numberofdoneprocess++;
        }
    }
}
// round robin functions
void rrpreemptive(struct process **header, int count)
{
    pidsort(header);
    int timequantum;
    cout << "please enter the quantum number" << endl
         << "your answer>>";
    cin >> timequantum;
    int numberofdoneprocess = 0;
    int startingtime = 0;
    struct process *temp = *header;
    struct process *selectedprocess;

    for (int i = 0; i < count; i++)
    {
        temp->isdone = false;
        temp = temp->next;
    }

    temp = *header;
    for (int i = 0; i < count; i++)
    {
        temp->remainingtime = temp->bursttime;
        temp = temp->next;
    }

    arrivaltimesort(header);
    struct process *readyqueue = NULL;
    while (numberofdoneprocess != count)
    {
        temp = *header;
        selectedprocess = NULL;
        while (temp != NULL)
        {
            struct process *temp2 = *header;
            while (temp2 != NULL)
            {
                struct process *readyqueuetemp = readyqueue;
                bool isInQueue = false;
                while (readyqueuetemp != NULL)
                {
                    if (readyqueuetemp->processid == temp2->processid)
                    {
                        isInQueue = true;
                        break;
                    }
                    readyqueuetemp = readyqueuetemp->next;
                }

                if (!isInQueue && temp2->arrivaltime <= startingtime)
                {
                    readyqueue = insertreadyqueue(readyqueue, temp2->processid, temp2->bursttime, temp2->arrivaltime, temp2->priority, temp2->remainingtime, temp2->waitingtime, temp2->isdone);
                }
                if (temp2->arrivaltime > startingtime)
                {
                    break;
                }

                temp2 = temp2->next;
            }

            if (readyqueue != NULL)
            {
                if (readyqueue->isdone == true)
                {
                    if (readyqueue->remainingtime > 0)
                    {
                        readyqueue->isdone = false;
                        readyqueue = insertreadyqueue(readyqueue, temp->processid, temp->bursttime, temp->arrivaltime, temp->priority, temp->remainingtime, temp->waitingtime, temp->isdone);
                        readyqueue = deletereadyqueue(readyqueue);
                    }
                }

                if (readyqueue->remainingtime > timequantum)
                {
                    readyqueue->remainingtime -= timequantum;
                    startingtime += timequantum;

                    temp2 = *header;
                    while (temp2 != NULL)
                    {
                        if (temp2->processid == readyqueue->processid)
                        {
                            temp2->remainingtime = readyqueue->remainingtime;
                            break;
                        }
                        temp2 = temp2->next;
                    }
                    readyqueue->isdone = true;
                }
                else if (readyqueue->remainingtime <= timequantum)
                {
                    startingtime += readyqueue->remainingtime;
                    readyqueue->remainingtime = 0;
                    numberofdoneprocess++;

                    temp2 = *header;
                    while (temp2 != NULL)
                    {
                        if (temp2->processid == readyqueue->processid)
                        {
                            temp2->remainingtime = 0;
                            temp2->waitingtime = startingtime - temp2->bursttime - temp2->arrivaltime;
                            break;
                        }
                        temp2 = temp2->next;
                    }
                    readyqueue = deletereadyqueue(readyqueue);
                }
            }
            else
            {
                startingtime++;
            }
            temp = temp->next;
        }
    }
}
void rrnonpreemptive(struct process **header, int count)
{
}
// linkedlist functions
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
// sort functions according to arrival time,bursttime and priority and also process id
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
// queue functions
struct process *insertreadyqueue(struct process *header, int pid, int bursttime, int arrivaltime, int priority, int remainingtime, int waitingtime, bool isdone)
{
    struct process *temp = createprocess(bursttime, arrivaltime, priority);
    temp->processid = pid;
    temp->remainingtime = remainingtime;
    temp->isdone = isdone;
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
struct process *deletereadyqueue(struct process *header)
{
    struct process *temp;
    if (header == NULL)
        return header;
    temp = header;
    header = header->next;
    free(temp);
    return header;
}
