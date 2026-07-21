#include<utils.h>
#include<iostream>
using namespace std;
using namespace mljet;
int main(int argc,char *argv[])
{
if(argc==0)
{
cout<<"Usage [test_trim string]"<<endl;
return 0;
}
string str(argv[1]);
trim(str);
cout<<"Trimmed ("<<str<<")"<<endl;
return 0;
}