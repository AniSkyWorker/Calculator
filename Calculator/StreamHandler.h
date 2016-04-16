#pragma once
#include <boost/noncopyable.hpp>
#include <functional>
#include <sstream>
#include "Calculator.h"

class ÑStreamHandler : boost::noncopyable
{
public:
	ÑStreamHandler(CCalculator calculator, std::istream &input, std::ostream &output);
	bool HandleCommand();

private:
	void SetVar(std::istream & args);
	void LetVarValue(std::istream & args);
	void SetFunction(std::istream &args);

	void PrintIdentifierValue(std::istream &args);

	void PrintVariables();
	void PrintFunctions();

	bool ParseStrToValues(const std::string & str, std::string & firstValue, std::string & secondValue);
	bool ParseValueToArithmeticOperation(const std::string & str, std::string & firstValue, std::string & operand, std::string & secondValue);
private:
	typedef std::map<std::string, std::function<void(std::istream & args) >> ActionMap;

	std::istream &m_input;
	std::ostream &m_output;

	CCalculator m_calculator;
	const ActionMap m_actionMap;
};