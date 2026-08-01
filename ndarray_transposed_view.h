#pragma once
#include<infix_expression.h>
#include<ndarray.h>
#include<utility>
class NDArray; 
class NDArrayTransposedView
{
private:
NDArray *ndarray;
bool is_read_only;
public:
NDArrayTransposedView(NDArray *ndarray,bool is_read_only);
void set(vector<uint64_t> &indexes,const double value);
double get(vector<uint64_t> indexes);
vector<uint64_t> get_dimensions();
InfixExpression<NDArrayTransposedView *,NDArray *> operator*(NDArray &right);
friend class NDArray;
};