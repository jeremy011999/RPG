#include "game.h"
#include "entity.h"
#include <Windows.h>

HANDLE HConsole = GetStdHandle(STD_OUTPUT_HANDLE);
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

entity::entity()
{
	attacks_are_dumb = 0; //variable to increase random seed by 1 per attack since they were happening too fast
	srand(time(NULL));
	int a = rand() % 3 + 1;
	if (a == 1)
		class_name = "Warrior";
	else if (a == 2)
		class_name = "Rogue";
	else if (a == 3)
		class_name = "Mage";
	if (class_name == "Warrior")
	{
		max_health = 20;
		dmg = 6;
		max_mana = 10;
		current_mana = max_mana;
		current_hp = max_health;
	}
	else if (class_name == "Rogue")
	{
		max_health = 16;
		dmg = 9;
		max_mana = 25;
		current_mana = max_mana;
		current_hp = max_health;
	}
	else if (class_name == "Mage")
	{
		max_health = 12;
		dmg = 3;
		max_mana = 40;
		current_mana = max_mana;
		current_hp = max_health;
	}
	string b = "Wild ";
	name = b + class_name;
}

entity::entity(int a)
{
	attacks_are_dumb = 0;
	if (a == 1)
		class_name = "warrior";
	else if (a == 2)
		class_name = "rogue";
	else if (a == 3)
		class_name = "mage";
	name = "You";
	if (class_name == "warrior")
	{
		max_health = 40;
		dmg = 6;
		max_mana = 10;
		current_mana = max_mana;
		current_hp = max_health;
	}
	else if (class_name == "rogue")
	{
		max_health = 32;
		dmg = 9;
		max_mana = 25;
		current_mana = max_mana;
		current_hp = max_health;
	}
	else if (class_name == "mage")
	{
		max_health = 24;
		dmg = 3;
		max_mana = 40;
		current_mana = max_mana;
		current_hp = max_health;
	}
}

void entity::attack(entity* target)
{
	srand(time(NULL) + attacks_are_dumb);
	int damage = (dmg + rand() % 3);
	if (target->is_tough == true)
	{
		{
			if (toughness_counter <= 2)
			{
				cout << target->name << " is super tough and only took " << damage/2 << " damage!" << endl;
				target->current_hp -= damage/2;
				toughness_counter++;
			}
			else
			{
				cout << target->name << " starts to look totally not as tough as before!\n";
				is_tough = false;
				toughness_counter = 0;
				attack(target);
				return;
			}
			attacks_are_dumb++;
		}
	}
	else
	{
		
		target->current_hp -= damage;
		cout << name << " did " << damage << " damage to " << target->name << endl;
		attacks_are_dumb++;
	}
}

bool entity::run()
{
	int i = 0;
	srand(time(NULL)+i);
	int random = rand() % 100;
	if (random <= 19)
		return true;
	else
		return false;
	i++;
}

bool entity::spells(entity* target, int turn)
{
	int i = 0;
	int intchoice;
	srand(time(NULL) + i);
	string choice;
	if (class_name == "mage" || class_name == "Mage")//mage spells
	{
		if (turn == 1)//computer
		{
			int available = check_mana();
			if (check_mana() <= 0)
				return false;
			int random = rand() % available + 1;
			switch (random)
			{
			case 1:
			{
				heal();
				return false;
			}
			case 2:
			{
				fireball(target);
				return false;
			}
			case 3:
			{
				blink(target, 1);
				cout << name << " blinked away!\n";
				return false;
			}
			}
		}
		else if (turn == 0)//player
		{
			cout << "1.Fireball (10 damage / 10 mana)\n2.Heal (+7 hp / 5 mana)\n3.Blink (Escape combat / 15 mana)\n";//output list
			cin >> choice;
			if (choice == "1")
			{
				if (current_mana >= 10)//check mana
				{
					system("CLS");
					fireball(target);
					return false;//exits spells
				}
				else
				{
					SetConsoleTextAttribute(HConsole, 12);
					cout << "You do not have enough mana!\n";
					SetConsoleTextAttribute(HConsole, 15);
					cout << "Please choose a new spell or q to go back\n";
					return true;//hopefully redoes spells

				}
			}
			if (choice == "2")
			{
				if (current_mana >= 5)
				{
					system("CLS");
					heal();
					return false;
				}
				else
				{
					SetConsoleTextAttribute(HConsole, 12);
					cout << "You do not have enough mana!\n";
					SetConsoleTextAttribute(HConsole, 15);
					cout << "Please choose a new spell or q to go back\n";
					return true;//hopefully redoes spells
				}
			}
			if (choice == "3")
			{
				if (current_mana >= 15)
				{
					system("CLS");
					blink(target, 0);
					return false;
				}
				else
				{
					SetConsoleTextAttribute(HConsole, 12);
					cout << "You do not have enough mana!\n";
					SetConsoleTextAttribute(HConsole, 15);
					cout << "Please choose a new spell or q to go back\n";
					return true;//hopefully redoes spells
				}
			}
			if (choice == "q" || choice == "Q")
			{
				is_casting = false; //knows to exit spells but return the turn counter to players turn
				return false;
			}
		}//end player
	}//end mage
	else if (class_name == "rogue" || class_name == "Rogue")//start rogue
	{
		if (turn == 1)//computer
		{
			int available = check_mana();
			if (available <= 0)
				return false;
			else if (available == 1)
			{
				poison_strike(target);
				return false;
			}
			else
			{
				int random = rand() % 2 + 1;
				switch (random)
				{
				case 1:
				{
					critical_strike(target);
					return false;
				}
				case 2:
				{
					poison_strike(target);
					return false;
				}
				}
			}
		}
		else if (turn == 0)//player
		{
			cout << "1.Critical Strike\n2.Poison Blade\n";//output list
			cin >> choice;
			if (choice == "1")
			{
				if (current_mana >= 10)//check mana
				{
					system("CLS");
					critical_strike(target);
					return false;//exits spells
				}
				else
				{
					SetConsoleTextAttribute(HConsole, 12);
					cout << "You do not have enough mana!\n";
					SetConsoleTextAttribute(HConsole, 15);
					cout << "Please choose a new spell or q to go back\n";
					return true;//hopefully redoes spells

				}
			}
			if (choice == "2")
			{
				if (current_mana >= 10)
				{
					system("CLS");
					poison_strike(target);
					return false;
				}
				else
				{
					SetConsoleTextAttribute(HConsole, 12);
					cout << "You do not have enough mana!\n";
					SetConsoleTextAttribute(HConsole, 15);
					cout << "Please choose a new spell or q to go back\n";
					return true;//hopefully redoes spells
				}
			}
			if (choice == "q" || choice == "Q")
			{
				is_casting = false;
				return false;
			}
		}//end player
	}//end rogue
	else if (class_name == "Warrior" || class_name == "warrior")
	{
		if (turn == 1)//computer
		{
			int available = check_mana();
			if (available <= 0)
				return false;
			else
			{
				toughness();
				return false;
			}
		}
		else if (turn == 0)//player
		{
			cout << "1.Toughness\n";//output list
			cin >> choice;
			if (choice == "1")
			{
				if (current_mana >= 10)//check mana
				{
					system("CLS");
					toughness();
					return false;//exits spells
				}
				else
				{
					SetConsoleTextAttribute(HConsole, 12);
					cout << "You do not have enough mana!\n";
					SetConsoleTextAttribute(HConsole, 15);
					cout << "Please choose a new spell or q to go back\n";
					return true;//hopefully redoes spells

				}
			}
			if (choice == "q" || choice == "Q")
			{
				is_casting = false;
				return false;
			}
		}//end player
	}//end warrior

}

void entity::fireball(entity* target)
{
	cout << name << " did 10 damage to " << target->name << " with fireball!\n";
	target->current_hp -= 10;
	current_mana -= 10;
}

void entity::heal()
{
	cout << name << " restored 7 health!\n";
	current_hp += 7;
	current_mana -= 5;
}

void entity::blink(entity* target, int turn)
{
	if (turn == 0)
	{
		cout << name << " used ";
		SetConsoleTextAttribute(HConsole, 12);
		cout << "blink\n";
		SetConsoleTextAttribute(HConsole, 15);
		target->current_hp = 0;
		current_mana -= 15;
	}
	if (turn == 1)
	{
		current_hp = 0;
	}
}

void entity::critical_strike(entity* target)
{
	cout << name << " used ";
	SetConsoleTextAttribute(HConsole, 12);
	cout << "Critical Strike ";
	SetConsoleTextAttribute(HConsole, 15);
	cout << "and did " << static_cast<int>(dmg * 1.5) << " damage!" << endl;
	target->current_hp -= static_cast<int>(dmg * 1.5);
	current_mana -= 15;
}

void entity::poison_strike(entity* target)
{
	cout << name << " used ";
	SetConsoleTextAttribute(HConsole, 12);
	cout << "Poison Strike ";
	SetConsoleTextAttribute(HConsole, 15);
	cout << target->name << " is now poisoned!" << endl;
	attack(target);
	target->is_poisoned = true;
	target->poison_counter = 0;
	current_mana -= 10;
}

void entity::toughness()
{
	cout << name << " uses Toughness to decrease damage for the next 3 turns!\n";
	is_tough = true;
	toughness_counter = 0;
	current_mana -= 10;
}

int entity::check_mana()
{
	if (class_name == "Mage")
	{
		if (current_mana >= 15)
			return 3;
		else if (current_mana < 15 && current_mana >= 10)
			return 2;
		else if (current_mana < 10 && current_mana >= 5)
			return 1;
		else if (current_mana < 5)
			return 0;
	}
	if (class_name == "Rogue")
	{
		if (current_mana >= 15)
			return 2;
		else if (current_mana < 15 && current_mana >= 10)
			return 1;
		else if (current_mana < 10)
			return 0;
	}
	if (class_name == "Warrior")
	{
		if (current_mana >= 10)
			return 1;
		else if (current_mana < 10)
			return 0;
	}
}

void entity::poisoned()
{
	if (poison_counter <= 2)
	{
		cout << name << " takes 5 poison damage!" << endl;
		current_hp -= 5;
		poison_counter++;
	}
	else
	{
		is_poisoned = false;
		poison_counter = 0;
		return;
	}
}


void entity::add_inventory(string item)
{
	if (inventory.size() <= 3)
	{
		inventory.push_back(item);
		cout << item << " was added to your inventory.\n";
		cout << "You have " << 3 - inventory.size() << " empty spaces left\n";
	}
	if (inventory.size() > 3)
	{
		string response;
		cout << "Bro thats like tOoOOooOo much stuffff! (Inventory max = 3 items)\n";
		cout << "Would you like to drop something and replace it with " << item << "?\n";
		cout << "(Y for yes / N for no)\n";
		getline(cin, response);
		if (response == "Y" || response == "y")
		{
			int dropped;
			list<string>::iterator iter;
			for (iter = inventory.begin(); iter != inventory.end(); iter++)
			{
				int i = 1;
				cout << i << ". " << *iter << endl;
			}
			cin >> dropped;
			if (cin.fail())
			{
				cin.ignore();
				cin.clear();
			}
			drop_item(dropped);
			add_inventory(item);
		}
		else if (response == "N" || response == "n")
		{
			cout<<"Okie dokie dropping " << item << " instead\n";
		}
	}
}

void entity::print_inventory()
{
	int i = 1, choice;
	string item;
	list<string>::iterator iter;
	if (inventory.size() > 0)
	{
		for (iter = inventory.begin(); iter != inventory.end(); iter++)
		{
			cout << i << ". " << *iter << endl;
			i++;
		}
		cout << i << ". Quit\n";
		while (1)
		{
			cout << "Which item would you like to use?\n";
			cin >> choice;
			if (choice == i)
			{
				cout << "Exiting inventory\n";
				break;
			}
			else if (choice > i)
			{
				cout << "Nope. Try again.\n";
			}
			else
			{
				iter = inventory.begin();
				for (int j = 1; j < choice; j++)
				{
					iter++;
				}
				item = *iter;
				use(item, choice);
				break;
			}
		}
	}

	else
	{
		cout << "No items in Inventory.\n";
	}
}

void entity::drop_item(int item)
{
	list<string> ::iterator iter = inventory.begin();
	for (int i = 1; i < item; i++)
	{
		iter++;
	}
	inventory.erase(iter);
}

void entity::use(string name, int position)
{
	if (name == "Mana Potion")
	{
		current_mana = max_mana;
		cout << "You used your Mana Potion!\nMana is restored!\n";
		drop_item(position);
	}
	else if (name == "Health Potion")
	{
		current_hp = max_health;
		cout << "You used your Health Potion!\nHealth is restored!\n";
		drop_item(position);
	}
}