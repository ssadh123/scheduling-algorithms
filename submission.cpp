#include <iostream> //Provides input/output stream functionality. Commonly used for cin, cout, cerr, and clog (console input/output).
#include <string> //Provides the std::string class and related utilities for working with strings in a safe, flexible way (as opposed to raw C-style char arrays).
#include <vector> //a dynamic array that automatically resizes itself as elements are added or removed, generally used for storing collections of data  
#include <algorithm> //Provides a collection of generic algorithms (sorting, searching, min/max, permutations, etc.) that work with STL containers and iterators
#include <tuple> //Provides the std::tuple type, which allows grouping together multiple values of potentially different types into a single object.
#include <utility> //Provides utility components such as std::pair, std::move, std::swap, and std::make_pair. Useful for general-purpose helper functionality.
#include <climits> //Provides limits for fundamental integral types (e.g., INT_MAX, INT_MIN, CHAR_MAX), useful for boundary checks and initialization.
#include <cmath> //Provides common mathematical functions (sqrt, pow, abs, sin, cos, etc.), constants like M_PI (implementation-dependent), and floating-point utilities.
using namespace std;

// Name: Saumya Sadh
// PID: A17043544 

///////////////////////////////////////////////////////
//////////////       QUESTION 1 HERE   ////////////////
///////////////////////////////////////////////////////

vector<int> myScheduleHelper(vector<int> P, vector<int> D){
    // Returns the final schedule order after Moore–Hodgson:
    // on-time jobs (EDD, after removals) followed by late jobs (in removal order).
    int n = (int)P.size();
    if (n == 0) return {};

    // Pack (id, p, d) and sort by due date (EDD)
    vector<tuple<int,int,int>> jobs; jobs.reserve(n);
    for (int i = 0; i < n; ++i) jobs.push_back(make_tuple(i+1, P[i], D[i]));
    sort(jobs.begin(), jobs.end(),
         [](const tuple<int,int,int>& a, const tuple<int,int,int>& b){
            return get<2>(a) < get<2>(b);
         });

    // Build feasible set; record removed (late) jobs in removal order
    vector<tuple<int,int,int>> scheduled;
    vector<int> late;
    int t = 0;
    for (const auto& job : jobs) {
        scheduled.push_back(job);
        t += get<1>(job);
        if (t > get<2>(job)) {
            // remove job with largest processing time
            auto it = max_element(scheduled.begin(), scheduled.end(),
                                  [](const tuple<int,int,int>& x, const tuple<int,int,int>& y){
                                      return get<1>(x) < get<1>(y);
                                  });
            t -= get<1>(*it);
            late.push_back(get<0>(*it));
            scheduled.erase(it);
        }
    }

    // Final order: scheduled IDs, then late IDs (in removal order)
    vector<int> sol; sol.reserve(scheduled.size() + late.size());
    for (const auto& job : scheduled) sol.push_back(get<0>(job));
    for (int id : late) sol.push_back(id);
    return sol;
}

vector<tuple<int,int>> myMoore(vector<int> P, vector<int> D){
    // Returns (jobID, endTime) across the FULL final order:
    // on-time jobs then late jobs, cumulative end times starting at 0.
    int n = (int)P.size();
    vector<tuple<int,int>> sol;
    if (n == 0) return sol;

    // Get final order (on-time then late)
    vector<int> order = myScheduleHelper(P, D);

    int t = 0;
    sol.reserve(order.size());
    for (int id : order) {
        t += P[id - 1];
        sol.push_back(make_tuple(id, t));
    }
    return sol;
}

vector<int> myMooreLate(vector<int> P, vector<int> D){
    // Returns only the late jobs, in the order they were removed.
    int n = (int)P.size();
    if (n == 0) return {};

    // Pack and sort by due date (EDD)
    vector<tuple<int,int,int>> jobs; jobs.reserve(n);
    for (int i = 0; i < n; ++i) jobs.push_back(make_tuple(i+1, P[i], D[i]));
    sort(jobs.begin(), jobs.end(),
         [](const tuple<int,int,int>& a, const tuple<int,int,int>& b){
            return get<2>(a) < get<2>(b);
         });

    // Moore–Hodgson with tracking of removed jobs
    vector<tuple<int,int,int>> scheduled;
    vector<int> late;
    int t = 0;
    for (const auto& job : jobs) {
        scheduled.push_back(job);
        t += get<1>(job);
        if (t > get<2>(job)) {
            auto it = max_element(scheduled.begin(), scheduled.end(),
                                  [](const tuple<int,int,int>& x, const tuple<int,int,int>& y){
                                      return get<1>(x) < get<1>(y);
                                  });
            t -= get<1>(*it);
            late.push_back(get<0>(*it));
            scheduled.erase(it);
        }
    }
    return late;
}

/////////////////////////////////////////////////////////
//////////////       QUESTION 2 HERE   //////////////////
/////////////////////////////////////////////////////////
vector<vector<tuple<int, float, float>>> myMcNaughton(vector<int> P, int m)
{
    vector<vector<tuple<int, float, float>>> X;
    int n = (int)P.size();
    if (m <= 0 || n == 0) return X; // guard

    X.assign(m, {}); // m processors

    // Compute optimal length of processor = max(total/m, max p_j)
    long long total = 0;
    int maxp = 0;
    for (int p : P) { total += p; if (p > maxp) maxp = p; }
    float L = (float)total / (float)m;
    if ((float)maxp > L) L = (float)maxp;

    int k = 0;                 // current processor index (0..m-1)
    float t_on_proc = 0.0f;    // current local time used on processor k

    for (int j = 0; j < n; ++j) {
        float rem = (float)P[j]; // remaining time for job j (job id = j+1)
        while (rem > 0.0f) {
            if (k >= m) return X; // safety: should not happen if L is chosen as above

            // If current processor is full, move to the next
            if (t_on_proc >= L) {
                ++k;
                t_on_proc = 0.0f;
                continue;
            }

            float space = L - t_on_proc;           // free capacity on this processor
            float take  = (rem < space ? rem : space); // amount of job to schedule here

            // schedule chunk [t_on_proc, t_on_proc + take) on processor k
            X[k].push_back(make_tuple(j + 1, t_on_proc, t_on_proc + take));

            t_on_proc += take;
            rem       -= take;
        }
    }
    return X;
}