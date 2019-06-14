#include <iostream>
#include <cstdlib>	//для system()
#include <conio.h>		//для паузы while (!_kbhit()) 
#include <Windows.h>	//для setconsole
#include <iomanip>	//для setprecision
#include <fstream>	//для работы с файлами
using namespace std;
struct abonent
{
	char fname[15];
	char sname[15];
	double number;
	char adress[20];
	unsigned int lgota;
	unsigned int dolg;
};
struct list
{
	abonent field;
	struct list* next;
};

struct list* INIT(abonent info)
{	//инициализация списка и заполнение корневого узлоа
	struct list* lst;
	lst = new list;		// выделение памяти под корень списка
	lst->field = info;		//заполнение поля корневого узла
	lst->next = lst;		// указатель на сам корневой узел
	return(lst);
}

struct list* ADDEL(list* lst, abonent info)
{	//добавление узла
	struct list* temp, * p;
	temp = new list;
	p = lst->next;	// сохранение указателя на следующий элемент
	lst->next = temp;	// предыдущий узел указывает на создаваемый
	temp->field = info;	 // сохранение поля данных добавляемого узла
	temp->next = p;		// созданный узел указывает на следующий элемент
	return(temp);
};

void RAMKA()
{	//рамка для вывода
	cout.setf(ios::left | ios::fixed); //устанавлиаем флаги, благодаря которому width выравнивает записи по левой границе, а double выводятся в обычном виде
	cout << "\n ";
	cout << " ==================================================================================================================================\n";
	cout << "  |№\t|";	cout.width(23);	cout << "Фамилия абонента";	cout.width(24);	cout << "|Имя абонента"; cout.width(16); cout << "|Номер телефона";	cout.width(26);
	cout << "|Адрес абонента";	cout.width(14);	cout << "|Льготы(RUB)"; cout.width(15); cout << "|Задолженность(RUB)" << "|\n";
	cout << "  ==================================================================================================================================\n";
}

void INFOPRINT(list* lst, int N)
{		//вывод определенной записи
	cout << "  |";		cout.width(5);		cout << N << "|";
	cout.width(23);		cout << lst->field.fname << "|";
	cout.width(23);		cout << lst->field.sname << "|";
	cout.width(15);		cout << setprecision(0) << lst->field.number << "|";	//устанавливаем количество цифр после запятой для double
	cout.width(25);		cout << lst->field.adress << "|";
	cout.width(13);		cout << lst->field.lgota << "|";
	cout.width(18);		cout << lst->field.dolg << "|\n";
}

void LISTPRINT(list* lst, list* end, int count)
{	//вывод списка
	struct list* p;
	int i = 0;
	if (lst != NULL)
	{
		lst = end->next;	//список в начало
		p = lst;
		RAMKA();
		do
		{
			i++;
			INFOPRINT(p, i);
			p = p->next;		// переход к следующему узлу
		} while (p != lst);		// условие окончания обхода
		cout << "  ==================================================================================================================================\n\n";
		cout << "Количество записей: " << count << "\n";
	}
	else cout << "Список пуст\n";
}

struct list* DELEL(list* lst)
{	//удаление узла
	struct list* temp;
	bool f = false;
	temp = lst;
	while (temp->next != lst)  // просматриваем список начиная с корня
		temp = temp->next;	// пока не найдем узел, предшествующий lst
	if (temp == temp->next)		f = true;	//если остался 1 узел
	temp->next = lst->next; // переставляем указатель
	delete lst; // освобождаем память удаляемого узла
	if (f == true)		temp = NULL;	//указываем, что lst - пустой
	return temp;
}
//============================================
bool XCHANGE(list* sled_sort, list* sort, list* lst, list* end, bool f1)
{	//часть сортировки, меняющая местами sort 
	if (sled_sort == end)		 f1 = true;	//если последняя запись меняется, то нужно изменить и end
	lst->next = sled_sort;		//меняем местами указатели sort(i узел) с sled_sort(i+1 узел)
	sort->next = sled_sort->next;
	sled_sort->next = sort;
	return f1;
}

void main()
{
	int  c1, c2, c3, count = 0, k, N;
	unsigned int min, max, sum;
	struct list* lst = NULL, * end = NULL, * sled_sort, * sort;
	bool f1 = false, f2 = false, f3 = false;
	const char* file = "list.dat";
	abonent atc;	//структура
	ofstream fout;	//объект класса ofstream для записи в файл 
	ifstream fin;	////объект класса ifstream для чтения из файла
	SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода(аналог setlocale(LC_ALL, "Russian"))
	SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода(для корректного вывода русских символов)
	do
	{
		system("cls");	//очистка экрана
		cout << "Главное меню\n";
		cout << "1. Создание файла и ввод информации\n";
		cout << "2. Вывод всех записей на экран в виде таблицы\n";
		cout << "3. Редактирование записей\n";
		cout << "4. Удаление записей\n";
		cout << "5. Удаление списка\n";
		cout << "6. Сортировка списка\n";
		cout << "7. Поиск по ключу\n";
		cout << "0. Закрыть программу\n";
		cin >> c1;
		system("cls");
		switch (c1)
		{
		case 1:
			system("cls");
			cout << "1. Сохранить информацию в файл\n";
			cout << "2. Считать информацию из файла\n";
			cout << "3. Добавить запись в список\n";
			cout << "0. Главное меню\n";
			cin >> c2;
			system("cls");
			switch (c2)
			{
			case 1:
				if (lst != NULL)
				{
					lst = end;				//в конец списка
					fout.open(file, ios_base::out | ios_base::trunc | ios_base::binary);
					do			//открыли файл бинарным образом для записи, перезапишем, если существует
					{
						lst = lst->next;
						atc = lst->field;
						fout.write((char*)& atc, sizeof atc);	//записываем поочередно информацию в файл
					} while (lst->next != end->next);
					fout.close();	//закрываем файл
				}
				else { cout << "Список пуст\n";		while (!_kbhit()); }
				break;
			case 2:
				fin.clear();	//восстанавливает ввод, если он окажется в ошибочном состоянии
				fin.open(file, ios_base::in || ios::binary);	//открыли файл бинарным образом для чтения
				while (fin.read((char*)& atc, sizeof atc))	//пока считывается информация из файла,	будем заполнять ей узлы списка
				{
					if (lst != NULL)
					{
						count++;
						lst = end;
						lst = ADDEL(lst, atc);
						end = lst;
					}
					if (lst == NULL)
					{
						count++;
						lst = INIT(atc);
						end = lst;
					}
				}
				fin.close();	//закрываем файл
				break;
			case 3:
				if (lst != NULL)	//если список имеется и содержит записи, то выводим
				{
					cout << "Список найден\n";		LISTPRINT(lst, end, count);
				}
				else cout << "Список пуст\n";
				cout << "Сколько записей нужно добавить?\n";
				cin >> k;		cout << "\n";
				if (k > 0)
				{
					if (lst == NULL)	//если списка нет, то создаем и вводим корневой узел списка
					{
						k--;
						count++;	//количество записей
						cout << "Запись " << count << "\n";
						cin.ignore();	//функция решает проблему того, что после cin cin.getline пропускает ввод данных, отбрасывая ввод до новой строки 
						cout << "    Фамилия: ";		cin.getline(atc.fname, 20);
						cout << "    Имя: ";			cin.getline(atc.sname, 20);
						cout << "    Номер: ";			cin >> atc.number;		cin.ignore();
						cout << "    Адрес: ";			cin.getline(atc.adress, 40);
						cout << "    Льготы: ";			cin >> atc.lgota;
						cout << "    Задолженность: ";	cin >> atc.dolg;
						lst = INIT(atc);	//создание списка
						end = lst;	//сохраняем конец списка
					}
					if (lst != NULL)	//если список имеется и содержит хотя бы 1 узел, то вводим последующие узлы 
					{
						lst = end;		//в конец списка
						while (k > 0)	//добавляем узлы в список столько, сколько задано в k
						{
							k--;
							count++;
							cout << "Запись " << count << "\n";
							cin.ignore();
							cout << "    Фамилия: ";		cin.getline(atc.fname, 20);
							cout << "    Имя: ";			cin.getline(atc.sname, 20);
							cout << "    Номер: ";			cin >> atc.number;	cin.ignore();
							cout << "    Адрес: ";			cin.getline(atc.adress, 40);
							cout << "    Льготы: ";			cin >> atc.lgota;
							cout << "    Задолженность: ";	cin >> atc.dolg;
							lst = ADDEL(lst, atc);	//добавление узла в конец списка
							end = lst;	//переназначаем конец
						}
					}
				}
				else cout << "Ошибка\n";
				break;
			}
			break;
			//==============================================
		case 2:
			LISTPRINT(lst, end, count);
			if (lst != NULL)
			{
				lst = end;
				min = lst->field.lgota;
				max = lst->field.lgota;
				sum = 0;
				do
				{
					lst = lst->next;
					sum += lst->field.lgota;
					if (lst->field.lgota > max)		max = lst->field.lgota;
					if (lst->field.lgota < min)		min = lst->field.lgota;
				} while (lst->next != end->next);
				cout << "Наибольший льготный показатель составляет " << max << " рублей\n";
				cout << "Наименьший льготный показатель составляет " << min << " рублей\n";
				cout << "Суммарный льготный показатель составляет " << sum << " рублей\n";
				cout << "Средний льготный показатель составляет " << sum / count << " рублей\n\n";
				lst = end;
				min = lst->field.dolg;
				max = lst->field.dolg;
				sum = 0;
				do
				{
					lst = lst->next;
					sum += lst->field.dolg;
					if (lst->field.dolg > max)		max = lst->field.dolg;
					if (lst->field.dolg < min)		min = lst->field.dolg;
				} while (lst->next != end->next);
				cout << "Наибольший показатель задолженности составляет " << max << " рублей\n";
				cout << "Наименьший показатель задолженности составляет " << min << " рублей\n";
				cout << "Суммарный показатель всех задолженностей составляет " << sum << " рублей\n";
				cout << "Средний показатель задолженностей составляет " << sum / count << " рублей\n\n";
			}
			while (!_kbhit());			//пауза в консоли, пока не нажата любая кнопка
			break;
			//==============================================
		case 3:
			if (lst != NULL)
			{
				LISTPRINT(lst, end, count);
				cout << "\n" << "Введите порядковый номер изменяемой записи: ";
				cin >> N;
				system("cls");
				if ((N > 0) && (N <= count))
				{
					lst = end;  // в конец списка, откуда будем двигаться до N записи 
					for (int i = 0; i < N; i++)
						lst = lst->next;
					do
					{
						RAMKA();
						INFOPRINT(lst, N);
						cout << "  ==================================================================================================================================\n\n";
						cout << "1. Изменить запись полностью\n";
						cout << "2. Изменить фамилию\n";
						cout << "3. Изменить имя\n";
						cout << "4. Изменить номер телефона\n";
						cout << "5. Изменить адрес\n";
						cout << "6. Изменить сумму льгот\n";
						cout << "7. Изменить сумму задолженности\n";
						cout << "0. Главное меню\n\n";
						cout << "Введите номер операции: ";
						cin >> c2;
						system("cls");
						RAMKA();
						INFOPRINT(lst, N);
						cout << "  ==================================================================================================================================\n\n";
						cin.ignore();
						if (c2 == 1)
						{
							cout << "    Фамилия: ";		cin.getline(atc.fname, 15);
							cout << "    Имя: ";			cin.getline(atc.sname, 15);
							cout << "    Номер: ";			cin >> atc.number;	cin.ignore();
							cout << "    Адрес: ";			cin.getline(atc.adress, 20);
							cout << "    Льготы: ";			cin >> atc.lgota;
							cout << "    Задолженность: ";	cin >> atc.dolg;
							lst->field = atc;
						}
						else
						{
							atc = lst->field;
							cout << "Введите новое значение: ";
							switch (c2)
							{
							case 2:	cin.getline(atc.fname, 15);		strcpy_s(lst->field.fname, atc.fname);		break;
							case 3:	cin.getline(atc.sname, 15);		strcpy_s(lst->field.sname, atc.sname);		break;
							case 4:	cin >> atc.number;				lst->field.number = atc.number;				break;
							case 5:	cin.getline(atc.adress, 20);	strcpy_s(lst->field.adress, atc.adress);	break;
							case 6:	cin >> atc.lgota;				lst->field.lgota = atc.lgota;				break;
							case 7:	cin >> atc.dolg;				lst->field.dolg = atc.dolg;					break;
							}
						}
						system("cls");
					} while (c2 != 0);
				}
				else { cout << "Ошибка. Такого порядкового номера не существует\n";	while (!_kbhit()); }
			}
			else { cout << "Список пуст\n";	while (!_kbhit()); }
			break;
			//==========================================
		case 4:
			if (lst != NULL)
			{
				LISTPRINT(lst, end, count);
				cout << "\n" << "Введите порядковый номер удаляемой записи: ";
				cin >> N;			cout << "\n";
				system("cls");
				if ((N > 0) && (N <= count))
				{
					lst = end;
					for (int i = 0; i < N; i++)		//поиск нужной записи
						lst = lst->next;
					lst = DELEL(lst);	//удаляем нужную запись
					if (N == count)		end = lst; //если удаляем последнюю, то перезаписываем end
					count--;		//понижение количества записей
					LISTPRINT(lst, end, count);
				}
				else
				{
					system("cls");		cout << "Ошибка. Такого порядкового номера не существует\n";	while (!_kbhit());
				}
			}
			else cout << "Список пуст\n";
			while (!_kbhit());
			break;
			//==============================================
		case 5:
			if (lst != NULL)
			{
				lst = end;
				while (lst != NULL)	//удаляем пока не закончится список
				{
					lst = DELEL(lst);
					count--;
				}
				end = lst;
				cout << "Список удален\n";
			}
			else cout << "Список пуст\n";		while (!_kbhit());
			break;
			//==========================================
		case 6:		//Сортировка пузырьком
			if (lst != NULL)
			{
				system("cls");
				cout << "1. Сортировать по возрастанию\n";
				cout << "2. Сортировать по убыванию\n";
				cout << "0. Главное меню\n";
				cin >> c2;
				if ((c2 > 0) && (c2 < 3))
				{
					system("cls");
					cout << "1. Сортировать по фамилиям\n";
					cout << "2. Сортировать по именам\n";
					cout << "3. Сортировать по суммам льгот\n";
					cout << "4. Сортировать по суммам задолженностей\n";
					cout << "0. Главное меню\n";
					cin >> c3;				//будем сравнивать i элемент цикла с i+1 элементом
					system("cls");		//в случае выполнения условия будем менять местами i и i+1 узлы списка		
					for (int j = 1; j <= count - 1; j++)
					{
						lst = end;	//i-1 элемент
						sort = lst->next;	//i элемент
						sled_sort = sort->next;	//i+1 элемент
						for (int i = 1; i <= count - j; i++)
						{
							if (c2 == 1)		//рассматриваем условие по взрастанию
								switch (c3)
								{
								case 1:
									if (strcmp(sort->field.fname, sled_sort->field.fname) > 0)
										f1 = XCHANGE(sled_sort, sort, lst, end, f1);
									else sort = sort->next;
									break;
								case 2:
									if (strcmp(sort->field.sname, sled_sort->field.sname) > 0)
										f1 = XCHANGE(sled_sort, sort, lst, end, f1);
									else sort = sort->next;
									break;
								case 3:
									if (sort->field.lgota > sled_sort->field.lgota)
										f1 = XCHANGE(sled_sort, sort, lst, end, f1);
									else sort = sort->next;
									break;
								case 4:
									if (sort->field.dolg > sled_sort->field.dolg)
										f1 = XCHANGE(sled_sort, sort, lst, end, f1);
									else sort = sort->next;
									break;
								}
							if (c2 == 2)		//рассматриваем условие по убыванию
								switch (c3)
								{
								case 1:
									if (strcmp(sort->field.fname, sled_sort->field.fname) < 0)
										f1 = XCHANGE(sled_sort, sort, lst, end, f1);
									else sort = sort->next;
									break;
								case 2:
									if (strcmp(sort->field.sname, sled_sort->field.sname) < 0)
										f1 = XCHANGE(sled_sort, sort, lst, end, f1);
									else sort = sort->next;
									break;
								case 3:
									if (sort->field.lgota < sled_sort->field.lgota)
										f1 = XCHANGE(sled_sort, sort, lst, end, f1);
									else sort = sort->next;
									break;
								case 4:
									if (sort->field.dolg < sled_sort->field.dolg)
										f1 = XCHANGE(sled_sort, sort, lst, end, f1);
									else sort = sort->next;
									break;
								}
							if (f1 == true)
							{	//если последний узел стал предпоследним, то перезаписываем новый end
								end = sort;
								f1 = false;
							}
							lst = lst->next;		//двигаемся по циклу дальше
							sled_sort = sort->next;
						}
					}
					LISTPRINT(lst, end, count);
					while (!_kbhit());
				}
			}
			else { cout << "Список пуст\n";	while (!_kbhit()); }
			break;
		case 7:
			if (lst != NULL)
			{
				system("cls");
				cout << "1. Поиск по фамилии\n";
				cout << "2. Поиск по имени\n";
				cout << "3. Поиск по номеру телефона\n";
				cout << "4. Поиск по адресу\n";
				cout << "5. Поиск по сумме льгот\n";
				cout << "6. Поиск по сумме задолженности\n";
				cout << "0. Главное меню\n\n";
				cout << "Введите критерий поиска: ";
				cin >> c2;
				N = 0;		//номер найденных записей
				lst = end;	//в конец списка
				cout << "Введите значение: ";
				cin.ignore();
				switch (c2)
				{
				case 1:		cin.getline(atc.fname, 15);		break;
				case 2:		cin.getline(atc.sname, 15);		break;
				case 3:		cin >> atc.number;				break;
				case 4:		cin.getline(atc.adress, 15);	break;
				case 5:		cin >> atc.lgota;				break;
				case 6: 	cin >> atc.dolg;				break;
				}
				system("cls");
				RAMKA();
				do {
					lst = lst->next;
					N++;
					switch (c2)
					{
					case 1:	if (strcmp(atc.fname, lst->field.fname) == 0)	INFOPRINT(lst, N);	break;
					case 2:	if (strcmp(atc.sname, lst->field.sname) == 0)	INFOPRINT(lst, N);	break;
					case 3:	if (atc.number == lst->field.number)			INFOPRINT(lst, N);	break;
					case 4:	if (strcmp(atc.adress, lst->field.adress) == 0)	INFOPRINT(lst, N);	break;
					case 5:	if (atc.lgota == lst->field.lgota)				INFOPRINT(lst, N);	break;
					case 6: if (atc.dolg == lst->field.dolg)				INFOPRINT(lst, N);	break;
					}
				} while (lst->next != end->next);
				cout << "  ==================================================================================================================================\n\n";
				if ((c2 >= 1) && (c2 <= 6)) while (!_kbhit());	//избегаем паузы, если введено неверное c2
			}
			else { cout << "Список пуст\n";	while (!_kbhit()); }
			break;
		}
	} while (c1 != 0);
	system("pause");
}