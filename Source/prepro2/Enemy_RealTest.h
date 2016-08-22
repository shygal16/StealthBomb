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
//	UFUNCTION()
//	void OnSeePlayer(APawn* pawn);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alive")
	bool isAlive = true;

	UFUNCTION(BlueprintCallable, category = "Level")
	void SetLevel(AMyLevelScriptActor* level) { mLevel = level; }

//	UFUNCTION()
//	void OnHearPlayer(APawn* pawn, const FVector &Location, float Volume);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screeching")
		bool Screeched = false;

private:
	float mWalkSpeed;
	float mScreechDelay = 0.5f;

	//Reference to level
	AMyLevelScriptActor* mLevel;
};
