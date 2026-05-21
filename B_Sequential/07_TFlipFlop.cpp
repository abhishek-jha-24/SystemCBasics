#include <systemc.h>

SC_MODULE(TFF) {
    sc_in<bool> t, clk, rst;
    sc_out<bool> q;

    bool internal_q;

    void process() {
        if (rst.read()) {
            internal_q = 0;
        } else if (t.read()) {
            internal_q = !internal_q;
        }
        q.write(internal_q);
    }

    SC_CTOR(TFF) {
        SC_METHOD(process);
        sensitive << clk.pos() << rst.pos(); // Asynchronous reset
        internal_q = 0;
    }
};

int sc_main(int, char*[]) { return 0; }
