#include "parser.hpp"
#include <sstream>
#include <fstream>
#include <algorithm>

CnfFile::CnfFile() : var_num_(0), paren_num_(0) {  }
CnfFile::CnfFile(const char *filename) : var_num_(0), paren_num_(0) { Parse(filename); }

void CnfFile::Parse(const char *filename)
{
	std::ifstream in_file(filename);
	if(!in_file.is_open())
		throw std::runtime_error("READ CNF FILE IO ERROR");

	std::string buf;

	int parens_array_index = 0, line_ind = 0;

#define THROW_PARSE_EXCEPTION throw std::runtime_error("READ CNF FILE PARSE ERROR AT #" + std::to_string(line_ind))
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

			int vec[PAREN_SIZE];

			Paren &paren = parens_array_[parens_array_index];
			paren.paren_index = parens_array_index;

			std::istringstream str_input(buf);
			for(int ind=0; ind<PAREN_SIZE; ++ind)
			{
				str_input >> vec[ind];
				ElementPair &data = paren.elements[ind];
				data = {std::abs(vec[ind]) - 1, vec[ind] < 0};

				if(data.var_index >= var_num_) 
					THROW_PARSE_EXCEPTION;

				related_parens_array_[data.nagative][data.var_index]
					.push_back(parens_array_ + parens_array_index);
			}
			parens_array_index++;
		}
	}

	for(int ind=0; ind<var_num_; ++ind)
	{
		std::vector<const Paren*> &vec0 = related_parens_array_[0][ind];
		std::sort(vec0.begin(), vec0.end());
		vec0.erase(std::unique(vec0.begin(), vec0.end()), vec0.end());

		std::vector<const Paren*> &vec1 = related_parens_array_[1][ind];
		std::sort(vec1.begin(), vec1.end());
		vec1.erase(std::unique(vec1.begin(), vec1.end()), vec1.end());
	}

	printf("%s parse successful with:\n", filename);
	printf("var num: %d\nparen num: %d\n\n", var_num_, paren_num_);
}
