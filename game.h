#pragma once
#include "entity.h"

using namespace std;

class game
{
public:
	game();
	game(int size_, int difficulty_);
	void print();
	bool check_chest();//sees if stepped on chest
	void fix_chest();//makes sure there isnt two chests in one spot
	void check_encounter();//checks if baddies are around
	void set_event();//sees what chest your gonna get
	void set_pieces();//sets the board up
	void start();//starts game duh
	void movement();//asks player to move and moves in that direction
	void do_chest(int position);//takes in position to delete chest and then performs that chest event
	void combat();//does all things combat
	void fancy_thing();//loading screen
	void print_lines(int number);//loading screen lines but customizable
	void print_spaces(int number);//loading screen spaces but customizable
	void set_values();//sets ranges for chest event possibilities (allows for the changing of event probability)
	void change_values(int number);//actually changes the values of probabilities
	void print_combat();//just outputs health and mana of both entities
	friend class entity;
protected:
	int size, pos, end, row, chest_event, class_choice, encounter_timer, range, treasure_counter, treasure_amount, current_x, current_y;
	double difficulty;
	int min1, max1, min2, max2, min3, max3, min4, max4, min5, max5, min6, max6, min7, max7;
	vector<int> chests, topFOV, bottomFOV;
	vector<string> board;
	string charSymbol, name;
	entity* player, *enemy;
	vector<int*> upperRangePointers, lowerRangePointers;
	set<int> fow;
	set<int>::iterator itr;
	bool activate_end;
};
