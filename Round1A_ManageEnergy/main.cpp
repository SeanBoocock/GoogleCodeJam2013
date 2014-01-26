
/*
Manage Your Energy
https://code.google.com/codejam/contest/2418487/dashboard#s=p1
Problem

You've got a very busy calendar today, full of important stuff to do. You worked hard to prepare and make sure all the activities don't overlap. Now it's morning,
and you're worried that despite all of your enthusiasm, you won't have the energy to do all of this with full engagement.

You will have to manage your energy carefully. You start the day full of energy - E joules of energy, to be precise. You know you can't go below zero joules,
or you will drop from exhaustion. You can spend any non-negative, integer number of joules on each activity (you can spend zero, if you feel lazy),
and after each activity you will regain R joules of energy. No matter how lazy you are, however, you cannot have more than E joules of energy at any time;
any extra energy you would regain past that point is wasted.

Now, some things (like solving Code Jam problems) are more important than others. For the ith activity, you have a value vi that expresses how important this
activity is to you. The gain you get from each activity is the value of the activity, multiplied by the amount of energy you spent on the activity (in joules).
You want to manage your energy so that your total gain will be as large as possible.

Note that you cannot reorder the activities in your calendar. You just have to manage your energy as well as you can with the calendar you have.

Input

The first line of the input gives the number of test cases, T. T test cases follow. Each test case is described by two lines. The first contains three integers: E,
the maximum (and initial) amount of energy, R, the amount you regain after each activity, and N, the number of activities planned for the day. The second line contains
N integers vi, describing the values of the activities you have planned for today.

Output

For each test case, output one line containing "Case #x: y", where x is the case number (starting from 1) and y is the maximum gain you can achieve by managing your energy that day.

Limits

1 ≤ T ≤ 100.
Small dataset

1 ≤ E ≤ 5.
1 ≤ R ≤ 5.
1 ≤ N ≤ 10.
1 ≤ vi ≤ 10.
Large dataset

1 ≤ E ≤ 107.
1 ≤ R ≤ 107.
1 ≤ N ≤ 104.
1 ≤ vi ≤ 107.
Sample


Input

Output

3
5 2 2
2 1
5 2 2
1 2
3 3 4
4 1 3 5
Case #1: 12
Case #2: 12
Case #3: 39

In the first case, we can spend all 5 joules of our energy on the first activity (for a gain of 10), regain 2 and spend them on the second activity.
In the second case, we spend 2 joules on the first activity, regain them, and spend 5 on the second. In the third case, our regain rate is equal to the maximum energy,
meaning we always recover all energy after each activity - so we can spend full 3 joules on each activity.
*/

#include <algorithm>
#include <fstream>
#include <memory>
#include <stdint.h>
#include <vector>

static auto s_outFilePath = "out.txt";

struct TestCase
{
	int64_t*** ResultsTable;
	const uint32_t MaxEnergy;
	const uint32_t EnergyRegained;
	std::vector<uint32_t> ActivityValues;
	TestCase(uint32_t _maxEnergy, uint32_t _energyRegained, uint32_t _activityCount)
		: MaxEnergy(_maxEnergy)
		, EnergyRegained(_energyRegained)
		, ResultsTable(new int64_t**[_activityCount + 1])
	{
		for (uint32_t i = 0; i <= _activityCount; ++i)
		{
			ResultsTable[i] = new int64_t*[_maxEnergy + 1];

			for (uint32_t j = 0; j <= _maxEnergy; ++j)
			{
				ResultsTable[i][j] = new int64_t[_maxEnergy + 1];
				for (uint32_t k = 0; k <= _maxEnergy; ++k)
				{
					ResultsTable[i][j][k] = 0;
				}
			}
		}
	}

	~TestCase()
	{
		if (ResultsTable == nullptr)
		{
			return;
		}

		for (uint32_t i = 0; i <= ActivityValues.size(); ++i)
		{
			for (uint32_t j = 0; j <= MaxEnergy; ++j)
			{
				delete[] ResultsTable[i][j];
			}

			delete[] ResultsTable[i];
		}

		delete[] ResultsTable;
	}

	TestCase(TestCase&& other)
		: MaxEnergy(other.MaxEnergy)
		, EnergyRegained(other.EnergyRegained)
		, ActivityValues(std::move(other.ActivityValues))
		, ResultsTable(other.ResultsTable)
	{
		other.ResultsTable = nullptr;
	}

	TestCase(const TestCase& other) = delete;
};

typedef uint32_t EnergyGain;

void OutputResults(const std::vector<EnergyGain>& results)
{
	std::fstream out(s_outFilePath, std::fstream::out);
	uint32_t caseCount = 1;
	for (auto& result : results)
	{
		out << "Case #" << caseCount++ << ": " << result << std::endl;
	}
}

std::vector<EnergyGain> Evaluate(std::vector<TestCase>& testCases)
{
	std::vector<EnergyGain> results;
	for (auto& testCase : testCases)
	{
		auto activityIt = testCase.ActivityValues.size() - 1;
		do
		{
			for (uint32_t energyAvailableIt = 0; energyAvailableIt <= testCase.MaxEnergy; ++energyAvailableIt)
			{
				for (uint32_t energyUseIt = 0; energyUseIt <= energyAvailableIt; ++energyUseIt)
				{
					EnergyGain energyGain = testCase.ActivityValues[activityIt] * energyUseIt;
					uint32_t energyAvailableNext = std::min((energyAvailableIt - energyUseIt) + testCase.EnergyRegained, testCase.MaxEnergy);

					EnergyGain energyGainToAdd = 0;
					for (uint32_t energyUseNextIt = 0; energyUseNextIt <= energyAvailableNext; ++energyUseNextIt)
					{
						EnergyGain gain = static_cast<EnergyGain>(testCase.ResultsTable[activityIt + 1][energyAvailableNext][energyUseNextIt]);

						if (energyGainToAdd < gain)
						{
							energyGainToAdd = gain;
						}
					}
					energyGain += energyGainToAdd;
					testCase.ResultsTable[activityIt][energyAvailableIt][energyUseIt] = energyGain;
				}
			}
		} while (activityIt-- != 0);

		EnergyGain energyGain = 0;
		for (uint32_t i = 0; i <= testCase.MaxEnergy; ++i)
		{
			EnergyGain gain = static_cast<EnergyGain>(testCase.ResultsTable[0][testCase.MaxEnergy][i]);
			if (gain > energyGain)
			{
				energyGain = gain;
			}
		}
		results.push_back(energyGain);
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
		uint32_t maxEnergy, energyRegained, activityCount;
		fileStream >> maxEnergy >> energyRegained >> activityCount;
		TestCase testCase(maxEnergy, energyRegained, activityCount);
		for (uint32_t activityCountIt = 0; activityCountIt < activityCount; ++activityCountIt)
		{
			uint32_t activityValue;
			fileStream >> activityValue;
			testCase.ActivityValues.push_back(activityValue);
		}
		cases.push_back(std::move(testCase));
	}

	return cases;
}

int main(int argc, char** argv)
{
	OutputResults(
		Evaluate(
		ParseInput(argc, argv)));

	return 0;
}