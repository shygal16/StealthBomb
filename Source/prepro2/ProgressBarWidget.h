// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include"Runtime/SlateCore/Public/Layout/Visibility.h"
#include "ProgressBarWidget.generated.h"

class AEnemy_RealTest;
/**
*
*/
UCLASS()
class PREPRO2_API UProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	UENUM(BlueprintType)
	enum class CompassState : uint8
	{
		Hidden,
		Visible,
		Entering,
		Leaving
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintBar Widget")
		float mSprintBarPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintBar Widget")
		float mXrayPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintBar Widget")
		float mBombPlantPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintBar Widget")
		bool mBombPlantVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		CompassState mCompassState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		float mAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		AEnemy_RealTest* mEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		bool mEnemyHeard = false;

	UFUNCTION(BlueprintCallable, category = "Enemies")
	void GetQuadrant();

};
