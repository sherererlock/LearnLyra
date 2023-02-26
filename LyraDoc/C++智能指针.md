# C++智能指针

普通指针

```c++
{
    int* ptr = new int(100);
    delete ptr;
}

```

智能指针

```c++
{
    std::unique_ptr<int> uptr = std::make_unique<int>(100);
    std::shared_ptr<int> sptr = std::make_shared<int>(100);
}

```

std::unique_ptr

```c++
{
    std::unique_ptr<int> uptr = std::make_unique<int>(200);
    std::unique_ptr<int> uptr1 = uptr;// error
    std::unique_ptr<int> uptr2 = std::move(uptr);
}
```

