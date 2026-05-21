#include <systemc.h>

SC_MODULE(LFSR) {
    sc_in<bool> clk;
    sc_out<sc_uint<4>> out;
    sc_uint<4> state;

    void process() {
        bool bit = state[3] ^ state[2]; // Feedback polynomial
        state = (state << 1) | bit;
        out.write(state);
    }

    SC_CTOR(LFSR) {
        SC_METHOD(process);
        sensitive << clk.pos();
        state = 1; // Seed cannot be 0
    }
};

int sc_main(int, char*[]) { return 0; }
