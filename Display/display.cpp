#include "display.h"

Input_manager::Input_manager(sf::RenderWindow& window, Space& space) : _window(window), _space(space) {}

// function that resolves events
void Input_manager::resolve_events() {
	sf::Event ev;
	while (_window.pollEvent(ev)) {
		switch (ev.type) {
		case sf::Event::Closed: // closing on close
			_window.close();
			break;
		case sf::Event::MouseWheelMoved: // zooming in and out on space
			if (ev.mouseWheel.delta < 0) _space._zoom_coef *= pow(zoom_up, abs(ev.mouseWheel.delta));
			else _space._zoom_coef *= pow(zoom_down, ev.mouseWheel.delta);
			break;
		case sf::Event::KeyReleased: // calling special key function on any key released
			resolve_key_pressed(ev);
			break;
		default: // default other events ignore
			break;
		}
	}
}

// manages key released events
void Input_manager::resolve_key_pressed(const sf::Event& ev) {
	switch (ev.key.scancode) {
	case sf::Keyboard::Scan::P: // speeding up the time
		_space._time_speed_coef *= zoom_up;
		break;
	case sf::Keyboard::Scan::O: // slowing down the time
		_space._time_speed_coef *= zoom_down;
		break;
	case sf::Keyboard::Scan::W: // boosting ship
		if (!_space._is_stopped) _space._player->boost_ship(_window, km_per_px * _space._zoom_coef, _space._window_center);
		break;
	case sf::Keyboard::Scan::Space: // pausing or unpausing the simulation
		_space._is_stopped = !_space._is_stopped;
		break;
	case sf::Keyboard::Scan::K: // set clockwise cosmic speed to the Objects that creates the highest gravity force on player
		_space.set_player_cosmic_speed();
		break;
	case sf::Keyboard::Scan::J: // set counterclockwise cosmic speed to the Objects that creates the highest gravity force on player
		_space.set_player_cosmic_speed();
		_space._player->speed = { -_space._player->speed.first, -_space._player->speed.second };
		break;
	case sf::Keyboard::Scan::L: // lock or unlock cam on player
		_space._cam_is_locked_player = !_space._cam_is_locked_player;
		break;
	default:
		break;
	}
}

Info_displayer::Info_displayer(const Space& space, sf::RenderWindow& window) : _space(space), _window(window) {
	// loading font file
	_font.loadFromFile("../begok.ttf");
}

// drawing the canvas background on correct position
void Info_displayer::draw_canvas() {
	sf::RectangleShape canvas(sf::Vector2f(info_width, fullscreen_mode.height));
	canvas.setPosition(fullscreen_mode.width - info_width, 0);
	canvas.setFillColor(sf::Color::Black);
	canvas.setOutlineColor(sf::Color::White);
	canvas.setOutlineThickness(4);
	_window.draw(canvas);
}

// writes speed attribute of player on canvas
void Info_displayer::write_speed_x() {
	sf::Text text("Speed X: " + std::to_string(_space._player->speed.first), _font, 20);
	text.setPosition(fullscreen_mode.width - info_width + pacing, 3*pacing);
	text.setFillColor(sf::Color::White);
	_window.draw(text);
}

// writes speed attribute of player on canvas
void Info_displayer::write_speed_y() {
	sf::Text text("Speed Y: " + std::to_string(_space._player->speed.second), _font, 20);
	text.setPosition(fullscreen_mode.width - info_width + pacing, 5*pacing);
	text.setFillColor(sf::Color::White);
	_window.draw(text);
}

// writes important player data on canvas 
void Info_displayer::write_player_data() {
	sf::Text text("Player data: ", _font, 20);
	text.setPosition(fullscreen_mode.width - info_width + pacing, pacing);
	text.setFillColor(sf::Color::White);
	_window.draw(text);
	write_speed_x();
	write_speed_y();
}

// write time speed coeficient value on canvas
void Info_displayer::write_time() {
	sf::Text text("Time coeficient: \n\t" + std::to_string(_space._time_speed_coef), _font, 20);
	text.setPosition(fullscreen_mode.width - info_width + pacing, 10 * pacing);
	text.setFillColor(sf::Color::White);
	_window.draw(text);
}

// write zoom coeficient value on canvas
void Info_displayer::write_zoom() {
	sf::Text text("Zoom coeficient: \n\t" + std::to_string(_space._zoom_coef), _font, 20);
	text.setPosition(fullscreen_mode.width - info_width + pacing, 13 * pacing);
	text.setFillColor(sf::Color::White);
	_window.draw(text);
}

// writes important space data on canvas
void Info_displayer::write_space_data() {
	sf::Text text("Space data: ", _font, 20);
	text.setPosition(fullscreen_mode.width - info_width + pacing, 8 * pacing);
	text.setFillColor(sf::Color::White);
	_window.draw(text);
	write_time();
	write_zoom();
}

// write all info on canvas
void Info_displayer::write_info() {
	draw_canvas();
	write_player_data();
	write_space_data();
}

// Write Game Over when player crashes
void Info_displayer::write_end() {
	sf::Text text("GAME OVER\n\nYou crashed", _font, 40);
	float xPos = (_window.getSize().x - text.getLocalBounds().width) / 2;
	float yPos = (_window.getSize().y - text.getLocalBounds().height) / 2;
	text.setPosition(xPos, yPos);
	text.setFillColor(sf::Color::White);
	_window.draw(text);
}

