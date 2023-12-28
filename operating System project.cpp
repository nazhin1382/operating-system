#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;
void firstmenu(struct process *);
void methodmenu(string *, bool, struct process *);
void preemptivemode(bool *, string, struct process *);
void showresult(struct process *);
void fcfs(bool, struct process *);
void sjf();
void priority();
void rr();

struct process *createprocess(int, int, int);
void displayprocess(struct process *);
struct process *insertprocess(struct process *, int, int, int);
struct process *swap(struct process *, struct process *);
void bubbleSort(struct process **);
void fcfsnonepreemptive(struct process *);

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

     firstmenu(header);
    cout << "this is the normal list" << endl;
    displayprocess(header);
    bubbleSort(&header);
    cout << "this is the sorted linked list " << endl;
    displayprocess(header);

    return 0;
}

void firstmenu(struct process *header)
{
    string selectedmethod = "1-none of the scheduling method chosen";
    bool preemptivem = false;
    while (true)
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
            methodmenu(&selectedmethod, preemptivem, header);
            break;
        case 2:
            preemptivemode(&preemptivem, selectedmethod, header);
            break;
        case 3:
            showresult(header);
            break;
        case 4:
            exit(1);
            break;
        default:
            cout << "please select between the menu choices (1-4)";
        }
    }
}

void methodmenu(string *selectedmethod, bool preemptivem, struct process *header)
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
        *selectedmethod = "1-None of scheduling method chosen";
        break;
    case 2:
        *selectedmethod = "2-First Come,First Served Scheduling";
        fcfs(preemptivem, header);
        break;
    case 3:
        *selectedmethod = "3-Shortest-Job-First Scheduling";
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
}

void preemptivemode(bool *preemptivem, string selectedmethod, struct process *header)
{
    if (selectedmethod == "1-none of the scheduling method chosen")
    {
        cout << "please first select a scheduling method ";
        firstmenu(header);
    }
    else
    {
        cout << endl;
        cout << "***please select the mode you want*** " << endl;
        cout << "1-preemptive mode(on)" << endl;
        cout << "2-nonepreemptive mode (off)" << endl;
        cout << "3-cancel" << endl;
    }
    int preemptivemodeanswer;
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
void showresult(struct process *header)
{
    firstmenu(header);
}
void fcfs(bool preemptivem, struct process *header)
{
    if (preemptivem)
    {
        cout << "first come first served is not supporting preemptive mode";
    }
    else
    {
        fcfsnonepreemptive(header);
    }
}
void sjf()
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

void bubbleSort(struct process **header)
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
void fcfsnonepreemptive(struct process *header)
{
    bubbleSort(&header);
    struct process *temp = header;

    int count = 0;
    int currenttime =0;
    while (temp->next != NULL)
    {
        count++;
        temp = temp->next;
    }
    temp=header;
    int waitingtime[count];
        int counter=0;
    while(temp!=NULL)
    {   
        waitingtime[counter++]=currenttime-temp->arrivaltime;
        cout<<"this is the waiting time of procces "<<counter<<waitingtime[counter-1]<<endl;
        currenttime+=temp->bursttime;
        temp=temp->next;

    }
}