/************************************************
 * Maths utility library.  Used for NVTrapSetQVar
 */

#ifndef NVMATHS_H
#define NVMATHS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <stack>
#include <queue>
#include <map>
#include <sstream>
#include <cmath>

#include <lg/interface.h>
#include <lg/scrservices.h>
#include <lg/scrmanagers.h>


/*
 * Maths classes.
 *
 * The main class is NVMathsOperations, which is a singleton containing an overloaded calculate() function.
 *
 * calculate(operator, op1, op2, reverse = false)
 * calculateReverse(operator, op1, op2)
 *		perform a single calculation with the operator and two operands supplied
 *
 * calculate(expression)
 *		parse and evaluate a string containing a mathematical expression
 *
 */

class BinaryOperator
{
public:
	virtual char getSymbol() = 0;
	virtual int getPriority() = 0;
	virtual bool getRightAssociativity() { return false; }

	virtual int calculate(int x, int y) = 0;
	virtual int calculateReverse(int x, int y) = 0;
};

class BinaryOperatorPlus : public BinaryOperator
{
public:
	virtual char getSymbol() { return '+'; }
	virtual int getPriority() { return 3; }
	virtual int calculate(int x, int y)
	{
		return x + y;
	}
	virtual int calculateReverse(int x, int y)
	{
		return x - y;
	}
};

class BinaryOperatorMinus : public BinaryOperator
{
public:
	virtual char getSymbol() { return '-'; }
	virtual int getPriority() { return 3; }
	virtual int calculate(int x, int y)
	{
		return x - y;
	}
	virtual int calculateReverse(int x, int y)
	{
		return x + y;
	}
};

class BinaryOperatorMultiply : public BinaryOperator
{
public:
	virtual char getSymbol() { return '*'; }
	virtual int getPriority() { return 4; }
	virtual int calculate(int x, int y)
	{
		return x * y;
	}
	virtual int calculateReverse(int x, int y)
	{
		if ( y != 0 )
		{
			return x / y;
		} else {
			DebugPrintf("NVScript Maths Error: Divide by zero requested in reversed multiply operation (operands: %i, %i)", x, y);
			return 0;
		}
	}
};

class BinaryOperatorDivide : public BinaryOperator
{
public:
	virtual char getSymbol() { return '/'; }
	virtual int getPriority() { return 4; }
	virtual int calculate(int x, int y)
	{
		if ( y != 0 )
		{
			return x / y;
		} else {
			DebugPrintf("NVScript Maths Error: Divide by zero requested in divide operation (operands: %i, %i)", x, y);
			return 0;
		}
	}
	virtual int calculateReverse(int x, int y)
	{
		return x * y;
	}
};

class BinaryOperatorModulus : public BinaryOperator
{
public:
	virtual char getSymbol() { return '%'; }
	virtual int getPriority() { return 4; }
	virtual int calculate(int x, int y)
	{
		if ( y != 0 )
		{
			return x % y;
		} else {
			DebugPrintf("NVScript Maths Error: Divide by zero requested in modulus operation (operands: %i, %i)", x, y);
			return 0;
		}
	}
	virtual int calculateReverse(int x, int y)
	{
		return x * y;
	}
};

class BinaryOperatorPower : public BinaryOperator
{
public:
	virtual char getSymbol() { return '^'; }
	virtual int getPriority() { return 5; }
	virtual int calculate(int x, int y)
	{
		return (int)pow((float)x, (float)y);
	}
	virtual int calculateReverse(int x, int y)
	{
		if ( y != 0 )
		{
			return (int)pow((float)x, 1/(float)y);
		} else {
			DebugPrintf("NVScript Maths Error: Divide by zero requested in reversed power operation (operands: %i, %i)", x, y);
			return 0;
		}
	}
};

class BinaryOperatorShiftLeft : public BinaryOperator
{
public:
	virtual char getSymbol() { return '{'; }
	virtual int getPriority() { return 2; }
	virtual int calculate(int x, int y)
	{
		return x << y;
	}
	virtual int calculateReverse(int x, int y)
	{
		return x >> y;
	}
};

class BinaryOperatorShiftRight : public BinaryOperator
{
public:
	virtual char getSymbol() { return '}'; }
	virtual int getPriority() { return 2; }
	virtual int calculate(int x, int y)
	{
		return x >> y;
	}
	virtual int calculateReverse(int x, int y)
	{
		return x << y;
	}
};

class BinaryOperatorRandomAdd : public BinaryOperator
{
public:
	virtual char getSymbol() { return '?'; }
	virtual int getPriority() { return 3; }
	virtual int calculate(int x, int y)
	{
		SService<IDataSrv> pDataSrv(g_pScriptManager);
		return x + pDataSrv->RandInt(0, y);
	}
	virtual int calculateReverse(int x, int y)
	{
		SService<IDataSrv> pDataSrv(g_pScriptManager);
		return x - pDataSrv->RandInt(0, y);
	}
};

class BinaryOperatorRandomAdd1 : public BinaryOperator
{
public:
	virtual char getSymbol() { return 'd'; }
	virtual int getPriority() { return 3; }
	virtual int calculate(int x, int y)
	{
		SService<IDataSrv> pDataSrv(g_pScriptManager);
		return x + pDataSrv->RandInt(1, y);
	}
	virtual int calculateReverse(int x, int y)
	{
		SService<IDataSrv> pDataSrv(g_pScriptManager);
		return x - pDataSrv->RandInt(1, y);
	}
};

class BinaryOperatorRandomSet : public BinaryOperator
{
public:
	virtual char getSymbol() { return 'r'; }
	virtual int getPriority() { return 3; }
	virtual int calculate(int x, int y)
	{
		SService<IDataSrv> pDataSrv(g_pScriptManager);
		return pDataSrv->RandInt(0, y);
	}
	virtual int calculateReverse(int x, int y)
	{
		return 0;
	}
};

class BinaryOperatorBitwiseOr : public BinaryOperator
{
public:
	virtual char getSymbol() { return '|'; }
	virtual int getPriority() { return 1; }
	virtual int calculate(int x, int y)
	{
		return x | y;
	}
	virtual int calculateReverse(int x, int y)
	{
		return x & (~y);
	}
};

class BinaryOperatorBitwiseOr2 : public BinaryOperatorBitwiseOr
{
public:
	virtual char getSymbol() { return '!'; }
};

class BinaryOperatorAppend : public BinaryOperator
{
public:
	virtual char getSymbol() { return '"'; }
	virtual int getPriority() { return 3; }
	virtual int calculate(int x, int y)
	{
		return ((x * 10) + (y % 10)) % 100000000; // Only keep 8 digits
	}
	virtual int calculateReverse(int x, int y)
	{
		return x / 10;
	}
};

class BinaryOperatorAssign : public BinaryOperator
{
public:
	virtual char getSymbol() { return '='; }
	virtual int getPriority() { return 1; }
	virtual int calculate(int x, int y)
	{
		return y;
	}
	virtual int calculateReverse(int x, int y)
	{
		return 0;
	}
};

class NVMathsOperations
{
private:
	std::map<char, BinaryOperator*> operators;

	void AddBinaryOperator(BinaryOperator* p)
	{
		operators[p->getSymbol()] = p;
	}

	NVMathsOperations()
	{
		AddBinaryOperator(new BinaryOperatorPlus());
		AddBinaryOperator(new BinaryOperatorMinus());
		AddBinaryOperator(new BinaryOperatorMultiply());
		AddBinaryOperator(new BinaryOperatorDivide());
		AddBinaryOperator(new BinaryOperatorModulus());
		AddBinaryOperator(new BinaryOperatorPower());
		AddBinaryOperator(new BinaryOperatorShiftLeft());
		AddBinaryOperator(new BinaryOperatorShiftRight());
		AddBinaryOperator(new BinaryOperatorRandomAdd());
		AddBinaryOperator(new BinaryOperatorRandomAdd1);
		AddBinaryOperator(new BinaryOperatorRandomSet);
		AddBinaryOperator(new BinaryOperatorBitwiseOr());
		AddBinaryOperator(new BinaryOperatorBitwiseOr2());
		AddBinaryOperator(new BinaryOperatorAppend);
		AddBinaryOperator(new BinaryOperatorAssign);
	}


public:

	~NVMathsOperations()
	{
		for (std::map<char, BinaryOperator*>::iterator it = operators.begin(); it != operators.end(); ++it )
		{
			delete[] it->second;
		}
	}

	static NVMathsOperations* getInstance()
	{
		static NVMathsOperations instance;
		return &instance;
	}

	int calculate(char op, int x, int y, bool reverse = false)
	{
		std::map<char, BinaryOperator*>::iterator bit = operators.find(op);
		if ( bit != operators.end() )
		{
			if ( !reverse )
			{
				return bit->second->calculate(x, y);
			} else {
				return bit->second->calculateReverse(x, y);
			}
		} else {
			DebugPrintf("NVScript Maths Error: Unknown operator specified: %c", op);
			return 0;
		}
	}

	int calculateReverse(char op, int x, int y)
	{
		return calculate(op, x, y, true);
	}

	std::queue<std::string> shuntMaths(char* pszEquation)
	{
		// Shunting yard algorithm to convert infix to postfix notation
		std::string buf = "";
		std::stack<char> s;
		std::queue<std::string> output;

		char* p = pszEquation;

		while ( *p != '\0')
		{
			if ( *p == ' ')
			{
				++p;
				continue; // Skip spaces
			}

			if ( *p == ')' )
			{
				if ( buf.length() > 0 )
				{
					output.push(buf);
					buf = "";
				}

				int za = 0;

				while ( s.top() != '(' )
				{
					if (za++ == 1 ) { return std::queue<std::string>(); }
					output.push(std::string(1, s.top()));
					s.pop();
					if ( s.empty() )
					{
						DebugPrintf("NVScript Maths Error: mismatched parantheses.");
						return std::queue<std::string>();
					}
				}

				s.pop();	// Pop the left bracket
			} else if ( *p == '(')
			{
				if ( buf.length() > 0 )
				{
					output.push(buf);
					buf = "";
				}

				s.push(*p);
			} else {
				// Hack to avoid using the alphanumeric operators (r, d)
				if ( ( *p > 'a' && *p < 'z' ) || ( *p > 'A' && *p < 'Z' ) || ( *p > '0' && *p < '9' ) )
				{
					buf += *p;
				} else {
					std::map<char, BinaryOperator*>::iterator itCurrOp = operators.find(*p);
					if ( itCurrOp != operators.end() )
					{
						if ( buf.length() > 0 )
						{
							output.push(buf);
							buf = "";
						}

						while ( !s.empty() )
						{
							std::map<char, BinaryOperator*>::iterator itTopOp = operators.find(s.top());
							if ( itTopOp != operators.end() )
							{
								BinaryOperator* topOp = itTopOp->second;
								BinaryOperator* currOp = itCurrOp->second;
								if (
									((currOp->getRightAssociativity() == false && currOp->getPriority() <= topOp->getPriority())
									|| (currOp->getRightAssociativity() == true && currOp->getPriority() < topOp->getPriority()))
									)
								{
									output.push(std::string(1, s.top()));

									s.pop();
								} else {
									// This operator didn't qualify to be popped off the stack due to its priority, so break out of the loop.
									break;
								}
							} else {
								// This isn't a known operator (it's probably an opening bracket), so break out of the loop.
								break;
							}
						}

						s.push(*p);
					} else {
						buf += *p;
					}
				}
			}

			++p;
		}


		if ( buf.length() > 0 )
		{
			output.push(buf);
//			buf = "";
		}

		while ( !s.empty())
		{
			output.push(std::string(1, s.top()));
			s.pop();
		}

		return output;
	}


	int calculate(char* pszEquation)
	{
		SService<IQuestSrv> pQSrv(g_pScriptManager);

		std::queue<std::string> tokens = shuntMaths(pszEquation);

		std::stack<int> s;

		while ( !tokens.empty() )
		{
			char op = tokens.front().at(0);
			std::map<char, BinaryOperator*>::iterator bit;

			if ( ( op > 'a' && op < 'z' ) || ( op > 'A' && op < 'Z' ) || ( op > '0' && op < '9' ) )
			{
				// Hack to avoid using the alphanumeric operators (r, d)
				bit = operators.end();
			} else {
				bit = operators.find(op);
			}

			if ( bit != operators.end() )
			{
				if ( s.size() < 2 )
				{
					DebugPrintf("NVScript Maths Error: not enough operands in expression.");
					return 0;
				}

				int op1 = s.top();
				s.pop();

				int op2 = s.top();
				s.pop();

				// Remember, the operator later on the stack is the first argument
				int y = bit->second->calculate(op2, op1);
				s.push(y);

			} else {
				std::string currToken = tokens.front();
				std::istringstream iss(currToken);
				int i = 0;

				if ( !(iss>>i) )
				{
					if ( currToken.at(0) == '?' )
					{ // If the first character is a '?', then use a random number between 0 and the QVar value.
						if (pQSrv->Exists(currToken.c_str()+1))
						{
							SService<IDataSrv> pDataSrv(g_pScriptManager);
							i = pDataSrv->RandInt(0, pQSrv->Get(currToken.c_str()+1)); // Get QVar value
						}
					} else {
						if (pQSrv->Exists(currToken.c_str()))
						{
							i = pQSrv->Get(currToken.c_str()); // Get QVar value
						}
					}
				}

				s.push(i);
			}
			tokens.pop();
		}

		if ( s.size() == 1 )
			return s.top();
		else {
			DebugPrintf("NVScript Maths Error: too many operands in expression.");
			return 0;
		}

		return 0;
	}
};


#endif // NVMATHS_H
