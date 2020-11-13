// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Public/EngineGlobals.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "DamageableInterface.h"
#include "MovementInterface.h"
#include "Math/IntPoint.h"
#include "UnitBase.generated.h"

class UGridManager; 
class UAbilityComponent;
class ARefactorController;

UCLASS(Abstract, Blueprintable)
class ATLASREFACTOR_API AUnitBase : public AActor, public IDamageableInterface, public IMovementInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitBase();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 movementRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 maxHP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 currentHP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 remainingMovementRange;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 gridIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UGridManager * gridManager;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent * UnitMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAbilityComponent * AbilityComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UWidgetComponent * WidgetComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ARefactorController * OwningController;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FIntPoint> MovementWaypoints;

	UFUNCTION(BlueprintCallable, BlueprintPure)
 	int32 GetMoveCost();

	UFUNCTION(BlueprintCallable, BlueprintPure)
 	TArray<FIntPoint> GetMovementTiles();

protected:
	// Called when the game starts or when spawne
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
