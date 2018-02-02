#include "data.hpp"
#include <random>
#include <cstdio>
Solution::Solution(CnfFile &file) : file_(file)
{
	var_num_ = file.GetVarNum();
	paren_num_ = file.GetParenNum();
}
void Solution::Randomize(std::mt19937 &generator)
{
	std::uniform_int_distribution<int> distribution(0, 1);
	for(int ind=0; ind<var_num_; ++ind)
		var_value_array_[ind] = distribution(generator);

	satisfied_count_ = 0;
	for(int ind=0; ind<paren_num_; ++ind)
	{
		paren_value_array_[ind] = getParenVal(file_.GetParensArray() + ind);
		satisfied_count_ += paren_value_array_[ind];
	}
}

void Solution::Output() const
{
	for(int ind=0; ind<var_num_; ++ind)
		printf("%d ", var_value_array_[ind]);
	printf("\n");
}

bool Solution::Check() const
{
	for(int ind=0; ind<paren_num_; ++ind)
		if(!getParenVal(file_.GetParensArray() + ind))
			return false;
	return true;
}
