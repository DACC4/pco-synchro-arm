/*****************************************************************************
 * Copyright (C) 2020 HEIG-VD                                                *
 *                                                                           *
 * This file is part of PcoSynchro.                                          *
 *                                                                           *
 * PcoSynchro is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU Lesser General Public License as published  *
 * by the Free Software Foundation, either version 3 of the License, or      *
 * (at your option) any later version.                                       *
 *                                                                           *
 * PcoSynchro is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU Lesser General Public License for more details.                       *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with PcoSynchro.  If not, see <https://www.gnu.org/licenses/>.      *
 *****************************************************************************/

#include <future>

#include <gtest/gtest.h>

#include "../src/pcomutex.h"
#include "../src/pcosemaphore.h"
#include "../src/pcoconditionvariable.h"
#include "../src/pcothread.h"
#include "../src/pcomanager.h"
#include "../src/pcotest.h"

TEST(PcoMutex, LockUnlock) {
    // Req: Locking and unlocking an open mutex should not block the caller

    ASSERT_DURATION_LE(1, {
                           PcoMutex mutex;
                           mutex.lock();
                           mutex.unlock();
                       })
}

TEST(PcoMutex, Blocked) {
    // Req: Calling twice lock() on a mutex blocks the caller

    // If we lock again we are blocked
    ASSERT_DURATION_GE(1, {
                           PcoMutex mutex1;
                           mutex1.lock();
                           mutex1.lock();
                       })
}


TEST(PcoMutex, RecursiveNotBlocked) {
    // Req: If we lock twice a recursive mutex it should not block

    // The two locks are in the macro, as they have to be called from
    // the same thread
    ASSERT_DURATION_LE(1, {
                           PcoMutex mutex(PcoMutex::Recursive);
                           mutex.lock();
                           mutex.lock();
                       })
}


TEST(PcoMutex, CriticalSection) {
    // Req: A critical section protected by a PcoMutex should not be reached
    //      by more that one thread

    PcoMutex mutex;
    bool oneIn = false;
    std::thread t1([&oneIn, &mutex](){
        mutex.lock();
        ASSERT_EQ(oneIn, false);
        oneIn = true;
        std::chrono::microseconds value(1000);
        std::this_thread::sleep_for(value);
        ASSERT_EQ(oneIn, true);
        oneIn = false;
        std::chrono::microseconds value1(1000);
        std::this_thread::sleep_for(value1);
        ASSERT_EQ(oneIn, false);
        mutex.unlock();
    });

    std::thread t2([&oneIn, &mutex](){
        mutex.lock();
        ASSERT_EQ(oneIn, false);
        oneIn = true;
        std::chrono::microseconds value(500);
        std::this_thread::sleep_for(value);
        ASSERT_EQ(oneIn, true);
        oneIn = false;
        std::chrono::microseconds value1(500);
        std::this_thread::sleep_for(value1);
        ASSERT_EQ(oneIn, false);
        mutex.unlock();
    });
    t1.join();
    t2.join();
}

TEST(PcoSemaphore, Blocked) {
    // Req: A semaphore that reaches a negative value blocks the caller

    // We block directly at least one thread
    ASSERT_DURATION_GE(1,PcoSemaphore sem(0);sem.acquire())
    ASSERT_DURATION_GE(1,PcoSemaphore sem(1);sem.acquire();sem.acquire())
    ASSERT_DURATION_GE(1,PcoSemaphore sem(2);sem.acquire();sem.acquire();sem.acquire())
}

TEST(PcoSemaphore, NotBlocked) {
    // Req: A semaphore that has a positive value should not block the caller

    // We do not block any thread
    ASSERT_DURATION_LE(1,PcoSemaphore sem(1);sem.acquire())
    ASSERT_DURATION_LE(1,PcoSemaphore sem(2);sem.acquire();sem.acquire())
    ASSERT_DURATION_LE(1,PcoSemaphore sem(3);sem.acquire();sem.acquire();sem.acquire())
}


TEST(PcoSemaphore, Order) {
    // Req: The releases on a semaphore are done in FIFO order

    PcoSemaphore sem;
    int numOut = 0;
    std::thread t1([&](){
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
        sem.acquire();
        ASSERT_EQ(numOut, 0);
        numOut ++;
    });

    std::thread t2([&](){
        std::this_thread::sleep_for(std::chrono::microseconds(2000));
        sem.acquire();
        ASSERT_EQ(numOut, 1);
        numOut ++;
    });

    std::thread t3([&](){
        std::this_thread::sleep_for(std::chrono::microseconds(3000));
        sem.acquire();
        ASSERT_EQ(numOut, 2);
        numOut ++;
    });

    std::this_thread::sleep_for(std::chrono::microseconds(4000));
    sem.release();
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    sem.release();
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    sem.release();
    std::this_thread::sleep_for(std::chrono::microseconds(1000));

    t1.join();
    t2.join();
    t3.join();
}

TEST(PcoConditionVariable, Blocked) {
    // Req: Waiting on a condition is blocking

    ASSERT_DURATION_GE(1, {
                           PcoMutex mutex;
                           PcoConditionVariable cond;
                           mutex.lock();
                           cond.wait(&mutex);
                       })
}


TEST(PcoConditionVariable, Notify) {
    // Req: A thread waiting on a condition variable is released by notifyOne()
    //      called from outside the critical section

    PcoMutex mutex;
    PcoConditionVariable cond;

    std::thread t1([&](){
        mutex.lock();
        cond.wait(&mutex);
        mutex.unlock();
    });

    std::thread t2([&](){
        std::this_thread::sleep_for(std::chrono::microseconds(2000));
        cond.notifyOne();
    });

    t1.join();
    t2.join();
}

TEST(PcoConditionVariable, Notify2) {
    // Req: A thread waiting on a condition variable is released by notifyOne()
    //      called from within the critical section

    PcoMutex mutex;
    PcoConditionVariable cond;

    std::thread t1([&](){
        mutex.lock();
        cond.wait(&mutex);
        mutex.unlock();
    });

    std::thread t2([&](){
        std::this_thread::sleep_for(std::chrono::microseconds(2000));
        mutex.lock();
        cond.notifyOne();
        mutex.unlock();
    });

    t1.join();
    t2.join();
}

TEST(PcoConditionVariable, NotifyAll) {
    // Req: A call to notifyAll() releases all threads waiting on a condition variable
    PcoMutex mutex;
    PcoConditionVariable cond;

    std::thread t1([&](){
        mutex.lock();
        cond.wait(&mutex);
        mutex.unlock();
    });

    std::thread t2([&](){
        mutex.lock();
        cond.wait(&mutex);
        mutex.unlock();
    });

    std::thread t3([&](){
        std::this_thread::sleep_for(std::chrono::microseconds(2000));
        mutex.lock();
        cond.notifyAll();
        mutex.unlock();
    });

    t1.join();
    t2.join();
    t3.join();
}

TEST(PcoConditionVariable, Notify3) {
    // Req: If two threads are waiting on a condition variable and
    //      two notifyOne() are issued, both threads should continue

    ASSERT_DURATION_LE(1, {

                           PcoMutex mutex;
                           PcoConditionVariable cond;

                           std::thread t1([&](){
                               mutex.lock();
                               cond.wait(&mutex);
                               mutex.unlock();
                           });

                           std::thread t2([&](){
                               mutex.lock();
                               cond.wait(&mutex);
                               mutex.unlock();
                           });

                           std::thread t3([&](){
                               std::this_thread::sleep_for(std::chrono::microseconds(2000));
                               mutex.lock();
                               cond.notifyOne();
                               cond.notifyOne();
                               mutex.unlock();
                           });

                           t1.join();
                           t2.join();
                           t3.join();
                       })
}

TEST(PcoConditionVariable, Notify4) {
    // Req: If two threads are waiting on a condition variable and
    //      one notifyOne() is issued, only one thread should continue,
    //      the other one shall stall

    int nbFininished = 0;

    ASSERT_DURATION_GE(1, {
                           PcoMutex mutex;
                           PcoConditionVariable cond;

                           std::thread t1([&](){
                               mutex.lock();
                               cond.wait(&mutex);
                               nbFininished ++;
                               mutex.unlock();
                           });

                           std::thread t2([&](){
                               mutex.lock();
                               cond.wait(&mutex);
                               nbFininished ++;
                               mutex.unlock();
                           });

                           std::thread t3([&](){
                               std::this_thread::sleep_for(std::chrono::microseconds(2000));
                               mutex.lock();
                               cond.notifyOne();
                               mutex.unlock();
                           });

                           t1.join();
                           t2.join();
                           t3.join();
                       })
            // We check that only a single thread finished
            ASSERT_EQ(nbFininished, 1);
}


TEST(PcoThread, Normal) {
    // Req: A thread should execute and finish, letting another one do the join

    int number = 0;
    PcoThread t1([&](){
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
        number ++;
    });

    t1.join();
    ASSERT_EQ(number, 1);
}


TEST(PcoThread, Arguments) {
    // Req: A thread should execute and finish, letting another one do the join

    int number = 10;
    PcoThread t1([&](int n){
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
        if (n == 10) {
            number ++;
        }
    }, number);

    t1.join();
    ASSERT_EQ(number, 11);
}

TEST(PcoThread, stopRequest) {
    // Req: A thread should be interruptible thanks to a stop request

    int number = 10;
    bool interrupted = false;
    PcoThread t2([&](int n){
        for (int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
        if (n == 10) {
            number ++;
        }

        if (PcoThread::thisThread()->stopRequested()) {
            interrupted = true;
            return;
        }
        }
    }, number);

    // Sleeping in a async is not supported. The test should be written
    // differently
    //std::this_thread::sleep_for(std::chrono::microseconds(3500));
    t2.requestStop();

    t2.join();
    ASSERT_EQ(interrupted, true);
    ASSERT_EQ(number, 11);
}

TEST(PcoThread, thisThread) {
    // Req: thisThread() should return a pointer for a PcoThread, nullptr else

    PcoThread t1([&](){
        auto id = PcoThread::thisThread();
        ASSERT_NE(id, nullptr);
        });

    auto id = PcoThread::thisThread();
    ASSERT_EQ(id, nullptr);

    t1.join();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
