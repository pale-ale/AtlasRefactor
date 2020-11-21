// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GridManager.h"
#include "TimerManager.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "RefactorController.h"
#include "Resolver.h"
#include "GameFramework/GameModeBase.h"
#include "AtlasRefactorGameState.h"
#include "AtlasRefactorGamemode.generated.h"

USTRUCT(BlueprintType)
struct FUnitLocation{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AUnitBase> Unit;
	UPROPERTY(EditDefaultsOnly)
	FIntPoint Location;
};
/**
 * 
 */
UCLASS()
class ATLASREFACTOR_API AAtlasRefactorGamemode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	AResolver* Resolver;

	UPROPERTY(BlueprintReadOnly)
	UGridManager* GridManager;

	UPROPERTY(BlueprintReadWrite)
	FDateTime EndOfTurnTime;

	UPROPERTY(EditDefaultsOnly)
	TArray<FUnitLocation> InitialSpawns;

	UPROPERTY(BlueprintReadOnly)
	TArray<ARefactorController*> ConnectedNetworkControllers;

	UFUNCTION(BlueprintCallable)
	void GetResolveTime(ETurnState TurnState, float& ResolveTime);

	UFUNCTION(BlueprintCallable)
	void OnUnitDied(AUnitBase * Unit);

	UFUNCTION()
	void QueueResolvables();

	UFUNCTION()
	void OnTimeIsUp();

	UFUNCTION()
	virtual void Tick(float DeltaTime) override;

	virtual void PostLogin(APlayerController * NewPlayer) override;

protected:
	virtual void BeginPlay() override;
};
