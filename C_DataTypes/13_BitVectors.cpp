#include <systemc.h>

SC_MODULE(BV_Concat) {
    sc_in<sc_bv<4>> a, b;
    sc_out<sc_bv<8>> out;

    void process() {
        out.write( (a.read(), b.read()) ); // Concatenation
    }
    SC_CTOR(BV_Concat) { SC_METHOD(process); sensitive << a << b; }
};
int sc_main(int, char*[]) { return 0; }
