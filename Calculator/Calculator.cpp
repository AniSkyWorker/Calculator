#include "stdafx.h"
#include "Calculator.h"

namespace
{
	bool IsNan(const double & numeric)
	{
		return numeric != numeric;
	}

	bool IsCharCorrect(char ch)
	{
		return std::isalnum(ch) || ch >= '_';
	}
}

bool CCalculator::SetVar(const std::string & var)
{
	if (!IsNameCorrect(var) || IsFunctionExist(var) || IsVarExist(var))
	{
		return false;
	}

	m_variables.insert({ var, std::numeric_limits<double>::quiet_NaN() });
	return true;
}

bool CCalculator::IsNameCorrect(const std::string & id) const
{
	if (id.empty() || std::isdigit(*id.begin()) || !std::all_of(id.begin(), id.end(), IsCharCorrect))
	{
		return false;
	}

	return true;
}

bool CCalculator::LetVarValue(const std::string & firstVar, const std::string & secondValue)
{
	if (secondValue.empty())
	{
		return false;
	}

	if (!IsVarExist(firstVar))
	{
		if (!SetVar(firstVar))
		{
			return false;
		}
	}

	m_variables[firstVar] = GetValue(secondValue);
	if (m_variables[firstVar] != m_variables[firstVar])
	{
		char *end = nullptr;
		auto value = strtod(secondValue.c_str(), &end);
		if (*end == '\0')
		{
			m_variables[firstVar] = std::atof(secondValue.c_str());
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool CCalculator::IsVarExist(const std::string & id) const
{
	return m_variables.find(id) != m_variables.end();
}

bool CCalculator::IsFunctionExist(const std::string & id) const
{
	return m_functions.find(id) != m_functions.end();
}

bool CCalculator::SetFunction(const std::string & fnId, const std::string & varId)
{
	if (IsVarExist(fnId) || IsFunctionExist(fnId) || !IsVarExist(varId) || !IsNameCorrect(fnId))
	{
		return false;
	}

	SFunctionData fnInfo;
	fnInfo.firstOperand = varId;
	m_functions.insert(make_pair(fnId, fnInfo));
	return true;
}

bool CCalculator::SetFunction(const std::string & fnId, const std::string & firstOperand, 
	const std::string & fnOperator, const std::string & secondOperand)
{
	if (IsVarExist(fnId)
		|| IsFunctionExist(fnId)
		|| !IsNameCorrect(fnId) 
		|| (!IsNameCorrect(firstOperand))
		|| fnOperator.empty()
		|| (!IsNameCorrect(secondOperand)))
	{
		return false;
	}

	auto symbol = C_OPERATORS_SYMBOLS.find(fnOperator);
	if (symbol == C_OPERATORS_SYMBOLS.end())
	{
		return false;
	}

	SFunctionData fnInfo;
	fnInfo.firstOperand = firstOperand;
	fnInfo.secondOperand = secondOperand;
	fnInfo.operatorType = symbol->second;

	m_functions.insert({ fnId, fnInfo });
	return true;
}

void CCalculator::CalculateTwoOperandsFunction(SFunctionData & fnInfo)
{
	double firstOperand = GetValue(fnInfo.firstOperand);
	double secondOperand = GetValue(fnInfo.secondOperand);

	if (!IsNan(firstOperand) && !IsNan(secondOperand))
	{
		double result = std::numeric_limits<double>::quiet_NaN();
		switch (fnInfo.operatorType)
		{
		case SFunctionData::Operator::Plus:
			result = firstOperand + secondOperand;
			break;
		case SFunctionData::Operator::Slash:
			result = firstOperand / secondOperand;
			break;
		case SFunctionData::Operator::Star:
			result = firstOperand * secondOperand;
			break;
		case SFunctionData::Operator::Minus:
			result = firstOperand - secondOperand;
			break;
		}

		fnInfo.value = result;
	}
}

double CCalculator::GetValue(const std::string & var)
{
	if (IsFunctionExist(var))
	{
		CalculateFunctionValue(var);
		return m_functions.at(var).value;;
	}

	if (IsVarExist(var))
	{
		return m_variables.at(var);
	}

	return std::numeric_limits<double>::quiet_NaN();
}


const std::map<std::string, double> & CCalculator::GetVars() const
{
	return m_variables;
}

const std::map<std::string, SFunctionData> & CCalculator::GetFns() const
{
	return m_functions;
}

void CCalculator::CalculateFunctionValue(const std::string & functionName)
{
	if (IsFunctionExist(functionName))
	{
		auto & func = m_functions[functionName];
		if (!(func.operatorType == SFunctionData::Operator::None))
		{
			CalculateTwoOperandsFunction(func);
		}
		else
		{
			func.value = GetValue(func.firstOperand);
		}
	}
}