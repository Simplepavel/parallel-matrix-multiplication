#include <vector>
#include <iostream>
#include <memory>
#include <stack>
#include <cstring>

enum letters : char
{
    NONE = -1,
    V2,
    V1,
    H2,
    H1,
    D2,
    D1,
    D
};

struct bound
{
    unsigned int start;
    unsigned int n;
    unsigned int m;
    bound() : start(0), n(0), m(0) {}
    bound(unsigned int _start, unsigned int _n, unsigned int _m) : start(_start), n(_n), m(_m) {}
};

struct rect
{
    unsigned int row_start;
    unsigned int column_start;
    unsigned int row_end;
    unsigned int column_end;
    rect(const unsigned int &_row_start, const unsigned int &_row_end, const unsigned int &_column_start, const unsigned int &_column_end)
    {
        if (_row_start > _row_end || _column_start > _column_end)
        {
            throw "invalid argument(rect)";
        }
        row_start = _row_start;
        column_start = _column_start;

        row_end = _row_end;
        column_end = _column_end;
    }

    unsigned int n() const { return row_end - row_start; }
    unsigned int m() const { return column_end - column_start; }
};

template <typename T>
struct memory
{
    T *m;
    unsigned int ptr = 0;
    unsigned int length = 0;
};

template <typename T>
class matrix
{
    T *_data;
    bound _bound;

public:
    void set_data(T *data)
    {
        _data = data;
    };
    const T *get_data() { return _data; }
    void set_bound(const bound &new_bound)
    {
        _bound = new_bound;
    }
    const bound &get_bound() { return _bound; }
    void show(std::ostream &cout) const
    {
        unsigned int end = _bound.start + _bound.n * _bound.m;
        for (unsigned int i = _bound.start; i < end; ++i)
        {
            cout << _data[i] << ' ';
            if ((i - _bound.start) % _bound.m == _bound.m - 1)
                cout << '\n';
        }
    }
    T &set(unsigned int n, unsigned int m)
    {
        if (n > _bound.n || n < 1 || m > _bound.m || m < 1)
        {
            throw "out of bound";
        }
        return _data[_bound.start + (n - 1) * _bound.m + (m - 1)];
    }
    const T &get(unsigned int n, unsigned int m) const
    {
        if (n > _bound.n || n < 1 || m > _bound.m || m < 1)
        {
            throw "out of bound";
        }
        return _data[_bound.start + (n - 1) * _bound.m + (m - 1)];
    }
    unsigned int n() const { return _bound.n; }
    unsigned int m() const { return _bound.m; }
    unsigned int count() const { return _bound.n * _bound.m; }
    void insert(const matrix<T> &argv, letters letter)
    {
        if (n() != argv.n() * 2)
        {
            throw "size mismatch";
        }
        switch (letter)
        {
        case (letters::D):
            plus(rect(0, n() / 2, 0, n() / 2), argv);
            plus(rect(n() / 2, n(), n() / 2, n()), argv);
            break;
        case (letters::D1):
            plus(rect(0, n() / 2, 0, n() / 2), argv);
            break;
        case (letters::D2):
            plus(rect(n() / 2, n(), n() / 2, n()), argv);
            break;
        case (letters::H1):
            minus(rect(0, n() / 2, 0, n() / 2), argv);
            plus(rect(0, n() / 2, n() / 2, n()), argv);
            break;
        case (letters::H2):
            plus(rect(n() / 2, n(), 0, n() / 2), argv);
            minus(rect(n() / 2, n(), n() / 2, n()), argv);
            break;
        case (letters::V1):
            plus(rect(n() / 2, n(), 0, n() / 2), argv);
            plus(rect(0, n() / 2, 0, n() / 2), argv);
            break;
        case (letters::V2):
            plus(rect(0, n() / 2, n() / 2, n()), argv);
            plus(rect(n() / 2, n(), n() / 2, n()), argv);
            break;
        default:
            break;
        }
    }
    void plus(const rect &b, const matrix<T> &argv)
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
    void minus(const rect &b, const matrix<T> &argv)
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

template <typename T>
struct task
{
    matrix<T> argv1;
    matrix<T> argv2;
    letters letter;
};

template <typename T>
matrix<T> copy(matrix<T> &source, letters letter, std::vector<memory<T>> &letter_buffers)
{
    matrix<T> ans;
    unsigned int n = source.n();
    ans.set_data(letter_buffers[letter].m);
    ans.set_bound(bound(letter_buffers[letter].ptr, n, n));
    letter_buffers[letter].ptr += n * n;
    for (unsigned int i = 0; i < n; ++i)
    {
        for (unsigned int j = 0; j < n; ++j)
        {
            ans.set(i + 1, j + 1) = source.get(i + 1, j + 1);
            source.set(i + 1, j + 1) = 0;
        }
    }
    return ans;
}

/*
разделяет данные на блоки и суммирует их
*/
template <typename T>
std::vector<matrix<T>> split_and_summ(const matrix<T> &argv1, const matrix<T> &argv2, memory<T> &operands, memory<T> &buffer)
{

    unsigned int N = argv1.n();
    unsigned int n = N / 2;
    std::vector<matrix<T>> matrix_summs(14);
    for (unsigned int i = 0; i < 14; ++i)
    {
        matrix_summs[i].set_data(operands.m);
        matrix_summs[i].set_bound(bound(operands.ptr, n, n));
        operands.ptr += n * n;
    }

    matrix<T> copy_argv1;
    copy_argv1.set_data(buffer.m);
    copy_argv1.set_bound(bound(buffer.ptr, N, N));
    buffer.ptr += N * N;

    matrix<T> copy_argv2;
    copy_argv2.set_data(buffer.m);
    copy_argv2.set_bound(bound(buffer.ptr, N, N));
    buffer.ptr += N * N;

    for (unsigned int i = 0; i < N; ++i)
    {
        for (unsigned int j = 0; j < N; ++j)
        {
            copy_argv1.set(i + 1, j + 1) = argv1.get(i + 1, j + 1);
            copy_argv2.set(i + 1, j + 1) = argv2.get(i + 1, j + 1);
        }
    }

    T q1 = 0;
    T q2 = 0;
    for (unsigned int i = 0; i < n; ++i)
    {
        for (unsigned int j = 0; j < n; ++j)
        {

            // A11 + A22
            q1 = copy_argv1.get(i + 1, j + 1);
            q2 = copy_argv1.get(n + i + 1, n + j + 1);
            matrix_summs[0].set(i + 1, j + 1) = q1 + q2;

            // B11 + B22
            q1 = copy_argv2.get(i + 1, j + 1);
            q2 = copy_argv2.get(n + i + 1, n + j + 1);
            matrix_summs[1].set(i + 1, j + 1) = q1 + q2;

            // A12 - A22
            q1 = copy_argv1.get(i + 1, n + j + 1);
            q2 = copy_argv1.get(n + i + 1, n + j + 1);
            matrix_summs[2].set(i + 1, j + 1) = q1 - q2;

            // B21 + B22
            q1 = copy_argv2.get(n + i + 1, j + 1);
            q2 = copy_argv2.get(n + i + 1, n + j + 1);
            matrix_summs[3].set(i + 1, j + 1) = q1 + q2;

            // A21 - A11
            q1 = copy_argv1.get(n + i + 1, j + 1);
            q2 = copy_argv1.get(i + 1, j + 1);
            matrix_summs[4].set(i + 1, j + 1) = q1 - q2;

            // B11 + B12
            q1 = copy_argv2.get(i + 1, j + 1);
            q2 = copy_argv2.get(i + 1, n + j + 1);
            matrix_summs[5].set(i + 1, j + 1) = q1 + q2;

            // A11 + A12
            q1 = copy_argv1.get(i + 1, j + 1);
            q2 = copy_argv1.get(i + 1, n + j + 1);
            matrix_summs[6].set(i + 1, j + 1) = q1 + q2;

            // B22 + 0;
            q1 = copy_argv2.get(n + i + 1, n + j + 1);
            matrix_summs[7].set(i + 1, j + 1) = q1;

            // A21 + A22
            q1 = copy_argv1.get(n + i + 1, j + 1);
            q2 = copy_argv1.get(n + i + 1, n + j + 1);
            matrix_summs[8].set(i + 1, j + 1) = q1 + q2;

            // B11 + 0;
            q1 = copy_argv2.get(i + 1, j + 1);
            matrix_summs[9].set(i + 1, j + 1) = q1;

            // A22 + 0
            q1 = copy_argv1.get(n + i + 1, n + j + 1);
            matrix_summs[10].set(i + 1, j + 1) = q1;

            // B21 - B11
            q1 = copy_argv2.get(n + i + 1, j + 1);
            q2 = copy_argv2.get(i + 1, j + 1);
            matrix_summs[11].set(i + 1, j + 1) = q1 - q2;

            // A11
            q1 = copy_argv1.get(i + 1, j + 1);
            matrix_summs[12].set(i + 1, j + 1) = q1;

            // B12 - B22
            q1 = copy_argv2.get(i + 1, n + j + 1);
            q2 = copy_argv2.get(n + i + 1, n + j + 1);
            matrix_summs[13].set(i + 1, j + 1) = q1 - q2;
        }
    }
    buffer.ptr = 0;
    std::memset(buffer.m, 0, buffer.length);
    return matrix_summs;
}

template <typename T>
/*
для понятности буду использовать только n() вызов, т.к n = m по предположению
*/
matrix<T> strassen(const matrix<T> &argv1, const matrix<T> &argv2, T *output)
{
    if (argv1.n() != argv2.n())
    {
        throw "size mismatch";
    }
    unsigned int n = argv1.n();
    const unsigned int s = 10000;

    memory<T> operands;
    operands.length = 4 * n * n;
    operands.m = new T[operands.length]{0};

    memory<T> buffer;
    buffer.length = 2 * n * n;
    buffer.m = new T[buffer.length]{0};

    std::vector<memory<T>> letter_buffers(7);
    unsigned int letter_buffer_length = (n * n) / 3 + 1;
    for (int i = 0; i < 7; ++i)
    {
        letter_buffers[i].length = letter_buffer_length;
        letter_buffers[i].m = new T[letter_buffer_length]{0};
    }

    std::stack<task<T>> ttt;
    std::stack<matrix<T>> rrr;
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
                matrix<T> A;
                A.set_data(buffer.m);
                A.set_bound(bound(buffer.ptr, 2 * n, 2 * n));
                for (auto i : {D, D1, D2, H1, H2, V1, V2})
                {
                    A.insert(rrr.top(), i);
                    letter_buffers[i].ptr -= n * n;
                    rrr.pop();
                }
                counter.pop();
                letters l = (letters)(counter.top());
                buffer.ptr = 0;
                rrr.push(copy(A, l, letter_buffers));
                counter.top() += 1;
            }
            break;
        }

        task<T> t = ttt.top();
        unsigned int current_n = t.argv1.n();
        ttt.pop();
        if (operands.ptr > 0)
        {
            operands.ptr -= 2 * current_n * current_n;
        }
        if (t.argv1.count() < s && t.argv2.count() < s)
        {
            matrix<T> result;
            result.set_data(letter_buffers[t.letter].m);
            result.set_bound(bound(letter_buffers[t.letter].ptr, current_n, current_n));
            letter_buffers[t.letter].ptr += current_n * current_n;
            for (unsigned int i = 0; i < current_n; ++i)
            {
                for (unsigned int j = 0; j < current_n; ++j)
                {
                    T summ = 0;
                    for (unsigned int k = 0; k < current_n; ++k)
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
                matrix<T> A;
                A.set_data(buffer.m);
                A.set_bound(bound(buffer.ptr, 2 * n, 2 * n));
                for (auto i : {D, D1, D2, H1, H2, V1, V2})
                {
                    A.insert(rrr.top(), i);
                    letter_buffers[i].ptr -= n * n;
                    rrr.pop();
                }
                counter.pop();
                letters l = (letters)(counter.top());
                buffer.ptr = 0;
                rrr.push(copy(A, l, letter_buffers));
                counter.top() += 1;
            }

            counter.push(0);
            std::vector<matrix<T>> matrix_sums = split_and_summ(t.argv1, t.argv2, operands, buffer);
            letters l[7]{D, D1, D2, H1, H2, V1, V2};
            for (unsigned int i = 0; i < 13; i += 2)
            {
                task<T> ti;
                ti.argv1 = matrix_sums[i];
                ti.argv2 = matrix_sums[i + 1];
                ti.letter = l[i / 2];
                ttt.push(ti);
            }
        }
    }

    delete[] operands.m;
    delete[] buffer.m;

    n = rrr.top().n();
    matrix<T> A;
    A.set_data(output);
    A.set_bound(bound(0, 2 * n, 2 * n));
    for (auto i : {D, D1, D2, H1, H2, V1, V2})
    {
        A.insert(rrr.top(), i);
        rrr.pop();
    }
    for (auto i : letter_buffers)
    {
        delete[] i.m;
    }
    return A;
}
