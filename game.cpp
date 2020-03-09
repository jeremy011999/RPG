#include "game.h"
#include "entity.h"
#include <Windows.h>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//SetConsoleTextAttribute(hConsole, 12);
#define BLACK			0
#define BLUE			1
#define GREEN			2
#define CYAN			3
#define RED				4
#define MAGENTA			5
#define BROWN			6
#define LIGHTGRAY		7
#define DARKGRAY		8
#define LIGHTBLUE		9
#define LIGHTGREEN		10
#define LIGHTCYAN		11
#define LIGHTRED		12
#define LIGHTMAGENTA	13
#define YELLOW			14
#define WHITE			15

game::game()
{

}

game::game(int size_, int difficulty_)
{
	row = size_;
	end = 0;
	size = (size_ * size_);
	encounter_timer = 0;
	treasure_counter = 0;
	set_values();
	activate_end = false;
	for (int i = 0; i < size; i++)
	{
		//string thing = to_string(i) + " ";
		//board.push_back(thing);
		board.push_back("# ");
	}
	if (difficulty_ == 1)
	{
		difficulty = 2.5;
		range = 4;
	}
	else if (difficulty_ == 2)
	{
		difficulty = 5;
		range = 3;
	}
	else if (difficulty_ == 3)
	{
		difficulty = 7.5;
		range = 2;
	}
	pos = board.size() - 1;
	for (int i = 0; i <= 5; i++)
	{
		upperRangePointers.push_back(new int);
		lowerRangePointers.push_back(new int);
		topFOV.push_back(0);
		bottomFOV.push_back(0);
		topFOV.push_back(0);
		bottomFOV.push_back(0);
	}
}

void game::start()
{
	int class_choice;
	cout << "Please choose a class: \n";
	SetConsoleTextAttribute(hConsole, 11);
	cout << setw(20) << "1.Warrior" << setw(20);
	cout << "2.Rogue" << setw(20);
	cout << "3.Mage" << endl << endl;
	SetConsoleTextAttribute(hConsole, 15);
	cout << "Base HP:" << setw(9) << "40" << setw(20) << "32" << setw(20) << "24" << endl << endl;
	cout << "Melee Dmg:" << setw(7) << "6-8" << setw(22) << "9-11" << setw(18) << "3-5" << endl << endl;
	cout << "Mana:" << setw(12) << "10" << setw(20) << "25" << setw(20) << "40" << endl;
	cin >> class_choice;
	system("CLS");
	player=new entity (class_choice);
	if (player->class_name == "warrior")
		charSymbol = "W ";
	if (player->class_name == "rogue")
		charSymbol = "R ";
	if (player->class_name == "mage")
		charSymbol = "M ";
	cin.clear();
	cin.ignore();
	set_pieces();
	print();
	while (treasure_counter!=treasure_amount && player->current_hp > 0)
	{
		movement();
		check_encounter();
	}
		if (player->current_hp <= 0)
		{
			system("CLS");
			cout << "You died :(\nTry again? (Press \"y\")\n";
			string try_again;
			cin >> try_again;
			if (try_again == "y" || try_again == "Y")
				start();
			else
			{
				delete player;
				for (int i = 0; i < upperRangePointers.size(); i++)
				{
					delete upperRangePointers[i];
					delete lowerRangePointers[i];
				}
			}
		}
		else
		{
			activate_end = true;
			board[end] = "E ";
			while (pos != end)
			{
				movement();
				check_encounter();
			}
				system("CLS");
				cout << "Congratulations! You have beat the game!\nPlay again? (Press \"y\")\n";
				string try_again;
				cin >> try_again;
				if (try_again == "y" || try_again == "Y")
					start();
				else
				{
					delete player;
					for (int i = 0; i < upperRangePointers.size(); i++)
					{
						delete upperRangePointers[i];
						delete lowerRangePointers[i];
					}
				}
		}
	
}

void game::print()
{
	system("CLS");
	int a = static_cast<int>(sqrt(size) + .5);//pretty sure this is row size
	int lower_distance= (pos%row); //number of spaces between position and left wall//if 0 then hit wall
	int upper_distance = row-(pos%row)-1; //number of spaces between position and right wall//if 0 then hit wall
	int upper_difference = upper_distance - range; //difference between what range should be and what the range actually can be (right)//negative values mean we are too close
	int lower_difference = lower_distance - range; //difference between what range should be and actually can be (left)

	if (upper_difference >= 0) //basically we are far enough from the wall to allow for full FOV
	{
		*upperRangePointers[0] = range; //set longest range
		for (int i = 0; i < 5; i++)
		{
			if (i <= range)
				*upperRangePointers[i + 1] = *upperRangePointers[i] - 1; //basically just sets them all to be one less than previous
			else
				*upperRangePointers[i] = -1; //rest are nonexistent
		}
	}
	else //close to right wall
	{
		for (int i = 0; i <=-upper_difference; i++) //setting all ranges to max length that reaches wall
			*upperRangePointers[i] = range+upper_difference; 
		for (int i = -upper_difference+1; i < 5; i++) //setting the next ones to appropriate ranges and rest to non existent
		{
			*upperRangePointers[i] = *upperRangePointers[i - 1] - 1;
			if (*upperRangePointers[i] < -1) //if value exceeds -1 which can cause problems set it to -1
				*upperRangePointers[i] = -1;
		}
	}
	if (lower_difference >= 0) //basically we are far enough from the wall to allow for full FOV
	{
		*lowerRangePointers[0] = range; //set longest range
		for (int i = 0; i < 5; i++)
		{
			if (i <= range)
				*lowerRangePointers[i + 1] = *lowerRangePointers[i] - 1; //basically just sets them all to be one less than previous
			else
				*lowerRangePointers[i] = -1; //rest are nonexistent
		}
	}
	else //close to left wall
	{
		for (int i = 0; i <= -lower_difference; i++) //setting all ranges to max length that reaches wall
			*lowerRangePointers[i] = range+lower_difference;
		for (int i = -lower_difference + 1; i < 5; i++) //setting the next ones to appropriate ranges and rest to non existent
		{
			*lowerRangePointers[i] = *lowerRangePointers[i - 1] - 1;
			if (*lowerRangePointers[i] < -1)
				*lowerRangePointers[i] = -1;
		}
	}
	
	for (int i = 0; i < 9; i++)
	{
		if (i<=4)
		topFOV[i]=((pos - (i*row)) - *lowerRangePointers[i]);
		else
			topFOV[i]=((pos - (i -4)*row) + *upperRangePointers[i -4]);
	}
	for (int i = 0; i < 9; i++) 
	{
		if (i <= 4)
			bottomFOV[i] = ((pos + (i*row)) + *upperRangePointers[i]);
		else
			bottomFOV[i] = ((pos + (i - 4)*row) - *lowerRangePointers[i - 4]);
	}
	
	for (int i = 0; i < board.size(); i++)
	{
		if (i == pos)
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << board[i];
			SetConsoleTextAttribute(hConsole, 15);
			if (((i + 1) % a) == 0)//if row is complete go to next line
				cout << endl;
		}
		else
		{
			if (fow.count(i) > 0)
				SetConsoleTextAttribute(hConsole, 2);
			if (i >= topFOV[0] && i <= bottomFOV[0])//adding center row to set
			{
				SetConsoleTextAttribute(hConsole, 15);
				fow.insert(i);
				if (board[i] == "T ")//just changin colors
					SetConsoleTextAttribute(hConsole, 14);
			}
			else if (i >= topFOV[1] && i <= topFOV[5]) //next row up
			{
				SetConsoleTextAttribute(hConsole, 15);
				fow.insert(i);
				if (board[i] == "T ")//just changin colors
					SetConsoleTextAttribute(hConsole, 14);
			}
			else if (i >= topFOV[2] && i <= topFOV[6]) //next row up
			{
				SetConsoleTextAttribute(hConsole, 15);
				fow.insert(i);
				if (board[i] == "T ")//just changin colors
					SetConsoleTextAttribute(hConsole, 14);
			}
			else if (i >= topFOV[3] && i <= topFOV[7]) //next row up
			{
				SetConsoleTextAttribute(hConsole, 15);
				fow.insert(i);
				if (board[i] == "T ")//just changin colors
					SetConsoleTextAttribute(hConsole, 14);
			}
			else if (i >= topFOV[4] && i <= topFOV[8]) //next row up
			{
				SetConsoleTextAttribute(hConsole, 15);
				fow.insert(i);
				if (board[i] == "T ")//just changin colors
					SetConsoleTextAttribute(hConsole, 14);
			}
			else if (i >= bottomFOV[5] && i <= bottomFOV[1])//adding center row to set
			{
				SetConsoleTextAttribute(hConsole, 15);
				fow.insert(i);
				if (board[i] == "T ")//just changin colors
					SetConsoleTextAttribute(hConsole, 14);
			}
			else if (i >= bottomFOV[6] && i <= bottomFOV[2]) //next row up
			{
				SetConsoleTextAttribute(hConsole, 15);
				fow.insert(i);
				if (board[i] == "T ")//just changin colors
					SetConsoleTextAttribute(hConsole, 14);
			}
			else if (i >= bottomFOV[7] && i <= bottomFOV[3]) //next row up
			{
				SetConsoleTextAttribute(hConsole, 15);
				fow.insert(i);
				if (board[i] == "T ")//just changin colors
					SetConsoleTextAttribute(hConsole, 14);
			}
			else if (i >= bottomFOV[8] && i <= bottomFOV[4]) //next row up
			{
				SetConsoleTextAttribute(hConsole, 15);
				fow.insert(i);
				if (board[i] == "T ")//just changin colors
					SetConsoleTextAttribute(hConsole, 14);
			}
			else
			{
				if (fow.count(i) > 0)
					SetConsoleTextAttribute(hConsole, 8);
				else
					SetConsoleTextAttribute(hConsole, 0);
			}
			if (board[i] == "E ")
				SetConsoleTextAttribute(hConsole, 14);
			cout << board[i];//outputting board
			SetConsoleTextAttribute(hConsole, 15);
			if (((i + 1) % a) == 0)//complete row again
				cout << endl;
		}

	}
	cout << "Encounter Timer: " << encounter_timer << "   ";
	cout << "Current HP: " << player->current_hp << "   ";
	cout << "Current Mana: " << player->current_mana << "   ";
	cout << "Treasure Found: " << treasure_counter << "/" << treasure_amount << endl;
}

void game::set_pieces()
{
	int checker = 0;
	int q = 0;
	srand(time(NULL)+q);
	int a = (static_cast<int>((sqrt(size) + .5))+1)/2;
	end = rand() % board.size();
	board[board.size() - 1] = charSymbol;
	pos = board.size() - 1;
	for (int i = 0; i < a; i++)//create vector of ints for chest that corresponds to location on board
	{
		chests.push_back(rand() % (board.size() - 2) + 1);//push_back random numbers for locations ranging from 1-boardsize-2 i think
		q++;
	}
	while (check_chest() == true) //should be just making sure that none are the same could probs do a set later but still need while loop i think
		fix_chest();
	for (int j = 0; j < chests.size(); j++)
	{
		board[chests[j]] = "T ";
	}
	treasure_amount = chests.size();
	while (1)
	{
		for (int f = 0; f < chests.size(); f++)
		{
			if (end == chests[f])
			{
				q++;
				end = rand();
			}
			else if (f == chests.size() - 1 && end != chests[f])
				checker++;
		}
		if (checker >= 2)
			break;
	}
}

void game::movement()
{
	cout << "Use WASD to move then press enter.\nPress \"i\" to view inventory\n"; //outputting twice need to find out why
	char c;
	string input;
	getline(cin, input);
	if (input.length() > 1)
	{
		cout << "Please enter one character at a time." << endl;
		c = 't';
	}
	else
		c = input[0];
	if (c == 'i')
	{
		player->print_inventory();
	}
	else if (c == 'w')
	{
		if (pos - row < 0)
		{
			cout << "Invalid movement\n";
		}
		else
		{
			board[pos] = "# ";
			pos = pos - row;
			board[pos] = charSymbol;
			encounter_timer++;
			for (int i = 0; i < chests.size(); i++)
			{
				if (pos == chests[i])
					do_chest(i);
			}
			print();
		}
	}
	else if (c == 's')
	{
		if (pos + row >= board.size())
		{
			cout << "Invalid movement\n";
		}
		else
		{
			board[pos] = "# ";
			pos = pos + row;
			board[pos] = charSymbol;
			encounter_timer++;
			for (int i = 0; i < chests.size(); i++)
			{
				if (pos == chests[i])
					do_chest(i);
			}
			print();
		}
	}
	else if (c == 'a')
	{
		if (pos % row == 0)
		{
			cout << "Invalid movement\n";
		}
		else
		{
			board[pos] = "# ";
			pos = pos - 1;
			board[pos] = charSymbol;
			encounter_timer++;
			for (int i = 0; i < chests.size(); i++)
			{
				if (pos == chests[i])
					do_chest(i);
			}
			print();
		}
	}
	else if (c == 'd')
	{
		if ((pos + 1) % row == 0)
		{
			cout << "Invalid movement\n";
		}
		else
		{
			board[pos] = "# ";
			pos = pos + 1;
			board[pos] = charSymbol;
			encounter_timer++;
			for (int i = 0; i < chests.size(); i++) //this could maybe be done better rather than checking all of them 
			{
				if (pos == chests[i])
					do_chest(i);
			}
			print();
		}
	}
}

void game::do_chest(int position)//position is to erase that chest from board
{
	set_event();
	cout << "You found a chest!" << endl;
	if (chest_event==2)
	{
		SetConsoleTextAttribute(hConsole, 12);
		cout << "You have found a weapon of epic proportions\n";
		SetConsoleTextAttribute(hConsole, 14);
		cout << "Damage +2\n";
		player->dmg +=2;
		SetConsoleTextAttribute(hConsole, 15);
		change_values(2);
	}
	if (chest_event == 4)
	{
		SetConsoleTextAttribute(hConsole, 12);
		cout << "You open the chest to find a fresh set of armor\n";
		SetConsoleTextAttribute(hConsole, 14);
		cout << "Max health increased by 25%\n";
		player->current_hp += .25*player->max_health;
		player->max_health *= 1.25;
		SetConsoleTextAttribute(hConsole, 15);
		change_values(4);
	}
	if (chest_event == 3)
	{
		SetConsoleTextAttribute(hConsole, 12);
		cout << "At the bottom of the chest lies a shiny, red potion of healing.\n";
		if (player->current_hp < player->max_health)
		{
			player->current_hp = player->max_health;
			SetConsoleTextAttribute(hConsole, 14);
			cout << "Health restored\n";
		}
		else if (player->current_hp == player->max_health)
		{
			player->inventory.push_back("Health Potion");
			SetConsoleTextAttribute(hConsole, 14);
			cout << "Health potion added to inventory\n";
		}
		SetConsoleTextAttribute(hConsole, 15);
	}
	if (chest_event == 1)
	{
		SetConsoleTextAttribute(hConsole, 12);
		cout << "Underneath a large pile of junk lies a mana potion.\n";
		if (player->current_mana < player->max_mana)
		{
			player->current_mana = player->max_mana;
			SetConsoleTextAttribute(hConsole, 14);
			cout << "Mana restored\n";
		}
		else if (player->current_mana == player->max_mana)
		{
			player->inventory.push_back("Mana Potion");
			SetConsoleTextAttribute(hConsole, 14);
			cout << "Mana potion added to inventory\n";
		}
		SetConsoleTextAttribute(hConsole, 15);
	}
	if (chest_event == 5)
	{
		SetConsoleTextAttribute(hConsole, 12);
		cout << "Entering into a cave you find a chest. To your dissapointment the chest is empty.\n";
		cout << "As you turn to leave the cave, you hear voices and footsteps outside pass\n";
		SetConsoleTextAttribute(hConsole, 14);
		cout << "Encounter Timer set to 0.\n";
		encounter_timer = 0;
		SetConsoleTextAttribute(hConsole, 15);
	}
	if (chest_event == 6)
	{
		SetConsoleTextAttribute(hConsole, 12);
		cout << "You attempt to open the chest but it is locked.\nYou hear a loud *thunk* as your vision goes dark.\n";
		SetConsoleTextAttribute(hConsole, 8);
		cout << "You awake to find yourself back where you started.\n";
		board[pos] = "# ";
		pos = board.size() - 1;
		board[pos] = charSymbol;
		SetConsoleTextAttribute(hConsole, 15);
		change_values(6);
	}
	if (chest_event == 7)
	{
		SetConsoleTextAttribute(hConsole, 12);
		cout << "You open up the chest and an enemy pops out!\n";
		SetConsoleTextAttribute(hConsole, 8);
		cout << "Intiate Combat\n";
		system("pause");
		SetConsoleTextAttribute(hConsole, 15);
		combat();
		SetConsoleTextAttribute(hConsole, 15);
	}
	treasure_counter++;
	system("pause");
	chests.erase(chests.begin() + position);
}

void game::set_event()
{
	srand(time(NULL));
	int random = rand() % 100;
	if (random >= min1 && random < max1)
	{
		chest_event = 1;
	}
	if (random >= min2 && random < max2)
	{
		chest_event = 2;
	}
	if (random >= min3 && random < max3)
	{
		chest_event = 3;
	}
	if (random >= min4 && random < max4)
	{
		chest_event = 4;
	}
	if (random >= min5 && random < max5)
	{
		chest_event = 5;
	}
	if (random >= min6 && random < max6)
	{
		chest_event = 6;
	}
	if (random >= min7 && random < max7)
	{
		chest_event = 7;
	}
}

	bool game::check_chest()
{
	for (int i = 0; i < chests.size() - 1; i++)
	{
		for (int j = 1 + 1; j < chests.size(); j++)
		{
			if (chests[i] == chests[j])
				return false;
		}
	}
	return true;
}

void game::fix_chest()
{
	for (int i = 0; i < chests.size()-1; i++)
	{
		for (int j = 1 + 1; j < chests.size(); j++)
		{
			if (chests[i] == chests[j])
				chests[j] = rand() % (board.size() - 1) + 1;
		}
	}
}

void game::check_encounter()
{
	srand(time(NULL));
	int encounter = rand() % 100;
	double max = difficulty * encounter_timer;
	if (encounter <= max)
	{
		system("Pause");
		combat();
		encounter_timer = 0;
	}
	else
		return;
}

void game::combat()
{
	system("CLS");
	fancy_thing();
	system("CLS");
	srand(time(NULL));
	int choice;
	int turn_counter = 0;
	int counter = 0;
	enemy = new entity();
	cout << "A " << enemy->name << " has appeared!\n";
	cout << "What will you do?\n";
	player->is_in_battle = true;
	enemy->is_in_battle = true;
	while (player->current_hp > 0 && enemy->current_hp > 0 && player->is_in_battle && enemy->is_in_battle) //is in battle allows for running away
	{
		player->is_casting = true;  //is casting allows 
		print_combat();
		if (player->is_poisoned == true)
		{
			player->poisoned();
			counter++;
		}
		if (turn_counter % 2 == 0)
		{
			cout << "It's your turn!\n";
			cout << "1.Attack\n2.Cast a Spell\n3.Attempt to run\n4.Open Inventory\n";
			cin >> choice;
			if (choice != 2)
				system("CLS");
			if (cin.fail())
			{
				cin.clear();
				cin.ignore();
			}
			switch (choice)
			{
			case 1:
			{
				player->attack(enemy);
				turn_counter++;
				break;
			}
			case 2:
			{
				while (player->spells(enemy, 0) == true)
					continue;
				if (!player->is_casting)
				{
					turn_counter++; //returns to players turn if false (when player hits q to quit spells)
				}
				turn_counter++;
				break;
			}
			case 3:
			{
				if (player->run() == true)
					player->is_in_battle=false;
				else
				{
					cout << "You were unable to escape." << endl;
					turn_counter++;
				}
				break;
			}
			}
		}
		if (enemy->current_hp <= 0)
			break;
		if (!player->is_in_battle)//ran away or blink
			break;
		if (turn_counter % 2 == 1)//enemy
		{
			if (enemy->is_poisoned == true)
			{
				enemy->poisoned();
				counter++;
			}
			double random = (double)rand() / RAND_MAX;
			double proportion = static_cast<double>((player->current_hp - enemy->current_hp) / enemy->max_health);
			double run_chance, attack_chance, spell_chance;
			if (proportion < 0)
				proportion *= -1;
			run_chance = exp(proportion - 2.5);
			if (enemy->check_mana() <= 0)
			{
				attack_chance = 1 - run_chance;
				spell_chance = 0;
			}
			else
			{
				attack_chance = (1 - run_chance) / 2;
				spell_chance = attack_chance;
			}
			if (random > run_chance&&random <= (attack_chance + run_chance))
			{
				enemy->attack(player);
				turn_counter++;

			}
			if (random > (attack_chance + run_chance) && random <= (spell_chance + run_chance + attack_chance))
			{
				enemy->spells(player, 1);
				turn_counter++;

			}
			if (random >= 0 && random <= run_chance)
			{
				if (enemy->run() == true)
					enemy->is_in_battle=false;
				else
				{
					cout << "The enemy " << enemy->class_name << " was unable to escape." << endl;
					turn_counter++;

				}
			}

		}
	}
	if (enemy->current_hp <= 0)
	{
		cout << "You defeated the evil " << enemy->class_name << "!" << endl;
		cout << "Regain 5 health and mana!\n" << endl;
		if (player->current_hp <= player->max_health - 5)
			player->current_hp += 5;
		else
			player->current_hp = player->max_health;
		if (player->current_mana <= player->max_mana - 5)
			player->current_mana += 5;
		else
			player->current_mana = player->max_mana;
	}
	if (!enemy->is_in_battle)
	{
		cout << "The " << name << " got away!\n";
	}
	if (!player->is_in_battle)
	{
		cout << "You managed to escape the battle with your life.\n";
	}
	player->is_tough = false;
	player->is_poisoned = false;
	system("Pause");
	system("CLS");
	cin.clear();
	cin.ignore();
	print();
	delete enemy;
}

void game::fancy_thing()
{
	int number = 60;
	for (int i = 0; i <= 28; i += 2)
	{
		Sleep(280-i*10);
		print_spaces(i);
		print_lines(number - (2 * i));
		print_spaces(i);
		cout << endl;
	}
	for (int i = 28; i >= 0; i -= 2)
	{
		Sleep(40);
		print_spaces(i);
		print_lines(number - (2 * i));
		print_spaces(i);
		cout << endl;
	}
}

void game::print_lines(int number)
{
	for (int i = 1; i <= number; i++)
	{
		cout << "-";
	}
}

void game::print_spaces(int number)
{
	for (int i = 1; i <= number; i++)
	{
		cout << " ";
	}
}

void game::set_values()
{
	min1 = 0;
	max1 = 20;
	min2 = 20;
	max2 = 30;
	min3 = 30;
	max3 = 45;
	min4 = 45;
	max4 = 55;
	min5 = 55;
	max5 = 70;
	min6 = 70;
	max6 = 80;
	min7 = 80;
	max7 = 100;
}

void game::change_values(int number)
{
	switch (number)
	{
	case 2:
	{
		min2 = max2 = -1;
		max1 += 5;
		min3 -= 5;
	}
	case 4:
	{
		min4 = max4 = -1;
		max3 += 5;
		min5 -= 5;
	}
	case 6:
	{
		min6 = max6 = -1;
		max5 += 5;
		min7 -= 5;
	}
	}
}

void game::print_combat()
{
	cout << "Current Health: " << player->current_hp << "   " << "Enemy health: " << enemy->current_hp << endl;
	cout << "Current Mana: " << player->current_mana << "     " << "Enemy mana: " << enemy->current_mana << endl;
}


/*****************
TO DO LIST

1. Finish FOW woot woot
2. Make exit only work when all chests have been found YYYYEEEEE
3. Randomize exit and only show and work when finished YAAAASSSSSSS

*/