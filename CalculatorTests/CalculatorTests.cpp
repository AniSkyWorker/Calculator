#include "stdafx.h"
#include "../Calculator/calculator.h"

struct CalculatorFixture
{
	CCalculator calc;
};

BOOST_FIXTURE_TEST_SUITE(Calculator_, CalculatorFixture)

	BOOST_AUTO_TEST_CASE(can_set_new_var)
	{
		BOOST_CHECK(calc.SetVar("y"));
	}

	BOOST_AUTO_TEST_CASE(value_of_not_declared_var_equal_to_NAN)
	{
		BOOST_CHECK(calc.GetValue("y") != calc.GetValue("y"));
	}

	BOOST_AUTO_TEST_CASE(if_had_letted_value_to_var_that_not_declared_this_var_will_be_declare)
	{
		BOOST_CHECK(calc.GetValue("x") != calc.GetValue("x"));
		BOOST_CHECK(calc.LetVarValue("x", "123"));
		BOOST_CHECK_EQUAL(calc.GetValue("x"), 123);
	}

	BOOST_AUTO_TEST_CASE(name_of_var_cant_be_empty_or_start_from_digit)
	{
		BOOST_CHECK(!calc.SetVar(""));
		BOOST_CHECK(!calc.SetVar("12qww"));
	}

	BOOST_AUTO_TEST_CASE(can_declare_function_with_non_existent_var)
	{
		BOOST_CHECK(calc.SetFunction("function", "z", "+", "f"));
		BOOST_CHECK(calc.GetValue("fuction") != calc.GetValue("fuction"));
	}

	BOOST_AUTO_TEST_CASE(function_name_can_not_be_empty)
	{
		BOOST_CHECK(calc.SetVar("var"));
		BOOST_CHECK(!calc.SetFunction("", "var", "", ""));
	}

	struct when_var_declared_ : CalculatorFixture
	{
		when_var_declared_()
		{
			calc.SetVar("y");
		}
	};

	BOOST_FIXTURE_TEST_SUITE(declare_new_var, when_var_declared_)

		BOOST_AUTO_TEST_CASE(var_value_default_equal_to_NAN)
		{
			BOOST_CHECK(calc.GetValue("y") != calc.GetValue("y"));
		}

		BOOST_AUTO_TEST_CASE(can_set_new_value_to_var)
		{
			BOOST_CHECK(calc.LetVarValue("y", "15"));
			BOOST_CHECK_EQUAL(calc.GetValue("y"), 15);
		}

		BOOST_AUTO_TEST_CASE(cant_set_var_with_same_id_again)
		{
			BOOST_CHECK(!calc.SetVar("y"));
		}

	BOOST_AUTO_TEST_SUITE_END()

	struct when_declared_some_vars_ : CalculatorFixture
	{
		when_declared_some_vars_()
		{
			calc.LetVarValue("y", "10");
			calc.LetVarValue("x", "30");
		}
	};

	BOOST_FIXTURE_TEST_SUITE(when_declared_some_vars, when_declared_some_vars_)

		BOOST_AUTO_TEST_CASE(can_set_var_value_of_another_var)
		{
			BOOST_CHECK_EQUAL(calc.GetValue("y"), 10);
			BOOST_CHECK_EQUAL(calc.GetValue("x"), 30);

			BOOST_CHECK(calc.LetVarValue("y", "x"));
			BOOST_CHECK_EQUAL(calc.GetValue("y"), 30);
		}

		BOOST_AUTO_TEST_CASE(cant_set_function_with_invalid_operand)
		{
			BOOST_CHECK(!calc.SetFunction("function", "&", "y", ""));
		}

		BOOST_AUTO_TEST_CASE(can_set_function_with_one_paramether)
		{
			BOOST_CHECK(calc.SetFunction("function", "x"));
			BOOST_CHECK_EQUAL(calc.GetValue("function"), calc.GetValue("x"));
		}

		BOOST_AUTO_TEST_CASE(cant_set_function_with_correct_operand)
		{
			BOOST_CHECK(calc.SetFunction("function", "x", "*", "x"));
			BOOST_CHECK_EQUAL(calc.GetValue("function"), 900);
		}

	BOOST_AUTO_TEST_SUITE_END()

	struct when_function_declared_ : when_declared_some_vars_
	{
		when_function_declared_()
		{
			calc.SetFunction("SumXandY", "x", "+", "y");
		}
	};

	BOOST_FIXTURE_TEST_SUITE(when_function_declared, when_function_declared_)

		BOOST_AUTO_TEST_CASE(can_get_calculated_value_of_fn)
		{
			BOOST_CHECK_EQUAL(calc.GetValue("SumXandY"), 40);
		}

		BOOST_AUTO_TEST_CASE(cant_set_fn_with_same_name)
		{
			BOOST_CHECK(!calc.SetFunction("SumXandY", "x", "+", "y"));
		}

		BOOST_AUTO_TEST_CASE(can_not_declare_function_if_her_name_is_busy)
		{
			BOOST_CHECK(!calc.SetFunction("x", "y", "", ""));
		}

	BOOST_AUTO_TEST_SUITE_END()

	struct when_declared_some_functions_ : when_function_declared_
	{
		when_declared_some_functions_()
		{
			calc.LetVarValue("z", "4");
			calc.SetFunction("DivSumXandYonZ", "SumXandY", "/", "z");
		}
	};

	BOOST_FIXTURE_TEST_SUITE(when_declared_some_functions, when_declared_some_functions_)

		BOOST_AUTO_TEST_CASE(can_get_ccalculated_value_fn_that_depend_on_another_fn)
		{
			BOOST_CHECK_EQUAL(calc.GetValue("DivSumXandYonZ"), 10);
		}

		BOOST_AUTO_TEST_CASE(var_can_store_value_of_function)
		{
			BOOST_CHECK(calc.LetVarValue("XandYdivZ", "DivSumXandYonZ"));
			BOOST_CHECK_EQUAL(calc.GetValue("XandYdivZ"), 10);
		}

		BOOST_AUTO_TEST_CASE(function_change_its_value_if_value_of_var_that_affect_on_fn_was_changed)
		{
			BOOST_CHECK(calc.LetVarValue("z", "10"));
			BOOST_CHECK_EQUAL(calc.GetValue("DivSumXandYonZ"), 4);
		}

	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()