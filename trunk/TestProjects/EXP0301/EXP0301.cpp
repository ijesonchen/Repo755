#include <stdio.h>
#include <deque>

struct XItem
{
	int coef;
	int exp;

	XItem(void): coef(0), exp(0){};
	XItem(int nc, int ne): coef(nc), exp(ne) {};
};

int main(void)
{
	std::deque<XItem> dqItems;
	do 
	{
		XItem xItem;
		scanf("%d %d", &xItem.coef, &xItem.exp);
		dqItems.push_back(xItem);
	} while (getchar() == ' ');

	// diff
	size_t nItem = dqItems.size();
	for (size_t ii = 0;	ii < nItem; ++ii)
	{
		XItem& xItem = dqItems[ii];
		xItem.coef = xItem.coef * xItem.exp;
		xItem.exp ? --xItem.exp : 0;
	}

	// output
	if (nItem)
	{
		printf("%d %d", dqItems[0].coef, dqItems[0].exp);
		for (size_t ii = 1; ii < nItem; ++ii)
		{
			if (dqItems[ii].coef)
			{
				printf(" %d %d", dqItems[ii].coef, dqItems[ii].exp);
			}
		}
	}

	return 0;
}