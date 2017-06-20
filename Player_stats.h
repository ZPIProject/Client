#pragma once
#include <string>


class Player_stats {
	float current_health;
	float MAX_health;
	float current_mana;
	float MAX_mana;
	float base_dmg;
	float speed;

	int knowledge;
	int wisdom;
	int witality;

	std::string name;

public:

	Player_stats(float Cur_HP = 0, float MAXHP = 0, float Cur_MN = 0, float MAXMN = 0, float BSDMG = 0, float SPD = 0, int know = 0, int wis = 0, int wit = 0, std::string nam = "") {
		current_health = Cur_HP;
		MAX_health = MAXHP;
		current_mana = Cur_MN;
		MAX_mana = MAXMN;
		base_dmg = BSDMG;
		speed = SPD;
		knowledge = know;
		wisdom = wis;
		witality = wit;
		name = nam;
	}



	float get_current_health() {
		return current_health;
	}
	float get_current_mana() {
		return current_mana;
	}
	float get_MAX_health() {
		return MAX_health;
	}
	float get_MAX_mana() {
		return MAX_mana;
	}
	float get_base_dmg() {
		return base_dmg;
	}
	float get_speed() {
		return speed;
	}

	int get_knowledge() {
		return knowledge;
	}
	int get_wisdom() {
		return wisdom;
	}
	int get_witality() {
		return witality;
	}
	std::string get_name() {
		return name;
	}

	void set_current_health(float new_value) {
		 current_health=new_value;
	}
	void set_current_mana(float new_value) {
		 current_mana = new_value;
	}
	void set_MAX_health(float new_value) {
		 MAX_health = new_value;
	}
	void set_MAX_mana(float new_value) {
		 MAX_mana = new_value;
	}
	void set_base_dmg(float new_value) {
		 base_dmg = new_value;
	}
	void set_speed(float new_value) {
		 speed = new_value;
	}

	void set_knowledge(int new_value) {
		 knowledge = new_value;
	}
	void set_wisdom(int new_value) {
		 wisdom = new_value;
	}
	void set_witality(int new_value) {
		 witality = new_value;
	}
	void set_name(std::string new_value) {
		 name = new_value;
	}
};