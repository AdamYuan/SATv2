#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "config.hpp"

struct ElementPair
{
	int var_index;
	bool nagative;
};
struct Paren
{
	int paren_index;
	ElementPair elements[PAREN_SIZE];
};
class CnfFile
{
	private:
		int var_num_, paren_num_;
		Paren parens_array_[MAX_PAREN_NUM];
		std::vector<const Paren*> related_parens_vector_[2][MAX_VAR_NUM];

	public:
		explicit CnfFile();
		explicit CnfFile(const char *filename);
		void Parse(const char *filename);
		inline int GetVarNum() const { return var_num_; }
		inline int GetParenNum() const { return paren_num_; }
		inline const std::vector<const Paren*> &GetRelatedParens(bool nagative, int var_index) const 
		{ return related_parens_vector_[nagative][var_index]; }
		inline const Paren *GetParensArray() const { return parens_array_; }
};

#endif
