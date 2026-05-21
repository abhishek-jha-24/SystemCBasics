#include <systemc.h>

SC_MODULE(DFF) {
    sc_in<bool> d, clk;
    sc_out<bool> q;

    void process() {
        q.write(d.read());
    }

    SC_CTOR(DFF) {
        SC_METHOD(process);
        sensitive << clk.pos(); // Trigger only on positive clock edge
    }
};

int sc_main(int, char*[]) { return 0; }
