#include <cmath>
#include <iostream>
#include <chrono>
#include <random>
using namespace std;
class Timer
{
private:
    using clock_type = chrono::high_resolution_clock;
    using second_type = chrono::duration<double, chrono::seconds::period>;

    chrono::time_point<clock_type> m_startTime;

public:
    Timer() : m_startTime(clock_type::now())
    {
    }

    double reset()
    {
        double dt = getElapsed();
        m_startTime = clock_type::now();
        return dt;
    }

    double getElapsed() const
    {
        return chrono::duration_cast<second_type>(clock_type::now() - m_startTime).count();
    }
};

#define SEED chrono::high_resolution_clock::now().time_since_epoch().count()

int main()
{
    // mt19937 mt(SEED);
    // int ub_width = 5, lb_width = 3;
    // int rand_width = (mt() % (ub_width - lb_width + 1)) + lb_width;
    // cout << rand_width << endl;
    Timer t;
    for (int i = 0; i < 500; i++)
    {
        _sleep(5);
    }
    cout << t.getElapsed();

    return 0;
}