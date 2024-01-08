#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
void menu(struct node *);
void fcfs(struct node *);
void sjfnonpre(struct node *);
void sjfpre(node **);
void prioritynonpre(struct node *);
void prioritypre(node **);
void rrpre(struct node *);
void result(struct node *);
struct node *insertBack(struct node *, int, int, int);
struct node *createNode(int, int, int);
void display(struct node *);
void swapNodes(struct node **, struct node *, struct node *);
void arrivalsort(struct node *&);
void apsort(struct node *&);
void burstsort(struct node *&);
void prioritysort(struct node *&);
void pidsort(struct node *&);
struct node
{
    int burst, arrival, priority, waitingtime;
    int pid;
    struct node *next;
    bool executed;
    int timepassed;
};

string inputFilename;
string outputFilename;
string smethod;
int id = 1;
bool preemtive;
int main(int argc, char *argv[])
{
    struct node *process = NULL;

    if (argc < 5)
    {
        cout << "wrong command" << endl
             << "command should be like this: ./program_name -f inputfile -o outputfile" << endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i)
    {

        if (string(argv[i]) == "-f")
        {

            if (i + 1 < argc)
            {

                inputFilename = string(argv[i + 1]);
            }
            else
            {

                cout << "No input file after -f" << endl;
                return 1;
            }
        }
        if (string(argv[i]) == "-o")
        {

            if (i + 1 < argc)
            {
                outputFilename = string(argv[i + 1]);
            }
            else
            {
                cout << "No output file after -o" << endl;
                return 1;
            }
        }
    }
    ifstream inputFile(inputFilename);

    if (!inputFile.is_open())
    {
        cout << "Unable to open input file" << endl;
        return 1;
    }

    stringstream b;
    b << inputFile.rdbuf();
    inputFile.close();
    string fileContents = b.str();

    istringstream iss(fileContents);
    string line;
    while (getline(iss, line, '\n'))
    {
        istringstream iss2(line);
        string num1, num2, num3;
        char delimiter = ':';
        if (getline(iss2, num1, delimiter) && getline(iss2, num2, delimiter) && getline(iss2, num3, delimiter))
        {
            process = insertBack(process, stoi(num1), stoi(num2), stoi(num3));
        }
        else
            return 1;
    }

    // this is just a test
    // clear this part
    cout << fileContents << endl;
    cout << "normal: " << endl;
    display(process);
    cout << endl
         << "burst sorted: " << endl;

    burstsort(process);
    display(process);
    cout << endl
         << " pid sorted: " << endl;

    pidsort(process);
    display(process);
    cout << endl
         << "ap sorted: " << endl;
    apsort(process);
    display(process);
    // end of test

    menu(process);
    return 0;
}
void menu(struct node *process)
{
    char preemtivechoose;
    char methodchoice;
    char menuchoose;
    cout << endl
         << "*************" << endl;
    cout << "CPU Scheduler Simulator" << endl;
    cout << "1) choosing Scheduling Method " << endl
         << "2) choosing Preemptive Mode" << endl
         << "3) Show Result" << endl
         << "4) End Program" << endl;
    cout << "*************" << endl;
    cin >> menuchoose;

    switch (menuchoose)
    {
    case '1':
        cout << "*************" << endl;
        cout << "choose your Scheduling Method (None)" << endl
             << "1-first come first served " << endl
             << "2-shortest job first" << endl
             << "3-priority scheduling" << endl
             << "4-Round-Robin" << endl
             << "5-none" << endl
             << "6-back to menu" << endl;
        cout << "*************" << endl;
        cin >> methodchoice;

        switch (methodchoice)
        {
        case '1':
            if (!preemtive)
                fcfs(process);
            else
            {
                cout << "please turn off preemtive mod off" << endl;
                menu(process);
            }

            break;

        case '2':
            if (!preemtive)
                sjfnonpre(process);
            else
            {
                sjfpre(&process);
            }

            break;
        case '3':
            if (!preemtive)
                prioritynonpre(process);
            else
            {
                prioritypre(&process);
            }

            break;
        case '4':
            //  rrpre(process);

            break;
        case '5':
            cout << "you choose non of the methodes" << endl;

            break;
        case '6':
            menu(process);
            break;

        default:
            cout << "wrong";
            menu(process);
            break;
        }

        break;

    case '2':
        cout << "*************" << endl;
        cout << "1-preemtive mode on " << endl
             << "2-preemtive mode off" << endl;
        cout << "*************" << endl;
        cin >> preemtivechoose;
        if (preemtivechoose == '1')
            preemtive = true;
        else
            preemtive = false;
        menu(process);
        break;

    case '3':
        result(process);
        menu(process);
        break;

    case '4': /*fcfs(process);
        result(process);
        sjfnonpre(process);
        result(process);
        sjfpre(process);
        result(process);
        priority(process);
        result(process);
        rrpre(process);
        result(process);
*/
        exit(1);
        break;

    default:
        cout << "*************" << endl;
        cout << "wrong choice";
        menu(process);
        break;
    }
}
struct node *createNode(int burst, int arrival, int priority)
{
    struct node *temp;
    temp = (struct node *)malloc(sizeof(struct node));
    temp->burst = burst;
    temp->arrival = arrival;
    temp->priority = priority;
    temp->waitingtime = 0;
    temp->next = NULL;
    temp->pid = id;
    temp->executed = false;
    temp->timepassed = 0;
    id++;
    return temp;
}
struct node *insertBack(struct node *header, int burst, int arrival, int priority)
{
    struct node *temp = createNode(burst, arrival, priority);
    struct node *headertemp;
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
int length(struct node *head)
{
    struct node *temp = head;
    int len = 0;
    while (temp != NULL)
    {
        len++;
        temp = temp->next;
    }
    return len;
}

void swapNode(struct node *&a, struct node *&b)
{
    int temp = a->burst;
    a->burst = b->burst;
    b->burst = temp;

    temp = a->arrival;
    a->arrival = b->arrival;
    b->arrival = temp;

    temp = a->priority;
    a->priority = b->priority;
    b->priority = temp;

    temp = a->waitingtime;
    a->waitingtime = b->waitingtime;
    b->waitingtime = temp;

    temp = a->pid;
    a->pid = b->pid;
    b->pid = temp;

    bool temp2 = a->executed;
    a->executed = b->executed;
    b->executed = temp;

    temp = a->timepassed;
    a->timepassed = b->timepassed;
    b->timepassed = temp;
}

void arrivalsort(struct node *&head)
{
    if (head == NULL || head->next == NULL)
    {
        // If the list is empty or has only one element, it's already sorted
        return;
    }

    int swapped;
    struct node *ptr1;
    struct node *lptr = NULL;

    do
    {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr)
        {
            if (ptr1->burst > ptr1->next->burst)
            {
                swapNode(ptr1, (ptr1->next));
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}
// sort functions
void burstsort(struct node *&head)
{
    if (head == NULL || head->next == NULL)
    {
        // If the list is empty or has only one element, it's already sorted
        return;
    }

    int swapped;
    struct node *ptr1;
    struct node *lptr = NULL;

    do
    {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr)
        {
            if (ptr1->burst > ptr1->next->burst)
            {
                swapNode(ptr1, (ptr1->next));
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}
void prioritysort(struct node *&head)
{
    if (head == NULL || head->next == NULL)
    {
        // If the list is empty or has only one element, it's already sorted
        return;
    }

    int swapped;
    struct node *ptr1;
    struct node *lptr = NULL;

    do
    {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr)
        {
            if (ptr1->priority > ptr1->next->priority)
            {
                swapNode(ptr1, (ptr1->next));
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}
void pidsort(struct node *&head)
{
    if (head == NULL || head->next == NULL)
    {
        // If the list is empty or has only one element, it's already sorted
        return;
    }

    int swapped;
    struct node *ptr1;
    struct node *lptr = NULL;

    do
    {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr)
        {
            if (ptr1->pid > ptr1->next->pid)
            {
                swapNode(ptr1, (ptr1->next));
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}
void apsort(struct node *&head)
{
    if (head == NULL || head->next == NULL)
    {
        // If the list is empty or has only one element, it's already sorted
        return;
    }
    int swapped;
    struct node *ptr1;
    struct node *lptr = NULL;

    do
    {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr)
        {

            if (ptr1->arrival > ptr1->next->arrival)
            {
                swapNode(ptr1, (ptr1->next));
                swapped = 1;
            }

            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    lptr = NULL;

    do
    {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr)
        {
            if (ptr1->arrival == ptr1->next->arrival)
            {
                if (ptr1->priority > ptr1->next->priority)
                {
                    swapNode(ptr1, (ptr1->next));
                    swapped = 1;
                }
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// scheduling methodes
void sjfnonpre(struct node *process)
{
    burstsort(process);
    smethod = "shortest job first _ non preemtive";

    if (!process)
    {
        cout << "No processes in the list." << endl;
        return;
    }

    struct node *current = process;
    int timer = 0;

    while (current)
    {
        if (current->arrival > timer)
        {
            timer = current->arrival;
        }

        current->waitingtime = timer - current->arrival;
        timer += current->burst;
        current = current->next;
    }
    menu(process);
}
void sjfpre(node **process)
{
    burstsort(*process);

    smethod = "shortest job first _ preemtive";
    struct node *temp = *process;

    int timer = 0;
    int count = length(*process);
    struct node *min;

    for (int i = 0; i < length(*process); i++)
    {
        temp->timepassed = 0;
        temp = temp->next;
    }

    while (count > 0)
    {
        temp = *process;
        min = NULL;
        while (min == NULL)
        {
            while (temp != NULL)
            {
                if (timer >= temp->arrival)
                {
                    if (temp->timepassed < temp->burst)
                    {
                        if (min == NULL)
                        {
                            min = temp;
                        }
                        else
                        {
                            if ((min->burst - min->timepassed) == (temp->burst - temp->timepassed) && min->arrival > temp->arrival)
                            {
                                min = temp;
                            }
                            else if ((min->burst - min->timepassed) > (temp->burst - temp->timepassed))
                            {
                                min = temp;
                            }
                        }
                    }
                }
                temp = temp->next;
            }
            timer++;
        }
        min->timepassed++;
        if (min->timepassed == min->burst)
        {
            min->waitingtime = timer - min->arrival - min->burst;
            count--;
        }
    }
    menu(*process);
}
void prioritynonpre(struct node *head)
{
    smethod = "priority scheduling _ non preemtive";

    if (head == nullptr || head->next == nullptr)
    {
        return; // No need to schedule
    }

    struct node *x = head;
    for (int i = 0; i < length(head); i++)
    {
        x->executed = false;
        x = x->next;
    }

    apsort(head);
    int timer = 0;
    struct node *temp;
    struct node *select;
    int remain = length(head);

    while (remain > 0)
    {
        temp = head;
        select = NULL;
        while (temp != NULL)
        {
            if (temp->arrival > timer)
            {
                break;
            }
            else if (temp->executed == true)
            {
                temp = temp->next;
                continue;
            }
            else if (select != NULL)
            {
                if (select->arrival > temp->arrival)
                    select = temp;
                else if (select->priority > temp->priority)
                    select = temp;
            }
            else
             select = temp;
            temp = temp->next;
        }
        if (select == NULL)
        {
            timer++;
        }
        else
        {
            remain--;
            select->waitingtime = timer - select->arrival;
            timer += select->burst;
            select->executed = true;
        }
    }

    menu(head);
}
void prioritypre(node **process)
{
    prioritysort(*process);

    smethod = "priority scheduling _ preemtive";
    struct node *temp = *process;

    int timer = 0;
    int count = length(*process);
    struct node *min;

    for (int i = 0; i < length(*process); i++)
    {
        temp->timepassed = 0;
        temp = temp->next;
    }

    while (count > 0)
    {
        temp = *process;
        min = NULL;
        while (min == NULL)
        {
            while (temp != NULL)
            {
                if (timer >= temp->arrival)
                {
                    if (temp->timepassed < temp->burst)
                    {
                        if (min == NULL)
                        {
                            min = temp;
                        }
                        else
                        {
                            if (min->priority == temp->priority && min->arrival > temp->arrival)
                            {
                                min = temp;
                            }
                            else if (min->priority > temp->priority)
                            {
                                min = temp;
                            }
                        }
                    }
                }
                temp = temp->next;
            }
            timer++;
        }
        min->timepassed++;
        if (min->timepassed == min->burst)
        {
            min->waitingtime = timer - min->arrival - min->burst;
            count--;
        }
    }
    menu(*process);
}
void rrpre()
{
    smethod = "round robbin _ preemtive";
}
void fcfs(struct node *head)
{

    smethod = "first come first served";

    arrivalsort(head);
    if (!head)
    {
        cout << "Linked list is empty.\n";
        return;
    }
    struct node *current = head;
    int timer = 0;
    float totalWaitingTime = 0.0;
    while (current)
    {
        int i = 1;
        if (current->arrival > timer)
        {
            timer = current->arrival;
        }
        current->waitingtime = timer - current->arrival;
        totalWaitingTime += current->waitingtime;
        timer += current->burst;
        current = current->next;
    }
    menu(head);
}

// result and displaying functions
void display(struct node *h)
{

    if (h == NULL)
        cout << "empty";

    struct node *t = h;
    while (t != NULL)
    {
        cout << t->burst << "," << t->arrival << "," << t->priority << " , "
             << "id : " << t->pid << endl;
        t = t->next;
    }
    cout << endl;
}
void result(struct node *process)
{
    pidsort(process);
    struct node *temp = process;
    float totalWaitingTime = 0.0;

    ofstream outputfile(outputFilename, std::ios::app);

    if (!outputfile.is_open())
    {
        cerr << "Error opening the file!" << endl;
        return;
    }

    cout << "Scheduling method : " << smethod << endl;
    outputfile << "Scheduling method : " << smethod << endl;
    for (int i = 0; i < length(process); i++)
    {
        cout << "p" << temp->pid << " : "
             << "waiting time = " << temp->waitingtime << endl;

        outputfile << "p" << temp->pid << " : "
                   << "waiting time = " << temp->waitingtime << endl;

        totalWaitingTime += temp->waitingtime;
        temp = temp->next;
    }
    float averageWaitingTime = totalWaitingTime / length(process);
    cout << "Average Waiting Time: " << averageWaitingTime << "\n";
    outputfile << "Average Waiting Time: " << averageWaitingTime << "\n";
}