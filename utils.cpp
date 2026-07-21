#include<stdint.h>
#include<vector>
#include<string>
#include<sstream>
#include<stdexcept>
#include<algorithm>
using namespace std;
namespace mljet
{
double min_max_scaler(double value,double min,double max)
{
return (value-min)/(max-min);
}
double to_double(const std::string &str)
{
if(str.length()==0) throw std::invalid_argument(string("Cannot convert empty string to double value"));
double value;
size_t index;
try
{
value=stod(str,&index);
if(str.c_str()[index]!='\0') throw invalid_argument(string("Bad double value : ")+str);
}catch(invalid_argument &i)
{
throw invalid_argument(string("Bad double value : ")+str);
}
return value;
}
void trim(std::string &str)
{
//str.erase(remove(str.begin(),str.end(),' '),str.end());
int i=0;
while(i<str.length() && (str[i]==' ' || str[i]=='\n' || str[i]=='\r'))
{
i++;
}
if(i==str.length())
{
str="";
return;
}
int start=i;
i=str.length()-1;
while(i>=0 && (str[i]==' ' || str[i]=='\n' || str[i]=='\r'))
{
i--;
}
int end=i;
if(start>0 || end<str.length()-1)
{
str=str.substr(start,end-start+1);
}
}
std::vector<std::string> split(const std::string &str,const char delimiter,uint32_t &delimiter_count)
{
vector<string> strings;
if(str.length()==0) return strings;
stringstream strm(str);
string part;
delimiter_count=0;
while(getline(strm,part,delimiter))
{
delimiter_count++;
strings.push_back(part);
}
if(str.c_str()[str.length()-1]!=delimiter) delimiter_count--;
return strings;
}
// the following does not perform any validations
std::vector<double> split(const std::string &str,const char delimiter)
{
vector<double> doubles;
if(str.length()==0) return doubles;
stringstream strm(str);
string part;
while(getline(strm,part,delimiter))
{
doubles.push_back(mljet::to_double(part));
}
return doubles;
}
}