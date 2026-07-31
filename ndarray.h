#pragma once
#include<time.h>
#include<sstream>
#include<utils.h>
#include<fstream>
#include<vector>
#include<stdint.h>
#include<iostream>
#include<algorithm>
#include<numeric>
#include<random>
#include<chrono>
#include<ndarray_transposed_view.h>
#include<infix_expression.h>
using namespace std;
using namespace mljet;
class NDArrayTransposedView; // forward declration is important
class NDArray
{
private:
vector<double> data;
vector<uint64_t> dimensions;
vector<uint64_t> indexComputationData;
NDArray();
uint64_t get_index(vector<uint64_t> &indices);
public:
NDArray(vector<uint64_t> d);
void set(const vector<uint64_t> &indexes,const double value);
double get(const vector<uint64_t> &indexes) const;
vector<uint64_t> get_dimensions();
void print();
void divide(InfixExpression<NDArray,NDArray> &expression);
void add(InfixExpression<NDArray,NDArray> &expression);
void subtract(InfixExpression<NDArray,NDArray> &expression);
void multiply(InfixExpression<NDArray,NDArray> &expression);
void multiply(InfixExpression<NDArrayTransposedView,NDArray> &expression);
void operator*=(double value);
void operator/=(double value);
void operator+=(double value);
void operator-=(double value);
void operator-=(NDArray &other);
InfixExpression<NDArray,NDArray> operator*(NDArray &right);
InfixExpression<NDArray,NDArray> operator/(NDArray &right);
InfixExpression<NDArray,NDArray> operator+(NDArray &right);
InfixExpression<NDArray,NDArray> operator-(NDArray &right);
NDArray operator/(double value);
NDArray operator*(double value);
NDArray operator+(double value);
NDArray operator-(double value);
NDArray & operator=(InfixExpression<NDArray,NDArray> expression);
NDArray & operator=(InfixExpression<NDArrayTransposedView,NDArray> expression);
NDArrayTransposedView get_transposed_view(bool is_read_only);
static NDArray from_2d_csv(const string &csv_name,uint32_t skip_lines);
static void copy(NDArray &target,const vector<uint64_t> &target_from,
const NDArray &source,const vector<uint64_t> &source_from,
const vector<uint64_t> &source_to);
static void fill(NDArray &ndArray,const vector<uint64_t> &from,
const vector<uint64_t> &to,double value);
static void fill_random(NDArray &ndArray,double min,double max);
friend class NDArrayTransposedView;
};