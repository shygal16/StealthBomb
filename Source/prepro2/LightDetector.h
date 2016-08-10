// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Perception/AIPerceptionComponent.h"
#include "LightDetector.generated.h"

UCLASS()
class PREPRO2_API ALightDetector : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightDetector();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mDummy;
	void SetActive(bool active) { mActive = active; }
	bool IsActive() { return mActive; }
private:
	bool mActive=false;
	
};
