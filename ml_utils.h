#pragma once
#include<utils.h>
#include<ndarray.h>
#include<csv_info.h>
#include<fstream>
class ml_utils
{
public:
enum class err_metric{MEAN_SQUARED_ERROR};
static void from_csv(csv_info &,NDArray &,NDArray &);
static double get_mse(NDArray &predicted_y,NDArray &y);
static double get_error(NDArray &predicted_y,NDArray &y,err_metric algorithm_name);
};