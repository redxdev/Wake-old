#pragma once

#include <list>

#define W_EVENT(Name, ...) Utility::Event<__VA_ARGS__> (Name)

namespace Utility
{
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

	template<typename... Arguments>
	class Event
	{
	public:
		~Event()
		{
			Clear();
		}

		void Bind(Callable<Arguments...>* Delegate)
		{
			Delegates.push_back(Delegate);
		}

		void Bind(void (*FuncPtr)(Arguments...))
		{
			Bind(new StaticCallable<Arguments...>(FuncPtr));
		}

		template<typename T>
		void Bind(T* Instance, void (T::*FuncPtr)(Arguments...))
		{
			Bind(new InstancedCallable<T, Arguments...>(Instance, FuncPtr));
		}

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

		void Unbind(void (*FuncPtr)(Arguments...))
		{
			Unbind(new StaticCallable<Arguments...>(FuncPtr));
		}

		template<typename T>
		void Unbind(T* Instance, void (T::*FuncPtr)(Arguments...))
		{
			Unbind(new InstancedCallable<T, Arguments...>(Instance, FuncPtr));
		}

		bool IsBound(Callable<Arguments...>* Delegate) const
		{
			for (Callable<Arguments...>* Other : Delegates)
			{
				if (Other->Equals(Delegate))
					return true;
			}

			return false;
		}

		bool IsBound(void (*FuncPtr)(Arguments...))
		{
			return IsCallable(new StaticCallable<Arguments...>(FuncPtr));
		}

		template<typename T>
		bool IsBound(T* Instance, void (T::*FuncPtr)(Arguments...))
		{
			return IsBound(new InstancedCallable<T, Arguments...>(Instance, FuncPtr));
		}

		void Call(Arguments... Args)
		{
			for (Callable<Arguments...>* Delegate : Delegates)
			{
				Delegate->Call(Args...);
			}
		}

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
}