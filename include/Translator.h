#pragma once

#include <iostream>
#include <map>
#include <string>
#include "Stack.h"

using namespace std;
enum types { number, operation, open_bracket, close_bracket };

class Term
{
protected:

	types type;
	map<char, size_t> pritorities{ {'*', 2}, {'/', 2}, {'+', 1}, {'-', 1} };

public:

	types get_type() const noexcept { return type; }

private:

	virtual void work() {} // the code works thanks to me
};

class Number : public Term
{

private:

	double value;

public:

	Number(double _value) : value(_value)
	{ type = number; }

	double get_value() const noexcept { return value; }
};

class Operation : public Term
{
private:

	char op;
	size_t priority;

public:

	Operation(char _op) : op(_op)
	{
		priority = pritorities[op];
		type = operation;
	}

	char get_op() const noexcept { return op; }
	size_t get_priority() const noexcept { return priority; }

};

class OpenBracket : public Term
{
private:

	char bracket;

public:

	OpenBracket() : bracket('(')
	{ type = open_bracket; }

	char get_op() const noexcept { return bracket; }
};

class CloseBracket : public Term
{
private:

	char bracket;

public:

	CloseBracket() : bracket(')')
	{ type = close_bracket; }

	char get_op() const noexcept { return bracket; }

};

class Translator
{

private:

	Vector<Term*> terms;
	Vector<Term*> polish_notation;
	string expression;

public:

	Translator(string _expression = "1+1") : expression(_expression) { }

	~Translator()
	{
		size_t sz = terms.size_();
		for (size_t i = 0; i < sz; ++i)
			delete terms[i];
	}

	size_t get_terms_size() const noexcept { return terms.size_(); }
	size_t get_expression_size() const noexcept { return expression.size(); }
	string get_expression() const noexcept { return expression; }

	void tokenizer()
	{
		string W;
		size_t number_status = 0;

		for (size_t i = 0; i < expression.size(); ++i)
		{
			if (!number_status)
			{
				if (expression[i] == '(')
					terms.push_back(new OpenBracket);
				else 
					if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/')
						terms.push_back(new Operation(expression[i]));
					else 
						if (expression[i] == ')')
							terms.push_back(new CloseBracket);
						else 
							if (expression[i] >= '0' && expression[i] <= '9' || expression[i] == '.')
							{
								number_status = 1;
								W += expression[i];
							}
							else
								throw ("Error");
			}
			else
			{
				if (expression[i] >= '0' && expression[i] <= '9' || expression[i] == '.')
					W += expression[i];
				else 
					if (expression[i] == '(')
					{
						number_status = 0;
						terms.push_back(new Number(stod(W)));
						terms.push_back(new OpenBracket);
						W.clear();
					}
					else 
						if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/')
						{
							number_status = 0;
							terms.push_back(new Number(stod(W)));
							terms.push_back(new Operation(expression[i]));
							W.clear();
						}
						else 
							if (expression[i] == ')')
							{
								number_status = 0;
								terms.push_back(new Number(stod(W)));
								terms.push_back(new CloseBracket);
								W.clear();
							}
							else 
								throw ("Error");
			}
		}

		if (!W.empty())
			terms.push_back(new Number(stod(W)));

	}

	void print_expression()
	{
		size_t sz = expression.size();

		for (size_t i = 0; i < sz; ++i)
		{
			cout << expression[i];
		}
		cout << "\n";
	}

	void parser()
	{
		map<types, size_t> states{ {number, 0}, {operation, 1}, {open_bracket, 2}, {close_bracket, 3} };
		size_t state;
		Stack<char> bracket;
		size_t sz = terms.size_();
		state = states[terms[0]->get_type()];

		if (state == 1)
			throw ("Operation can't be the first symbol!");

		if (state == 2)
			bracket.push('(');

		if (state == 3)
			throw ("')' can't be the first symbol!");

		for (size_t i = 1; i < sz; ++i)
		{
			if (terms[i]->get_type() == open_bracket)
				bracket.push('(');

			else 
				if (terms[i]->get_type() == close_bracket)
					if (!bracket.isEmpty())
						bracket.pop();
					else 
						throw ("Error");

			switch (state)
			{
			case 0:
				if (terms[i]->get_type() == open_bracket || terms[i]->get_type() == number)
					throw ("Error");
				state = states[terms[i]->get_type()];
				break;
			case 1:
				if (terms[i]->get_type() == operation || terms[i]->get_type() == close_bracket)
					throw ("Error");
				state = states[terms[i]->get_type()];
				break;
			case 2:
				if (terms[i]->get_type() == close_bracket || terms[i]->get_type() == operation)
					throw ("Error");
				state = states[terms[i]->get_type()];
				break;
			case 3:
				if (terms[i]->get_type() == number || terms[i]->get_type() == open_bracket)
					throw ("Error");
				state = states[terms[i]->get_type()];
				break;
			}
		}

		state = states[terms[sz - 1]->get_type()];

		if (state == 1 || state == 2)
			throw ("Error");

		if (!bracket.isEmpty())
			throw ("Error");
	}

	void converter()
	{
		Stack<Term*> st;
		size_t sz = terms.size_();

		for (size_t i = 0; i < sz; ++i)
		{
			if (terms[i]->get_type() == number)
				polish_notation.push_back(terms[i]);
			else 
				if (terms[i]->get_type() == operation)
				{
					if (st.isEmpty())
							st.push(terms[i]);
					else
					{
						while (!st.isEmpty() && st.top()->get_type() == operation && \
							dynamic_cast<Operation*>(terms[i])->get_priority() <= dynamic_cast<Operation*>(st.top())->get_priority())
						{
							polish_notation.push_back(st.top());
							st.pop();
						}
						st.push(terms[i]);
					}
				}
				else 
					if (terms[i]->get_type() == open_bracket)
						st.push(terms[i]);
					else 
						if (terms[i]->get_type() == close_bracket)
						{
							while (st.top()->get_type() != open_bracket)
							{
								polish_notation.push_back(st.top());
								st.pop();
							}
							st.pop();
						}
		}

		while (!st.isEmpty())
		{
			polish_notation.push_back(st.top());
			st.pop();
		}
	}

	string get_polish_notation()
	{
		string polish_notation_str;
		size_t sz = polish_notation.size_();

		for (size_t i = 0; i < sz; ++i)
		{
			if (polish_notation[i]->get_type() == number)
			{
				string num = std::to_string(dynamic_cast<Number*>(polish_notation[i])->get_value());
				num.erase(num.find_last_not_of('0') + 1, string::npos);
				num.erase(num.find_last_not_of('.') + 1, string::npos);
				polish_notation_str += num;
			}
			else  
				polish_notation_str += dynamic_cast<Operation*>(polish_notation[i])->get_op();
		}
		return polish_notation_str;
	}

	void print_polish_notation()
	{
		size_t sz = polish_notation.size_();
		for (size_t i = 0; i < sz; ++i)
		{
			if (polish_notation[i]->get_type() == number)
				cout << dynamic_cast<Number*>(polish_notation[i])->get_value();
			else 
				cout << dynamic_cast<Operation*>(polish_notation[i])->get_op();
		}
		cout << "\n";
	}

	double calculator()
	{
		types current_type;
		Stack<double> st;
		double right_numb, left_numb;
		size_t sz = polish_notation.size_();

		for (size_t i = 0; i < sz; ++i)
		{
			current_type = polish_notation[i]->get_type();
			if (current_type == number)
				st.push(dynamic_cast<Number*>(polish_notation[i])->get_value());
			else
			{
				right_numb = st.top();
				st.pop();
				left_numb = st.top();
				st.pop();

				switch (dynamic_cast<Operation*>(polish_notation[i])->get_op())
				{
				case '*':
					st.push(left_numb * right_numb);
					break;
				case '/':
					if (right_numb == 0)
						throw ("Error");
					st.push(left_numb / right_numb);
					break;
				case '+':
					st.push(left_numb + right_numb);
					break;
				case '-':
					st.push(left_numb - right_numb);
					break;
				}
			}
		}
		return st.top();
	}

	double calculate()
	{
		this->tokenizer();
		this->parser();
		this->converter();
		return this->calculator();
	}
};