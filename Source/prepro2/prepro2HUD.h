// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "prepro2Character.h"
#include "Runtime/Engine/Classes/Engine/Font.h"
#include "Enemy_RealTest.h"
#include "prepro2HUD.generated.h"

UCLASS()
class Aprepro2HUD : public AHUD
{
	GENERATED_BODY()

public:
	Aprepro2HUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool DisplayPopup=false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UTexture2D* GivenTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool FirstPickup=true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		float Fade = 1;
private:
	void BeginPlay() override;

	class UTexture2D* EButtonPrompt;
	class UTexture2D* EButtonPromptFirst;



	UFont* mFont;
public:
	
};

