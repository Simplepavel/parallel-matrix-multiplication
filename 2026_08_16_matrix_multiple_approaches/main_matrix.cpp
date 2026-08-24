#include <chrono>
#include <cstddef>
#include <iostream>

void multiply_matrix_as_in_math(
	int **a,
	std::size_t l,
	std::size_t m,
	int **b,
	std::size_t n,
	int **c);

void multiply_matrix_for_cache_v1(
	int **a,
	std::size_t l,
	std::size_t m,
	int **b,
	std::size_t n,
	int **c);

void multiply_matrix_for_cache_v2(
	int **a,
	std::size_t l,
	std::size_t m,
	int **b,
	std::size_t n,
	int **c);

int main()
{
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

	// Однопоточные умножения
	{
		// Обычное умножение матриц из алгебры без оптимизаций
		start = std::chrono::steady_clock::now();
		multiply_matrix_as_in_math(matrix_a, size, size, matrix_b, size, matrix_c);
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
		multiply_matrix_for_cache_v1(matrix_a, size, size, matrix_b, size, matrix_c);
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
		multiply_matrix_for_cache_v2(matrix_a, size, size, matrix_b, size, matrix_c);
		end = std::chrono::steady_clock::now();

		elapsed_seconds = end - start;
		std::cout
			<< "Время умножения матриц для кэша (вариант 2) = "
			<< elapsed_seconds.count()
			<< " секунд."
			<< std::endl;
	}

	return 0;
}

void multiply_matrix_as_in_math(
	int **a,
	std::size_t l,
	std::size_t m,
	int **b,
	std::size_t n,
	int **c)
{
	for (unsigned int i = 0; i < l; ++i)
	{
		for (unsigned int j = 0; j < n; ++j)
		{
			int s = 0;
			for (unsigned int k = 0; k < m; ++k)
			{
				int q1 = a[i][k];
				int q2 = b[k][j];
				s += q1 * q2;
			}
			c[i][j] = s;
		}
	}
}

/*
	TODO: написать комментарий о том, в чём состоит оптимизация
*/
void multiply_matrix_for_cache_v1(
	int **a,
	std::size_t l,
	std::size_t m,
	int **b,
	std::size_t n,
	int **c)
{
	// TODO
}

/*
	TODO: написать комментарий о том, в чём состоит оптимизация
*/
void multiply_matrix_for_cache_v2(
	int **a,
	std::size_t l,
	std::size_t m,
	int **b,
	std::size_t n,
	int **c)
{
	// TODO
}
