#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <list>
#include <set>

using namespace std;

class entity
{
public:
	entity();
	entity(int a);
	void attack(entity* target);
	bool run();
	bool spells(entity* target, int turn);
	friend class game;
	void fireball(entity* target);
	void heal();
	void blink(entity* target, int turn);
	void toughness();
	void critical_strike(entity* target);
	void poison_strike(entity* target);
	int check_mana();
	void poisoned();
	void add_inventory(string item);
	void print_inventory();
	void drop_item(int item);
	void use(string name, int position);
protected:
	string name, class_name;
	int max_health, dmg, max_mana, current_hp, current_mana, toughness_counter = 0, poison_counter = 0, attacks_are_dumb;
	bool is_tough, is_poisoned, is_in_battle, is_casting = true;
	list<string> inventory;
};
