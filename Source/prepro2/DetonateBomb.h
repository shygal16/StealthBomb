// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DestructibleMesh.h"
#include "Components/DestructibleComponent.h"
#include "Explosive.h"
#include "DetonateBomb.generated.h"

UCLASS()
class PREPRO2_API ADetonateBomb : public AExplosive
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADetonateBomb();

	UPROPERTY(EditAnywhere)
		UDestructibleComponent* mBombModel;

	UPROPERTY(EditAnywhere)
		float mDisappearDelay;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void SetActive(bool active);

	bool IsActive()		{ return mIsActive; }

	void XRayBomb(bool On);
	void PingNoise();
protected:
	bool mIsActive;

private:
	UDestructibleMesh* mOriginalMesh;
	//UPrimitiveComponent* mMesh;
	float mDisappearTimer;
};
