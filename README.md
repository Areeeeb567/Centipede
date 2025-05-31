# Centipede SFML Game

## 🛠 Requirements (One-Time Setup)

Run these commands once to set up your environment:

```bash
# Install GNU G++ Compiler
sudo apt-get install g++

# Install SFML Library
sudo apt-get install libsfml-dev

# Step 1: Compile source file
g++ -c Centipede.cpp

# Step 2: Link object file and create executable
g++ Centipede.o -o sfml-app -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

./sfml-app
