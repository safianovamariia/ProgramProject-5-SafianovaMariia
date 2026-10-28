// compiler MSVC
// Author: Safianova Mariia K25
// Variant: 10
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <future>
#include <syncstream>

using namespace std;

void slow(const string &name)
{
    this_thread::sleep_for(7s);
    osyncstream(cout) << name << endl;
}

void quick(const string &name)
{
    this_thread::sleep_for(1s);
    osyncstream(cout) << name << endl;
}

void work()
{
    auto start = chrono::steady_clock::now();

    //tells thread 2 that B is done    
    promise<void> bDone;
    future<void> bReady = bDone.get_future();

    //thread 1
    future<void> a2 = async(launch::async, [] { slow("A2"); });

    //thread 2
   future<void> dfc = async(launch::async, [&]
    {
        slow("D");
        quick("F");
        bReady.wait();
        quick("C2");
    });

    //main thread
    slow("A1");
    a2.get();
    quick("B");
    bDone.set_value();
    quick("C1");

    dfc.get();

    chrono::duration<double> elapsed = chrono::steady_clock::now() - start;
    cout << "Time: " << elapsed.count() << "s" << endl;
    cout << "Work is done!" << endl;
}

int main()
{
    work();
    return 0;
}
