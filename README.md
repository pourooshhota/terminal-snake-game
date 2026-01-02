# 🐍 Terminal Snake Game in C (ncurses)

A classic **Snake game implemented in C** using the **ncurses** library, designed to run entirely in the terminal.


> Built and tested on **Ubuntu (WSL)**.

---

## 🎮 Features

- ⌨️ Smooth keyboard controls (arrow keys)
- 🎨 Colored terminal UI (snake, food, border)
- ⏸️ Pause & resume gameplay
- 🔄 Restart game without exiting
- 🌍 Wall wrap mode (snake passes through edges)
- 📈 Progressive speed increase
- 💾 Persistent high score storage

---

## 🕹️ Controls

| Key | Action |
|---|---|
| ↑ ↓ ← → | Move snake |
| **P** | Pause / Resume |
| **R** | Restart game |
| **Q** | Quit game |

---

## 🧱 Requirements

### Linux / WSL
- GCC compiler
- ncurses library

Install dependencies on Ubuntu:
```bash
sudo apt update
sudo apt install build-essential libncurses5-dev
