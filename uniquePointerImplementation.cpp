#include <iostream>
using namespace std;

template <typename T>

class uniqueptr
{

private:
    T *res; // --> it will have the same size as resource pointer , no increase in size as compared to raw pointers ,

public:
    uniqueptr(T *a = nullptr) : res(a)
    {
        cout << "constructor called" << endl;
    }

    uniqueptr(const uniqueptr<T> &ptr) = delete; // deleted copy constructor ref2

    uniqueptr &operator=(const uniqueptr<T> &ptr) = delete; // deleted copy assignemnet. ref1

    uniqueptr(uniqueptr<T> &&ptr)
    { //&& means r-value for move.  //move copy constrctor
        res = ptr.res;
        // we need to remove ptr res;

        ptr.res = nullptr;
    }

    uniqueptr &operator=(uniqueptr<T> &&ptr)
    { //&& means r-value for move. //move saassignement operator
        if (this != &ptr)
        {
            if (res)
            {
                delete res;
            }
            res = ptr.res;
            ptr.res = nullptr;
        }

        return *this;
    }

    T *operator->()
    {
        return res;
    }

    T &operator*()
    {
        return *res;
    }

    T *get()
    {
        return res;
    }

    void reset(T *newres = nullptr)
    {
        if (res)
        {
            delete res;
        }
        res = newres;
    }

    ~uniqueptr()
    {

        cout << "destrutor called \n";
        if (res)
        {
            delete res;
            res = nullptr;
        }
    }
};

int main()
{

    uniqueptr<int> ptr1(new int(2)); // this should make ptr1 a pointer to a the memory space returned by new int(2).  , just like int * ptr1 = new int(2);

    // uniqueptr<int> ptr2(ptr1);
    // unique pointer means that there should be unique and exclusive ownership
    // so when we assign ptr1 to ptr2 ,
    // this means that ptr2 will now be pointing to the memory location that was earlier pointed to by ptr1 ,
    // and ptr1 will now not point to that location
    // so this should be illegal and now it should give compilation error

    // uniqueptr<int> ptr3 = ptr1;  //compilation error.      // copy constructor ref2

    uniqueptr<int> ptr5(new int(500));
    // ptr4 = ptr3; //compilation error.  // copy assignment operator , not the same thing as previous one

    // to transfer ownership
    // uniqueptr<int> ptr5 = std::move(ptr1);
    ptr5 = std::move(ptr1); // move copy constructor or move copy assignment ref1

    // ptr1->func();

    cout << *(ptr5) << endl; // we have to implement this star also , should wiork liek a normal pointer;

    cout << ptr5.get() << endl; // get address of pointer

    ptr1.reset(new int(10)); // this si a functionality given by smart pointers that if I wan to reset it to a new memory blockl;

    cout << *(ptr1) << endl;

    // we have to implement destructor also;//to free up the resource;

    return 0;
}
