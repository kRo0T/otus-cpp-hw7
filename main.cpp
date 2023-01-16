#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;
/*
class Observer {
public:
    virtual void update(const vector<int>& data) = 0;
};

class ConsoleObserver : public Observer {
public:
    void update(const vector<int>& data) {
        for (int i : data) {
            cout << i << " ";
        }
        cout << endl;
    }
};

class FileObserver : public Observer {
private:
    string fileName;
public:
    FileObserver(string name) : fileName(name) {}
    void update(const vector<int>& data) {
        ofstream file(fileName);
        for (int i : data) {
            file << i << " ";
        }
        file << endl;
    }
};

class Subject {
private:
    vector<int> data;
    vector<Observer*> observers;
public:
    void attach(Observer* observer) {
        observers.push_back(observer);
    }

    void setData(vector<int> d) {
        data = d;
        notify();
    }

    void notify() {
        for (Observer* observer : observers) {
            observer->update(data);
        }
    }
};

int main() {
    ConsoleObserver consoleObserver;
    FileObserver fileObserver("output.txt");
    Subject subject;
    subject.attach(&consoleObserver);
    subject.attach(&fileObserver);
    subject.setData({1, 2, 3});
    return 0;
}
*/
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