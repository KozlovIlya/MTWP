// Copyright Epic Games, Inc. All Rights Reserved.

#include "MTWPProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h" 

#include "MTWPAudioSubsystem.h"

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
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}

	if (auto GI = GetGameInstance(); IsValid(GI))
	{
		if (auto AS = GI->GetSubsystem<UMTWPAudioSubsystem>(); IsValid(AS))
		{
			auto HitSwitchValue = HitSwitchValueDefault;
			if (auto Material = Hit.Component->GetMaterial(0); IsValid(Material))
			{
				if (auto PhysMaterial = Material->GetPhysicalMaterial())
				{
					if (PhysMaterial->SurfaceType == EPhysicalSurface::SurfaceType1)
					{
						HitSwitchValue = HitSwitchValueMetal;
					}
				}
			}
			AS->PlaySound(HitSoundEvent, FMTWPAudioCreationParams(Hit.ImpactPoint), FMTWPAudioPlaybackParams({ HitSoundEventCooldownSeconds, HitSwitchValue, HitSwitchGroupName }));
		}
	}

	
}