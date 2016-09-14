// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "CompassWidget.h"
#include "Compass.generated.h"

UCLASS()
class PREPRO2_API ACompass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACompass();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mBody;
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mRing;

	UStaticMeshComponent* mEmpty;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
		bool mCompassShown = false;

	void UpdateState();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		AEnemy_RealTest* mEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		float mAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		float mBlinkRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blink")
		bool mBlinkVisible = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blink")
		float mBlinkDistance = 2000.f;

	UFUNCTION(BlueprintCallable, category = "Enemies")
		void GetQuadrant(float DeltaTime);

	float sign = 1.f;

	float mBlinkTimer = 1.f;
};
