// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityBase.h"
#include "SpawnedAbilityBase.h"
#include "UnitBase.h"


void UAbilityBase::Activate_Implementation(FVector newMousePos){
    if (GEngine) //Intellisense hiccup
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Activated"));
    }
}

void UAbilityBase::StartPreview_Implementation(){
    if (GEngine) //Intellisense hiccup
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Started Preview"));
    }
}

void UAbilityBase::StopPreview_Implementation(){
    if (GEngine) //Intellisense hiccup
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Stopped Preview"));
    }
}

void UAbilityBase::UpdateMousePos_Implementation(FVector newMousePos){
if (GEngine) //Intellisense hiccup
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("New MousePos"));
    }
}

void UAbilityBase::GetUnit(AUnitBase*& parentUnit){
    if(parentComponent && parentComponent->GetOwner()){
        AActor * parent = parentComponent->GetOwner();
        if (parent->IsA<AUnitBase>()){
            parentUnit = (Cast<AUnitBase, AActor>(parent));
        }
    }
}

void UAbilityBase::SetResolveCallback_Implementation(const FunctionDelegate& Callback){
    OnFinishResolvingCallback = Callback;
}

void UAbilityBase::CallFinishResolvingCallback(){
    OnFinishResolvingCallback.ExecuteIfBound();
}
