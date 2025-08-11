#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <limits>

using namespace std;

class FileManager {
public:
    // Overwrite (write) using simple ofstream per line
    static void writeStandard(const string& filename, const vector<string>& lines) {
        ofstream ofs(filename, ios::trunc);
        for (const auto& line : lines) {
            ofs << line << '\n';
        }
    }

    // Overwrite (write) using a single buffered write call
    static void writeBuffered(const string& filename, const vector<string>& lines) {
        ofstream ofs(filename, ios::trunc);
        string buffer;
        buffer.reserve(lines.size() * 50);  // rough pre-reserve
        for (const auto& line : lines) {
            buffer += line;
            buffer += '\n';
        }
        ofs << buffer;
    }

    // Append lines to the file
    static void append(const string& filename, const vector<string>& lines) {
        ofstream ofs(filename, ios::app);
        for (const auto& line : lines) {
            ofs << line << '\n';
        }
    }

    // Read entire file to stdout
    static void readAll(const string& filename) {
        ifstream ifs(filename);
        if (!ifs) {
            cerr << "Failed to open " << filename << " for reading.\n";
            return;
        }
        string line;
        while (getline(ifs, line)) {
            cout << line << '\n';
        }
    }

    // Utility to measure time taken by a function
    template<typename Func>
    static double timeFunction(Func f) {
        auto t0 = chrono::high_resolution_clock::now();
        f();
        auto t1 = chrono::high_resolution_clock::now();
        chrono::duration<double> diff = t1 - t0;
        return diff.count();
    }
};

int main() {
    string filename;
    int choice;
    cout << "Simple C++ File Manager\n";
    cout << "Enter target file name: ";
    getline(cin, filename);

    do {
        cout << "\nMenu:\n"
             << "1. Read file\n"
             << "2. Write file (overwrite) with performance demo\n"
             << "3. Append to file\n"
             << "0. Exit\n"
             << "Choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            cout << "\n--- File Contents ---\n";
            FileManager::readAll(filename);
            cout << "--- End of File ---\n";
            break;

        case 2: {
            int n;
            cout << "How many lines do you want to write? ";
            cin >> n;
            cin.ignore();
            vector<string> lines;
            lines.reserve(n);
            for (int i = 1; i <= n; ++i) {
                lines.push_back("This is line number " + to_string(i));
            }

            double t_standard = FileManager::timeFunction([&]() {
                FileManager::writeStandard(filename, lines);
            });
            cout << "Standard write took " << t_standard << " seconds.\n";

            double t_buffered = FileManager::timeFunction([&]() {
                FileManager::writeBuffered(filename, lines);
            });
            cout << "Buffered write took " << t_buffered << " seconds.\n";

            cout << "Performance gain: "
                 << (t_standard / t_buffered)
                 << "× faster with buffering (higher is better)\n";
            break;
        }

        case 3: {
            int m;
            cout << "How many lines to append? ";
            cin >> m;
            cin.ignore();
            vector<string> lines;
            lines.reserve(m);
            for (int i = 1; i <= m; ++i) {
                lines.push_back("Appended line " + to_string(i));
            }
            FileManager::append(filename, lines);
            cout << m << " lines appended to " << filename << ".\n";
            break;
        }

        case 0:
            cout << "Exiting.\n";
            break;

        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
