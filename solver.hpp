#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "parser.hpp"
#include "data.hpp"
#include <random>

struct SatSolver
{
	bool multi_thread;
	unsigned long seed;
	int max_tries;
	float max_temperature, min_temperature;
	SatSolver();
	void OutputArgs();
	void Run(CnfFile &file);
	inline bool try_impl(std::mt19937 &generator, Solution &solution, int try_id)
	{
		int var_num = solution.GetVarNum();
		std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
		solution.Randomize(generator);

		int counter = 0;
		float temperature = max_temperature;
		while(temperature > min_temperature)
		{
			if(solution.Satisfied())
				return true;
			temperature = max_temperature * expf(float(-(counter++)) / float(var_num * try_id));

			for(int ind=0; ind<var_num; ++ind)
			{
				int new_satisfied_count = solution.TestFlip(ind);

				float separator = 1.0f / (
						1.0f + expf(
							float(solution.GetSatisfiedCount() - new_satisfied_count) / temperature
							)
						);
				if(distribution(generator) < separator)
					solution.ApplyFlip(ind);
			}
		}
		return false;
	}
};

#endif
