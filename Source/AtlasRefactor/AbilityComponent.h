// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitBase.h"
#include "TurnState.h"
#include "TimerManager.h"
#include "Resolvable.h"
#include "AbilityComponent.generated.h"

class UAbilityBase;
class ASpawnedAbilityBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ATLASREFACTOR_API UAbilityComponent : public UActorComponent, public IResolvable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();
	FTimerDelegate delegate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<UAbilityBase>> AbilitiesClasses;

	UPROPERTY(BlueprintReadOnly)
	int32 ResolvingAbilities;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UAbilityBase*> AbilitiesObjects;

	UFUNCTION(BlueprintCallable)
	void ActivateAbility(int abilityIndex);

	//UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Resolve_Implementation(ETurnState TurnState);

	//UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void getResolveTimeSeconds_Implementation(float& duration, ETurnState TurnState);

	UFUNCTION(BlueprintCallable)
	ASpawnedAbilityBase * SpawnAbility(TSubclassOf<ASpawnedAbilityBase> abilityToSpawn);

	UFUNCTION(BlueprintCallable)
	AActor * SpawnOther(TSubclassOf<AActor> abilityToSpawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void SetResolveCallback(const FunctionDelegate& Callback);
		void SetResolveCallback_Implementation(const FunctionDelegate& Callback);


private:
	UFUNCTION()
	void ChildResolvedCallback(){
		ResolvingAbilities--;
	};

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly)
	FunctionDelegate OnFinishResolvingCallback;
		
};
