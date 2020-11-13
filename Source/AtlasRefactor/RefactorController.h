// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/Plane.h"
#include "GameFramework/PlayerController.h"
#include "AbilityBase.h"
#include "Components/WidgetInteractionComponent.h"
#include "Engine/World.h"

#include "W_MainHud.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "RefactorController.generated.h"

class AAtlasRefactorGamemode;

UENUM(BlueprintType)
enum class EUserSelectionState : uint8
{
    SelectingUnit  			UMETA(DisplayName="SelectingUnit"),
    Default  				UMETA(DisplayName="Default"),
    SelectingMovement 		UMETA(DisplayName="SelectingMovement"),
    SelectingTargetTile  	UMETA(DisplayName="SelectingTargetTile"),
    SelectingTargetUnit  	UMETA(DisplayName="SelectingTargetUnit"),
    SelectingTargetSmooth  	UMETA(DisplayName="SelectingTargetSmooth"),
    NoSelectAllowed  		UMETA(DisplayName="NoSelectAllowed"),
    LAST  					UMETA(Hidden)
};
/**
 * 
 */
UCLASS()
class ATLASREFACTOR_API ARefactorController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ARefactorController(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere)
	UWidgetInteractionComponent * WidgetInteractionComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainHudWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	UMainHudWidget* HUDWidget;

	UPROPERTY()
	EUserSelectionState CurrentSelecitonState;

	UPROPERTY()
	AUnitBase* SelectedUnit;

	UPROPERTY()
	UAbilityBase* PreviewedAbility;

	UPROPERTY()
	FPlane FallbackPlane = FPlane(FVector(0, 0, 70), FVector(0, 0, 1));

	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetUnderCursor(FPlane fbPlane, FVector& Location, AActor*& Actor);


private:

	void LMBDown();
	void LMBUp();
	
	void RMBDown();

	void OneDown();
	void TwoDown();
	void FiveDown();

	UFUNCTION(BlueprintCallable)
	void DefaultToUnitSelect();
	UFUNCTION(BlueprintCallable)
	void DefaultToTileSelect(UAbilityBase* Ability);
	UFUNCTION(BlueprintCallable)
	void DefaultToSmoothSelect(UAbilityBase* Ability);
	UFUNCTION(BlueprintCallable)
	void UnitSelectToDefault(AUnitBase* NewSelectedUnit);
	UFUNCTION(BlueprintCallable)
	void SmoothSelectToDefault();
	UFUNCTION(BlueprintCallable)
	void TileSelectToDefault();
	
protected:
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void EndTurnTimeReached();
};
