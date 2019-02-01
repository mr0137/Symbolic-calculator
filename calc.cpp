#include <iostream>
#include <string>

#pragma warning (disable : 4244 4018)
#define PI 3.14159265

using namespace std;


string correct(string Data);										/*	
																		Убирает лишние пробелы и знак '=' в конце.		
																	*/
string Find_prev_elem(string &Data, int &pos, string &Chars, int elem);/*
																		Находит операнд перед знаком операции основываясь
																		на позиции самого знака. После "чистит" в "нужном
																		месте" данные в строке Data.
																	*/
string Find_next_elem(string &Data, int pos, string &Chars, int elem);/*
																		Работает аналогично _prev_.
																		Фунции должны вызываться одна за другой в конкретном
																		порядке:
																			1. next;
																			2. prev;
																		При вызове в обратном порядке позиция для чистки next
																		будет не верной.
																	*/
float Operation(int elem, string &Data, string &Chars, bool Deg);				/*
																		Функция для выполнения операций над соседними
																		операндами.
																	*/
float Сalculate(string &Data, string &Chars, int count, bool Deg);			/*
																		Подсчёт с учётом приоритетов операций.
																	*/

int Sort(string &Data, string &Chars);								/*	
																		Функция для отделения знаков операций и чисел.
																	*/
int Capacity(const string Data);									/*
																		Выдаёт количество операндов основываясь на пробелах.
																	*/
int Find_curr_pos(const string Data, const string Chars, int elem); /*
																		Возвращает позицию знака конкретной операции в строке 
																		Data.
																    */
int BKT(string &Data, string Chars, bool Deg);						/*
																		Функция выполняющая расстановку приоритетов по
																		средству поиска символов "(" и ")".
																		Выполняет рекурсивные действия.
																		До конца не оптимизирована(иногда "накидывает" пару
																		лишних уровней рекурсии).
																	*/

void Start(string &Data, string &Chars, bool Deg);							/*
																		Функция для вызова Sort и Calculate.
																	*/


bool bkt_check(string Data);										/*
																		Функция для выполнения операций над соседними
																		операндами.
																	*/

void Bugs(string &Data);											/*
																		Костыль : убирает баг, когда в ходе решения встречаются
																		такие моменты, как : "++" "+-" и т.д.
																	*/
int Find_Chars(string Chars);										/*
																		Для упрощения поиска приоритета для sin, cos, tg, ctg, 
																		sqrt и ^
																	*/

string correct(string Data)
{
	int i = 0;

	while (Data.find(' ') != -1)
	{
		i = Data.find(' ');
		if (Data[i] == (char)' ')
		{
			for (i; i < Data.length(); i++)
			{
				Data[i] = Data[i + 1];
			}
			Data.resize(Data.length() - 1);
		}
	}

	for (i = 0; i < Data.length(); i++)
	{
		if ((int)Data[i] >= 65 && (int)Data[i] <= 90)
		{
			Data[i] = (char)((int)Data[i] + 32);
		}
	}

	if (Data[Data.length() - 1] == '=')
	{
		Data.resize(Data.length() - 1);
	}

	cout << Data << endl;

	//вторая проверка для упрощения работы
	for (i = 0; i < Data.length(); i++)
	{
		if ((Data[i] == (char)'s') and (Data[i + 1] == (char)'i') and (Data[i + 2] == (char)'n'))
		{
			Data.erase(i, 3);
			Data.insert(i, "s");
		}
		else if ((Data[i] == (char)'c') and (Data[i + 1] == (char)'o') and (Data[i + 2] == (char)'s'))
		{
			Data.erase(i, 3);
			Data.insert(i, "c");
		}
		else if ((Data[i] == (char)'t') and (Data[i + 1] == (char)'g'))
		{
			Data.erase(i, 2);
			Data.insert(i, "t");
		}
		else if ((Data[i] == (char)'c') and (Data[i + 1] == (char)'t') and (Data[i + 2] == (char)'g'))
		{
			Data.erase(i, 3);
			Data.insert(i, "g");
		}
		else if ((Data[i] == (char)'s') and (Data[i + 1] == (char)'q') and (Data[i + 2] == (char)'r') and (Data[i + 3] == (char)'t'))
		{
			Data.erase(i, 4);
			Data.insert(i, "v");
		}
	}
	
	Data.insert(0, " ");
	Data.insert(Data.length(), " ");

	return Data;
}

int Capacity(const string Data)
{
	int i = 0, counter = 0;

	while (Data[i] != NULL)
	{
		if (Data[i] == ' ')
		{
			counter++;
		}
		i++;
	}
	return counter - 1;
}

int Sort(string &Data, string &Chars)
{
	int i = 0, count = 0;

	while (Data.length() > i)
	{
		if (Data[i] == '+' or Data[i] == '-' or
			Data[i] == '/' or Data[i] == '*' or
			Data[i] == 'v' or Data[i] == '^' or 
			Data[i] == 's' or Data[i] == 'c' or
			Data[i] == 't' or Data[i] == 'g')
		{
			Chars = Chars + (char)Data[i];
			Data[i] = ' ';
			count++;
		}

		i++;
	}

	return count;
}

string Find_prev_elem(string &Data, int &pos, string &Chars, int elem)
{
	string element;
	int i = pos - 1, counter = 0;

	while (Data[i] != ' ')
	{
		i--;
	}
	counter = fabs(pos - i);

	element = Data.substr(pos - counter, counter);
	Data.erase(pos - counter, counter);
	pos = pos - counter;
	return element;
}

string Find_next_elem(string &Data, int pos, string &Chars, int elem)
{
	string element;
	string a = "\0";
	int i = pos + 1, counter = 1;

	if (Data[i] == ' ')
	{
		i++;
	}

	while (Data[i] != ' ')
	{
		i++;
	}
	counter = fabs(pos - i);

	if (Data[pos + 1] == ' ')
	{
		Data.erase(pos, 1);
		a = Chars[elem + 1];
		Chars.erase(elem + 1, 1);
		counter--;
	}

	element = a + Data.substr(pos + 1, counter);
	Data.erase(pos + 1, counter);

	return element;
}

int Find_curr_pos(const string Data, const string Chars, int elem)
{
	int pos = 0, counter = 0;

	while (Data[pos] != NULL)
	{
		if (Data[pos] == ' ')
		{
			if (counter == elem + 1)
			{
				return pos;
			}
			counter++;
		}
		pos++;
	}

	return -1;
}

void Bugs(string &Data)
{
	int i = 0;

	while (Data[i] != NULL)
	{
		if (Data[i] == '+')
		{
			if (Data[i + 1] == '-')
			{
				Data.erase(i, 1);
				i = 0;
			}
			else if (Data[i + 1] == '+')
			{
				Data.erase(i + 1, 1);
				i = 0;
			}
		}
		else if (Data[i] == '-')
		{
			if (Data[i + 1] == '-')
			{
				Data.erase(i, 2);
				Data.insert(i, "+");
				i = 0;
			}
			else if (Data[i + 1] == '+')
			{
				Data.erase(i + 1, 1);
				i = 0;
			}
		}
		else if (Data[i] == '*')
		{
			if (Data[i + 1] == '+')
			{
				Data.erase(i + 1, 1);
				i = 0;
			}
			
		}
		else if (Data[i] == '/')
		{
			if (Data[i + 1] == '+')
			{
				Data.erase(i + 1, 1);
				i = 0;
			}
		}

		i++;
	}
}

float Operation(int elem, string &Data, string &Chars, bool Deg)
{
	int pos = Find_curr_pos(Data, Chars, elem);
	float res = 0;
	string a, b;

	if (pos == -1)
	{
		cout << "error";
	}
	//менять местами а и b запрещено !!!
	b = Find_next_elem(Data, pos, Chars, elem);
	a = Find_prev_elem(Data, pos, Chars, elem);
	
	switch ((int)Chars[elem])
	{
	case 42:
		// "*"
		res = atof(a.c_str()) * atof(b.c_str());
		break;
	case 43:
		// "+"
		res = atof(a.c_str()) + atof(b.c_str());
		break;
	case 45:	
		// "-"
		/*if (a[0] != ' ')
		{*/
			res = atof(a.c_str()) - atof(b.c_str());
		/*}
		else
		{
			res = -1 * atof(b.c_str());
		}*/
		break;
	case 47:	
		// "/"
		res = atof(a.c_str()) / atof(b.c_str());
		break;
	case 115://sin
		if (!Deg)
		{
			res = sin(atof(b.c_str()));
		}
		else
		{
			res = sin(atof(b.c_str()) * PI/180);
		}
		break;
	case 99://cos
		if (!Deg)
		{
			res = cos(atof(b.c_str()));
		}
		else
		{
			res = cos(atof(b.c_str()) * PI / 180);
		}
		break;
	case 116://tg
		if (!Deg)
		{
			res = sin(atof(b.c_str())) / cos(atof(b.c_str()));
		}
		else
		{
			res = sin(atof(b.c_str()) * PI / 180) / cos(atof(b.c_str()) * PI / 180);
		}
		
		break;
	case 103://ctg
		if (!Deg)
		{
			res = cos(atof(b.c_str()) * PI / 180) / sin(atof(b.c_str()) * PI / 180);
		}
		else
		{
			res = cos(atof(b.c_str())) / sin(atof(b.c_str()));
		}
		break;
	case 94:// ^
		res = pow(atof(a.c_str()),atof(b.c_str()));
		break;
	case 118:// sqrt
		res = pow(atof(b.c_str()),0.5);
		break;
	default:
		break;
	}

	a = " " + to_string(res);
	Data.insert(pos, a);

	Chars.erase(elem, 1);

	return res;
}

int Find_Chars(string Chars)
{
	int pos = 0, i = 0, j = 0, temp;
	int s = 0, c = 0, g = 0, t = 0, v = 0;
	int mass[6];
	mass[0] = (int)Chars.find('s');
	mass[1] = (int)Chars.find('c');
	mass[2] = (int)Chars.find('t');
	mass[3] = (int)Chars.find('g');
	mass[4] = (int)Chars.find('v');
	mass[5] = (int)Chars.find('^');

	for (i = 0; i < 5 ; i++) 
	{
		for (j = 0; j < 5 - i; j++) 
		{
				temp = mass[j];
				mass[j] = mass[j + 1];
				mass[j + 1] = temp;
		}
	}

	for (i = 0; i < 6; i++)
	{
		if (mass[i] != -1)
		{
			pos = mass[i];
			break;
		}
	}


	return pos;
}

float Сalculate(string & Data, string & Chars, int count, bool Deg)
{
	int i = 0, j = 0, k = 0;
	float result = 0;

	if (Chars.find('s') != -1 or Chars.find('c') != -1 or
		Chars.find('t') != -1 or Chars.find('g') != -1 or
		Chars.find('v') != -1 or Chars.find('^') != -1)
	{
		i = Find_Chars(Chars);
		Operation(i, Data, Chars, Deg);

	}
	else if (Chars.find('*') != -1 or Chars.find('/') != -1)
	{
		i = Chars.find('*');
		j = Chars.find('/');

		if (j != -1 and i != -1)
		{
			if (i < j)
			{
				Operation(i, Data, Chars, Deg);
			}
			else
			{
				Operation(j, Data, Chars, Deg);
			}
		}
		else if (j != -1)
		{
			Operation(j, Data, Chars, Deg);
		}
		else
		{
			Operation(i, Data, Chars, Deg);
		}
	}
	else
	{
		Operation(0, Data, Chars, Deg);
	}

	return result;
}

int BKT(string &Data, string Chars, bool Deg)
{
	int i = Data.find('(') + 1;
	int counter = 0, bkt_counter = 1, temp = 0;
	string bkt_Data;

	Bugs(Data);
	bkt_Data = Data;

	if (i != 0)
	{
		bkt_Data.clear();
		while (1)
		{
			if (Data[i + counter] == '(')
			{
				bkt_counter++;
			}
			else if (Data[i + counter] == ')')
			{
				bkt_counter--;
			}
			
			if (Data[i + counter] == ')' && bkt_counter == 0)
			{
				break;
			}
			bkt_Data += Data[i + counter];
			counter++;

		}

		bkt_Data.insert(0, " ");
		temp = bkt_Data.length();
		bkt_Data.insert(temp, " ");

		BKT(bkt_Data, Chars, Deg);
	}
	else
	{
		Start(bkt_Data, Chars, Deg);
	}

	if (i != 0)
	{
		bkt_Data.erase(0, 1);
		bkt_Data.erase(bkt_Data.length() - 1, 1);
		Data.erase(i - 1, counter + 2);
		Data.insert(i - 1, bkt_Data);
		BKT(Data, Chars, Deg);
	}
	else
	{
		Data = bkt_Data;
	}
	return i;
}

bool bkt_check(string Data)
{
	int bkt_counter = 0, i = 0;

	while (Data[i] != NULL)
	{
		if (Data[i] == '(')
		{
			bkt_counter++;
		}
		else if (Data[i] == ')')
		{
			bkt_counter--;
		}
		i++;
	}

	if (bkt_counter == 0)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void Start(string &Data, string &Chars, bool Deg)
{
	int i = 0;

	if (Sort(Data, Chars) != 0)
	{
		while (1 != Capacity(Data))
		{
			Сalculate(Data, Chars, i, Deg);
		}
	}
	
}
//"-(-144.12/-(12.112*-14) + 11.64235/2) * -60 * -(-3 )="
int main()
{
	string Data = "sin(sin(cos(60)))=", Chars;
	int i = 1;
	float res = 0.0;
	bool Deg = true;

	//getline(cin, Data);
	Data = correct(Data);
	if (!bkt_check(Data))
	{
		cout << "Error : missed '(' or ')'";
		system("pause");
		return -1;
	}
	while (i != 0)
	{
		 i = BKT(Data, Chars, Deg);
	}
	
	cout << Data;
	system("pause"); 
	return 0;
}