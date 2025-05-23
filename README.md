# SpaceShipGame

A C++ simulation/game where you control or observe the movement of objects in space. The simulation features real-time gravity effects, interactive controls, and visual feedback, all rendered using SFML.

## Features

- Real-time simulation of space and gravity
- Interactive player controls
- Visual information overlays and game-over screen
- Modular code structure for easy extension

## Project Structure

- `program.cpp`: Main entry point, runs the simulation loop and manages game state
- `Display/`: Rendering and display logic
- `Object/`: Classes for space objects/entities
- `Space/`: Logic for the space environment and rules
- `CMakeLists.txt`, `CMakeSettings.json`: CMake build configuration files
- `begok.ttf`: Font used for displaying information

## Requirements

- C++17 or later
- [SFML library](https://www.sfml-dev.org/) (Simple and Fast Multimedia Library)
- CMake

## Building

```bash
git clone https://github.com/Mzimec/SpaceShipGame.git
cd SpaceShipGame
mkdir build
cd build
cmake ..
make
```

## Running
```bash
./SpaceShipGame
```

## Controls

> The controls below are typical for SFML-based games. Please adjust based on your actual implementation if needed.

- **Arrow UP / W**: Thrust ship toward the cursor
- **P**: Speeding up the flow of time
- **O**: Slowing down the flow of time
- **Spacebar**: Pausing the time, thrusting is still possible.
- **K**: Set the speed of the ship to circle clockwise around the closest object
- **J**: Set the speed of the ship to circle counter-clockwise around the closest object
- **L**: Loc or unlock camera from the ship view.

## How It Works

- The game initializes a simulated space and its objects.
- The main loop processes user input, updates the simulation, and renders the visuals.
- The simulation continues until a game-over condition is met, at which point a summary is displayed.

## Contributing

Feel free to open issues or submit pull requests for improvements or bug fixes.

## License

MIT.
