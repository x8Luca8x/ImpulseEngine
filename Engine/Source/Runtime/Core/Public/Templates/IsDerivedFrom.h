#pragma once

template<typename Check, typename Base>
struct TIsDerivedFrom
{
	static consteval bool Test(Base* In)
	{
		return true;
	}

	static consteval bool Test(const Base* In)
	{
		return true;
	}

	static consteval bool Test(...)
	{
		return false;
	}

	static consteval Check* GetPtr()
	{
		return nullptr;
	}

	static consteval bool IsDerivedFrom()
	{
		return Test(GetPtr());
	}
};

#define IS_DERIVED_FROM(Check, Base) TIsDerivedFrom<Check, Base>::IsDerivedFrom()