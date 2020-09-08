# 解题思路
难点在于第三个任务, 响应Button按钮事件,以及快捷键Z和M 自动生成随机数
大致步骤如下:
- VC++开发DialogBoxParamA的回调函数:处理按钮消息以及Z/M键
- 回调函数开发完成后封装到rvs2dll.dll中
- 反汇编修改PE文件, 加载rvs2dll.dll,同时将回调函数地址设置为新开发的回调函数

**回调函数的难点在于如何响应按键Z和M? **
因为模态对话框回调函数无法获取到键盘消息.

**思考之后有两种思路可以解决:**
## 1.设置钩子获取当前进程键盘消息
窗口回调函数中, 处理WM_INITDIALOG时
SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_instance, GetCurrentThreadId());
然后在KeyboardProc中就可以处理键盘消息

## 2.设置定时器定时读取键盘状态
窗口回调函数中, 处理WM_INITDIALOG时调用:SetTimer,
每隔100ms产生WM_TIMER消息.
在窗口回调函数中处理VM_TIMER消息:
用GetAsyncKeyState分别读取Z/M键的状态.

经验证,两种思路都可以很好响应键盘事件.本例中采用的是设置钩子的方法.

# 实现步骤

## 获取关键数据
  button和edit的控件ID   
  回调函数中需要用控件ID来处理VM_COMMAND消息.  
  用Visual Studio自带的spy++工具可以读取到Windows的控件ID  
  ```
  Button1 -> 1005 (3EDh)
  Button2 -> 1006 (3EEh)
  Edit1   -> 1008 (3F0h)
  Edit2	  -> 1007 (3EFh)
  ```
  
## VC++代码实现
详见:https://github.com/maluodi/ReverseMeRepo/tree/master/Rvs2Project
其中Rvs2Dll是窗口回调函数项目.其中窗口回调函数名是**MyWndProc**.  
所以生成了了Rvs2dll.dll后, 只需要把**MyWndProc**设置为窗口的回调函数即可. 

我想如果要用汇编完成一项功能,不妨先尝试用C/C++语言实现完整功能.  
如果VC++都实现不了,反汇编就更谈不上了.  
所以为了验证窗口回调函数,Rvs2Project中创建了个和reme2.exe一样的窗口.  

## 反汇编定义字符串变量
  在reme2.exe文件.data区块中定义了两个字符串变量
  ```
  004031A0  52 76 73 32 44 6C 6C 2E 64 6C 6C 00 00 00 00 00  Rvs2Dll.dll.....  
  004031B0  5F 4D 79 57 6E 64 50 72 6F 63 40 31 36 00 00 00  _MyWndProc@16...  
  ```
  
  之前定义函数是**MyWndProc**,为什么这里的函数名称是**_MyWndProc@16**?  
  这是因为窗口回调函数是stdcall标准,VC++在生成dll时修改了导出函数名.  
  
  所以在通过LoadLibrary/GetProcAddress来调用dll导出函数的时候,不能想当然用自己定义的名称.  
  可以用工具(LordPE/CFF Exploer之类)查看一下dll中的导出表信息,看下真实导出函数的名称.  

## 反汇编修改PE文件
   ```
    0x401000:
    push 0x004031A0       ;Rvs2Dll.dll
	call 0x0040116C       ;<JMP.&LoadLibraryA>
	push 0x004031B0       ;_MyWndProc@16
	push eax              ;dllModule也就是dll的虚拟内存中的BaseAddress
	call 0x00401166       ;<JMP.&GetProcAddress>
	mov dword ptr ds:[4031C0],eax ;回调函数地址存起来放到.data区块
	push 0
	call 0x00401160               ;<JMP.&GetModuleHandleA>
	mov dword ptr ds:[4030A0],eax
	push 0
	push dword ptr ds:[4031C0]    ;设置回调函数地址
	push 0
	push 0x00403005               ;窗口资源ID
	push dword ptr ds:[4030A0]
	call 0x00401142  ;<JMP.&DialogBoxParamA>
	jmp 0x004010B6
   ```

# 实践心得

- spy++工具可以查看到Windows的控件ID

- dll导出表中函数名和实际函数名不一致
  动态加载时先用工具查看导出函数名称

- 想要用汇编完成一项功能,先尝试用C/C++语言实现

- 动态库和静态库的区别:
-- 静态库是程序在链接的时候就将代码拷贝到exe文件中去了.exe文件可以脱离lib文件运行.  
-- 动态库是程序在运行过程中加载的.exe文件无法离开dll文件单独运行  
-- 动态库中也有lib文件,但是lib文件只有导出的函数声明.  

- X64Dbg调试技巧:Shift+C/Shift+V
  可以复制粘贴汇编代码
  
- 反汇编时注意stdcall调用还是cdecl调用
C/C++的函数传递和WindowsApi 参数传递标准不一样.  
这一点在用汇编调用函数的时候要非常注意维护栈平衡.  
否则会导致程序崩溃.出现各种非预期的结果.  
