#include <systemc.h>

SC_MODULE(SRAM) {
    sc_in<sc_uint<4>> addr;
    sc_in<sc_uint<8>> data_in;
    sc_in<bool> we, clk;
    sc_out<sc_uint<8>> data_out;

    sc_uint<8> mem[16];

    void process() {
        if (we.read()) mem[addr.read()] = data_in.read();
        data_out.write(mem[addr.read()]);
    }
    SC_CTOR(SRAM) { SC_METHOD(process); sensitive << clk.pos(); }
};
int sc_main(int, char*[]) { return 0; }
