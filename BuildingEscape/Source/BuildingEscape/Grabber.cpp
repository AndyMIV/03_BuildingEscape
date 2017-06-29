// Andy Montoya 2017

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT      // helpful for us to see what variables are modified as a ressult of function


/// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

/// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();

}

void UGrabber::FindPhysicsHandleComponent() {
	/// Look for attached physics handle - generic signature, looks for a class
	// protection
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		// physics handle is found
		FString object = GetOwner()->GetName();
		UE_LOG(LogTemp, Error, TEXT("Missing Physics in object: %s"), *object);
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))

		// Line Trace - try and reach any actors with physics body collision channel set
		auto HitResult = GetFirstPhysicsBodyInReach();
		auto ComponentToGrab = HitResult.GetComponent();
		auto ActorHit = HitResult.GetActor();

		// if we hit something, then attach a physics handle
		if (ActorHit) {
			// protection
			if (!PhysicsHandle) { return; }
			// attach physics handle allowing rotation
			PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None,
				ComponentToGrab->GetOwner()->GetActorLocation(),
				ComponentToGrab->GetOwner()->GetActorRotation());
		}
	
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released"))
		if (!PhysicsHandle) { return; }
		PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetLineTraceEnd() {
	/// Get player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);


	FVector LineTraceDirection = PlayerViewPointRotation.Vector();

	return PlayerViewPointLocation + (LineTraceDirection * reach);
}

FVector UGrabber::GetLineTraceStart() {
	/// Get player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}


void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// protection
	if (!PhysicsHandle) { return; }
	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		//move the object that we're holding.
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}

/// look for attached input component
void UGrabber::SetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Missing input component for object %s"),
			*(GetOwner()->GetName()))	
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Found input component"))

			/// bind the input action
			/// when we push a key, that is attached to grab, then we call a function on this (which is the grabber)
			InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}


const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	
	/// Setup Query Params - Simple or complex geometry setup, actor to ignore (ignore ourself)
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line trace (Ray-cast) out to reach distance (private variable)
	/// we are sorting for an object type, which is a physics body
	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		GetLineTraceStart(),
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);


	/// See what we hit
	AActor *ActorHit = hit.GetActor();
	// makes sure actor isnt associated with nothing...
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Object hit: %s"),
			*(ActorHit->GetName()));
	}
	return hit;
}

