#include <systemc.h>

SC_MODULE(ShiftReg) {
    sc_in<bool> serial_in, clk;
    sc_out<sc_uint<8>> parallel_out;

    sc_uint<8> reg;

    void process() {
        reg = (reg << 1) | serial_in.read();
        parallel_out.write(reg);
    }

    SC_CTOR(ShiftReg) {
        SC_METHOD(process);
        sensitive << clk.pos();
        reg = 0;
    }
};

int sc_main(int, char*[]) { return 0; }
