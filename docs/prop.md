## Property Templates

The `prop_[...]` template classes provide a convenient way to implement properties in C++ classes, similar to properties in C# or Python. Properties can be:
- Read-only, write-only, or read/write
- Direct value storage or custom getter/setter implementations
- Return by value, const reference, or const pointer
- Atomic access for thread safety

### Types of Properties

#### Storage Types
- `value`: Direct value storage
- `atomic`: Thread-safe atomic storage
- `ptr`: Unique pointer storage
- `custom`: Custom getter/setter implementations

#### Return Types
- `cpy_*`: Return by value
- `cref_*`: Return by const reference
- `cptr_*`: Return by const pointer

### Property Classes

#### Main Property Templates
- `prop_get<T, type>`: Read-only property
- `prop_get_set<T, type>`: Read-write property
- `prop_set<T, type>`: Write-only property

### Examples

#### Basic Value Property

```cpp
class MyClass
{
private:
    int value_m = 0;
    
public:
    // Read-only property returning by value
    prop_get<int, prop::cpy_value> value{value_m};
    
    // Read-write property returning by const reference
    prop_get_set<int, prop::cref_value> value_rw{value_m};
};

int main()
{
    MyClass obj;
    
    // Read value
    int val = obj.value;
    
    // Read/write value
    obj.value_rw = 42;
    int new_val = obj.value_rw;
    
    return 0;
}
```

#### Atomic Property

```cpp
class ThreadSafeClass
{
private:
    std::atomic<int> counter_m{0};
    
public:
    // Thread-safe read-write property
    prop_get_set<int, prop::cpy_atomic> counter{counter_m};
};

int main()
{
    ThreadSafeClass obj;
    
    // Thread-safe operations
    obj.counter = 1;
    int val = obj.counter;
    
    return 0;
}
```

### Custom Property

```cpp
class CustomClass
{
private:
    int value_m = 0;
    
public:
    // Custom property with validation
    prop_get_set<int, prop::cpy_custom> value
    {
        // Getter
        [this](const prop_get_set<int, prop::cpy_custom>*, status& result) 
        {
            result = status::ok;
            return value_m;
        },
        // Setter with validation
        [this](prop_get_set<int, prop::cpy_custom>*, const int& new_value) 
        {
            if(new_value < 0)
            {
                return status::invalid_param;
            }
            value_m = new_value;
            return status::ok;
        }
    };
};

int main()
{
    CustomClass obj;
    
    // Using custom property
    try 
    {
        obj.value = 42;    // OK
        obj.value = -1;    // Throws status_error
    }
    catch(const ctle::status_error& e)
    {
        // Handle error
    }
    
    return 0;
}
```
