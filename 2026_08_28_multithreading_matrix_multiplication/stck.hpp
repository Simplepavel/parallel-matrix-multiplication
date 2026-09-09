// Потокобезопасная версия стека
#ifndef TS_STACK
#define TS_STACK
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

template <typename T>
class ts_stck // thread safe stack
{
    T *data;
    unsigned int size;
    mutable std::mutex mtx;
    std::condition_variable cond;
    static unsigned int optimal;
    bool flag; // конец работы
    unsigned int cap;

public:
    ts_stck(unsigned int capacity) : size(0), flag(false), cap(capacity)
    {
        data = new T[capacity];
    }
    ~ts_stck()
    {
        std::lock_guard<std::mutex> lock(mtx);
        delete[] data;
    }
    void push(const T &argv)
    {
        std::lock_guard<std::mutex> lock(mtx);
        data[size++] = argv;
        if (size > optimal)
        {
            cond.notify_all();
        }
    }

    bool pop_or_wait(T &result)
    {
        std::unique_lock<std::mutex> lock_mtx(mtx);
        cond.wait(lock_mtx, [this]()
                  { return flag || size != 0; });
        if (size > 0)
        {
            result = data[--size];
            return true;
        }
        return false;
    }

    bool empty()
    {
        std::lock_guard<std::mutex> lock_mtx(mtx);
        return size == 0;
    }

    void end()
    {
        std::lock_guard<std::mutex> lock_mtx(mtx);
        flag = true;
        cond.notify_all();
    }
};

template <typename T>
unsigned int ts_stck<T>::optimal = std::thread::hardware_concurrency();

#endif

#ifndef STACK
#define STACK

template <typename T>
struct stck // simple stck
{
    T *data;
    unsigned int size;

    stck(unsigned int capacity) : data(nullptr), size(0)
    {
        data = new T[capacity];
    }
    ~stck()
    {
        delete[] data;
    }
    void push(const T &value)
    {
        data[size++] = value;
    }
    T &top()
    {
        if (size == 0)
        {
            throw "nothing on top\n";
        }
        return data[size - 1];
    }
    void pop()
    {
        if (size == 0)
        {
            throw "nothing to pop\n";
        }
        --size;
    }
    bool empty() { return size == 0; }
};
#endif
