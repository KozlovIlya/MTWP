// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <AkAudioEvent.h>
#include <AkSwitchValue.h>

#include "MTWPAudioSubsystem.h"

#include "MTWPProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class AMTWPProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Projectile | Audio")
	TObjectPtr<UMTWPAudioEntity> AudioEntity;

 // Audio
//public:
//
//	UPROPERTY(EditDefaultsOnly, Category = "Projectile | Audio")
//	FMTWPAudioEventDefinition_WWise AudioEventDefinition;
//
//	UPROPERTY(EditDefaultsOnly, Category = "Projectile | Audio")
//	FMTWPAudioPlaybackParamsDefinition_WWise AudioPlaybackParamsDefinition;
//
//	UPROPERTY(EditDefaultsOnly, Category = "Projectile | Audio")
//	FMTWPRtpcDefinition_WWise HitSwitchDefinitionDefault;
//
//	UPROPERTY(EditDefaultsOnly, Category = "Projectile | Audio")
//	FMTWPRtpcDefinition_WWise HitSwitchDefinitionMetal;


public:
	AMTWPProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

