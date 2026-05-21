#include <systemc.h>

SC_MODULE(TriState) {
    sc_in<sc_logic> in;
    sc_in<bool> enable;
    sc_out<sc_logic> out;

    void process() {
        if (enable.read()) out.write(in.read());
        else               out.write(SC_LOGIC_Z); // High impedance
    }
    SC_CTOR(TriState) { SC_METHOD(process); sensitive << in << enable; }
};
int sc_main(int, char*[]) { return 0; }
