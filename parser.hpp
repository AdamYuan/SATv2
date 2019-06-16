#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "config.hpp"

struct ElementPair { int var_index; bool nagative; };
struct Clause { int clause_index; std::vector<ElementPair> elements; };

class CnfFile
{
	private:
		int m_var_cnt, m_clause_cnt;
		Clause m_clause_arr[MAX_PAREN_NUM];
		std::vector<const Clause*> m_related_clauses_vec[2][MAX_VAR_NUM];

	public:
		explicit CnfFile();
		explicit CnfFile(const char *filename);
		void Parse(const char *filename);
		inline int GetVarCount() const { return m_var_cnt; }
		inline int GetClauseCount() const { return m_clause_cnt; }
		inline const std::vector<const Clause*> &GetRelatedClauses(bool nagative, int var_index) const 
		{ return m_related_clauses_vec[nagative][var_index]; }
		inline const Clause *GetClauseArray() const { return m_clause_arr; }
};

#endif
