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
uint64_t get_index(vector<uint64_t> &indices)
{
uint64_t idx;
idx=0;
for(uint64_t i=0;i<indices.size();++i)
{
idx=idx+(indices[i]*indexComputationData[i]);
}
return idx;
}
void __just_do_it(vector<uint64_t> &from,vector<uint64_t> &to,int level,vector<uint64_t> &tmp,vector
<uint64_t> &computedIdx)
{
if(level==from.size()-1)
{
int idx;
for(int i=tmp[level];i<=to[level];++i)
{
tmp[level]=i;
idx=get_index(tmp);
computedIdx.push_back(idx);
}
}
else
{
int k;
for(int i=from[level];i<=to[level];++i)
{
tmp[level]=i;
__just_do_it(from,to,level+1,tmp,computedIdx);
for(k=level;k<tmp.size();++k) tmp[k]=from[k];
}
}
}
void just_do_it(vector<uint64_t> from,vector<uint64_t> to,vector<uint64_t> &computedIdx)
{
vector<uint64_t> tmp=from;
__just_do_it(from,to,0,tmp,computedIdx); // 0 is level(1)
}
void generate_indices_vector_code_for_copying()
{
vector<uint64_t> from={3};
vector<uint64_t> to={8};
vector<uint64_t> tmp=from;
uint64_t last_index=from.size()-1;
int64_t i;
while(true)
{
while(tmp[last_index]<=to[last_index])
{
for(auto x:tmp) cout<<x<<" ";
cout<<endl;
tmp[last_index]++;
}
for(i=last_index-1;i>=0 && tmp[i]==to[i];--i);
if(i==-1) break;
tmp[i]++;
for(i++;i<tmp.size();i++) tmp[i]=from[i];
}
} // function ends
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
//cout<<"In set : "<<idx<<endl;
}
double get(const vector<uint64_t> &indexes) const
{
// validation pending
uint64_t idx;
idx=0;
for(uint64_t i=0;i<indexes.size();++i)
{
idx=idx+(indexes[i]*indexComputationData[i]);
}
//cout<<"In get : "<<idx<<endl;
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
vector<uint64_t> from(this->dimensions.size());
for(int i=0;i<from.size();i++) from[i]=0;
vector<uint64_t> to=this->dimensions;
for(int i=0;i<to.size();i++) to[i]=to[i]-1;
double value;
vector<uint64_t> tmp=from;
uint64_t last_index=from.size()-1;
int64_t i;
while(true)
{
while(tmp[last_index]<=to[last_index])
{
value=this->get(tmp);
ostringstream oss;
for(auto g:tmp) oss<<"["<<g<<"]";
oss<<"="<<value;
cout<<oss.str()<<endl;
tmp[last_index]++;
}
for(i=last_index-1;i>=0 && tmp[i]==to[i];--i);
if(i==-1) break;
tmp[i]++;
for(i++;i<tmp.size();i++) tmp[i]=from[i];
}
}
vector<uint64_t> get_dimensions()
{
return this->dimensions;
}
static void copy(NDArray &target,const vector<uint64_t> &target_from,const NDArray &source,const vec
tor<uint64_t> &source_from,const vector<uint64_t> &source_to)
{
/*
if(source.dimensions.size()!=source_from.size() || source.dimensions.size()!=source_to.size())
{
// source range in invalid, size is [5][5][5]
ostringstream oss;
oss<<"copy:source range in invalid";
for(auto x:source_from) oss<<"["<<x<<"]";
oss<<" - ";
for(auto x:source_to) oss<<"["<<x<<"]";
oss<<", size is ";
for(auto x:source.dimensions) oss<<"["<<x<<"]";
throw string(oss.str());
}
uint8_t sourceRangeError=0;
for(int i=0;i<source.dimensions.size();i++)
{
if(source_from[i]>=source.dimensions[i])
{
sourceRangeError=1;
break;
}
if(source_to[i]>=source.dimensions[i])
{
sourceRangeError=1;
break;
}
if(source_to[i]<source_from[i])
{
sourceRangeError=1;
break;
}
}
if(sourceRangeError==1)
{
ostringstream oss;
oss<<"copy:source range in invalid";
for(auto x:source_from) oss<<"["<<x<<"]";
oss<<" - ";
for(auto x:source_to) oss<<"["<<x<<"]";
oss<<", size is ";
for(auto x:source.dimensions) oss<<"["<<x<<"]";
throw string(oss.str());
}
if(target.dimensions.size()!=target_from.size())
{
ostringstream oss;
oss<<"copy:target indices are invalid";
for(auto x:target_from) oss<<"["<<x<<"]";
oss<<", size is ";
for(auto x:target.dimensions) oss<<"["<<x<<"]";
throw string(oss.str());
}
vector<uint64_t> target_to;
for(int i=0;i<target_from.size();++i)
{
target_to.push_back(target_from[i]+(source_to[i]-source_from[i]));
}
uint8_t targetRangeError=0;
for(int i=0;i<target.dimensions.size();i++)
{
if(target_from[i]>=target.dimensions[i])
{
targetRangeError=1;
break;
}
if(target_to[i]>=target.dimensions[i])
{
targetRangeError=1;
break;
}
}
if(targetRangeError==1)
{
ostringstream oss;
oss<<"copy:target range in invalid";
for(auto x:target_from) oss<<"["<<x<<"]";
oss<<" - ";
for(auto x:target_to) oss<<"["<<x<<"]";
oss<<", size is ";
for(auto x:target.dimensions) oss<<"["<<x<<"]";
throw string(oss.str());
}
// we feel that all validations are done
// following has to be changed
// we are thinking, that number of dimensions
// in source and target are same
// dimensions are not same
// this has to change in next session
*/
vector<uint64_t> source_tmp=source_from;
vector<uint64_t> target_tmp=target_from;
uint64_t last_index=source_to.size()-1;
int64_t i;
double value;
int offset;
offset=0;
if(target.dimensions.size()!=source.dimensions.size())
{
offset=target.dimensions.size()-source.dimensions.size();
}
while(true)
{
while(source_tmp[last_index]<=source_to[last_index])
{
value=source.get(source_tmp);
target.set(target_tmp,value);
source_tmp[last_index]++;
target_tmp[(uint64_t)(last_index+offset)]++;
}
for(i=last_index-1;i>=0 && source_tmp[i]==source_to[i];--i);
if(i==-1) break;
source_tmp[i]++;
target_tmp[(uint64_t)(i+offset)]++;
for(i++;i<source_tmp.size();i++)
{
source_tmp[i]=source_from[i];
target_tmp[(uint64_t)(i+offset)]=target_from[(uint64_t)(i+offset)];
}
}
}
static void fill(NDArray &ndArray,const vector<uint64_t> &from,const vector<uint64_t> &to,double val
ue)
{
if(ndArray.dimensions.size()!=from.size() || ndArray.dimensions.size()!=to.size())
{
ostringstream oss;
oss<<"fill:Invalid range ";
for(auto j:from) oss<<"["<<j<<"]";
oss<<" - ";
for(auto j:to) oss<<"["<<j<<"]";
oss<<", size of array is ";
for(auto j:ndArray.dimensions) oss<<"["<<j<<"]";
throw oss.str();
}
for(int i=0;i<ndArray.dimensions.size();i++)
{
if(from[i]>=ndArray.dimensions[i] || to[i]>=ndArray.dimensions[i])
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
vector<uint64_t> tmp=from;
uint64_t last_index=from.size()-1;
int64_t i;
while(true)
{
while(tmp[last_index]<=to[last_index])
{
ndArray.set(tmp,value);
tmp[last_index]++;
}
for(i=last_index-1;i>=0 && tmp[i]==to[i];--i);
if(i==-1) break;
tmp[i]++;
for(i++;i<tmp.size();i++) tmp[i]=from[i];
}
}
static void fill_random(NDArray &ndArray,double min,double max)
{
vector<uint64_t> from=ndArray.dimensions;
vector<uint64_t> to=ndArray.dimensions;
for(int i=0;i<from.size();i++)
{
from[i]=0;
to[i]=ndArray.dimensions[i]-1;
}
vector<uint64_t> tmp=from;
uint64_t last_index=from.size()-1;
int64_t i;
while(true)
{
while(tmp[last_index]<=to[last_index])
{
//ndArray.set(tmp,value); // value part pending
tmp[last_index]++;
}
for(i=last_index-1;i>=0 && tmp[i]==to[i];--i);
if(i==-1) break;
tmp[i]++;
for(i++;i<tmp.size();i++) tmp[i]=from[i];
}
}
void generate_series()
{
// this will work for dimensions>2
// creating from with all filled with 0 as default value
vector<uint64_t> from(this->dimensions.size()-2,0);
vector<uint64_t> tmp=from;
vector<uint64_t> to(this->dimensions.size()-2);
for(int i=0;i<to.size();++i) to[i]=this->dimensions[i]-1;
uint64_t last_index=from.size()-1;
int64_t i;
while(true)
{
while(tmp[last_index]<=to[last_index])
{
//ndArray.set(tmp,value);
for(auto j:tmp) cout<<"["<<j<<"]";
cout<<endl;
tmp[last_index]++;
}
for(i=last_index-1;i>=0 && tmp[i]==to[i];--i);
if(i==-1) break;
tmp[i]++;
for(i++;i<tmp.size();i++) tmp[i]=from[i];
}
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
/*
int main() // main written by
{
try
{
NDArray x=NDArray::from_2d_csv("IceCreamSales.csv",1);
x.print(); // print will be removed at later stage
vector<uint64_t> dimensions=arr.get_dimensions();
NDArray y({dimensions[0]}); // output vector
NDArray::copy(y,{0},arr,{0,1},{dimensions[0]-1,1});
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
*/
int copy_equal_dimensions_ndarray_test()
{
// testing done
/*NDArray arr({10,10});
NDArray::fill(arr,{2,5},{3,7},3.39);
arr.print();*/
//testing done
/*
NDArray a({10});
double value=21.0;
for(uint64_t i=0;i<10;i++,value++) a.set({i},value);
NDArray b({20});
NDArray::fill(b,{0},{19},33.34);
NDArray::copy(b,{2},a,{4},{7});
b.print();
*/
return 0;
}
int main()
{
/*
NDArray x({2,4,3});
x.generate_series(); // generate series is temp
NDArray x({3,2,4,3});
x.generate_series(); // generate series is temp
NDArray x({2,3,2,4,3});
x.generate_series(); // generate series is temp
*/
NDArray x({4,3});
x.generate_series(); // generate series is temp
return 0;
}
