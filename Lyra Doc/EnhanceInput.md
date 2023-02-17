## 增强输入系统

#### UPlayerMappableInputConfig

这代表了一组播放器可映射控制器/键映射。你可以使用这个输入配置来创建。为你的玩家创建默认的映射，以便在游戏中开始使用。它提供了一个简单的方法来获取玩家的可映射的按键动作，而且它可以用来一次为玩家添加多个UInputMappingContext的

#### UInputMappingContext

提供了键盘是、鼠标、手柄等输入硬件与Input Action的映射。

#### UInputAction

提供了Action到执行函数的映射

#### UEnhancedInputComponent

绑定到Pawn的输入组件，主要负责将UPlayerMappableInputConfig绑定到EnhancedInputSubSystem上

#### UEnhancedInputLocalPlayerSubsystem

将输入-事件的映射注册到系统

#### 总结

- PMI中存储了IMC，IMC中有操作到IA的映射，UEnhancedInputComponent会通过PMI把IMC绑定到EnhancedInputSubSystem上
- LyraHero Component中定义了一系列处理输入的函数，并在条件成熟时将这些函数与IA绑定起来，这样整个流程就通了

#### Asset Manger

#### Tag Manager

## GamePlay
