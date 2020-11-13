// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityComponent.h"
#include "AbilityBase.h"
#include "SpawnedAbilityBase.h"
#include "Engine.h"

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UAbilityComponent::Resolve_Implementation(ETurnState TurnState)
{
	UAbilityBase *resolvingAbility;
	float resolvingAbilityDelay = 0;
	for (int i = 0; i < AbilitiesObjects.Num(); i++)
	{
		resolvingAbility = AbilitiesObjects[i];
		Execute_GetResolveTimeSeconds(resolvingAbility, resolvingAbilityDelay, TurnState);
		if (resolvingAbility->bNeedsResolving && resolvingAbility->ResolvingTurnStates.Contains(TurnState))
		{
			Execute_Resolve(resolvingAbility, TurnState);
			ResolvingAbilities++;
			FTimerHandle handle;
			delegate.BindUFunction(this, "Resolve", TurnState);
			GetWorld()->GetTimerManager().SetTimer(handle, delegate, 1.0f, false, resolvingAbilityDelay);
			return;
		}
	}
	if (ResolvingAbilities == 0){
		OnFinishResolvingCallback.ExecuteIfBound();
	}
}

void UAbilityComponent::SetResolveCallback_Implementation(const FunctionDelegate& Callback){
    OnFinishResolvingCallback = Callback;
}

void UAbilityComponent::getResolveTimeSeconds_Implementation(float &duration, ETurnState TurnState)
{
	UAbilityBase *resolvingAbility;
	for (int i = 0; i < AbilitiesObjects.Num(); i++)
	{
		resolvingAbility = AbilitiesObjects[i];
		if (resolvingAbility->bNeedsResolving && resolvingAbility->ResolvingTurnStates.Contains(TurnState))
		{
			float partialduration = 0;
			Execute_GetResolveTimeSeconds(resolvingAbility, partialduration, TurnState);
			duration += partialduration;
		}
	}
}

// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < AbilitiesClasses.Num(); i++)
	{
		if (AbilitiesClasses[i] != NULL)
		{
			UAbilityBase *newAbilityObject = NewObject<UAbilityBase>(this, AbilitiesClasses[i]);
			newAbilityObject->parentComponent = this;
			FunctionDelegate del;
			del.BindUFunction(this, "ChildResolvedCallback");
			Execute_SetResolveCallback(newAbilityObject, del);
			AbilitiesObjects.Add(newAbilityObject);
		}
	}
}

ASpawnedAbilityBase *UAbilityComponent::SpawnAbility(TSubclassOf<ASpawnedAbilityBase> abilityToSpawn)
{
	return GetWorld()->SpawnActor<ASpawnedAbilityBase>(abilityToSpawn);
}

// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UAbilityComponent::ActivateAbility(int abilityIndex)
{
	if (abilityIndex < 0 || abilityIndex >= AbilitiesObjects.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bad AbilityIndex in UnitBase.cpp"));
		return;
	}
	AbilitiesObjects[abilityIndex]->StartPreview();
	auto c = ((AUnitBase *)GetOwner())->OwningController;
}

AActor *UAbilityComponent::SpawnOther(TSubclassOf<AActor> actorToSpawn)
{
	return GetWorld()->SpawnActor<AActor>(actorToSpawn);
}
