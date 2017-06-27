#include <stdio.h>
#include <ctype.h>
#include <stack>

enum OperTag{
	tagInvalid,
	tagOperand,
	tagOperator,
};

struct OperObject 
{
	int tag;
	union
	{
		double value;
		char opersign;
	};
};

// + + 2 * 3 - 7 4 / 8 4

OperObject getop(char*& p)
{
	OperObject oper;

	if (*p == NULL)
	{
		oper.tag = tagInvalid;
		return oper;
	}

	// by-pass space
	while (*p && (*p == ' '))
	{
		++p;
	}

	// one operator only
	if (!isdigit(*p) && 
		(*(p+1) == ' ' || *(p+1) == NULL) )
	{
		// operator
		oper.tag = tagOperator;
		switch (*p)
		{
		case '+':
		case '-':
		case '*':
		case '/':
			oper.opersign = *p;
			break;

		default:
			oper.tag = tagInvalid;
			break;
		}
		p += 2; // move to next
		return oper;
	}

	oper.tag = tagOperand;
	oper.value = atof(p);

	// move to next
	while (*p && (*p != ' '))
	{
		++p;
	}
	++p;

	return oper;
}

const int nMaxInput = 30;
int main(void)
{
	char szInput[nMaxInput + 1] = {0};
	int nIdx = 0;
	char ch = 0;
	while (((ch = getchar()) != '\n')
		&& (ch != EOF))
	{
		szInput[nIdx++] = ch;
		if (nIdx >= nMaxInput)
		{
			return -1;
		}
	}

	char* p = szInput;

	std::stack<OperObject> stObjects;
	OperObject oper;
	oper.tag = tagOperand;
	while (oper.tag != tagInvalid)
	{
		oper = getop(p);
		if (oper.tag != tagInvalid)
		{
			stObjects.push(oper);
		}
	}

	double operand1 = 0;
	double operand2 = 0;
	double result = 0;
	std::stack<double> stOperands;
	while (stObjects.size())
	{
		OperObject& oper = stObjects.top();
		if (oper.tag == tagOperand)
		{
			stOperands.push(oper.value);
		}
		else if (oper.tag == tagOperator)
		{
			operand1 = stOperands.top();
			stOperands.pop();
			operand2 = stOperands.top();
			stOperands.pop();

			switch (oper.opersign)
			{
			case '+':
				stOperands.push(operand1 + operand2);
				break;

			case '-':
				if (operand2 == 0)
				{
					printf("ERROR\n");
					return 0;
				}
				stOperands.push(operand1 - operand2);
				break;

			case '*':
				stOperands.push(operand1 * operand2);
				break;

			case '/':
				stOperands.push(operand1 / operand2);
				break;

			default:
				return -1;
				break;
			}
		}
		else
		{
			return -2;
		}

		stObjects.pop();
	}

	printf("%0.1lf\n", stOperands.top());

	return 0;
}