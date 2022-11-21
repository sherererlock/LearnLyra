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

   