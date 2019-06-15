#include "solver.hpp"
#include <chrono>
#include <cmath>
#include <future>
#include <atomic>
#include <mutex>

SatSolver::SatSolver() : 
	multi_thread(false), max_tries(50), max_temperature(0.3f), min_temperature(0.01f)
{
	std::random_device rd{};
	seed = rd();
}

void SatSolver::OutputArgs()
{
	printf("multithreaded: %d\nseed: %lu\nmax tries: %d\nmax temperature: %f\nmin temperature: %f\n\n", 
			multi_thread, seed, max_tries, max_temperature, min_temperature);
}

void SatSolver::Run(const CnfFile &file)
{
	auto start = std::chrono::system_clock::now();

#define PRINT_TRY_NO(x) printf("\rTRY NO. %d", (x)); fflush(stdout)
#define PRINT_ELAPSED_SECONDS printf("time: %lf sec, ", std::chrono::duration<double>(std::chrono::system_clock::now() - start).count())
#define PRINT_CORRECT(stmt) printf("correct: %d\n", stmt);
#define PRINT_SOLUTION(solution, try_num) { printf("\n"); solution.Output(); PRINT_ELAPSED_SECONDS; printf("try: %d, ", (try_num)); PRINT_CORRECT(solution.Check()); }
	if(multi_thread)
	{
		unsigned cores = std::thread::hardware_concurrency();
		std::vector<std::future<void>> future_vector;
		future_vector.reserve(cores);

		std::atomic_int try_id(1);
		std::mt19937 seed_generator(seed);
		std::mutex output_mutex; //lock for stdout

		while(cores --)
		{
			future_vector.push_back(std::async(
						[&](unsigned long thread_seed) -> void
						{
							std::mt19937 generator(thread_seed);
							Solution solution(file);

							while(true)
							{
								int index = try_id ++;
								if(index > max_tries)
									break;

								if(output_mutex.try_lock())
								{
									PRINT_TRY_NO(index);
									output_mutex.unlock();
								}

								if(try_impl(generator, solution, index))
								{
									try_id = max_tries + 1;

									std::lock_guard<std::mutex> guard(output_mutex);
									PRINT_SOLUTION(solution, index);
									break;
								}
							}
						}, seed_generator() //pass seed
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
			{
				PRINT_SOLUTION(solution, i);
				break;
			}
		}
	}

#undef PRINT_TRY_NO
#undef PRINT_CORRECT
#undef PRINT_ELAPSED_SECONDS
#undef PRINT_SOLUTION
}

bool SatSolver::try_impl(std::mt19937 &generator, Solution &solution, int try_id)
{
	int var_cnt = solution.GetVarCount();
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	solution.Randomize(generator);

	const float kDivider = float(var_cnt * try_id);

	int counter = 0;
	float temperature = max_temperature;
	while(temperature > min_temperature)
	{
		if(solution.Satisfied())
			return true;
		temperature = max_temperature * expf(float(-(counter++)) / kDivider);

		for(int ind = 0; ind < var_cnt; ++ind)
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
