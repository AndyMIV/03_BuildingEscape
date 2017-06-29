// Andy Montoya 2017

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float reach = 100.0f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// ray cast and grab whats in reach
	void Grab();
	void Release();

	// find (assumed)  attached physics handle
	void FindPhysicsHandleComponent();

	// setup (assumed) attached input component
	void SetupInputComponent();

	// return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	// return final object position
	FVector GetLineTraceEnd();

	FVector GetLineTraceStart();
	
};
