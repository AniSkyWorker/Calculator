#include "stdafx.h"
#include "StreamHandler.h"

using namespace std;
using namespace std::placeholders;

ÑStreamHandler::ÑStreamHandler(CCalculator calculator, std::istream &input, std::ostream &output)
	: m_calculator(calculator)
	, m_input(input)
	, m_output(output)
	, m_actionMap({
		{ "var", std::bind(&ÑStreamHandler::SetVar, this, std::placeholders::_1) },
		{ "let", std::bind(&ÑStreamHandler::LetVarValue, this, std::placeholders::_1) },
		{ "fn", std::bind(&ÑStreamHandler::SetFunction, this, std::placeholders::_1) },
		{ "print", std::bind(&ÑStreamHandler::PrintIdentifierValue, this, std::placeholders::_1) },
		{ "printvars", std::bind(&ÑStreamHandler::PrintVariables, this) },
		{ "printfns", std::bind(&ÑStreamHandler::PrintFunctions, this) }
})
{
}

bool ÑStreamHandler::HandleCommand()
{
	std::string commandLine;
	std::getline(m_input, commandLine);
	std::istringstream strm(commandLine);

	std::string action;
	strm >> action;

	auto it = m_actionMap.find(action);
	if (it != m_actionMap.end())
	{
		it->second(strm);
		return true;
	}

	return false;
}

void ÑStreamHandler::SetVar(std::istream &args)
{
	std::string identifier;
	args >> identifier;

	if (!m_calculator.SetVar(identifier))
	{
		std::cout << "Invalid name of varible!" << std::endl;
	}
}

void ÑStreamHandler::LetVarValue(std::istream &args)
{
	std::string expression;
	args >> expression;

	std::string fValue;
	std::string sValue;
	if (!ParseStrToValues(expression, fValue, sValue) || !m_calculator.LetVarValue(fValue, sValue))
	{ 
		std::cout << "Invalid expression!" << std::endl;
	}
}

void ÑStreamHandler::SetFunction(std::istream &args)
{
	std::string expression;
	args >> expression;

	std::string fValue, sValue, operand , tValue;
	Operator expressionOperator;
	if ((!ParseStrToValues(expression, fValue, sValue) || !m_calculator.SetFunction(fValue, sValue))
		&& (!ParseValueToArithmeticOperation(sValue, expression, expressionOperator, tValue) || !m_calculator.SetFunction(fValue, expression, expressionOperator, tValue)))
	{
		std::cout << "Invalid expression!" << std::endl;
	}
}

void ÑStreamHandler::PrintIdentifierValue(std::istream &args)
{
	std::string identifier;
	args >> identifier;

	std::cout << std::setprecision(2) << m_calculator.GetValue(identifier) << std::endl;
}

void ÑStreamHandler::PrintVariables()
{
	for (auto it : m_calculator.GetVars())
	{
		std::cout << it.first << ":" << std::setprecision(2) << it.second << std::endl;
	}
}

void ÑStreamHandler::PrintFunctions()
{
	for (auto it : m_calculator.GetFns())
	{
		std::cout << it.first << ":" << std::setprecision(2) << m_calculator.GetValue(it.first) << std::endl;
	}
}

bool ÑStreamHandler::ParseStrToValues(const std::string & str, std::string & firstValue, std::string & secondValue)
{
	auto parsePosition = str.find("=");

	if ((parsePosition == std::string::npos) || (parsePosition + 1 == str.size()))
	{
		return false;
	}

	firstValue = str.substr(0, parsePosition);
	secondValue = str.substr(parsePosition + 1, str.size());

	return true;
}

bool ÑStreamHandler::ParseValueToArithmeticOperation(const std::string & str,
	std::string & firstValue, Operator & operand, std::string & secondValue)
{
	auto operandPos = std::string::npos;

	for (auto elem : C_OPERATORS_SYMBOLS)
	{
		operandPos = str.find(elem.first);
		if (operandPos != std::string::npos)
		{
			operand = elem.second;
			break;
		}
	}

	if (operandPos != std::string::npos)
	{
		firstValue = str.substr(0, operandPos);
		secondValue = str.substr(operandPos + 1, str.size());
	}
	else
	{
		return false;
	}

	return true;
}
