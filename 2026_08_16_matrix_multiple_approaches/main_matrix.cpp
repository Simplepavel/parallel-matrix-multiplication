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

/*
В классической версии алгоритма имеем следующие недостатки:
При самом первом доступе к элементу матрицы A имеем 2 кеш-промаха: первый при доступе
к a[0](в кэш линию подгружаем адреса строк) и при обращении к a[0][0] (подгружаем первые числа первой строки)
Далее получаем кэш промахи каждые q шагов(если предположить, что длина кэш линии 64 байта, то имеем q = 64/sizeof(int))
Суммарно на всю матрицу имеем приблизительно l / q1 + l * m / q, где q1 = 64/sizeof(int*).

При работе с элементами матрицы B получаем минимум по 1 кэш промаху при каждом обращении, т.к сначала обращамся к адресу
b[k], которого возможно нет в кэше, затем переходим к b[k][j], которого точно не будет в кэше.
Суммарно на всю матрицу имеем приблизительно m*n + n/q1, где q1 = 64/sizeof(int*).


При работе с элементами матрицы C имеем проход по строкам, так что тут верна следующая оценка:
l/q1 + l * n / q.
*/

void multiply_matrix_as_in_math(
	int **a,
	std::size_t l, // строки 1й
	std::size_t m, // столбцы 1й, строки 2й матриц
	int **b,
	std::size_t n, // столбцы 2й
	int **c)
{
	int s;
	int q1;
	int q2;
	for (unsigned int i = 0; i < l; ++i)
	{
		for (unsigned int j = 0; j < n; ++j)
		{
			s = 0;
			for (unsigned int k = 0; k < m; ++k)
			{
				q1 = a[i][k];
				q2 = b[k][j];
				s += q1 * q2;
			}
			c[i][j] = s;
		}
	}
}

/*
В улучшенной версии алгоритма имеем следующие преимущества над классиеским умножением:
Для матриц A и C показатели остаются прежними т.к обход происходит так же по строкам

При работе с матрицей B получаем небольшой выигрыш. Теперь и в этом случае происходит обход по строкам
Верна cледующая оценка: m / q1 + m * n / q. (при q = q1 = 4 она оптимальнее предыдущей для любых натуральных n, m).
*/

void multiply_matrix_for_cache_v1(
	int **a,
	std::size_t l, // строки 1й
	std::size_t m, // столбцы 1й, строки 2й матриц
	int **b,
	std::size_t n, // столбцы 2й
	int **c)
{
	int q1;
	int q2;
	for (unsigned int i = 0; i < l; ++i)
	{
		for (unsigned int j = 0; j < m; ++j)
		{
			q1 = a[i][j];
			for (unsigned int k = 0; k < n; ++k)
			{
				q2 = b[j][k];
				c[i][k] += q1 * q2;
			}
		}
	}
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
