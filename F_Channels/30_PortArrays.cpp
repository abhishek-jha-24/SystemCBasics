#include <systemc.h>

// Module with array of ports
SC_MODULE(BusRouter) {
    sc_in<int> inputs[4];
    sc_out<int> outputs[4];

    void route() {
        for(int i=0; i<4; i++) outputs[i].write(inputs[i].read());
    }

    SC_CTOR(BusRouter) {
        SC_METHOD(route);
        for(int i=0; i<4; i++) sensitive << inputs[i];
    }
};

int sc_main(int, char*[]) { return 0; }
