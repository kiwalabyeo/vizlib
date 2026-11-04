#WEEK 4 PROGRESS REPORT
Name: Sengendo Crystal Zawedde
Reg No: 25/U/08615/PS
Module: ECDF Plot Module

##WHAT WAS DONE
Created Unit Tests: I built a unit test file named tests/test_ecdf.cpp using std::cout. I did not use any outside test libraries. 
For the empty vector test, I made sure that passing an empty list of numbers returns no points safely without crashing. 
For the normal data test, I checked that unsorted numbers like 3.0, 1.0, and 2.0 get sorted into 1.0, 2.0, and 3.0, and that the calculated y values are correct.

##SUMMARY OF TEST RESULTS
Test Name: test_empty_vector
Input: empty list
Expected Output: no points returned
Status: PASSED

Test Name: test_standard_data
Input: 3.0, 1.0, 2.0
Expected Output: points at x=1.0 y=0.333, x=2.0 y=0.666, x=3.0 y=1.0
Status: PASSED

##NEXT STEPS
Talking to members in charge of other dependencies to bring all the code together. 
