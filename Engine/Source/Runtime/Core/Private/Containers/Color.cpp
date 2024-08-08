#include "Containers/Color.h"

FColor FColor::White = FColor(255, 255, 255);
FColor FColor::Black = FColor(0, 0, 0);
FColor FColor::Red = FColor(255, 0, 0);
FColor FColor::Green = FColor(0, 255, 0);
FColor FColor::Blue = FColor(0, 0, 255);
FColor FColor::Yellow = FColor(255, 255, 0);
FColor FColor::Cyan = FColor(0, 255, 255);
FColor FColor::Magenta = FColor(255, 0, 255);
FColor FColor::Orange = FColor(243, 156, 18);
FColor FColor::Purple = FColor(169, 7, 228);
FColor FColor::Turquoise = FColor(26, 188, 156);
FColor FColor::Silver = FColor(189, 195, 199);
FColor FColor::Emerald = FColor(46, 204, 113);
FColor FColor::Default = FColor(0, 0, 0);

FColor::FColor(const FLinearColor& color)
{
	*this = color.ToColor();
}

FLinearColor FColor::ToLinearColor() const
{
	FLinearColor result{};

	result.R = R / 255.f;
	result.G = G / 255.f;
	result.B = B / 255.f;
	result.A = A / 255.f;

	return result;
}

FLinearColor::FLinearColor(const FColor& color)
{
	*this = color.ToLinearColor();
}

FColor FLinearColor::ToColor() const
{
	FColor result{};

	result.R = static_cast<uint8>(R * 255.f);
	result.G = static_cast<uint8>(G * 255.f);
	result.B = static_cast<uint8>(B * 255.f);
	result.A = static_cast<uint8>(A * 255.f);

	return result;
}