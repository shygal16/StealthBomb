// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		bool mVisible = false;

};
