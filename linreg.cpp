#include<ndarray.h>
#include<ndarray_transposed_view.h>
#include<csv_info.h>
#include<ml_utils.h>
int main(int cc,char *vv[])
{
if(cc!=3)
{
cout<<"Usage [linreg number_of_training_cycles learning_rate"<<endl;
exit(1);
}
uint64_t number_of_training_cycles=atoi(vv[1]);
double learning_rate=atof(vv[2]);
try
{
csv_info csv("ics1.csv",1);
NDArray x({csv.get_rows(),csv.get_columns()});
NDArray y({csv.get_rows()});
ml_utils::from_csv(csv,x,y);
auto x_dimensions=x.get_dimensions();
auto x_rows=x_dimensions[0];
auto x_columns=x_dimensions[1];
NDArrayTransposedView x_transposed=x.get_transposed_view(true);
NDArray model({x_columns});
NDArray tmp({x_columns});
NDArray::fill_random(model,0.0,1.0);
//model.print();
auto y_dimensions=y.get_dimensions();
auto y_rows=y_dimensions[0];
ofstream err("errors.txt");
NDArray predicted_y({y_rows});
NDArray difference({y_rows});
NDArray difference_squared_sum({1});
NDArray error_value({1});
uint64_t cycle=1;
while(cycle<=number_of_training_cycles)
{
predicted_y=x*model;
//predicted_y.print();
difference=predicted_y-y;
//difference.print();
// we have programmed * functionality in such
// a way, that of product is a 1x1 scalar value
// then left operand will be 1 row and n columns
// and right operand will be n rows and 1 column,
// hence diffrence_square is logically
difference_squared_sum=difference*difference;
error_value=difference_squared_sum/(((double)2)*((double)y_rows));
cout<<"Error value : ";
error_value.print();
err<<error_value.get({0})<<"\n";
// create infra to update model
tmp=x_transposed*difference;
tmp/=y_rows;
tmp*=learning_rate;
model-=tmp;
cycle++;
}
err.close();
//NDArray::to_binary(model,"model.binary");
/* something like this will go in to_2d_csv
FILE *f=fopen("model.csv","wb");
write how many dimensions
write info about dimensions of model NDArray
fwrite(model.collection.data(),sizeof(double)*model.size(),f);
fclose(f);
*/
}catch(const invalid_argument &argument)
{
cout<<argument.what()<<endl;
}
catch(string &message)
{
cout<<message<<endl;
}
return 0;
}