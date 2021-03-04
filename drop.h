#include <iostream>
using namespace std;

class DropCommand:public Useful,Parse,DataManipCommands
{
	string nouaComanda;
	char* indexName;
	char* tableName;
	string initialCommand;
	bool exists;
	static const int dropTableMinLength;

public:
	bool verifyIfExists(char* tableName) {
		bool temp = this->Useful::verifyIfExists(this->tableName);
		cout << "Searching for the table..";
		return temp;
	}

	void deleteExistingFile()
	{
		string NumeFisier = string(this->tableName);
		string NumeFisierBinar = string(this->tableName);
		NumeFisier = NumeFisier.append(".txt");
		NumeFisierBinar = NumeFisierBinar.append(".bin");
		remove(NumeFisier.c_str());
		remove(NumeFisierBinar.c_str());
	}
	void deleteTrace()
	{
		string oldname = "listofNewTables.txt";
		string newname = "listofTables.txt";
		string NumeCurent;
		fstream raport("listOfTables.txt");
		if (raport.is_open())
		{
			while (!raport.eof())
			{
				string nameOfExistingTable;
				char buffer[1000];
				raport.getline(buffer, 1000);
				nameOfExistingTable = string(buffer);
				if (nameOfExistingTable.find(this->tableName) != string::npos)
				{
					this->exists = 1;
				}
				else
				{
					ofstream raport("listOfNewTables.txt", ios::app);
					if (raport.is_open())
					{
						if (nameOfExistingTable != "")
							raport << nameOfExistingTable << '\n';
						raport.close();
					}
				}
			}
			raport.close();
			remove(newname.c_str());
			rename(oldname.c_str(), newname.c_str());
		}

		else
		{
			cout << "The file was not created.";
		}
	}

	DropCommand()
	{
		this->indexName = nullptr;
		this->tableName = nullptr;
	}

	DropCommand(const char* indexName, const char* tableName)
	{
		this->indexName = nullptr;
		this->tableName = nullptr;
		cout << "new DROP COMMAND";
		if (strlen(indexName) > 1)
		{
			this->setIndexName(indexName);
			if (strcmp(tableName, "") == 0)
			{
				this->initialCommand = " new DROP INDEX command";
			}
		}
		else
		{
			if (strlen(tableName) >= DropCommand::dropTableMinLength)
			{
				this->setTableName(tableName);
				this->initialCommand = " new DROP TABLE command";
			}
		}
	}
	void setTableName(const char* tableName)
	{
		if (DropCommand::dropTableMinLength <= strlen(tableName))
		{
			if (this->tableName != nullptr)
			{
				delete[] this->tableName;
			}
			this->tableName = new char[strlen(tableName) + 1];
			strcpy(this->tableName, tableName);
		}
		else
		{
			cout << "you need to enter a valid table name" << endl;
		}
	}
	void setIndexName(const char* indexName)
	{
		if (DropCommand::dropTableMinLength <= strlen(indexName))
		{
			if (this->indexName != nullptr)
			{
				delete[] this->indexName;
			}
			this->indexName = new char[strlen(indexName) + 1];
			strcpy(this->indexName, indexName);
		}
		else
		{
			cout << "you need to enter a valid index name" << endl;
		}
	}

	void receiveCommand(string comanda)
	{
		this->nouaComanda = comanda;
		this->initialCommand = comanda;
		createParse();
	}
	void createParse()
	{
		int newSize = this->nouaComanda.size();
		this->nouaComanda = this->nouaComanda.substr(5, newSize - 5);
		int backr = 0;
		if (this->nouaComanda.find('\r') != string::npos)
		{
			backr = this->nouaComanda.find('\r');
			this->nouaComanda = this->nouaComanda.substr(0, backr);
		}
		if (this->nouaComanda.find("TABLE") == 0)
		{
			newSize = this->nouaComanda.size();
			this->tableName = new char[newSize + 1];
			this->nouaComanda = this->nouaComanda.substr(6, newSize - 6);
			strcpy(this->tableName, this->nouaComanda.c_str());

			int i;
			for (i = 0; i < strlen(this->tableName); i++)
			{
				if ((this->tableName[i] < 'a' && this->tableName[i] != '_' && (this->tableName[i] < 48 || this->tableName[i] > 57)) || this->tableName[i] > 'z')
					break;
			}
			if (i != strlen(this->tableName))
			{
				cout << "Please reenter the command with a valid table name";
			}
			else
			{

				if (verifyIfExists(this->tableName))
				{
					this->deleteExistingFile();
					cout << "Drop is successful" << endl;
					this->deleteTrace();
					string name = string(this->tableName);
					name = name.append(".bin");
					ifstream f(name);
					if (f.is_open()) {
						remove(name.c_str());
					}
				}
				else
				{
					cout << "This table doesn't exists";
				}
			}
		}
		else if (this->nouaComanda.find("INDEX") == 0)
		{
			this->nouaComanda = this->nouaComanda.substr(6, newSize - 6);
			newSize = nouaComanda.size();
			this->indexName = new char[newSize];
			strcpy(this->indexName, this->nouaComanda.c_str());
			int i;
			for (i = 0; i < strlen(this->indexName); i++)
			{
				if ((this->indexName[i] < 'a' && this->indexName[i] != '_' && (this->indexName[i] < 48 || this->indexName[i] > 57)) || this->indexName[i] > 'z')
					break;
			}
			if (i != strlen(this->indexName))
			{
				cout << "Please reenter the command with a valid index name";
			}
			else
			{
				cout << "You have dropped the index: " << this->indexName << endl;
			}
		}
		else
			cout << "Please enter a valid command";
	}
	~DropCommand()
	{
		if (this->indexName)
		{
			delete[] this->indexName;
		}
		if (this->tableName)
		{
			delete[] this->tableName;
		}
	}

	DropCommand(const DropCommand& drop)
	{
		this->indexName = nullptr;
		this->tableName = nullptr;
		if (drop.tableName != nullptr)
			this->setTableName(drop.tableName);
		else if (drop.indexName != nullptr)
			this->setIndexName(drop.indexName);
	}
	friend ostream& operator<<(ostream& console, DropCommand& drop);
	friend void operator>>(istream& input, DropCommand& dr1);
	friend bool operator==(DropCommand dr1, DropCommand dr2);
	friend bool operator>=(DropCommand dr1, DropCommand dr2);


	//Overloading operator "[]" - DONE
//	char& operator[](int index) {
//		if (index >= 0 && index < strlen(this->indexName)) {
//			return this->indexName[index];
//		}
//		else {
//			cout<<"The index is wrong";
//		}
//	}


	//Overloading operator "!" - Comparing the length of the tableName with a constant value - DONE
	bool operator!() {
		if (strlen(this->indexName) > 12) {
			return true;
		}
		else {
			return false;
		}
	}


	//Overloading operator "+=" - DONE
	DropCommand& operator+=(DropCommand& name) {
		int x = strlen(this->indexName);
		char* temp = new char[x + 1];
		strcpy(temp, this->indexName);
		if (this->indexName) {
			delete[]this->indexName;
		}
		this->indexName = new char[x + strlen(name.indexName) + 1];
		strcpy(this->indexName, temp);
		strcat(this->indexName, name.indexName);
		delete[]temp;
		return *this;
	}
	//Overloading cast - DONE
	explicit operator int() {
		return strlen(this->indexName);
	}
};
const int DropCommand::dropTableMinLength = 2;
void operator>>(istream& input, DropCommand& dr1)
{
	cout << endl
		<< "What would you like to drop?" << endl;
	cout << "[ 1 ] table" << endl
		<< "[ 2 ] index" << endl;
	int option;
	cin >> option;
	switch (option)
	{
	case 1:
	{
		cout << "Now please enter the name of the table" << endl;
		char buffer[1000];
		input >> buffer;
		if (strlen(buffer) > 1)
			dr1.setTableName(buffer);
		cout << "Table dropped" << endl;
		break;
	}
	case 2:
	{
		cout << "Now please enter the name of the index" << endl;
		char buffer[1000];
		input >> buffer;
		if (strlen(buffer) > 1)
			dr1.setIndexName(buffer);

		cout << "Index dropped" << endl;
		break;
	}
	default:
	{
		cout << "Please enter a valid option" << endl;
		break;
	}
	}
}
ostream& operator<<(ostream& console, DropCommand& drop)
{

	console << endl
		<< "-------------------------------------";
	if (drop.initialCommand.find("INDEX") && drop.indexName != nullptr)
	{
		console << endl
			<< "The index you have dropped is: " << drop.indexName << endl;
	}
	else
	{
		if (drop.initialCommand.find("TABLE") && drop.tableName != nullptr)
			console << endl
			<< "The name of the table you have dropped is : " << drop.tableName << endl;
	}
	return console;
}
//Overloading operator "=="
bool operator==(DropCommand dr1, DropCommand dr2) {
	if (strcmp(dr1.indexName, dr2.indexName) == 0) {
		return true;
	}
	else {
		return false;
	}
}

//Overloading operator ">="
bool operator>=(DropCommand dr1, DropCommand dr2) {
	if (strlen(dr1.indexName) >= strlen(dr2.indexName)) {
		return true;
	}
	else {
		return false;
	}
}

