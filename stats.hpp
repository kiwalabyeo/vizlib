#include <vector>
using namespace std;
class summaryStatistics{
    vector<double> data;
public:
    summaryStatistics(const vector<double>& data):data(data){}
    void collect_data(const vector<double>& data){
        this->data=data;
    }
    double mean(const vector<double>& data);
    vector<double> sorted(const vector<double>& data);
    double median(const vector<double>& sorted_data);
    double range(const vector<double>& sorted_data);
    vector<double> mode(const vector<double>& data);
    double standard_deviation(const vector<double>& data);
    double quantile(const vector<double>& sorted_data, double q);
    double interquartile_range(const vector<double>& sorted_data);
    int count(const vector<double>& data, double value);
};
double mean(const vector<double>& data);
vector<double> sorted(const vector<double>& data);
double median(const vector<double>& sorted_data);
double range(const vector<double>& sorted_data);
vector<double> mode(const vector<double>& data);
double standard_deviation(const vector<double>& data);
double quantile(const vector<double>& sorted_data, double q);
double interquartile_range(const vector<double>& sorted_data);
int count(const vector<double>& data, double value);
