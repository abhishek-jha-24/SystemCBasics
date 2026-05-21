#include <systemc.h>

// Custom Interface
class write_if : virtual public sc_interface {
public: virtual void write(int) = 0;
};
class read_if : virtual public sc_interface {
public: virtual void read(int&) = 0;
};

// Custom Channel implementing interfaces
class CustomChannel : public sc_channel, public write_if, public read_if {
    int data;
public:
    CustomChannel(sc_module_name name) : sc_channel(name) {}
    void write(int d) { data = d; }
    void read(int& d) { d = data; }
};

int sc_main(int, char*[]) { return 0; }
