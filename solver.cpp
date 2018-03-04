#include "solver.hpp"
#include <chrono>
#include <cmath>
#include <future>
#include <atomic>
#include <mutex>

SatSolver::SatSolver() : multi_thread(false), max_tries(50), max_temperature(0.3f), min_temperature(0.01f)
{
	seed = (unsigned long)(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

void SatSolver::OutputArgs()
{
	printf("seed: %lu\nmax tries: %d\nmax temperature: %f\nmin temperature: %f\n\n", 
			seed, max_tries, max_temperature, min_temperature);
}

void SatSolver::Run(CnfFile &file)
{
	auto start = std::chrono::system_clock::now();

#define PRINT_CORRECT(stmt) printf("correct: %d\n", stmt);
#define PRINT_TRY_NO(x) printf("\rTRY NO. %d", (x)); fflush(stdout)
#define PRINT_ELAPSED_SECONDS printf("time: %lf sec\n", std::chrono::duration<double>(std::chrono::system_clock::now() - start).count())
#define PRINT_SOLUTION(solution) printf("\n"); solution.Output(); PRINT_ELAPSED_SECONDS; PRINT_CORRECT(solution.Check())

	if(multi_thread)
	{
		unsigned cores = std::thread::hardware_concurrency();
		std::vector<std::future<void>> future_vector;
		future_vector.reserve(cores);

		std::atomic_int try_id{1};
		std::mt19937 seed_generator(seed);
		std::mutex output_mutex;

		while(cores --)
		{
			future_vector.push_back(std::async(
						[&](unsigned long thread_seed)
						{
							std::mt19937 generator(thread_seed);
							Solution solution(file);
							while(true)
							{
								int index = try_id ++;
								if(index > max_tries)
									break;
								PRINT_TRY_NO(index);

								if(try_impl(generator, solution, index))
								{
									try_id = max_tries + 1;

									std::lock_guard<std::mutex> guard(output_mutex); //set lock to prevent format error
									PRINT_SOLUTION(solution);
									break;
								}
							}
						}, seed_generator()
			));
		}
	}
	else
	{
		std::mt19937 generator(seed);
		Solution solution(file);
		for(int i=1; i<=max_tries; ++i)
		{
			PRINT_TRY_NO(i);
			if(try_impl(generator, solution, i))
				break;
		}
		PRINT_SOLUTION(solution);
	}

#undef PRINT_TRY_NO
#undef PRINT_CORRECT
#undef PRINT_ELAPSED_SECONDS
#undef PRINT_SOLUTION
}
