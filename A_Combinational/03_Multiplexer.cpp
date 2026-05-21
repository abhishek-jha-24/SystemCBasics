#include <systemc.h>

// A 2-to-1 Multiplexer selects one of two inputs based on a selection signal.
SC_MODULE(Mux2to1) {
    sc_in<bool> in0, in1, sel;
    sc_out<bool> out;

    void process() {
        if (sel.read() == 0) out.write(in0.read());
        else                 out.write(in1.read());
    }

    SC_CTOR(Mux2to1) {
        SC_METHOD(process);
        sensitive << in0 << in1 << sel;
    }
};

int sc_main(int argc, char* argv[]) {
    return 0;
}
