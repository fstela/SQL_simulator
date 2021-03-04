#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include "Utilities.h"
#include "insert.h"
#include "create.h"
#include "delete.h"
#include "display.h"
#include "drop.h"
#include "select.h"
#include "update.h"
#include "ImportCSV.h"

using namespace std;

class CommandsFromFile:public DataManipCommands
{

public:
	CommandsFromFile() {}
	void forEachArgFile(const char* filename) {
		this->ReadCommands(filename);
	}

	void ReadCommands(const char* filename)
	{
		fstream inputFile;
		inputFile.open(filename, ios::in);
		if (inputFile.is_open())
		{
			string command;
			while (!inputFile.eof())
			{
				char buffer[1000];
				command = "";

				inputFile.getline(buffer, 1000);
				command = string(buffer);
				if (command.find("CREATE TABLE") != string::npos)
				{
					CreateCommand* cc1 = new CreateCommand();
					cc1->receiveCommand(command);
					cout << *cc1;
					delete cc1;
				}
				else if (command.find("INSERT INTO") != string::npos) {
					InsertCommand* ic1 = new InsertCommand();
					ic1->receiveCommand(command);
					delete ic1;
				}
				else if (command.find("SELECT") != string::npos) {
					SelectCommand* sc1 = new SelectCommand();
					sc1->receiveCommand(command);
					delete sc1;
				}
				else if (command.find("DISPLAY") != string::npos) {
					DisplayCommand* dc1 = new DisplayCommand();
					dc1->receiveCommand(command);
					delete dc1;
				}
				else if (command.find("DROP") != string::npos) {
					DropCommand* dc1 = new DropCommand();
					dc1->receiveCommand(command);
					delete dc1;
				}
				else if (command.find("UPDATE") != string::npos) {
					UpdateCommand* up1 = new UpdateCommand();
					up1->receiveCommand(command);
					delete up1;
				}
				else if (command.find("DELETE FROM") != string::npos) {
					DeleteCommand* del1 = new DeleteCommand();
					del1->receiveCommand(command);
					delete del1;
				}
			}
			inputFile.close();
		}
		else
		{
			cout << endl
				<< "File not opened";
		}
	}
	~CommandsFromFile() {}
};
class CommandProcessor:public DataManipCommands
{
	static const int tableNameLengthConstraing;

public:
	CommandProcessor(){}

	string comanda;
	void commandReader()
	{
		cin.ignore();
		getline(cin, comanda);
		if (comanda.find(CommandProcessor::CREATE) == 0)
		{
			CreateCommand cc1;
			cc1.receiveCommand(comanda);
			cout << cc1;
		}
		else
		{
			if (comanda.find(CommandProcessor::DROP) == 0)
			{
				DropCommand dc1;
				dc1.receiveCommand(comanda);
				cout << dc1;
			}
			else
			{
				if (comanda.find(CommandProcessor::DISPLAY) == 0)
				{
					DisplayingConsole dis1;
					dis1.receiveCommand(comanda);
					cout << dis1;

				}
				else
				{
					if (comanda.find(CommandProcessor::INSERTINTO) == 0)
					{
						InsertingValues ic1;
						ic1.receiveCommand(comanda);
						cout << ic1;
					}
					else
					{
						if (comanda.find(CommandProcessor::DELETE) == 0)
						{
							DeleteCommand dlt1;
							dlt1.receiveCommand(comanda);
							cout << dlt1;
						}
						else
						{
							if (comanda.find(CommandProcessor::SELECT) == 0)
							{
								SelectCommand sc1;
								sc1.receiveCommand(comanda);
								cout << sc1;
							}
							else
							{
								if (comanda.find(CommandProcessor::UPDATE) == 0)
								{
									UpdateCommand uc1;
									uc1.receiveCommand(comanda);
									cout << uc1;
								}
								else {
									if (comanda.find(CommandProcessor::IMPORT) == 0)
									{
										InsertCSVCommand icc1;
										icc1.receiveCommand(comanda);
										cout << icc1;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	static string CREATE;
	static string DROP;
	static string DISPLAY;
	static string DELETE;
	static string UPDATE;
	static string SELECT;
	static string INSERTINTO;
	static string IMPORT;
};

const int CommandProcessor::tableNameLengthConstraing = 2;


class UserInterface
{
	CommandProcessor cp1;
	CommandsFromFile cff;

public:
	UserInterface(char* fileName) {
		this->Title(fileName);
	}
	void Title(char* fileName)
	{
		cout << R"(

 /$$      /$$ /$$     /$$
| $$$    /$$$|  $$   /$$/
| $$$$  /$$$$ \  $$ /$$/
| $$ $$/$$ $$  \  $$$$/
| $$  $$$| $$   \  $$/
| $$\  $ | $$    | $$
| $$ \/  | $$    | $$
|__/     |__/    |__/
  /$$$$$$   /$$$$$$  /$$
 /$$__  $$ /$$__  $$| $$
| $$  \__/| $$  \ $$| $$
|  $$$$$$ | $$  | $$| $$
 \____  $$| $$  | $$| $$
 /$$  \ $$| $$/$$ $$| $$
|  $$$$$$/|  $$$$$$/| $$$$$$$$
 \______/  \____ $$$|________/
                \__/

)" << endl;

		cout << endl
			<< "         Welcome to our SQL Database. Please choose from the following options:" << endl;
		int option = 0;
		while (option != 4) {
			cout << endl
				<< "[ 1 ] Enter a command";
			cout << endl
				<< "[ 2 ] See the syntax";
			cout << endl
				<< "[ 3 ] Import command files";
			cout << endl
				<< "[ 4 ] Close the application \n"
				<< endl;

			//int option;
			cin >> option;
			menu(option, fileName);
		}
	}

	void menu(int option, char* fileName)
	{

		switch (option)
		{
		case 1:
		{
			cout << endl
				<< "Please enter a command" << endl;
			cp1.commandReader();
			break;
		}
		case 2:
			cout << endl
				<< "The syntax for the commands" << endl;
			cout<< "CREATE TABLE students ((id, integer, 1000, 0), (nume, text, 128, íí), (grupa, text,50,í1000í))\n";
			cout << "CREATE TABLE students IF NOT EXISTS((id, integer, 1000, 0), (nume, text, 128, íí), (grupa, text, 50, í1000í))\n";
			cout << "DROP TABLE studenti\n";
			cout << "DISPLAY TABLE studenti\n";
			cout << "INSERT INTO studenti VALUES(1, îJohnî, î1001î)\n";
			cout << "INSERT INTO produse VALUES(1, îPepsiî, 23.5, îsummerî)\n";
					cout <<"SELECT(nume) FROM studenti WHERE id = 1\n";
					cout <<"SELECT(id, nume, grupa)  FROM  studenti WHERE id = 1\n";
					cout << "SELECT(id, nume, grupa)  FROM  studenti\n";
					cout << "SELECT ALL FROM studenti WHERE id = 1\n";
						cout << "SELECT ALL FROM studenti\n";
						cout << "DELETE FROM studenti WHERE nume = ìJohnî\n";
						cout << "UPDATE studenti SET nume = ìAliceî WHERE id = 1\n";
						cout << "UPDATE studenti SET nume = ìAliceî WHERE nume = ìBiancaî\n";

			break;
		case 3:
		{
			cout << endl
				<< "We will import your text file" << endl;
			cff.forEachArgFile(fileName);
			break;
		}
		case 4:
		{
			cout << endl
				<< "Goodbye!"
				<< endl;
			break;
		}

		default:
		{
			cout << endl
				<< "Please choose a valid option \n"
				<< endl;
			cin >> option;
			menu(option, fileName);
			break;
		}
		}
	}
	friend ostream& operator<<(ostream& console, UserInterface& ui1);
};
ostream& operator<<(ostream& console, UserInterface& ui1)
{
	return console;
}
string CommandProcessor::CREATE = "CREATE";
string CommandProcessor::DROP = "DROP";
string CommandProcessor::DISPLAY = "DISPLAY";
string CommandProcessor::DELETE = "DELETE";
string CommandProcessor::UPDATE = "UPDATE";
string CommandProcessor::SELECT = "SELECT";
string CommandProcessor::INSERTINTO = "INSERT INTO";
string CommandProcessor::IMPORT = "IMPORT";

