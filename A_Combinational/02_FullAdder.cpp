#include <systemc.h>

// A Full Adder handles 3 inputs (A, B, Carry-In).
// We implement it structurally using two Half Adders.
SC_MODULE(HalfAdder) {
    sc_in<bool> a, b;
    sc_out<bool> sum, carry;
    void process() { sum = a ^ b; carry = a & b; }
    SC_CTOR(HalfAdder) { SC_METHOD(process); sensitive << a << b; }
};

SC_MODULE(FullAdder) {
    sc_in<bool> a, b, cin;
    sc_out<bool> sum, cout;

    HalfAdder *ha1, *ha2;
    sc_signal<bool> s1, c1, c2;

    void process_or() {
        cout.write(c1.read() | c2.read());
    }

    SC_CTOR(FullAdder) {
        ha1 = new HalfAdder("ha1");
        ha1->a(a); ha1->b(b); ha1->sum(s1); ha1->carry(c1);

        ha2 = new HalfAdder("ha2");
        ha2->a(s1); ha2->b(cin); ha2->sum(sum); ha2->carry(c2);

        SC_METHOD(process_or);
        sensitive << c1 << c2;
    }
    ~FullAdder() { delete ha1; delete ha2; }
};

int sc_main(int argc, char* argv[]) {
    // Basic testbench setup...
    return 0;
}
