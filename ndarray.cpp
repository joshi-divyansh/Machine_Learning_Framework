#include<sstream>
#include<utils.h>
#include<utils.cpp>
#include<fstream>
#include<vector>
#include<stdint.h>
#include<iostream>
#include<algorithm>
#include<numeric>
using namespace std;
using namespace mljet;
class NDArray
{
private:
vector<double> data;
vector<uint64_t> dimensions;
vector<uint64_t> indexComputationData;
NDArray()
{
}
public:
NDArray(vector<uint64_t> d)
{
dimensions=d;
uint64_t product=accumulate(dimensions.begin(),dimensions.end(),1,multiplies<uint64_t>());
data.resize(product);
indexComputationData.resize(dimensions.size());
uint64_t acc=1;
int i;
for(i=dimensions.size()-1;i>=0;--i)
{
indexComputationData[i]=acc;
acc=acc*dimensions[i];
}
}
void set(const vector<uint64_t> &indexes,const double value)
{
// validation pending
uint64_t idx;
idx=0;
for(uint64_t i=0;i<indexes.size();++i)
{
idx=idx+(indexes[i]*indexComputationData[i]);
}
data[idx]=value;
cout<<"In set : "<<idx<<endl;
}
double get(const vector<uint64_t> &indexes)
{
// validation pending
uint64_t idx;
idx=0;
for(uint64_t i=0;i<indexes.size();++i)
{
idx=idx+(indexes[i]*indexComputationData[i]);
}
cout<<"In get : "<<idx<<endl;
return data[idx];
}
static NDArray from_2d_csv(const string &csv_name,uint32_t skip_lines)
{
ifstream file(csv_name);
if(!file.is_open()) throw string("csv not found");
string line;
int i=1;
while(i<=skip_lines)
{
if(!getline(file,line))
{
file.close();
throw string("Invalid value of skip_lines");
}
i++;
}
vector<double> v;
uint64_t rows,columns;
vector<double> data;
rows=0;
i=0;
while(getline(file,line))
{
v=split(line,',');
if(rows==0)
{
columns=v.size();
}
data.insert(data.end(),v.begin(),v.end());
rows++;
}
file.close();
NDArray ndarray;
ndarray.data=move(data);
ndarray.dimensions.push_back(rows);
ndarray.dimensions.push_back(columns);
ndarray.indexComputationData.resize(ndarray.dimensions.size());
uint64_t acc=1;
for(i=ndarray.dimensions.size()-1;i>=0;--i)
{
ndarray.indexComputationData[i]=acc;
acc=acc*ndarray.dimensions[i];
}
return ndarray;
}
void print()
{
int i=0;
for(int r=0;r<dimensions[0];++r)
{
for(int c=0;c<dimensions[1];c++)
{
cout<<data[i]<<" ";
i++;
}
cout<<endl;
}
}
vector<uint64_t> get_dimensions()
{
return this->dimensions;
}
static void copy(NDArray &target,const vector<uint64_t> &target_from,const vector<uint64_t> &target_
to,const NDArray &source,const vector<uint64_t> &source_from,const vector<uint64_t> &source_to)
{
}
static void fill(NDArray &ndArray,const vector<uint64_t> &from,const vector<uint64_t> &to,double val
ue)
{
if(ndArrary.dimensions.size()!=from.size() || ndArray.dimensions.size()!=to.size())
{
ostringstream oss;
oss<<"Invalid range ";
for(auto j:from) oss<<"["<<j<<"]";
oss<<" - ";
for(auto j:to) oss<<"["<<j<<"]";
oss<<", size of array is ";
for(auto j:ndArray.dimensions) oss<<"["<<j<<"]";
throw oss.str();
}
for(int i=0;i<ndArray.dimensions.size();i++)
{
if(from[i]>=ndArray.dimensions[i]) || to[i]>=ndArray.dimensions[i])
{
ostringstream oss;
oss<<"Invalid range ";
for(auto j:from) oss<<"["<<j<<"]";
oss<<" - ";
for(auto j:to) oss<<"["<<j<<"]";
oss<<", size of array is ";
for(auto j:ndArray.dimensions) oss<<"["<<j<<"]";
throw oss.str();
}
}
//// ?
}
static void fill_random(NDArray &ndArray,double min,double max)
{
}
};
void test_set()
{
NDArray arr1({2,3,4});
uint64_t i1,i2,i3;
double v=10.0;
for(i1=0;i1<2;i1++)
{
for(i2=0;i2<3;i2++)
{
for(i3=0;i3<4;i3++)
{
arr1.set({i1,i2,i3},v);
v+=5.0;
}
}
}
} // test_set ends
int main() // main written by
{
try
{
NDArray x=NDArray::from_2d_csv("IceCreamSales.csv",1);
x.print(); // print will be removed at later stage
vector<uint64_t> dimensions=arr.get_dimensions();
/* posibility : 1
NDArray y({dimensions[0]}); // output vector
NDArray::copy(y,{0},{dimensions[0]-1},arr,{0,1},{dimensions[0]-1,1}); //a=b
NDArray::copy(x,{0,1},{dimensions[0]-1,1},x,{0,0},{dimensions[0]-1,0});
NDArray::fill(x,{0,0},{dimensions[0]-1,0},1.0);
NDArray model({dimensions[1]});
NDArrray::fill_random(model,0.0,1.0);
uint64_t number_of_cycles;
ofstream err("errors.txt");
number_of_cycles=1;
NDArray predicted_y({dimensions[0]});
double error_value;
while(number_of_cycles<=5000)
{
//NDArray::multiply(x,model,predicted_y);
predicted_y=x*model;
error_value=ml::get_error(predicted_y,y,MEAN_SQUARED_ERROR);
cout<<"Error value : "<<error_value<<endl;
err<<error_value<<"\n";
//over here will will update model
number_of_cycles++;
}
err.close();
NDArray::to_2d_csv(model,"model.csv");
}catch(string &message)
{
cout<<message<<endl;
}
return 0;
}