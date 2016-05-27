// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include"Runtime/SlateCore/Public/Layout/Visibility.h"
#include "ProgressBarWidget.generated.h"

/**
*
*/
UCLASS()
class PREPRO2_API UProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintBar Widget")
		float mSprintBarPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintBar Widget")
		float mXrayPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SprintBar Widget")
		float mBombPlantPercentage;


};
