// Copyright tzlFirefly, 2023. All Rights Reserved.


#include "FireflyAbilitySystemTypes.h"

#include "AbilitySystemComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/Texture2D.h"


void FFireflyGameplayEffectUIDisplay::operator=(const FFireflyGameplayEffectUIDisplay& Other)
{
	Material = Other.Material;
	Texture = Other.Texture;
	if (!Other.Title.IsEmpty())
	{
		Title = Other.Title;
	}
	if (!Other.Description.IsEmpty())
	{
		Description = Other.Description;
	}
}

bool FFireflyGameplayEffectUIDisplay::IsValid() const
{
	return Material || Texture || !Title.IsEmpty() || !Description.IsEmpty();
}

bool FFireflyGameplayEffectUIDisplay::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint8 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (Material)
		{
			RepBits |= 1 << 0;
		}
		if (Texture)
		{
			RepBits |= 1 << 1;
		}
		if (!Title.IsEmpty())
		{
			RepBits |= 1 << 2;
		}
		if (!Description.IsEmpty())
		{
			RepBits |= 1 << 3;
		}
	}

	Ar.SerializeBits(&RepBits, 4);

	if (RepBits & (1 << 0))
	{
		Ar << Material;
	}
	if (RepBits & (1 << 1))
	{
		Ar << Texture;
	}
	if (RepBits & (1 << 2))
	{
		Ar << Title;
	}
	if (RepBits & (1 << 3))
	{
		Ar << Description;
	}

	bOutSuccess = true;
	return true;
}