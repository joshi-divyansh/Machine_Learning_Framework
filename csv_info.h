#pragma once
#include<iostream>
#include<string>
#include<sstream>
#include<cstdint>
#include<fstream>
using namespace std;
class csv_info
{
uint64_t rows;
uint64_t columns;
uint32_t skip_lines;
string name;
public:
csv_info(string name,uint32_t skip_lines);
uint64_t get_rows();
uint64_t get_columns();
uint32_t get_header_lines_count();
string get_name();
};