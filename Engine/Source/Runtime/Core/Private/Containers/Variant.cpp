#include "Containers/Variant.h"

FVariant::FVariant(const FVariant& Other)
{
	if (Other.Data)
	{
		Type = Other.Type;
		Data = Other.Data->MakeCopy();
	}
}

FVariant::FVariant(FVariant&& Other) noexcept
{
	Data = Other.Data;
	Type = Other.Type;

	Other.Data = nullptr;
	Other.Type = EVariantType::Unknown;
}

FVariant::~FVariant()
{
	FreeData();
}

FVariant& FVariant::operator=(const FVariant& Other)
{
	if (this == &Other)
		return *this;

	FreeData();

	if (Other.Data)
	{
		Data = Other.Data->MakeCopy();
		Type = Other.Type;
	}

	return *this;
}

FVariant& FVariant::operator=(FVariant&& Other) noexcept
{
	if (this == &Other)
		return *this;

	FreeData();

	Data = Other.Data;
	Type = Other.Type;

	Other.Data = nullptr;
	Other.Type = EVariantType::Unknown;

	return *this;
}

void FVariant::FreeData()
{
	if (Data)
	{
		delete Data;
		Data = nullptr;

		Type = EVariantType::Unknown;
	}
}