#include <systemc.h>

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
