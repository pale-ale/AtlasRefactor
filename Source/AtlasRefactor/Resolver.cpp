// Fill out your copyright notice in the Description page of Project Settings.


#include "Resolver.h"

// Sets default values
AResolver::AResolver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);
	bAllowTickBeforeBeginPlay = false;
}

bool AResolver::bMayProceedToNextTurnState(){
	return (GetFittingArray(CurrentlyResolvingTurnState).Num() == 0) && (CurrentlyResolving == 0);
}

bool AResolver::bMayQueueNextResolvable(){
	return (CurrentlyResolving < MaxConcurrentResolves) && (GetFittingArray(CurrentlyResolvingTurnState).Num() > 0);
}

void AResolver::QueueResolvable(TScriptInterface<IResolvable> Resolvable, ETurnState TurnState){
	UE_LOG(LogTemp, Warning, TEXT("Queueing Resolvable in phase %d"), (int32)TurnState);
	GetFittingArray(TurnState).Add(Resolvable);
}


TArray<TScriptInterface<IResolvable>>& AResolver::GetFittingArray(ETurnState TurnState){
	switch (TurnState)
	{
	case ETurnState::Prep:
		return QueuedResolvablesPrep;
	case ETurnState::Dash:
		return QueuedResolvablesDash;
	case ETurnState::Burst:
		return QueuedResolvablesBurst;
	case ETurnState::Move:
		return QueuedResolvablesMove;
	default:
		return QueuedResolvablesMove;
	}
}



// Called when the game starts or when spawned
void AResolver::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

// Called every frame
void AResolver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bMayProceedToNextTurnState()){
		if (CurrentlyResolvingTurnState == ETurnState::Move){
			UE_LOG(LogTemp, Warning, TEXT("Finished Resolving"));
			SetActorTickEnabled(false);
			return;
		}else{
			CurrentlyResolvingTurnState = (ETurnState)((int32)CurrentlyResolvingTurnState + 1);
			UE_LOG(LogTemp, Warning, TEXT("Proceeding to next phase: %d"), (int32)CurrentlyResolvingTurnState);
			return;
		}
	}else{
		if(bMayQueueNextResolvable()){
			UE_LOG(LogTemp, Warning, TEXT("Starting resolve"));
			CurrentlyResolving++;
			TScriptInterface<IResolvable> nextResolvable = GetFittingArray(CurrentlyResolvingTurnState)[0];
			FunctionDelegate del;
			del.BindUFunction(this, "OnResolve");
			nextResolvable->Execute_SetResolveCallback(nextResolvable.GetObject(), del);
			nextResolvable->Execute_Resolve(nextResolvable.GetObject(), CurrentlyResolvingTurnState);
			MaxConcurrentResolves = (CurrentlyResolvingTurnState == ETurnState::Move) ? 8 : 1;
			GetFittingArray(CurrentlyResolvingTurnState).RemoveAt(0);
		}
	}
}

