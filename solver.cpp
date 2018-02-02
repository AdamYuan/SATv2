#include "solver.hpp"
#include "data.hpp"
#include <random>
#include <chrono>
#include <cmath>

SatSolver::SatSolver() : max_tries(128), max_temperature(0.3f), min_temperature(0.01f)
{
	seed = (unsigned long)(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

void SatSolver::OutputArgs()
{
	printf("seed: %lu\nmax tries: %d\nmax temperature: %f\nmin temperature: %f\n", 
			seed, max_tries, max_temperature, min_temperature);
}

bool SatSolver::Run(CnfFile &file)
{
	std::mt19937 generator(seed);
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

	int var_num = file.GetVarNum();
	Solution solution(file);
	for(int k=1; k<=max_tries; ++k)
	{
		printf("\rTRY NO. %d", k);
		fflush(stdout);
		solution.Randomize(generator);

		int times = 0;
		float temperature = max_temperature;
		while(temperature > min_temperature)
		{
			if(solution.Satisfied())
			{
				printf("\n");
				solution.Output();
				return solution.Check();
			}
			temperature = max_temperature * expf(float(-(times++)) / float(var_num * k));

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
	}
	return false;
}
