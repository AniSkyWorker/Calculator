#include "stdafx.h"
#include "Calculator.h"

bool CCalculator::SetVar(const std::string & var)
{
	if (!IsNameCorrect(var) || IsFunctionExist(var) || IsVarExist(var))
	{
		return false;
	}

	m_variables.insert(std::make_pair(var, std::numeric_limits<double>::quiet_NaN()));
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

	if (IsFunctionExist(secondValue))
	{
		m_variables[firstVar] = GetCalculatedValue(secondValue);
	}
	else if (IsVarExist(secondValue))
	{
		m_variables[firstVar] = m_variables[secondValue];
	}
	else
	{
		char *end;
		auto value = strtod(secondValue.c_str(), &end);
		if (value != NULL)
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
	CalculateFunctionValue(fnId);
	return true;
}

bool CCalculator::SetFunction(const std::string & fnId, const std::string & firstOperand, 
	const std::string & fnOperator, const std::string & secondOperand)
{
	if (IsVarExist(fnId) || IsFunctionExist(fnId)|| !IsNameCorrect(fnId) ||
		firstOperand.empty() || fnOperator.empty() || secondOperand.empty())
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

	m_functions.insert(make_pair(fnId, fnInfo));
	CalculateFunctionValue(fnId);
	return true;
}

double CCalculator::GetCalculatedValue(const std::string & fnId) const
{
	if (IsFunctionExist(fnId))
	{
		return m_functions.at(fnId).value;
	}
	else if (IsVarExist(fnId))
	{
		return m_variables.at(fnId);
	}

	return std::numeric_limits<double>::quiet_NaN();
}

void CCalculator::CalculateTwoOperandsFunction(SFunctionData & fnInfo)
{
	double firstOperand = GetCalculatedValue(fnInfo.firstOperand);
	double secondOperand = GetCalculatedValue(fnInfo.secondOperand);

	if (firstOperand == firstOperand && secondOperand == secondOperand)
	{
		double result;
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

double CCalculator::GetValue(const std::string & var) const
{
	if (IsFunctionExist(var))
	{
		return GetCalculatedValue(var);
	}

	if (IsVarExist(var))
	{
		return m_variables.at(var);
	}

	return std::numeric_limits<double>::quiet_NaN();
}


std::map<std::string, double> CCalculator::GetVars() const
{
	return m_variables;
}

std::map<std::string, SFunctionData> CCalculator::GetFns() const
{
	return m_functions;
}

void CCalculator::CalculateFunctionValue(const std::string & function)
{
	if (IsFunctionExist(function))
	{
		if (!m_functions[function].secondOperand.empty())
		{
			CalculateTwoOperandsFunction(m_functions[function]);
		}
		else
		{
			auto firstOperand = m_functions[function].firstOperand;

			if (IsFunctionExist(firstOperand))
			{
				m_functions[function] = m_functions[firstOperand];
			}
			else
			{
				m_functions[function].value = m_variables[firstOperand];
			}
		}
	}
}