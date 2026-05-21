#include <systemc.h>

// Simple 3-Stage Pipeline: Fetch, Decode, Execute
SC_MODULE(Pipeline) {
    sc_in<bool> clk;

    // Pipeline registers
    int if_id_inst;
    int id_ex_inst;

    void fetch() {
        int pc = 0;
        while (true) {
            wait();
            if_id_inst = pc++; // Dummy instruction fetch
            cout << "Time: " << sc_time_stamp() << " | FETCH   inst: " << if_id_inst << endl;
        }
    }

    void decode() {
        while (true) {
            wait();
            id_ex_inst = if_id_inst;
            cout << "Time: " << sc_time_stamp() << " | DECODE  inst: " << id_ex_inst << endl;
        }
    }

    void execute() {
        while (true) {
            wait();
            cout << "Time: " << sc_time_stamp() << " | EXECUTE inst: " << id_ex_inst << endl;
            cout << "------------------------" << endl;
        }
    }

    SC_CTOR(Pipeline) {
        if_id_inst = -1; id_ex_inst = -1;
        SC_CTHREAD(fetch, clk.pos());
        SC_CTHREAD(decode, clk.pos());
        SC_CTHREAD(execute, clk.pos());
    }
};

int sc_main(int, char*[]) {
    sc_clock clk("clk", 10, SC_NS);
    Pipeline pipe("pipe");
    pipe.clk(clk);
    sc_start(50, SC_NS);
    return 0;
}
