#include<csv_info.h>
csv_info::csv_info(string name,uint32_t skip_lines)
{
this->name=name;
this->skip_lines=skip_lines;
ifstream file(this->name);
if(file.fail())
{
string msg=string("File : ")+this->name+string(" missing.");
throw invalid_argument(msg);
}
string line;
int x=1;
while(x<=this->skip_lines && getline(file,line)) x++;
if(x<=this->skip_lines)
{
file.close();
string msg=string("File : ")+this->name+string(" doesn't have header of "+to_string(this->skip_lines
)+string(" lines"));
throw invalid_argument(msg);
}
if(!getline(file,line))
{
file.close();
string msg=string("File : ")+this->name+string(" doesn't have comma separated values");
throw invalid_argument(msg);
}
stringstream ss(line);
string column;
this->columns=0;
while(getline(ss,column,',')) this->columns++;
this->rows=1;
while(getline(file,line))
{
this->rows++;
}
file.close();
}
uint64_t csv_info::get_rows()
{
return this->rows;
}
uint64_t csv_info::get_columns()
{
return this->columns;
}
uint32_t csv_info::get_header_lines_count()
{
return this->skip_lines;
}
string csv_info::get_name()
{
return this->name;
}