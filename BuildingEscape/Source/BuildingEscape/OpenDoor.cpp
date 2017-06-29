// Andy Montoya 2017

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *GetOwner()->GetName());
	}
	// sets a pawn to an actor. REMEMBER A Pawn is a Actor
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



	// poll the trigger volume every frame. 
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) {  // TODO make into a parameter
		OnOpenRequest.Broadcast();
	}
	else  {
		OnCloseRequest.Broadcast();
	}
	// if the actor that opens is in the volume, then we open the door.

	// check if its time to close the door:

}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {

	// find all the overlapping actors
	// Give me a tarray of overlapping pointers
	TArray<AActor*> OverlappingActors;
	float TotalMass = 0.1f;

	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// iterate through them adding their masses
	// actor does not change
		for (const auto* element : OverlappingActors)
		{
			
			TotalMass += element->FindComponentByClass<UPrimitiveComponent>()->GetMass();
			UE_LOG(LogTemp, Warning, TEXT("object on plate: %s"), *(element->GetName()));
			
			
		}

	

	return TotalMass;

}