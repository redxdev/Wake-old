#pragma once

#include <list>

/**
 * Defines an event.
 */
#define W_EVENT(Name, ...) Event<__VA_ARGS__> (Name)

/**
 * Base callable class.
 */
template<typename... Arguments>
class Callable
{
public:
	virtual ~Callable() {}

	virtual void Call(Arguments... Args)
	{
	}

	virtual bool Equals(Callable<Arguments...>* Other) const
	{
		return false;
	}
};

/**
 * Contains a callable function.
 */
template<typename... Arguments>
class StaticCallable : public Callable<Arguments...>
{
public:
	typedef void (*FuncPtr)(Arguments...);

	StaticCallable(FuncPtr Func)
	{
		Function = Func;
	}

	virtual void Call(Arguments... Args) override
	{
		if (!Function)
			return;

		Function(Args...);
	}

	virtual bool Equals(Callable<Arguments...>* Other) const override
	{
		auto OtherPtr = dynamic_cast<const StaticCallable<Arguments...>*>(Other);
		if (!OtherPtr)
			return false;

		return Function == OtherPtr->Function;
	}

private:
	FuncPtr Function;
};

/**
 * Contains a callable member function.
 */
template<typename T, typename... Arguments>
class InstancedCallable : public Callable<Arguments...>
{
public:
	typedef void (T::*FuncPtr)(Arguments...);

	InstancedCallable(T* Inst, FuncPtr Func)
	{
		Instance = Inst;
		Function = Func;
	}

	virtual void Call(Arguments... Args) override
	{
		if (!Instance || !Function)
			return;

		(Instance->*Function)(Args...);
	}

	virtual bool Equals(Callable<Arguments...>* Other) const override
	{
		auto OtherPtr = dynamic_cast<const InstancedCallable<T, Arguments...>*>(Other);
		if (!OtherPtr)
			return false;

		return Instance == OtherPtr->Instance && Function == OtherPtr->Function;
	}

private:
	T* Instance;
	FuncPtr Function;
};

/**
 * A callable event. Use the W_EVENT macro for defining events.
 */
template<typename... Arguments>
class Event
{
public:
	~Event()
	{
		Clear();
	}

	/**
	 * Bind a Callable delegate to this event.
	 */
	void Bind(Callable<Arguments...>* Delegate)
	{
		Delegates.push_back(Delegate);
	}

	/**
	 * Bind a function to this event.
	 */
	void Bind(void (*FuncPtr)(Arguments...))
	{
		Bind(new StaticCallable<Arguments...>(FuncPtr));
	}

	/**
	 * Bind a member function to this event.
	 */
	template<typename T>
	void Bind(T* Instance, void (T::*FuncPtr)(Arguments...))
	{
		Bind(new InstancedCallable<T, Arguments...>(Instance, FuncPtr));
	}

	/**
	 * Unbind a callable delegate. This will delete the Callable object.
	 */
	void Unbind(Callable<Arguments...>* Delegate)
	{
		for (auto Itr = Delegates.begin(); Itr != Delegates.end(); ++Itr)
		{
			if (Itr->Equals(Delegate))
			{
				delete *Itr;
				Delegates.erase(Itr);
			}
		}
	}

	/**
	 * Unbind a function.
	 */
	void Unbind(void (*FuncPtr)(Arguments...))
	{
		Unbind(new StaticCallable<Arguments...>(FuncPtr));
	}

	/**
	 * Unbind a member function.
	 */
	template<typename T>
	void Unbind(T* Instance, void (T::*FuncPtr)(Arguments...))
	{
		Unbind(new InstancedCallable<T, Arguments...>(Instance, FuncPtr));
	}

	/**
	 * Check if a Callable delegate is bound to this event.
	 */
	bool IsBound(Callable<Arguments...>* Delegate) const
	{
		for (Callable<Arguments...>* Other : Delegates)
		{
			if (Other->Equals(Delegate))
				return true;
		}

		return false;
	}

	/**
	 * Check if a function is bound to this event.
	 */
	bool IsBound(void (*FuncPtr)(Arguments...))
	{
		return IsCallable(new StaticCallable<Arguments...>(FuncPtr));
	}

	/**
	 * Check if a member function is bound to this event.
	 */
	template<typename T>
	bool IsBound(T* Instance, void (T::*FuncPtr)(Arguments...))
	{
		return IsBound(new InstancedCallable<T, Arguments...>(Instance, FuncPtr));
	}

	/**
	 * Call this event.
	 */
	void Call(Arguments... Args)
	{
		for (Callable<Arguments...>* Delegate : Delegates)
		{
			Delegate->Call(Args...);
		}
	}

	/**
	 * Clear all binds from this event, deleting the Callable objects.
	 */
	void Clear()
	{
		for (Callable<Arguments...>* Delegate : Delegates)
		{
			delete Delegate;
		}

		Delegates.clear();
	}

private:
	std::list<Callable<Arguments...>*> Delegates;
};