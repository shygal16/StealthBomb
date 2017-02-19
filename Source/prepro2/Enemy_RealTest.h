// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Enemy_RealTest.generated.h"

class AMyLevelScriptActor;

UCLASS()
class PREPRO2_API AEnemy_RealTest : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy_RealTest();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float mHealth;
	
	UPROPERTY(EditAnywhere, Category=Behavior)
	class UBehaviorTree* BehaviorTree;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alive")
	bool isAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screeching")
		bool Screeched = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screeching")
		bool Growled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screeching")
		float GrowlFrequency = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screeching")
	USoundBase* GrowlSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
		TArray<FVector> patrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
		int patrolIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
		bool isPatrolRandom;

private:
	float mWalkSpeed;
	float mScreechDelay = 0.5f;	
	float mGrowlDelay = 35.f;
};
