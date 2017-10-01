#include "safe_mutex.hpp"
#include <chrono>
#include <mutex>
#include <thread>
#include <gtest/gtest.h>

TEST(SafeMutex, ShouldDeleteEvenIfLocked)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::unique_ptr<std::thread> thread;

    {
        ft::safe_mutex<std::mutex> mutex;

        thread.reset(new std::thread(
                [&mutex] ()
                {
                    mutex.lock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    mutex.unlock();
                }));

        // Let the thread start and lock the mutex
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // Destroy the mutex; this should block until the thread unlocks it
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    EXPECT_GE(diff, std::chrono::milliseconds(40));

    thread->join();
}

TEST(SafeMutex, WithUniquePtr)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::unique_ptr<ft::safe_mutex<std::mutex>> mutex
        = std::make_unique<ft::safe_mutex<std::mutex>>();

    // NB: Don't pass a reference to the unique_ptr, that will cause a crash;
    // and this is not how unique_ptr are meant to be used.
    ft::safe_mutex<std::mutex>* ptr = mutex.get();
    std::thread thread(
            [ptr] ()
            {
                ptr->lock();
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                ptr->unlock();
            });

    // Let the thread start and lock the mutex
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Destroy the mutex; this should block until the thread unlocks it
    mutex.reset();
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    EXPECT_GE(diff, std::chrono::milliseconds(40));

    thread.join();
}

TEST(SafeMutex, WithUniqueLock)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::unique_ptr<std::thread> thread;

    {
        ft::safe_mutex<std::mutex> mutex;

        thread.reset(new std::thread(
                [&mutex] ()
                {
                    std::unique_lock<ft::safe_mutex<std::mutex>> lock(mutex);
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }));

        // Let the thread start and lock the mutex
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // Destroy the mutex; this should block until the thread unlocks it
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    EXPECT_GE(diff, std::chrono::milliseconds(40));

    thread->join();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
