# 🐚Mini-Shell

# 📜Descripition

The Mini Shell is a command-line interpreter developed in C on Linux that mimics the basic functionality of the Linux Bash shell. It accepts user commands, parses them, and executes internal or external programs using Linux system calls. The shell supports features like command execution, process creation, background execution, command history, environment variables, signal handling, and built-in commands.

# 🔷Features
- Interactive command prompt
  
- Execute Linux commands (`ls`, `pwd`, `cat`, `mkdir`, etc.)
  
- Built-in commands
  
  - `cd`
    
  - `pwd`
    
  - `echo`
    
  - `exit`
    
- Execute commands with arguments
  
- Background process execution using `&`
  
- Command history support
  
- Signal handling (`Ctrl+C`, `Ctrl+Z`)
  
- Environment variable support

- Error handling for invalid commands
  
- Parent-child process management using `fork()` and `wait()`


# Linux System Calls Used

- `fork()`
- `execvp()`
- `wait()`
- `waitpid()`
- `chdir()`
- `getcwd()`
- `signal()`


## ✅ Validation

| Test Case | Status |
|-----------|--------|
| Execute Linux commands | ✅ |
| Built-in commands | ✅ |
| Change directory (`cd`) | ✅ |
| Background process (`&`) | ✅ |
| Invalid command handling | ✅ |
| Signal handling | ✅ |
| Command history | ✅ |
| Exit shell | ✅ |

## 💻 Sample Output

```text
MiniShell$ pwd
/home/user

MiniShell$ ls
main.c
shell.c
parser.c
Makefile

MiniShell$ echo Hello Linux
Hello Linux

MiniShell$ sleep 5 &
[1] Background Process Started

MiniShell$ history
1 pwd
2 ls
3 echo Hello Linux
4 sleep 5 &

MiniShell$ xyz
Command not found: xyz
```

## 👩‍💻Author
Praveena
