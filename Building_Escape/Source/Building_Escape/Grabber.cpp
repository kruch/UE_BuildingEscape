// Fill out your copyright notice in the Description page of Project Settings.
#include "Building_Escape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

//find attached physics
void UGrabber::FindPhysicsHandleComponent()
{
	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	//Look for attached physics handle
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - input component found"), *GetOwner()->GetName())
			InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - missing input component"), *GetOwner()->GetName())
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	//if the physics handle is attached 
	if (PhysicsHandle->GrabbedComponent)
	{
		//move object that player is holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

void UGrabber::Grab()
{
	//Try and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); //gets the mesh in our case
	auto ActorHit = HitResult.GetActor();

	//If we hit something then attach a physics handle

	if (ActorHit != nullptr) {
		if (!PhysicsHandle) { return; }
		// attach physics handle
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None, //no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation()
			//true //allow rotation
		);
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
	
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	//Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	///Line-trace
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return Hit;
}

FVector UGrabber::GetReachLineStart()
{
	/// get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotator);

	//log out to test
	//UE_LOG(LogTemp, Warning, TEXT("location:  %s, position %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotator.ToString());
	/// Draw red trace to world to visualize 
	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	/// get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotator);

	//log out to test
	//UE_LOG(LogTemp, Warning, TEXT("location:  %s, position %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotator.ToString());
	/// Draw red trace to world to visualize 
	return PlayerViewPointLocation + PlayerViewPointRotator.Vector() * Reach;
}