// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "DetonateBomb.h"
#include "ProgressBarWidget.h"
#include "LightDetector.h"
#include "prepro2Character.generated.h"

class UInputComponent;

UCLASS(config=Game)
class Aprepro2Character : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;
public:
	Aprepro2Character();

	void Sprint();
	void StopSprint();

	UPROPERTY(EditAnywhere)
	ALightDetector* Target;
	//ADetectionDummy* LightDetection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		UAudioComponent* FootStepAudio;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light")
		USpotLightComponent* Light;
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class Aprepro2Projectile> ProjectileClass;



	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/** Bomb class to spawn */
	UPROPERTY(EditAnywhere, Category = Bomb)
		TSubclassOf<class ADetonateBomb> BombClass;

	UPROPERTY(EditAnywhere, Category = Perception)
		TSubclassOf<class ALightDetector> LightDetectionClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	float VisionBarMax = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
		float footStepDelay = 0.5f;
	float FootStepTimer=0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	float SprintBarMax = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
		float PulseCooldown = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
		float RayCastDistance = 400;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
		float PlantTime = 4;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
		float mHealth = 300;
	//How much faster you go while sprinting
	UPROPERTY(Category = Custom, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float SpeedMult = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool UseXray=true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float XrayRegen = 0.1;

	UPROPERTY(EditFixedSize)
	uint8 mMaxBombs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 mNumBombs;

	UFUNCTION(BlueprintCallable, category = "pickup")
		void PickUpBomb(ADetonateBomb* bomb);


	UFUNCTION(BlueprintCallable, category = "pickup")
		void PickUpVisionBoost(float boost);

	UFUNCTION(BlueprintCallable, category = "pickup")
		bool IsPickUpTriggerActivated() { return mInsideTriggerBox; }

	UFUNCTION(BlueprintCallable, category = "pickup")
		void SetTriggerActive(bool val) { mInsideTriggerBox = val; }

	
	/*UPROPERTY(EditAnywhere)
		static const int mMaxBombs;*/

public: 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, category = "XRAY")
		bool GetXray() { return *XrayOn; }

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	uint8 GetNumBombs() const		{ return mNumBombs;     }
	int GetBombPlanted() const		{ return mBombsPlanted;   }
	int GetBombSelected() const		{ return mBombSelected; }

	//float InternalTakeRadialDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

protected:
	void StartCrouch();
	void EndCrouch();
	/** Fires a projectile. */
	void OnFire();
	void TogglePause();
	void ToggleXray();
	//Throw a bomb
	void Bomb();

	void DetonateAllBombs();

	void DetonateBomb();

	void BombPulse();

	void SelectBomb();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	void FootStepNoise();

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UProgressBarWidget> mProgressBarsClass;

	class UProgressBarWidget* mProgressBars;
	
	float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<UUserWidget> PauseWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	bool Keycard = false;
private:
	bool* XrayOn;
	TArray<ADetonateBomb*> mBombs;
	int mBombsPlanted;
	int mBombSelected;
	void InitBombs();
	float VisionBar;
	
	bool GamePaused;

	void BombPlant();
	void BombStopPlant();
	bool PlantingBomb=false;
	float PlantProgress=0;

	float SprintBar;
	bool Sprinting;
	float sprintSpeed;
	float WalkSpeed;

	float PulseRecharge=100;

	bool mInsideTriggerBox;
	

	
};

