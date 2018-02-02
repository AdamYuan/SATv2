#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "parser.hpp"

class SatSolver
{
	public:
		unsigned long seed;
		int max_tries;
		float max_temperature, min_temperature;
		SatSolver();
		void OutputArgs();
		bool Run(CnfFile &file);
};

#endif
