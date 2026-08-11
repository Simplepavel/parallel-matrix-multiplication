#include "matrix.hpp"
#include <fstream>
#include <string>
#include <chrono>

// matrix<int> fill(std::ifstream &input)
// {
//     unsigned int n;
//     unsigned int m;
//     input >> n >> m;
//     matrix<int> result(n, m);
//     for (int i = 0; i < n; ++i)
//     {
//         for (int j = 0; j < m; ++j)
//         {
//             input >> result.set(i + 1, j + 1);
//         }
//     }
//     return result;
// }

// bool test1(int &comp_time)
// {
//     std::ifstream input1("../tests/small1.txt");
//     std::ifstream input2("../tests/small2.txt");
//     std::ifstream result("../tests/small_result.txt");

//     matrix<int> m1 = fill(input1);
//     matrix<int> m2 = fill(input2);
//     matrix<int> True = fill(result);

//     auto start = std::chrono::steady_clock::now();
//     matrix<int> comp = (m1 * m2);
//     auto end = std::chrono::steady_clock::now();
//     comp_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//     return (comp == True);
// }

// bool test2(int &comp_time)
// {
//     std::ifstream input1("../tests/middle1.txt");
//     std::ifstream input2("../tests/middle2.txt");
//     std::ifstream result("../tests/middle_result.txt");

//     matrix<int> m1 = fill(input1);
//     matrix<int> m2 = fill(input2);
//     matrix<int> True = fill(result);

//     auto start = std::chrono::steady_clock::now();
//     matrix<int> comp = (m1 * m2);
//     auto end = std::chrono::steady_clock::now();
//     comp_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//     return (comp == True);
// }

// bool test3(int &comp_time)
// {
//     std::ifstream input1("../tests/large1.txt");
//     std::ifstream input2("../tests/large2.txt");
//     std::ifstream result("../tests/large_result.txt");

//     matrix<int> m1 = fill(input1);
//     matrix<int> m2 = fill(input2);
//     matrix<int> True = fill(result);

//     auto start = std::chrono::steady_clock::now();
//     matrix<int> comp = (m1 * m2);
//     auto end = std::chrono::steady_clock::now();
//     comp_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//     return (comp == True);
// }

int main()
{
    matrix<int> mother1(256, 256);

    for (int i = 1; i <= mother1.n(); ++i)
    {
        for (int j = 1; j <= mother1.m(); ++j)
        {
            mother1.set(i, j) = rand() % 10 - 5;
        }
    }
    std::ofstream file1("../tests/m1");
    mother1.show(file1);

    matrix<int> mother2(256, 256);

    for (int i = 1; i <= mother2.n(); ++i)
    {
        for (int j = 1; j <= mother2.m(); ++j)
        {
            mother2.set(i, j) = rand() % 10 - 5;
        }
    }

    std::ofstream file2("../tests/m2");
    mother2.show(file2);
    try
    {
        matrix<int> mother = strassen(mother1, mother2);
        std::ofstream file("../tests/m");
        mother.show(file);
    }
    catch (char const *message)
    {
        std::cout << message;
    }
    return 0;
}