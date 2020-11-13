// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnState.h"
#include "Resolvable.h"
#include "GameFramework/Actor.h"
#include "Resolver.generated.h"

UCLASS()
class ATLASREFACTOR_API AResolver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResolver();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxConcurrentResolves = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentlyResolving = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TScriptInterface<IResolvable>> QueuedResolvablesPrep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TScriptInterface<IResolvable>> QueuedResolvablesDash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TScriptInterface<IResolvable>> QueuedResolvablesBurst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TScriptInterface<IResolvable>> QueuedResolvablesMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETurnState CurrentlyResolvingTurnState;

private:
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable)
	TArray<TScriptInterface<IResolvable>>& GetFittingArray(ETurnState TurnState);

	UFUNCTION(BlueprintCallable)
	void QueueResolvable(TScriptInterface<IResolvable> Resolvable, ETurnState TurnState);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool bMayProceedToNextTurnState();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool bMayQueueNextResolvable();

	UFUNCTION()
	void OnResolve(){CurrentlyResolving--;};


	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
