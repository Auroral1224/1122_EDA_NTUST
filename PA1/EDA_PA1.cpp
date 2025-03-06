#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> // for "find()" in vector

using namespace std;

class Macro
{
private:
    string id;
    int x1;
    int y1;
    int x2;
    int y2;

public:
    string getID(void) { return id; }
    void setID(string x) { id = x; }
    int getX1(void) { return x1; }
    void setX1(int val) { x1 = val; }
    int getX2(void) { return x2; }
    void setX2(int val) { x2 = val; }
    int getY1(void) { return y1; }
    void setY1(int val) { y1 = val; }
    int getY2(void) { return y2; }
    void setY2(int val) { y2 = val; }
};

int bound_x, bound_y, macro_num;

int main(int argc, char **argv)
{
    /*
        open the input file
    */
    if (argc != 3)
    {
        cout << "Usage: ./[exe] [input file] [output file]" << endl;
        exit(1);
    }

    fstream fin;
    fin.open(argv[1], fstream::in);
    if (!fin.is_open())
    {
        cout << "Error: the input file is not opened!!" << endl;
        exit(1);
    }

    /*
        parse the input file
    */
    cout << "======================" << endl;
    cout << "        Parser        " << endl;
    cout << "======================" << endl;

    vector<Macro> macro_all;

    string buffer; // just a buffer for input
    fin >> buffer;
    if (buffer != "Boundary")
    {
        cerr << "Error: the input format is INCORRECT. (\"Boundary\"?)" << endl;
        exit(1);
    }
    fin >> buffer;
    bound_x = stoi(buffer);
    fin >> buffer;
    bound_y = stoi(buffer);

    fin >> buffer;
    if (buffer != "Macro")
    {
        cerr << "Error: the input format is INCORRECT. (\"Macro\"?)" << endl;
        exit(1);
    }
    fin >> buffer;
    macro_num = stoi(buffer);
    for (int i = 0; i < macro_num; ++i)
    {
        Macro t;
        fin >> buffer;
        t.setID(buffer);
        fin >> buffer;
        t.setX1(stoi(buffer));
        fin >> buffer;
        t.setY1(stoi(buffer));
        fin >> buffer;
        t.setX2(stoi(buffer));
        fin >> buffer;
        t.setY2(stoi(buffer));
        macro_all.push_back(t);
    }

    /*
        open the output file
    */
    fstream fout;
    fout.open(argv[2], fstream::out);
    if (!fout.is_open())
    {
        cout << "Error: the output file is not opened!!" << endl;
        exit(1);
    }

    /*
        output the GNUplot result
    */
    fout << "reset" << endl;
    // fout << "set size ratio -1" << endl;
    fout << "set title \"result\"" << endl;
    fout << "set xlabel \"X\"" << endl;
    fout << "set ylabel \"Y\"" << endl;
    for (int i = 0; i < macro_num; ++i)
    {
        int center_x = (macro_all[i].getX1() + macro_all[i].getX2()) / 2;
        int center_y = (macro_all[i].getY1() + macro_all[i].getY2()) / 2;
        fout << "set object " << i + 1 << " rect from " << macro_all[i].getX1() << ", " << macro_all[i].getY1() << " to " << macro_all[i].getX2() << ", " << macro_all[i].getY2() << endl;
        fout << "set label \"" << macro_all[i].getID() << "\" at " << center_x << ", " << center_y << " center" << endl;
    }
    int tic_x = bound_x / 5;
    int tic_y = bound_y / 5;
    fout << "set xtics " << tic_x << endl;
    fout << "set ytics " << tic_y << endl;
    fout << "plot [0:" << bound_x << "][0:" << bound_y << "]0" << endl;
    fout << "set terminal png size 1024, 768" << endl;
    fout << "set output \"output.png\"" << endl;
    fout << "replot" << endl;
    cout << "Finished, the output file's name is \"" << argv[2] << "\", press any key to exit..." << endl;
    cin.get();
    return 0;
}