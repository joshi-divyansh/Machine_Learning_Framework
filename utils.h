#pragma once
#include<stdint.h>
#include<vector>
#include<string>
#include<sstream>
#include<stdexcept>
#include<algorithm>
using namespace std;
namespace mljet
{
double min_max_scaler(double value,double min,double max);
double to_double(const std::string &str);
void trim(std::string &str);
std::vector<std::string> split(const std::string &str,const char delimiter,uint32_t &delimiter_count);
std::vector<double> split(const std::string &str,const char delimiter);
}