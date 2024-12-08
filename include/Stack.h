#pragma once
#include <iostream>

using namespace std;

template<typename T> class Vector
{
protected:

    size_t sz;
    size_t capacity;
    T* data;

public:

    // Конструкторы
    Vector(size_t size = 0) : sz(size), capacity(sz * 2 + 2)
    {
        data = new T[capacity];
        if (data == nullptr)
            throw ("Error");
    }

    Vector(T* arr, size_t s) : sz(s), capacity(sz * 2 + 2)
    {
        data = new T[capacity];
        if (data == nullptr)
            throw ("Error");
        else copy(arr, arr + sz, data);
    }

    Vector(const Vector& t) : sz(t.sz), capacity(t.capacity)
    {
        data = new T[capacity];
        if (data == nullptr)
            throw ("Error");
        else copy(t.data, t.data + t.sz, data);
    }

    Vector(const Vector&& t) noexcept
    {
        data = nullptr;
        swap(*this, t);
    }

    // Деструктор
    ~Vector()
    {
        delete[] data;
        data = nullptr;
    }

    // operator =
    Vector& operator=(const Vector& v)
    {
        if (&v == this)
            return *this;

        if (sz != v.sz)
        {
            T* p = new T[v.capacity];
            if (p == nullptr)
                throw ("Error");
            else {
                delete[] data; data = nullptr;
                data = p;
                sz = v.sz;
                capacity = v.capacity;
                p = nullptr;
            }
        }
        copy(v.data, v.data + v.sz, data);
        return *this;
    }

    Vector& operator=(const Vector&& v) noexcept
    {
        delete[] data;
        data = nullptr;
        swap(*this, v);
        return *this;
    }

    friend void swap(Vector& lhs, Vector& rhs)
    {
        swap(lhs.sz, rhs.sz);
        swap(lhs.capacity, rhs.capacity);
        swap(lhs.data, rhs.data);
    }

    size_t size_() const noexcept { return sz; };
    size_t capacity_() const noexcept { return capacity; };
    bool isEmpty() const noexcept { return (sz == 0); };
    bool isFull() const noexcept { return (sz == capacity); };

    // сравнение
    bool operator==(const Vector& t) const noexcept
    {
        if (sz != t.sz)
            return 0;
        else
            for (size_t i = 0; i < sz; i++)
                if (data[i] != t.data[i])
                    return 0;
        return 1;
    }

    bool operator!=(const Vector& t) const noexcept
    {
        return (!(*this == t));
    }

    // индексация
    T operator[](size_t ind)
    {
        return data[ind];
    }

    const T operator[](size_t ind) const
    {
        return data[ind];
    }


    T back()
    {
        if (!(this->isEmpty()))
            return data[sz - 1];
        else
            throw ("Vector is empty");
    };

    void resize(int new_size)
    {
        T* mem = new T[new_size * 2 + 2];
        if (mem != nullptr)
        {
            copy(data, data + capacity, mem);
            delete[] data;
            capacity = new_size * 2 + 2;
            data = mem;
            mem = nullptr;
        }
    }

    void push_back(T elem)
    {
        insert(elem, sz);
    }

    void push_front(T elem)
    {
        insert(elem, 0);
    }

    void pop_back()
    {
        erase(sz);
    }

    void pop_front()
    {
        erase(0);
    }

    void insert(T elem, size_t ind)
    {
        if ((this->isFull()))
            this->resize(size_t(size_t((int)sz + 1)));
        for (size_t i = sz; i > ind; i--)
            data[i] = data[i - 1];
        data[ind] = elem; sz++;
    }

    void erase(size_t ind)
    {
        if (!(this->isEmpty()))
        {
            for (size_t i = ind; i < sz; i++)
                data[i] = data[i + 1];
            sz--;
        }
        else throw ("Vector is empty");
    }
};




template<typename T> class Stack : private Vector<T>
{
    using Vector<T>::sz;
    using Vector<T>::capacity;
    using Vector<T>::data;
    using Vector<T>::pop_back;
    using Vector<T>::push_back;
    using Vector<T>::back;

public:

    // конструктор
    Stack() : Vector<T>(0) {}

    // Наследуем
    using Vector<T>::operator[];
    using Vector<T>::isFull;
    using Vector<T>::isEmpty;
    using Vector<T>::size_;
    using Vector<T>::capacity_;
    using Vector<T>::resize;


    void push(T elem)
    {
        this->push_back(elem);
    }
    void pop()
    {
        this->pop_back();
    }
    T top()
    {
        return this->back();
    }
};




template<typename T> class Queue : private Vector<T>
{
    using Vector<T>::sz;
    using Vector<T>::capacity;
    using Vector<T>::data;
    size_t front, back;
public:

    Queue() : Vector<T>(0), front(0), back(0) {}

    using Vector<T>::isFull;
    using Vector<T>::isEmpty;
    using Vector<T>::size;
    using Vector<T>::capacity_;
    using Vector<T>::resize;

    void push(T elem)
    {
        if (!this->isFull())
            this->resize(size_t(size_t((int)sz + 1)));
        data[back] = elem;
        sz++;
        if (back == capacity - 1) back = 0;
        else back++;
    }

    void pop()
    {
        if (!this->isEmpty())
        {
            if (front == capacity - 1) front = 0;
            else front++;
            sz--;
        }
    }

    T get_front()
    {
        return data[front];
    }

    T get_back()
    {
        if (back == 0)
            return data[capacity - 1];
        else
            return data[back - 1];
    }

    T get_back_ind()
    {
        return back;
    }
};



template<typename T> class Queue2Stacks : private Vector<Stack<T>>
{
private:

    using::Vector<Stack<T>>::data;
    size_t sz, capacity;

public:

    Queue2Stacks() : sz(data[0].size() + data[1].size()), capacity(data[0].capacity_() + data[1].capacity_()) {}

    size_t size_() const noexcept { return data[0].size() + data[1].size(); };
    size_t capacity_() const noexcept { return data[0].capacity_() + data[1].capacity_(); };
    bool isEmpty() const noexcept { return (data[0].isEmpty() && data[1].isEmpty()); };
    bool isFull() const noexcept { return (data[0].isFull() && data[1].isFull()); };

    void push(T elem)
    {
        data[0].push(elem);
        sz++;
    }

    void pop()
    {
        if (data[1].isEmpty())
        {
            while (!data[0].isEmpty())
            {
                T mem = data[0].top();
                data[1].push(mem);
                data[0].pop();
            }
        }
        data[1].pop();
        sz--;
    }

    T back()
    {
        if (!data[1].isEmpty())
            return data[1][0];
        if (!data[0].isEmpty())
            return data[0][sz];
        else return 0;
    }

    T front()
    {
        if (!data[1].isEmpty())
            return data[1][sz];
        if (!data[0].isEmpty())
            return data[0][0];
        else return 0;
    }
};