#include <systemc.h>
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
        
        cout << "\n--- Statistics ---" << endl;
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
