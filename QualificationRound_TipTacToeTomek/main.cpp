
/*
	Tip-Tac-Toe-Tomek
	https://code.google.com/codejam/contest/2270488/dashboard
	Problem

	Tic-Tac-Toe-Tomek is a game played on a 4 x 4 square board. The board starts empty, except that a single 'T' symbol may appear in one of the 16 squares. There are two players: X and O. They take turns to make moves, with X starting. In each move a player puts her symbol in one of the empty squares. Player X's symbol is 'X', and player O's symbol is 'O'.

	After a player's move, if there is a row, column or a diagonal containing 4 of that player's symbols, or containing 3 of her symbols and the 'T' symbol, she wins and the game ends. Otherwise the game continues with the other player's move. If all of the fields are filled with symbols and nobody won, the game ends in a draw. See the sample input for examples of various winning positions.

	Given a 4 x 4 board description containing 'X', 'O', 'T' and '.' characters (where '.' represents an empty square), describing the current state of a game, determine the status of the Tic-Tac-Toe-Tomek game going on. The statuses to choose from are:

	"X won" (the game is over, and X won)
	"O won" (the game is over, and O won)
	"Draw" (the game is over, and it ended in a draw)
	"Game has not completed" (the game is not over yet)
	If there are empty cells, and the game is not over, you should output "Game has not completed", even if the outcome of the game is inevitable.
	Input

	The first line of the input gives the number of test cases, T. T test cases follow. Each test case consists of 4 lines with 4 characters each, with each character being 'X', 'O', '.' or 'T' (quotes for clarity only). Each test case is followed by an empty line.

	Output

	For each test case, output one line containing "Case #x: y", where x is the case number (starting from 1) and y is one of the statuses given above. Make sure to get the statuses exactly right. When you run your code on the sample input, it should create the sample output exactly, including the "Case #1: ", the capital letter "O" rather than the number "0", and so on.

	Limits

	The game board provided will represent a valid state that was reached through play of the game Tic-Tac-Toe-Tomek as described above.

*/

#include <array>
#include <fstream>
#include <stdint.h>
#include <vector>

static auto s_defaultSmallTestCasePath = "TestCases/A-small-practice.in";
static auto s_defaultLargeTestCasePath = "TestCases/A-large-practice.in";

static auto s_outFilePath = "out.txt";

static const uint32_t s_maskCount = 10;

static uint32_t s_xWonMasks[s_maskCount] = 
{
	//Row masks										Column masks									Diagonal masks
	0x0000000F, 0x000000F0, 0x00000F00, 0x0000F000, 0x00001111, 0x00002222, 0x00004444, 0x00008888, 0x00008421, 0x00001248 
};

static uint32_t s_oWonMasks[s_maskCount] = 
{ 
	//Row masks										Column masks									Diagonal masks
	0x000F0000, 0x00F00000, 0x0F000000, 0xF0000000, 0x11110000, 0x22220000, 0x44440000, 0x88880000, 0x84210000, 0x12480000
};

typedef std::array<char, 20> GameBuffer;

struct Game
{
	enum Result : uint8_t
	{
		Uninitialized,
		XWon,
		OWon,
		Draw,
		NotOver
	};

	uint32_t State;
	Result Result;
	bool HasEmptySpaces;

	const char* ResultToString() const 
	{
		const char* toReturn = nullptr;
		switch(Result)
		{
		case Uninitialized:
			toReturn = "Uninitialized";
			break;
		case XWon:
			toReturn = "X won";
			break;
		case OWon:
			toReturn = "O won";
			break;
		case Draw:
			toReturn = "Draw";
			break;
		case NotOver:
			toReturn = "Game has not completed";
			break;
		}
		return toReturn;
	}

	Game()
		: State(0)
		, Result(Uninitialized)
		, HasEmptySpaces(false)
	{}

	Game(const GameBuffer& buffer)
		: State(0)
		, Result(Uninitialized)
		, HasEmptySpaces(false)
	{
		uint32_t position = 0;
		for(decltype(buffer.size()) i = 0; 
			i < buffer.size();
			++i, ++position)
		{
			uint32_t toOr = 0;
			switch(buffer[i])
			{
			case 'X':
				toOr = 1 << position;
				break;
			case 'O':
				toOr = 1 << (position + 16);
				break;
			case 'T':
				toOr = (1 << position) | (1 << (position + 16));	
				break;
			case '.':
				HasEmptySpaces = true;
				break;
			default:
				--position;
				break;
			}
			State |= toOr;
		}
	}
};

std::vector<Game> ParseInput(int argumentCount, char** arguments)
{
	const char* testCasePath = nullptr;
	std::vector<Game> games;
	if (argumentCount <= 1)
	{
		testCasePath = s_defaultLargeTestCasePath;
	}
	else if (argumentCount == 2)
	{
		testCasePath = arguments[1];
	}

	GameBuffer buffer;
	GameBuffer trash;
	uint32_t testCases = 0;
	std::ifstream fileStream;
	fileStream.open(testCasePath, std::fstream::in);
	if (!fileStream.good())
	{
		return games;
	}

	fileStream >> testCases;
	fileStream.ignore(1,'f');
	buffer.assign('\0');
	for(uint32_t i = 0; i < testCases && fileStream.good(); ++i)
	{
		fileStream.getline(buffer.data(), 20);
		fileStream.getline(buffer.data()+4, 16);
		fileStream.getline(buffer.data()+8, 12);
		fileStream.getline(buffer.data()+12, 8);
		fileStream.getline(trash.data(), 20);
		games.emplace_back(Game(buffer));
	}

	return games;
}

void EvaluateGames(std::vector<Game>& games)
{
	for(auto& game : games)
	{
		auto result = Game::Uninitialized;
		for(uint32_t i = 0; i < s_maskCount && result == Game::Uninitialized; ++i)
		{
			if ((game.State & s_xWonMasks[i]) == s_xWonMasks[i])
			{
				result = Game::XWon;
			}
			else if ((game.State & s_oWonMasks[i]) == s_oWonMasks[i])
			{
				result = Game::OWon;
			}
		}
		game.Result = result == Game::Uninitialized
						? (game.HasEmptySpaces ? Game::NotOver : Game::Draw)
						: result;
	}
}

void ReportResults(const std::vector<Game>& games)
{
	std::fstream out(s_outFilePath, std::fstream::out);
	uint32_t gameCount = 1;
	for(auto& game : games)
	{
		out << "Case #" << gameCount++ << ": " << game.ResultToString() << std::endl;
	}
}

int main(int argumentCount, char** arguments)
{
	auto games = ParseInput(argumentCount, arguments);
	
	EvaluateGames(games);

	ReportResults(games);

	return 0;
}