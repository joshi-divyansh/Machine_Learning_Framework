#include<fstream>
#include<iostream>
#include<string>
#include<stdlib.h>
#include<time.h>
using namespace std;
int main(int argc,char *argv[])
{
if(argc!=4)
{
cout<<"Usage [random_csv_generator csv_name rows columns]"<<endl;
return 0;
}
string csv_name=string(argv[1]);
int generate_rows=atoi(argv[2]);
int generate_columns=atoi(argv[3]);
ofstream file(csv_name,ios::out); // open a new filee
// existing will be replaced
if(!file.is_open())
{
cout<<"Unable to open file "<<csv_name<<endl;
return 0;
}
srand(time(NULL));
int num;
for(int rows=0;rows<generate_rows;++rows)
{
for(int columns=0;columns<generate_columns;++columns)
{
num=rand();
file<<num;
if(columns<generate_columns-1) file<<",";
}
file<<"\n";
}
file.close();
return 0;
}