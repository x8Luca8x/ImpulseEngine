#pragma once

#include "Definitions.h"

class FLinearColor;

class FColor
{
public:

	uint8 R;
	uint8 G;
	uint8 B;
	uint8 A;

	FColor() : R(0), G(0), B(0), A(0) {}
	FColor(uint8 r, uint8 g, uint8 b, uint8 a) : R(r), G(g), B(b), A(a) {}
	FColor(uint8 r, uint8 g, uint8 b) : R(r), G(g), B(b), A(255) {}

	CORE_API FColor(const FLinearColor& color);
	CORE_API FLinearColor ToLinearColor() const;

public:

	static FColor White;
	static FColor Black;
	static FColor Red;
	static FColor Green;
	static FColor Blue;
	static FColor Yellow;
	static FColor Cyan;
	static FColor Magenta;
	static FColor Orange;
	static FColor Purple;
	static FColor Turquoise;
	static FColor Silver;
	static FColor Emerald;
	static FColor Default;
};

class FLinearColor
{
public:

	float R;
	float G;
	float B;
	float A;

	FLinearColor() : R(0), G(0), B(0), A(0) {}
	FLinearColor(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
	FLinearColor(float r, float g, float b) : R(r), G(g), B(b), A(1) {}

	CORE_API FLinearColor(const FColor& color);
	CORE_API FColor ToColor() const;

public:

	static FLinearColor White;
	static FLinearColor Black;
	static FLinearColor Red;
	static FLinearColor Green;
	static FLinearColor Blue;
	static FLinearColor Yellow;
	static FLinearColor Cyan;
	static FLinearColor Magenta;
	static FLinearColor Orange;
	static FLinearColor Purple;
	static FLinearColor Turquoise;
	static FLinearColor Silver;
	static FLinearColor Emerald;
	static FLinearColor Default;
};