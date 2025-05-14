#include "space.h"

Space::Space() : _player(nullptr) {}

// creates dynamically new instance of Object
void Space::add_object(const std::string& id, const int mass, const double diam, const int image_diam, const std::pair<double, double>& speed, const std::pair<double, double>& coord) {
	std::unique_ptr<Object> ptr = std::make_unique<Object>(mass, diam, image_diam, speed, coord, id);
	space_objects[id] = std::move(ptr);
}

// creates dynamically new instance of Player_Object
void Space::add_player_object(const std::string& id, const int mass, const double diam, const int image_diam, const std::pair<double, double>& speed, const std::pair<double, double>& coord) {
	std::unique_ptr<Object> ptr = std::make_unique<Player_Object>(mass, diam, image_diam, speed, coord, id);
	if (_player == nullptr) _player = dynamic_cast<Player_Object*>(ptr.get());
	space_objects[id] = std::move(ptr);
}

// calls draw_object() on every Object in space_objects
void Space::draw_objects(sf::RenderWindow& window) {
	if (_cam_is_locked_player) _window_center = _player->coord;
	for (auto&& [id, obj] : this->space_objects) {
		obj->draw_object(window, _window_center, km_per_px * _zoom_coef);
	}
}

void Space::remove_object(const std::string& id) {
	space_objects.erase(id);
}

void Space::clear_objects() {
	space_objects.clear();
}

// On every Object in space_objects it applies gravity using apply_gravity_from_object() from every other Object in space_objects
void Space::gravity_calc() {
	for (auto&& [id1, obj1] : space_objects) {
		obj1->acc.first = 0;
		obj1->acc.second = 0;
		for (auto&& [id2, obj2] : space_objects) {
			if (id1 == id2) continue;
			obj1->apply_gravity_from_object(obj2.get());
		}
		obj1->acc.first *= g_const;
		obj1->acc.second *= g_const;
	}
}

void Space::collision_check() {
	//creates container of pairs to remove
	VectorPairs to_remove;
	for (auto&& [id1, obj1] : space_objects) {
		for (auto&& [id2, obj2] : space_objects) {
			if (id1 == id2) continue;
			std::pair<std::string, std::string> reverse{ id2,id1 };
			//checks if we are not already removing the reverse version of pair and if the Objects are colliding
			if (obj1->is_colliding(obj2.get()) &&
				std::find(to_remove.cbegin(), to_remove.cend(), reverse) == to_remove.end()) {
				if (obj1.get() == _player || obj2.get() == _player) _is_game_over = true;
				to_remove.push_back({ id1,id2 });
			}
		}
	}
	// apply on the container to remove collision_resolver()
	collision_resolver(to_remove);
}

void Space::collision_resolver(const VectorPairs& to_resolve) {
	/*
		creates vector of vector of ids, here we will store collision groups
		one collision group is vector of Objects ids that are in collision connected through each other
	*/
	std::vector<std::vector<std::string>> connectivity_classes;
	// here we for every pair of Objects in to_resolve create new connectivity class or add it to one
	for (auto&& [id1, id2] : to_resolve) {
		auto first_it = std::find_if(connectivity_classes.cbegin(), connectivity_classes.cend(),
			[&](const std::vector<std::string>& ids) {return is_in_id_list(ids, id1);});
		auto second_it = std::find_if(connectivity_classes.cbegin(), connectivity_classes.cend(),
			[&](const std::vector<std::string>& ids) {return is_in_id_list(ids, id2);});
		//this case takes care of the example when one pair would connect two classes using bind_classes()
		if (first_it != connectivity_classes.end() && second_it != connectivity_classes.end()) {
			bind_classes(connectivity_classes, first_it, second_it);
		}
		else if (first_it != connectivity_classes.end()) connectivity_classes[first_it - connectivity_classes.begin()].push_back(id2);
		else if (second_it != connectivity_classes.end()) connectivity_classes[second_it - connectivity_classes.begin()].push_back(id1);
		else connectivity_classes.push_back(std::vector<std::string>{id1, id2});
	}
	connect_classes(connectivity_classes);
}

// bool that checks if there is specific id in vector of ids
bool Space::is_in_id_list(const std::vector<std::string>& ids, const std::string& id) const {
	for (auto&& tmp : ids) {
		if (tmp == id) return true;
	}
	return false;
}

// function that takes care of binding two connectivity classes using insert algortithm
void Space::bind_classes(std::vector<std::vector<std::string>>& classes, const Iterator& first_it, const Iterator& second_it) {
	std::vector<std::string> first_class = *first_it;
	const std::vector<std::string>& second_class = *second_it;

	first_class.insert(first_class.end(), second_class.begin(), second_class.end());

	classes.erase(second_it);
}

//function that computes correct values for ne w clashed object
void Space::connect_classes(std::vector<std::vector<std::string>>& classes) {
	for (auto&& vec : classes) {
		double max_diam = 0;
		double max_image_diam = 0;
		double mass = 0;
		std::pair<double, double> speed;
		std::pair<int, int> coord;
		std::string name;
		// in this cycle we find max diameter, sum of masses and max iamge diameter, computes speed and final coordinates
		for (auto&& id : vec) {
			if (space_objects[id]->diam > max_diam) {
				name = id;
				max_diam = space_objects[id]->diam;
				max_image_diam = space_objects[id]->image_diam;
			}
			if (mass == 0) {
				speed.first = space_objects[id]->speed.first;
				speed.second = space_objects[id]->speed.second;
				coord.first = space_objects[id]->coord.first;
				coord.second = space_objects[id]->coord.second;
			}
			else {
				speed.first = (speed.first * mass + space_objects[id]->speed.first * space_objects[id]->mass) /
					(mass + space_objects[id]->mass);
				speed.second = (speed.second * mass + space_objects[id]->speed.second * space_objects[id]->mass) /
					(mass + space_objects[id]->mass);
				coord.first = (coord.first * mass + space_objects[id]->coord.first * space_objects[id]->mass) /
					(mass + space_objects[id]->mass);
				coord.second = (coord.second * mass + space_objects[id]->coord.second * space_objects[id]->mass) /
					(mass + space_objects[id]->mass);
			}
			mass += space_objects[id]->mass;
			// we remove the object than is already counted to the new one 
			remove_object(id);
		}
		// here we dynamically create new object with correct attributes
		add_object(name, mass, max_diam, max_image_diam, speed, coord);
	}
}

void Space::move_objects(double dt) {
	for (auto&& [id, obj] : space_objects) obj->move_object(dt);
}

void Space::actualize_speed_of_objects(double dt, sf::RenderWindow& window) {
	for (auto&& [id, obj] : space_objects) obj->actualize_speed(dt, window);
}

void Space::move_cam(double dt) {
	if (!_cam_is_locked_player) {
		double cur_cam_speed = cam_move_speed * dt * km_per_px * _zoom_coef;
		// we apply the movement based on pressed buttons
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) _window_center.second -= cur_cam_speed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) _window_center.second += cur_cam_speed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) _window_center.first += cur_cam_speed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) _window_center.first -= cur_cam_speed;
	}
}

void Space::set_player_cosmic_speed() {
	double max_gravity = 0;
	Object* impactful_obj = nullptr;
	// in this for cycle  we are finding the Objects that is causing the most gravity force on player
	for (auto&& [id, obj] : space_objects) {
		if (_player != obj.get()) {
			if (_player->get_gravity(obj.get()) > max_gravity) {
				max_gravity = _player->get_gravity(obj.get());
				impactful_obj = obj.get();
			}
		}
	}
	// here we update player attributes
	_player->speed = _player->get_cosmic_speed(impactful_obj);
	_player->acc = { 0,0 };
}

void Space::simulate(double dt, sf::RenderWindow& window) {
	if (!_is_stopped) {
		move_objects(dt * _time_speed_coef);
		gravity_calc();
		actualize_speed_of_objects(dt * _time_speed_coef, window);
		collision_check();
	}
	draw_objects(window);
	move_cam(dt);
}

void Space::initialize_space_objects() {
	add_object("Earth", earth_mass, earth_diam, 5, { 0, -earth_perihelion_speed }, { -earth_perihelion, 0 });
	add_object("Mars", mars_mass, mars_diam, 5, { 0, -mars_perihelion_speed }, { -mars_perihelion, 0 });
	add_object("Sun", sun_mass, sun_diam, 10, { 0, 0 }, { 0, 0 });
	add_player_object("Player", ship_mass, ship_diam, 3, { 0, -std::sqrt(ship_speed_pow) }, { -ship_height, 0 });
	add_object("Merkury", merkur_mass, merkur_diam, 5, { 0, -merkur_perihelion_speed }, { -merkur_perihelion, 0 });
	add_object("Venus", venus_mass, venus_diam, 5, { 0, -venus_perihelion_speed }, { -venus_perihelion, 0 });
	add_object("Jupiter", jupiter_mass, jupiter_diam, 5, { 0, -jupiter_perihelion_speed }, { -jupiter_perihelion, 0 });
	add_object("Uranus", uran_mass, uran_diam, 5, { 0, -uran_perihelion_speed }, { -uran_perihelion, 0 });
	add_object("Saturnus", saturn_mass, saturn_diam, 5, { 0, -saturn_perihelion_speed }, { -saturn_perihelion, 0 });
	add_object("Neptune", neptun_mass, neptun_diam, 5, { 0, -neptun_perihelion_speed }, { -neptun_perihelion, 0 });
}