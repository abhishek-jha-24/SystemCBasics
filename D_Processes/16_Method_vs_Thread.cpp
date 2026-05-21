#include <systemc.h>

SC_MODULE(ProcessCompare) {
    sc_in<bool> clk;

    void my_method() {
        cout << "Method executed at " << sc_time_stamp() << endl;
        // Cannot use wait() here
    }

    void my_thread() {
        while(true) {
            cout << "Thread waiting at " << sc_time_stamp() << endl;
            wait(); // Waits for next clock edge
            cout << "Thread resumed at " << sc_time_stamp() << endl;
        }
    }

    SC_CTOR(ProcessCompare) {
        SC_METHOD(my_method); sensitive << clk.pos();
        SC_THREAD(my_thread); sensitive << clk.pos();
    }
};
int sc_main(int, char*[]) { return 0; }
