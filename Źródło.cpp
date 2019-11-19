#include <iostream>
#include <windows.h>
#include <ctime>

using namespace std;


#pragma region funkcje.
void Set_screen_size_1();
void Render_map();
void Ship_choose_loc();
int Place_ship(int, int, int[], int);
int Test_ship_place(int, int, int[], int);
int Place_ship_blockades(int, int);
#pragma endregion



#pragma region Opcje.
const int map_height = 10;
const int map_width = 10;
const int UI_bonus_size = 3;
const int waves = 4;
const int ship_amount_table[waves][2] = { {1,4},{2,3},{3,2},{4,1}}; // statki {ilosc_statkow, ilosc masztow}
#pragma endregion


int map_status_tileset[map_height][map_width]{};



int main()
{
	srand(time(NULL));
	Set_screen_size_1();

	
	Ship_choose_loc();

	Render_map();

	cin.get();
	return 0;
}

void Set_screen_size_1()
{
HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

		COORD a;
		a.X = 0;
		a.Y = 300;

		SMALL_RECT rect;
		rect.Top = 0;
		rect.Left = 0;
		rect.Bottom = map_height+UI_bonus_size;
		rect.Right = map_width*3+UI_bonus_size;


		SetConsoleScreenBufferSize(handle, a);

		SetConsoleWindowInfo(handle, 1, &rect);
}

void Render_map()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	char litera_oznaczenia_mapy = 'A';

	for (int i1 = -1;i1 < map_height;i1++)
	{
		if (i1 < 0)
		{
			cout.width(1);
			//cout.fill(' ');
			cout << " X ";
			for (int i1_liczby = 0;i1_liczby < map_width;i1_liczby++)
			{
				cout.width(2);
				cout<< litera_oznaczenia_mapy<<" ";
				litera_oznaczenia_mapy++;
				if (litera_oznaczenia_mapy == 'Z' + 1)
				{
					litera_oznaczenia_mapy = 'a';
				}
			}
		}
		else
		for (int i2 = -1;i2 < map_width;i2++)
		{
			if (i2 < 0)
			{
				cout.width(2);
				cout<< i1<<" ";
			}
			else
			switch (map_status_tileset[i1][i2])
			{
			case 0:
				SetConsoleTextAttribute(hConsole, 11);
				cout << "[~]";
				SetConsoleTextAttribute(hConsole, 7);
				break;
			case 1:
				SetConsoleTextAttribute(hConsole, 13);
				cout << "[S]";
				SetConsoleTextAttribute(hConsole, 7);
				break;
			default:
				SetConsoleTextAttribute(hConsole, 11);
				cout << "[~]";
				SetConsoleTextAttribute(hConsole, 7);
				break;
			}
		}

		cout << endl;
	}
}

void Ship_choose_loc()
{
	int ship_mast_amount{};
	int flag{};
	int rand_x{};
	int rand_y{};
	int rand_direction[2];//VECTOR 2D
	int ship_amount{};

	for(int s_wave=0;s_wave<waves;s_wave++)
	{
		ship_amount = ship_amount_table[s_wave][0];
		ship_mast_amount = ship_amount_table[s_wave][1];
		for (int s_amount = 0;s_amount < ship_amount;s_amount++)
		{

			flag = 0;//swap later
					
			while (flag == 0)
			{	
				rand_x = rand() % map_height;
				rand_y = rand() % map_width;
				
				switch (rand() % 4)
					{
				case 0:
					rand_direction[0] = 0;
					rand_direction[1] = 1;
					break;
				case 1:
					rand_direction[0] = 1;
					rand_direction[1] = 0;
					break;
				case 2:
					rand_direction[0] = -1;
					rand_direction[1] = 0;
					break;
				case 3:
					rand_direction[0] = 0;
					rand_direction[1] = -1;
					break;
					}
				
				

				/*check if possible*/
				flag = Test_ship_place(rand_x, rand_y, rand_direction, ship_mast_amount);			
			}
			Place_ship(rand_x, rand_y, rand_direction, ship_mast_amount);
		}//for_amount
	}//for_table_size
}

int Place_ship(int x, int y, int dir[], int mast_amount)
{
	for (int p1 = 0;p1 < mast_amount;p1++)
	{
		map_status_tileset[x + (dir[0]*p1) ][y + (dir[1]*p1) ] = 1;
		Place_ship_blockades(x + (dir[0] * p1), y + (dir[1] * p1));
	}

	return 0;
}

int Place_ship_blockades(int x, int y)
{
	for (int i1 = -1;i1 < 2;i1++)
	{
		for (int i2 = -1;i2 < 2;i2++)
		{
			if (map_status_tileset[x + i1][y + i2] != 1)
			{
				map_status_tileset[x + i1][y + i2] = -1;
			}
		}
	}
	return 0;
}

int Test_ship_place(int x, int y, int dir[], int mast_amount)
{
	if (x + (dir[0] * mast_amount)<0 || x + (dir[0] * mast_amount) > map_height )
	{
		return 0;
	}
	if (y + (dir[1] * mast_amount) < 0 || y + (dir[1] * mast_amount) > map_width)
	{
		return 0;
	}
	
	for (int p1 = 0;p1 < mast_amount;p1++)
	{
		if (map_status_tileset[x + (dir[0] * p1)][y + (dir[1] * p1)] !=0)
		{
			
			return 0;
		}
		
	}
	
	return 1;
}