// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Escape.h"
#include "OpenDoor.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *GetOwner()->GetName())
	}
	GetTotalMassOfActorsOnPlate();
	//ActorThatOpens=GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//if the actor that opens is in the volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OnOpen.Broadcast();
	}
	//check if it's time to close the door
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	//finding all the actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Iterate through them adding their masses
	for (const auto& Actor : OverlappingActors) {
		//Actor->we are searching actor for component <name of the component>-> on this component we are using a method
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT(" %s on pressureplate"), *Actor->GetName())
	}
	return TotalMass;
}

