#include<iostream>
#include<cstdint>
#include<fstream>
#include<string>
#include<filesystem>
#include<vector>
#include<utils.h>
using namespace std;
using namespace mljet;
int main(int argc,char *argv[])
{
if(argc!=5)
{
cout<<"Usage"<<endl;
cout<<"one_hot_encoder source_csv target_csv skip_lines \"column-names:title_1,title_2,...\""<<endl;
cout<<"or"<<endl;
cout<<"one_hot_encoder source_csv target_csv skip_lines \"column-numbers:2,3,...\""<<endl;
cout<<"First column should be denoted by 1"<<endl;
return 1;
}
string columns=argv[4];
/*
1) get position of : or check if it starts with column-names or column-numbers
if not, story ends
2) get the rest of the string and split it
*/
size_t pos=columns.find(':');
if(pos==string::npos)
{
cout<<"Last argument is invalid"<<endl;
cout<<"Usage"<<endl;
cout<<"one_hot_encoder source_csv target_csv skip_lines \"column-names:title_1,title_2,...\""<<endl;
cout<<"or"<<endl;
cout<<"one_hot_encoder source_csv target_csv skip_lines \"column-numbers:2,3,...\""<<endl;
cout<<"First column should be denoted by 1"<<endl;
return 1;
}
string prefix=columns.substr(0,pos);
if((prefix!="column-names" && prefix!="column-numbers") || columns.length()==pos+1)
{
cout<<"Last argument is invalid"<<endl;
cout<<"Usage"<<endl;
cout<<"one_hot_encoder source_csv target_csv skip_lines \"column-names:title_1,title_2,...\""<<endl;
cout<<"or"<<endl;
cout<<"one_hot_encoder source_csv target_csv skip_lines \"column-numbers:2,3,...\""<<endl;
cout<<"First column should be denoted by 1"<<endl;
return 1;
}
if(skip_lines==0)
{
if(prefix=="column-names")
{
cout<<"Last argument is invalid"<<endl;
cout<<"Usage"<<endl;
cout<<"one_hot_encoder source_csv target_csv skip_lines \"column-names:title_1,title_2,...\""<<endl;
cout<<"or"<<endl;
cout<<"one_hot_encoder source_csv target_csv skip_lines \"column-numbers:2,3,...\""<<endl;
cout<<"First column should be denoted by 1"<<endl;
cout<<"If skip_lines is zero then last argument should start with column-numbers"<<endl;
return 1;
}
}
string columns_to_encode=columns.substr(pos+1);
uint32_t comma_count;
vector<string> column_header_parts=split(columns_to_encode,',',comma_count);
for(auto &str:column_header_parts)
{
trim(str);
if(str.length()==0)
{
cout<<"Last argument is invalid"<<endl;
cout<<"Usage"<<endl;
cout<<"one_hot_encoder source_csv target_csv skip_lines \"column-names:title_1,title_2,...\""<<endl;
cout<<"or"<<endl;
cout<<"one_hot_encoder source_csv target_csv skip_lines \"column-numbers:2,3,...\""<<endl;
cout<<"First column should be denoted by 1"<<endl;
return 1;
}
}
string source_csv=argv[1];
if(!std::filesystem::exists(source_csv))
{
cout<<source_csv<<" not found"<<endl;
return 1;
}
ifstream source_file(source_csv);
if(!source_file.is_open())
{
cout<<"Unable to open "<<source_csv<<endl;
return 1;
}
int skip_lines=std::stoi(argv[3]);
string line;
int x;
x=1;
while(getline(source_file,line) && x<=skip_lines)
{
++x;
}
if(x<=skip_lines)
{
source_file.close();
cout<<"Not enough data in "<<source_csv<<" to process"<<endl;
return 1;
}
cout<<"all is well"<<endl;
string target_csv=argv[2];
ofstream target_file(target_csv);
if(!target_file.is_open())
{
cout<<"Unable to create "<<target_csv<<endl;
source_file.close();
return 1;
}
// yet to be completed
return 0;
}