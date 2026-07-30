#pragma once
template<typename LEFT,typename RIGHT>
class InfixExpression
{
public:
LEFT left;
char oper;
RIGHT right;
InfixExpression(LEFT left,char oper,RIGHT right)
{
this->left=left;
this->right=right;
this->oper=oper;
}
};