#pragma once
#include <algorithm>
#include <stdexcept>
#include <vector>
namespace vizlib{
class summaryStatistics{
    std::vector<double> Data;
    std::vector<double> Sorted_data;
public:
    summaryStatistics(const std::vector<double>& data,const std::vector<double>& sorted_data):Data(data),Sorted_data(sorted_data){}
    void collect_data(const std::vector<double>& data){
        if(Data.empty()){
        throw std::invalid_argument("data cannot be empty");
    }
        this->Data =data;
    }
    void sorted(const std::vector<double>& data){ //sorts the data once and places it in the sorted vector
    std::vector<double> s = data;                            // useful for median, quantiles,range,
    std::sort(s.begin(),s.end());
    this->Sorted_data = s;
    };

    double mean ()const ;
    double median ()const;
    double range()const;
    std::vector<double> mode()const;
    double standard_deviation()const;
    double quantile( double q)const; //q must be between 0 and 1
    double interquartile_range() const;
    int count(double value) const;
    std::vector<double> histogram_bin_edges() const;
};
}
