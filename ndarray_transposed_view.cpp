#include<ndarray_transposed_view.h>
NDArrayTransposedView::NDArrayTransposedView(NDArray *ndarray,bool is_ready_only):
ndarray(ndarray),is_read_only(is_read_only)
{
}
void NDArrayTransposedView::set(vector<uint64_t> &indexes,const double value)
{
if(this->is_read_only) throw invalid_argument("NDArrayTransposedView is read only");
if(this->ndarray==nullptr) throw invalid_argument("NDArrayTransposedView is invalidated");
auto sz=indexes.size();
uint64_t r,c;
if(sz>1)
{
r=indexes[sz-2];
c=indexes[sz-1];
indexes[sz-2]=c;
indexes[sz-1]=r;
}
this->ndarray->set(indexes,value);
if(sz>1)
{
indexes[sz-2]=r;
indexes[sz-1]=c;
}
}
double NDArrayTransposedView::get(vector<uint64_t> indexes)
{
if(this->ndarray==nullptr) throw invalid_argument("NDArrayTransposedView is invalidated");
auto sz=indexes.size();
uint64_t r,c;
if(sz>1)
{
r=indexes[sz-2];
c=indexes[sz-1];
indexes[sz-2]=c;
indexes[sz-1]=r;
}
return this->ndarray->get(indexes);
}
vector<uint64_t> NDArrayTransposedView::get_dimensions()
{
vector<uint64_t> dimensions;
if(this->ndarray==nullptr) return dimensions;
dimensions=this->ndarray->get_dimensions();
size_t sz=dimensions.size();
if(sz==1) return dimensions;
uint64_t r;
uint64_t c;
r=dimensions[sz-2];
c=dimensions[sz-1];
dimensions[sz-2]=c;
dimensions[sz-1]=r;
return dimensions;
}
InfixExpression<NDArrayTransposedView *,NDArray *> NDArrayTransposedView::operator*(NDArray &right)
{
return InfixExpression<NDArrayTransposedView *,NDArray *>(this,'*',&right);
}