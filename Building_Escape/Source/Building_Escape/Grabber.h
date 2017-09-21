// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Grabber.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

	public:
		// Called every frame
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		UGrabber();

	private:
		float Reach = 100.f;

		UPhysicsHandleComponent* PhysicsHandle = nullptr;
		UInputComponent* InputComponent=nullptr;
	
		void Grab(); //raycasts and grabs what's in reach
		void Release(); // called when grabber released
		void FindPhysicsHandleComponent(); //find attached physics handle
		void SetupInputComponent(); //setup attached input component
		const FHitResult GetFirstPhysicsBodyInReach(); //Return hit for first physics body in reach
		FVector GetReachLineStart();//Return current start of reach line
		FVector GetReachLineEnd(); //Return current end of reach line

	protected:
		// Called when the game starts
		virtual void BeginPlay() override;
		
};
