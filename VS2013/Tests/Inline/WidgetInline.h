#ifndef WidgetInline_h__
#define WidgetInline_h__

class WidgetInline
{
public:

	inline WidgetInline();

	inline ~WidgetInline();

	inline int GetA();

	inline int GetB();
private:
	int x;
};



WidgetInline::WidgetInline()
{}

WidgetInline::~WidgetInline()
{}

int WidgetInline::GetA()
{
	return x;
}

int WidgetInline::GetB()
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
#endif // WidgetInline_h__
