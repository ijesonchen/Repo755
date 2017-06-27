#ifndef SharePtr_h__
#define SharePtr_h__

//引用计数类
template<class Type>
class CSharePtr
{
public:
	typedef LONG RefCountType;
	CSharePtr():w_pRefCount(NULL), w_pType(NULL)
	{
	}

	CSharePtr(const CSharePtr& otherSharePtr) : w_pType(otherSharePtr.w_pType)
	{
		w_pRefCount = otherSharePtr.w_pRefCount;
		if(otherSharePtr.w_pRefCount != NULL)
		{
			InterlockedIncrement(w_pRefCount);
		}
	}
	CSharePtr& operator= (const CSharePtr& otherSharePtr)
	{
		if (w_pType == otherSharePtr.w_pType)
		{
			return *this;
		}
		ClearData();
		w_pType = otherSharePtr.w_pType;
		w_pRefCount = otherSharePtr.w_pRefCount;
		if(otherSharePtr.w_pRefCount != NULL)
		{
			InterlockedIncrement(w_pRefCount);
		}
		return *this;
	}

	CSharePtr(Type *pType):w_pRefCount(new volatile RefCountType(1)), w_pType(pType)
	{
		ASSERT(pType != NULL);
	}
	CSharePtr& operator= (Type *pType)
	{
		ASSERT(pType != NULL);
		if (w_pType == pType)
		{
			return *this;
		}
		ClearData();
		w_pType = pType;
		w_pRefCount = new volatile RefCountType(1);
		return *this;
	}
	~CSharePtr()
	{
		ClearData();
	}
	void Free()
	{
		ClearData();
		w_pType = NULL;
		w_pRefCount = NULL;
	}
	operator Type*() const
	{
		return w_pType;
	}
	Type* operator->() const
	{
		return w_pType;
	}
	Type& operator*() const
	{
		return *w_pType;
	}
	inline Type* GetBuffer() const
	{
		return w_pType;
	}
	bool IsValid() const
	{
		return w_pRefCount != NULL && w_pType != NULL;
	}
private:
	inline void ClearData()
	{
		if (w_pRefCount != NULL && InterlockedDecrement(w_pRefCount) == 0)
		{
			delete w_pRefCount;
			w_pRefCount = NULL;
			delete w_pType;
			w_pType = NULL;
		}
	}
	volatile RefCountType* w_pRefCount;
	Type* w_pType;
};

#endif // SharePtr_h__
