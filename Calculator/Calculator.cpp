#include "stdafx.h"
#include "Calculator.h"

bool CCalculator::SetVar(const std::string & var)
{
	if (!IsNameCorrect(var) || IsFunctionExist(var) || IsVarExist(var))
	{
		return false;
	}

	m_variables.insert(std::make_pair(var, FLT_MAX));
	return true;
}

bool CCalculator::IsNameCorrect(const std::string & id) const
{
	if (id.empty() || IsNumericChar(*id.begin()) || !std::all_of(id.begin(), id.end(), IsCharCorrect))
	{
		return false;
	}

	return true;
}

bool CCalculator::LetVarValue(const std::string & firstVar, const std::string & secondVar)
{
	if (secondVar.empty() || (!IsVarExist(secondVar) && !IsFunctionExist(secondVar)))
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

	if (IsFunctionExist(secondVar))
	{
		m_variables[firstVar] = GetCalculatedValue(secondVar);
	}
	else
	{
		m_variables[firstVar] = m_variables[secondVar];
	}

	return true;
}

bool CCalculator::LetVarValue(const std::string & var, const float & value)
{
	if (IsFunctionExist(var))
	{
		return false;
	}

	if (!IsVarExist(var))
	{
		if (!SetVar(var))
		{
			return false;
		}
	}

	m_variables[var] = value;
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

float CCalculator::GetCalculatedValue(const std::string & fnId)
{
	if (IsFunctionExist(fnId))
	{
		return m_functions[fnId].value;
	}
	else if (IsVarExist(fnId))
	{
		return m_variables[fnId];
	}

	return FLT_MAX;
}

void CCalculator::CalculateTwoOperandsFunction(SFunctionData & fnInfo)
{
	float firstOperand = GetCalculatedValue(fnInfo.firstOperand);
	float secondOperand = GetCalculatedValue(fnInfo.secondOperand);

	if (firstOperand != FLT_MAX && secondOperand != FLT_MAX)
	{
		float result;
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

std::string GetFormatedString(const double & value)
{
	return str(boost::format("%.2f") % value);
}

std::string CCalculator::GetValue(const std::string & var)
{
	if (IsFunctionExist(var))
	{
		auto value = GetCalculatedValue(var);
		return value != FLT_MAX ? GetFormatedString(value) : "nan";
	}

	if (IsVarExist(var))
	{
		return m_variables[var] != FLT_MAX ? GetFormatedString(m_variables[var]) : "nan";
	}

	return "";
}


void CCalculator::PrintVars()
{
	for (auto it : m_variables)
	{
		std::cout << it.first << ":" << (it.second != FLT_MAX ? GetFormatedString(it.second) : "nan") << std::endl;
	}
}

void CCalculator::PrintFns() 
{
	for (auto it : m_functions)
	{
		auto id = it.first;
		auto value = GetCalculatedValue(id);
		std::cout << id << ":" << (value != FLT_MAX ? GetFormatedString(GetCalculatedValue(id)) : "nan") << std::endl;
	}
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