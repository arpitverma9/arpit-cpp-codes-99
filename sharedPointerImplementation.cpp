//this is currently not working

#include <iostream>
using namespace std;

template <typename T>

class sharedptr
{
private:
    T *res;
    int *counter; // this needs to be on heap because when we get a new object , then it needs to also point to the same pointer.
    // we cannot make it static because static is one variable for whole code , so all objects have 1 counter this is not what we want .  so this is per class
    // pointer on heap will be per resource and shared as we want
    void incrementcounter()
    {
        if (counter)
        {
            (*counter)++;
        }
    }
    void decrementcounter()
    {
        if (counter)
        {
            (*counter)--;
            if ((*counter) == 0)
            {
                if (res)
                {
                    delete res;
                    delete counter;
                    res = nullptr;
                }
            }
        }
    }

public:
    sharedptr(T *ptr = nullptr) : res(ptr), counter(new int(1))
    {
        cout << "constructor is called\n";
    } // this is normal and parametrised constructor for 1st 2 case

    sharedptr<T>(const sharedptr<T> &ptr)
    { // this is copy constructor
        res = ptr.res;
        counter = ptr.counter;
        incrementcounter();
    }

    sharedptr<T> &operator=(const sharedptr<T> &ptr)
    { // this is copy assignment operator
        if (this != &ptr)
        {
            decrementcounter();
            res = ptr.res;
            counter = ptr->counter;
            incrementcounter();
        }
        return *this;
    }

    // move copy contructor
    sharedptr<T>(sharedptr<T> &&ptr)
    { // did not understand this ????
        res = ptr.res;
        counter = ptr.counter;
        ptr.res = nullptr;
        ptr.counter = nullptr;
        // incrementcounter();
    }

    // move copy assignment
    sharedptr<T> &operator=(sharedptr<T> &&ptr)
    {
        if (this != &ptr)
        {
            decrementcounter();
            res = ptr->res;
            counter = ptr->counter;
            // incrementcounter();  not required

            ptr.res = nullptr;
            ptr.counter = nullptr;
        }
        return *this;
    }

    void reset(T *ptr = nullptr)
    {
        decrementcounter(); // is this required?
        res = ptr;
        counter = new int(1);
    }

    int get_count()
    {
        if (counter)
        {
            return *(counter);
        }
        return -1;
    }

    T *operator->()
    {
        return res;
    }

    T &operator*()
    {
        return (*res);
    }

    T *get()
    {
        return res;
    }

    ~sharedptr()
    {
        decrementcounter();
    }
};

int main()
{
    sharedptr<int> ptr1;                  // default ctor
    sharedptr<int> ptr2(new int(10));     // parameterised constructor
    sharedptr<int> ptr3(ptr2);            // copy constructor
    ptr3 = ptr2;                          // copy assignment operator
    sharedptr<int> ptr4(std::move(ptr1)); // move copy constructor
    ptr2 = std::move(ptr3);               // move copy assignment operator

    ptr1.reset(); // set to null
    ptr1.reset(new int(5));

    cout << *ptr1; // value it is pointing to;  /implementibng * operator
    // ptr1->func(); // implementing -> operator
    ptr1.get();       // get raw pointer
    ptr1.get_count(); // numer of objects pointing to the same resource

    // destructor
}
