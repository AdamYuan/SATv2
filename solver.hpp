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
	void Run(const CnfFile &file);

	bool try_impl(std::mt19937 &generator, Solution &solution, int try_id);
};
#endif
