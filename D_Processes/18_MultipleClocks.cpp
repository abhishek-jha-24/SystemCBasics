#include <systemc.h>

SC_MODULE(MultiClock) {
    sc_in<bool> clk_fast, clk_slow;

    void fast_task() { /* runs fast */ }
    void slow_task() { /* runs slow */ }

    SC_CTOR(MultiClock) {
        SC_METHOD(fast_task); sensitive << clk_fast.pos();
        SC_METHOD(slow_task); sensitive << clk_slow.pos();
    }
};
int sc_main(int, char*[]) { return 0; }
