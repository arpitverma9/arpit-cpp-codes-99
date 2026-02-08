#include <thread>
#include <iostream>
using std::cout;
using std::endl;

void test(int x)
{
    std::cout << " hello from thread " << endl;
    std::cout << " argument passed in " << x << endl;
}

int main()
{

    std::thread myThread(&test, 100);
    myThread.join();
    std::cout << "hello from my main thread " << std::endl;

    return 0;
}
