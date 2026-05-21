#include <systemc.h>

SC_MODULE(Producer) {
    sc_fifo_out<int> out;
    void run() { out.write(42); }
    SC_CTOR(Producer) { SC_THREAD(run); }
};

SC_MODULE(Consumer) {
    sc_fifo_in<int> in;
    void run() { int val = in.read(); }
    SC_CTOR(Consumer) { SC_THREAD(run); }
};

int sc_main(int, char*[]) {
    sc_fifo<int> channel(10); // FIFO of depth 10
    Producer p("p"); p.out(channel);
    Consumer c("c"); c.in(channel);
    return 0;
}
