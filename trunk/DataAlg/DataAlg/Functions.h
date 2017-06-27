
#include "..\..\Public\DebugNew.h"

template <class T>
inline void Swap(T& a, T& b)
{
	T temp(a);
	a = b;
	b = temp;
}

//////////////////////////////////////////////////////////////////////////
// permutation
template <class T>
void permulation(T* list, int nStart, int nEnd)
{
	if (nStart == nEnd)
	{
		for (int jj = 0; jj <= nEnd; jj++)
		{
			cout<<list[jj];
		}
		cout<<endl;
	}
	else
	{
		for (int ii = nStart; ii <= nEnd; ii++)
		{
			Swap(list[ii], list[nStart]);
			permulation(list, nStart+1, nEnd);
			Swap(list[ii], list[nStart]);
		}
	}
}