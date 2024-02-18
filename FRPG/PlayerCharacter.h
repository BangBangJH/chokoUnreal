// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerWeapon.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class FRPG_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float CameraTimer = 0.0f;
	float CameraTimerWeightedValue = 0.0f;
	bool bCameraZoom = false; //Zoom �̺�Ʈ bool
	bool TargetCameraIndex = 1; //Zoom Ÿ�� ī�޶� �ε���
	bool CurrentCameraIndex = 0; //Zoom ���� ī�޶� �ε���

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void CameraZoomIn();
	void CameraZoomOut();
	void CameraSmoothMove();

	void ReceiveCharacterData(FString StringData);
	void RC_Move();
	bool IsDoll = false;

	void SpawnWeapon();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<APlayerWeapon> WeaponClass;
	APlayerWeapon* PlayerWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool IsBattle = false;
	UPROPERTY(EditAnywhere, Category = "Weapon")
	class UNiagaraSystem* NS_Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	class UCameraComponent* PlayerCam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	class USpringArmComponent* PlayerCamSpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	FVector CameraZoomValue ; //��ġ�ƴ�, (�������� ����, ����ȸ��Y, ����ȸ��Z);
	FVector CameraValue; //��ġ�ƴ�, (�������� ����, ����ȸ��Y, ����ȸ��Z);

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* AttackMontage;
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void AttackEnd();
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void MoveAbleChange(bool Able);
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void AttackTimeOver();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bWaitComboInput = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bComboInput = false;
	bool bAttack = false;
private:
	//ĳ���� ����
	//

	FVector Rc_Position = FVector::ZeroVector;
	FRotator Rc_Rotation = FRotator::ZeroRotator;
	FVector Rc_Velocity = FVector::ZeroVector;

	class AMyPlayerController* PlayerCtr; //�÷��̾� ��Ʈ�ѷ� ����

	void Attack();
	UFUNCTION()
	void ApplyDamage(AActor*& Target);

};
