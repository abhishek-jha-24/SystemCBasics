#include <systemc.h>

SC_MODULE(TimeoutEvent) {
    sc_event e;

    void receiver() {
        wait(10, SC_NS, e); // Wait for e, BUT timeout after 10ns
    }

    SC_CTOR(TimeoutEvent) { SC_THREAD(receiver); }
};
int sc_main(int, char*[]) { return 0; }
