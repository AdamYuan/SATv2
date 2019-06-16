#ifndef DATA_HPP
#define DATA_HPP

#include "parser.hpp"
#include "config.hpp"
#include <cstring>
#include <random>

struct ModifyInfo
{
	int clause_index, value;
};

class Solution
{
	private:
		bool m_var_value[MAX_VAR_NUM];
		int m_clause_value[MAX_PAREN_NUM];
		int m_satisfied_cnt, m_satisfied_cnt_tmp, m_clause_val_modify_cnt;

		ModifyInfo m_clause_val_modify_arr[MAX_PAREN_NUM];
		const CnfFile &m_file;
		inline int GetClauseVal(const Clause *clause) const
		{
			int ret = 0;
			for(const ElementPair &e : clause->elements)
				if(e.nagative != m_var_value[e.var_index]) ++ret;
			return ret;
		}
	public:
		Solution(const CnfFile &file);
		void Randomize(std::mt19937 &generator);
		inline int TestFlip(int var_index)
		{
			m_satisfied_cnt_tmp = m_satisfied_cnt;
			m_clause_val_modify_cnt = 0;

			const std::vector<const Clause*> &related_clauses = m_file.GetRelatedClauses(m_var_value[var_index], var_index);
			const std::vector<const Clause*> &related_clauses_update = m_file.GetRelatedClauses(!m_var_value[var_index], var_index);

			for(const Clause* ptr : related_clauses)
			{
				m_clause_val_modify_arr[m_clause_val_modify_cnt++] = {ptr->clause_index, 1};
				if(m_clause_value[ptr->clause_index] == 0) m_satisfied_cnt_tmp ++;
			}
			for(const Clause* ptr : related_clauses_update)
			{
				m_clause_val_modify_arr[m_clause_val_modify_cnt++] = {ptr->clause_index, -1};
				if(m_clause_value[ptr->clause_index] == 1) m_satisfied_cnt_tmp --;
			}
			return m_satisfied_cnt_tmp;
		}
		inline void ApplyFlip(int var_index)
		{
			m_var_value[var_index] ^= 1;
			while(m_clause_val_modify_cnt--)
			{
				const ModifyInfo &info = m_clause_val_modify_arr[m_clause_val_modify_cnt];
				m_clause_value[info.clause_index] += info.value;
			}
			m_satisfied_cnt = m_satisfied_cnt_tmp;
		}
		void Output() const;
		bool Check() const;
		inline int GetVarCount() const { return m_file.GetVarCount(); }
		inline int GetClauseCount() const { return m_file.GetClauseCount(); }
		inline int GetSatisfiedCount() const { return m_satisfied_cnt; }
		inline bool Satisfied() const { return m_satisfied_cnt == GetClauseCount(); }
};

#endif
