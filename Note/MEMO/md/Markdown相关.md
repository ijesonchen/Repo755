
---
# 简介
- 入门教程
    [认识与入门 Markdown](http://note.youdao.com/)
    
- 优点
1. 专注你的文字内容而不是排版样式。
1. 轻松的导出 HTML、PDF 和本身的 .md 文件。
1. 纯文本内容，兼容所有的文本编辑器与字处理软件。
1. 可读，直观。适合所有人的写作语言。

- 语法扩展
    支持语法扩展，如github扩展的语法包括：
1. 代码块 
> ```
> the code
> ```
2. 删除线 
> ~~to delete~~
3. 任务列表 
> - [ ] not complete
> - [x] completed

- 关于有序列表
    有序列表会自动转换为一个顺序序列，md文件中序列不一定要有序，但建议从1开始，因为将来语法可能限定为非1开始的序列。

---
# 工具
- **Haroopad** 
    推荐。来自韩国，跨平台，支持多种扩展（含github），支持离线编辑预览。但是vmware下的win7中文字符无法显示，需要修改css文件（偏好设置菜单仍然不显示中文，未找到解决方案）。
- MarkdownPad
    据说是windows下最好用的工具，但是似乎不支持github语法扩展
- Cmd Markdown
    虽然有windows版程序，但是是基于在线方式的，离线下载无法使用。
- QMarkdowner
    基于pyQt，发布的程序需要360云盘，所以未测试
- MarkPad
    微软app方式下载，下载失败
- MultiMarkdown
    二进制程序被墙，无法下载


---
# Haroopad
- 中文无法显示的问题

    [Haroopad中文显示问题](http://http://blog.csdn.net/hustd10/article/details/50533954)
安装Haroopad之后发现编辑器中输入的中文字体看不到，而右边的预览框中显示正常。然后把字体调小之后又可以正常显示。 
在“文件”下打开“偏好设置”，选择编辑器，找到默认主题，点击编辑，如图：
![Haroopad中文显示问题](Haroopad中文显示问题.jpg)
可以看到default.css文件，修改成如下，字体根据自己喜欢修改。重新启动即可。
例如：
editor {
  font-family: "微软雅黑", "Meiryo UI", "Malgun Gothic", "Segoe UI", "Trebuchet MS", Helvetica, sans-serif !important;
}

- 常用快捷键

    帮助中可以看到所有快捷键列表
    常用的包括：
    
Shortcut                    | Description
----------------------------|-----------------------
SHIFT - CTRL - 1            | (editor : viewer) mode
SHIFT - CTRL - 2            | (viewer : editor) mode
SHIFT - CTRL - 3            | editor mode
SHIFT - CTRL - 4            | viewer mode
SHIFT - CTRL - ]            | editor width +5%
SHIFT - CTRL - [            | editor width -5%
SHIFT - CTRL - G            | toggle line number
SHIFT - CTRL - H            | toggle markdown helper
ALT - UP                    | editor font size +1px
ALT - DOWN                  | editor font size -1px
SHIFT - ALT - UP            | viewer font size +1px
SHIFT - ALT - DOWN          | viewer font size -1px
CTRL - Q                    | Folding (quit?)
SHIFT - CTRL - D            | Date & Time

Shortcut                    | Example            | Description
----------------------------|:------------------:|-------------------
COMMAND (CTRL) - 1          | #                  | Header 1
COMMAND (CTRL) - 2          | ##                 | Header 2
COMMAND (CTRL) - 3          | ###                | Header 3
COMMAND (CTRL) - 4          | ####               | Header 4
COMMAND (CTRL) - 5          | #####              | Header 5
COMMAND (CTRL) - 6          | ######             | Header 6
COMMAND (CTRL) - B          | \*\*Bold\*\*       | bold
COMMAND (CTRL) - I          | \*Italic\*         | italic
COMMAND (CTRL) - L          | \[link\](url)      | link
SHIFT - CTRL - C            | - [ ] task         | task list
SHIFT - CTRL - O            | 1. markdown        | ordered list
SHIFT - CTRL - L            | * haroopad         | unordered list
