# SCGUI Windows 在线编译方案（GitHub Actions）

## 原理

把代码推到 GitHub → GitHub 服务器自动用 MinGW 编译 → 你下载编译好的 `scgui_win.exe`。

**全程网页操作，不需要本地安装任何编译器。**

---

## 一、首次操作（一次性）

### 1. 注册 GitHub 账号
https://github.com/signup

### 2. 新建一个仓库
- 名字随便，比如 `scgui_win`
- 选择 **Public**（私有仓库 Actions 分钟数有限）
- 勾选 **Add a README file**

### 3. 上传代码到仓库

**方法A（网页上传，最简单）：**
1. 打开仓库页面 → 点 **Add file** → **Upload files**
2. 把 `scgui_windows/` 目录里的所有文件拖进去
3. Commit message 写 `init: add scgui windows project`
4. 点 **Commit changes**

**方法B（本地 git push，推荐）：**
```bash
# 在你电脑上（有 git 就行，不需要编译器）
cd C:\Users\Administrator\.qclaw\workspace-agent-822fc5af\scgui_windows
git init
git add .
git commit -m "init: add scgui windows project"
git remote add origin https://github.com/你的用户名/scgui_win.git
git push -u origin main
```

### 4. 启用 Actions
- 进入仓库 → 点顶部 **Actions** 标签
- 如果看到黄色的「Enable Actions」按钮，点一下启用

---

## 二、自动编译

上传代码后：
1. 进入仓库 → **Actions** 标签
2. 你会看到一个名为 **Build SCGUI Windows Simulator** 的工作流正在运行（黄色圆点）
3. 点进去 → 左侧看到运行日志，等 2-3 分钟
4. 出现绿色 ✓ 表示编译成功

---

## 三、下载编译好的 exe

1. 进入仓库 → **Actions** → 点最新的绿色工作流程
2. 页面底部 **Artifacts** 区域会出现 `scgui_win.exe`
3. 点击下载
4. 解压，双击 `scgui_win.exe` 运行

---

## 四、修改代码后重新编译

每次你 **push 新代码**（修改 `.c` / `.h` 文件），Actions 会自动重新编译。

网页操作：
1. 打开要改的文件（比如 `SCGUI/sc_common.h`）
2. 点右上角 ✏️ 编辑
3. 改完拉到下面 → **Commit changes**
4. 等 2-3 分钟 → 重新下载 Artifact

---

## 五、编译失败的排错

如果 Actions 页面出现红色 ✗：
1. 点进工作流程 → 展开 **Build with gcc (MinGW)** 步骤
2. 看红色的错误日志
3. 把错误信息复制给我，我帮你修

---

## 六、如果不用 GitHub

### 方案2：GitLab CI（同理）
1. 注册 gitlab.com
2. 上传代码 + `.gitlab-ci.yml`（我帮你写）
3. CI/CD → Jobs → 下载 Artifact

### 方案3：WSL 编译（你本地有 Windows 10/11）
```powershell
# 管理员 PowerShell 运行
wsl --install
# 安装完后打开 Ubuntu，运行：
sudo apt update && sudo apt install mingw-w64 -y
# 然后编译：
x86_64-w64-mingw32-gcc main.c sdl_lcd.c ... -o scgui_win.exe
```

---

## 七、当前工程文件清单

确保你上传到 GitHub 的文件包括：

```
scgui_windows/
├── .github/workflows/build.yml   ← 自动编译配置（已写好）
├── main.c
├── sdl_lcd.c
├── sdl_lcd.h
├── sc_demo_test.c
├── sc_demo_test.h
├── SCGUI/
│   ├── sc_common.c / .h
│   ├── sc_gui.c / .h
│   ├── sc_arc.c / .h
│   ├── sc_compose.c / .h
│   ├── sc_event_task.c / .h
│   ├── sc_keyboard.c / .h
│   ├── sc_lvgl_font.c / .h
│   ├── sc_menu.c / .h
│   ├── sc_transform.c / .h
│   └── lvgl.h
├── build_mingw.bat
├── build_msvc.bat
└── README.md / README_zh.md
```

---

## 八、下一步

1. 按「一、首次操作」把代码传上去
2. 等 Actions 跑完（约 3 分钟）
3. 下载 `scgui_win.exe`，双击运行
4. 截图发我，我帮你确认显示是否正确
5. 然后我们继续调 UI，准备移植到 CH582 + ST7789
