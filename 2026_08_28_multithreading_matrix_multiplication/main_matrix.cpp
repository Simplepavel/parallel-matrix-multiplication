/**
	Многопоточные варианты реализации умножения матриц на CPU.

	Количество запускаемых потоков равно количеству ядер машины. Т.е. программа
	предварительно узнаёт количество потоков у машины, а потом этот параметр
	передаёт в функцию реализации.
*/
#include <cstddef>
#include <iostream>
#include <fstream>
#include "stck.hpp"

/*
	Идея всех алгоритмов одна и та же: один поток раздает задачи(кладет их в стек). Обычно это основной поток
	программы. Остальные же потоки(рабочие) получают доступ к задачам и данным. Тип задачи зависит от конретного алгоритма
	и содержит параметры задачи.

	В классической версии алгоритма(C = A * B) задача одного рабочего потока - это выполнить умножение столбца матрицы B на все матрицу A.
	В V1 версии задача одного потока - это умножить строку A на матрицу B(при этом обеспечивается преимущества кэш памяти).
	В V2 версии задача одного потока - это умножение блоков, размер которых помещается в кэш полностью.
*/

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
	int thread_count = std::thread::hardware_concurrency(); // TODO
	{
		// Обычное умножение матриц из алгебры без оптимизаций
		start = std::chrono::steady_clock::now();
		multiply_matrix_as_in_math_with_multithreading(
			thread_count,
			matrix_a, size, size,
			matrix_b, size,
			matrix_c);
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
			matrix_c);
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
			matrix_c);
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

// Math
void multiply_matrix_as_in_math_with_multithreading(
	int thread_count,
	int **a,
	std::size_t l,
	std::size_t m,
	int **b,
	std::size_t n,
	int **c)
{
	ts_stck<unsigned int> tasks(n);
	std::thread *pool = new std::thread[thread_count];
	for (int i = 0; i < thread_count; ++i)
	{
		auto lambda = [&]()
		{
			unsigned int column;
			while (tasks.pop_or_wait(column))
			{
				for (unsigned int i = 0; i < m; ++i)
				{
					for (unsigned int j = 0; j < l; ++j)
					{
						c[j][column] += a[j][i] * b[i][column];
					}
				}
			}
		};
		pool[i] = std::thread(lambda);
	}

	for (unsigned int i = 0; i < n; ++i)
	{
		tasks.push(i);
	}

	tasks.end();
	for (int i = 0; i < thread_count; ++i)
	{
		pool[i].join();
	}
	delete[] pool;
}

// V1

void multiply_matrix_for_cache_v1_with_multithreading(
	int thread_count,
	int **a,
	std::size_t l,
	std::size_t m,
	int **b,
	std::size_t n,
	int **c)
{
	ts_stck<unsigned int> tasks(l); // номер строки для вычисления
	std::thread *pool = new std::thread[thread_count];
	for (int i = 0; i < thread_count; ++i)
	{
		auto lambda = [&]()
		{
			unsigned int row;
			while (tasks.pop_or_wait(row))
			{
				for (unsigned int i = 0; i < m; ++i)
				{
					for (unsigned int j = 0; j < n; ++j)
					{
						c[row][j] += a[row][i] * b[i][j];
					}
				}
			}
		};
		pool[i] = std::thread(lambda);
	}

	for (unsigned int i = 0; i < l; ++i)
	{
		tasks.push(i);
	}
	tasks.end();

	for (int i = 0; i < thread_count; ++i)
	{
		pool[i].join();
	}

	delete[] pool;
}

// V2

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

void processing(ts_stck<task> &stack, int **a, int **b, int **c, std::mutex &mtx, std::size_t block_size)
{
	int *buffer1 = new int[block_size];
	int *buffer2 = new int[block_size];
	int *buffer = new int[block_size];
	task t;
	unsigned int idx;

	unsigned int _m1;
	unsigned int _m2;

	int q1;
	int q2;

	int result;
	while (stack.pop_or_wait(t))
	{
		bound &b1 = t.b1;
		bound &b2 = t.b2;
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

		// Считаем скопированные значения
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
				buffer[i * _m2 + j] = result;
			}
		}

		// Копирование результа (Тут защита mutex)

		{
			std::lock_guard<std::mutex> lk(mtx);
			for (unsigned int i = 0; i < row; ++i)
			{
				for (unsigned int j = 0; j < _m2; ++j)
				{
					c[b1.row_start + i][b2.column_start + j] += buffer[i * _m2 + j];
				}
			}
		}
	}
	delete[] buffer1;
	delete[] buffer2;
	delete[] buffer;
}

void multiply_matrix_for_cache_v2_with_multithreading(
	int thread_count,
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
	stck<task> stack(7 * degree); // в этот стек кладем задачи по разбиению(доступ с одного потока)

	/*
	Памяти может не хватить!!!! Точная оценка требует O(n^3)
	Решение: написать более медленный стек через односвязный список
	*/
	ts_stck<task> tasks(mx * mx); // в этот стек кладем задачи по умножению

	stack.push(task(bound(0, 0, l, m), bound(0, 0, m, n)));

	std::mutex mtx;
	std::thread *pool = new std::thread[thread_count];

	std::size_t block_size = 64 * 64;

	for (int i = 0; i < thread_count; ++i)
	{

		pool[i] = std::thread(processing, std::ref(tasks), a, b, c, std::ref(mtx), block_size);
	}

	task t1;
	task t2;
	task t3;
	task t4;
	task t5;
	task t6;
	task t7;
	task t8;

	unsigned int n1;
	unsigned int m1;

	unsigned int n2;
	unsigned int m2;

	task t;
	while (!stack.empty())
	{
		task t = stack.top();
		bound &b1 = t.b1;
		bound &b2 = t.b2;
		stack.pop();
		if (b1.count() < block_size && b2.count() < block_size)
		{
			tasks.push(t);
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
	tasks.end();
	for (int i = 0; i < thread_count; ++i)
	{
		pool[i].join();
	}
	delete[] pool;
}