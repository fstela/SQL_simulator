#include <iostream>
#include <fstream>
using namespace std;
class DisplayCommand:public Useful,Parse,DataManipCommands
{
protected:
	string nouaComanda;
	char* nameTable;
	static const int displayTableMinLength;
	bool exists = 0;


public:
	bool verifyIfExists(char* tableName) {
		bool temp = this->Useful::verifyIfExists(this->nameTable);
		cout << "Searching for the table..";
		return temp;
	}

	virtual void openingTableFile() {
		string NumeFisier = string(this->nameTable);
		NumeFisier = NumeFisier.append(".txt");
		ifstream filename(NumeFisier, ios::in);
		ofstream output("Display.txt", ios::out | ios::app);
		if (filename.is_open()) {
			cout << endl << "----------------------------------" << endl;
			output << endl << "\n You are displaying the table: " << this->nameTable << endl;
			output << "----------------------------" << endl;
			while (!filename.eof()) {
				string nameOfExistingColumn;
				char buffer[1000];
				filename.getline(buffer, 1000);
				nameOfExistingColumn=string(buffer);
				cout << nameOfExistingColumn<<endl;
				if (output.is_open()) {
					output << nameOfExistingColumn<<endl;
				}
			}

			filename.close();
			output.close();
		}
	}
	DisplayCommand()
	{
		this->nameTable = nullptr;
	}
	DisplayCommand(const char* nameTable)
	{
		this->nameTable = nullptr;
		if (nameTable != nullptr)
			this->setNameTable(nameTable);
	}
	void setNameTable(const char* nameTable)
	{
		if (DisplayCommand::displayTableMinLength <= strlen(nameTable))
		{
			if (this->nameTable != nullptr)
			{
				delete[] this->nameTable;
			}
			this->nameTable = new char[strlen(nameTable) + 1];
			strcpy(this->nameTable, nameTable);
		}
	}
	void receiveCommand(string comanda)
	{
		this->nouaComanda = comanda;
		createParse();
	}
	void createParse()
	{
		int newSize = this->nouaComanda.size();
		this->nouaComanda = this->nouaComanda.substr(8, newSize - 8);
		int backr = 0;
		if (this->nouaComanda.find('\r') != string::npos)
		{
			backr = this->nouaComanda.find('\r');
			this->nouaComanda = this->nouaComanda.substr(0, backr);
		}
		if (this->nouaComanda.find("TABLE") == 0)
		{
			newSize = this->nouaComanda.size();
			if (this->nameTable != nullptr)
				delete[] this->nameTable;
			this->nameTable = new char[newSize + 1];
			this->nouaComanda = this->nouaComanda.substr(6, newSize - 6);
			strcpy(this->nameTable, this->nouaComanda.c_str());
			int i;
			for (i = 0; i < strlen(this->nameTable); i++)
			{
				if ((this->nameTable[i] < 'a' && this->nameTable[i] != '_' && (this->nameTable[i] < 48 || this->nameTable[i] > 57)) || this->nameTable[i] > 'z')
					break;
			}
			if (i != strlen(this->nameTable))
			{
				cout << "Please reenter the command with a valid table name";
			}
			else {
				this->exists=this->verifyIfExists(this->nameTable);
				if (this->exists) {
					this->openingTableFile();
				}
			}
		}
		else
			cout << "Please enter a valid command";
	}
	~DisplayCommand()
	{
		if (this->nameTable)
		{
			delete[] this->nameTable;
		}
	}
	DisplayCommand(const DisplayCommand& display)
	{
		this->nameTable = nullptr;
		if (display.nameTable != nullptr)
			this->setNameTable(display.nameTable);
	}
	friend void operator>>(istream& input, DisplayCommand& dis1);
	friend ostream& operator<<(ostream& console, DisplayCommand& display);
	friend bool operator==(DisplayCommand dis1, DisplayCommand dis2);
	friend bool operator>=(DisplayCommand dis1, DisplayCommand dis2);


	//Overloading operator "[]" - DONE
//	char& operator[](int index) {
//		if (index >= 0 && index < strlen(this->nameTable)) {
//			return this->nameTable[index];
//		}
//		else {
//			cout<<"The index is wrong";
//		}
//	}


	//Overloading operator "!" - Comparing the length of the tableName with a constant value - DONE
	bool operator!() {
		if (strlen(this->nameTable) > 12) {
			return true;
		}
		else {
			return false;
		}
	}


	//Overloading operator "+=" - DONE
	DisplayCommand& operator+=(DisplayCommand& name) {
		int x = strlen(this->nameTable);
		char* temp = new char[x + 1];
		strcpy(temp, this->nameTable);
		if (this->nameTable) {
			delete[]this->nameTable;
		}
		this->nameTable = new char[x + strlen(name.nameTable) + 1];
		strcpy(this->nameTable, temp);
		strcat(this->nameTable, name.nameTable);
		delete[]temp;
		return *this;
	}
	//Overloading cast - DONE
	explicit operator int() {
		return strlen(this->nameTable);
	}
};
const int DisplayCommand::displayTableMinLength = 2;
ostream& operator<<(ostream& console, DisplayCommand& display)
{

	console << endl
		<< "-------------------------------------";
	if (display.nameTable != nullptr)
		console << endl
		<< "The name of the table you have displayed is: " << display.nameTable << endl;

	return console;
}
class DisplayingConsole: public DisplayCommand {
protected:
	int counter = 0;
public:
	DisplayingConsole(){}
	void openingTableFile() {
		//cout << this->tableName << endl;
		string NumeFisier = string(this->nameTable);
		NumeFisier = NumeFisier.append(".txt");
		ifstream filename(NumeFisier, ios::in);
		ofstream output("Display.txt", ios::out | ios::app);
		if (filename.is_open()) {
			cout << endl << "----------------------------------" << endl;
			output << endl << "\n You are displaying the table: " << this->nameTable << endl;
			output << "----------------------------" << endl;
			while (!filename.eof()) {
				string nameOfExistingColumn;
				char buffer[1000];
				filename.getline(buffer, 1000);
				nameOfExistingColumn = string(buffer);
				cout << nameOfExistingColumn << endl;
				counter++;
				if (output.is_open()) {
					output << nameOfExistingColumn << endl;
				}
			}

			filename.close();
			output.close();
		}
		cout << endl << "You have displayed " << counter<< " rows.";
	}
};
void operator>>(istream& input, DisplayCommand& dis1)
{
	cout << "Which table would you like to display?" << endl;
	char buffer[1000];
	input >> buffer;
	if (strlen(buffer) > 1)
	{
		dis1.setNameTable(buffer);
		cout << "Table displayed" << endl;
	}
	else
	{
		cout << "Please enter a valid name";
	}
}
//Overloading operator "=="
bool operator==(DisplayCommand dis1, DisplayCommand dis2) {
	if (strcmp(dis1.nameTable, dis2.nameTable) == 0) {
		return true;
	}
	else {
		return false;
	}
}

//Overloading operator ">="
bool operator>=(DisplayCommand dis1, DisplayCommand dis2) {
	if (strlen(dis1.nameTable) >= strlen(dis2.nameTable)) {
		return true;
	}
	else {
		return false;
	}
}
