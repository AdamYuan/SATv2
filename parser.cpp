#include "parser.hpp"
#include <sstream>
#include <fstream>
#include <algorithm>
#include <set>

CnfFile::CnfFile() : var_num_(0), paren_num_(0) {  }
CnfFile::CnfFile(const char *filename) : var_num_(0), paren_num_(0) { Parse(filename); }

void CnfFile::Parse(const char *filename)
{
	std::ifstream in_file(filename);
	if(!in_file.is_open())
		throw std::runtime_error("CNF FILE IO ERROR");

	std::string buf;

	int parens_array_index = 0, line_ind = 0;

	std::set<Paren*> related_parens_set[2][MAX_VAR_NUM];
#define THROW_PARSE_EXCEPTION throw std::runtime_error("CNF FILE PARSE ERROR AT LINE #" + std::to_string(line_ind))
	while(std::getline(in_file, buf))
	{
		line_ind ++;
		if(buf.empty()) continue;

		const char &head = buf.front();
		if(head == '%' || head == '0') break; //end
		else if(head == 'c') continue; //comment
		else if(head == 'p') //p cnf [var_num_] [paren_num_]
			sscanf(buf.c_str(), "%*c%*s%d%d", &var_num_, &paren_num_);
		else //n1 n2 n3 0
		{
			//range check
			if(parens_array_index >= paren_num_)
				THROW_PARSE_EXCEPTION;

			Paren &paren = parens_array_[parens_array_index];
			paren.paren_index = parens_array_index;

			std::istringstream str_input(buf);
			int counter = 0, buf;
			while(str_input >> buf)
			{
				if(buf == 0) break;

				ElementPair &data = paren.elements[counter];
				data = {std::abs(buf) - 1, buf < 0};

				if(data.var_index >= var_num_) 
					THROW_PARSE_EXCEPTION;

				related_parens_set[data.nagative][data.var_index].insert(parens_array_ + parens_array_index);

				counter ++;
			}
			if(counter != 0)
			{
				if(counter != PAREN_SIZE)
					THROW_PARSE_EXCEPTION;
				parens_array_index++;
			}
		}
	}

	for(int ind=0; ind<var_num_; ++ind)
	{
		//find true-forever parens
		for(auto iter = related_parens_set[0][ind].begin(); iter != related_parens_set[0][ind].end(); )
		{
			Paren *ptr = *iter;
			if(related_parens_set[1][ind].count(ptr))
			{
				printf("WARNING PAREN #%d IS TRUE FOREVER\n", ptr->paren_index);
				related_parens_set[1][ind].erase(ptr);
				iter = related_parens_set[0][ind].erase(iter);
			}
			else
				++iter;
		}
		for(int t=0; t<=1; ++t)
			related_parens_vector_[t][ind] = std::vector<const Paren*>(related_parens_set[t][ind].begin(), related_parens_set[t][ind].end());
	}

	printf("%s parse successful with:\n", filename);
	printf("var num: %d\nparen num: %d\n\n", var_num_, paren_num_);
}
