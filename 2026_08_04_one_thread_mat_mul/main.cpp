#include "matrix.hpp"
#include <fstream>
#include <string>
#include <chrono>

matrix<int> fill(std::ifstream &input)
{
    unsigned int n;
    unsigned int m;
    input >> n >> m;
    matrix<int> result(n, m);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            input >> result.set(i + 1, j + 1);
        }
    }
    return result;
}

bool test1(int &comp_time)
{
    std::ifstream input1("../tests/small1.txt");
    std::ifstream input2("../tests/small2.txt");
    std::ifstream result("../tests/small_result.txt");

    matrix<int> m1 = fill(input1);
    matrix<int> m2 = fill(input2);
    matrix<int> True = fill(result);

    auto start = std::chrono::steady_clock::now();
    matrix<int> comp = (m1 * m2);
    auto end = std::chrono::steady_clock::now();
    comp_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    return (comp == True);
}

bool test2(int &comp_time)
{
    std::ifstream input1("../tests/middle1.txt");
    std::ifstream input2("../tests/middle2.txt");
    std::ifstream result("../tests/middle_result.txt");

    matrix<int> m1 = fill(input1);
    matrix<int> m2 = fill(input2);
    matrix<int> True = fill(result);

    auto start = std::chrono::steady_clock::now();
    matrix<int> comp = (m1 * m2);
    auto end = std::chrono::steady_clock::now();
    comp_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    return (comp == True);
}

bool test3(int &comp_time)
{
    std::ifstream input1("../tests/large1.txt");
    std::ifstream input2("../tests/large2.txt");
    std::ifstream result("../tests/large_result.txt");

    matrix<int> m1 = fill(input1);
    matrix<int> m2 = fill(input2);
    matrix<int> True = fill(result);

    auto start = std::chrono::steady_clock::now();
    matrix<int> comp = (m1 * m2);
    auto end = std::chrono::steady_clock::now();
    comp_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    return (comp == True);
}

int main()
{

    int comp1;
    if (test1(comp1))
    {
        std::cout << "Test 1: Done\n";
        std::cout << "Duration: " << comp1 << " ms\n";
    }
    else
    {
        std::cout << "Test 1: Fail\n";
        return 1;
    }

    int comp2;
    if (test2(comp2))
    {
        std::cout << "Test 2: Done\n";
        std::cout << "Duration: " << comp2 << " ms\n";
    }
    else
    {
        std::cout << "Test 2: Fail\n";
        return 1;
    }

    int comp3;
    if (test3(comp3))
    {
        std::cout << "Test 3: Done\n";
        std::cout << "Duration: " << comp3 << " ms\n";
    }
    else
    {
        std::cout << "Test 3: Fail\n";
        return 1;
    }
    return 0;
}