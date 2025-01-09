## multithread_pool.h

The `multithread_pool.h` file provides a template class for creating a pool of objects that can be shared by tasks in multiple threads. This is useful for objects that are expensive to allocate or have allocated, allowing them to be shared among multiple threads/tasks instead of having one pool per thread.

### Example Usage: Creating and Using a Multithread Pool

```cpp
#include "multithread_pool.h"
#include <iostream>
#include <vector>
#include <memory>
#include <thread>

class ExpensiveObject 
{
public:
    ExpensiveObject() { std::cout << "ExpensiveObject created" << std::endl; }
    ~ExpensiveObject() { std::cout << "ExpensiveObject destroyed" << std::endl; }
    void do_work() { std::cout << "ExpensiveObject is working" << std::endl; }
};

void worker(ctle::multithread_pool<ExpensiveObject>& pool) 
{
    auto obj = pool.borrow();
    if(obj) 
	{
        (*obj)->do_work();
        pool.return_object(*obj);
    } 
	else 
	{
        std::cout << "No available objects in the pool" << std::endl;
    }
}

int main() 
{
	// make 5 expensive objects
    std::vector<std::unique_ptr<ExpensiveObject>> objects;
    for (int i = 0; i < 5; ++i) 
	{
        objects.push_back(std::make_unique<ExpensiveObject>());
    }

	// create a pool out of the expensive objects
    ctle::multithread_pool<ExpensiveObject> pool;
    pool.initialize(objects);

	// run a number of workers which need to use an object from the pool
    std::thread t1(worker, std::ref(pool));
    std::thread t2(worker, std::ref(pool));
    std::thread t3(worker, std::ref(pool));

    t1.join();
    t2.join();
    t3.join();

    std::vector<std::unique_ptr<ExpensiveObject>> returned_objects;
    bool all_returned = pool.deinitialize(returned_objects);

    std::cout << "All objects has been returned: " << all_returned << std::endl;

    return 0;
}
```