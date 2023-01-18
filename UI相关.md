## UI相关

```mermaid
classDiagram
class UUserWidget
class UCommonUserWidget

class ULyraWeaponUserInterface{
	+ void OnWeaponChanged(OldWeapon, NewWeapon);
	+ ULyraWeaponInstance* CurrentInstance 
}

class ULyraReticleWidgetBase{
	+ void OnWeaponInitialized()
	+ ULyraWeaponInstance* WeaponInstance
	+ ULyraInventoryItemInstance* InventoryInstance
}
UUserWidget<|--UCommonUserWidget
UCommonUserWidget<|--ULyraWeaponUserInterface
UCommonUserWidget<|--ULyraReticleWidgetBase

class W_Reticle_Pistol 
class W_WeaponReticleHost
ULyraWeaponUserInterface<|--W_WeaponReticleHost
ULyraReticleWidgetBase<|--W_Reticle_Pistol 
```

装备武器UI流程

```c++
void ULyraWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    // 找到Pawn身上的ULyraEquipmentManagerComponent获取当前WeaponInstance
    W_WeaponReticleHost::OnWeaponChanged(OldWeapon, CurrentInstance);
    {
        W_WeaponReticleHost::ClearExistingWidgets();
        AddChildOvelay
    }
}
```

界面UI添加流程

```c++

```

