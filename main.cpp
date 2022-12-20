#include <iostream>
#include <queue>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;

template<int N>
class PacketHandler {
    queue<string> q;
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
            if (q.empty()) {
                block_time = time(nullptr);
            }
            q.emplace(line);
            if (dyn_block_nesting == 0 && q.size() == N) {
                pop_print();
            }
        }
    }

    void pop_print() {
        if (!q.empty()) {
            ofstream file;
            file.open(to_string(block_time) + ".log");
            cout << "bulk: ";
            file << "bulk: ";
            while (!q.empty()) {
                cout << q.front();
                file << q.front();
                q.pop();
                if (!q.empty()) {
                    cout << ", ";
                    file << ", ";
                }
            }
            cout << endl;
            file << endl;
            file.close();
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