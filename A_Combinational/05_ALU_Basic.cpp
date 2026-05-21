#include <systemc.h>

// Simple ALU performing Add, Sub, AND, OR
SC_MODULE(ALU) {
    sc_in<sc_int<8>> a, b;
    sc_in<sc_uint<2>> op;
    sc_out<sc_int<8>> res;

    void process() {
        switch(op.read()) {
            case 0: res.write(a.read() + b.read()); break;
            case 1: res.write(a.read() - b.read()); break;
            case 2: res.write(a.read() & b.read()); break;
            case 3: res.write(a.read() | b.read()); break;
        }
    }

    SC_CTOR(ALU) {
        SC_METHOD(process);
        sensitive << a << b << op;
    }
};

int sc_main(int argc, char* argv[]) { return 0; }
