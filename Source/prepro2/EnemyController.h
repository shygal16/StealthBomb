// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionComponent.h"
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


	UAIPerceptionComponent* perceptionComponent;
	UAISenseConfig_Sight* sightConfig;
	UAISenseConfig_Hearing* soundConfig;
	//sensing stuff
	UFUNCTION()
		void SenseStuff(TArray<AActor*> testActors);
	uint8 TargetKeyID;
	uint8 TargetLocationID;
	
public:
	void SetTargetEnemy(APawn * Target);
	//void ClearTarget();
};
