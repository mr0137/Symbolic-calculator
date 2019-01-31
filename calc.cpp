#include <iostream>
#include <string>

using namespace std;


string correct(string Data);							/*	
											Убирает лишние пробелы и знак '=' в конце.		
										*/
string Find_prev_elem(string &Data, int &pos);					/* 
											Находит операнд перед знаком операции основываясь
											на позиции самого знака. После "чистит" в "нужном
											месте" данные в строке Data.
										*/
string Find_next_elem(string &Data, int pos);					/*
											Работает аналогично _prev_.
											Фунции должны вызываться одна за другой в конкретном
											порядке:
												1. next;
												2. prev;
											При вызове в обратном порядке позиция для чистки next
											Будет не верной.
										*/
float Operation(int elem, string &Data, string &Chars);				/*
											Функция для выполнения операций над соседними
											операндами.
										*/
float Сalculate(string &Data, string &Chars, int count);			/*
											Подсчёт с учётом приоритетов операций.
										*/
int Sort(string &Data, string &Chars);						/*	
											Функция для отделения знаков операций и чисел.
										*/
int Capacity(const string Data);						/*
											Выдаёт количество операндов основываясь на пробелах.
										*/
int Find_curr_pos(const string Data, const string Chars, int elem); 		/*
											Возвращает позицию знака конкретной операции в строке 
											Data.
										*/
int BKT(string &Data, string Chars);						/*
											Функция выполняющая расстановку приоритетов по
											средству поиска символов "(" и ")".
											Выполняет рекурсивные действия.
											До конца не оптимизирована(иногда "накидывает" пару
											лишних уровней рекурсии).
										*/
void Start(string &Data, string &Chars);					/*
											Функция для вызова Sort и Calculate.
										*/
bool bkt_check(string Data);							/*
											Функция для выполнения операций над соседними
											операндами.
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

	if (Data[Data.length() - 1] == '=')
	{
		Data.resize(Data.length() - 1);
	}

	cout << Data << endl;

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
			Data[i] == '/' or Data[i] == '*' )
		{
			Chars = Chars + (char)Data[i];
			Data[i] = ' ';
			count++;
		}

		i++;
	}

	return count;
}

string Find_prev_elem(string &Data, int &pos)
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

string Find_next_elem(string &Data, int pos)
{
	string element;
	int i = pos + 1, counter = 1;

	while (Data[i] != ' ')
	{
		i++;
	}
	counter = fabs(pos - i);

	element = Data.substr(pos + 1, counter);
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

float Operation(int elem, string &Data, string &Chars)
{
	int pos = Find_curr_pos(Data, Chars, elem);
	float res = 0;
	string a, b;

	if (pos == -1)
	{
		cout << "error";
	}
	//менять местами а и b запрещено !!!
	b = Find_next_elem(Data, pos);
	a = Find_prev_elem(Data, pos);
	
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
		res = atof(a.c_str()) - atof(b.c_str());
		break;
	case 47:	
		// "/"
		res = atof(a.c_str()) / atof(b.c_str());
		break;
	default:
		break;
	}

	a = " " + to_string(res);
	Data.insert(pos, a);

	Chars.erase(elem, 1);

	return res;
}

float Сalculate(string & Data, string & Chars, int count)
{
	int i = 0, j = 0;
	float result = 0;

	if (Chars.find('*') != -1 or Chars.find('/') != -1)
	{
		i = Chars.find('*');
		j = Chars.find('/');
		if (j != -1 and i != -1)
		{
			if (i < j)
			{
				Operation(i, Data, Chars);
			}
			else
			{
				Operation(j, Data, Chars);
			}
		}
		else if (j != -1)
		{
			Operation(j, Data, Chars);
		}
		else
		{
			Operation(i, Data, Chars);
		}
	}
	else
	{
		Operation(0, Data, Chars);
	}

	return result;
}

int BKT(string &Data, string Chars)
{
	int i = Data.find('(') + 1;
	int counter = 0, bkt_counter = 1, temp = 0;
	string bkt_Data = Data;

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

		BKT(bkt_Data, Chars);
	}
	else
	{
		Start(bkt_Data, Chars);
	}

	if (i != 0)
	{
		bkt_Data.erase(0, 1);
		bkt_Data.erase(bkt_Data.length() - 1, 1);
		Data.erase(i - 1, counter + 2);
		Data.insert(i - 1, bkt_Data);
		BKT(Data, Chars);
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

void Start(string &Data, string &Chars)
{
	int i = 0;

	if (Sort(Data, Chars) != 0)
	{
		while (1 != Capacity(Data))
		{
			Сalculate(Data, Chars, i);
		}
	}
	
}

int main()
{
	string Data = "(144/(12*14) + 11/2) * 44 * ( 3 )=", Chars;
	int i = 1;
	float res = 0.0;

//	getline(cin, Data);
	Data = correct(Data);
	if (!bkt_check(Data))
	{
		cout << "Error : missed '(' or ')'";
		system("pause");
		return -1;
	}
	while (i != 0)
	{
		 i = BKT(Data, Chars);
	}
	
	cout << Data;
	system("pause"); 
	return 0;
}
