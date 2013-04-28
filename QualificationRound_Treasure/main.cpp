
/*
	Treasure
	https://code.google.com/codejam/contest/dashboard?c=2270488#s=p3
	Problem

	Following an old map, you have stumbled upon the Dread Pirate Larry's secret treasure trove!

	The treasure trove consists of N locked chests, each of which can only be opened by a key of a specific type. Furthermore, once a key is used to open a chest, it can never be used again. Inside every chest, you will of course find lots of treasure, and you might also find one or more keys that you can use to open other chests. A chest may contain multiple keys of the same type, and you may hold any number of keys.

	You already have at least one key and your map says what other keys can be found inside the various chests. With all this information, can you figure out how to unlock all the chests?

	For example, suppose the treasure trove consists of four chests as described below, and you began with exactly one key of type 1:

	Chest Number  |  Key Type To Open Chest  |  Key Types Inside
	--------------+--------------------------+------------------
	1             |  1                       |  None
	2             |  1                       |  1, 3
	3             |  2                       |  None
	4             |  3                       |  2
	You can open all the chests in this example if you do them in the order 2, 1, 4, 3. If you start by opening chest #1 first, then you will have used up your only key, and you will be stuck.
	Input

	The first line of the input gives the number of test cases, T. T test cases follow. Each test case begins with a single line containing two positive integers K and N, representing the number of keys you start with and the number of chests you need to open.

	This is followed by a line containing K integers, representing the types of the keys that you start with.

	After that, there will be N lines, each representing a single chest. Each line will begin with integers Ti and Ki, indicating the key type needed to open the chest and the number of keys inside the chest. These two integers will be followed by Ki more integers, indicating the types of the keys contained within the chest.

	Output

	For each test case, output one line containing "Case #x: C1 C2 ... CN", where x is the case number (starting from 1), and where Ci represents the index (starting from 1) of the ith chest that you should open.

	If there are multiple ways of opening all the chests, choose the "lexicographically smallest" way. In other words, you should choose to make C1 as small as possible, and if there are multiple ways of making C1 as small as possible, choose the one that makes C2 as small as possible, and so on.

	If there is no way to open all the chests, you should instead output one line containing "Case #x: IMPOSSIBLE".

	Limits

	1 ≤ T ≤ 25.
	1 ≤ K.
	All key types will be integers between 1 and 200 inclusive.
	Small dataset

	1 ≤ N ≤ 20.
	In each test case, there will be at most 40 keys altogether.
	Large dataset

	1 ≤ N ≤ 200.
	In each test case, there will be at most 400 keys altogether.

*/

#include <bitset>
#include <cmath>
#include <fstream>
#include <stdint.h>
#include <vector>

static auto s_defaultSmallTestCasePath = "TestCases/D-small-practice.in";
static auto s_defaultLargeTestCasePath = "TestCases/D-large-practice-1.in";
static auto s_outFilePath = "out.txt";

typedef uint8_t KeyType;

struct Chest
{
	KeyType ToOpen;
	std::vector<KeyType> KeysInside;
};

struct TestCase
{
	std::vector<KeyType> InitialKeys;
	std::vector<Chest> Chests;
};

struct Result
{
	std::vector<uint16_t> Chests;
};

typedef std::vector<TestCase> TestList;

const TestList ParseInput(int argumentCount, char** arguments)
{
	const char* testCasePath = nullptr;
	TestList testCases;
	if (argumentCount <= 1)
	{
		testCasePath = s_defaultSmallTestCasePath;
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
		uint32_t numberOfKeys = 0, numberOfChests = 0;
		fileStream >> numberOfKeys >> numberOfChests;

		testCase.InitialKeys.resize(numberOfKeys);
		for(auto& key : testCase.InitialKeys)
		{
			uint32_t keyType = 0;
			fileStream >> keyType;
			key = static_cast<KeyType>(keyType);
		}

		testCase.Chests.resize(numberOfChests);
		for(auto& chest : testCase.Chests)
		{
			uint32_t numberOfChestKeys = 0, keyType = 0;
			fileStream >> keyType >> numberOfChestKeys;
			chest.ToOpen = static_cast<KeyType>(keyType);
			if(numberOfChestKeys > 0)
			{
				chest.KeysInside.resize(numberOfChestKeys);
				for(auto& chestKey : chest.KeysInside)
				{
					fileStream >> keyType;
					chestKey = static_cast<KeyType>(keyType);
				}
			}
		}
	}
	
	return testCases;
}

bool Evaluate(const std::vector<KeyType>& currentKeys, std::bitset<200> unlockedChests, Result& chestOrder, const TestCase& testCase, uint32_t unopenedChestCount)
{
	if(unopenedChestCount == 0)
	{
		return true;
	}

	if(currentKeys.empty())
	{
		return false;
	}

	for(auto keyIt = currentKeys.begin(), endKeyIt = currentKeys.end();
		keyIt != endKeyIt;
		++keyIt)
	{
		uint32_t chestIndex = 0;
		for(auto& chest : testCase.Chests)
		{
			if(unlockedChests.at(chestIndex)
				|| *keyIt != chest.ToOpen)
			{
				++chestIndex;
				continue;
			}

			auto level = chestOrder.Chests.size();
			chestOrder.Chests.push_back(chestIndex);
			std::vector<KeyType> nextLevel(currentKeys);
			nextLevel.erase(nextLevel.begin() + std::distance(currentKeys.begin(), keyIt));
			nextLevel.insert(nextLevel.end(), chest.KeysInside.begin(), chest.KeysInside.end());
			unlockedChests.set(chestIndex);
			if(Evaluate(nextLevel, unlockedChests, chestOrder, testCase, --unopenedChestCount))
			{
				return true;
			}
			unlockedChests.set(chestIndex, false);
			chestOrder.Chests.erase(chestOrder.Chests.begin() + level, chestOrder.Chests.end());
			++chestIndex;
		}
	}

	return false;
}

bool IsBetterResult(const Result& existingResult, const Result& newResult)
{
	if(existingResult.Chests.empty())
	{
		return true;
	}

	for(auto existingIt = existingResult.Chests.begin(), newIt = newResult.Chests.begin();
		existingIt != existingResult.Chests.end() && newIt != newResult.Chests.end();
		++existingIt, ++newIt)
	{
		if(*existingIt < *newIt)
		{
			return false;
		}
		else if(*newIt < *existingIt)
		{
			return true;
		}
	}

	return false;
}

std::vector<Result> EvaluateTestCases(const TestList& testCases)
{
	std::vector<Result> results(testCases.size());
	auto resultIt = results.begin();
	for(auto& test : testCases)
	{
		std::bitset<200> unlockedChests;
		std::vector<KeyType> keys(test.InitialKeys);
		for(uint32_t index = 0, end = keys.size(); index < end; ++index)
		{
			Result result;
			if(!Evaluate(keys, unlockedChests, result, test, test.Chests.size()))
			{
				break;
			}

			if(IsBetterResult(*resultIt, result))
			{
				if(resultIt->Chests.empty())
				{
					resultIt->Chests.resize(result.Chests.size());
				}
				std::copy(result.Chests.begin(), result.Chests.end(), resultIt->Chests.begin());
			}

			auto frontKey = keys.front();
			keys.erase(keys.begin());
			keys.push_back(frontKey);	
		}
		++resultIt;
	}
	return results;
}

void ReportResults(const std::vector<Result>& testCaseResults)
{
	std::fstream out(s_outFilePath, std::fstream::out);
	uint32_t resultCount = 1;
	for(auto& result : testCaseResults)
	{
		out << "Case #" << resultCount++ << ":";
		if(result.Chests.empty())
		{
			out << " IMPOSSIBLE" << std::endl;
		}
		else
		{
			for(auto& chest : result.Chests)
			{
				out << " " << (chest + 1);
			}
			out << std::endl;
		}
	}
}

int main(int argumentCount, char** arguments)
{
	auto testCases = ParseInput(argumentCount, arguments);
	
	auto results = EvaluateTestCases(testCases);

	ReportResults(results);

	return 0;
}