
/*
	Bullseye
	https://code.google.com/codejam/contest/2418487/dashboard#s=p0
	Problem

	Maria has been hired by the Ghastly Chemicals Junkies (GCJ) company to help them manufacture bullseyes. A bullseye consists of a number of concentric rings (rings that are centered at the same point), and it usually represents an archery target. GCJ is interested in manufacturing black-and-white bullseyes. 

 

	Maria starts with t millilitres of black paint, which she will use to draw rings of thickness 1cm (one centimetre). A ring of thickness 1cm is the space between two concentric circles whose radii differ by 1cm.

	Maria draws the first black ring around a white circle of radius r cm. Then she repeats the following process for as long as she has enough paint to do so:

	Maria imagines a white ring of thickness 1cm around the last black ring.
	Then she draws a new black ring of thickness 1cm around that white ring.
	Note that each "white ring" is simply the space between two black rings.
	The area of a disk with radius 1cm is π cm2. One millilitre of paint is required to cover area π cm2. What is the maximum number of black rings that Maria can draw? Please note that:

	Maria only draws complete rings. If the remaining paint is not enough to draw a complete black ring, she stops painting immediately.
	There will always be enough paint to draw at least one black ring.
	Input

	The first line of the input gives the number of test cases, T. T test cases follow. Each test case consists of a line containing two space separated integers: r and t.

	Output

	For each test case, output one line containing "Case #x: y", where x is the case number (starting from 1) and y is the maximum number of black rings that Maria can draw.

	Limits

	Small dataset

	1 ≤ T ≤ 1000.
	1 ≤ r, t ≤ 1000.

	Large dataset

	1 ≤ T ≤ 6000.
	1 ≤ r ≤ 1018.
	1 ≤ t ≤ 2 × 1018.

	Sample


	Input 
 	
	5
	1 9
	1 10
	3 40
	1 1000000000000000000
	10000000000000000 1000000000000000000


	Output 
 
	Case #1: 1
	Case #2: 2
	Case #3: 3
	Case #4: 707106780
	Case #5: 49


*/

#include <fstream>
#include <stdint.h>
#include <vector>

static auto s_outFilePath = "out.txt";

struct TestCase
{
	uint32_t Radius;
	uint32_t Paint;
	TestCase(uint32_t _radius, uint32_t _paint)
		: Radius(_radius)
		, Paint(_paint)
	{}
};

typedef uint32_t RingCount;

void OutputResults(const std::vector<RingCount>& results)
{
	std::fstream out(s_outFilePath, std::fstream::out);
	uint32_t caseCount = 1;
	for (auto& result : results)
	{
		out << "Case #" << caseCount++ << ": " << result << std::endl;
	}
}

std::vector<RingCount> Evaluate(const std::vector<TestCase>& testCases)
{
	std::vector<RingCount> results;
	for (auto testCase: testCases)
	{
		RingCount count = 0;
		double paint = static_cast<double>(testCase.Paint);
		for (uint32_t radius = testCase.Radius;; radius += 2)
		{
			auto ithCirclePaint = 2.0 * radius + 1.0;
			if (ithCirclePaint > paint)
			{
				break;
			}
			paint -= ithCirclePaint;
		}
		results.push_back(count);
	}
	return results;
}

std::vector<TestCase> ParseInput(int argc, char** argv)
{
	std::vector<TestCase> cases;

	if (argc != 2)
	{
		return cases;
	}

	std::ifstream fileStream;
	fileStream.open(argv[1], std::fstream::in);
	if (!fileStream.good())
	{
		return cases;
	}

	uint32_t caseCount;
	fileStream >> caseCount;

	for (uint32_t i = 0; i < caseCount; ++i)
	{
		uint32_t radius, length;
		fileStream >> radius >> length;
		cases.emplace_back(radius, length);
	}

	return cases;
}

int main(int argc, char** argv)
{
	OutputResults( 
			Evaluate( 
				ParseInput(argc, argv) ) );

	return 0;
}