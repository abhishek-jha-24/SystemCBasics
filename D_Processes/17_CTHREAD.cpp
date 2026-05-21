#include <systemc.h>

SC_MODULE(ClockedThread) {
    sc_in<bool> clk;

    void my_cthread() {
        while(true) {
            wait(); // Implicitly waits for clk.pos() because of CTHREAD setup
            // Do cycle accurate work
        }
    }

    SC_CTOR(ClockedThread) {
        SC_CTHREAD(my_cthread, clk.pos());
    }
};
int sc_main(int, char*[]) { return 0; }
