#pragma once
#include "stdafx.h"

enum class Operator
{
	Plus,
	Star,
	Slash,
	Minus,
	None
};

struct SFunctionData
{
	SFunctionData()
		: operatorType(Operator::None)
	{}

	double value = std::numeric_limits<double>::quiet_NaN();
	std::string firstOperand;
	std::string secondOperand;
	Operator operatorType;
};

static const std::map<std::string, Operator> C_OPERATORS_SYMBOLS
{
	{ "+", Operator::Plus },
	{ "-", Operator::Minus },
	{ "/", Operator::Slash },
	{ "*", Operator::Star }
};

class CCalculator
{
public:
	CCalculator() :m_variables(), m_functions() {};

	bool SetVar(const std::string & var);

	bool LetVarValue(const std::string & var, const std::string & otherVar);

	bool SetFunction(const std::string & varFunction, const std::string & var);
	bool SetFunction(const std::string & varFunction, const std::string &firstOperand,
		const Operator & operatorFn, const std::string &secondOperand);

	double GetValue(const std::string & var) const;
	const std::map<std::string, double> & GetVars() const;
	const std::map<std::string, SFunctionData> & GetFns() const;

private:
	void CalculateTwoOperandsFunction(SFunctionData & fnInfo);
	void CalculateFunctionValue(const std::string & function);

	bool IsVarExist (const std::string & var) const;
	bool IsFunctionExist (const std::string & nameFunction) const;
	bool IsNameCorrect (const std::string & identificator) const;

private:
	std::map<std::string, SFunctionData> m_functions;
	std::map<std::string, double> m_variables;
};

