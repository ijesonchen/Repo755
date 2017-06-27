#include "ch1-3.h"
#include <random>
#include <time.h>
#include <sstream>
#include <stack>
#include <cmath>
#include <cstdio>

using namespace std;

void TestBag(void)
{
	default_random_engine generator(static_cast<unsigned>(time(nullptr)));
	uniform_int_distribution<int> distribution(1, 3);
	int bagcnt = 10;
	BagArray<int> ba;
	BagList<int> bl;
	BagListInt bli;

	cout << "array size: " << ba.size() << " capacity: " << ba.capacity() << " empty: " << ba.isEmpty() << endl;
	cout << "list size: " << bl.size() << " capacity: " << bl.capacity() << " empty: " << bl.isEmpty() << endl;
	cout << "ilist size: " << bl.size() << " capacity: " << bl.capacity() << " empty: " << bl.isEmpty() << endl;

	cout << "data: ";
	while (--bagcnt)
	{
		auto d = distribution(generator);
		cout << d << " ";
		ba.add(d);
		bl.add(d);
		bli.add(d);
	}
	cout << endl;
	
	cout << "array: " << endl;
	ba.print();
	cout << "size: " << ba.size() << " capacity: " << ba.capacity() << " empty: " << ba.isEmpty() << endl;

	cout << "list: " << endl;
	bl.print();
	cout << "size: " << bl.size() << " capacity: " << bl.capacity() << " empty: " << bl.isEmpty() << endl;

	cout << "ilist: " << endl;
	bli.print();
	cout << "size: " << bli.size() << " capacity: " << bli.capacity() << " empty: " << bli.isEmpty() << endl;
}

void TestStatck(void)
{
	string input = "to be or not to - be - - that - - - is";
	string output = "to be not that or be";
	int nleft = 2;
	istringstream is(input);
	string str;
	vector<string> v;
	while (is >> str)
	{
		v.push_back(str);
	}
	vector<string> voutput;
	is.clear();
	is.str(output);
	while (is >> str)
	{
		voutput.push_back(str);
	}

	LinkedStack<string> ls;


	vector<string> result;
	for (auto& s : v)
	{
		if (s == "-")
		{
			result.push_back(ls.pop());
		}
		else
		{
			ls.push(s);
		}
	}
	if (result == voutput && ls.size() == nleft)
	{
		cout << "check passed." << endl;
	}
	else
	{
		cout << "check failed." << endl;
	}
}


// Dijkstra双栈算术表达式求值
bool IsSampleOperators(const string& s)
{
	string strops = "+ - * / ";
	vector<string> ops;
	istringstream iss(strops);
	string str;
	while (iss >> str)
	{
		ops.push_back(str);
	}
	for (auto& it : ops)
	{
		if (it == s)
		{
			return true;
		}
	}
	return false;
}

double fnDijkstraEvaluate(const string& input)
{
	vector<double> oprands;
	vector<string> ops;

	istringstream iss(input);
	string str;
	while (iss >> str)
	{
		if (str == "(")
		{
			continue;
		}
		else if (str == ")")
		{
			// calculate
			auto op = ops.back();
			ops.pop_back();
			auto r2 = oprands.back();
			oprands.pop_back();
			if (op == "sqrt")
			{
				oprands.push_back(sqrt(r2));
				continue;
			}
			auto r1 = oprands.back();
			oprands.pop_back();
			if (op == "+")
			{
				oprands.push_back(r1 + r2);
			}
			else if (op == "-")
			{
				oprands.push_back(r1 - r2);
			}
			else if (op == "*")
			{
				oprands.push_back(r1 * r2);
			}
			else if (op == "/")
			{
				oprands.push_back(r1 / r2);
			}
			else
			{
				throw "invalid input.";
			}
			continue;
		}
		else if (IsSampleOperators(str) || str == "sqrt")
		{
			ops.push_back(str);
		}
		else
		{
			oprands.push_back(atof(str.c_str()));
		}
	}

	return oprands.back();
}

bool CompFloatVector(const vector<double>& v1, const vector<double>& v2, double delta = 0.0001)
{
	if (v1.size() != v2.size())
	{
		return false;
	}
	for (auto it1 = v1.begin(), it2 = v2.begin(); it1 != v1.end(); ++it1, ++it2)
	{
		if (*it1 - *it2 > delta)
		{
			return false;
		}
	}
	return true;
}

void DijkstraEvaluate(void)
{
	vector<string> inputs = { 
		"( 1 + ( ( 2 + 3 ) * ( 4 * 5 ) ) )",
		"( ( 1 + sqrt ( 5 ) ) / 2.0 )" };
	vector<double> results = { 101, 1.618033988749895f };

	vector<double> outputs;
	for (auto& input : inputs)
	{
		auto output = fnDijkstraEvaluate(input);
		outputs.push_back(output);
	}

	if (!CompFloatVector(results, outputs))
	{
		cout << "failed." << endl;
	}
	else
	{
		cout << "passed." << endl;
	}
}

// 出入栈序列是否可能
bool fn010301(const string& input, const int nMax)
{		
	// expected output vector
	vector<int> vi;
	istringstream is(input);
	int ni;
	while (is >> ni)
	{
		vi.push_back(ni);
	}

	// index of target to match in vi
	int idx = 0;
	// next expected popped value
	auto next = vi[idx];
	// stack to simulate operation
	LinkedStack<int> ls;
	// number push to ls
	int ns = 0;
	// find first out number
	while (ns <= vi[0] && ns < nMax)
	{
		ls.push(ns++);
	}

	if (ls.size() == 0 || next != ls.pop())
	{
		return false;
	}

	bool bPass = true;

	while (++idx < static_cast<int>(vi.size()))
	{
		next = vi[idx];

		while (ns <= next && ns < nMax)
		{
			ls.push(ns++);
		}

		if (next > ns)
		{
			if (ls.size() == 0 || next != ls.pop())
			{
				bPass = false;
				break;
			}
		}
		else
		{
			if (ls.size() == 0 || next != ls.pop())
			{
				bPass = false;
				break;
			}
		}
	}

	return bPass;
}

void ex010301(void)
{
	int nMax = 10;
	vector<string> inputs = { "4 3 2 1 0 9 8 7 6 5" ,
	"4 6 8 7 5 3 2 9 0 1",
	"2 5 6 7 4 8 9 3 1 0",
	"4 3 2 1 0 5 6 7 8 9",
	"1 2 3 4 5 6 9 8 7 0",
	"0 4 6 5 3 8 1 7 2 9",
	"1 4 7 9 8 6 5 3 0 2",
	"2 1 4 3 6 5 8 7 9 0"};

	vector<bool> results = { 1, 0, 1, 1, 1, 0, 0, 1 };
	vector<bool> outputs;
	for (auto& input : inputs)
	{
		outputs.push_back(fn010301(input, nMax));
		cout << "test " << input << " " << outputs.back() << endl;
	}
	if (results != outputs)
	{
		cout << "failed." << endl;
	}
	else
	{
		cout << "passed." << endl;
	}
}



// 中序表达式补全左括号

string fn010309(const string& input)
{
	stack<string> oprands;
	stack<string> oprators;

	istringstream iss(input);
	string str;
	while (iss >> str)
	{
		if (IsSampleOperators(str))
		{
			oprators.push(str);
			continue;
		}
		else if (str == ")")
		{
			string oprand2 = oprands.top();
			oprands.pop();
			string oprand1 = oprands.top();
			oprands.pop();
			string op = oprators.top();
			oprators.pop();
			ostringstream oss;
			oss << "( " << oprand1 << " " << op << " " << oprand2 << " )";
			oprands.push(oss.str());
		}
		else
		{
			oprands.push(str);
		}
	}

	return oprands.top();
}

void ex010309(void)
{
	string input = "1 + 2 ) * 3 - 4 ) * 5 - 6 ) ) )";
	string result = "( ( 1 + 2 ) * ( ( 3 - 4 ) * ( 5 - 6 ) ) )";

	auto output = fn010309(input);

	cout << input << endl;
	cout << output << endl;

	if (output != result)
	{
		cout << "failed." << endl;
	}
	else
	{
		cout << "passed." << endl;
	}
}


// 中序表达式转换为后序表达式 InfixToPostfix
string fn010310(const string& input)
{
	stack<string> oprands;
	stack<string> oprators;

	istringstream iss(input);
	string str;
	while (iss >> str)
	{
		if (str == "(")
		{
			continue;
		}
		else if (IsSampleOperators(str))
		{
			oprators.push(str);
		}
		else if (str == ")")
		{
			// output
			auto b = oprands.top();
			oprands.pop();
			auto a = oprands.top();
			oprands.pop();
			auto op = oprators.top();
			oprators.pop();
			ostringstream oss;
			oss << a << " " << b << " " << op;
			oprands.push(oss.str());
		}
		else
		{
			oprands.push(str);
		}
	}
	return oprands.top();
}

void ex010310(void)
{
	string input = "( ( 1 + 2 ) * ( ( 3 - 4 ) * ( 5 - 6 ) ) )";
	string result = "1 2 + 3 4 - 5 6 - * *";

	auto output = fn010310(input);

	cout << input << endl;
	cout << output << endl;

	if (output != result)
	{
		cout << "failed." << endl;
	}
	else
	{
		cout << "passed." << endl;
	}
}


// 计算后序表达式 EvaluatePostFix
int fn010311(const string& input)
{
	stack<int> oprands;
	stack<string> oprators;

	istringstream iss(input);
	string str;
	while (iss >> str)
	{
		if (IsSampleOperators(str))
		{
			// output
			auto b = oprands.top();
			oprands.pop();
			auto a = oprands.top();
			oprands.pop();
			int c = 0;
			if (str == "+") c = a + b;
			else if (str == "-") c = a - b;
			else if (str == "*") c = a * b;
			else if (str == "/") c = a / b;
			oprands.push(c);
		}
		else
		{
			oprands.push(atoi(str.c_str()));
		}
	}
	return oprands.top();
}

void ex010311(void)
{
	string input = "1 2 + 3 4 - 5 6 - * *";
	int result = 3;

	auto output = fn010311(input);

	cout << input << endl;
	cout << output << endl;

	if (output != result)
	{
		cout << "failed." << endl;
	}
	else
	{
		cout << "passed." << endl;
	}
}
//////////////////////////////////////////////////////////////////////////
// 节点操作
struct Node
{
	int data;
	Node* next;

};

void ex0103LinkOperation(void)
{

}