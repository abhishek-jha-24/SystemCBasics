#include <systemc.h>

SC_MODULE(SemaphoreExample) {
    sc_semaphore sem;

    void worker() {
        sem.wait(); // acquire
        // do work
        sem.post(); // release
    }

    SC_CTOR(SemaphoreExample) : sem(3) { // Allow 3 concurrent accesses
        SC_THREAD(worker);
    }
};
int sc_main(int, char*[]) { return 0; }
