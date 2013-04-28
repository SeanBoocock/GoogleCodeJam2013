
/*
	Fair and Square
	https://code.google.com/codejam/contest/dashboard?c=2270488#s=p2
	Problem

	Little John likes palindromes, and thinks them to be fair (which is a fancy word for nice). A palindrome is just an integer that reads the same backwards and forwards - so 6, 11 and 121 are all palindromes, while 10, 12, 223 and 2244 are not (even though 010=10, we don't consider leading zeroes when determining whether a number is a palindrome).

	He recently became interested in squares as well, and formed the definition of a fair and square number - it is a number that is a palindrome and the square of a palindrome at the same time. For instance, 1, 9 and 121 are fair and square (being palindromes and squares, respectively, of 1, 3 and 11), while 16, 22 and 676 are not fair and square: 16 is not a palindrome, 22 is not a square, and while 676 is a palindrome and a square number, it is the square of 26, which is not a palindrome.

	Now he wants to search for bigger fair and square numbers. Your task is, given an interval Little John is searching through, to tell him how many fair and square numbers are there in the interval, so he knows when he has found them all.

	Solving this problem

	Usually, Google Code Jam problems have 1 Small input and 1 Large input. This problem has 1 Small input and 2 Large inputs. Once you have solved the Small input, you will be able to download any of the two Large inputs. As usual, you will be able to retry the Small input (with a time penalty), while you will get only one chance at each of the Large inputs.

	Input

	The first line of the input gives the number of test cases, T. T lines follow. Each line contains two integers, A and B - the endpoints of the interval Little John is looking at.

	Output

	For each test case, output one line containing "Case #x: y", where x is the case number (starting from 1) and y is the number of fair and square numbers greater than or equal to A and smaller than or equal to B.

	Limits

	Small dataset

	1 ≤ T ≤ 100.
	1 ≤ A ≤ B ≤ 1000.

	First large dataset

	1 ≤ T ≤ 10000.
	1 ≤ A ≤ B ≤ 1014.

	Second large dataset

	1 ≤ T ≤ 1000.
	1 ≤ A ≤ B ≤ 10100.

*/

#include <cmath>
#include <fstream>
#include <stdint.h>
#include <vector>
#include <utility>

static auto s_defaultSmallTestCasePath = "TestCases/C-small-practice.in";
static auto s_defaultLargeTestCasePath = "TestCases/C-large-practice-1.in";
static auto s_defaultLargeTestCaseTwoPath = "TestCases/C-large-practice-2.in";
static auto s_outFilePath = "out.txt";

typedef std::vector<std::pair<uint64_t,uint64_t>> IntervalList;

const IntervalList ParseInput(int argumentCount, char** arguments)
{
	const char* testCasePath = nullptr;
	IntervalList testCases;
	if (argumentCount <= 1)
	{
		testCasePath = s_defaultLargeTestCasePath;
	}
	else if (argumentCount == 2)
	{
		testCasePath = arguments[1];
	}

	std::ifstream fileStream;
	fileStream.open(testCasePath, std::fstream::in);
	if (!fileStream.good())
	{
		return testCases;
	}

	uint32_t caseCount = 0;
	fileStream >> caseCount;
	testCases.resize(caseCount);
	for(auto& testCase : testCases)
	{
		fileStream >> testCase.first >> testCase.second;
	}
	
	return testCases;
}

uint64_t ReverseNumerBaseTen(uint64_t number)
{
	uint64_t reversed = 0;
	while(number > 0)
	{
		reversed *= 10;
		reversed += number % 10;
		number /= 10;
	}
	return reversed;
}

bool IsPalindrome(const uint64_t number)
{
	auto reversed = ReverseNumerBaseTen(number);
	return (number - reversed) == 0;
}

//Use global skip list as optimization
const std::vector<uint32_t> EvaluateIntervals(IntervalList& intervals)
{
	std::vector<uint32_t> results(intervals.size());
	auto resultIt = results.begin();
	for(auto& interval : intervals)
	{
		auto root = static_cast<decltype(interval.first)>(std::ceil(std::sqrt(static_cast<double>(interval.first))));
		auto square = root * root;
		while(square <= interval.second)
		{
			if(IsPalindrome(root) && IsPalindrome(square))
			{
				++(*resultIt);
			}
			++root;
			square = root * root;
		}
		++resultIt;
	}

	return results;
}

void ReportResults(const std::vector<uint32_t>& testCaseResults)
{
	std::fstream out(s_outFilePath, std::fstream::out);
	uint32_t resultCount = 1;
	for(auto& result : testCaseResults)
	{
		out << "Case #" << resultCount++ << ": " << result << std::endl;
	}
}

int main(int argumentCount, char** arguments)
{
	auto intervals = ParseInput(argumentCount, arguments);
	
	auto results = EvaluateIntervals(intervals);

	ReportResults(results);

	return 0;
}