// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/LevelScriptActor.h"
#include "prepro2Character.h"
#include "Enemy_RealTest.h"
#include "MyLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class PREPRO2_API AMyLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
protected:
	//void BeginPlay() override;

public:

	AMyLevelScriptActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPoints")
		TArray<FVector> mPatrolPointsSet1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPoints")
		TArray<FVector> mPatrolPointsSet2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPoints")
		TArray<FVector> mPatrolPointsSet3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPoints")
		TArray<FVector> mPatrolPointsSet4;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
	TArray<AEnemy_RealTest*> mEnemies;
	
	void FirstEnemyDeathEvent();
	
};
