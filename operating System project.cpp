include <iostream>
using namespace sd;
void firstmenu();
void methodmenu();
void fcfs();
void sjf();
void priority();
void RR();
int main ()
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
switch(firstmenueanswer)
 { case 1: methodmenu();
  case 2: preemtivemode();
  case 3: result();
  case 4: exit();
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
  case 5: RR();break;
  default: cout<<"please select between the mene choices (1-5)";
 }

}