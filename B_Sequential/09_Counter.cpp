#include <systemc.h>

SC_MODULE(Counter) {
    sc_in<bool> clk, rst, load, up_down;
    sc_in<sc_uint<4>> data_in;
    sc_out<sc_uint<4>> count;

    sc_uint<4> internal_count;

    void process() {
        if (rst.read()) internal_count = 0;
        else if (load.read()) internal_count = data_in.read();
        else if (up_down.read()) internal_count++; // up
        else internal_count--;                     // down
        
        count.write(internal_count);
    }

    SC_CTOR(Counter) {
        SC_METHOD(process);
        sensitive << clk.pos() << rst.pos();
    }
};

int sc_main(int, char*[]) { return 0; }
