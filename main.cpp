#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;

class IPrinter {
protected:
    void print_to(ostream& stream, const vector<string>& bulk) {
        stream << "bulk: ";
        for (int i = 0; i < bulk.size(); i++) {
            stream << bulk[i];
            if (i != bulk.size()-1) {
                stream << ", ";
            }
        }
        stream << endl;
    }
public:
    virtual void print(const vector<string>& bulk, time_t block_time) = 0;
};

class ConsolePrinter : public IPrinter {
public:
    void print(const vector<string>& bulk, time_t block_time) {
        print_to(cout, bulk);
    }
};

class FilePrinter : public IPrinter {
public:
    void print(const vector<string>& bulk, time_t block_time) {
        ofstream file;
        file.open("bulk" + to_string(block_time) + ".log");
        print_to(file, bulk);
        file.close();
    }
};

template<int N>
class PacketHandler {
    vector<string> bulk;
    int dyn_block_nesting = 0;
    std::time_t block_time;
    list<IPrinter*> printers;
public:
    PacketHandler() = default;
    void add_packet(string line) {
        if (line == "{") {
            dyn_block_nesting++;
            if (dyn_block_nesting == 1) {
                pop_print();
            }
        } else if (line == "}") {
            dyn_block_nesting--;
            if (dyn_block_nesting == 0) {
                pop_print();
            }
        } else {
            if (bulk.empty()) {
                block_time = time(nullptr);
            }
            bulk.push_back(line);
            if (dyn_block_nesting == 0 && bulk.size() == N) {
                pop_print();
            }
        }
    }

    void pop_print() {
        if (!bulk.empty()) {
            notify();
            bulk.clear();
        }
    }

    void process() {
        for (string line; getline(cin, line);) {
            add_packet(line);
        }
        if (dyn_block_nesting == 0) {
            pop_print();
        }
    }

    void attach(IPrinter* printer) {
        printers.push_back(printer);
    }

    void detach(IPrinter* printer) {
        printers.remove(printer);
    }

    void notify() {
        for (auto printer : printers) {
            printer->print(bulk, block_time);
        }
    }
};

int main() {
    PacketHandler<3> ph;
    ConsolePrinter cp;
    FilePrinter fp;
    ph.attach(&cp);
    ph.attach(&fp);
    ph.process();
    return 0;
}