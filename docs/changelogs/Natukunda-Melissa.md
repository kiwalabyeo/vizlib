# Changelog: Member 2 (Melissa Natukunda)

**Module:** Statistical Summaries i.e stats.hpp and stats.cpp
**GitHub:** melissanatukunda-xtian

<!-- Newest entries at the top. Add an entry in the same PR as the change. -->
## 2026-10-28
- **Changed:**
I included a base class binningStrategy with derived classes FreedmanDiaconis and Sturges to handle the different methods of histogram binning.
- **Why**
The method used before did not cater for some instances such as a small, normally disrtibuted dataset. Also, in order to utiise the aspects of OOP such as polymorphism and inheritance.
- **PR:** 83
- **Tested:** Tested the statisticalSummaries class and its functions,and the different binning strategies.
## 2026-10-24
- **Changed:**
I fixed some of the errors on the binning funtion. 
I also fixed the class summaryStatistics. The public functions required one to constantly provide an argument. It was very ambigous I think.It also did not utilise the capabilities of the class concept that C++ provides.
- **Why**
The errors were causing major issues such as unexpected output especially when getting the bins for the joint plots. The way the functions were declared did not enable the functions utilise the data vector directly. 
- **PR:** 67
- **Tested:** No tests done 

## 2026-10-17
- **Changed:**
I removed the header file and source file from the general repo and placed them in their allocated folder. I also added the histogram binning function as well as fixed some errors in the code. I aslo properly implemented a class summaryStatisics
- **Why:** 
For efficiency and organistion of our program. The function is required for joint plots
- **PR:** 53
- **Tested:** Not yet done
