#include "matrix.hpp"
#include <fstream>
#include <string>
#include <chrono>

int main()
{
    srand(time(nullptr));
    unsigned int size = 1024;



    int *data1 = new int[size * size]{0};
    matrix<int> m1;
    m1.set_data(data1);
    m1.set_bound(bound(0, size, size));
    for (int i = 0; i < m1.n(); ++i)
    {
        for (int j = 0; j < m1.m(); ++j)
        {
            m1.set(i + 1, j + 1) = rand() % 10 - 5;
        }
    }
    std::ofstream file1("../tests/m1");
    m1.show(file1);


    
    int *data2 = new int[size * size]{0};
    matrix<int> m2;
    m2.set_data(data2);
    m2.set_bound(bound(0, size, size));
    for (int i = 0; i < m2.n(); ++i)
    {
        for (int j = 0; j < m2.m(); ++j)
        {
            m2.set(i + 1, j + 1) = rand() % 10 - 5;
        }
    }
    std::ofstream file2("../tests/m2");
    m2.show(file2);

    int *data3 = new int[size * size]{0};
    auto start = std::chrono::steady_clock::now();
    matrix<int> q = strassen(m1, m2, data3);
    auto end = std::chrono::steady_clock::now();
    unsigned int seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    std::cout << "Time: " << seconds << 's';


    std::ofstream file3("../tests/m");
    q.show(file3);
    delete[] data1;
    delete[] data2;
    delete[] data3;
    return 0;
}