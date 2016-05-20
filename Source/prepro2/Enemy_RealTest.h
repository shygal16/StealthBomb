// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionComponent.h"
#include "Enemy_RealTest.generated.h"

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

	//sensing stuff
	UFUNCTION()
	void SenseStuff(TArray<AActor*> testActors);

	UAIPerceptionComponent* perceptionComponent;

	UAISenseConfig_Sight* sightConfig;
	UAISenseConfig_Hearing* soundConfig;
	
};
