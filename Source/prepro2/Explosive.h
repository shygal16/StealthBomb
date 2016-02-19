// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/PhysicsEngine/RadialForceComponent.h"
#include "Explosive.generated.h"

UCLASS(abstract)
class PREPRO2_API AExplosive : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExplosive();

	UPROPERTY(EditAnywhere)
		URadialForceComponent* mRadForce;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* mParticleSystem;

	UPROPERTY(EditAnywhere)
		float mExplosionDelay;

	void Explode();

	bool IsExploded()	{ return mExploded; }
	bool IsBombTriggered()	{ return mBombTriggered; }

	void TriggerBomb()	{ mBombTriggered = true; }

protected:
	bool mBombTriggered;
	float mExplosionTimer;
	bool mExploded;
	
};
