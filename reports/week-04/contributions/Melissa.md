**What was done**
 I created kde.hpp and kde.cpp, which calculate the kernel density estimate (KDE) for the data points in a dataset.

KDE is a way of estimating the shape of the distribution a set of data came from, without assuming it follows a particular form. It places a small smooth curve (a kernel, such as a Gaussian) on top of each data point and adds all the curves together. The result is a smooth density curve that is higher where data points are close together and lower where they are sparse. The width of each kernel is controlled by a setting called the bandwidth: a small bandwidth gives a bumpy curve that follows the individual points, while a large one gives a smoother curve that can hide detail. KDE is often used as a smoother alternative to a histogram, since it does not depend on where the bin edges are placed.

The KDE plot then uses these estimates to draw the density curve.

**Challenges faced**
I did not understand what KDE was at first, as the idea was a bit abstract.

**Lessons learnt**
I learnt how to research and teach myself new concepts, and how to see how new ideas apply to real problems and to the things I can build.

**Tools used**
- Claude AI: to help generate the kde.cpp and kde.hpp files.
- Wikipedia: to understand what KDE actually is and what it does. https://en.wikipedia.org/wiki/Kernel_density_estimation
- GeeksforGeeks: to understand how to implement KDE, so that I understood the code that was generated. https://www.geeksforgeeks.org/machine-learning/kernel-density-estimation/
