#pragma once

#include <chrono>
#include <type_traits>
#include <utility>
#include <functional>
#include <iostream>

/*
* << Algorithm Profiler Class >>
* 
* This class is for visualizing and comparing time complexity of algorithms 
* var ARG is incremented for each iteration, which can modify the properties of TestCase, such as size
* ARG is referred as 'arg'
* var ARG increments from 0 to 'some value', and it is set to 40 by default
* you can modify the default value via setIterationAmount
* 
* <TestCase> is a type that holds all information needed to run <Algorithm>
* Example1: Testing a function that sorts std::vector<int>, TestCase = std::vector<int>
* Example2: Testing a function that sorts an array pointed by <int*>, then the following can be used as <TestCase>
* struct MyTestCase {
*     int* arr;
*     int N;
* }
*
* 
* <TestCaseGenerator> is a type of a (wrapper) function that generates and returns <TestCase>:
* TestCase(int arg)
*
* arg means var ARG, and can be used to alter TestCase it produces
* Example: <MyTestCase> generator function can be implemented as:
* MyTestCase generate(int arg) {
*     MyTestCase tc;
*     tc.arr = new int[arg + 1]; // arg starts at 0
*     tc.N = arg + 1;
*     return tc;
* }
* 
* 
* After <MyTestCase, TestCaseGenerator> are determined, you can instantiate Profiler class by:
* Profiler<MyTestCase, decltype(generate)> profiler(generate);
* 
* 
* <Algorithm> is a type of a (wrapper) function that takes <const TestCase&> as its argument for launching "toBeTested" function:
* void (const TestCase& tc)
*
* Example: To test <std::sort> with <MyTestCase>:
* void wrapper(const MyTestCase& tc) {
*     std::sort(tc.arr, tc.arr + tc.N);
*     delete[] tc.arr; // always free memory!
* }
* 
* Now pass std::vector<long long> result and <Algorithm> wrapper function to profile method:
* profiler.profile(result, wrapper);
* 
*/
template<class TestCase, class TestCaseGenerator>
class Profiler
{
public:

	Profiler(TestCaseGenerator gen) : gen(gen) {}
	Profiler(TestCaseGenerator gen, int s, int i) : gen(gen), smpl_sz(s), it_no(i) {}

	template<class Algorithm>
	void profile(std::vector<long long>& result, Algorithm test)
	{
		result.reserve(it_no);
		for (int t = 0; t < it_no; t++)
		{
			long long avg = 0;
			for (int a = 0; a < smpl_sz; a++)
			{
				auto testcase = gen(t);

				std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

				test(testcase);

				std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
				std::chrono::microseconds ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
				avg += ms.count();
			}
			avg /= smpl_sz;

			result.push_back(avg);

			std::cout << int(double(t) / it_no * 100) << "% Done.\n";
		}
	}

	int getSamplingSize() {
		return smpl_sz;
	}

	int getIterationAmount() {
		return it_no;
	}

	void setSamplingSize(int s) {
		smpl_sz = s;
	}

	void setIterationAmount(int i) {
		it_no = i;
	}

private:
	int smpl_sz = 5;
	int it_no = 40;
	TestCaseGenerator gen;

};

/*
* Displays the resulting vector from Profiler.profile func
* w = weidth of displayed graph
* h = height of displayed graph
* clip = maximum value displayed by the graph
*/
inline void graphResult(const std::vector<long long>& src, int w, int h, int clip)
{
	int cols = w - 1;
	int rows = h - 1;

	std::string display((w + 1) * h, ' ');
	for (int y = 0; y < h; y++)
	{
		display[y * (w + 1) + w] = '\n';
		display[y * (w + 1) + 0] = '|';
	}
	display[rows * (w + 1) + 0] = '+';
	for (int x = 1; x < w; x++)
		display[rows * (cols + 2) + x] = '-';

	for (int i = 0; i < src.size(); i++)
	{
		double yscale = (double)rows / clip;
		double xscale = (double)cols / src.size();

		int y = (int)(src[i] * yscale); if (y >= rows) continue;
		int x = (int)(i * xscale); if (x >= cols) x = cols - 1;

		display[(rows - y - 1) * (w + 1) + x + 1] = '*';
	}

	std::cout << display;
	printf("DISPLAY INFO: RES=%d*%d MAX_LIMIT=%d\n", w, h, clip);
}
