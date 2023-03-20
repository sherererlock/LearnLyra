#### Ability System Component

- PlayerState的成员变量（角色可复活）

- PlayerState的构造函数中创建

- PlayerState实现GetAbilitySystemComponent

- 初始化(InitAbilityActorInfo)

  - OwnerActor和PhysicsActor

- BindAbilityActivationToInputComponent

  与输入相关的绑定

#### GameAbility

- 需要调用ASC->GiveAbility接口才能其效果
- 在FGameplayAbilitySpec设置输入映射，ASC->GiveAbility映射其效果
- 有消耗的技能需要CommitAbility才能消耗属性
- 加速使用蓝图实现
- 跳使用c++实现

#### GameAttribute

- 由HealthAttributeSet组织
- 由ASC管理，可以设置修改时的回调函数
- 由GE初始化
- 由GE修改

#### GameEffect

- DefaultEffect

  设置所有基础属性，即时生效，修改BaseValue

- GE_Regen

  修改基础属性,永久有效

- GE_Cost

  技能消耗，即时生效

#### UI相关

蓝图制作

主要是血量的显示。

- 主角属性显示UI
- 头顶属性显示UI

初始化：

AGLPlayerController::CreateHUD时从PS中获取各种属性然后设置给UI

变化时：

通过AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate绑定了回调函数，在回调函数中设置UI显示



