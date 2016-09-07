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

	UPROPERTY(EditAnywhere)
		UWidgetComponent* mWidgetComp;

	UStaticMeshComponent* mEmpty;;

	UPROPERTY(EditAnywhere)
		TSubclassOf <UUserWidget> InWidgetClass;
};
