#include <systemc.h>

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
