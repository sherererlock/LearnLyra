无法解析的外部链接：

1. 可能是模块没有加进来，需要在Build.CS中加入相应类所在的模块名

无法打开包含文件

1. 需要在Build.CS中加入路径名

   ```c#
   		PublicIncludePaths.AddRange(
   			new string[] {
   				"LearnLyra"
   			}
   		);
   ```


跳跃技能不触发

- 根据SpecHandle找不到AbilitySpec
  - 引擎是Release的，一部分断点信息不可信

跳跃技能没有调用EndAbility导致只能触发一次

- WaitInputRelease节点的OnRelease方法没有调用
  - 需要重写AbilitySpecInputReleased，以便在其中保证WaitInputRelease得到调用

子类的对象没法被找到，设置不到变量上去

- 没有继承为子类对象，而是创建了DataAsset
- 无法找到Plugins中的对象

Experience数据文件不加载

- 需要添加到AssetManager的PrimaryAssetType中

在WeaponSpawner中定义了InventoryItemDefinition之后，设置时找不到对象

- 有时候需要定义Class Reference
- 重点时区分class Reference和objectReference的区别

冲刺与表情技能无法释放

- 只能引用ProjectContent目录下的资源，不能直接引用Plugins下的资源
- 