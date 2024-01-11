Language:[English](README.md)|简体中文

![logo](assets/logo.gif)
## 以撒MOD的新时代来临了。

REPENTOGON是一个基于《以撒的结合：忏悔》版本v1.7.9b.J835(未经测试，可能在其他版本上不受支持)的MOD，它扩展了Lua API，提供了急需的bug修复、额外功能和性能增强。

REPENTOGON通常被社区称为“代码扩展MOD”，它的工作方式与传统的mod非常不同。由 *LibZHL* 作为技术支持，与 *[Antibirth](https://antibirth.com/)* 使用相同的框架，REPENTOGON直接连接到游戏中，允许直接控制以前不可能在mod中复现的功能，或者需要显著的性能密集型侵入或重写才能实现。

# LUA API文档
REPENTOGON对Lua API进行了添加和修改，并添加了大量新功能。你可以在这里查看文档:[https://repentogon.com/docs.html](https://repentogon.com/docs.html)

# 安装
1. 从[GitHub Actions](https://github.com/TeamREPENTOGON/REPENTOGON/actions/workflows/ci.yml)下载构建。
2. 将压缩文件中的内容解压至你的游戏目录。你可以通过Steam找到目录，在Steam库中右击`The Binding of Isaac: Rebirth`，点击`属性`，然后在弹出的窗口中点击`已安装文件`、`浏览`（位于右上角）。
   * 如果你通过Proton在Linux中游玩, 你需要点击`属性`，在`通用`中的`启动选项`中输入`WINEDLLOVERRIDES="dsound=n,b" %command%`。REPENTOGON无法在缺少启动选项的情况下启动。
3. 启动游戏。如果REPENTOGON成功加载，你能够在游戏目录中看到`zhl.log`和`repentogon.log`，并且标题栏应显示`Binding of Isaac: Repentance (+ REPENTOGON vX.X.X)`。

# 卸载
技术上来说，在启动选项中输入`-repentogonoff`或删除`dsound.dll`即可禁用REPENTOGON，如果想要完全卸载：
1. 从游戏目录中删除`dsound.dll`、`freetype.dll`、`libzhl.dll`、`Lua5.4.dll`、`resources-repentogon`和`zhlREPENTOGON.dll`.
2. 可选，删除日志文件`dsound.log`、`repentogon.log`和`zhl.log`。
3. 删除`resources\rooms`（其中的`26.The Void_ex.stb`）和`resources\shaders` （其中的`coloroffset_gold_mesafix`）。
4. 打开`resources\scripts`文件夹，删除`main_ex.lua`和`enums_ex.lua`。
5. 在`User\Documents\My Games\Binding of Isaac Repentance`（或者是`savedatapath.txt`中的存档路径）文件夹中删除`Repentogon`文件夹。注意这会完全清除所有自定义的成就和标记！

# 构建
（除非您是开发者，我们推荐您从[GitHub Actions](https://github.com/TeamREPENTOGON/REPENTOGON/actions/workflows/ci.yml)获取构建。）
### 需求
这个项目的性质要求我们使用与游戏相同的编译器。因此，明确需求Windows系统，以及以下内容:
* CMake 3.13 或更高
* Git
* Visual Studio 2019 或更高

### 步骤
我们假设在本教程期间使用Git Bash以及CMake GUI。
1. *逐步*克隆仓库：`git clone --recursive https://github.com/TeamREPENTOGON/REPENTOGON`
2. 启动CMake。
3. 在"Where is the source code"中将文件定位为克隆仓库的根目录（通常命名为REPENTOGON）。
4. 在"Where to put the binaries"中选择任意文件夹。这里是生成文件的目录。
![cmake1](assets/cmake1.png)
1. 在CMake GUI的底部，点击"Configure"。
    * 如果这是第一次构建，将提示您输入更多信息。 
    * 编译器必须与您的Visual Studio版本匹配。
    * platform**必须**为Win32。
    * 其他选项保持默认，然后点击Finish。
    ![cmake2](assets/cmake2.png)
2. 配置完成后点击“Generate”。会在您上文选定的文件夹中生成.sln文件。
3. 在Visual Studio中打开生成的.sln文件。
4. 构建项目。除非您是开发者，否则我们推荐您使用Release模式以获得更好的性能。
5. 构建完成后将`resources`、`resources-repentogon`、`dsound.dll`、`freetype.dll`、`libzhl.dll`、`Lua5.4.dll`和`zhlREPENTOGON.dll`复制至游戏目录。
  * 可选，将`ISAAC_DIRECTORY`设置为游戏根目录以在构建结束后自动复制文件。

# 协议
REPENTOGON使用GNU通用公共许可证第2版。

LibZHL使用MIT许可协议。因此，`libzhl`和`libzhlgen`文件夹中的所有内容都是MIT授权的，但以下文件和文件夹除外，它们是REPENTOGON的组件:
* `libzhl/functions`与其中的内容
* `libzhl/IsaacRepentance_static.cpp`

`libs`文件夹中的文件夹是*外部依赖项*，并且有自己的许可信息。检查这些文件夹(或者，在子模块的情况下，检查它们各自的依赖项)以获取更多信息。

# 隐私保护声明
REPENTOGON不会收集或发送任何未经明确同意的用户信息。我们拥有可选的更新机制，该机制将在启动时将用户的IP地址传输到GitHub以确定是否有可用的新版本。但除此之外，我们不会处理或存储任何数据。