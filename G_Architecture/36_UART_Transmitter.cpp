#include <systemc.h>

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
