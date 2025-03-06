#ifndef SLICING_TREE_H
#define SLICING_TREE_H
#include <iostream>  // cin / cout
#include <fstream>   // parse data and output result to .txt
#include <string>    // strings
#include <sstream>   // for parsing test tree
#include <vector>    // containers
#include <algorithm> // for "sort()" in vector
#include <random>    // for random perturbation
#include <iomanip>   // for cout
#include <stack>     // for cost evaluation
#include <chrono>    // for rand seed, timer
#include <stdexcept> // for ctrl+C during SA
#include <csignal>   // for ctrl+C during SA
#include <unistd.h>  // for ctrl+C during SA
using namespace std;

/* little enums */
enum init_mode // initial mode enumarate
{
    V,
    H,
    VH_alt
};
enum output_format // output format enumarate
{
    GNUPlot,
    PA2
};

/* for output format (GNUPlot or PA2 output format) */
#define OUTPUT_FORMAT output_format::GNUPlot

/* for ctrl+c during SA */
volatile sig_atomic_t ctrl_c_signal = 0;

/* for timeout (10 min) */
#define TIMEOUT 600.0
bool timeout_flag = false;

/* for random (mt19937) */
#define SEED chrono::high_resolution_clock::now().time_since_epoch().count()

/*
*********************************************************************
    the parameters below are for SA.
    In SA, I've chosen "cost()" to be the the bigger, the better.
*********************************************************************
*/
/* for cost function, area_cost + ar_weight * ar_cost = 1.0, cost max = 1 */
#define AREA_WEIGHT 1
#define AR_WEIGHT -0.3
#define INIT_TEMP 3000.0
/* for prob, bigger for lower probability (prob = e^(x * delta)/temp) */
#define DELTA_MULTIPLIER 6000.0
/* for initial temp function, NG */
// #define TIMES_DURING_INITTEMP 40
// #define P 0.99
/* multiplier for #pertub in a fixed temp, #pertub = x * expression.size() */
#define TIMES_PER_TEMP 10
/* new temp coef, T = rT */
#define TEMP_REDUCE_RATE 0.9
/* SA stops when temp = frozen */
#define FROZEN_TEMP 1.0

template <typename T>
inline T parseInfo(fstream &fin, string name)
{
    string buffer;
    fin >> buffer;
    if (buffer != name)
    {
        cout << "buffer = " << buffer << endl;
        cerr << "Error: the input format is INCORRECT. (\"" << name << "\"?)" << endl;
        exit(1);
    }
    T t;
    fin >> t;
    return t;
}

void ifPartition(const int &numBlocks, bool &isPartition, int &partition_amount, double &solution_quality)
{
    if (numBlocks <= 25)
    {
        isPartition = false;
        solution_quality = 0.97;
    }
    else if (numBlocks < 100)
    {
        isPartition = true;
        partition_amount = 4;
        solution_quality = 0.95;
    }
    else if (numBlocks < 225)
    {
        isPartition = true;
        partition_amount = 9;
        solution_quality = 0.95;
    }
    else if (numBlocks < 400)
    {
        isPartition = true;
        partition_amount = 16;
        solution_quality = 0.95;
    }
    // else if (numBlocks <= 625)
    else
    {
        isPartition = true;
        partition_amount = 25;
        solution_quality = 0.95;
    }
}

class Macro
{
private:
    string name;
    int width;
    int height;
    int x;
    int y;

public:
    Macro(string n = "", int w = 0, int h = 0) : name(n), width(w), height(h), x(0), y(0){};
    ~Macro(){};
    int getWidth() const { return width; };
    int getHeight() const { return height; };
    long long area() const { return 1LL * width * height; };
    bool operator>(const Macro &rhs) const
    {
        return this->area() > rhs.area();
    }

    // operator overloading for class Macro ">>"
    friend istream &operator>>(istream &is, Macro &m);

    // operator overloading for class Macro "<<"
    friend ostream &operator<<(ostream &os, Macro &m);

    friend class allMacros;
    friend class Expression;
};

class allMacros
{
private:
    vector<Macro> macros;
    // long long area;
    int numBlocks;
    double minAR;
    double maxAR;

public:
    friend class Expression;
    allMacros(){};
    allMacros(int n, double i, double x) : numBlocks(n), minAR(i), maxAR(x){};
    int size() const { return macros.size(); };
    void parseData(fstream &fin);
    friend ostream &operator<<(ostream &os, allMacros &m);
};

class Expression
{
private:
    vector<string> e;
    long long area;
    bool isBalloting() const;
    bool isSkewed() const;
    bool isOperator(const string &x) const { return (x == "H" || x == "V"); };
    // long long partitionMacroArea(const allMacros &A) const;
    Macro mergeBlocks(const Macro &a, const Macro &b, const string &op) const;
    vector<Macro *> parseMacros(allMacros &A) const;
    Expression changeMacroXY(const Expression &a, const Expression &b, const string &op, allMacros &A) const;
    Macro evaluateMacroDim(const allMacros &A) const;
    void M1();             // operands swap
    void M2();             // invert chain
    void M3();             // operand/operator swap
    void M4(allMacros &A); // rotate
    void M5(allMacros &A); // aspect ratio change
    void outputGNUPlot(const allMacros &A, fstream &fs) const;
    void outputPA2(const allMacros &A, fstream &fs) const;

public:
    Expression() : area(0){};
    Expression(const string &);
    Expression(const allMacros &m, const int &mode);
    ~Expression(){};
    int size() const { return e.size(); };
    double cost(const allMacros &A) const;
    vector<Expression> &partition(const allMacros &A, const int &divisor, const int &INIT_MODE, const bool &isPartition) const;
    void combinePartition(const vector<Expression> &EE, const int &partition_amount);
    void perturbation(allMacros &A, const int &mode);
    void traceXY(allMacros &A) const;
    // double initTemp(const allMacros &A) const;
    bool simulatedAnnealing(allMacros &A, bool &isPartition, double &solution_quality);
    void printSpec(const allMacros &A) const;
    void outputData(const allMacros &A, fstream &fs, const output_format &of) const;
    Expression operator+(const Expression &b) const;
    friend ostream &operator<<(ostream &os, const Expression &s);
};

// // Timer class, ref: https://www.learncpp.com/cpp-tutorial/8-16-timing-your-code/
class Timer
{
private:
    using clock_type = chrono::high_resolution_clock;
    using second_type = chrono::duration<double, chrono::seconds::period>;

    chrono::time_point<clock_type> m_startTime;

public:
    Timer() : m_startTime(clock_type::now()) {}
    double reset()
    {
        double dt = getElapsed();
        m_startTime = clock_type::now();
        return dt;
    }
    double getElapsed() const { return chrono::duration_cast<second_type>(clock_type::now() - m_startTime).count(); }
};

istream &operator>>(istream &is, Macro &m)
{
    is >> m.name >> m.width >> m.height;
    // m.name.replace(m.name.begin(), m.name.begin() + 6, "");
    return is;
}

ostream &operator<<(ostream &os, Macro &m)
{
    os << left << setw(12) << m.name << setw(8) << m.width << setw(8) << m.height << setw(8) << m.x << setw(8) << m.y;
    return os;
}

ostream &operator<<(ostream &os, allMacros &m)
{
    for (int i = 0; i < m.size(); ++i)
    {
        cout << m.macros[i] << endl;
    }
    return os;
}

ostream &operator<<(ostream &os, const Expression &s)
{
    for (int i = 0; i < s.size(); ++i)
    {
        os << left << setw(4) << s.e[i];
    }
    return os;
}

void allMacros::parseData(fstream &fin)
{
    // macros.reserve(numBlocks * 2 - 1);
    for (int i = 0; i < numBlocks; ++i)
    {
        Macro t;
        fin >> t;
        macros.push_back(t);
    }
}

// test, it's only valid for macro 0~9, and ops like 'H', 'V'
inline Expression::Expression(const string &x) : area(0)
{
    char a;
    stringstream ss(x);
    while (ss >> a)
    {
        string z(1, a);
        e.push_back(z);
    }
}

Expression::Expression(const allMacros &A, const int &mode)
{
    this->area = 0;
    for (int i = 0; i < A.size(); ++i)
    {
        // e.push_back(to_string(i));
        string id = A.macros[i].name;
        id.replace(id.begin(), id.begin() + 6, ""); // replace "block_"
        e.push_back(id);
        this->area += A.macros[i].area();
        if (i)
        {
            switch (mode)
            {
            case 0:
                e.push_back("V");
                break;
            case 1:
                e.push_back("H");
                break;
            case 2:
                if (i % 2)
                    e.push_back("V");
                else
                    e.push_back("H");
            case 3:

            default:
                break;
            }
        }
    }
}

Expression Expression::operator+(const Expression &b) const
{
    Expression c = *this;
    c.e.insert(c.e.end(), b.e.begin(), b.e.end());
    c.area += this->area;
    return c;
    // Expression *c = new Expression;
    // *c = *this;
    // c->e.insert(c->e.end(), b.e.begin(), b.e.end());
    // c->area += this->area;
    // return *c;
}

inline bool Expression::isBalloting() const
{
    int op_count = 0, num_count = 0;
    for (int i = 0; i < e.size(); ++i)
    {
        if (isOperator(e[i]))
        {
            op_count++;
        }
        else
        {
            num_count++;
        }
        if (num_count <= op_count)
            return false;
    }
    return true;
}

inline bool Expression::isSkewed() const
{
    for (int i = 0; i < e.size() - 1; ++i)
    {
        if (isOperator(e[i]) && e[i] == e[i + 1])
        {
            return false;
        }
    }
    return true;
}

// inline long long Expression::partitionMacroArea(const allMacros &A) const
// {
//     long long area = 0;
//     for (int i = 0; i < this->size(); ++i)
//     {
//         if(isOperator(this->e[i])) continue;
//         else
//         {
//             area += A.macros[stoi(e[i])].area();
//         }
//     }
//     return area;
// }

inline Macro Expression::mergeBlocks(const Macro &a, const Macro &b, const string &op) const
{
    Macro m = a;
    if (op == "H")
    {
        m.width = max(a.width, b.width);
        m.height = a.height + b.height;
    }
    else
    {
        m.width = a.width + b.width;
        m.height = max(a.height, b.height);
    }
    // cout << m << endl;
    return m;
}

inline Macro Expression::evaluateMacroDim(const allMacros &A) const
{
    stack<Macro> s;
    Macro a, b;
    for (int i = 0; i < size(); ++i)
    {
        if (isOperator(e[i]))
        {
            b = s.top();
            s.pop();
            a = s.top();
            s.pop();
            // cout << "merge " << a << ", " << b << "!" << endl;
            // macro a, macro b, operator ith expression
            a = mergeBlocks(a, b, e[i]);
            // cout << a << endl;
            s.push(a);
        }
        else // operand
        {
            // cout << "push " << e[i] << "!" << endl;
            s.push(A.macros[stoi(e[i])]);
        }
    }

    if (s.size() != 1)
    {
        cerr << "macro evaluate failed." << endl;
        exit(1);
    }
    a = s.top();
    s.pop(); // The result should be the last item on the stack
    return a;
}

inline double Expression::cost(const allMacros &A) const
{
    if (!size())
        return 0;
    // cout << "calculating cost for " << *this << endl;
    Macro a = evaluateMacroDim(A);
    double AR_cost = double(a.height) / double(a.width); // higher better, max 1
    AR_cost = AR_cost > 1 ? (1 / AR_cost) : AR_cost;
    AR_cost = 1 - AR_cost;
    double usage_cost = double(this->area) / double(a.height * a.width); // higher better, max 1
    double cost = AREA_WEIGHT * usage_cost + AR_WEIGHT * AR_cost;
    return cost;
}

inline void Expression::traceXY(allMacros &A) const
{
    stack<Expression> s;
    Expression a, b;
    // cout << "tracing XY!" << endl;
    for (int i = 0; i < size(); ++i)
    {
        if (isOperator(e[i]))
        {
            // cout << i + 1 << " th token = " << e[i] << ", is operator, tracing..." << endl;
            // cout << "stack size before pop two : " << s.size() << endl;
            b = s.top();
            s.pop();
            a = s.top();
            s.pop();
            // expression a, expression b, operator ith expression, all macro data for changing XY
            Expression t = changeMacroXY(a, b, e[i], A);
            // cout << a << endl;
            s.push(t);
        }
        else // operand
        {
            // cout << i + 1 << " th token = " << e[i] << ", is operand, pushing back..." << endl;
            // cout << "stack size before pushback : " << s.size() << endl;
            Expression t;
            t.e.push_back(e[i]);
            s.push(t);
        }
    }

    if (s.size() != 1)
    {
        cerr << "trace XY for expression \"" << *this << "\" failed." << endl;
        exit(1);
    }
}

inline vector<Macro *> Expression::parseMacros(allMacros &A) const
{
    vector<Macro *> t;
    for (int i = 0; i < size(); ++i)
    {
        if (!isOperator(e[i]))
        {
            t.push_back(&A.macros[stoi(e[i])]);
        }
    }

    // cout << "after parse macros from expression :" << *this << endl;
    // cout << "macros = " << endl;
    // for (int i = 0; i < t.size(); ++i)
    // {
    //     cout << left << setw(4) << *t[i];
    // }
    // cout << endl;

    return t;
}

inline Expression Expression::changeMacroXY(const Expression &a, const Expression &b, const string &op, allMacros &A) const
{
    Expression connected_e, op_e(op);
    vector<Macro *> to_changes;
    to_changes = b.parseMacros(A);
    Macro movement;
    // cout << "a = " << a << endl;
    if (a.size() > 1) // if expression a looks like <0, 2, V>, we need parse macros 0 and 2
        movement = a.evaluateMacroDim(A);
    else // else, directly parse 1st element of expression
        movement = A.macros[stoi(a.e[0])];
    for (int i = 0; i < to_changes.size(); ++i)
    {
        if (op == "H")
            to_changes[i]->y += movement.height;
        else
            to_changes[i]->x += movement.width;
    }

    connected_e = a + b + op_e;
    return connected_e;
}

inline vector<Expression> &Expression::partition(const allMacros &A, const int &divisor, const int &INIT_MODE, const bool &isPartition) const
{
    static vector<Expression> E_partition;
    if (!isPartition)
    {
        return E_partition;
    }
    vector<allMacros> result(divisor);
    allMacros A_copy = A;
    sort(A_copy.macros.begin(), A_copy.macros.end(), greater<Macro>());
    vector<int> partitionAreas(divisor, 0);
    for (int i = 0; i < A_copy.numBlocks; ++i)
    {
        // Greedy: find partition that has the smallest area now
        vector<int>::iterator it = min_element(partitionAreas.begin(), partitionAreas.end());
        int index = distance(partitionAreas.begin(), it);

        // push the biggest macro into this partition
        result[index].macros.push_back(A_copy.macros[i]);

        // update the area of this partition
        partitionAreas[index] += A_copy.macros[i].area();
    }

    for (int i = 0; i < result.size(); ++i)
    {
        Expression t(result[i], INIT_MODE);
        E_partition.push_back(t);
    }
    return E_partition;
}

// /* cool merge vector function from stackOverflow. */
// // REF: https://stackoverflow.com/questions/54229010/is-there-a-way-to-concatenate-multiple-vectors-simply
// template <typename T>
// vector<T> &concat(const initializer_list<vector<T>> &vectors)
// {
//     static vector<T> result;

//     int totalLength = 0;
//     for (const auto &vec : vectors)
//     {
//         totalLength += vec.size();
//     }
//     result.reserve(totalLength);

//     for (const auto &vec : vectors)
//     {
//         result.insert(result.end(), vec.begin(), vec.end());
//     }
//     return result;
// }

inline void Expression::combinePartition(const vector<Expression> &EE, const int &partition_amount)
{
    // const vector<string> op_H(1, "H"), op_V(1, "V");
    Expression op_H("H"), op_V("V"), c;
    switch (partition_amount)
    {
    case 4:
        c = EE[0] + EE[1] + op_H + EE[2] + EE[3] + op_H + op_V;
        this->e = c.e;
        // this->e = concat({EE[0].e, EE[1].e, op_H, EE[2].e, EE[3].e, op_H, op_V});
        break;
    case 9:
        c = EE[0] + EE[1] + op_H + EE[2] + EE[3] + op_H + op_V +
            EE[4] + EE[5] + op_H + op_V + EE[6] + EE[7] + op_V +
            EE[8] + op_V + op_H;
        this->e = c.e;
        // this->e = concat({EE[0].e, EE[1].e, op_H, EE[2].e, EE[3].e, op_H, op_V,
        //                   EE[4].e, EE[5].e, op_H, op_V, EE[6].e, EE[7].e, op_V,
        //                   EE[8].e, op_V, op_H});
        break;
    case 16:
        c = EE[0] + EE[1] + op_H + EE[2] + EE[3] + op_H + op_V +
            EE[4] + EE[5] + op_H + op_V + EE[6] + EE[7] + op_V +
            EE[8] + op_V + op_H + EE[9] + EE[10] + op_H + EE[11] +
            op_H + op_V + EE[12] + EE[13] + op_V + EE[14] + op_V +
            EE[15] + op_V + op_H;
        this->e = c.e;
        // this->e = concat({EE[0].e, EE[1].e, op_H, EE[2].e, EE[3].e, op_H, op_V,
        //                   EE[4].e, EE[5].e, op_H, op_V, EE[6].e, EE[7].e, op_V,
        //                   EE[8].e, op_V, op_H, EE[9].e, EE[10].e, op_H, EE[11].e,
        //                   op_H, op_V, EE[12].e, EE[13].e, op_V, EE[14].e, op_V,
        //                   EE[15].e, op_V, op_H});
        break;
    case 25:
        c = EE[0] + EE[1] + op_H + EE[2] + EE[3] + op_H + op_V +
            EE[4] + EE[5] + op_H + op_V + EE[6] + EE[7] + op_V +
            EE[8] + op_V + op_H + EE[9] + EE[10] + op_H + EE[11] +
            op_H + op_V + EE[12] + EE[13] + op_V + EE[14] + op_V +
            EE[15] + op_V + op_H + EE[16] + EE[17] + op_H + EE[18] +
            op_H + EE[19] + op_H + op_V + EE[20] + EE[21] + op_V +
            EE[22] + op_V + EE[23] + op_V + EE[24] + op_V + op_H;
        this->e = c.e;
        break;
    default:
        cerr << "partition amount currently only support 4, 9, 16, 25." << endl;
        exit(1);
        break;
    }
}

inline void Expression::perturbation(allMacros &A, const int &mode)
{
    switch (mode)
    {
    case 1: // M1
        M1();
        break;
    case 2: // M2
        M2();
        break;
    case 3: // M3
        M3();
        break;
    case 4:
        M4(A); // rotate
        break;
    case 5:
        M5(A); // aspect ratio change
        break;
    default:
        cerr << "no such perturbation. (case = " << mode << ")" << endl;
        exit(1);
    }
}

inline void Expression::M1()
{
    // cout << "M1" << endl;
    mt19937 mt(SEED);
    bool ok_flag = false;
    while (!ok_flag)
    {
        int index = mt() % (e.size());
        if (isOperator(e[index]))
            continue;
        vector<string>::iterator it = e.begin() + index + 1;
        for (; it != e.end(); ++it)
        {
            if (isOperator(*it))
                continue;
            else
            {
                // cout << "to exchange: " << e[index] << ", " << (*it) << endl;
                string temp;
                temp = e[index];
                e[index] = (*it);
                (*it) = temp;
                ok_flag = true;
                break;
            }
        }
    }
}

inline void Expression::M2()
{
    Expression e_init = *this;
    // cout << "M2" << endl;
    // cout << "before M2:" << *this << endl;
    bool ok_flag = false;
    mt19937 mt(SEED);
    // RNG rng(0, e.size() - 1);
    while (!ok_flag)
    {
        int index = mt() % (e.size());
        if (!isOperator(e[index]))
            continue;
        vector<string>::iterator it1 = e.begin() + index; // include itself
        vector<string>::iterator it2 = e.begin() + index - 1;
        while (it1 != e.end())
        {
            if (!isOperator(*it1))
                break;
            // cout << "change +" << endl;
            (*it1++) = (*it1) == "H" ? "V" : "H";
        }
        // cout << "change!" << endl;
        // while (it2 != e.begin() - 1)
        while (1)
        {
            if (!isOperator(*it2))
                break;
            // cout << "change -" << endl;
            if (it2 != e.begin())
                (*it2--) = (*it2) == "H" ? "V" : "H";
            else
                break;
        }
        if (isSkewed())
        {
            ok_flag = true;
        }
        else
        {
            *this = e_init;
        }
    }
    // cout << "after  M2:" << *this << endl;
}

inline void Expression::M3()
{
    Expression e_init = *this;
    // cout << "M3" << endl;
    mt19937 mt(SEED);
    // RNG rng(0, e.size() - 1);
    while (1)
    {
        while (1)
        {
            int index = mt() % e.size();
            // cout << "index = " << index << ", token = " << e[index] << endl;

            if (isOperator(e[index]))
            {
                // cout << "token is op!" << endl;
                // forward check
                if (index != e.size() - 1)
                {
                    // string info = isOperator(e[index + 1]) ? ", isOP" : ", notOP";
                    // cout << "next token = " << e[index + 1] << info << endl;
                    if (!isOperator(e[index + 1]))
                    {
                        // cout << "changing" << endl;
                        string temp;
                        temp = e[index];
                        e[index] = e[index + 1];
                        e[index + 1] = temp;
                        break;
                    }
                }

                // backward check
                if (index != 0)
                {
                    // string info = isOperator(e[index - 1]) ? ", isOP" : ", notOP";
                    // cout << "prev token = " << e[index - 1] << info << endl;
                    if (!isOperator(e[index - 1]))
                    {
                        // cout << "changing" << endl;
                        string temp;
                        temp = e[index];
                        e[index] = e[index - 1];
                        e[index - 1] = temp;
                        break;
                    }
                }
                // continue;
            }
            else
            {
                // cout << "token is macro!" << endl;
                // forward check
                if (index != e.size() - 1)
                {
                    // string info = isOperator(e[index + 1]) ? ", isOP" : ", notOP";
                    // cout << "next token = " << e[index + 1] << info << endl;

                    if (isOperator(e[index + 1]))
                    {
                        // cout << "changing" << endl;
                        string temp;
                        temp = e[index];
                        e[index] = e[index + 1];
                        e[index + 1] = temp;
                        break;
                    }
                }

                // backward check
                if (index != 0)
                {
                    // string info = isOperator(e[index - 1]) ? ", isOP" : ", notOP";
                    // cout << "prev token = " << e[index - 1] << info << endl;
                    if (isOperator(e[index - 1]))
                    {
                        // cout << "changing" << endl;
                        string temp;
                        temp = e[index];
                        e[index] = e[index - 1];
                        e[index - 1] = temp;
                        break;
                    }
                }
                // continue;
            }
        }
        // cout << "M3 performed : " << *this << endl;
        // cout << "check balloting property" << endl;
        if (isBalloting())
        {
            if (isSkewed())
            {
                break;
            }
        }
        else
        {
            *this = e_init;
        }
    }
}

void Expression::M4(allMacros &A)
{
    // cout << "before m4:" << endl;
    // cout << A << endl;
    mt19937 mt(SEED);
    int index;
    do
    {
        index = mt() % this->size();
    } while (isOperator(e[index]));
    Macro &a = A.macros[stoi(e[index])];
    int temp = a.height;
    a.height = a.width;
    a.width = temp;

    // cout << "after m4:" << endl;
    // cout << A << endl;
}

inline void Expression::M5(allMacros &A)
{
    Expression e_init = *this;
    // cout << "M5" << endl;
    mt19937 mt(SEED);
    int index, area, AR;
    do
    {
        index = mt() % this->size();
    } while (isOperator(e[index]));
    Macro &a = A.macros[stoi(e[index])];
    // cout << "before M5:" << a;
    area = a.width * a.height;
    while (1)
    {
        int ub_width = sqrt(double(area) / double(A.minAR));
        int lb_width = ceil(sqrt(double(area) / double(A.maxAR))); // e.g. Area = 4, width lower bound = ceil(sqrt2) = 2
        if (ub_width == lb_width)                                  // exception: it can't be shaped due to restriction
            break;
        int rand_width = (mt() % (ub_width - lb_width + 1)) + lb_width;
        // double rand_AR = double(rand_height) / double(rand_width);
        // if(rand_AR >= A.minAR && rand_AR <= A.maxAR && rand_area == area)
        if (!(area % rand_width))
        {
            a.width = rand_width;
            a.height = area / rand_width;
            break;
        }
    }
    // cout << "after M5 :" << a;
}

// inline double Expression::initTemp(const allMacros &A) const
// {
//     mt19937 mt(SEED);
//     Expression initExpression = *this;
//     allMacros initMacros = A;
//     int count = 0;
//     double oldCost = initExpression.cost(initMacros, 0);
//     double newCost, absDelta;
//     while (count++ < TIMES_DURING_INITTEMP)
//     {
//         initExpression.perturbation(initMacros, mt() % 5 + 1);
//         newCost = initExpression.cost(initMacros, 0);
//         // cout << "old cost = " << oldCost <<", new cost = " << newCost << endl;
//         absDelta += abs(newCost - oldCost);
//         oldCost = newCost;
//     }
//     double avgDelta = absDelta * DELTA_MULTIPLIER / TIMES_DURING_INITTEMP;

//     double T0 = P < 1 ? (-1 * avgDelta) / log(P) : avgDelta / log(P);
//     return T0;
// }
void ctrl_c_handler(sig_atomic_t s)
{
    ctrl_c_signal = s;
}
inline bool Expression::simulatedAnnealing(allMacros &A, bool &isPartition, double &solution_quality)
{
    signal(SIGINT, ctrl_c_handler); // ctrl + c
    static Timer timer;             // 10 min
    mt19937 mt(SEED);
    Expression best = *this, init = *this;
    allMacros A_best = A, A_init = A;
redo:
    Expression solution = *this;
    allMacros A_sol = A;
    Expression neighbor = *this;
    allMacros A_nei = A;

    double temp = INIT_TEMP;
    // double temp = initTemp(A);
    // cout << "temp = " << temp << endl;
    // cin.get();
    // int total_count = 0;
    // int loop_count = 1; // prevent 0/0
    // int accepted = 0;
    // int rejected = 0;
    // exit(1);
    // while (temp > FROZEN_TEMP && (double(rejected) / double(loop_count) <= 0.95))
    while (temp > FROZEN_TEMP)
    {
        // loop_count = 0;
        // accepted = 0;
        // rejected = 0;
        for (int i = 0; i < TIMES_PER_TEMP * A.size(); ++i)
        {
            int mode = mt() % 5 + 1;
            neighbor.perturbation(A_nei, mode);
            // total_count++;
            // loop_count++;
            double cost_n = neighbor.cost(A_nei);
            double cost_s = solution.cost(A_sol);
            double delta = cost_n - cost_s;
            // cout << "neighbor = " << neighbor << ", mode =" << mode << ", neighbor cost = " << cost_n << ", solution cost = " << cost_s << endl;
            // cin.get();
            delta *= DELTA_MULTIPLIER;
            if (delta >= 0)
            {
                solution = neighbor;
                A_sol = A_nei;
                // if (this->cost(A) >= solution.cost(A))
                double cost_b = this->cost(A);

                // cout << "best cost = " << cost_b << ", sol cost = " << cost_s << endl;
                // cout << "current best = " << *this << "\nsol          = " << solution << endl;
                if (cost_b <= cost_s)
                {
                    *this = solution;
                    A = A_sol;
                    // cout << "NEW best     = " << *this << endl;
                }
            }
            else
            {
                double prob = exp(delta / temp);
                double rng = double(mt() % 10000) / 10000.0;
                // cout << "\n***************probability:" << prob << ", rng = " << rng << ", delta = " << delta << endl;
                // cout << "\n***************probability:" << prob << ", temp = " << temp << ", delta = " << delta << endl;
                if (rng < prob)
                {
                    // cout << "climb up accepted" << endl;
                    // accepted++;
                    solution = neighbor;
                    A_sol = A_nei;
                }
                else
                {
                    // cout << "rejected" << endl;
                    // rejected++;
                    neighbor = solution;
                    A_nei = A_sol;
                }
            }
            // cin.get();
        }
        if (ctrl_c_signal || timer.getElapsed() >= TIMEOUT)
        {
            *this = best;
            A = A_best;
            if (ctrl_c_signal)
                throw runtime_error("ctrl + c during SA");
            else
            {
                timeout_flag = true;
                throw length_error("10 minutes exceeded");
            }
        }
        temp = TEMP_REDUCE_RATE * temp;
    }

    if (this->cost(A) <= solution_quality)
    {

        if (isPartition)
        {
            return false;
        }

        if (best.cost(A_best) < this->cost(A))
        {
            cout << "NG, but updating current best, spec: " << endl;
            best = *this;
            A_best = A;
        }
        else
        {
            cout << "NG, spec:\n";
        }
        this->printSpec(A);
        cout << "retry..." << endl;
        cout << endl;

        // int retry_pertub = A.size() / 2;
        // while (retry_pertub--)
        //     this->perturbation(A, mt() % 5 + 1);

        // reset local solution to initial condition
        *this = init;
        A = A_init;

        // cin.get();
        goto redo;
    }
    return true;
    // cout << "total tries:" << total_count << ", up-hill accepted count = " << accepted << ", rejected count = " << rejected << endl;
}

inline void Expression::printSpec(const allMacros &A) const
{
    Macro s = evaluateMacroDim(A);
    long long macrosArea = this->area;
    long long chipArea = 1LL * s.height * s.width;
    double chipAR = double(s.height) / double(s.width);
    double chipUsage = double(macrosArea) / double(chipArea) * 100.0;
    cout << "macros area   = " << macrosArea << endl;
    cout << "chip area     = " << chipArea << endl;
    cout << "aspect ratio  = " << chipAR << endl;
    cout << "space usage % = " << chipUsage << endl;
}

inline void Expression::outputData(const allMacros &A, fstream &fs, const output_format &of) const
{
    switch (of)
    {
    case output_format::GNUPlot:
        outputGNUPlot(A, fs);
        break;

    case output_format::PA2:
    default:
        outputPA2(A, fs);
        break;
    }
}

inline void Expression::outputGNUPlot(const allMacros &A, fstream &fs) const
{
    Macro t = evaluateMacroDim(A);
    /*
        output the GNUplot result
    */
    fs << "reset" << endl;
    fs << "set size ratio -1" << endl;
    fs << "set title \"result\"" << endl;
    fs << "set xlabel \"X\"" << endl;
    fs << "set ylabel \"Y\"" << endl;
    for (int i = 0; i < A.numBlocks; ++i)
    {
        int center_x = (2 * A.macros[i].x + A.macros[i].width) / 2;
        int center_y = (2 * A.macros[i].y + A.macros[i].height) / 2;
        fs << "set object " << i + 1 << " rect from " << A.macros[i].x << ", " << A.macros[i].y << " to " << A.macros[i].x + A.macros[i].width << ", " << A.macros[i].y + A.macros[i].height << endl;
        fs << "set label \""
           //    << "block_"
        //    << A.macros[i].name
           << A.macros[i].name.substr(6) // remove "block_"
           << "\" at " << center_x << ", " << center_y << " center" << endl;
    }
    int tic_x = t.width / 5;
    int tic_y = t.height / 5;
    fs << "set xtics " << tic_x << endl;
    fs << "set ytics " << tic_y << endl;
    fs << "plot [0:" << t.width << "][0:" << t.height << "]0" << endl;
    fs << "set terminal png size 1024, 768" << endl;
    fs << "set output \"output.png\"" << endl;
    fs << "replot" << endl;
}

void Expression::outputPA2(const allMacros &A, fstream &fs) const
{
    Macro t = evaluateMacroDim(A);
    fs << t.width << ' ' << t.height << endl;
    for (int i = 0; i < A.size(); ++i)
    {
        fs << A.macros[i].name << ' ' << A.macros[i].width << ' ' << A.macros[i].height << ' '
           << A.macros[i].x << ' ' << A.macros[i].y << ' '
           << A.macros[i].x + A.macros[i].width << ' ' << A.macros[i].y + A.macros[i].height
           << endl;
    }
}
#endif