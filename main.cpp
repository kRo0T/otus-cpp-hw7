#include <iostream>
#include <queue>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;

template<int N>
class PacketHandler {
    queue<string> q;
    vector<string> bulk;
    int dyn_block_nesting = 0;
    std::time_t block_time;
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

    void print_to(ostream& stream) {
        stream << "bulk: ";
        for (int i = 0; i < bulk.size(); i++) {
            stream << bulk[i];
            if (i != bulk.size()-1) {
                stream << ", ";
            }
        }
        stream << endl;
    }

    void pop_print() {
        if (!bulk.empty()) {
            print_to(cout);
            ofstream file;
            file.open("bulk" + to_string(block_time) + ".log");
            print_to(file);
            file.close();
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
};

int main() {
    PacketHandler<3> ph;
    ph.process();
    return 0;
}