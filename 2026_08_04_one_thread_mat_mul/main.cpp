#include "matrix.hpp"
#include <fstream>



int main()
{
    srand(time(nullptr));

    matrix<int> m1(5000, 1);
    for (int i = 0; i < m1.n(); ++i)
    {
        for (int j = 0; j < m1.m(); ++j)
        {
            m1.set(i + 1, j + 1) = rand() % 10 - 5;
        }
    }

    std::ofstream file1("m1");

    m1.show(file1);

    matrix<int> m2(1, 2000);

    for (int i = 0; i < m2.n(); ++i)
    {
        for (int j = 0; j < m2.m(); ++j)
        {
            m2.set(i + 1, j + 1) = rand() % 10 - 5;
        }
    }

    std::ofstream file2("m2");

    m2.show(file2);

    std::ofstream file3("m3");
    matrix m3 = m1 * m2;
    m3.show(file3);
}