// Fill out your copyright notice in the Description page of Project Settings.


#include "RefactorController.h"
#include "AtlasRefactorGamemode.h"

ARefactorController::ARefactorController(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
    WidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent>("WidgetInteraction");
    WidgetInteractionComponent->bEditableWhenInherited = true;
}

void ARefactorController::BeginPlay(){
    Super::BeginPlay();
    if (IsLocalController()){
        HUDWidget = CreateWidget<UMainHudWidget>(this, HUDWidgetClass);
        HUDWidget->AddToViewport();
        FInputModeGameAndUI mode;
        mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        mode.SetHideCursorDuringCapture(true);
        SetInputMode(mode);
    }
}

void ARefactorController::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
    if (PreviewedAbility){
        FVector hitLocation;
        AActor* _;
        GetUnderCursor(FallbackPlane, hitLocation, _);
        PreviewedAbility->UpdateMousePos(hitLocation);
    }
}


void ARefactorController::DefaultToUnitSelect(){
    this->SelectedUnit = nullptr;
    HUDWidget->HideUnitInfo();
    CurrentSelecitonState = EUserSelectionState::SelectingUnit;
}

void ARefactorController::DefaultToTileSelect(UAbilityBase* Ability){
    this->PreviewedAbility = Ability;
    CurrentSelecitonState = EUserSelectionState::SelectingTargetTile;
}

void ARefactorController::DefaultToSmoothSelect(UAbilityBase* Ability){
    this->PreviewedAbility = Ability;
    CurrentSelecitonState = EUserSelectionState::SelectingTargetSmooth;
}

void ARefactorController::UnitSelectToDefault(AUnitBase* NewSelectedUnit){
    this->SelectedUnit = NewSelectedUnit;
    HUDWidget->DisplayUnitInfo(NewSelectedUnit);
    CurrentSelecitonState = EUserSelectionState::Default;
}

void ARefactorController::SmoothSelectToDefault(){
    this->PreviewedAbility = nullptr;
    CurrentSelecitonState = EUserSelectionState::Default;
}

void ARefactorController::TileSelectToDefault(){
    this->PreviewedAbility = nullptr;
    CurrentSelecitonState = EUserSelectionState::Default;
}


void ARefactorController::GetUnderCursor(FPlane fbPlane, FVector& location, AActor*& actor){
    TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
    TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
    TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
    TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
    FHitResult hit;
    GetHitResultUnderCursorForObjects(TraceObjectTypes, true, hit);
    if (hit.Actor.IsValid()){
        location = hit.Location;
        actor = hit.Actor.Get();
    } else {
        FVector wloc;
        FVector wdir;
        FVector endPoint;
        DeprojectMousePositionToWorld(wloc, wdir);
        wdir *= 10000;
        endPoint = wloc + wdir;
        location = FMath::LinePlaneIntersection(wloc, endPoint, fbPlane);
    }
}

void ARefactorController::SetupInputComponent(){
    Super::SetupInputComponent();
    InputComponent->BindAction("LMB", IE_Pressed, this, &ARefactorController::LMBDown);
    InputComponent->BindAction("LMB", IE_Released, this, &ARefactorController::LMBUp);
    InputComponent->BindAction("RMB", IE_Pressed, this, &ARefactorController::RMBDown);
    InputComponent->BindAction("One", IE_Pressed, this, &ARefactorController::OneDown);
    InputComponent->BindAction("Two", IE_Pressed, this, &ARefactorController::TwoDown);
    InputComponent->BindAction("Five", IE_Pressed, this, &ARefactorController::FiveDown);
}

void ARefactorController::LMBDown(){
    FVector location;
    AActor* hitActor;
    GetUnderCursor(FallbackPlane, location, hitActor);
    if (CurrentSelecitonState == EUserSelectionState::SelectingUnit){
        auto unit = (AUnitBase*)hitActor;
        if (unit){
            UnitSelectToDefault(unit);
        }
    }else if (CurrentSelecitonState == EUserSelectionState::SelectingTargetTile){
        WidgetInteractionComponent->PressPointerKey(FKey("LeftMouseButton"));
    }else if (CurrentSelecitonState == EUserSelectionState::SelectingTargetSmooth){
        PreviewedAbility->Activate(location);
    }
}

void ARefactorController::LMBUp(){
    if (WidgetInteractionComponent){
        WidgetInteractionComponent->ReleasePointerKey(FKey("LeftMouseButton"));
    }else{
        UE_LOG(LogTemp, Warning, TEXT("No WidgetInteractionComponent"));
    }
}

void ARefactorController::RMBDown(){
    if (CurrentSelecitonState == EUserSelectionState::Default){
        DefaultToUnitSelect();
    }else if (CurrentSelecitonState == EUserSelectionState::SelectingTargetTile || 
              CurrentSelecitonState == EUserSelectionState::SelectingTargetSmooth){
        if(PreviewedAbility){
            PreviewedAbility->StopPreview();
        }
    }
}

void ARefactorController::OneDown(){
    if (CurrentSelecitonState == EUserSelectionState::Default){
        SelectedUnit->AbilityComponent->ActivateAbility(0);
    }
}

void ARefactorController::TwoDown(){
    if (CurrentSelecitonState == EUserSelectionState::Default){
        SelectedUnit->AbilityComponent->ActivateAbility(1);
    }
}

void ARefactorController::FiveDown(){
    if (CurrentSelecitonState == EUserSelectionState::Default){
        SelectedUnit->AbilityComponent->ActivateAbility(4);
    }
}

void ARefactorController::EndTurnTimeReached(){
    HUDWidget->SetEnableControls(false);
    if (CurrentSelecitonState == EUserSelectionState::SelectingTargetSmooth){
        PreviewedAbility->StopPreview();
    }
}