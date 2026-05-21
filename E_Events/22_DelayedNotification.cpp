#include <systemc.h>

SC_MODULE(DelayEvent) {
    sc_event e;

    void trigger() {
        e.notify(10, SC_NS); // Notify exactly 10ns from now
    }
    void receiver() {
        wait(e);
    }

    SC_CTOR(DelayEvent) {
        SC_THREAD(trigger);
        SC_THREAD(receiver);
    }
};
int sc_main(int, char*[]) { return 0; }
