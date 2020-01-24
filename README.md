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
    void operator()() const {
        cout<<"yo!";
    }
};

Handler* handler = new Handler();
onClickListeners.addListener(handler);
```

## Executing the listeners
### Without arguments
```C++
listeners.execute();
```
### With arguments
```C++
listeners.execute(5, "some string");
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
