#include<iostream>
#include<utils.h>
#include<fstream>
#include<random>
#include<chrono>
#include<numeric>
#include<unordered_set>
using namespace std;
using namespace mljet;
int main(int argc,char *argv[])
{
if(argc<4)
{
cout<<"Usage : [create_training_and_test_dataset input.csv skip_lines testing_data_percentage]"<<endl;
return 0;
}
int testing_data_percentage;
testing_data_percentage=atoi(argv[3]);
if(testing_data_percentage<=0 || testing_data_percentage>=100)
{
cout<<"Testing data percentage should be between (1-99)"<<endl;
return 0;
}
string input_file=argv[1];
ifstream file(input_file,ios::binary);
if(!file.is_open())
{
cout<<"Unable to open file : "<<input_file<<endl;
return 0;
}
string training_file_name="training_"+input_file;
string testing_file_name="test_"+input_file;
ofstream training_file(training_file_name);
if(!training_file.is_open())
{
file.close();
cout<<"Unable to create : "<<training_file_name<<endl;
return 0;
}
ofstream testing_file(testing_file_name);
if(!testing_file.is_open())
{
file.close();
training_file.close();
cout<<"Unable to create : "<<testing_file_name<<endl;
return 0;
}
int skip_lines=atoi(argv[2]);
string line;
for(int x=1;x<=skip_lines;x++)
{
if(!getline(file,line)) break;
}
long pos=file.tellg();
uint64_t number_of_lines;
number_of_lines=0;
while(getline(file,line)) number_of_lines++;
file.clear();
file.seekg(pos,std::ios::beg);
// calculate number of lines for testing
uint64_t number_of_lines_for_testing=(uint64_t)((testing_data_percentage*number_of_lines)/100);
auto seed=std::chrono::system_clock::now().time_since_epoch().count();
mt19937 engine(seed);
uniform_int_distribution<uint64_t> d(1,number_of_lines);
unordered_set<uint64_t> s;
uint64_t random_number;
while(s.size()!=number_of_lines_for_testing)
{
random_number=d(engine);
s.insert(random_number);
}
file.clear();
file.seekg(0,ios::beg);
for(int i=1;i<=skip_lines;i++)
{
getline(file,line);
trim(line);
testing_file<<line<<"\n"; // line has \n at end
training_file<<line<<"\n"; // line has \n at end
}
uint64_t ln=1;
while(getline(file,line))
{
trim(line);
if(s.find(ln)!=s.end()) // is part of set
{
testing_file<<line<<"\n"; // line has \n at end
}
else // is not part of set
{
training_file<<line<<"\n"; // line has \n at end
}
ln++;
}
testing_file.close();
training_file.close();
file.close();
cout<<training_file_name<<" and "<<testing_file_name<<" created"<<endl;
return 0;
}