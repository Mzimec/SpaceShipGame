#include "object.h"


Object::Object(double mass, double diam, double image_diam, std::pair<int, int> speed, std::pair<int, int> coord, std::string id)
	: acc{}, speed(speed), mass(mass), diam(diam), image_diam(image_diam), coord(coord) {}

/*
	It draws two circles one white with the minimal diameter to showcase object and one cyan which is the collider of the object.
	It is drawn on the position in the space corresponding to Objects space coordinates cam corrdionates and zoom coeficient
*/
void Object::draw_object(sf::RenderWindow& window, const std::pair<double, double>& center, const double zoom_ratio) {
	double cur_diam = diam / zoom_ratio;
	sf::CircleShape circle(image_diam);
	sf::CircleShape collider(cur_diam);
	circle.setOrigin(image_diam, image_diam);
	collider.setOrigin(cur_diam, cur_diam);
	collider.setFillColor(sf::Color::Cyan);
	circle.move((this->coord.first - center.first) / zoom_ratio + screen_width_half, (this->coord.second - center.second) / zoom_ratio + screen_height_half);
	collider.move((this->coord.first - center.first) / zoom_ratio + screen_width_half, (this->coord.second - center.second) / zoom_ratio + screen_height_half);
	window.draw(circle);
	window.draw(collider);
}

double Object::get_gravity(const Object* other) const {
	double distance = get_distance(coord.first, coord.second, other->coord.first, other->coord.second);
	/* 
		the computation of the gravity accelaration without the needless use general gravity constant
		we are using it just for comparation so it would be pointless computation or we later apply the g_const to it
	*/
	return other->mass / distance / distance; 
}

/*
	function that uses the gravity from get_gravity() and get_norm_vec2() to compute corretly the addition of the gravity accelaration between to Objects
*/
void Object::apply_gravity_from_object(const Object* other) {
	double nondir_acc = get_gravity(other);
	std::pair<double, double> norm_dir_vec = get_norm_vec2(coord.first, coord.second, other->coord.first, other->coord.second);
	acc.first += nondir_acc * norm_dir_vec.first;
	acc.second += nondir_acc * norm_dir_vec.second;
}

void Object::move_object(const double dt) {
	coord.first += speed.first * dt;
	coord.second += speed.second * dt;
}

void Object::actualize_speed(const double dt, sf::RenderWindow& window) {
	speed.first += acc.first * dt;
	speed.second += acc.second * dt;
}

// it computes if the distance is bigger then their diameter
bool Object::is_colliding(const Object* other) const {
	double distance = get_distance(other->coord.first, other->coord.second, coord.first, coord.second);
	return distance < diam + other->diam;
}

Player_Object::Player_Object(double mass, double diam, double image_diam, std::pair<int, int> speed, std::pair<int, int> coord, std::string id)
	: Object(mass, diam, image_diam, speed, coord, id) {}

// function that computes the norm vector between the mouse cursor and player
std::pair<double,double> Player_Object::get_dir(sf::RenderWindow& window, const double zoom_ratio, const std::pair<double,double>& center) const {
	auto cursor_pos = sf::Mouse::getPosition(window); // gets cursor position in Window coordinates
	// computes normal vector and in space coordinates (the crazy computation is converting window perspective to space perspective
	return get_norm_vec2(coord.first, coord.second, (cursor_pos.x - screen_width_half) * zoom_ratio + center.first, (cursor_pos.y - screen_height_half) * zoom_ratio + center.second);
}

// boost ship in the direction of the cursor
void Player_Object::boost_ship(sf::RenderWindow& window, const double zoom_ratio, const std::pair<double,double>& center) {
	auto dir = get_dir(window, zoom_ratio, center);
	speed.first += ship_acc * dir.first;
	speed.second += ship_acc * dir.second;
}

// simillar to the draw_object()
void Player_Object::draw_object(sf::RenderWindow& window, const std::pair<double, double>& center, const double zoom_ratio) {
	double cur_diam = diam / zoom_ratio;
	sf::CircleShape circle(image_diam);
	sf::CircleShape collider(cur_diam);
	circle.setOrigin(image_diam, image_diam);
	collider.setOrigin(cur_diam, cur_diam);
	circle.setFillColor(sf::Color::Magenta);
	collider.setFillColor(sf::Color::Yellow);
	circle.setPosition((coord.first - center.first) / zoom_ratio + screen_width_half, (coord.second - center.second) / zoom_ratio + screen_height_half);
	collider.setPosition((coord.first - center.first) / zoom_ratio + screen_width_half, (coord.second - center.second) / zoom_ratio + screen_height_half);
	window.draw(circle);
	window.draw(collider);
}

// just computational function
std::pair<double, double> Object::get_cosmic_speed(const Object* other) const {
	double distance = get_distance(other->coord.first, other->coord.second, coord.first, coord.second);
	double c_speed = sqrt(g_const * other->mass / distance);
	auto norm_vec = get_norm_vec2(other->coord.first, other->coord.second, coord.first, coord.second);
	return { -norm_vec.second * c_speed + other->speed.first, norm_vec.first * c_speed + other->speed.second };
}

