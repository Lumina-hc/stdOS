# stdOS

32位保护模式操作系统，支持 FAT16 文件系统、IDE PIO 磁盘读写、键盘。

依赖：NASM, i686-elf-gcc, QEMU。运行 `make.bat`。

| 命令 | 说明 |
|------|------|
| `help` | 显示帮助 |
| `clear` | 清屏 |
| `about` | 版本信息 |
| `ls` | 列出目录 |
| `cat <文件>` | 读取文件 |
| `cd <目录>` | 切换目录 |
| `mkdir <目录>` | 创建目录 |
| `rm <文件>` | 删除文件 |
| `write <文件> <内容>` | 写入文件 |
| `echo <文本>` | 打印文本 |
| `hlt` | 停止 CPU |

---

32-bit protected mode OS with FAT16 filesystem, IDE PIO disk I/O, keyboard.

Requires: NASM, i686-elf-gcc, QEMU. Run `make.bat`.

| Command | Description |
|---------|-------------|
| `help` | Show help |
| `clear` | Clear screen |
| `about` | Show version |
| `ls` | List directory |
| `cat <file>` | Read file |
| `cd <dir>` | Change directory |
| `mkdir <dir>` | Create directory |
| `rm <file>` | Delete file |
| `write <file> <data>` | Write file |
| `echo <text>` | Print text |
| `hlt` | Halt CPU |
