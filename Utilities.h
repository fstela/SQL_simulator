#pragma once
#include<iostream>
#include<string>
#include<fstream>

using namespace std;


class Useful{
public:
	Useful() {};
	virtual bool verifyIfExists(char* tableName)
	{
		bool exists = 0;
		fstream raport("listOfTables.txt");
		if (raport.is_open())
		{
			while (!raport.eof())
			{
				string nameOfExistingTable;
				char buffer[1000];
				raport.getline(buffer, 1000);
				nameOfExistingTable = string(buffer);
				if (nameOfExistingTable.find(tableName) >= 0 && nameOfExistingTable.find(tableName)<=nameOfExistingTable.size())
				{
					exists = 1;
				}
			}
			raport.close();
		}
		return exists;

	}
};
class RemoveSpaces {
public:
	RemoveSpaces(){}
	virtual void removeSpaces(string nouaComanda)
	{
		string tempS = "";
		string const delim{ " " };		
		int beginning, pos = 0;
		while ((beginning = nouaComanda.find_first_not_of(delim, pos)) != string::npos)
		{
			pos = nouaComanda.find_first_of(delim, beginning);
			if (pos == -1 && beginning >= 0)
			{

				int length = nouaComanda.size();
				tempS = tempS.append(nouaComanda.substr(beginning, length - beginning));
			}
			else
				tempS = tempS.append(nouaComanda.substr(beginning, pos - beginning));
		}
		nouaComanda = tempS;
	}

};
class Parse{
public:
	Parse() {}
	virtual void createParse() = 0;
	virtual void receiveCommand(string comanda) = 0;
};
class DataManipCommands
{
protected:
	string nouaComanda = "";
	char* tableName = nullptr;
	int sizeTableName = 0;
	string allFromColumn = "";
	string currentColumn = "";
	string currentType = "";
	string columnBinary = "";
	bool exists = 0;

public:
	DataManipCommands() {

	}
	void crudCommands() {
		/*DataManipCommands* Crud_Commands[3];
		SelectCommand sc1;
		DeleteCommand dc1;
		UpdateCommand uc1;
		Crud_Commands[0] = &sc1;
		Crud_Commands[1] = &dc1;
		Crud_Commands[2]*/
	}
	~DataManipCommands() {

	}
};
class mainBase {
public:
	mainBase(){}
	~mainBase(){}

};
