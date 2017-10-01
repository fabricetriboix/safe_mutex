# safe_mutex

A wrapper around a mutex type to allow a safe deletion. Mutexes can be
deleted even if they are locked, which is usually an "undefined"
condition, such as with `std::mutex`.

This is implemented by having the underlying mutex locked in the
destructor, which means the destructor is potentially blocking, pending
the underlying mutex to be released by whomever owns it. Whether that's
acceptable or not for your application is up to your requirements.

To use the safe_mutex, just copy the header file where you need it.

Usage:

    #include <mutex> // for example
    #include <safe_mutex.hpp>

    safe_mutex<std::mutex> safe_mtx;
    std::unique_lock<safe_mutex<std::mutex>> lock();

