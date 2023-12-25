#include <iostream>
#include <unistd.h>
#include fstream;
using namespace std;
void firstmenu();
void methodmenu();
void preemtivemode();
void result();
void fcfs();
void sjf();
void priority();
void rr();
int getopt(int argc, char * const argv[],const char *optstring);
struct node * createprocess(string);
void display processes(struct process *);
struct node * addprocess(struct process *, string);
struct process{
 string data ;
 struct process *next;
};
int main (int argc, char *argv[])
{

firstmenu();

return 0;

}
void firstmenu()
{
//menue for the user 
cout<<"CPU Scheduler Simulator"<<endl;
cout<<"1-Scheduling Method"<<endl;
cout<<"2-Preemptive Mode"<<endl;
cout<<"3-Show Result"<<endl;
cout<<"4-End Program";
int firstmenuanswer;
cin>>firstmenuanswer;
switch(firstmenuanswer)
 { case 1: methodmenu();break;
   case 2: preemtivemode();break;
   case 3: result();break;
   case 4: exit(1);break;
   default: cout<<"please select between the mene choices (1-4)";

 }

}
void methodmenu()
{
//methodmenue for the user 
cout<<"1-None of scheduling method chosen"<<endl;
cout<<"2-First Come,First Served Scheduling"<<endl;
cout<<"3-Shortest-Job-First Scheduling"<<endl;
cout<<"4-Priority Scheduling"<<endl;
cout<<"5-Round-Robin Scheduling (You should also obtain time quantum value)"<<endl;
int methodmenuanswer;
cin>>methodmenuanswer;
switch(methodmenuanswer)
 {case 1: firstmenu();break;
  case 2: fcfs();break;
  case 3: sjf();break;
  case 4: priority();break;
  case 5: rr();break;
  default: cout<<"please select between the mene choices (1-5)";
 }

}
void result()
{
    firstmenu();    
}
void preemtivemode()
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
struct process * createprocess(string data)
{
     struct process * temp;
     temp = (struct process *) malloc(sizeof(process));
     temp->item = data;
     temp->next = NULL;
     return temp;
}
struct process* insertprocess(struct process *header, string data)
{
       struct process * temp = createprocess(data);
       struct process * headertemp;
       if (header == NULL)
       {
          header = temp;
          return header;
       }
       headertemp=header;
       while(headertemp->next != NULL)
            headertemp=headertemp->next;
       headertemp->next = temp;
       return header;
}