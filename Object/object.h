#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>

#include <vector>
#include <cmath>
#include <iostream>


constexpr double g_const = 66.743; // General Gravity constant on with units: km^3 * Zkg^-1 * s^-2 (Zkg = 10^21 kg)
constexpr int km_per_px = 2000000; // scale of the space 1 : 2 000 000
constexpr double ship_acc = 20; // accelaration on press km/s^-2
constexpr double ship_dacc = 0.0000001; // deaccelation on press km/s^-2

static sf::VideoMode fullscreen_mode = sf::VideoMode::getDesktopMode(); // Video Mode so the render would be fullscreen.
static const double screen_width_half = fullscreen_mode.width / 2; // center of the RenderWindow width
static const double screen_height_half = fullscreen_mode.height / 2; // center of the RenderWindow height

// template function to get distance of two objects on the 2D plane
template<typename T, typename S>
static double get_distance(const T a1, const T a2, const S b1, const S b2) {
	return std::sqrt(std::pow(std::abs(b1 - a1), 2) +
		std::pow(std::abs(b2 - a2), 2));
}

// template function to get norm vector of vector made by two points on 2D plane
template<typename T, typename S>
static std::pair<double, double> get_norm_vec2(const T a1, const T a2, const S b1, const S b2) {
	double length = get_distance(a1, a2, b1, b2);
	return { (b1 - a1) / length, (b2 - a2) / length };

}

/*
	Class that specifies any space object
*/
class Object {
private:
public:
	const double mass;	//mass of the object
	const double diam;	// diameter of the object also used as collider with the object
	const double image_diam; // minimal diameter for displaying object
	std::pair<double, double> coord; // coordinates of objects in the space
	std::pair<double, double> speed; // speed of the object, has x a y elements
	std::pair<double, double> acc;	// accelaration of the object, has x a y elements
	std::string id; // id of the object

	Object(double mass, double diam, double image_diam, std::pair<int, int> speed, std::pair<int, int> coord, std::string id);

	// function that draws the object
	void virtual draw_object(sf::RenderWindow& window, const std::pair<double,double>& center, const double zoom_ratio);

	// funtion that computes gravity accelartion that other Object applies on this Object
	double get_gravity(const Object* other) const;

	// function that aplies accelartion change of gravity force from other Object on our accelaration
	void apply_gravity_from_object(const Object* other);

	// bool that checks if two Objects are colliding
	bool is_colliding(const Object* other) const;

	// function that move the Objects in correspondence with its speed
	void move_object(const double dt);
	
	// function that computes and update speed of the object based on its accelaration
	virtual void actualize_speed(const double dt, sf::RenderWindow& window);

	// gets speed that is needed for staying on circular trajectory around a object
	std::pair<double, double> get_cosmic_speed(const Object* other)const;
};

/*
	Class derived from Object that adds needed attributes to be placeholder for player ship
*/
class Player_Object : public Object {
private:
	// function that gets norm vector of direction between mouse cursor and player ship
	std::pair<double,double> get_dir(sf::RenderWindow& window, const double zoom_ratio, const std::pair<double,double>& cneter) const;

public:
	Player_Object(double mass, double diam, double image_diam, std::pair<int, int> speed, std::pair<int, int> coord, std::string id);

	// function that speed up ship in chosen direction
	void boost_ship(sf::RenderWindow& window, const double zoom_ratio, const std::pair<double,double>& center);

	// override funtion that draws Player ship
	void draw_object(sf::RenderWindow& window, const std::pair<double,double>& center, const double zoom_ratio) override;
};

#endif // OBJECT_H
