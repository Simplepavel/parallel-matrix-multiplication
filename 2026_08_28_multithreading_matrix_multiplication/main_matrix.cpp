/**
	Многопоточные варианты реализации умножения матриц на CPU. 

	Количество запускаемых потоков равно количеству ядер машины. Т.е. программа 
	предварительно узнаёт количество потоков у машины, а потом этот параметр 
	передаёт в функцию реализации.
*/
#include <chrono>
#include <cstddef>
#include <iostream>
#include <fstream>

// TODO: добавляем многопоточность к существующим подходам.
void multiply_matrix_as_in_math_with_multithreading(
	int thread_count,
	int **a,
	std::size_t l,
	std::size_t m,
	int **b,
	std::size_t n,
	int **c);

void multiply_matrix_for_cache_v1_with_multithreading(
	int thread_count,
	int **a,
	std::size_t l,
	std::size_t m,
	int **b,
	std::size_t n,
	int **c);

void multiply_matrix_for_cache_v2_with_multithreading(
	int thread_count,
	int **a,
	std::size_t l,
	std::size_t m,
	int **b,
	std::size_t n,
	int **c);

// TODO: Поискать ещё варианты произведения матриц при использовании многопоточности.

std::size_t log2(std::size_t t);

int main()
{
	srand(time(nullptr));
	const std::size_t size = 1000;
	std::cout << "Объём данных = " << size << std::endl;

	int **matrix_a = new int *[size];
	for (std::size_t i = 0; i < size; i++)
	{
		matrix_a[i] = new int[size];
	}
	for (std::size_t i = 0; i < size; i++)
	{
		for (std::size_t j = 0; j < size; j++)
		{
			matrix_a[i][j] = 1;
		}
	}

	int **matrix_b = new int *[size];
	for (std::size_t i = 0; i < size; i++)
	{
		matrix_b[i] = new int[size];
	}
	for (std::size_t i = 0; i < size; i++)
	{
		for (std::size_t j = 0; j < size; j++)
		{
			matrix_b[i][j] = 1;
		}
	}

	int **matrix_c = new int *[size];
	for (std::size_t i = 0; i < size; i++)
	{
		matrix_c[i] = new int[size];
	}
	for (std::size_t i = 0; i < size; i++)
	{
		for (std::size_t j = 0; j < size; j++)
		{
			matrix_c[i][j] = 0;
		}
	}

	auto start{std::chrono::steady_clock::now()};
	auto end{start};
	std::chrono::duration<double> elapsed_seconds;

	// Многопоточные умножения
	int thread_count; // TODO
	{
		// Обычное умножение матриц из алгебры без оптимизаций
		start = std::chrono::steady_clock::now();
		multiply_matrix_as_in_math_with_multithreading(
			thread_count,
			matrix_a, size, size, 
			matrix_b, size, 
			matrix_c
		);
		end = std::chrono::steady_clock::now();

		elapsed_seconds = end - start;
		std::cout
			<< "Время умножения матриц как в математике = "
			<< elapsed_seconds.count()
			<< " секунд."
			<< std::endl;
	}
	{
		// Умножение матриц с оптимизацией для кэша вариант 1
		for (std::size_t i = 0; i < size; i++)
		{
			for (std::size_t j = 0; j < size; j++)
			{
				matrix_c[i][j] = 0;
			}
		}

		start = std::chrono::steady_clock::now();
		multiply_matrix_for_cache_v1_with_multithreading(
			thread_count,
			matrix_a, size, size, 
			matrix_b, size, 
			matrix_c
		);
		end = std::chrono::steady_clock::now();

		elapsed_seconds = end - start;
		std::cout
			<< "Время умножения матриц для кэша (вариант 1) = "
			<< elapsed_seconds.count()
			<< " секунд."
			<< std::endl;
	}
	{
		// Умножение матриц с оптимизацией для кэша вариант 2
		for (std::size_t i = 0; i < size; i++)
		{
			for (std::size_t j = 0; j < size; j++)
			{
				matrix_c[i][j] = 0;
			}
		}

		start = std::chrono::steady_clock::now();
		multiply_matrix_for_cache_v2_with_multithreading(
			thread_count,
			matrix_a, size, size, 
			matrix_b, size, 
			matrix_c
		);
		end = std::chrono::steady_clock::now();

		elapsed_seconds = end - start;
		std::cout
			<< "Время умножения матриц для кэша (вариант 2) = "
			<< elapsed_seconds.count()
			<< " секунд."
			<< std::endl;
	}
	{
		// TODO: Поискать ещё варианты произведения матриц при использовании многопоточности.

	}

	return 0;
}

// TODO