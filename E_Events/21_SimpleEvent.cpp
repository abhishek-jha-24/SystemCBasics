#include <systemc.h>

SC_MODULE(EventBasic) {
    sc_event e;

    void trigger() {
        wait(5, SC_NS);
        e.notify();
    }
    void receiver() {
        wait(e);
        cout << "Event received!" << endl;
    }

    SC_CTOR(EventBasic) {
        SC_THREAD(trigger);
        SC_THREAD(receiver);
    }
};
int sc_main(int, char*[]) { return 0; }
