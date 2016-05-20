// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class PREPRO2_API AEnemyController : public AAIController
{
	GENERATED_BODY()
		AEnemyController();
	UBlackboardComponent* mBlackboard;

	
	UBehaviorTreeComponent* mBehaviortree;

	virtual void Possess(class APawn* InPawn) override;

	virtual void BeginInactiveState() override;

	void SetTargetEnemy(APawn * Target);

};
