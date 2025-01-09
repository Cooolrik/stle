## readers_writer_lock

The `readers_writer_lock` class is a lock for concurrent read and exclusive write operations. It allows multiple threads to read concurrently while ensuring exclusive access for write operations.

### Guard Classes

- `class read_guard`
- `class write_guard`

### Examples

#### Example 1: Basic Usage

```cpp
#include "readers_writer_lock.h"
#include <iostream>
#include <thread>
#include <vector>

void reader(ctle::readers_writer_lock &lock, int id)
{
    lock.read_lock();
    std::cout << "Reader " << id << " is reading." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    lock.read_unlock();
}

void writer(ctle::readers_writer_lock &lock, int id)
{
    lock.write_lock();
    std::cout << "Writer " << id << " is writing." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    lock.write_unlock();
}

int main()
{
    ctle::readers_writer_lock lock;
    std::vector<std::thread> threads;

    // Create reader threads
    for (int i = 0; i < 5; ++i)
    {
        threads.emplace_back(reader, std::ref(lock), i);
    }

    // Create writer threads
    for (int i = 0; i < 2; ++i)
    {
        threads.emplace_back(writer, std::ref(lock), i);
    }

    // Join all threads
    for (auto &t : threads)
    {
        t.join();
    }

    return 0;
}
```

#### Example 2: Using Guard Classes

```cpp
#include "readers_writer_lock.h"
#include <iostream>
#include <thread>
#include <vector>

void reader(ctle::readers_writer_lock &lock, int id)
{
    ctle::readers_writer_lock::read_guard guard(lock);
    std::cout << "Reader " << id << " is reading." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void writer(ctle::readers_writer_lock &lock, int id)
{
    ctle::readers_writer_lock::write_guard guard(lock);
    std::cout << "Writer " << id << " is writing." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main()
{
    ctle::readers_writer_lock lock;
    std::vector<std::thread> threads;

    // Create reader threads
    for (int i = 0; i < 5; ++i)
    {
        threads.emplace_back(reader, std::ref(lock), i);
    }

    // Create writer threads
    for (int i = 0; i < 2; ++i)
    {
        threads.emplace_back(writer, std::ref(lock), i);
    }

    // Join all threads
    for (auto &t : threads)
    {
        t.join();
    }

    return 0;
}
```