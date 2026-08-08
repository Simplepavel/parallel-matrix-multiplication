#include <vector>
#include <iostream>
#include <memory>
#include <queue>

template <typename T>
struct data
{
    std::vector<T> _numbers;
    unsigned int _n;
    unsigned int _m;
    data(unsigned int n, unsigned int m) : _n(n), _m(m), _numbers(n * m, 0) {}
};

enum operations
{
    PLUS,
    MINUS,
    MULTIPLY
};

struct bound
{
    unsigned int row_start;
    unsigned int column_start;
    unsigned int row_end;
    unsigned int column_end;

    bound(unsigned int _row_start, unsigned int _row_end, unsigned int _column_start, unsigned int _column_end)
    {
        if (_row_start > _row_end || _column_start > _column_end)
        {
            throw "invalid argument(bound)";
        }
        row_start = _row_start;
        column_start = _column_start;

        row_end = _row_end;
        column_end = _column_end;
    }
};

template <typename T>
class matrix
{
    std::shared_ptr<data<T>> _data;
    bound _bound;

public:
    matrix(unsigned int n, unsigned int m) : _data(std::make_shared<data<T>>(n, m)), _bound(0, n, 0, m) {}
    matrix(const matrix<T> &parent, const bound &_b) : _data(parent._data), _bound(_b)
    {
        _bound.row_start += parent._bound.row_start;
        _bound.column_start += parent._bound.column_start;
        _bound.row_end += parent._bound.row_start;
        _bound.column_end += parent._bound.column_start;
    }
    void show(std::ostream &cout)
    {
        unsigned int step = _data->_m;
        for (unsigned int i = _bound.row_start; i < _bound.row_end; ++i)
        {
            for (unsigned int j = _bound.column_start; j < _bound.column_end; ++j)
            {
                cout << _data->_numbers[i * step + j] << ' ';
            }
            cout << '\n';
        }
    }
    T &set(unsigned int n, unsigned int m)
    {
        unsigned int _n = _bound.row_end - _bound.row_start;
        unsigned int _m = _bound.column_end - _bound.column_start;
        unsigned int step = _data->_m;
        if (n < 1 || n > _n || m < 1 || m > _m)
        {
            throw "Out of bounds";
        }
        unsigned int idx1 = _bound.row_start + n - 1;
        unsigned int idx2 = _bound.column_start + m - 1;
        return _data->_numbers[idx1 * step + idx2];
    }

    const T &get(unsigned int n, unsigned int m) const
    {
        unsigned int _n = _bound.row_end - _bound.row_start;
        unsigned int _m = _bound.column_end - _bound.column_start;
        unsigned int step = _data->_m;
        if (n < 1 || n > _n || m < 1 || m > _m)
        {
            throw "Out of bounds";
        }
        unsigned int idx1 = _bound.row_start + n - 1;
        unsigned int idx2 = _bound.column_start + m - 1;
        return _data->_numbers[idx1 * step + idx2];
    }
    std::vector<matrix<T>> split()
    {
        unsigned int _n = _bound.row_end - _bound.row_start;
        unsigned int _m = _bound.column_end - _bound.column_start;
        const matrix<T> &mother = *this;
        if (_n == 1 && _m > 1)
        {
            bound b1(0, mother.n(), 0, mother.m() / 2);
            matrix<int> A11(mother, b1);

            bound b2(0, mother.n(), mother.m() / 2, mother.m());
            matrix<int> A12(mother, b2);

            return std::vector<matrix<T>>{A11, A12};
        }
        if (_n > 1 && _m == 1)
        {
            bound b1(0, mother.n() / 2, 0, mother.m());
            matrix<int> A11(mother, b1);

            bound b3(mother.n() / 2, mother.n(), 0, mother.m());
            matrix<int> A21(mother, b3);

            return std::vector<matrix<T>>{A11, A21};
        }
        if (_n == 1 && _m == 1)
        {
            bound b1(0, mother.n(), 0, mother.m());
            matrix<int> A11(mother, b1);
            return std::vector<matrix<int>>{A11};
        }

        bound b1(0, mother.n() / 2, 0, mother.m() / 2);
        matrix<int> A11(mother, b1);

        bound b2(0, mother.n() / 2, mother.m() / 2, mother.m());
        matrix<int> A12(mother, b2);

        bound b3(mother.n() / 2, mother.n(), 0, mother.m() / 2);
        matrix<int> A21(mother, b3);

        bound b4(mother.n() / 2, mother.n(), mother.m() / 2, mother.m());
        matrix<int> A22(mother, b4);

        return std::vector<matrix<T>>{A11, A12, A21, A22};
    }
    unsigned int n() const { return _bound.row_end - _bound.row_start; }
    unsigned int m() const { return _bound.column_end - _bound.column_start; }
    unsigned int count() const { return n() * m(); }
};

template <typename U>
struct task
{
    matrix<U> argv1;
    matrix<U> argv2;
    operations operation;
};

template <typename T>
matrix<T> operator*(const matrix<T> &argv1, const matrix<T> &argv2)
{
    if (argv1.m() != argv2.n())
    {
        throw "size mismatch";
    }
    std::queue<task<T>> qqq;
    matrix<T> ans(argv1.n(), argv2.m());
    task<T> t0;
    t0.argv1 = argv1;
    t0.argv2 = argv2;
    t0.operation = operations::MULTIPLY;

    qqq.push(t0);

    while (!qqq.empty())
    {
        task<T> t = qqq.front();
        qqq.pop();
        if (t.argv1.count() < 10000 && t.argv2.count() < 10000)
        {
            for (unsigned int i = t.argv1._bound.row_start; i < t.argv1._bound.row_end; ++i) // номер строки 1-й матрицы
            {
                for (unsigned int j = t.argv2._bound.column_start; j < t.argv2._bound.column_end; ++j)
                {
                    T result = 0;
                    for (unsigned int k = t.argv1._bound.column_start; k < t.argv1._bound.column_end; ++k)
                    {
                        {
                            T q1 = t.argv1.get(i + 1, k + 1);
                            T q2 = t.argv2.get(k + 1, j + 1);
                            result += q1 * q2;
                        }
                        ans.set(i + 1, j + 1) += result;
                    }
                }
            }
        }
        else
        {
            std::vector<matrix<T>> daugther1 = t.argv1.split();
            std::vector<matrix<T>> daugther2 = t.argv2.split();
            if (t.argv1.n() > 1 && t.argv1.m() > 1 && t.argv2.n() > 1 && t.argv2.m() > 1)
            {
                 
            }
        }
    }
}

template <typename T>
matrix<T> operator+(const matrix<T> &argv1, const matrix<T> &argv2)
{
    if (argv1.n() != argv2.n() || argv1.m() != argv2.m())
    {
        throw "size mismatch";
    }
    matrix<T> result(argv1.n(), argv1.m());
    for (int i = 0; i < argv1.count(); ++i)
    {
        unsigned int idx1 = i / argv1.m();
        unsigned int idx2 = i % argv1.m();
        result.set(idx1 + 1, idx2 + 1) = argv1.get(idx1 + 1, idx2 + 1) + argv2.get(idx1 + 1, idx2 + 1);
    }
    return result;
}

template <typename T>
matrix<T> operator-(const matrix<T> &argv1, const matrix<T> &argv2)
{
    if (argv1.n() != argv2.n() || argv1.m() != argv2.m())
    {
        throw "size mismatch";
    }
    matrix<T> result(argv1.n(), argv1.m());
    for (int i = 0; i < argv1.count(); ++i)
    {
        unsigned int idx1 = i / argv1.m();
        unsigned int idx2 = i % argv1.m();
        result.set(idx1 + 1, idx2 + 1) = argv1.get(idx1 + 1, idx2 + 1) - argv2.get(idx1 + 1, idx2 + 1);
    }
    return result;
}