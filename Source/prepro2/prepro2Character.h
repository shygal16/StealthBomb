// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
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

	void PlayFootStep();

	UPROPERTY(EditAnywhere)
	ALightDetector* Target;
	//ADetectionDummy* LightDetection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterInfo)
		TSubclassOf <UCameraShake> CameraShaker;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		UAudioComponent* FootStepAudio;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Component")
		USoundCue* FootStepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Component")
		USoundCue* LightOn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Component")
		USoundCue* LightOff;

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



	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;



	UPROPERTY(EditAnywhere, Category = Perception)
		TSubclassOf<class ALightDetector> LightDetectionClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	float VisionBarMax = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
		float footStepDelay = 0.27f;
	float FootStepTimer=1;
	float FootstepPitch=1;
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
		bool Hiding = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float XrayRegen = 0.1;

	
	UFUNCTION(BlueprintCallable, category = "pickup")
		void PickUpVisionBoost(float boost);

	UFUNCTION(BlueprintCallable, category = "pickup")
		bool IsPickUpTriggerActivated() { return mInsideTriggerBox; }

	UFUNCTION(BlueprintCallable, category = "pickup")
		void SetTriggerActive(bool val) { mInsideTriggerBox = val; }



public: 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, category = "XRAY")
		bool GetXray() { return *XrayOn; }

	UFUNCTION(BlueprintCallable, category = "GameState")
		void GameExit();
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;


	//float InternalTakeRadialDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

protected:
	void StartCrouch();
	void EndCrouch();

	void TogglePause();
	void ToggleXray();

	void TurnFlashLightOff();
	void TurnFlashLightOn();


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

	
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UProgressBarWidget> mProgressBarsClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	class UProgressBarWidget* mProgressBars;
	
	float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<UUserWidget> PauseWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<UUserWidget> GameOverClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<UUserWidget> GameWinClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		int Keycard = 0;

private:


	//Flashlight
	float VisionBar;
	bool* XrayOn;
	bool FlashLightOn;
	//Extra Menus
	bool GamePaused;
	void GameOver();

	//Movement
	float SprintBar;
	bool Sprinting;
	float sprintSpeed;
	float WalkSpeed;

	//Misc
	float PulseRecharge=100;
	bool mInsideTriggerBox;

};

