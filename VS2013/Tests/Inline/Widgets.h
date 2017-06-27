#pragma once
class Widgets
{
public:
	Widgets();
	~Widgets();

	int GetA()
	{
		return x;
	}
private:
	int x;
public:
	inline int GetB()
	{
		int n = 0;
		for (int ii = 0; ii < x; ++ii)
		{
			for (int jj = 0; jj < x; ++jj)
			{
				n += ii * jj;
			}
		}
		return n;
	};
	int GetC();
};

int GetD();

inline int GetE()
{
	return 11;
}

int GetF();