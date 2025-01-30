# Pac-Man for LandTiger Board

## Project Overview
This project implements a Pac-Man game on the LandTiger development board, featuring an ARM Cortex-M3 processor. The implementation follows the original Pac-Man game mechanics while incorporating specific requirements for the LandTiger board, including joystick controls, game logic, AI-controlled ghosts, sound effects, and CAN bus communication.

## Requirements
### Extrapoint 1 - Basic Pac-Man Game
1. **Labyrinth and Pills:**
   - Display a maze on the LandTiger screen with exactly **240 Standard Pills**.
   - Maintain a **central box**, which may be used later.
   - Generate **6 Power Pills** in random positions at random times.

2. **Game Mechanics:**
   - Pac-Man moves continuously in the last chosen direction until a wall or new input.
   - Joystick controls allow direction changes (left, right, up, down).
   - Speed should be playable and user-friendly.
   - **Teleportation:** When Pac-Man reaches a teleport point, he reappears on the opposite side.
   - **Scoring System:**
     - Standard Pills: **+10 points**
     - Power Pills: **+50 points**
     - Every **1000 points**, Pac-Man earns an extra life.
   - **Pause Feature:** Pressing **INT0** pauses and resumes the game.
   - **Countdown Timer:** Starts from **60 seconds**; game ends when it reaches 0.
   - **Win Condition:** Pac-Man eats all the pills.
   - **Lose Condition:** Countdown reaches zero.

### Extrapoint 2 - Advanced Features
1. **AI-Controlled Ghost (Blinky)**
   - Blinky actively chases Pac-Man.
   - Two movement strategies:
     - **Chase Mode:** Blinky follows Pac-Man using an AI algorithm (e.g., A* pathfinding).
     - **Frightened Mode:** Activates when Pac-Man eats a Power Pill:
       - Blinky turns blue for **10 seconds** and runs away.
       - If Pac-Man eats him, **+100 points**.
       - Blinky respawns in the central box after **3 seconds**.
   
2. **Audio Integration**
   - Configure the speaker to play **sound effects** and **background music**.

3. **CAN Bus Communication**
   - Send **score, remaining lives, and countdown timer** through the CAN bus.
   - Configure **CAN1 (sender)** and **CAN2 (receiver)** for external loopback mode.

## Development Environment
- **Platform:** LandTiger Board with ARM Cortex-M3
- **IDE:** Keil µVision
- **Compilation Target:** SW_Debug (for emulator) / Physical Board (for advanced features)

## How to Build and Run
1. **Compile the code** in Keil µVision.
2. **Flash the program** into the LandTiger board.
3. **Use the joystick** to control Pac-Man.
4. **Monitor game status** on the display and CAN bus.
5. **Listen to sound effects** from the speaker.

## Contributors
- **Author:** Simone Candido

## License
This project is released under the [MIT License](LICENSE).

