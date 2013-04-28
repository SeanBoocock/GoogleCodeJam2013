
/*
	Lawnmower
	https://code.google.com/codejam/contest/dashboard?c=2270488#s=p1
	Problem

	Alice and Bob have a lawn in front of their house, shaped like an N metre by M metre rectangle. Each year, they try to cut the lawn in some interesting pattern. They used to do their cutting with shears, which was very time-consuming; but now they have a new automatic lawnmower with multiple settings, and they want to try it out.

	The new lawnmower has a height setting - you can set it to any height h between 1 and 100 millimetres, and it will cut all the grass higher than h it encounters to height h. You run it by entering the lawn at any part of the edge of the lawn; then the lawnmower goes in a straight line, perpendicular to the edge of the lawn it entered, cutting grass in a swath 1m wide, until it exits the lawn on the other side. The lawnmower's height can be set only when it is not on the lawn.

	Alice and Bob have a number of various patterns of grass that they could have on their lawn. For each of those, they want to know whether it's possible to cut the grass into this pattern with their new lawnmower. Each pattern is described by specifying the height of the grass on each 1m x 1m square of the lawn.

	The grass is initially 100mm high on the whole lawn.

	Input

	The first line of the input gives the number of test cases, T. T test cases follow. Each test case begins with a line containing two integers: N and M. Next follow N lines, with the ith line containing M integers ai,j each, the number ai,j describing the desired height of the grass in the jth square of the ith row.

	Output

	For each test case, output one line containing "Case #x: y", where x is the case number (starting from 1) and y is either the word "YES" if it's possible to get the x-th pattern using the lawnmower, or "NO", if it's impossible (quotes for clarity only).

	Limits

	1 ≤ T ≤ 100.

	Small dataset

	1 ≤ N, M ≤ 10.
	1 ≤ ai,j ≤ 2.
	Large dataset

	1 ≤ N, M ≤ 100.
	1 ≤ ai,j ≤ 100.

*/

#include <bitset>
#include <fstream>
#include <stdint.h>
#include <vector>
#include <queue>
#include <functional>

static auto s_defaultSmallTestCasePath = "TestCases/B-small-practice.in";
static auto s_defaultLargeTestCasePath = "TestCases/B-large-practice.in";
static auto s_outFilePath = "out.txt";

struct Node
{
	enum VisitStatus : uint8_t
	{
		NotVisited,
		Horizontal,
		Vertical
	};

	uint8_t Height;
	VisitStatus Visited;
	uint8_t WIndex;
	uint8_t HIndex;

	Node()
		: Height(0)
		, Visited(NotVisited)
		, WIndex(0)
		, HIndex(0)
	{}

	void Initialize(const uint8_t height, const uint8_t wIndex, const uint8_t hIndex)
	{
		Height = height;
		WIndex = wIndex;
		HIndex = hIndex;
	}
};

struct Lawn
{
	Lawn(const uint8_t width, const uint8_t height)
		: CompareFunction([](const Node* lhs, const Node* rhs) -> bool { return lhs->Height > rhs->Height; })
		, Queue(CompareFunction)
		, Width(width)
		, Height(height)
	{
		LawnNodes.resize(width * height);
	}

	bool ExploreVertical(Node* node)
	{
		auto wIndex = node->WIndex;
		auto height = node->Height;
		uint8_t i = 0;
		for(; i < Height; ++i)
		{
			auto& lawnNode = LawnNodes[wIndex + i*Width];
			//If we encounter a node with greater height, we know there
			//is no way we could have traversed the lawn vertically at
			//the through the argument node (otherwise the encountered
			//node would be at the argument node's height)
			if(lawnNode.Height > height)
			{
				return false;
			}

			if(lawnNode.Height < height
				&& lawnNode.Visited == Node::Vertical)
			{
				return false;
			}

			if(node == &lawnNode)
			{
				continue;
			}

			if(lawnNode.Height == height
				&& lawnNode.Visited == Node::Vertical)
			{
				break;
			}
		}

		for(uint8_t visitIndex = 0; visitIndex < i; ++visitIndex)
		{
			LawnNodes[wIndex + visitIndex*Width].Visited = Node::Vertical;
		}

		return true;
	}

	bool ExploreHorizontal(Node* node)
	{
		auto hIndex = node->HIndex;
		auto height = node->Height;
		uint8_t i = 0;
		for(; i < Width; ++i)
		{
			auto& lawnNode = LawnNodes[i + hIndex*Width];
			//If we encounter a node with greater height, we know there
			//is no way we could have traversed the lawn vertically at
			//the through the argument node (otherwise the encountered
			//node would be at the argument node's height)
			if(lawnNode.Height > height)
			{
				return false;
			}

			if(lawnNode.Height < height
				&& lawnNode.Visited == Node::Horizontal)
			{
				return false;
			}

			if(node == &lawnNode)
			{
				continue;
			}

			if(lawnNode.Height == height
				&& lawnNode.Visited == Node::Horizontal)
			{
				break;
			}
		}

		for(uint8_t visitIndex = 0; visitIndex < i; ++visitIndex)
		{
			LawnNodes[visitIndex + hIndex*Width].Visited = Node::Horizontal;
		}

		return true;
	}

	std::function<bool(const Node*,const Node*)> CompareFunction;
	std::priority_queue<Node*,
						std::vector<Node*>,
						decltype(CompareFunction)> Queue;
	std::vector<Node> LawnNodes;
	const uint8_t Width;
	const uint8_t Height;
};

void ReportResults(const std::bitset<100>& results, const uint32_t testCaseCount)
{
	std::fstream out(s_outFilePath, std::fstream::out);
	for(uint32_t index = 0; index < testCaseCount; )
	{
		auto isSet = results.at(index);
		out << "Case #" << ++index << ": " << (isSet ? "YES" : "NO") << std::endl;
	}
}

bool IsLawnPossible(Lawn& lawn)
{
	if(lawn.Width == 1 || lawn.Height == 1)
	{
		return true;
	}

	while(!lawn.Queue.empty())
	{
		auto node = lawn.Queue.top();
		if(node->Visited != Node::NotVisited)
		{
			lawn.Queue.pop();
			continue;
		}

		auto possible = lawn.ExploreHorizontal(node) || lawn.ExploreVertical(node);
		if(!possible)
		{
			return false;
		}

		lawn.Queue.pop();
	}

	return true;
}

std::bitset<100> ParseInput(int argumentCount, char** arguments, uint32_t& caseCount)
{
	std::bitset<100> results;
	const char* testCasePath = nullptr;
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
		return results;
	}

	fileStream >> caseCount;
	for(uint32_t i = 0; i < caseCount && fileStream.good(); ++i)
	{
		uint32_t lawnWidth = 0, lawnHeight = 0, nodeCount = 0, oldHeight = 0;
		bool mowed = false;
		fileStream >> lawnHeight >> lawnWidth;
		Lawn lawn(static_cast<uint8_t>(lawnWidth), static_cast<uint8_t>(lawnHeight));
		for(auto& node : lawn.LawnNodes)
		{
			uint32_t height = 0;
			fileStream >> height;
			node.Initialize(static_cast<uint8_t>(height), static_cast<uint8_t>(nodeCount % lawnWidth), static_cast<uint8_t>(nodeCount / lawnWidth));
			lawn.Queue.push(&node);
			++nodeCount;
			mowed |= (oldHeight != 0) && (oldHeight != height);
			oldHeight = height;
		}

		if(!mowed || IsLawnPossible(lawn))
		{
			results.set(i);
		}
	}
	
	return results;
}


int main(int argumentCount, char** arguments)
{
	uint32_t testCaseCount = 0;
	auto results = ParseInput(argumentCount, arguments, testCaseCount);

	ReportResults(results, testCaseCount);

	return 0;
}