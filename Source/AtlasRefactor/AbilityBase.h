// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture.h"
#include "Resolvable.h"
#include "TurnState.h"
#include "AbilityComponent.h"
#include "AbilityBase.generated.h"

class AUnitBase;
class ASpawnedAbilityBase;

UCLASS(ABSTRACT, Blueprintable)
class ATLASREFACTOR_API UAbilityBase : public UObject, public IResolvable
{
	GENERATED_BODY()

	public:
		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D * image;

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<ETurnState> ResolvingTurnStates;

		UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<ASpawnedAbilityBase> SpawnedAbilityClass;

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ASpawnedAbilityBase * SpawnedAbility;

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UAbilityComponent * parentComponent;

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString name;

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 cooldown;

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 remainingCooldown;

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bNeedsResolving;

		UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Activate(FVector newMousePos);
		void Activate_Implementation(FVector newMousePos);

		UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void StartPreview();
		void StartPreview_Implementation();

		UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void StopPreview();
		void StopPreview_Implementation();

		UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void UpdateMousePos(FVector newMousePos);
		void UpdateMousePos_Implementation(FVector newMousePos);

		UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void SetResolveCallback(const FunctionDelegate& Callback);
		void SetResolveCallback_Implementation(const FunctionDelegate& Callback);

		UFUNCTION(BlueprintCallable, BlueprintPure)
		void GetUnit(AUnitBase*& parentUnit);

		UFUNCTION(BlueprintCallable)
		void CallFinishResolvingCallback();

		UPROPERTY(BlueprintReadOnly)
		FunctionDelegate OnFinishResolvingCallback;
};
