#include <iostream>
#include <string>

using namespace std;

float Сalculate(string &Data, string &Chars, int count);//сам подчёт с учётом '*' и '/'
string correct(string Data);//убирает лишние пробелы и знак '=' в конце
int Sort(string &Data, string &Chars);//модуль для отделения знаков и чисел


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
		if (Data[i] == '+' or Data[i] == '-' or Data[i] == '/' or Data[i] == '*')
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

int Find_curr_pos(const string Data, const string Chars, int elem)//возвращает позицию в Data
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



float Operation(int elem, string &Data, string &Chars)//для обработки двух чисел
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


float Сalculate(string & Data, string & Chars, int count)//модуль для поиска операций по приоритету
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

int main()
{
	string Data = "144/12*14 + 11/2 * 44=", Chars;
	int i = 0;
	float res = 0.0;

	getline(cin, Data);
	Data = correct(Data);
	Sort(Data, Chars);
	while (1 != Capacity(Data))
	{
		Сalculate(Data, Chars, i);
	}
	cout << Data;
	system("pause"); 
	return 0;
}


