#include <fstream>   // for parse txt data
#include <iomanip>   // for cout looks better
#include <iostream>  // cout, cin
#include <string>    // string
#include <utility>   // std pair
// #include <map>     // map
#include <vector>  // conatainer

using namespace std;

// a hole(via) with its x and y
class hole {
   public:
    hole(int hole_id_in = 0, int x_in = 0, int y_in = 0) : hole_id(hole_id_in), x(x_in), y(y_in){};
    ~hole(){};

    friend istream &operator>>(istream &is, hole &h) {
        is >> h.hole_id >> h.x >> h.y;
        return is;
    }

    friend ostream &operator<<(ostream &os, hole &h) {
        os << left << setw(12) << h.hole_id
           << setw(8) << h.x << setw(8) << h.y;
        return os;
    }

    int hole_id;
    int x;
    int y;
    // int layer;  // for double check
};

// a conflict edge that contains two vertices
class edge {
   public:
    edge(hole vi_in, hole vj_in) : vi(vi_in), vj(vj_in){};
    ~edge(){};
    hole vi, vj;
};

// to determine if two vertices is conflicted
bool isConflict(const hole &a, const hole &b) {
    return (!(abs(a.x - b.x) > 2 || abs(a.y - b.y) > 2));
}

// input
void checkInputFile(fstream &fin, int &argc, char **&argv) {
    if (argc != 3) {
        cout << "Usage: ./[exe] [input file] [output file]" << endl;
        exit(1);
    }

    fin.open(argv[1], fstream::in);
    if (!fin.is_open()) {
        cout << "Error: the input file is not opened!!" << endl;
        exit(1);
    }
}

// input
void parseInputFile(fstream &fin, int &grid_x, int &grid_y, int &numHoles, vector<hole> &allHoles) {
    cout << "======================" << endl;
    cout << "        Parser        " << endl;
    cout << "======================" << endl;
    string buffer;
    fin >> buffer;
    if (buffer != "layout") {
        cout << "buffer = " << buffer << endl;
        cerr << "Error: the input format is INCORRECT. (\"" << "layout" << "\"?)" << endl;
        exit(1);
    }
    fin >> grid_x >> grid_y;
    fin >> buffer;
    if (buffer != "num") {
        cout << "buffer = " << buffer << endl;
        cerr << "Error: the input format is INCORRECT. (\"" << "num holes" << "\"?)" << endl;
        exit(1);
    }
    fin >> buffer;
    if (buffer != "holes") {
        cout << "buffer = " << buffer << endl;
        cerr << "Error: the input format is INCORRECT. (\"" << "num holes" << "\"?)" << endl;
        exit(1);
    }
    fin >> numHoles;
    allHoles.reserve(numHoles);
    hole t;
    for (int i = 0; i < numHoles; ++i) {
        fin >> t;
        allHoles.push_back(t);
    }
}

int printMask(const IloNum &x1, const IloNum &x2) {
    int x_1 = static_cast<int>(x1);
    int x_2 = static_cast<int>(x2);
    if (x_1 == 0 && x_2 == 0) {
        return 1;
    } else if (x_1 == 0 && x_2 == 1) {
        return 2;
    } else if (x_1 == 1 && x_2 == 0) {
        return 3;
    } else {
        cerr << "unknown mask state" << endl;
        exit(1);
    }
}