#ifndef DATA_HPP
#define DATA_HPP

#include "parser.hpp"
#include "config.hpp"
#include <cstring>
#include <random>

struct ModifyInfo
{
	int paren_index;
	bool value;
};

class Solution
{
	private:
		int var_num_, paren_num_;
		CnfFile &file_;
		bool var_value_array_[MAX_VAR_NUM], paren_value_array_[MAX_PAREN_NUM];
		ModifyInfo paren_value_modify_array_[MAX_PAREN_NUM];
		int satisfied_count_, satisfied_count_tmp_, paren_value_modify_count_;
		inline bool getParenVal(const Paren *paren) const
		{
			for(int ind=0; ind<PAREN_SIZE; ++ind)
			{
				const ElementPair &element = paren->elements[ind];
				if(element.nagative != var_value_array_[element.var_index])
					return true;
			}
			return false;
		}
	public:
		explicit Solution(CnfFile &file);
		void Randomize(std::mt19937 &generator);
		inline int TestFlip(int var_index)
		{
			satisfied_count_tmp_ = satisfied_count_;
			paren_value_modify_count_ = 0;

			bool &var_ref = var_value_array_[var_index];
			const std::vector<const Paren*> &related_parens = file_.GetRelatedParens(var_ref, var_index);
			var_ref = !var_ref;
			const std::vector<const Paren*> &related_parens_update = file_.GetRelatedParens(var_ref, var_index);

			for(const Paren* ptr : related_parens)
			{
				if(!paren_value_array_[ptr->paren_index])
				{
					paren_value_modify_array_[paren_value_modify_count_++] = {ptr->paren_index, true};
					satisfied_count_tmp_ ++;
				}
			}

			for(const Paren* ptr : related_parens_update)
			{
				const bool &paren_value_old = paren_value_array_[ptr->paren_index];
				bool paren_value_new = getParenVal(ptr);
				if(paren_value_new != paren_value_old)
				{
					paren_value_modify_array_[paren_value_modify_count_++] = {ptr->paren_index, paren_value_new};
					satisfied_count_tmp_ += (int)paren_value_new - (int)paren_value_old;
				}
			}
			var_ref = !var_ref;
			return satisfied_count_tmp_;
		}
		inline void ApplyFlip(int var_index)
		{
			var_value_array_[var_index] = !var_value_array_[var_index];
			while(paren_value_modify_count_--)
			{
				const ModifyInfo &info = paren_value_modify_array_[paren_value_modify_count_];
				paren_value_array_[info.paren_index] = info.value;
			}
			satisfied_count_ = satisfied_count_tmp_;
		}
		void Output() const;
		bool Check() const;
		inline int GetSatisfiedCount() const { return satisfied_count_; }
		inline bool Satisfied() const { return satisfied_count_ == paren_num_; }
};

#endif
