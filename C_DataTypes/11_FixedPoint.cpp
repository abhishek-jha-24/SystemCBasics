#define SC_INCLUDE_FX
#include <systemc.h>

SC_MODULE(FixedMath) {
    sc_in<sc_fixed<8,4>> a, b;
    sc_out<sc_fixed<9,5>> res;

    void process() {
        res.write(a.read() + b.read());
    }
    SC_CTOR(FixedMath) { SC_METHOD(process); sensitive << a << b; }
};
int sc_main(int, char*[]) { return 0; }
