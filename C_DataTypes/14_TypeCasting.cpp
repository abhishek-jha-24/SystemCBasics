#include <systemc.h>

SC_MODULE(Caster) {
    sc_in<int> cpp_int;
    sc_out<sc_uint<32>> sys_int;

    void process() {
        sys_int.write((sc_uint<32>)cpp_int.read());
    }
    SC_CTOR(Caster) { SC_METHOD(process); sensitive << cpp_int; }
};
int sc_main(int, char*[]) { return 0; }
