#include <systemc.h>

SC_MODULE(DynamicSense) {
    sc_in<bool> a, b;

    void process() {
        cout << "Triggered!" << endl;
        if (a.read()) next_trigger(b.pos()); // Dynamically wait for b
        else          next_trigger(a.pos()); // Dynamically wait for a
    }

    SC_CTOR(DynamicSense) {
        SC_METHOD(process); // No static sensitivity list!
    }
};
int sc_main(int, char*[]) { return 0; }
