#include<iostream>
#include<string.h>
#include<vector>
#include<fstream>
#include<utils.h>
#include<stdint.h>
#include<stdlib.h>
using namespace mljet;
using namespace std;
int main(int argc,char *argv[])
{
if(argc<5)
{
cout<<"Usage [min_max_scaler input_csv_name skip_lines output_csv_name columns....]"<<endl;
return 0;
}
string filename=argv[1];
ifstream file(filename,ios::binary);
if(!file.is_open())
{
cout<<"Unable to open file "<<filename<<endl;
return 0;
}
int skip_lines=atoi(argv[2]);
string line;
while(skip_lines>0)
{
if(!getline(file,line)) break;
trim(line);
--skip_lines;
}
auto pos=file.tellg();
vector<double> min;
vector<double> max;
vector<double> tmp;
string part;
double d;
if(!getline(file,line))
{
cout<<"Nothing to scale"<<endl;
return 0;
}
trim(line);
min=split(line,',');
max=min;
int i;
while(getline(file,line))
{
trim(line);
tmp=split(line,',');
for(i=0;i<min.size();++i)
{
d=tmp[i];
if(d<min[i]) min[i]=d;
if(d>max[i]) max[i]=d;
}
} // loop on file ends
// the internal file pointer is at the end
// the want the internal pointer to go to first byte
// in c rewind(f)
file.clear();
file.seekg(pos,std::ios::beg);
string output_filename=argv[3];
// if exists, it will be replaced
ofstream output_file(output_filename,ios::out | ios::binary);
if(!output_file.is_open())
{
cout<<"Unable to create "<<output_filename<<endl;
file.close();
return 0;
}
vector<uint32_t> indexes;
for(int i=4;i<argc;++i)
{
indexes.push_back(atoi(argv[i]));
}
while(getline(file,line))
{
trim(line);
tmp=split(line,',');
for(auto idx:indexes)
{
if(idx>=tmp.size()) continue;
d=min_max_scaler(tmp[idx],min[idx],max[idx]);
tmp[idx]=d;
}
for(i=0;i<tmp.size()-1;++i) output_file<<tmp[i]<<",";
output_file<<tmp[i]<<"\n";
}
output_file.close();
file.close();
char *str=new char[strlen(argv[3])+1+8]; 
strcpy(str,output_filename.c_str());
strcat(str,".min-max");
ofstream min_max_file(str,iostream::out | iostream::binary);
int x;
for(x=0;x<min.size()-1;++x)
{
min_max_file<<min[x]<<",";
}
min_max_file<<min[x]<<"\n";
for(x=0;x<max.size()-1;++x)
{
min_max_file<<max[x]<<",";
}
min_max_file<<max[x]<<"\n";
min_max_file.close();
delete [] str;
return 0;
}