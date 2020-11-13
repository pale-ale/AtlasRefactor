#pragma once

#include "Blueprint/UserWidget.h"
#include "UnitBase.h"

#include "W_MainHud.generated.h"


UCLASS(ABSTRACT)
class UMainHudWidget : public UUserWidget{
    GENERATED_BODY()

   

public:
    UPROPERTY(BlueprintReadWrite)
    AUnitBase * DisplayedUnit;

    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void DisplayUnitInfo(AUnitBase* Unit);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void HideUnitInfo();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetEnableControls(bool Enable);
};