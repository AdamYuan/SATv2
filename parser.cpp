#include "parser.hpp"
#include <sstream>
#include <fstream>
#include <algorithm>
#include <set>

CnfFile::CnfFile() : m_var_cnt(0), m_clause_cnt(0) {  }
CnfFile::CnfFile(const char *filename) : m_var_cnt(0), m_clause_cnt(0) { Parse(filename); }

std::set<Clause*> related_clauses_set[2][MAX_VAR_NUM];
void CnfFile::Parse(const char *filename)
{
	std::ifstream in_file(filename);
	if(!in_file.is_open())
		throw std::runtime_error("CNF FILE IO ERROR");

	std::string buf;

	int clause_arr_index = 0, line_id = 0;

#define THROW_PARSE_EXCEPTION throw std::runtime_error("CNF FILE PARSE ERROR AT LINE #" + std::to_string(line_id))
	while(std::getline(in_file, buf))
	{
		line_id ++;
		if(buf.empty()) continue;

		const char &head = buf.front();
		if(head == '%' || head == '0') break; //end
		else if(head == 'c') continue; //comment
		else if(head == 'p') //p cnf [m_var_cnt] [m_clause_cnt]
			sscanf(buf.c_str(), "%*c%*s%d%d", &m_var_cnt, &m_clause_cnt);
		else //n1 n2 n3 0
		{
			//range check
			if(clause_arr_index >= m_clause_cnt)
				THROW_PARSE_EXCEPTION;

			Clause &clause = m_clause_arr[clause_arr_index];
			clause.clause_index = clause_arr_index;

			std::istringstream str_input(buf);
			//counter: the count of number in this line
			int buf;
			while(str_input >> buf)
			{
				if(buf == 0) break;

				clause.elements.emplace_back();

				ElementPair &data = clause.elements.back();
				data.var_index = std::abs(buf) - 1;
				data.nagative = buf < 0;

				if(data.var_index >= m_var_cnt) 
					THROW_PARSE_EXCEPTION;

				related_clauses_set[data.nagative][data.var_index].insert(m_clause_arr + clause_arr_index);
			}
			if(!clause.elements.empty())
				clause_arr_index++;
		}
	}

	for(int i = 0; i < m_var_cnt; ++i)
	{
		//find true-forever clauses
		for(auto iter = related_clauses_set[0][i].begin(); iter != related_clauses_set[0][i].end(); )
		{
			Clause *ptr = *iter;
			if(related_clauses_set[1][i].count(ptr))
			{
				printf("WARNING PAREN #%d IS TRUE FOREVER\n", ptr->clause_index);
				related_clauses_set[1][i].erase(ptr);
				iter = related_clauses_set[0][i].erase(iter);
			}
			else
				++iter;
		}
		m_related_clauses_vec[0][i] = std::vector<const Clause*>(related_clauses_set[0][i].begin(), related_clauses_set[0][i].end());
		m_related_clauses_vec[1][i] = std::vector<const Clause*>(related_clauses_set[1][i].begin(), related_clauses_set[1][i].end());
	}

	printf("%s parse successful with:\n", filename);
	printf("var num: %d\nclause num: %d\n\n", m_var_cnt, m_clause_cnt);
}
