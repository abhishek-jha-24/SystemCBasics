#include <systemc.h>

SC_MODULE(Resolved) {
    sc_signal_resolved sig;
    sc_in<sc_logic> in1, in2;
    sc_out<sc_logic> out;

    void process1() { sig.write(in1.read()); }
    void process2() { sig.write(in2.read()); }
    void reader()   { out.write(sig.read()); }

    SC_CTOR(Resolved) {
        SC_METHOD(process1); sensitive << in1;
        SC_METHOD(process2); sensitive << in2;
        SC_METHOD(reader);   sensitive << sig;
    }
};
int sc_main(int, char*[]) { return 0; }
