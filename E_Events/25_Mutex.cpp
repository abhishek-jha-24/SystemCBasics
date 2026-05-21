#include <systemc.h>

SC_MODULE(MutexExample) {
    sc_mutex mut;

    void thread1() { mut.lock(); /* critical section */ mut.unlock(); }
    void thread2() { mut.lock(); /* critical section */ mut.unlock(); }

    SC_CTOR(MutexExample) {
        SC_THREAD(thread1); SC_THREAD(thread2);
    }
};
int sc_main(int, char*[]) { return 0; }
