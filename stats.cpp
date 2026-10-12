#include <vector>
# include <algorithm>
# include <iostream>
#include <unordered_map>
#include <math.h>
using namespace std;

double mean(const vector<double>& data){
    double sum = 0;
    for (double number: data){
    sum += number;
     }
   return sum/data.size();
}
const vector<double> sorted(const vector<double>& data){ //sorts the data once and returns a sorted vector of doubles
    vector<double> s = data;                            // useful for median, quantiles,range,
    sort(s.begin(),s.end());
    return s;
}
double median(const vector<double>& sorted_data){
    if (sorted_data.size()%2==0){
        return (sorted_data[(sorted_data.size()/2)-1]+ sorted_data[ (sorted_data.size()/2)])/2;
    }
    else 
    return sorted_data[(sorted_data.size()-1)/2];
}

double range(const vector<double>&sorted_data){
    return sorted_data.back()-sorted_data.front();
}

vector<double> mode(const vector<double>& data){
        unordered_map<double,int> count;
        for (auto number: data){  // maps each number to its frequency
            count[number]++;
        }
        int max_freq = 0;
        for (auto i:count){  // loops through the map to find the maximum frequency
            if (i.second>max_freq){
                max_freq=i.second;
            }
        }
        vector<double> modes;
        for (auto i:count){  // loops through the map to find all numbers with the maximum frequency
            if (i.second==max_freq){
                modes.push_back(i.first);
            }
        }
        return modes;

}
double standard_deviation(const vector<double>& data){
    double sum_of_square_diffs = 0;
    for (double number: data){
        sum_of_square_diffs += (number-mean(data))*(number-mean(data));
    }
    return sqrt(sum_of_square_diffs/data.size());
}
double quantile(const vector<double>& sorted_data, double q){ //q must be between 0 and 1
    if(q<0 || q>1){
        throw invalid_argument("q must be between 0 and 1");
    }
    int rank=q*(sorted_data.size()-1);
    return sorted_data[rank];

}
double interquartile_range(const vector<double>& sorted_data){
    return quantile(sorted_data, 0.75) - quantile(sorted_data, 0.25);
}

int count(const vector<double>& data, double value){
    int count =0;
    for (double number: data){
        if (number==value){
            count++;
        }
    }
    return count;
}

int main(){
    vector<double> fake={1,2,8,9,3,4,5,6,7,8,9};
    vector<double> sfake= sorted(fake);
    cout<<"range"<<range(sfake)<<endl;
    cout<<"median"<<median(sfake)<<endl;
    cout<<"mean"<<mean(fake);
    cout<<"mode"<<mode(fake)[0]<<endl;
}