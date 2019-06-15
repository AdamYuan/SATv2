#include "data.hpp"
#include <random>
#include <cstdio>
Solution::Solution(const CnfFile &file) : m_file(file) {  }
void Solution::Randomize(std::mt19937 &generator)
{
	for(int i = 0; i < m_file.GetVarCount(); ++i)
		m_var_value[i] = generator() & 1;

	m_satisfied_cnt = 0;
	for(int i = 0; i < m_file.GetClauseCount(); ++i)
	{
		m_clause_value[i] = GetClauseVal(m_file.GetClauseArray() + i);
		m_satisfied_cnt += m_clause_value[i] ? 1 : 0;
	}
}

void Solution::Output() const
{
	for(int i = 0; i < m_file.GetVarCount(); ++i)
		printf("%d ", m_var_value[i]);
	printf("\n");
}

bool Solution::Check() const
{
	for(int i = 0; i < m_file.GetClauseCount(); ++i)
		if(!GetClauseVal(m_file.GetClauseArray() + i))
			return false;
	return true;
}
