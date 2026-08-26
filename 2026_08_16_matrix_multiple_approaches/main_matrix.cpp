#include <chrono>
#include <cstddef>
#include <iostream>
#include <fstream>

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


struct bound
{
	unsigned int row_start;
	unsigned int column_start;

	unsigned int row_end;
	unsigned int column_end;
	bound() : row_start(0), column_start(0), row_end(0), column_end(0) {}
	bound(unsigned int rs, unsigned int cs, unsigned int re, unsigned int ce) : row_start(rs), column_start(cs), row_end(re), column_end(ce)
	{
	}
	unsigned int count()
	{
		return (row_end - row_start) * (column_end - column_start);
	}
};

struct task
{
	bound b1;
	bound b2;
	task() {};
	task(const bound &_b1, const bound &_b2) : b1(_b1), b2(_b2) {}
};

template <typename T>
struct stck
{
	T *data;
	unsigned int size;
	unsigned int capacity; // удалить данное поле

	stck(T *new_data, unsigned int _cap) : data(new_data), size(0), capacity(_cap) {}
	void push(const T &value)
	{
		data[size++] = value;
	}
	T &top()
	{
		if (size == 0)
		{
			throw "nothing on top\n";
		}
		return data[size - 1];
	}
	void pop()
	{
		if (size == 0)
		{
			throw "nothing to pop\n";
		}
		--size;
	}
	bool empty() { return size == 0; }
};
std::size_t log2(std::size_t t)
{
	std::size_t ans = 0;
	while (t > 0)
	{
		t >>= 1;
		++ans;
	}
	return ans;
}

void multiply_matrix_for_cache_v2(
	int **a,
	std::size_t l,
	std::size_t m,
	int **b,
	std::size_t n,
	int **c)
{
	std::size_t mx = l;
	if (m > mx)
		mx = m;
	if (n > mx)
		mx = n;
	std::size_t degree = log2(mx);
	task *data = new task[7 * degree];
	std::size_t block_size = 64 * 64;
	int *buffer1 = new int[block_size];
	int *buffer2 = new int[block_size];
	stck<task> stack(data, 7 * degree);
	std::size_t idx;
	stack.push(task(bound(0, 0, l, m), bound(0, 0, m, n)));

	int S;
	int q1;
	int q2;

	unsigned int n1;
	unsigned int m1;

	unsigned int n2;
	unsigned int m2;

	unsigned int _m1;
	unsigned int _m2;

	int result;

	task t1;
	task t2;
	task t3;
	task t4;
	task t5;
	task t6;
	task t7;
	task t8;
	while (!stack.empty())
	{
		bound &b1 = stack.top().b1;
		bound &b2 = stack.top().b2;
		stack.pop();
		if (b1.count() < block_size && b2.count() < block_size)
		{
			// копирование
			idx = 0;
			for (std::size_t i = b1.row_start; i < b1.row_end; ++i)
			{
				for (std::size_t j = b1.column_start; j < b1.column_end; ++j)
				{
					buffer1[idx++] = a[i][j];
				}
			}
			idx = 0;
			for (std::size_t i = b2.row_start; i < b2.row_end; ++i)
			{
				for (std::size_t j = b2.column_start; j < b2.column_end; ++j)
				{
					buffer2[idx++] = b[i][j];
				}
			}
			// копирование
			_m1 = b1.column_end - b1.column_start;
			_m2 = b2.column_end - b2.column_start;

			unsigned int row = b1.row_end - b1.row_start;
			for (unsigned int i = 0; i < row; ++i)
			{
				for (unsigned int j = 0; j < _m2; ++j)
				{
					result = 0;
					for (unsigned int k = 0; k < _m1; ++k)
					{
						q1 = buffer1[i * _m1 + k];
						q2 = buffer2[k * _m2 + j];
						result += q1 * q2;
					}
					c[b1.row_start + i][b2.column_start + j] += result;
				}
			}
		}
		else
		{
			m1 = b1.row_end - b1.row_start;
			n1 = b1.column_end - b1.column_start;

			m2 = b2.row_end - b2.row_start;
			n2 = b2.column_end - b2.column_start;

			if (m1 > 1 && n1 > 1 && m2 > 1 && n2 > 1)
			{

				// A11 * B11
				t1.b1.row_start = b1.row_start;
				t1.b1.column_start = b1.column_start;

				t1.b1.row_end = b1.row_start + m1 / 2;
				t1.b1.column_end = b1.column_start + n1 / 2;

				t1.b2.row_start = b2.row_start;
				t1.b2.column_start = b2.column_start;

				t1.b2.row_end = b2.row_start + m2 / 2;
				t1.b2.column_end = b2.column_start + n2 / 2;

				stack.push(t1);

				// A12 * B21
				t2.b1.row_start = b1.row_start;
				t2.b1.column_start = b1.column_start + n1 / 2;

				t2.b1.row_end = b1.row_start + m1 / 2;
				t2.b1.column_end = b1.column_start + n1;

				t2.b2.row_start = b2.row_start + m2 / 2;
				t2.b2.column_start = b2.column_start;

				t2.b2.row_end = b2.row_start + m2;
				t2.b2.column_end = b2.column_start + n2 / 2;

				stack.push(t2);

				// A11 * B12

				t3.b1.row_start = b1.row_start;
				t3.b1.column_start = b1.column_start;

				t3.b1.row_end = b1.row_start + m1 / 2;
				t3.b1.column_end = b1.column_start + n1 / 2;

				t3.b2.row_start = b2.row_start;
				t3.b2.column_start = b2.column_start + n2 / 2;

				t3.b2.row_end = b2.row_start + m2 / 2;
				t3.b2.column_end = b2.column_start + n2;

				stack.push(t3);

				// A12 * B22
				t4.b1.row_start = b1.row_start;
				t4.b1.column_start = b1.column_start + n1 / 2;

				t4.b1.row_end = b1.row_start + m1 / 2;
				t4.b1.column_end = b1.column_start + n1;

				t4.b2.row_start = b2.row_start + m2 / 2;
				t4.b2.column_start = b2.column_start + n2 / 2;

				t4.b2.row_end = b2.row_start + m2;
				t4.b2.column_end = b2.column_start + n2;

				stack.push(t4);

				// A21 * B11

				t5.b1.row_start = b1.row_start + m1 / 2;
				t5.b1.column_start = b1.column_start;

				t5.b1.row_end = b1.row_start + m1;
				t5.b1.column_end = b1.column_start + n1 / 2;

				t5.b2.row_start = b2.row_start;
				t5.b2.column_start = b2.column_start;

				t5.b2.row_end = b2.row_start + m2 / 2;
				t5.b2.column_end = b2.column_start + n2 / 2;

				stack.push(t5);

				// A22 * B21

				t6.b1.row_start = b1.row_start + m1 / 2;
				t6.b1.column_start = b1.column_start + n1 / 2;

				t6.b1.row_end = b1.row_start + m1;
				t6.b1.column_end = b1.column_start + n1;

				t6.b2.row_start = b2.row_start + m2 / 2;
				t6.b2.column_start = b2.column_start;

				t6.b2.row_end = b2.row_start + m2;
				t6.b2.column_end = b2.column_start + n2 / 2;

				stack.push(t6);

				// A21 * B12

				t7.b1.row_start = b1.row_start + m1 / 2;
				t7.b1.column_start = b1.column_start;

				t7.b1.row_end = b1.row_start + m1;
				t7.b1.column_end = b1.column_start + n1 / 2;

				t7.b2.row_start = b2.row_start;
				t7.b2.column_start = b2.column_start + n2 / 2;

				t7.b2.row_end = b2.row_start + m2 / 2;
				t7.b2.column_end = b2.column_start + n2;

				stack.push(t7);

				// A22 * B22

				t8.b1.row_start = b1.row_start + m1 / 2;
				t8.b1.column_start = b1.column_start + n1 / 2;

				t8.b1.row_end = b1.row_start + m1;
				t8.b1.column_end = b1.column_start + n1;

				t8.b2.row_start = b2.row_start + m2 / 2;
				t8.b2.column_start = b2.column_start + n2 / 2;

				t8.b2.row_end = b2.row_start + m2;
				t8.b2.column_end = b2.column_start + n2;

				stack.push(t8);
			}
			else if (m1 == 1 && n1 > 1 && m2 > 1 && n2 > 1)
			{
				t1.b1.row_start = b1.row_start;
				t1.b1.column_start = b1.column_start;

				t1.b1.row_end = b1.row_start + m1;
				t1.b1.column_end = b1.column_start + n1 / 2;

				t1.b2.row_start = b2.row_start;
				t1.b2.column_start = b2.column_start;

				t1.b2.row_end = b2.row_start + m2 / 2;
				t1.b2.column_end = b2.column_start + n2;

				stack.push(t1);

				t2.b1.row_start = b1.row_start;
				t2.b1.column_start = b1.column_start + n1 / 2;

				t2.b1.row_end = b1.row_start + m1;
				t2.b1.column_end = b1.column_start + n1;

				t2.b2.row_start = b2.row_start + m2 / 2;
				t2.b2.column_start = b2.column_start;

				t2.b2.row_end = b2.row_start + m2;
				t2.b2.column_end = b2.column_start + n2;

				stack.push(t2);
			}

			else if (m1 > 1 && n1 == 1 && m2 == 1 && n2 > 1)
			{
				std::cout << "1\n";
				t1.b1.row_start = b1.row_start;
				t1.b1.column_start = b1.column_start;

				t1.b1.row_end = b1.row_start + m1 / 2;
				t1.b1.column_end = b1.column_start + n1;

				t1.b2.row_start = b2.row_start;
				t1.b2.column_start = b2.column_start;

				t1.b2.row_end = b2.row_start + m2;
				t1.b2.column_end = b2.column_start + n2 / 2;

				stack.push(t1);

				t2.b1.row_start = b1.row_start + m1 / 2;
				t2.b1.column_start = b1.column_start;

				t2.b1.row_end = b1.row_start + m1;
				t2.b1.column_end = b1.column_start + n1;

				t2.b2.row_start = b2.row_start;
				t2.b2.column_start = b2.column_start + n2 / 2;

				t2.b2.row_end = b2.row_start + m2;
				t2.b2.column_end = b2.column_start + n2;

				stack.push(t2);
			}
			else if (m1 > 1 && n1 > 1 && m2 > 1 && n2 == 1)
			{
				t1.b1.row_start = b1.row_start;
				t1.b1.column_start = b1.column_start;

				t1.b1.row_end = b1.row_start + m1;
				t1.b1.column_end = b1.column_start + n1 / 2;

				t1.b2.row_start = b2.row_start;
				t1.b2.column_start = b2.column_start;

				t1.b2.row_end = b2.row_start + m2 / 2;
				t1.b2.column_end = b2.column_start + n2;

				stack.push(t1);

				t2.b1.row_start = b1.row_start;
				t2.b1.column_start = b1.column_start + n1 / 2;

				t2.b1.row_end = b1.row_start + m1;
				t2.b1.column_end = b1.column_start + n1;

				t2.b2.row_start = b2.row_start + m2 / 2;
				t2.b2.column_start = b2.column_start;

				t2.b2.row_end = b2.row_start + m2;
				t2.b2.column_end = b2.column_start + n2;

				stack.push(t2);
			}
		}
	}
	delete[] buffer1;
	delete[] buffer2;
	delete[] data;
}
