#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <assert.h>
#include "config.h"
#include "myassert.h"

#define SLASHES std::cout << "-----------------------------------------\n";
#define EMPT    std::cout << "\n";
//!-------------------------------
void print_bits(unsigned short value);
//!-------------------------------
template<typename T> class Stack_t
{
    private:
        T* data_;
        size_t stack_size_;
        size_t current_;
        Stack_t();
    protected:
        inline bool stack_ok() const;
        inline void dump() const;
        void reallocateStack(size_t newSize);
    public:
        Stack_t(size_t theSize, T initializer);
        Stack_t(const Stack_t<T>& classCopy);
        virtual ~Stack_t();
        //!-----------------------
        void operator = (const Stack_t<T>& classCopy);
        //!-----USER-FUNCTIONS----
        void push(const T& element);
        T pop();
        T top() const;
        void print() const;
};
//!-------------------------------
class BoolEcoStack_t: public Stack_t<unsigned short>
{
    private:
        int Cbit_counter;
    public:
        BoolEcoStack_t(size_t theSize, unsigned short initializer): Stack_t<unsigned short>(theSize, initializer), Cbit_counter(0) {}
        ~BoolEcoStack_t() {}
        void push_bool(bool element);
        bool pop_bool();
};
//!-------------------------------
template<typename T> Stack_t<T>::Stack_t(size_t theSize, T initializer): stack_size_(theSize), data_(new T[theSize]), current_(0)
{
    for (size_t i = 0; i < stack_size_; i++)
    {
        assert(0 <= i && i < stack_size_);
        data_[i] = initializer;
    }

    ASSERT_STK(stack_ok());
}
//!-------------------------------
template<typename T> Stack_t<T>::Stack_t(const Stack_t<T>& classCopy)
{
    this->current_ = classCopy.current_;
    this->stack_size_ = classCopy.stack_size_;
    this->data_ = new T[classCopy.stack_size_];

    for (int i = 0; i < classCopy.stack_size_; i++)
        this->data_[i] = classCopy.data_[i];
}
//!-------------------------------
template<typename T> Stack_t<T>::~Stack_t()
{
    delete[] data_;
}
//!-------------------------------
template<typename T> inline bool Stack_t<T>::stack_ok() const
{
    if (!data_)
        return false;
    if (stack_size_ < 0)
        return false;
    if (current_ >= stack_size_)
        return false;

    return true;
}
//!--------------------------------
template<typename T> inline void Stack_t<T>::dump() const
{
    ASSERT_STK(stack_ok());

    EMPT SLASHES std::cout << "\t\tSTACK DUMP\n"; SLASHES
                 std::cout << "Stack size = " << stack_size_ << "\ncnt = " << current_ << "\n"; SLASHES

    for (size_t i = 0; i < stack_size_; i++)
    {
        if (i < current_)
             std::cout << "->";
        else std::cout << "  ";

        std::cout << "data[" << i <<"] = " << data_[i] << "\n";
    }

    ASSERT_STK(stack_ok());
}
//!--------------------------------
template<typename T> void Stack_t<T>::push(const T& element)
{
    ASSERT_STK(stack_ok());

    if (current_ == stack_size_ - 1)
        reallocateStack(stack_size_ * 2);

    data_[current_] = element;
    current_++;

    ASSERT_STK(stack_ok());
}
//!--------------------------------
template<typename T> T Stack_t<T>::pop()
{
    ASSERT_STK(stack_ok());
    return data_[--current_];
}
//!--------------------------------
template<typename T> void Stack_t<T>::reallocateStack(size_t newSize)
{
    ASSERT_STK(stack_ok());

    T* newStack = new T[newSize];
    std::copy(data_, data_ + std::min(stack_size_, newSize), newStack);
    delete[] data_;
    data_ = newStack;
    stack_size_ = newSize;

    ASSERT_STK(stack_ok());
}

template<typename T> inline void Stack_t<T>::print() const
{
    ASSERT_STK(stack_ok());

    EMPT SLASHES
    std::cout << "STACK:\n";
    std::cout << "SIZE: " << current_ << "\n";
    for (size_t i = 0; i < current_; i++)
    {
        assert(0 <= i && i < current_);
        std::cout << i << ": " << data_[i] << "\n";
    }

    ASSERT_STK(stack_ok());
}
//!--------------------------------
template<typename T> inline T Stack_t<T>::top() const
{
    ASSERT_STK(stack_ok());
    return data_[current_ - 1];
}
//!--------------------------------
//!---------ECOSTACK-FUNCS---------
//!--------------------------------
void BoolEcoStack_t::push_bool(bool element)
{
    ASSERT_STK(stack_ok());
    if (Cbit_counter >= 16)
        Cbit_counter = 0;

    unsigned short work = 0;
    if (0 < Cbit_counter && Cbit_counter < 16)
        work = Stack_t<unsigned short>::pop();

    work = work << 1;
    if (element)
        work = work | 1;

    Cbit_counter++;

    Stack_t<unsigned short>::push(work);
    ASSERT_STK(stack_ok());
}
//!--------------------------------
bool BoolEcoStack_t::pop_bool()
{
    bool result = false;
    if (!Cbit_counter)
    {
        Cbit_counter = 16;
        Stack_t<unsigned short>::pop();
    }
    else
        Cbit_counter--;

    unsigned short before_pop = Stack_t<unsigned short>::pop();
    unsigned short work = before_pop | 1;

    if (work == before_pop)
        result = true;

    work = work >> 1;
    Stack_t<unsigned short>::push(work);
    return result;
}
//!--------------------------------
int main()
{
    BoolEcoStack_t boolStack(1, 0);
    return 0;
}
