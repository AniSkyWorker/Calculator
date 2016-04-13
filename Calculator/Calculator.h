#pragma once
#include "stdafx.h"

struct SFunctionData
{
	enum  Operator
	{
		Plus,
		Star,
		Slash,
		Minus
	};

	float value = FLT_MAX;
	std::string firstOperand;
	std::string secondOperand = "";
	Operator operatorType;
};

static const std::map<std::string, SFunctionData::Operator> C_OPERATORS_SYMBOLS
{
	{ "+", SFunctionData::Operator::Plus },
	{ "-", SFunctionData::Operator::Minus },
	{ "/", SFunctionData::Operator::Slash },
	{ "*", SFunctionData::Operator::Star }
};

class CCalculator
{
public:
	CCalculator() :m_variables(), m_functions() {};

	bool SetVar(const std::string & var);

	bool LetVarValue(const std::string & var, const std::string & otherVar);
	bool LetVarValue(const std::string & var, const float & value);

	bool SetFunction(const std::string & varFunction, const std::string & var);
	bool SetFunction(const std::string & varFunction, const std::string &firstOperand,
		const std::string & operatorFn, const std::string &secondOperand);

	std::string GetValue(const std::string & var);

	void PrintVars();
	void PrintFns();

private:
	void CalculateTwoOperandsFunction(SFunctionData & fnInfo);
	void CalculateFunctionValue(const std::string & function);

	float GetCalculatedValue(const std::string & name);

	bool IsVarExist (const std::string & var) const;
	bool IsFunctionExist (const std::string & nameFunction) const;
	bool IsNameCorrect (const std::string & identificator) const;

	std::function<bool(char)> IsNumericChar
		= [](const auto & chr)
	{ return (chr >= '0' && chr <= '9'); };

	std::function<bool(char)> IsCharCorrect
		= [](const auto & chr)
	{ return ((chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z') || (chr >= '_')) || (chr >= '0' && chr <= '9'); };

private:
	std::map<std::string, SFunctionData> m_functions;
	std::map<std::string, float> m_variables;
};

