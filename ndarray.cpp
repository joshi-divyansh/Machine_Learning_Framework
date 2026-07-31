#include<ndarray.h>
using namespace std;
using namespace mljet;
NDArray::NDArray()
{
}
uint64_t NDArray::get_index(vector<uint64_t> &indices)
{
uint64_t idx;
idx=0;
for(uint64_t i=0;i<indices.size();++i)
{
idx=idx+(indices[i]*indexComputationData[i]);
}
return idx;
}
NDArray::NDArray(vector<uint64_t> d)
{
dimensions=d;
uint64_t product=accumulate(dimensions.begin(),dimensions.end(),1,multiplies<uint64_t>());
this->data.resize(product);
indexComputationData.resize(dimensions.size());
uint64_t acc=1;
int i;
for(i=dimensions.size()-1;i>=0;--i)
{
indexComputationData[i]=acc;
acc=acc*dimensions[i];
}
}
void NDArray::set(const vector<uint64_t> &indexes,const double value)
{
// validation pending
uint64_t idx;
idx=0;
for(uint64_t i=0;i<indexes.size();++i)
{
idx=idx+(indexes[i]*indexComputationData[i]);
}
this->data[idx]=value;
}
double NDArray::get(const vector<uint64_t> &indexes) const
{
// validation pending
uint64_t idx;
idx=0;
for(uint64_t i=0;i<indexes.size();++i)
{
idx=idx+(indexes[i]*indexComputationData[i]);
}
return this->data[idx];
}
NDArray NDArray::from_2d_csv(const string &csv_name,uint32_t skip_lines)
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
void NDArray::print()
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
vector<uint64_t> NDArray::get_dimensions()
{
return this->dimensions;
}
void NDArray::copy(NDArray &target,const vector<uint64_t> &target_from,const NDArray &source,const vector<uint64_t> &source_from,const vector<uint64_t> &source_to)
{
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
void NDArray::fill(NDArray &ndArray,const vector<uint64_t> &from,const vector<uint64_t> &to,double value)
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
void NDArray::fill_random(NDArray &ndArray,double min,double max)
{
uniform_real_distribution unif(min,max);
mt19937 engine(chrono::system_clock::now().time_since_epoch().count());
double value;
srand(time(NULL));
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
value=unif(engine);
ndArray.set(tmp,value);
tmp[last_index]++;
}
for(i=last_index-1;i>=0 && tmp[i]==to[i];--i);
if(i==-1) break;
tmp[i]++;
for(i++;i<tmp.size();i++) tmp[i]=from[i];
}
}
void NDArray::divide(InfixExpression<NDArray,NDArray> &expression)
{
uint64_t index;
index=0;
for(auto &t:this->data)
{
t=expression.left->data[index]/expression.right->data[index];
index++;
}
}
void NDArray::add(InfixExpression<NDArray,NDArray> &expression)
{
uint64_t index;
index=0;
for(auto &t:this->data)
{
t=expression.left->data[index]+expression.right->data[index];
index++;
}
}
void NDArray::subtract(InfixExpression<NDArray,NDArray> &expression)
{
uint64_t index;
index=0;
for(auto &t:this->data)
{
t=expression.left->data[index]-expression.right->data[index];
index++;
}
}
void NDArray::multiply(InfixExpression<NDArray,NDArray> &expression)
{
auto left=expression.left;
auto left_array=left->data.data();
auto right=expression.right;
auto right_array=right->data.data();
size_t i;
size_t left_matrix_rows;
size_t left_matrix_columns;
size_t left_matrix_length;
size_t right_matrix_rows;
size_t right_matrix_columns;
size_t right_matrix_length;
size_t product_matrix_rows;
size_t product_matrix_columns;
int number_of_left_matrices;
int number_of_right_matrices;
if(left->dimensions.size()==1 && right->dimensions.size()>1)
{
i=right->dimensions.size();
right_matrix_rows=right->dimensions[i-2];
right_matrix_columns=right->dimensions[i-1];
right_matrix_length=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
i=left->dimensions.size();
left_matrix_rows=1;
left_matrix_columns=left->dimensions[0];
left_matrix_length=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
}
else
if(left->dimensions.size()>1 && right->dimensions.size()==1)
{
i=left->dimensions.size();
left_matrix_rows=left->dimensions[i-2];
left_matrix_columns=left->dimensions[i-1];
left_matrix_length=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
i=right->dimensions.size();
right_matrix_rows=right->dimensions[0];
right_matrix_columns=1;
right_matrix_length=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
}
else
if(left->dimensions.size()==1 && right->dimensions.size()==1)
{
if(this->dimensions.size()==1 && this->dimensions[0]==1)
{
i=left->dimensions.size();
left_matrix_rows=1;
left_matrix_columns=left->dimensions[0];
left_matrix_length=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
i=right->dimensions.size();
right_matrix_rows=right->dimensions[0];
right_matrix_columns=1;
right_matrix_length=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
} // product is a scalar value
else
if(this->dimensions.size()>=2)
{
left_matrix_rows=left->dimensions[0];
left_matrix_columns=1;
left_matrix_length=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
right_matrix_rows=1;
right_matrix_columns=right->dimensions[0];
right_matrix_length=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
}//product_matrix is a 2d
else
if(left->dimensions[0]==1)
{
left_matrix_rows=1;
left_matrix_columns=1;
left_matrix_length=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
right_matrix_rows=1;
right_matrix_columns=right->dimensions[0];
right_matrix_length=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
}
else
if(right->dimensions[0]==1)
{
left_matrix_rows=left->dimensions[0];
left_matrix_columns=1;
left_matrix_length=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
right_matrix_rows=1;
right_matrix_columns=1;
right_matrix_length=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
}
}
else
if(left->dimensions.size()>1 && right->dimensions.size()>1)
{
i=left->dimensions.size();
left_matrix_rows=left->dimensions[i-2];
left_matrix_columns=left->dimensions[i-1];
left_matrix_length=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
if(i>2)
{
// assume {5,10,4,3}
for(int j=0;j<i-2;++j) number_of_left_matrices*=left->dimensions[j];
}
i=right->dimensions.size();
right_matrix_rows=right->dimensions[i-2];
right_matrix_columns=right->dimensions[i-1];
right_matrix_length=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
if(i>2)
{
// assume {5,10,4,3}
for(int j=0;j<i-2;++j) number_of_right_matrices*=right->dimensions[j];
}
}
auto product_array=this->data.data();
auto product_matrix_length=left_matrix_rows*right_matrix_columns;
// prefill product matrix with 0
for(auto &k:this->data) k=0.0;
uint64_t r,c,c1;
auto left_matrix=left_array;
auto right_matrix=right_array;
auto product_matrix=product_array;
for(i=0;i<number_of_left_matrices;++i)
{
// logic to multiply 1 matrix with anoter starts here
#pragma omp parallel for private(c,c1)
for(r=0;r<left_matrix_rows;++r)
{
for(c=0;c<left_matrix_columns;++c)
{
for(c1=0;c1<right_matrix_columns;++c1)
{
//product_matrix[r][c1]+=(left_matrix[r][c]*right_matrix[c][c1]);
product_matrix[r*right_matrix_columns+c1]+=(left_matrix[r*left_matrix_columns+c]*right_matrix[c*right_matrix_columns+c1]);
}
}
}
// implict barrier will be created (wait for threads to join)
// logic to multiply 1 matrix with anoter ends here
left_matrix+=left_matrix_length;
right_matrix+=right_matrix_length;
product_matrix+=product_matrix_length;
} // loop on number_of_left_matrices ends here
} // multiply ends (NDArray,NDArray)
void NDArray::multiply(InfixExpression<NDArrayTransposedView,NDArray> &expression)
{
auto left=expression.left;
auto left_array=left->ndarray->data.data();
auto right=expression.right;
auto right_array=right->data.data();
auto left_dimensions=left->get_dimensions();
size_t i;
size_t left_matrix_rows;
size_t left_matrix_columns;
size_t left_matrix_length;
size_t right_matrix_rows;
size_t right_matrix_columns;
size_t right_matrix_length;
size_t product_matrix_rows;
size_t product_matrix_columns;
int number_of_left_matrices;
int number_of_right_matrices;
if(left_dimensions.size()==1 && right->dimensions.size()>1)
{
i=right->dimensions.size();
right_matrix_rows=right->dimensions[i-2];
right_matrix_columns=right->dimensions[i-1];
right_matrix_length=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
i=left_dimensions.size();
left_matrix_rows=1;
left_matrix_columns=left_dimensions[0];
left_matrix_length=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
}
else
if(left_dimensions.size()>1 && right->dimensions.size()==1)
{
i=left_dimensions.size();
left_matrix_rows=left_dimensions[i-2];
left_matrix_columns=left_dimensions[i-1];
left_matrix_length=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
i=right->dimensions.size();
right_matrix_rows=right->dimensions[0];
right_matrix_columns=1;
right_matrix_length=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
}
else
if(left_dimensions.size()==1 && right->dimensions.size()==1)
{
if(this->dimensions.size()==1 && this->dimensions[0]==1)
{
i=left_dimensions.size();
left_matrix_rows=1;
left_matrix_columns=left_dimensions[0];
left_matrix_length=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
i=right->dimensions.size();
right_matrix_rows=right->dimensions[0];
right_matrix_columns=1;
right_matrix_length=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
} // product is a scalar value
else
if(this->dimensions.size()>=2)
{
left_matrix_rows=left_dimensions[0];
left_matrix_columns=1;
left_matrix_length=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
right_matrix_rows=1;
right_matrix_columns=right->dimensions[0];
right_matrix_length=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
}//product_matrix is a 2d
else
if(left_dimensions[0]==1)
{
left_matrix_rows=1;
left_matrix_columns=1;
left_matrix_length=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
right_matrix_rows=1;
right_matrix_columns=right->dimensions[0];
right_matrix_length=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
}
else
if(right->dimensions[0]==1)
{
left_matrix_rows=left_dimensions[0];
left_matrix_columns=1;
left_matrix_length=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
right_matrix_rows=1;
right_matrix_columns=1;
right_matrix_length=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
}
}
else
if(left_dimensions.size()>1 && right->dimensions.size()>1)
{
i=left_dimensions.size();
left_matrix_rows=left_dimensions[i-2];
left_matrix_columns=left_dimensions[i-1];
left_matrix_length=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
if(i>2)
{
// assume {5,10,4,3}
for(int j=0;j<i-2;++j) number_of_left_matrices*=left_dimensions[j];
}
i=right->dimensions.size();
right_matrix_rows=right->dimensions[i-2];
right_matrix_columns=right->dimensions[i-1];
right_matrix_length=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
if(i>2)
{
// assume {5,10,4,3}
for(int j=0;j<i-2;++j) number_of_right_matrices*=right->dimensions[j];
}
}
auto product_array=this->data.data();
auto product_matrix_length=left_matrix_rows*right_matrix_columns;
// prefill product matrix with 0
for(auto &k:this->data) k=0.0;
uint64_t r,c,c1;
auto left_matrix=left_array;
auto right_matrix=right_array;
auto product_matrix=product_array;
for(i=0;i<number_of_left_matrices;++i)
{
// logic to multiply 1 matrix with anoter starts here
#pragma omp parallel for private(c,c1)
for(r=0;r<left_matrix_rows;++r)
{
for(c=0;c<left_matrix_columns;++c)
{
for(c1=0;c1<right_matrix_columns;++c1)
{
//product_matrix[r][c1]+=(left_matrix[r][c]*right_matrix[c][c1]);
// in formula instead left_matrix_columns
// we have used left_matrix_rows, because in actual
// ndarray whose transposed view is left_matrix
// number of columns of original is number of rows of transposed view
product_matrix[r*right_matrix_columns+c1]+=(left_matrix[c*left_matrix_rows+r]*right_matrix[c*right_matrix_columns+c1]);
}
}
}
// implict barrier will be created (wait for threads to join)
// logic to multiply 1 matrix with anoter ends here
left_matrix+=left_matrix_length;
right_matrix+=right_matrix_length;
product_matrix+=product_matrix_length;
} // loop on number_of_left_matrices ends here
} // multiply ends
void NDArray::operator*=(double value)
{
for(auto &v:this->data) v*=value;
}
void NDArray::operator/=(double value)
{
for(auto &v:this->data) v/=value;
}
void NDArray::operator+=(double value)
{
for(auto &v:this->data) v+=value;
}
void NDArray::operator-=(double value)
{
for(auto &v:this->data) v-=value;
}
void NDArray::operator-=(NDArray &other)
{
uint64_t i=0;
for(auto &v:this->data)
{
v-=other.data[i];
++i;
}
}
InfixExpression<NDArray,NDArray> NDArray::operator*(NDArray &right)
{
return InfixExpression<NDArray,NDArray>(this,'*',&right);
}
InfixExpression<NDArray,NDArray> NDArray::operator/(NDArray &right)
{
return InfixExpression<NDArray,NDArray>(this,'/',&right);
}
InfixExpression<NDArray,NDArray> NDArray::operator+(NDArray &right)
{
return InfixExpression<NDArray,NDArray>(this,'+',&right);
}
InfixExpression<NDArray,NDArray> NDArray::operator-(NDArray &right)
{
return InfixExpression<NDArray,NDArray>(this,'-',&right);
}
NDArray & NDArray::operator=(InfixExpression<NDArray,NDArray> expression)
{
if(expression.oper=='*') this->multiply(expression);
else if(expression.oper=='/') this->divide(expression);
else if(expression.oper=='+') this->add(expression);
else if(expression.oper=='-') this->subtract(expression);
return *this;
}
NDArray & NDArray::operator=(InfixExpression<NDArrayTransposedView,NDArray> expression)
{
if(expression.oper=='*') this->multiply(expression);
return *this;
}
NDArrayTransposedView NDArray::get_transposed_view(bool is_read_only)
{
return NDArrayTransposedView(this,is_read_only);
}
NDArray NDArray::operator/(double value)
{
NDArray tmp=*this;
for(auto &t:tmp.data) t/=value;
return tmp;
}
NDArray NDArray::operator*(double value)
{
NDArray tmp=*this;
for(auto &t:tmp.data) t*=value;
return tmp;
}
NDArray NDArray::operator+(double value)
{
NDArray tmp=*this;
for(auto &t:tmp.data) t+=value;
return tmp;
}
NDArray NDArray::operator-(double value)
{
NDArray tmp=*this;
for(auto &t:tmp.data) t-=value;
return tmp;
}
