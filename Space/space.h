#ifndef SPACE_H
#define SPACE_H

#include "../object/object.h"

#include <cmath>
#include <unordered_map>
#include <string>
#include <algorithm>

using VectorPairs = std::vector<std::pair<std::string, std::string>>; //vector of pairs
using Iterator = std::vector<std::vector<std::string>>::const_iterator; 

constexpr int info_width = 400; // width of the info window
constexpr int pacing = 20; // pacing of text in info window

constexpr double zoom_down = 0.8; // scalling down factor (for zooming in and quicken the time)
constexpr double zoom_up = 1.25; // scalling up factor (for zooming out and slowing the time)
constexpr double cam_move_speed = 0.0004; // speed of the cam movement in km/s^-1

//Earth data
constexpr double earth_mass = 5972.2; // 10^21 kg
constexpr double earth_perihelion = 147095000; // km
constexpr double earth_perihelion_speed = 30.29; // km/s
constexpr double earth_diam = 12756; //km

//Mars data
constexpr double mars_mass = 641.69; // 10^21 kg
constexpr double mars_perihelion = 206650000; // km
constexpr double mars_perihelion_speed = 26.50; // km/s
constexpr double mars_diam = 6792; //km

//Sun data
constexpr double sun_mass = 1988500000; // 10^21 kg
constexpr double sun_diam = 1391400; // km

//Venus data
constexpr double venus_mass = 4867.3; // 10^21 kg
constexpr double venus_perihelion = 107480000; //  km
constexpr double venus_perihelion_speed = 35.26; // km/s
constexpr double venus_diam = 12102; //km

//Merkur data
constexpr double merkur_mass = 330.1; // 10^21 kg
constexpr double merkur_perihelion = 46000000; //  km
constexpr double merkur_perihelion_speed = 58.97; // km/s
constexpr double merkur_diam = 4881; //km

//Jupiter data
constexpr double jupiter_mass = 1898130; // 10^21 kg
constexpr double jupiter_perihelion = 740595000; //  km
constexpr double jupiter_perihelion_speed = 13.72; // km/s
constexpr double jupiter_diam = 142984; //km

//Saturn data
constexpr double saturn_mass = 568320; // 10^21 kg
constexpr double saturn_perihelion = 1357554000; //  km
constexpr double saturn_perihelion_speed = 10.14; // km/s
constexpr double saturn_diam = 120536; //km

//Uran data
constexpr double uran_mass = 86811; // 10^21 kg
constexpr double uran_perihelion = 2732696000; //  km
constexpr double uran_perihelion_speed = 7.13; // km/s
constexpr double uran_diam = 51118; //km

//Neptun data
constexpr double neptun_mass = 102409; // 10^21 kg
constexpr double neptun_perihelion = 4471050000; //  km
constexpr double neptun_perihelion_speed = 5.47; // km/s
constexpr double neptun_diam = 49528; //km

//Ship data
constexpr double ship_mass = 0.0000001; // 10^21 kg
constexpr double ship_height = 20000000 + earth_perihelion; // km
constexpr double ship_speed_pow = g_const * earth_mass / ship_height;
constexpr double ship_diam = 1; // km


class Space {
private:
	// map of all space objects
	std::unordered_map<std::string, std::unique_ptr<Object>> space_objects;

	// function that applies gravitation of other object on every object
	void gravity_calc();

	// collision cotrol of all objects and resolving the collisions
	void collision_check();

	// sub function of collision_check() that resolves the collisions that happend
	void collision_resolver(const VectorPairs& to_resolve);

	// bool for collision resolver that just checks that we dont put more than once pair of object to be resolved
	bool is_in_id_list(const std::vector<std::string>& ids, const std::string& id) const;

	// function that just connect two groups of colliding Objects if at least one of them is in both
	void bind_classes(std::vector<std::vector<std::string>>& classes, const Iterator& first_it, const Iterator& second_it);

	// function that creates ne Objects from the Objects that crashed together
	void connect_classes(std::vector<std::vector<std::string>>& classes);

	// function that moves all Objects in space_objects in corrolation with their speed
	void move_objects(double dt);

	// function that updates speed of all Objects in space_objects in corrolation with their accelaration
	void actualize_speed_of_objects(double dt, sf::RenderWindow& window);

	// function that takes care of moving the cam around the space
	void move_cam(double dt);

public:
	double _zoom_coef = 1; // zoom coeficient that indicates from which distance we are looking at space
	double _time_speed_coef = 1; // coeficient of speed of time, it determines how quickly is the time passing in space
	bool _is_stopped = true; // indicates if the simulation is stopped
	bool _cam_is_locked_player = false; // indicates if the cam is locked on player position or if it free
	bool _is_game_over = false; // indicates if the palyer crashed
	std::pair<double, double> _window_center = { screen_width_half, screen_height_half }; // holds space coordinates of the middle of the RenderWindow
	Player_Object* _player; // helper pointer to the player ship

	Space();

	// function that adds object to the space
	void add_object(const std::string& id, const int mass, const double diam, const int iamge_diam, const std::pair<double, double>& speed, const std::pair<double, double>& coord);

	// funtion taht adds palyer object to the space
	void add_player_object(const std::string&, const int mass, const double diam, const int image_diam, const std::pair<double, double>& speed, const std::pair<double, double>& coord);
	
	// funtion that removes object from space
	void remove_object(const std::string& id);

	// function that removes all objects from space
	void clear_objects();

	// function that simulate the space in time difference dt
	void simulate(double dt, sf::RenderWindow& window);

	// function that draws all objects of space
	void draw_objects(sf::RenderWindow& window);

	// function that takes care of inicialization of space objects
	void initialize_space_objects();

	// function that sets player speed to the cosmic speed of the Object that is causing the most gravity force
	void set_player_cosmic_speed();
};


#endif // SPACE_H
