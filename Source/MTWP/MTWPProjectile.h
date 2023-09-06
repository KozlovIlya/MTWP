// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <AkAudioEvent.h>
#include <AkSwitchValue.h>

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

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAkAudioEvent> HitSoundEvent;

	UPROPERTY(EditDefaultsOnly)
	float HitSoundEventCooldownSeconds = .5f;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAkSwitchValue> HitSwitchValueDefault;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAkSwitchValue> HitSwitchValueMetal;

	UPROPERTY(EditDefaultsOnly)
	FString HitSwitchGroupName;

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

