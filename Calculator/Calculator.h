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

	double value = std::numeric_limits<double>::quiet_NaN();
	std::string firstOperand;
	std::string secondOperand;
	Operator operatorType;
};

static const std::map<std::string, SFunctionData::Operator> C_OPERATORS_SYMBOLS
{
	{ "+", SFunctionData::Operator::Plus },
	{ "-", SFunctionData::Operator::Minus },
	{ "/", SFunctionData::Operator::Slash },
	{ "*", SFunctionData::Operator::Star }
};

// TODO: add class CInterpreter
class CCalculator
{
public:
	CCalculator() :m_variables(), m_functions() {};

	bool SetVar(const std::string & var);

	// TODO: remove, use GetValue/LetVarValue
	bool LetVarValue(const std::string & var, const std::string & otherVar);
	bool LetVarValue(const std::string & var, const double & value);

	bool SetFunction(const std::string & varFunction, const std::string & var);
	bool SetFunction(const std::string & varFunction, const std::string &firstOperand,
		const std::string & operatorFn, const std::string &secondOperand);

	double GetValue(const std::string & var);

	void PrintVars();
	void PrintFns();

private:
	void CalculateTwoOperandsFunction(SFunctionData & fnInfo);
	void CalculateFunctionValue(const std::string & function);

	double GetCalculatedValue(const std::string & name);

	bool IsVarExist (const std::string & var) const;
	bool IsFunctionExist (const std::string & nameFunction) const;
	bool IsNameCorrect (const std::string & identificator) const;

	std::function<bool(char)> IsCharCorrect
		= [](const auto & chr)
	{ return std::isalnum(chr) || chr >= '_'; };

private:
	std::map<std::string, SFunctionData> m_functions;
	std::map<std::string, double> m_variables;
};

