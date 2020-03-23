#pragma once
#include <string>
#include <vector>
#include <exception>
#define CMDFormatException 0
#define FileNotFoundException 1
#define FileFormatException 2
#define CoordinateLimitExceedException 3
#define LineDefinitionException 4
#define CircleDefinitionException 5
#define InfinateIntersctionException 6
using namespace std;


class Exception
{
public:
	int m_excType;
	string m_excMsg;

	Exception(int excType, string msg) : m_excType(excType), m_excMsg(msg) {}

	void print()
	{
		cout << m_exc[m_excType] << ": " << m_excMsg << endl;
		return;
	}

private:
	vector<string> m_exc = {
		"CMDFormatException",
		"FileNotFoundException",
		"FileFormatException",
		"CoordinateLimitExceedException",
		"LineDefinitionException",
		"CircleDefinitionException",
		"InfinateIntersctionException"
	};
};
