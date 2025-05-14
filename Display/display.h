#ifndef DISPLAY_H
#define DISPLAY_H

#include "../space/space.h"
/*
	It takes care of most of the palyer runtime input
*/
class Input_manager {
private:
	// window on which it operates
	sf::RenderWindow& _window;
	// space on which it operates
	Space& _space;

	// function that takes care of different Keys presses
	void resolve_key_pressed(const sf::Event& ev);

public:
	Input_manager(sf::RenderWindow& window, Space& space);
	// public function that could be callled from anywhere to resolve all the Events
	void resolve_events();
};

/*
	Class that takes care of displaying information on the window for player
*/
class Info_displayer {
private:
	// space on which it operates
	const Space& _space;
	// window on which it operates
	sf::RenderWindow& _window;
	// font of the written text
	sf::Font _font;

	//function that creates background of canvas
	void draw_canvas();
	
	//function for writing necessary data about player on the canvas
	void write_player_data();
	void write_speed_x();
	void write_speed_y();

	//function for writing necessary space about player on the canvas
	void write_space_data();
	void write_zoom();
	void write_time();

public:
	Info_displayer(const Space& space, sf::RenderWindow&);
	// public function to write all the important data on the canvas
	void write_info();
	// public function to announce player that he crashed
	void write_end();
};

#endif // DISPLAY_H
