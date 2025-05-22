# Plainfuck
A simpler version of brainfuck, readable but still hell.

# What's different?
| Plainfuck       | What they do                        | Brainfuck |
|-----------------|------------------------------------|-----------|
| `inc N`         | Increment current cell by `N`      | `+`       |
| `dec N`         | Decrement current cell by `N`      | `-`       |
| `move_right`    | Move pointer right by 1            | `>`       |
| `move_right_N`  | Move pointer right by `N`          | `>`       |
| `move_left`     | Move pointer left by 1             | `<`       |
| `move_left_N`   | Move pointer left by `N`           | `<`       |
| `print`         | Print current cell as a character  | `.`       |
| `input`         | Read one character into cell       | `,`       |
| `loop_start`    | Begin loop                         | `[`       |
| `loop_end`      | End loop                           | `]`       |


# Usage
## Windows
- Download the ``plainfuck.exe`` file in the releases section
- Open your terminal in the directory that ``plainfuck.exe`` was installed in
- Run ``plainfuck.exe path\to\your\plainfuck_file.pf``

## Linux
- Download the ``plainfuck`` binary in the releases section
- Open your terminal in the directory that the ``plainfuck`` binary was installed in
- Run ``./plainfuck path/to/your/plainfuck_file.pf``
