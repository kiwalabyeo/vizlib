#include "stats.hpp"
#include <vector>
# include <algorithm>
# include <iostream>
#include <unordered_map>
#include <cmath>
#include <stdexcept>
namespace vizlib{ 

double summaryStatistics::mean()const{

    double sum = 0;
    for (double number: Data){
    sum += number;
     }
   return sum/Data.size();
}

double summaryStatistics::median() const{
    if (Sorted_data.size()%2==0){
        return (Sorted_data[(Sorted_data.size()/2)-1]+ Sorted_data[ (Sorted_data.size()/2)])/2;
    }
    else 
    return Sorted_data[(Sorted_data.size()-1)/2];
}

double summaryStatistics::range() const{

    return Sorted_data.back()-Sorted_data.front();
}

std::vector<double> summaryStatistics:: mode() const{


    std::unordered_map<double,int> count;
    for (auto number: Data){  // maps each number to its frequency
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
double summaryStatistics::standard_deviation() const{

    double mean_value = mean();
    double sum_of_square_diffs = 0;
    for (double number: Data){
        sum_of_square_diffs += (number-mean_value)*(number-mean_value);
    }
    return sqrt(sum_of_square_diffs/Data.size());
}

double summaryStatistics::quantile(double q)const{ //q must be between 0 and 1
    if(q<0 || q>1){
        throw std::invalid_argument("q must be between 0 and 1");
    }

    double rank = q * (Sorted_data.size() - 1);
    int lower_index = static_cast<int>(std::floor(rank));
    int upper_index = static_cast<int>(std::ceil(rank));

    if (lower_index == upper_index) {
            return Sorted_data[lower_index];
        }
        
        // Linear interpolation
        double weight = rank - lower_index;
        return (Sorted_data[lower_index] * (1.0 - weight)) + (Sorted_data[upper_index] * weight);

}
double summaryStatistics::interquartile_range() const{
    return quantile(0.75) - quantile(0.25);
}

int summaryStatistics::count(double value) const{
    int count =0;
    for (double number: Data){
        if (number==value){
            count++;
        }
    }
    return count;
}
std:: vector<double> summaryStatistics:: histogram_bin_edges() const{

 std::vector<double> bin_edges;
 double bin_width = 2 * interquartile_range()/cbrt(Sorted_data.size()); // Freedman-Diaconis rule for bin width
 if(bin_width==0){
bin_edges.push_back(Sorted_data.front());
return bin_edges;
 }
    
 int number_of_bins= std::ceil(range()/bin_width);
 for (int i=0; i<=number_of_bins; i++){
    bin_edges.push_back(Sorted_data.front()+i*bin_width);
 }
 return bin_edges;
}}


