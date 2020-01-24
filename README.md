# CppListenersSet
Very lightweight and simple C++ Singal/Slot library

## Introduction
There are many singal/slot libraries like boost::signal and signal/slots in Qt. However this one is extremely simple, lighweight and powerfull enough for most use cases.

## ListenersSet Class
ListenersSet is a container of listeners with spesified signature, similar to a set of std::function. To create a new object you specify the list of arguments as template parameters. The return value is always void and you shouldn't specify it.

Example 1: Listeners without arguments
```C++
CppUtils::ListenersSet<> onClickListeners;
```

Example 2: Listeners with int and std::string arguments
```C++
CppUtils::ListenersSet<int, std::string> onNewListItemAdded;
```
