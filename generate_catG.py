import os

base_dir = r"c:\Users\abhi2\Documents\profile\systemCBasics"

tutorials = {
    "G_Architecture": [
        ("31_DirectMappedCache", """#include <systemc.h>
#include <iomanip>

// Simple Direct-Mapped Cache Simulator
// 16 lines, 1 word per line
SC_MODULE(DirectMappedCache) {
    sc_in<bool> clk;
    
    struct CacheLine {
        bool valid;
        int tag;
        int data;
        CacheLine() : valid(false), tag(0), data(0) {}
    };

    CacheLine cache[16];
    
    // Statistics
    int hits;
    int misses;

    void process_request(int address, int write_data, bool is_write) {
        // Simple address breakdown: 
        // Lower 4 bits = Index (16 lines)
        // Upper bits = Tag
        int index = address & 0x0F;
        int tag = address >> 4;

        if (cache[index].valid && cache[index].tag == tag) {
            hits++;
            cout << "Time: " << sc_time_stamp() << " | HIT  | Addr: " << address;
            if (is_write) {
                cache[index].data = write_data;
                cout << " | Wrote: " << write_data << endl;
            } else {
                cout << " | Read: " << cache[index].data << endl;
            }
        } else {
            misses++;
            cout << "Time: " << sc_time_stamp() << " | MISS | Addr: " << address;
            // Fetch from memory (simulated here by just taking the data or returning 0)
            cache[index].valid = true;
            cache[index].tag = tag;
            if (is_write) {
                cache[index].data = write_data;
                cout << " | Wrote: " << write_data << endl;
            } else {
                cache[index].data = address * 10; // Dummy memory data
                cout << " | Fetched: " << cache[index].data << endl;
            }
        }
    }

    void test_thread() {
        wait(10, SC_NS);
        process_request(0x12, 0, false); // Miss
        wait(10, SC_NS);
        process_request(0x12, 0, false); // Hit
        wait(10, SC_NS);
        process_request(0x22, 55, true); // Miss (conflict on index 2)
        wait(10, SC_NS);
        process_request(0x12, 0, false); // Miss (evicted by 0x22)
        
        cout << "\\n--- Statistics ---" << endl;
        cout << "Hits: " << hits << " | Misses: " << misses << endl;
    }

    SC_CTOR(DirectMappedCache) {
        hits = 0; misses = 0;
        SC_THREAD(test_thread);
    }
};

int sc_main(int, char*[]) {
    DirectMappedCache dmc("dmc");
    sc_start();
    return 0;
}
"""),
        ("32_SetAssociativeCache", """#include <systemc.h>

// 2-Way Set Associative Cache Simulator
// 8 sets, 2 ways per set
SC_MODULE(SetAssociativeCache) {
    struct CacheLine {
        bool valid;
        int tag;
        int data;
        int last_used; // For LRU
        CacheLine() : valid(false), tag(0), data(0), last_used(0) {}
    };

    CacheLine cache[8][2]; // [Set][Way]
    int access_counter;

    void process_request(int address) {
        int index = address & 0x07; // 8 sets
        int tag = address >> 3;
        access_counter++;

        // Check for hit
        for (int way = 0; way < 2; way++) {
            if (cache[index][way].valid && cache[index][way].tag == tag) {
                cache[index][way].last_used = access_counter; // Update LRU
                cout << "HIT  | Addr: " << address << " in Set " << index << " Way " << way << endl;
                return;
            }
        }

        // Miss - Find replacement
        int lru_way = 0;
        if (!cache[index][0].valid) lru_way = 0;
        else if (!cache[index][1].valid) lru_way = 1;
        else if (cache[index][0].last_used < cache[index][1].last_used) lru_way = 0;
        else lru_way = 1;

        cout << "MISS | Addr: " << address << " replacing Set " << index << " Way " << lru_way << endl;
        cache[index][lru_way].valid = true;
        cache[index][lru_way].tag = tag;
        cache[index][lru_way].last_used = access_counter;
    }

    void test_thread() {
        process_request(0x01); // Miss (Set 1, Way 0)
        process_request(0x09); // Miss (Set 1, Way 1)
        process_request(0x01); // Hit  (Set 1, Way 0)
        process_request(0x11); // Miss (Set 1, Way 1 - LRU replaces 0x09)
        process_request(0x09); // Miss (Set 1, Way 0 - LRU replaces 0x01)
    }

    SC_CTOR(SetAssociativeCache) {
        access_counter = 0;
        SC_THREAD(test_thread);
    }
};

int sc_main(int, char*[]) {
    SetAssociativeCache sac("sac");
    sc_start();
    return 0;
}
"""),
        ("33_BasicPipeline", """#include <systemc.h>

// Simple 3-Stage Pipeline: Fetch, Decode, Execute
SC_MODULE(Pipeline) {
    sc_in<bool> clk;

    // Pipeline registers
    int if_id_inst;
    int id_ex_inst;

    void fetch() {
        int pc = 0;
        while (true) {
            wait();
            if_id_inst = pc++; // Dummy instruction fetch
            cout << "Time: " << sc_time_stamp() << " | FETCH   inst: " << if_id_inst << endl;
        }
    }

    void decode() {
        while (true) {
            wait();
            id_ex_inst = if_id_inst;
            cout << "Time: " << sc_time_stamp() << " | DECODE  inst: " << id_ex_inst << endl;
        }
    }

    void execute() {
        while (true) {
            wait();
            cout << "Time: " << sc_time_stamp() << " | EXECUTE inst: " << id_ex_inst << endl;
            cout << "------------------------" << endl;
        }
    }

    SC_CTOR(Pipeline) {
        if_id_inst = -1; id_ex_inst = -1;
        SC_CTHREAD(fetch, clk.pos());
        SC_CTHREAD(decode, clk.pos());
        SC_CTHREAD(execute, clk.pos());
    }
};

int sc_main(int, char*[]) {
    sc_clock clk("clk", 10, SC_NS);
    Pipeline pipe("pipe");
    pipe.clk(clk);
    sc_start(50, SC_NS);
    return 0;
}
"""),
        ("34_BusArbiter", """#include <systemc.h>

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
"""),
        ("35_TLM_Introduction", """#include <systemc.h>

// A highly simplified representation of TLM concepts using standard C++ structures.
// (Note: True TLM 2.0 requires the systemc-tlm library and b_transport interfaces,
// but this illustrates the conceptual difference between pin-wiggling and transaction passing).

struct Transaction {
    int address;
    int data;
    bool is_write;
};

SC_MODULE(Initiator) {
    sc_fifo_out<Transaction> socket_out;

    void run() {
        Transaction t;
        t.address = 0x100; t.data = 42; t.is_write = true;
        cout << "Initiator sending WRITE transaction to " << t.address << endl;
        socket_out.write(t); // Send entire transaction in one burst
    }
    SC_CTOR(Initiator) { SC_THREAD(run); }
};

SC_MODULE(Target) {
    sc_fifo_in<Transaction> socket_in;

    void run() {
        while(true) {
            Transaction t = socket_in.read(); // Receive entire transaction
            cout << "Target received ";
            if (t.is_write) cout << "WRITE: " << t.data << " to Addr: " << t.address << endl;
            else cout << "READ request for Addr: " << t.address << endl;
        }
    }
    SC_CTOR(Target) { SC_THREAD(run); }
};

int sc_main(int, char*[]) {
    sc_fifo<Transaction> bus;
    Initiator init("init");
    Target targ("targ");
    init.socket_out(bus);
    targ.socket_in(bus);
    sc_start();
    return 0;
}
"""),
        ("36_UART_Transmitter", """#include <systemc.h>

SC_MODULE(UART_Tx) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<bool> tx_start;
    sc_in<sc_uint<8> > data_in;
    sc_out<bool> tx_out;
    sc_out<bool> tx_ready;

    sc_uint<10> shift_reg;
    int bit_count;
    bool sending;

    void transmit() {
        if (reset.read()) {
            tx_out.write(true);
            tx_ready.write(true);
            sending = false;
            bit_count = 0;
        } else {
            if (!sending && tx_start.read()) {
                // start bit (0) + data + stop bit (1)
                shift_reg = (1 << 9) | (data_in.read() << 1) | 0;
                sending = true;
                bit_count = 10;
                tx_ready.write(false);
            }

            if (sending) {
                tx_out.write(shift_reg[0]);
                shift_reg = shift_reg >> 1;
                bit_count--;
                if (bit_count == 0) {
                    sending = false;
                    tx_ready.write(true);
                }
            }
        }
    }

    SC_CTOR(UART_Tx) {
        SC_METHOD(transmit);
        sensitive << clk.pos() << reset;
    }
};

int sc_main(int, char*[]) {
    sc_clock clk("clk", 10, SC_NS);
    sc_signal<bool> reset, tx_start, tx_out, tx_ready;
    sc_signal<sc_uint<8> > data_in;

    UART_Tx uart("uart");
    uart.clk(clk);
    uart.reset(reset);
    uart.tx_start(tx_start);
    uart.data_in(data_in);
    uart.tx_out(tx_out);
    uart.tx_ready(tx_ready);

    sc_start(0, SC_NS);
    reset.write(true);
    sc_start(10, SC_NS);
    reset.write(false);
    data_in.write(0xAB);
    tx_start.write(true);
    sc_start(10, SC_NS);
    tx_start.write(false);
    sc_start(150, SC_NS);

    return 0;
}
""")
,
        ("37_DMA_Controller", """#include <systemc.h>

// Simple DMA Controller Simulator
SC_MODULE(DMA_Controller) {
    sc_in<bool> clk;
    sc_in<bool> dma_req;
    sc_out<bool> dma_ack;
    
    // Memory interface
    sc_out<int> mem_addr;
    sc_in<int> mem_data_read;
    sc_out<int> mem_data_write;
    sc_out<bool> mem_write_en;

    // DMA Registers
    int source_addr;
    int dest_addr;
    int transfer_length;
    bool busy;

    void do_transfer() {
        dma_ack.write(false);
        mem_write_en.write(false);
        busy = false;

        while (true) {
            wait();
            if (dma_req.read() && !busy) {
                busy = true;
                dma_ack.write(true);
                cout << "@" << sc_time_stamp() << " DMA Transfer Started. Src: " 
                     << source_addr << " Dest: " << dest_addr << " Len: " << transfer_length << endl;
            }

            if (busy && transfer_length > 0) {
                // Read from source
                mem_addr.write(source_addr);
                mem_write_en.write(false);
                wait(); // Wait for memory read
                int data = mem_data_read.read();

                // Write to dest
                mem_addr.write(dest_addr);
                mem_data_write.write(data);
                mem_write_en.write(true);
                wait(); // Wait for memory write

                cout << "@" << sc_time_stamp() << " DMA transferred data: " << data 
                     << " from " << source_addr << " to " << dest_addr << endl;

                source_addr++;
                dest_addr++;
                transfer_length--;

                if (transfer_length == 0) {
                    busy = false;
                    dma_ack.write(false);
                    mem_write_en.write(false);
                    cout << "@" << sc_time_stamp() << " DMA Transfer Complete." << endl;
                }
            }
        }
    }

    SC_CTOR(DMA_Controller) {
        SC_CTHREAD(do_transfer, clk.pos());
    }
};

int sc_main(int, char*[]) {
    // Dummy main to allow compilation
    sc_clock clk("clk", 10, SC_NS);
    DMA_Controller dma("dma");
    dma.clk(clk);
    return 0;
}
""")
,
        ("38_Interrupt_Controller", """#include <systemc.h>

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
""")
    ]
}

def generate():
    for category, files in tutorials.items():
        cat_path = os.path.join(base_dir, category)
        os.makedirs(cat_path, exist_ok=True)
        for name, content in files:
            file_path = os.path.join(cat_path, f"{name}.cpp")
            with open(file_path, "w") as f:
                f.write(content)
                
generate()
