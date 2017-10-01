# safe_mutex

A wrapper around a mutex type to allow a safe deletion. Mutexes can be deleted
even if they are locked by another thread, which is usually an "undefined"
condition, such as with `std::mutex`.

This is implemented by having the underlying mutex locked in the destructor,
which means the destructor is potentially blocking, pending the underlying
mutex to be released by whomever owns it. Whether that's acceptable or not for
your application is up to your requirements.

You should be able to wrap a safe_mutex is a RAII lock, such
as `std::unique_lock`.

To use the safe_mutex, just copy the header file wherever you need it.

Usage:

    #include <mutex> // for example
    #include <safe_mutex.hpp>

    ft::safe_mutex<std::mutex> safe_mutex;
    std::unique_lock<ft::safe_mutex<std::mutex>> lock(safe_mutex);

