#include <vector>
#include <iostream>
#include <queue>
#include <stack>
struct bound
{
    std::pair<int, int> start;
    std::pair<int, int> end;
};

struct task
{
    bound bound1;
    bound bound2;
};

template <typename T>
class matrix
{
    unsigned int _n; // row
    unsigned int _m; // column
    std::vector<T> _data;

public:
    matrix(unsigned int n, unsigned int m) : _n(n), _m(m), _data(n * m, 0) {}
    unsigned int n() const { return _n; }
    unsigned int m() const { return _m; }
    unsigned int count() const { return _m * _n; }
    const std::vector<T> &data() const { return _data; }
    T &set(unsigned int n, unsigned int m)
    {
        if (n < 1 || n > _n || m < 1 || m > _m)
        {
            throw "Out of bounds";
        }
        return _data[(n - 1) * _m + (m - 1)];
    }
    const T &get(unsigned int n, unsigned int m) const
    {
        if (n < 1 || n > _n || m < 1 || m > _m)
        {
            throw "Out of bounds";
        }
        return _data[(n - 1) * _m + (m - 1)];
    }
    void show(std::ostream &out)
    {
        for (int idx = 0; idx < _data.size(); ++idx)
        {
            out << _data[idx] << ' ';
            if (idx % _m == _m - 1)
            {
                out << '\n';
            }
        }
    }
};

template <typename T>
matrix<T> operator*(const matrix<T> &argv1, const matrix<T> &argv2)
{

    if (argv1.m() != argv2.n())
    {
        throw "invalid argument";
    }
    std::stack<task> stck;
    task t0;
    matrix<T> ans(argv1.n(), argv2.m());

    t0.bound1.start = std::pair<int, int>(0, 0);
    t0.bound1.end = std::pair<int, int>(argv1.n(), argv1.m());

    t0.bound2.start = std::pair<int, int>(0, 0);
    t0.bound2.end = std::pair<int, int>(argv2.n(), argv2.m());

    stck.push(t0);

    while (!stck.empty())
    {
        task t = stck.top();
        stck.pop();
        bound b1 = t.bound1;

        unsigned int n1 = b1.end.first - b1.start.first;
        unsigned int m1 = b1.end.second - b1.start.second;

        bound b2 = t.bound2;

        unsigned int n2 = b2.end.first - b2.start.first;
        unsigned int m2 = b2.end.second - b2.start.second;

        if (n1 * m1 < 10000 && n2 * m2 < 10000)
        {
            for (unsigned int i = b1.start.first; i < b1.end.first; ++i) // номер строки 1-й матрицы
            {
                for (unsigned int j = b2.start.second; j < b2.end.second; ++j)
                {
                    T result = 0;
                    for (unsigned int k = b1.start.second; k < b1.end.second; ++k)
                    {
                        int q1 = argv1.get(i + 1, k + 1);
                        int q2 = argv2.get(k + 1, j + 1);
                        result += q1 * q2;
                    }
                    ans.set(i + 1, j + 1) += result;
                }
            }
        }
        else
        {
            if (n1 > 1 && m1 > 1 && n2 > 1 && m2 > 1)
            {
                task t1;
                t1.bound1.start = std::pair<int, int>(b1.start.first, b1.start.second);
                t1.bound1.end = std::pair<int, int>(b1.start.first + n1 / 2, b1.start.second + m1 / 2);

                t1.bound2.start = std::pair<int, int>(b2.start.first, b2.start.second);
                t1.bound2.end = std::pair<int, int>(b2.start.first + n2 / 2, b2.start.second + m2 / 2);

                stck.push(t1);

                task t2;
                t2.bound1.start = std::pair<int, int>(b1.start.first, b1.start.second + m1 / 2);
                t2.bound1.end = std::pair<int, int>(b1.start.first + n1 / 2, b1.start.second + m1);

                t2.bound2.start = std::pair<int, int>(b2.start.first + n2 / 2, b2.start.second);
                t2.bound2.end = std::pair<int, int>(b2.start.first + n2, b2.start.second + m2 / 2);

                stck.push(t2);

                task t3;
                t3.bound1.start = std::pair<int, int>(b1.start.first, b1.start.second);
                t3.bound1.end = std::pair<int, int>(b1.start.first + n1 / 2, b1.start.second + m1 / 2);

                t3.bound2.start = std::pair<int, int>(b2.start.first, b2.start.second + m2 / 2);
                t3.bound2.end = std::pair<int, int>(b2.start.first + n2 / 2, b2.start.second + m2);

                stck.push(t3);

                task t4;

                t4.bound1.start = std::pair<int, int>(b1.start.first, b1.start.second + m1 / 2);
                t4.bound1.end = std::pair<int, int>(b1.start.first + n1 / 2, b1.start.second + m1);

                t4.bound2.start = std::pair<int, int>(b2.start.first + n2 / 2, b2.start.second + m2 / 2);
                t4.bound2.end = std::pair<int, int>(b2.start.first + n2, b2.start.second + m2);

                stck.push(t4);

                task t5;

                t5.bound1.start = std::pair<int, int>(b1.start.first + n1 / 2, b1.start.second);
                t5.bound1.end = std::pair<int, int>(b1.start.first + n1, b1.start.second + m1 / 2);

                t5.bound2.start = std::pair<int, int>(b2.start.first, b2.start.second);
                t5.bound2.end = std::pair<int, int>(b2.start.first + n2 / 2, b2.start.second + m2 / 2);

                stck.push(t5);

                task t6;

                t6.bound1.start = std::pair<int, int>(b1.start.first + n1 / 2, b1.start.second + m1 / 2);
                t6.bound1.end = std::pair<int, int>(b1.start.first + n1, b1.start.second + m1);

                t6.bound2.start = std::pair<int, int>(b2.start.first + n2 / 2, b2.start.second);
                t6.bound2.end = std::pair<int, int>(b2.start.first + n2, b2.start.second + m2 / 2);

                stck.push(t6);

                task t7;

                t7.bound1.start = std::pair<int, int>(b1.start.first + n1 / 2, b1.start.second);
                t7.bound1.end = std::pair<int, int>(b1.start.first + n1, b1.start.second + m1 / 2);

                t7.bound2.start = std::pair<int, int>(b2.start.first, b2.start.second + m2 / 2);
                t7.bound2.end = std::pair<int, int>(b2.start.first + n2 / 2, b2.start.second + m2);

                stck.push(t7);

                task t8;

                t8.bound1.start = std::pair<int, int>(b1.start.first + n1 / 2, b1.start.second + m1 / 2);
                t8.bound1.end = std::pair<int, int>(b1.start.first + n1, b1.start.second + m1);

                t8.bound2.start = std::pair<int, int>(b2.start.first + n2 / 2, b2.start.second + m2 / 2);
                t8.bound2.end = std::pair<int, int>(b2.start.first + n2, b2.start.second + m2);

                stck.push(t8);
            }
            else if (n1 == 1 && m1 > 1 && n2 > 1 && m2 > 1)
            {
                task t1;
                t1.bound1.start = std::pair<int, int>(b1.start.first, b1.start.second);
                t1.bound1.end = std::pair<int, int>(b1.end.first + n1, b1.end.second + m1 / 2);

                t1.bound2.start = std::pair<int, int>(b2.start.first, b2.start.second);
                t1.bound2.end = std::pair<int, int>(b2.end.first + n2 / 2, b2.end.second + m2);

                stck.push(t1);

                task t2;

                t2.bound1.start = std::pair<int, int>(b1.start.first, b1.start.second + m1 / 2);
                t2.bound1.end = std::pair<int, int>(b1.end.first + n1, b1.end.second + m1);

                t2.bound2.start = std::pair<int, int>(b2.start.first + n2 / 2, b2.start.second);
                t2.bound2.end = std::pair<int, int>(b2.end.first + n2, b2.end.second + m2);

                stck.push(t2);
            }
            else if (n1 > 1 && m1 == 1 && n2 == 1 && m2 > 1)
            {
                task t1;
                t1.bound1.start = std::pair<int, int>(b1.start.first, b1.start.second);
                t1.bound1.end = std::pair<int, int>(b1.end.first + n1 / 2, b1.end.second + m1);

                t1.bound2.start = std::pair<int, int>(b2.start.first, b2.start.second);
                t1.bound2.end = std::pair<int, int>(b2.end.first + n2, b2.end.second + m2 / 2);

                stck.push(t1);

                task t2;

                t2.bound1.start = std::pair<int, int>(b1.start.first + n1 / 2, b1.start.second);
                t2.bound1.end = std::pair<int, int>(b1.end.first + n1, b1.end.second + m1);

                t2.bound2.start = std::pair<int, int>(b2.start.first, b2.start.second + m2 / 2);
                t2.bound2.end = std::pair<int, int>(b2.end.first + n2, b2.end.second + m2);

                stck.push(t2);
            }
            else if (n1 > 1 && m1 > 1 && n2 > 1 && m2 == 1)
            {
                task t1;
                t1.bound1.start = std::pair<int, int>(b1.start.first, b1.start.second);
                t1.bound1.end = std::pair<int, int>(b1.end.first + n1, b1.end.second + m1 / 2);

                t1.bound2.start = std::pair<int, int>(b2.start.first, b2.start.second);
                t1.bound2.end = std::pair<int, int>(b2.end.first + n2 / 2, b2.end.second + m2);

                stck.push(t1);

                task t2;

                t2.bound1.start = std::pair<int, int>(b1.start.first, b1.start.second + m1 / 2);
                t2.bound1.end = std::pair<int, int>(b1.end.first + n1, b1.end.second + m1);

                t2.bound2.start = std::pair<int, int>(b2.start.first + n2 / 2, b2.start.second);
                t2.bound2.end = std::pair<int, int>(b2.end.first + n2, b2.end.second + m2);

                stck.push(t2);
            }
        }
    }
    return ans;
}