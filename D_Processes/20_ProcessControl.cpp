#include <systemc.h>

SC_MODULE(Control) {
    sc_in<bool> clk;
    sc_event e;
    sc_process_handle ph;

    void worker() {
        while(true) { wait(clk.pos()); /* working */ }
    }
    void controller() {
        ph.suspend();
        wait(10, SC_NS);
        ph.resume();
    }

    SC_CTOR(Control) {
        SC_THREAD(worker); ph = sc_get_current_process_handle();
        SC_THREAD(controller);
    }
};
int sc_main(int, char*[]) { return 0; }
