#include <vector>
# include <algorithm>
# include <iostream>
#include <unordered_map>
#include <math.h>
#include <stdexcept>
namespace vizlib{ 
class summaryStatistics{
std::vector<double> data;
public:
    summaryStatistics(const std::vector<double>& data):data(data){}
    void collect_data(const std::vector<double>& data){
        this->data=data;
    }
    double mean(const std::vector<double>& data);
    const std::vector<double> sorted(const std::vector<double>& data);
    double median(const std::vector<double>& sorted_data);
    double range(const std::vector<double>& sorted_data);
    std::vector<double> mode(const std::vector<double>& data);
    double standard_deviation(const std::vector<double>& data);
    double quantile(const std::vector<double>& sorted_data, double q);
    double interquartile_range(const std::vector<double>& sorted_data);
    int count(const std::vector<double>& data, double value);
    std::vector<double> histogram_bin_edges(const std::vector<double>&data);
};
double summaryStatistics::mean(const std::vector<double>& data){
    if(data.empty()){
        throw std::invalid_argument("data cannot be empty");
    }
    double sum = 0;
    for (double number: data){
    sum += number;
     }
   return sum/data.size();
}
const std::vector<double> summaryStatistics::sorted(const std::vector<double>& data){ //sorts the data once and returns a sorted vector of doubles
    std::vector<double> s = data;                            // useful for median, quantiles,range,
    std::sort(s.begin(),s.end());
    return s;
}
double summaryStatistics::median(const std::vector<double>& sorted_data){
    if (sorted_data.size()%2==0){
        return (sorted_data[(sorted_data.size()/2)-1]+ sorted_data[ (sorted_data.size()/2)])/2;
    }
    else 
    return sorted_data[(sorted_data.size()-1)/2];
}

double summaryStatistics::range(const std::vector<double>&sorted_data){
    if(sorted_data.empty()){
        throw std::invalid_argument("data cannot be empty");
    }
    return sorted_data.back()-sorted_data.front();
}

std::vector<double> summaryStatistics:: mode(const std::vector<double>& data){
    if(data.empty()){
        throw std::invalid_argument("data cannot be empty");
    }

    std::unordered_map<double,int> count;
    for (auto number: data){  // maps each number to its frequency
        count[number]++;
    }
    int max_freq = 0;
    for (auto i:count){  // loops through the map to find the maximum frequency
        if (i.second>max_freq){
            max_freq=i.second;
        }
    }
    std::vector<double> modes;
    for (auto i:count){  // loops through the map to find all numbers with the maximum frequency
        if (i.second==max_freq){
            modes.push_back(i.first);
        }
    }
    return modes;
}
double summaryStatistics::standard_deviation(const std::vector<double>& data){
    if (data.empty()){
        throw std::invalid_argument("data cannot be empty");
    }
    double mean_value = mean(data);
    double sum_of_square_diffs = 0;
    for (double number: data){
        sum_of_square_diffs += (number-mean_value)*(number-mean_value);
    }
    return sqrt(sum_of_square_diffs/data.size());
}
double summaryStatistics::quantile(const std::vector<double>& sorted_data, double q){ //q must be between 0 and 1
    if(q<0 || q>1){
        throw std::invalid_argument("q must be between 0 and 1");
    }
    if (sorted_data.empty()){
        throw std::invalid_argument("data cannot be empty");
    }
    double rank = q * (sorted_data.size() - 1);
    int lower_index = static_cast<int>(std::floor(rank));
    int upper_index = static_cast<int>(std::ceil(rank));

    if (lower_index == upper_index) {
            return sorted_data[lower_index];
        }
        
        // Linear interpolation
        double weight = rank - lower_index;
        return (sorted_data[lower_index] * (1.0 - weight)) + (sorted_data[upper_index] * weight);

}
double summaryStatistics::interquartile_range(const std::vector<double>& sorted_data){
    return quantile(sorted_data, 0.75) - quantile(sorted_data, 0.25);
}

int summaryStatistics::count(const std::vector<double>& data, double value){
    int count =0;
    for (double number: data){
        if (number==value){
            count++;
        }
    }
    return count;
}
std:: vector<double> summaryStatistics:: histogram_bin_edges(const std::vector<double>&data){
    if (data.empty()){
        throw std::invalid_argument("data cannot be empty");
    }

 std::vector<double> bin_edges;
 double bin_width = 2 * interquartile_range(data)/pow(data.size(),1.0/3.0);
 int number_of_bins= range(data)/bin_width;
 for (int i=0; i<=number_of_bins; i++){
    bin_edges.push_back(data.front()+i*bin_width);
 }
 return bin_edges;
}}


