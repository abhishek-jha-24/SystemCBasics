#include <systemc.h>

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
