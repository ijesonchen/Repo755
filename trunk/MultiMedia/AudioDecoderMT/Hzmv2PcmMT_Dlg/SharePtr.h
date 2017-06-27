#pragma once

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


//引用计数类
template<class HandleType, class Freer, HandleType InvalidHandle = NULL>

class CShareHandle
{
public:
	typedef LONG RefCountType;
	CShareHandle(const Freer &freer = Freer())
		:w_pRefCount(NULL), w_handle(InvalidHandle), w_freer(freer)
	{
	}

	CShareHandle(const CShareHandle& otherSharePtr)
		: w_handle(otherSharePtr.w_handle)
		, w_freer(otherSharePtr.w_freer)
	{
		w_pRefCount = otherSharePtr.w_pRefCount;
		if(otherSharePtr.w_pRefCount != NULL)
			InterlockedIncrement(w_pRefCount);
	}
	CShareHandle& operator= (const CShareHandle& otherSharePtr)
	{
		if (w_handle == otherSharePtr.w_handle)
		{
			return *this;
		}
		ClearData();

		w_handle = otherSharePtr.w_handle;
		w_pRefCount = otherSharePtr.w_pRefCount;
		w_freer = otherSharePtr.w_freer;
		if(otherSharePtr.w_pRefCount != NULL)
			InterlockedIncrement(w_pRefCount);
		return *this;
	}

	CShareHandle(HandleType handle, const Freer &freer = Freer())
		:w_pRefCount(new volatile RefCountType(1)), w_handle(handle), w_freer(freer)
	{
		ASSERT(handle != InvalidHandle);
	}
	void SetFreer(const Freer& freer)
	{
		w_freer = freer;
	}

	const Freer& GetFreer() const
	{
		return w_freer;
	}

	CShareHandle& operator= (HandleType handle)
	{
		ASSERT(handle != InvalidHandle);
		if (w_handle == handle)
		{
			return *this;
		}
		ClearData();
		w_handle = handle;
		w_pRefCount = new volatile RefCountType(1);
		return *this;
	}

	~CShareHandle()
	{
		ClearData();
	}
	
	operator HandleType() const
	{
		return w_handle;
	}

	HandleType& GetHandle()
	{
		return w_handle;
	}

	void Free()
	{
		ClearData();
		w_handle = InvalidHandle;
		w_pRefCount = NULL;
	}
	
	bool IsValid() const
	{
		return w_pRefCount != NULL && w_handle != InvalidHandle;
	}

private:
	inline void ClearData()
	{
		if (w_pRefCount != NULL && InterlockedDecrement(w_pRefCount) == 0)
		{
			delete w_pRefCount;
			w_pRefCount = NULL;
			w_freer(w_handle);
			w_handle = InvalidHandle;
		}
	}

	volatile RefCountType* w_pRefCount;
	Freer w_freer;
	HandleType w_handle;
};
//引用计数类
template<class HandleType, class Freer, HandleType InvalidHandle = NULL>
class CSingleHandle
{
public:
	CSingleHandle(const Freer &freer = Freer())
		:w_handle(InvalidHandle), w_freer(freer)
	{
	}

	CSingleHandle(CSingleHandle& otherSingleHandle)
		: w_handle(otherSingleHandle.w_handle)
		, w_freer(otherSingleHandle.w_freer)
	{
		otherSingleHandle.w_handle = InvalidHandle;
	}

	CSingleHandle& operator= (CSingleHandle& otherSingleHandle)
	{
		if (w_handle == otherSingleHandle.w_handle)
		{
			return *this;
		}
		ClearData();

		w_handle = otherSingleHandle.w_handle;
		w_freer = otherSingleHandle.w_freer;
		otherSingleHandle.w_handle = InvalidHandle;
		return *this;
	}

	CSingleHandle(HandleType handle, const Freer &freer = Freer())
		: w_handle(handle), w_freer(freer)
	{
	}

	void SetFreer(const Freer& freer)
	{
		w_freer = freer;
	}

	const Freer& GetFreer() const
	{
		return w_freer;
	}

	CSingleHandle& operator= (HandleType handle)
	{
		if (w_handle == handle)
		{
			return *this;
		}
		ClearData();
		w_handle = handle;
		return *this;
	}

	~CSingleHandle()
	{
		ClearData();
	}

	operator HandleType() const
	{
		return w_handle;
	}

	HandleType& GetHandle()
	{
		return w_handle;
	}

	void Free()
	{
		ClearData();
	}

	bool IsValid() const
	{
		return w_handle != InvalidHandle;
	}

private:
	inline void ClearData()
	{
		if (w_handle != InvalidHandle)
		{
			w_freer(w_handle);
			w_handle = InvalidHandle;
		}
	}
	Freer w_freer;
	HandleType w_handle;
};
