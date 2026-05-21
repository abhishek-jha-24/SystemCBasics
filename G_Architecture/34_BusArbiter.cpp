#include <systemc.h>

// Round-Robin Bus Arbiter for 3 Masters
SC_MODULE(Arbiter) {
    sc_in<bool> clk;
    sc_in<bool> req[3];
    sc_out<bool> grant[3];

    int current_priority;

    void arbitrate() {
        // Default to no grants
        for(int i=0; i<3; i++) grant[i].write(false);

        // Check requests starting from current priority
        for(int i=0; i<3; i++) {
            int check_idx = (current_priority + i) % 3;
            if (req[check_idx].read()) {
                grant[check_idx].write(true);
                current_priority = (check_idx + 1) % 3; // Update priority
                break;
            }
        }
    }

    SC_CTOR(Arbiter) {
        current_priority = 0;
        SC_METHOD(arbitrate);
        sensitive << clk.pos();
    }
};

int sc_main(int, char*[]) { return 0; }
