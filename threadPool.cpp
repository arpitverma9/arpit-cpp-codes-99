#include <iostream>
#include <thread>
#include <memory>
#include <condition_variable>
#include <queue>
#include <mutex>
#include <functional>
#include <vector>
using namespace std;

/*
//target -> create a thread pool with n threads ,
when you get a new task , you pick up a thread from thread pool to do the work


*/

class ThreadPool
{
private:
    queue<function<void()>> tasks;
    mutex m;
    condition_variable cv;
    vector<thread> workers;
    bool stop = false;

private:
    void workerLoop()
    {
        while (1)
        {
            unique_lock<mutex> lock(m);

            cv.wait(lock, [&]
                    {
                        return stop || !tasks.empty(); // this means that cv will wait until stop becomes true
                        // tasks.empty() -> means task queue is empty . !tasks.empty() , means there is some task to do , im which case cv will proceed
                    /*
                        Worker sleeps until:
                        shutdown requested
                        OR
                        work available
                    */ });

            if (stop && tasks.empty())
                break;

            auto task = std::move(tasks.front());
            tasks.pop();

            lock.unlock();
            cout << endl;
            task();
        }
    }

public:
    ThreadPool(int numThreads)
    {
        for (int i = 0; i < numThreads; i++)
        {
            std::thread t([this]
                          { workerLoop(); });
            workers.push_back(std::move(t));
        }
    }

    ~ThreadPool()
    {
        {
            lock_guard<mutex> lock(m);
            stop = true;
        }
        cv.notify_all();
        for (thread &t : workers)
        {
            if (t.joinable())
                t.join();
        }
    }

    void addTask(function<void()> task)
    {
        {
            lock_guard<mutex> lock(m); // acquire a lock over m

            if (stop)
            {
                /* code */
                throw runtime_error("Thread pool is stopped");
            }

            tasks.push(std::move(task));
        }
        cv.notify_one();
    }
};
int main()
{
    ThreadPool tp(3);

    tp.addTask([]()
               { cout << "this is a task"; });

    for (int i = 1; i <= 10; i++)
    {
        tp.addTask([i]()
                   {
        cout << "Task " << i
             << " started by "
             << this_thread::get_id()
             << endl;

        this_thread::sleep_for(
            chrono::milliseconds(500));

        cout << "Task " << i
             << " finished by "
             << this_thread::get_id()
             << endl; });
    }

    cout << "main ends here" << endl;

    return 0;
}