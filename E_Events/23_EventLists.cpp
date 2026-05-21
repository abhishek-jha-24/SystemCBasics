#include <systemc.h>

SC_MODULE(EventLists) {
    sc_event e1, e2;

    void receiver() {
        wait(e1 & e2); // Wait for BOTH
        wait(e1 | e2); // Wait for EITHER
    }

    SC_CTOR(EventLists) { SC_THREAD(receiver); }
};
int sc_main(int, char*[]) { return 0; }
