# RandomProfiler

just a random project that I though it would be fun to make
to summarize, you can test the time complexity of your alogrithm implementation with this template class  
it's not a serious project, so use it at your own risk.

## Profiler class

`Profiler` class can be used after the header file is included.
```cpp
#include "analysis.h"
```
`Profiler` class is a template class that takes 2 template arguments: `TestCase`, `TestCaseGenerator`  

#### TestCase
`TestCase` is a user-defined container type that holds neccessary informations for your algorithm to run.  
For example, you can define `TestCase` as a `struct`:
```cpp
struct MyTestCase {
  int* array_for_testing;
  int N;
};
```
#### TestCaseGenerator
`TestCaseGenerator` is a user-defined function that takes one `int` `iteration_argument` as a parameter (usually it means the *size* of one testcase) and returns `TestCase`.  
This function is responsible for (randomly) generating testcases for your algorithm. You can use `iteration_argument` to determine how `TestCase`s are generated.  
Note that `iteration_argument` starts at `0` and ends at `Profiler::it_no` that can be set via constructor call, or `Profiler::setIterationAmount` method, so do keep in mind.  
For example, you can define `TestCaseGenerator` as:
```cpp
MyTestCase generate(int iteration_argument) {
  MyTestCase tc;
  tc.array_for_testing = new int[iteration_argument + 1];
  tc.N = arg + 1;
  for (int i = 0; i <= tc.N; i++)
    tc.array_for_testing[i] = rand();
  return tc;
}
```
You can now instantiate `Profiler` like this:  
```cpp
Profiler<MyTestCase, decltype(generate)> profiler(generate);
Profiler<MyTestCase, decltype(generate)> profiler(generate, 5, 40); // ... or this
```
The second constructor sets `Profiler::smpl_sz` and `Profiler::it_no`.  
`smpl_sz` is the number of times the algorithm is run to average elapsed time for each *iteration*. The default value is `40`.  
`it_no` is the number of *iteration*. For each *iteration*, `TestCaseGenerator` will receive the same `iteration_argument`. The default value is `5`.  
Both can be resetted by setter methods: `Profiler::setSamplingSize` `Profiler::setIterationAmount`.  
For larger `smpl_sz`, the result of the `Profiler::profile` will be more "stable", but would require more time to profile.  
For larger `it_no`, it'll be easier to see the complexity of your algorithm, since it'll test for more `N`, but would require more time to profile.  

#### Profiler::profile
```cpp
<template class Algorithm>
void Profiler::profile(std::vector<long long>& result, Algorithm test) { ... }
```
Template argument `Algorithm` is a functor that takes `TestCase` as an argument (or const reference of `TestCase`), and runs your 'algorithm'.
It's likely that your alogrithm does not take `TestCase` directly, so `Algorithm` should be a wrapper function of your algorithm. The return value of `Algorithm` does not matter, since it is simply ignored for profiling.  
```cpp
// your favorite algorithm implementation for profiling...
int algo(int* arr, int n);

// to make your algorithm compatible with MyTestCase we implement a wrapper function
void algo_wrapper(const MyTestCase& tc) {
  algo(tc.arr_for_testing, tc.N);
  delete[] tc.arr_for_testing; // you must do clean-ups in here if necessary
}

// then, profile your algorithm like this
profiler.profile(result, algo_wrapper);

// even better, we could use lambdas instead of declaring a wrapper in global scope
profiler.profile(result, [](const MyTestCase& tc){
  algo(tc.arr_for_testing, tc.N);
  delete[] tc.arr_for_testing;
});
```
a series of elapsed time (in ms) will be stored in `result`.  
You can draw (view) its graph using `graphResult` function.

## Viewing the result
You can view the result with `graphResult` function.
```cpp
graphResult(result, width_of_displayed_graph, height_of_displayed_graph, max_value_displayed_by_graph);
```
the graph will be printed out on `stdout` like this:
```
|
|       ******
|   ****
| **
|*
+-------------
DISPLAY_INFO: RES=W*H MAX_LIMIT=C
```
