// According to RFC4180, string with just spaces
// is not to be looked upon as an empty string
// header part is pending
#include<unordered_set>
#include<stdint.h>
#include<utils.h>
#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
using namespace mljet;
int main()
{
string csv_name="abcd.csv";
// uint32_t columns=0; // user has not specified columns
uint32_t columns=3; // user has specified 3 columns
// input file stream
int skip_lines=3;
ifstream fs(csv_name);
if(!fs.is_open())
{
cout<<"Unable to open file "<<csv_name<<endl;
return 0;
}
string line;
vector<pair<uint64_t,uint32_t>> lines_with_invalid_column_count;
vector<uint64_t> empty_lines;
uint64_t line_number=0;
vector<string> strings;
uint32_t comma_count;
unordered_set<uint32_t> column_counts;
uint32_t column_count;
while(skip_lines>0)
{
if(!getline(fs,line)) break;
--skip_lines;
}
while(getline(fs,line))
{
++line_number;
cout<<line<<endl; // remove, once the code is finalized
if(line.length()==0) empty_lines.push_back(line_number);
else
{
strings=split(line,',',comma_count);
column_count=comma_count+1;
column_counts.insert(column_count);
if(columns>0) // user has specified number of columns
{
if((column_count)!=columns)
{
lines_with_invalid_column_count.push_back(pair<uint64_t,uint32_t>(line_number,column_count));
}
}
}
}
fs.close();
if(empty_lines.size()>0)
{
cout<<"Empty lines"<<endl;
for(uint64_t e:empty_lines) cout<<e<<endl;
}
if(columns==0 && column_counts.size()>1)
{
cout<<"Number of columns vary"<<endl;
for(uint32_t n:column_counts) cout<<n<<endl;
}
if(lines_with_invalid_column_count.size()>0)
{
cout<<"Lines with invalid number of columns"<<endl;
cout<<"Required number of columns on each line : "<<columns<<endl;
for(pair<uint64_t,uint32_t> p:lines_with_invalid_column_count)
{
cout<<"Line number ("<<p.first<<"), columns("<<p.second<<")"<<endl;
}
}
return 0;
}