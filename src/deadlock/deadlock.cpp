/* ----------------------------------------------------------------------------
Copyright (C), 2024-2025, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: deadlock.cpp
@version: I
Creation: 11/03/2024
Last modification: 12/03/2024
------------------------------------------------------------------------------*/
#include <iostream>
#include <thread>
#include <mutex>

std::mutex File1, File2;

/* the processes processA and processB acquire file1 and file2 in different orders, resulting in a deadlock, which causes indefinite blocking of the threads  */
void processA() {
    while (true) {
        // acquire the first file try
        File1.lock();
        std::cout << "processA acquired file1" << std::endl;
        // work simulation & force a context switch to cause the deadlock
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        File2.lock();

        // acquire the second file try
        std::cout << "processA acquired file2" << std::endl;
        
        // work with both resources simulation
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // resources release
        File1.unlock();
        File2.unlock();
    }
}

void processB() {
    while (true) {
        File2.lock();
        // acquire the second file try
        std::cout << "processB acquired file2" << std::endl;
        // work simulation & force a context switch to cause the deadlock
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
        File1.lock();

        // acquire the first file try
        std::cout << "processB acquired file1" << std::endl;
        
        // work with both resources simulation
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // resources release
        File2.unlock();
        File1.unlock();
    }
}

int main() {
    std::thread t1(processA);
    std::thread t2(processB);

    t1.join();
    t2.join();

    return 0;
}