#include <systemc>

using namespace sc_core;

SC_MODULE(Test) {

    sc_signal<int> sig;

    void writer() {

        sig.write(10);

        std::cout << "After write: "
                  << sig.read()
                  << std::endl;
    }

    SC_CTOR(Test) {

        SC_METHOD(writer);
    }
};

int sc_main(int argc, char* argv[]) {

    Test t("test");

    sc_start();

    return 0;
}