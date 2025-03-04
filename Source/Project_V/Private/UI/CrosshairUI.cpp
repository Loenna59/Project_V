// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CrosshairUI.h"

#include "Components/Image.h"

void UCrosshairUI::UpdateCircle(float strength)
{

	float value = FMath::Lerp(1, 0.4f, strength * 0.01f);
	Circle->SetRenderScale(FVector2D::One() * value);
}
