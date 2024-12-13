#include "gtest.h"
#include "Translator.h"
#include "string"

using namespace std;

TEST(Number, can_create_number)
{
	ASSERT_NO_THROW(Number num(1.0));
}

TEST(Number, can_get_number_value)
{
	Number num(1.0);
	EXPECT_EQ(num.get_value(), 1.0);
}

TEST(Operation, can_create_operation)
{
	ASSERT_NO_THROW(Operation op(1.0));
}

TEST(Operation, can_get_operation)
{
	Operation op('*');
	EXPECT_EQ(op.get_op(), '*');
}

TEST(OpenBracket, can_create_open_bracket)
{
	ASSERT_NO_THROW(OpenBracket ob);
}

TEST(OpenBracket, can_get_open_bracket)
{
	OpenBracket ob;
	EXPECT_EQ(ob.get_op(), '(');
}

TEST(CloseBracket, can_create_close_bracket)
{
	ASSERT_NO_THROW(CloseBracket ob);
}

TEST(CloseBracket, can_get_close_bracket)
{
	CloseBracket cb;
	EXPECT_EQ(cb.get_op(), ')');
}

TEST(Translator, can_create_translator)
{
	ASSERT_NO_THROW(Translator expr);
}

TEST(Translator, can_create_translator_with_expression)
{
	ASSERT_NO_THROW(Translator expr);
}

TEST(Translator, throws_then_tokenize_non_correctly_syntax_expression)
{
	Translator expr("42+WW42+(4*2)");
	ASSERT_ANY_THROW(expr.tokenizer());
}

TEST(Translator, throws_then_parse_non_correctly_terms_expression)
{
	Translator expr("1+1(42+1)");
	expr.tokenizer();
	ASSERT_ANY_THROW(expr.parser());
}

TEST(Translator, throws_then_parse_non_correctly_brackets_expression)
{
	Translator expr("322+42)");
	expr.tokenizer();
	ASSERT_ANY_THROW(expr.parser());
}

TEST(Translator, throws_then_parse_non_correctly_brackets_order_expression)
{
	Translator T(")(()");
	T.tokenizer();
	ASSERT_ANY_THROW(T.parser());
}

TEST(Translator, throws_then_parse_non_correctly_first_symbol_expression)
{
	Translator expr(")1+1");
	expr.tokenizer();
	ASSERT_ANY_THROW(expr.parser());
}

TEST(Translator, throws_then_parse_non_correctly_last_symbol_expression)
{
	Translator expr("1+1*");
	expr.tokenizer();
	ASSERT_ANY_THROW(expr.parser());
}

TEST(Translator, throws_then_division_by_zero)
{
	Translator expr("52/(1-1)");
	ASSERT_ANY_THROW(expr.calculate());
}

TEST(Translator, can_create_expression)
{
	ASSERT_NO_THROW(Translator expr("1+2-1*2"));
}

TEST(Translator, combination_of_operations_without_errors)
{
	Translator expr("(5+4)+((10/5)+5*55)");
	ASSERT_NO_THROW(expr.calculate());
}

TEST(Translator, combination_of_operations_and_priority_with_correct_answer)
{
	Translator expr("(5+4)+((10/5)+5*55)");
	double q = (5 + 4) + ((10 / 5) + 5 * 55);
	EXPECT_EQ(q, expr.calculate());
}