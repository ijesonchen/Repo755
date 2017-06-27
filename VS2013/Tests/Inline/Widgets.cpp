#include "Widgets.h"
#include "WidgetTmpl.h"

Widgets::Widgets()
{
	x = 100;
}


Widgets::~Widgets()
{
}


int Widgets::GetC()
{
	return x;
}

int GetD()
{
	return 11;
}

int GetF()
{
	WidgetImpl wi;
	wi.Set(23.5);
	return wi.Get();
//	return 11;
}
