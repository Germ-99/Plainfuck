# Plainfuck
A simpler version of brainfuck, readable but still hell.

# What's different?
| Plainfuck       | What they do                        | Brainfuck |
|-----------------|------------------------------------|-----------|
| `inc N`         | Increment current cell by `N`      | `+`       |
| `dec N`         | Decrement current cell by `N`      | `-`       |
| `move_right_N`  | Move pointer right by `N`          | `>`       |
| `move_left_N`   | Move pointer left by `N`           | `<`       |
| `print`         | Print current cell as a character  | `.`       |
| `input`         | Read one character into cell       | `,`       |
| `loop_start`    | Begin loop                         | `[`       |
| `loop_end`      | End loop                           | `]`       |

# 'Hello World!' example in Plainfuck
```
inc 10
loop_start
    move_right_1
    inc 7
    move_right_1
    inc 10
    move_right_1
    inc 3
    move_right_1
    inc 1
    move_left_4
    dec 1
loop_end

move_right_1
inc 2
print

move_right_1
inc 1
print

inc 7
print
print

inc 3
print

move_right_1
inc 2
print

move_left_2
inc 15
print

move_right_1
print

inc 3
print

dec 6
print

dec 8
print

move_right_1
inc 1
print

move_right_1
print
```


# Usage
## Windows
- Download the ``plainfuck.exe`` file in the releases section
- Open your terminal in the directory that ``plainfuck.exe`` was installed in
- Run ``plainfuck.exe path\to\your\plainfuck_file.pf``

## Linux
- Download the ``plainfuck`` binary in the releases section
- Open your terminal in the directory that the ``plainfuck`` binary was installed in
- Run ``./plainfuck path/to/your/plainfuck_file.pf``
