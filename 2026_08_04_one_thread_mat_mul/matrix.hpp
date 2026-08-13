#include <vector>
#include <iostream>
#include <memory>
#include <stack>

template <typename T>
struct data
{
    std::vector<T> _numbers;
    unsigned int _n;
    unsigned int _m;
    data(unsigned int n, unsigned int m) : _n(n), _m(m), _numbers(n * m, 0) {}
};

struct bound
{
    unsigned int row_start;
    unsigned int column_start;
    unsigned int row_end;
    unsigned int column_end;
    // bound() : row_start(0), column_start(0), row_end(0), column_end(0) {}
    bound(const unsigned int &_row_start, const unsigned int &_row_end, const unsigned int &_column_start, const unsigned int &_column_end)
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

    unsigned int n() const { return row_end - row_start; }
    unsigned int m() const { return column_end - column_start; }
};

enum letters : char
{
    NONE,
    D,
    D1,
    D2,
    H1,
    H2,
    V1,
    V2
};

template <typename T>
class matrix
{
    std::shared_ptr<data<T>> _data;
    bound _bound;

public:
    matrix() : _data(nullptr), _bound(0, 0, 0, 0) {};
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

    void insert(const matrix<T> &argv, letters letter)
    {
        if (n() != argv.n() * 2)
        {
            qwertuiyoqwert throw "size mismatch";
        }
        switch (letter)
        {
        case (letters::D):
            plus(bound(0, n() / 2, 0, n() / 2), argv);
            plus(bound(n() / 2, n(), n() / 2, n()), argv);
            break;
        case (letters::D1):
            plus(bound(0, n() / 2, 0, n() / 2), argv);
            break;
        case (letters::D2):
            plus(bound(n() / 2, n(), n() / 2, n()), argv);
            break;
        case (letters::H1):
            minus(bound(0, n() / 2, 0, n() / 2), argv);
            plus(bound(0, n() / 2, n() / 2, n()), argv);
            break;
        case (letters::H2):
            plus(bound(n() / 2, n(), 0, n() / 2), argv);
            minus(bound(n() / 2, n(), n() / 2, n()), argv);
            break;
        case (letters::V1):
            plus(bound(n() / 2, n(), 0, n() / 2), argv);
            plus(bound(0, n() / 2, 0, n() / 2), argv);
            break;
        case (letters::V2):
            plus(bound(0, n() / 2, n() / 2, n()), argv);
            plus(bound(n() / 2, n(), n() / 2, n()), argv);
            break;
        default:
            break;
        }
    }

    void plus(const bound &b, const matrix<T> &argv)
    {
        if (argv.n() != b.n() || argv.m() != b.m())
        {
            throw "size mismatch";
        }
        for (unsigned int i = 0; i < argv.n(); ++i)
        {
            for (unsigned int j = 0; j < argv.m(); ++j)
            {
                set(b.row_start + i + 1, b.column_start + j + 1) += argv.get(i + 1, j + 1);
            }
        }
    }

    void minus(bound b, const matrix<T> &argv)
    {
        if (argv.n() != b.n() || argv.m() != b.m())
        {
            throw "size mismatch";
        }
        for (unsigned int i = 0; i < argv.n(); ++i)
        {
            for (unsigned int j = 0; j < argv.m(); ++j)
            {
                set(b.row_start + i + 1, b.column_start + j + 1) -= argv.get(i + 1, j + 1);
            }
        }
    }
};

template <typename U>
struct task
{
    matrix<U> argv1;
    matrix<U> argv2;
    letters letter;
};

template <typename T>
/*
для понятности буду использовать только n() вызов, т.к n = m по предположению
*/
matrix<T> strassen(const matrix<T> &argv1, const matrix<T> &argv2)
{
    if (argv1.n() != argv2.n())
    {
        throw "size mismatch";
    }
    std::stack<task<T>> ttt;   // tasks
    std::stack<matrix<T>> rrr; // results
    std::stack<unsigned int> counter;

    task<T> t0;
    t0.argv1 = argv1;
    t0.argv2 = argv2;

    ttt.push(t0);

    while (true)
    {
        if (ttt.empty())
        {
            while (counter.size() > 1 && counter.top() == 7)
            {
                unsigned int n = rrr.top().n();
                matrix<T> A(2 * n, 2 * n);

                for (auto i : {D, D1, D2, H1, H2, V1, V2})
                {
                    A.insert(rrr.top(), i);
                    rrr.pop();
                }
                counter.pop();
                rrr.push(A);
                counter.top() += 1;
            }
            break;
        }
        task<T> t = ttt.top();
        ttt.pop();

        if (t.argv1.count() < 4 && t.argv2.count() < 4)
        {
            unsigned int n = t.argv1.n();
            matrix<T> result(n, n);
            for (unsigned int i = 0; i < n; ++i)
            {
                for (unsigned int j = 0; j < n; ++j)
                {
                    T summ = 0;
                    for (unsigned int k = 0; k < n; ++k)
                    {
                        T q1 = t.argv1.get(i + 1, k + 1);
                        T q2 = t.argv2.get(k + 1, j + 1);

                        summ += q1 * q2;
                    }
                    result.set(i + 1, j + 1) = summ;
                }
            }
            rrr.push(result);
            counter.top() += 1;
        }
        else
        {
            while (!counter.empty() && counter.top() == 7)
            {
                unsigned int n = rrr.top().n();
                matrix<T> A(2 * n, 2 * n);

                for (auto i : {D, D1, D2, H1, H2, V1, V2})
                {
                    A.insert(rrr.top(), i);
                    rrr.pop();
                }
                counter.pop();
                rrr.push(A);
                counter.top() += 1;
            }

            counter.push(0);
            std::vector<matrix<T>> daughter1 = t.argv1.split();
            std::vector<matrix<T>> daughter2 = t.argv2.split();

            task<T> t1;
            t1.argv1 = daughter1[0] + daughter1[3];
            t1.argv2 = daughter2[0] + daughter2[3];
            t1.letter = D;
            ttt.push(t1);

            task<T> t2;
            t2.argv1 = daughter1[1] - daughter1[3];
            t2.argv2 = daughter2[2] + daughter2[3];
            t2.letter = D1;
            ttt.push(t2);

            task<T> t3;
            t3.argv1 = daughter1[2] - daughter1[0];
            t3.argv2 = daughter2[0] + daughter2[1];
            t3.letter = D2;
            ttt.push(t3);

            task<T> t4;
            t4.argv1 = daughter1[0] + daughter1[1];
            t4.argv2 = daughter2[3];
            t4.letter = H1;
            ttt.push(t4);

            task<T> t5;
            t5.argv1 = daughter1[2] + daughter1[3];
            t5.argv2 = daughter2[0];
            t5.letter = H2;
            ttt.push(t5);

            task<T> t6;
            t6.argv1 = daughter1[3];
            t6.argv2 = daughter2[2] - daughter2[0];
            t6.letter = V1;
            ttt.push(t6);

            task<T> t7;
            t7.argv1 = daughter1[0];
            t7.argv2 = daughter2[1] - daughter2[3];
            t7.letter = V2;
            ttt.push(t7);
        }
    }

    unsigned int n = rrr.top().n();
    matrix<T> A(2 * n, 2 * n);

    for (auto i : {D, D1, D2, H1, H2, V1, V2})
    {
        A.insert(rrr.top(), i);
        rrr.pop();
    }
    return A;
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
