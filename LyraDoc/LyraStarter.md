## LyraStarter



### LyraGameMode

从WorldSetting中获取ExperienceId，并通知LyraExperienceManagerComponent开始加载Experience

### LyraExperienceManagerComponent

加载Experience，加载完成之后通知UGameFeaturesSubsystem激活ShooterCore 

### ShooterCore GFD

| Action         | Target        | Component/Config                                     |
| -------------- | ------------- | ---------------------------------------------------- |
| AddComponent   | LyraCharacter | LyraEquipmentManagerComponent                        |
| AddComponent   | Controller    | LyraInventoryStateComponent                          |
| AddComponent   | Controller    | LyraWeaponStateComponent                             |
| AddInputConfig |               | PMI_Default_KBM、IMC_Default_KBM(IA与键盘鼠标的映射) |
| AddInputConfig |               | PMI_ShooterDefaultConfig_KBM、IMC_ShooterGame_KBM    |
|                |               |                                                      |
|                |               |                                                      |

IMC_Default_KBM(IA与键盘鼠标的映射)：基础操作，比如移动、开火、装弹、跳

![image-20230222154722746](C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20230222154722746.png)

IMC_ShooterGame_KBM(IA与键盘鼠标的映射)：瞄准、手雷、近战、显示得分

![image-20230222154932088](D:\URPRJ\LearLyraGit\LearnLyra\LyraDoc\pic\image-20230222154932088.png)

### B_ShooterGame_Elimination

| Action           | Target          | Components/Config/Ability/Widgets           | Asset                              |
| ---------------- | --------------- | ------------------------------------------- | ---------------------------------- |
| AddComponent     | LyraGameState   | B_ShooterBotSpawner                         |                                    |
| AddAbility       | LyraPlayerState | AbilitySet_Elimination(GA_AutoSpawner)      | AbilitySet_Elimination             |
| AddInputMapping  |                 | IMC_ShooterGame_KBM                         | LAS_ShooterGame_SharedInput        |
| AddInputBindings |                 | InputData_ShooterGame_AddOns(Tag与IA的绑定) | LAS_ShooterGame_SharedInput        |
| AddComponent     | Controller      | B_QuickBarComponent                         | LAS_ShooterGame_StandardComponents |
| AddComponent     | B_HeroShooter   | NamePlate                                   | LAS_ShooterGame_StandardComponents |
| AddWidgets       |                 | W_ShooterHUDLayout                          | LAS_ShooterGame_StandardHUD        |
| AddWidgets       |                 | W_WeaponReticleHost                         | LAS_ShooterGame_StandardHUD        |
|                  |                 |                                             |                                    |

InputData_ShooterGame_AddOns

![image-20230222162800510](D:\URPRJ\LearLyraGit\LearnLyra\LyraDoc\pic\image-20230222162800510.png)

### OnExperienceLoaded

#### LyraPlayerState

AbilitySet_ShooterHero（GA_与Tag的映射）

**学习技能**

![image-20230222164028229](D:\URPRJ\LearLyraGit\LearnLyra\LyraDoc\pic\image-20230222164028229.png)

**IA与Tag的映射**

![image-20230222170444915](D:\URPRJ\LearLyraGit\LearnLyra\LyraDoc\pic\image-20230222170444915.png)

#### LyraGameMode

根据Experience中的PawnData记录的PawnClass生成Character

#### UAsyncAction_ExperienceReady

等到Pawn被Posess后，添加武器

#### ULyraBotCreationComponent

生成AI Pawn

------

### LyraCharacter

bPawnReadyToInitialize条件

- PawnData
- Possed
- InputComponent & not Bot

#### 初始化

1. ULyraPawnExtensionComponent::InitializeAbilitySystem

   1. 调用InitAbilityActorInfo，处理GA的回调的绑定

   1. ULyraHealthComponent处理Attribute修改时的绑定

2. ULyraHeroComponent::InitializePlayerInput

   - Tag与技能的绑定
   - Tag与普通函数的绑定

3. 相机相关的处理

------

