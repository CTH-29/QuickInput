# QuickInput

## 简介

将常用的文本存入到按钮里，然后点击按钮，复制到剪切板

可以将常用的指令和需要的信息保存下来，通过点击，然后Ctrl+V实现快速输入

这些信息都在保存在配置文件QuickInput.ini里，你可以自行修改

不建议保存密码，因为这些信息没有加密

## 环境

系统 `Windows10`

软件 `QT 5.12.12`

编译器 `MinGW 7.3.0 64-bit`


## 使用

下载 release 压缩包，解压之后就可以使用了，因为包含了QT运行时的DLL，所以体积会大一下

<https://github.com/CTH-29/QuickInput/releases/>

通过菜单栏 `Settings` 下的 `Show/Hide Edit` 可以显示/隐藏用来编辑的文本框

通过菜单栏 `Settings` 下的 `Always on Top` 可以将当前窗口置顶（始终显示在最上层）或者取消置顶


通过菜单栏 `Edit` 下的 `Add Tab` 或者 `Ctrl+T` 可以新增一个tab页面

通过菜单栏 `Edit` 下的 `Add Button` 或者 `Ctrl+B` 可以新增一个按钮

通过菜单栏 `Edit` 下的 `Save Button` 或者 `Ctrl+S` 可以将编辑框里的文本保存在按钮里

## 许可

MIT
