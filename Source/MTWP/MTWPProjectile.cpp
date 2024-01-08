// Copyright Epic Games, Inc. All Rights Reserved.

#include "MTWPProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "EngineUtils.h"

#include "MTWPAudioSubsystem.h"

#include "MTWPAudioInterface_WWise.h"
#include "AkGameplayStatics.h"

#include "Kismet/GameplayStatics.h"

#include "AkComponent.h"

AMTWPProjectile::AMTWPProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AMTWPProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AMTWPProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}

	if (auto GI = GetGameInstance(); IsValid(GI))
	{
		if (auto AS = GI->GetSubsystem<UMTWPAudioSubsystem>(); IsValid(AS) && IsValid(AS->WWiseAudioInterface))
		{
			//auto AI = AS->WWiseAudioInterface->CreateAudioInstanceAtLocation(AudioEntity, HitComp->GetComponentLocation());
			//auto AI = AS->WWiseAudioInterface->CreateAudioInstance2D(AudioEntity);
		    auto AI = AS->WWiseAudioInterface->CreateAudioInstanceAttached(AudioEntity, HitComp);
			if (IsValid(AI))
			{
				if (IsValid(OtherActor))
				{
					FVector RelativeVelocity = OtherActor->GetVelocity() - this->GetVelocity();
					float HitPower = 0.000002f * FMath::Pow(RelativeVelocity.Size(), 2);
					AI->SetParameterValueNumeric("HitPower", HitPower);
				}

				if (auto Material = Hit.Component->GetMaterial(0); IsValid(Material))
				{
					if (auto PhysMaterial = Material->GetPhysicalMaterial())
					{
						if (PhysMaterial->SurfaceType == EPhysicalSurface::SurfaceType1)
						{
							AI->SetParameterValueString("Surface", "Metal");
							//Cast<UMTWPAudioInstance_WWise>(AI)->Component->SetSwitch(nullptr, "Surface", "Metal");
						}
					}
				}
				AI->Play();
			}
		}
		else
		{
            UE_LOG(LogTemp, Warning, TEXT("Audio Subsystem or Audio Interface is not valid"));
		}
	}
}
