#include <systemc.h>

// Simple Programmable Interrupt Controller (PIC)
SC_MODULE(InterruptController) {
    sc_in<bool> clk;
    sc_in<bool> rst;
    
    // 4 interrupt sources
    sc_in<bool> irq[4];
    
    // Output to CPU
    sc_out<bool> cpu_int;
    sc_out<int> cpu_vector; // Interrupt vector for CPU
    
    // CPU Ack
    sc_in<bool> int_ack;

    // Registers
    int mask_reg;       // 1 = masked, 0 = enabled
    int pending_reg;
    int priority_reg;   // Simplified: 0 has highest priority

    void monitor_irqs() {
        if (rst.read()) {
            mask_reg = 0x0; // All enabled
            pending_reg = 0x0;
            cpu_int.write(false);
            cpu_vector.write(0);
        } else {
            // Read IRQs and update pending register
            int current_irq = 0;
            for (int i=0; i<4; i++) {
                if (irq[i].read()) {
                    current_irq |= (1 << i);
                }
            }
            pending_reg |= current_irq; // Latch them

            // Process interrupts if CPU is not acknowledging a previous one
            if (!int_ack.read()) {
                int unmasked_pending = pending_reg & (~mask_reg);
                if (unmasked_pending != 0) {
                    // Find highest priority (lowest bit set)
                    int vector = -1;
                    for (int i=0; i<4; i++) {
                        if ((unmasked_pending >> i) & 1) {
                            vector = i;
                            break;
                        }
                    }

                    if (vector != -1) {
                        cpu_int.write(true);
                        cpu_vector.write(vector);
                        // Clear the pending bit since we are serving it
                        pending_reg &= ~(1 << vector);
                        cout << "@" << sc_time_stamp() << " PIC: Interrupt " << vector << " sent to CPU." << endl;
                    }
                } else {
                    cpu_int.write(false);
                }
            } else {
                // CPU acknowledged
                cpu_int.write(false);
            }
        }
    }

    SC_CTOR(InterruptController) {
        SC_METHOD(monitor_irqs);
        sensitive << clk.pos() << rst;
        for (int i=0; i<4; i++) {
            sensitive << irq[i];
        }
        sensitive << int_ack;
    }
};

int sc_main(int, char*[]) {
    // Dummy main for compilation
    sc_clock clk("clk", 10, SC_NS);
    InterruptController pic("pic");
    pic.clk(clk);
    return 0;
}
