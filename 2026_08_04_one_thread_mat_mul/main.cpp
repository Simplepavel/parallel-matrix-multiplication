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
    std::shared_ptr<int[]> data(new int[64]{});
    matrix<int> m1;
    matrix<int> m2;
    m1.set_data(data);
    m2.set_data(data);
    m1.set_bound(bound(0, 8, 8));
    m2.set_bound(bound(0, 8, 8));
    m1.set(1, 1) = 256;
    m1.set(2, 2) = 12;
    m1.set(3, 3) = 81;
    m1.set(4, 4) = 11;
    m1.set(5, 5) = -8;
    m1.show(std::cout);
    strassen(m1, m2);
    return 0;
}