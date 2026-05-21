#include <systemc.h>

// 2-to-4 Decoder
SC_MODULE(Decoder2to4) {
    sc_in<sc_uint<2>> in;
    sc_out<sc_uint<4>> out;

    void process() {
        switch(in.read()) {
            case 0: out.write(1); break; // 0001
            case 1: out.write(2); break; // 0010
            case 2: out.write(4); break; // 0100
            case 3: out.write(8); break; // 1000
        }
    }

    SC_CTOR(Decoder2to4) {
        SC_METHOD(process);
        sensitive << in;
    }
};

int sc_main(int argc, char* argv[]) {
    return 0;
}
