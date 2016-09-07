// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WidgetBlueprint.h"
#include "CompassWidget.generated.h"


class AEnemy_RealTest;
/**
 * 
 */
UCLASS()
class PREPRO2_API UCompassWidget : public UUserWidget
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
