#ifndef WidgetTmpl_h__
#define WidgetTmpl_h__

template<typename T>
class WidgetTmpl
{
public:
	WidgetTmpl();
	virtual ~WidgetTmpl() = 0;

	int Conv(T& t);
};


class WidgetImpl : public WidgetTmpl<double>
{
public:
	inline WidgetImpl();
	inline ~WidgetImpl();

	inline int Get(void);
	inline void Set(double d);
	
private:
	double x;
};


//////////////////////////////////////////////////////////////////////////
// body


template<typename T>
WidgetTmpl<T>::WidgetTmpl()
{}

template<typename T>
WidgetTmpl<T>::~WidgetTmpl()
{}

template<typename T>
int WidgetTmpl<T>::Conv(T& t)
{
	return static_cast<int>(t);
}


WidgetImpl::WidgetImpl()
{};
WidgetImpl::~WidgetImpl()
{};


int WidgetImpl::Get(void)
{
	return Conv(x);
}
void WidgetImpl::Set(double d)
{
	x = d;
}

#endif // WidgetTmpl_h__
