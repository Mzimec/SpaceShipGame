
#include "display/display.h"

#include <iostream>
#include <chrono>

/*
	Main function just initiate every class managing the simulation
	and using the while cycle simulate the space movement
*/
int main(int argc, char** argv) {
	Space space;
	sf::RenderWindow window(fullscreen_mode, "GravitySimulation");
	Input_manager input_manager(window, space);
	Info_displayer info_canvas(space, window);
	space.initialize_space_objects();
	auto last_time = std::chrono::high_resolution_clock::now();
	/*
		In the while cycle is straight forward.We just calculate how the cosmos chcnges through the time of the one loop computation.
		Then it draws the space on the window
	*/
	while (window.isOpen()) {
		auto now = std::chrono::high_resolution_clock::now();
		double dt = std::chrono::duration_cast<std::chrono::microseconds>(now - last_time).count();
		last_time = now;
		input_manager.resolve_events();
		window.clear();
		// If statement that controls if the player is still alive and the simulation should continue.
		if (!space._is_game_over) {
			space.simulate(dt, window);
			info_canvas.write_info();
		}
		else info_canvas.write_end();
		window.display();
	}
}

