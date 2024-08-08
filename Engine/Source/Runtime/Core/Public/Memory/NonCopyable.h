#pragma once

class FNoneCopyable
{
	FNoneCopyable(const FNoneCopyable&) = delete;
	FNoneCopyable& operator=(const FNoneCopyable&) = delete;
};

class FNoneMovable
{
	FNoneMovable(FNoneMovable&&) = delete;
	FNoneMovable& operator=(FNoneMovable&&) = delete;
};

class FNoneCopyableAndMovable : public FNoneCopyable, public FNoneMovable
{
};

class FNoneConstructible
{
	FNoneConstructible() = delete;
};

class FNoneDestructible
{
	~FNoneDestructible() = delete;
};

class FStaticClass : public FNoneConstructible, public FNoneCopyableAndMovable
{
};