
// Client //

#include <iostream>
//#include "stdafx.h"

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>

#include <fstream>
#include <string>

#pragma warning(disable: 4996) // Для ошибки в 25 inet_addr("127.0.0.1")

using namespace std;

SOCKET Connection;

void ClientHandler()
{
	char msg[256];
	while (true)
	{
		recv(Connection, msg, sizeof(msg), NULL);
		cout << "< " << msg << endl;
	}
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru");

	//WSAStartup
	WSAData wsaData;	// Структура
	WORD DLLVersion = MAKEWORD(2, 2);		// Переменная, версия

	if (WSAStartup(DLLVersion, &wsaData) != 0)		// Версия, ссылка
	{
		std::cout << "Error load" << std::endl;
		exit(1);
	}

	cout << "Starting..." << endl;

	// Адресс сокета
	SOCKADDR_IN addr;		// Хранине адреса
	int sizeofadder = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");		// Айпи хранит

	string port;
	ifstream file;
	file.open("port.dll");
	if (!file.is_open())
	{
		cout << "Error open file !" << endl;
	}
	// Read lines
	while (!file.eof())		// eof = End text (end)
	{
		getline(file, port);
	}

	// String in int
	int portInt = stoi(port);
	//int portInt = stoi(port.c_str());

	addr.sin_port = htons(portInt);		// Порт (почти любой)
	addr.sin_family = AF_INET;		// Симество протаколов (надо писать)


	// Сокет для соединиения
	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	// Проверка на соединение
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
		// Сокет, указатель на структуру, размер стрктуры сокаадр
	{
		std::cout << "Error: failed connect to server." << std::endl;
		return 1;
		//exit(1);
	}

	// Если  все ок

	std::cout << "Connected!" << std::endl;

	char msg2[256];
	cin >> msg2;

	send(Connection, msg2, sizeof(msg2), NULL);

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	char msg[256];
	while(true)
	{
		cin.getline(msg, sizeof(msg));
		send(Connection, msg, sizeof(msg), NULL);
		Sleep(10);
	}
	



	system("pause");
	return 0;
}