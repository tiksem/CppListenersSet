# CppListenersSet
Very lightweight and simple C++ Singal/Slot library

## Introduction
There are many singal/slot libraries like boost::signal and signal/slots in Qt. However this one is extremely simple, lighweight and powerfull enough for most use cases.

## ListenersSet Class
ListenersSet is a container of listeners with spesified signature, similar to a set of std::function. To create a new object you specify the list of arguments as template parameters. The return value is always void and you shouldn't specify it.

### Listeners without arguments
```C++
CppUtils::ListenersSet<> onClickListeners;
```

### Listeners with int and std::string arguments
```C++
CppUtils::ListenersSet<int, std::string> onNewListItemAdded;
```

## Adding functions and functional objects as listeners
You can pass any std::function compatible object/function to addListener method

### Passing lambda:
```C++
CppUtils::ListenersSet<> onClickListeners;
onClickListeners.addListener([] {
    cout<<"Clicked";          
})
```

### Passing function pointer:
To pass function pointer, you need to wrap it with std::function
```C++

void handler() {
    cout<<"Clicked";    
}

CppUtils::ListenersSet<> onClickListeners;
onClickListeners.addListener(std::function<void()>(handler))
```

### Passing functional oject:
```C++
class Handler {
public:
    void operator()() const {
        cout<<"yo!";
    }
};

CppUtils::ListenersSet<> onClickListeners;
Handler handler;
// handler is passed by value here and it's copied
onClickListeners.addListener(handler);
```

### Passing a pointer to functional object:
Sometimes you want to pass an object as a listener, but you don't want to copy the object. In this case you can pass a pointer to functional object
```C++
class Handler {
public:
    void operator()() {
        cout<<"yo!";
    }
};

Handler* handler = new Handler();
onClickListeners.addListener(handler);
```

## Executing the listeners
### Without arguments
```C++
listeners.executeAll();
```
### With arguments
```C++
listeners.executeAll(5, "some string");
```

## Removing listeners
Listeners are stored by key for funcational objects, function pointers and lambdas. The key is returned by addListener method.
Pointers to functional objects can be removed only by the pointer itself, so there is no assosiated key.
### Removing objects
```C++
Handler handler;
int key = onClickListeners.addListener(handler);
onClickListeners.removeListener(key);
```
### Removing pointers to objects
```C++
Handler* handler = new Handler();
onClickListeners.addListener(handler);
onClickListeners.removeListener(handler);
```

## Single-shot lisetners
Sometimes you want to execute the listener once and delete it after that
```C++
#include "ListenersSet.h"

class Handler {
public:
    void operator()() const {
        cout<<"yo!";
    }
};

int main(int argc, const char * argv[]) {
    CppUtils::ListenersSet<> onClickListeners;
    Handler handler;
    onClickListeners.addOneShotListener(handler);
    // Outputs yo!
    onClickListeners.executeAll();
    // Outputs nothing!
    onClickListeners.executeAll();
    return 0;
}
```

## Listeners with action
Sometimes you want an advanced logic, where you decide to delete the listener or not after it's execution
```C++
#include "ListenersSet.h"

int main(int argc, const char * argv[]) {
    CppUtils::ListenersSet<> onClickListeners;
    int n = 2;
    onClickListeners.addListenerWithAction([&] () -> CppUtils::ListenerAction {
        cout << "yo!";
        n--;
        if (n != 0) {
            return CppUtils::DONT_DELETE_LISTENER;
        } else {
            return CppUtils::DELETE_LISTENER;
        }
    });
    // Outputs yo!
    onClickListeners.executeAll();
    // Outputs yo!
    onClickListeners.executeAll();
    // Outputs nothing
    onClickListeners.executeAll();
    return 0;
}
```

## Multithreading and synchronization
`ListenersSet` is not synchronized by default. So adding/deleting/executing the listeners should be done on the same thread. If you want a synchronization you should use a synchrnoized version `SynchronizedListenersSet`, it uses std::mutex for synchronization.
### Specifying custom mutex
If you want to pass your own mutex you can do it. The mutex should have lock/unlock methods
```C++
struct MyMutex {
    inline void lock() {
        // your code goes here
    }
    inline void unlock() {
        // your code goes here
    }
};

CppUtils::BaseListenersSet<MyMutex, Arg1,...,ArgN> listeners;
```
## Debugging and assertations
If NDEBUG macro is defined explicitly(usually every compiler adds NDEBUG macro for release mode), there are no any assertations. But if NDEBUG is not defined you get assertations when you use non-synchrnoized `ListenersSet` in different threads and in some other cases. To disable or enable assertations explicitly use `#define/#undef NDEBUG` before including "ListenersSet.h" header.
### Sepcifying custom assert
C assert from assert.h is used bu default. You can define a custom assert by overriding assert macro using #define
```C++
#define assert /*Custom assert override*/
#include "ListenersSet.h"
```
## Handling listeners lifetime
C++ is manual memory management landguage. So sometimes it may happen, that the object is deleted from memory, but the listener assosiated with this object is still in the set. In this case you may face an undefined behaviour. To solve this issue you can manually delete a listener using removeListener method, but sometimes it's inconvinient. The library provides an ability to specify a parent, while adding a listener. And when this parent is deleted the listener will be deleted as well. The parent should extend CppUtils::DestructorQueue or CppUtils::SynchronizedDestructorQueue. 
```C++
#include "ListenersSet.h"

class Parent : public CppUtils::DestructorQueue {
};

int main(int argc, const char * argv[]) {
    CppUtils::ListenersSet<> onClickListeners;
    Parent* parent = new Parent();
    onClickListeners.addListener([&] {
        cout << "yo!";
    }, parent);
    // Outputs yo!
    onClickListeners.executeAll();
    // Outputs nothing!
    delete parent;
    onClickListeners.executeAll();
    return 0;
}
```
