#include <cstdio>
#include <cstring>
#include "solver.hpp"

static const char kHelpStr[]{
	"AdamYuan's 3-SAT solver\n"\
		"\t-in [CNF FILE NAME]\n"
		"\t-max_tries [MAX TRIES]\n"\
		"\t-max_temp [MAX TEMPERATURE]\n"\
		"\t-min_temp [MIN TEMPERATURE]\n"\
		"\t-seed [RANDOM SEED]\n"\
		"\t-mthread\n"};

#define PRINT_HELP_AND_EXIT { printf("%s\n", kHelpStr); return 1; }

int main(int argc, char **argv)
{
	argc--; argv++;

	CnfFile file;
	SatSolver solver;
	char filename[512] = {};

	if(argc == 0)
		PRINT_HELP_AND_EXIT;
	for(int i = 0; i < argc; )
	{
		char *(&cmd){argv[i]};
		if(strcmp("-mthread", cmd) == 0)
		{
			solver.multi_thread = true;
			++ i;
		}
		else if(i < argc - 1)
		{
			char *(&arg){argv[i + 1]};
			if(strcmp("-in", cmd) == 0)
				strcpy(filename, arg);
			else if(strcmp("-max_tries", cmd) == 0)
				sscanf(arg, "%d", &solver.max_tries);
			else if(strcmp("-max_temp", cmd) == 0)
				sscanf(arg, "%f", &solver.max_temperature);
			else if(strcmp("-min_temp", cmd) == 0)
				sscanf(arg, "%f", &solver.min_temperature);
			else if(strcmp("-seed", cmd) == 0)
				sscanf(arg, "%lu", &solver.seed);
			else
				PRINT_HELP_AND_EXIT;
			i += 2;
		}
		else
			PRINT_HELP_AND_EXIT;
	}

	file.Parse(filename);
	solver.OutputArgs();
	solver.Run(file);
}
