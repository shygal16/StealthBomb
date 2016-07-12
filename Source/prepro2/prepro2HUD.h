// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "prepro2Character.h"
#include "Runtime/Engine/Classes/Engine/Font.h"
#include "prepro2HUD.generated.h"

UCLASS()
class Aprepro2HUD : public AHUD
{
	GENERATED_BODY()

public:
	Aprepro2HUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

	class UTexture2D* mBombUsed;
	class UTexture2D* mBombUnused;
	class UTexture2D* mBombSelected;

	UFont* mFont;

	float mBombTextureWidth;

	
};

