// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <Blueprint/UserWidget.h>
#include "InventoryUI.h"
#include <NavigationSystem.h>
#include "PlayerCharacter.h"

AMyPlayerController::AMyPlayerController()
{
	bShowMouseCursor = true;
	bReplicates = true;
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	if (bClickRightMouse && bMoveAble)
	{
		MoveToMouseCursor();
	}
}

void AMyPlayerController::InputRightMouseButtonPressed()
{
	bClickRightMouse = true;
}

void AMyPlayerController::InputRightMouseButtonReleased()
{
	bClickRightMouse = false;
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("RightClick", EInputEvent::IE_Pressed, this, &AMyPlayerController::InputRightMouseButtonPressed);
	InputComponent->BindAction("RightClick", EInputEvent::IE_Released, this, &AMyPlayerController::InputRightMouseButtonReleased);
	InputComponent->BindAction("Inventory", EInputEvent::IE_Pressed, this, &AMyPlayerController::OpenInventory);
	InputComponent->BindAction("Test1", EInputEvent::IE_Pressed, this, &AMyPlayerController::TestInventory);
}


void AMyPlayerController::Server_MovePlayer_Implementation(APlayerController* PlayerCtr, FVector Destination)
{
	Multicast_MovePlayer(PlayerCtr, Destination);
}

void AMyPlayerController::Multicast_MovePlayer_Implementation(APlayerController* PlayerCtr, FVector Destination)
{
	MovePlayer(PlayerCtr, Destination);
}

void AMyPlayerController::MovePlayer(APlayerController* PlayerCtr, FVector Destination)
{
	APawn* ControlledPawn = PlayerCtr->GetPawn();
	if (ControlledPawn != nullptr)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerCtr->GetCharacter());
		if (PlayerCharacter->bAttack)
		{
			PlayerCharacter->AttackEnd(); //�̵� �Է½� ĳ���� ���� ����
		}
		FVector WorldDirection = Destination - ControlledPawn->GetActorLocation(); //ĳ����, Destination ���Ͱ����� ���� ���
		WorldDirection.Z = 0;
		if(WorldDirection.Length() <= 150.f)
		{
			StopMovement();
			WorldDirection.GetUnsafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection * 50.0f); //�������� ������ �̵�, ���� ������� ����ﶧ�� ������ �̵�
		}
		else
		{
			UNavigationSystemV1* NavSys = PlayerCtr ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(PlayerCtr->GetWorld()) : nullptr;
			const FVector AgentNavLocation = PlayerCtr->GetNavAgentLocation();
			const ANavigationData* NavData = NavSys->GetNavDataForProps(PlayerCtr->GetNavAgentPropertiesRef(), AgentNavLocation);
			FPathFindingQuery Query(PlayerCtr, *NavData, AgentNavLocation, Destination);
			UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
			if (NavSystem->TestPathSync(Query)) //�׺�� �� �� ������
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(PlayerCtr, Destination); //�׺�޽� �̵�
			}
			else
			{
				WorldDirection.GetUnsafeNormal();
				ControlledPawn->AddMovementInput(WorldDirection * 50.0f); //�������� ������ �̵�, ���� ������� ����ﶧ�� ������ �̵�
			}
		}

	}

}



void AMyPlayerController::MoveToMouseCursor()
{
	FHitResult Hit;
	bool bHit = GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	if (bHit)
	{
		Server_MovePlayer(this, Hit.Location);
	}
}

void AMyPlayerController::OpenInventory()
{
	if (!UI_Iventory) // �κ��丮 UI ���� Ȯ��, ������ ����
	{
		if (UI_Inventory_Class)
		{
			UI_Iventory = CreateWidget(GetWorld(), UI_Inventory_Class);
			if (UI_Iventory)
			{
				UI_Iventory->AddToViewport();
			}
		}
	}
	else
	{
		// ���� ���� ���� �״�
		if (UI_Iventory->GetVisibility() != ESlateVisibility::Hidden)
		{
			UI_Iventory->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			UI_Iventory->SetVisibility(ESlateVisibility::Visible);
		}

	}
}

void AMyPlayerController::TestInventory()
{
	if (UI_Iventory)
	{
		UInventoryUI* InvenUI = Cast<UInventoryUI>(UI_Iventory);
		InvenUI->TestItem();
	}
}

void AMyPlayerController::MoveUnable()
{
	StopMovement();
	bMoveAble = false;
}

void AMyPlayerController::MoveAble()
{
	bMoveAble = true;
}


