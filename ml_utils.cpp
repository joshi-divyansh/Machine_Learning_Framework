#include<ml_utils.h>
double ml_utils::get_mse(NDArray &predicted_y,NDArray &y)
{
uint64_t r;
double sum,value;
uint64_t rows=predicted_y.get_dimensions()[0];
for(sum=0.0,r=0;r<rows;++r)
{
value=predicted_y.get({r})-y.get({r});
sum+=(value*value);
}
return sum/(double)rows;
}
double ml_utils::get_error(NDArray &predicted_y,NDArray &y,err_metric algorithm_name)
{
if(algorithm_name==err_metric::MEAN_SQUARED_ERROR)
{
return get_mse(predicted_y,y);
}
return 0;
}
void ml_utils::from_csv(csv_info &csv,NDArray &x,NDArray &y)
{
ifstream file(csv.get_name());
string line;
for(int i=1;i<=csv.get_header_lines_count();++i)
{
getline(file,line);
}
uint64_t r=0;
while(getline(file,line))
{
vector<double> v=split(line,',');
y.set({r},v[v.size()-1]);
x.set({r,0},1.0);
for(uint64_t u=1;u<v.size();++u) x.set({r,u},v[u-1]);
r++;
}
}