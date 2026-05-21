#include <systemc.h>

// A Half Adder is the simplest combinational circuit.
// It adds two 1-bit inputs and produces a sum and carry output.
SC_MODULE(HalfAdder) {
    sc_in<bool> a, b;
    sc_out<bool> sum, carry;

    void process() {
        // sum = a XOR b
        sum.write(a.read() ^ b.read());
        // carry = a AND b
        carry.write(a.read() & b.read());
    }

    SC_CTOR(HalfAdder) {
        SC_METHOD(process);
        sensitive << a << b; // Trigger on any input change
    }
};

int sc_main(int argc, char* argv[]) {
    sc_signal<bool> sig_a, sig_b, sig_sum, sig_carry;
    HalfAdder ha("ha");
    ha.a(sig_a); ha.b(sig_b); ha.sum(sig_sum); ha.carry(sig_carry);

    sc_start(0, SC_NS); // Initialize
    cout << "A B | Sum Carry\n";
    sig_a = 0; sig_b = 0; sc_start(1, SC_NS); cout << sig_a << " " << sig_b << " |  " << sig_sum << "    " << sig_carry << "\n";
    sig_a = 0; sig_b = 1; sc_start(1, SC_NS); cout << sig_a << " " << sig_b << " |  " << sig_sum << "    " << sig_carry << "\n";
    sig_a = 1; sig_b = 0; sc_start(1, SC_NS); cout << sig_a << " " << sig_b << " |  " << sig_sum << "    " << sig_carry << "\n";
    sig_a = 1; sig_b = 1; sc_start(1, SC_NS); cout << sig_a << " " << sig_b << " |  " << sig_sum << "    " << sig_carry << "\n";
    return 0;
}
