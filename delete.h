#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class DeleteCommand:public Useful,Parse, DataManipCommands
{

	char* columnName;
	string initialCommand;
	static const int deleteTableMinLength;
	char* value;

public:
	bool verifyIfExists(char* tableName) {
		bool temp = this->Useful::verifyIfExists(this->tableName);
		cout << "Searching for the table..";
		return temp;
	}

	void deleteFrom()
	{
		string NumeFisier = string(this->tableName);
		NumeFisier = NumeFisier.append(".txt");
		string numeFisier = string(this->tableName) + ".bin";
		ifstream input(numeFisier, ios::in | ios::binary);
		ofstream temp("temp.bin", ios::out | ios::binary);
		bool ok = 0;
		int pos = 0;
		string delBufferText[1000];
		int delBufferInt[1000];
		float delBufferFloat[1000];
		int bufferLength[1000];
		if (input.is_open())
		{
			input.seekg(0, input.end);
			int length = input.tellg();
			input.seekg(0, input.beg);
			bool works = 0;
			while (pos < length)
			{
				ifstream filename(NumeFisier, ios::in);
				if (filename.is_open())
				{
					int indexInt = 0;
					int indexText = 0;
					int indexFloat = 0;
					int indexBuffer = 0;
					while (!filename.eof() && filename.is_open() && pos < length)
					{

						char buffer[1000];
						filename.getline(buffer, 1000);
						this->currentColumn = string(buffer);
						this->currentType = string(buffer);
						this->currentColumn = this->currentColumn.substr(0, this->currentColumn.find_first_of(" "));
						int x = this->currentType.find_first_of(" ");
						this->currentType = this->currentType.substr(x + 1, this->currentType.size() - x);
						this->currentType = this->currentType.substr(0, this->currentType.find_first_of(" "));
						if (this->currentType == "integer")
						{
							int currentInt;
							input.read((char*)&currentInt, sizeof(int));
							delBufferInt[indexInt++] = currentInt;
							if (this->value == to_string(currentInt))
							{
								works = 1;
							}

							pos += sizeof(currentInt);
						}
						else if (this->currentType == "float")
						{
							float currentFloat;
							input.read((char*)&currentFloat, sizeof(float));
							delBufferFloat[indexFloat++] = currentFloat;
							if (this->value == to_string(currentFloat))
							{
								works = 1;
							}

							pos += sizeof(currentFloat);
						}
						else if (this->currentType == "text")
						{
							int currentTextLength;
							string Text;
							input.read((char*)&currentTextLength, sizeof(int));
							char bufferText[100];
							input.read(bufferText, currentTextLength * sizeof(char));
							bufferText[currentTextLength] = '\0';
							Text = string(bufferText);
							bufferLength[indexBuffer++] = currentTextLength;
							delBufferText[indexText++] = Text;
							if (strcmp(this->value, bufferText) == 0)
							{
								works = 1;
							}

							pos += sizeof(char) * currentTextLength + sizeof(int);
						}
						if (filename.eof())
						{
							filename.close();
							ok = 0;
						}
					}
					if (works == 0)
					{
						ifstream filename(NumeFisier, ios::in);
						if (filename.is_open())
						{
							int indexInt = 0;
							int indexText = 0;
							int indexFloat = 0;
							int indexBuffer = 0;
							while (!filename.eof() && filename.is_open())
							{

								char buffer[1000];
								filename.getline(buffer, 1000);
								this->currentColumn = string(buffer);
								this->currentType = string(buffer);
								this->currentColumn = this->currentColumn.substr(0, this->currentColumn.find_first_of(" "));
								int x = this->currentType.find_first_of(" ");
								this->currentType = this->currentType.substr(x + 1, this->currentType.size() - x);
								this->currentType = this->currentType.substr(0, this->currentType.find_first_of(" "));
								if (this->currentType == "integer")
								{

									int currentInt = delBufferInt[indexInt++];
									temp.write((char*)&currentInt, sizeof(int));
								}
								else if (this->currentType == "float")
								{
									float currentFloat = delBufferFloat[indexFloat++];
									temp.write((char*)&currentFloat, sizeof(float));
								}
								else if (this->currentType == "text")
								{
									int currentTextLength = bufferLength[indexBuffer];
									string Text = delBufferText[indexText++];
									temp.write((char*)&currentTextLength, sizeof(int));
									temp.write(Text.c_str(), sizeof(char) * currentTextLength);
								}
								if (filename.eof())
								{
									filename.close();
									ok = 0;
								}
							}
						}
					}
					if (works == 1)
					{
						works = 0;
					}
					columnBinary = "";
				}
				if (pos == length)
				{
					input.close();
					temp.close();

					remove(numeFisier.c_str());
					rename("temp.bin", numeFisier.c_str());
				}
			}
		}
	}
	DeleteCommand()
	{
		this->tableName = nullptr;
		this->sizeTableName = 0;
		this->columnName = nullptr;
		this->value = nullptr;
	}
	DeleteCommand(const char* tableName, int sizeTableName, const char* columnName, const char* value)
	{
		this->columnName = nullptr;
		this->tableName = nullptr;
		this->value = nullptr;
		cout << "new DELETE FROM command" << endl;
		if (tableName != nullptr)
		{
			this->setTableName(tableName);
			this->sizeTableName = sizeTableName;
			this->setColumnName(columnName);
			this->setValue(value);
		}
		else
		{
			cout << "Please enter valid values" << endl;
		}
	}
	void setTableName(const char* tableName)
	{
		if (DeleteCommand::deleteTableMinLength <= strlen(tableName))
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
	void setColumnName(const char* columnName)
	{
		if (columnName != nullptr)
		{
			if (this->columnName != nullptr)
			{
				delete[] this->columnName;
			}
			this->columnName = new char[strlen(columnName) + 1];
			strcpy(this->columnName, columnName);
		}
		else
		{
			cout << "you need to enter a valid column name" << endl;
		}
	}
	void setValue(const char* value)
	{
		if (value != nullptr)
		{
			if (this->value != nullptr)
			{
				delete[] this->value;
			}
			this->value = new char[strlen(value) + 1];
			strcpy(this->value, value);
		}
		else
		{
			cout << "you need to enter a valid value" << endl;
		}
	}
	void receiveCommand(string comanda)
	{
		this->nouaComanda = comanda;
		if (this->nouaComanda.find('\r') != string::npos)
		{
			int backr = this->nouaComanda.find('\r');
			this->nouaComanda = this->nouaComanda.substr(0, backr);
		}
		createParse();
	}
	void createParse()
	{
		int newSize = this->nouaComanda.size();
		this->nouaComanda = this->nouaComanda.substr(7, newSize - 7);
		if (nouaComanda.find("FROM") == 0)
		{
			newSize = this->nouaComanda.size();
			this->nouaComanda = this->nouaComanda.substr(5, newSize - 5);
			newSize = this->nouaComanda.size();

			this->sizeTableName = nouaComanda.find_first_of("WHERE") - 1;
			if (this->sizeTableName < 0 || this->sizeTableName >= newSize)
			{
				cout << endl
					<< "You forgot the where clause";
			}
			else
			{
				this->tableName = new char[this->sizeTableName + 1];
				this->tableName = strcpy(this->tableName, nouaComanda.substr(0, this->sizeTableName).c_str());
				bool ok = 1;
				for (int i = 0; i < this->sizeTableName && ok == 1; i++)
				{
					if ((this->tableName[i] < 'a' && this->tableName[i] != '_' && (this->tableName[i] < 48 || this->tableName[i] > 57)) || this->tableName[i] > 'z')
					{
						ok = 0;
						break;
					}
				}
				if (ok == 0)
					cout << "the name you entered isn't correct";
				else
				{
					this->nouaComanda = this->nouaComanda.substr(this->sizeTableName + 7, newSize);
					newSize = this->nouaComanda.size();
					int sizeColumnName;
					sizeColumnName = this->nouaComanda.find("=") - 1;
					if (sizeColumnName > 0 && sizeColumnName <= newSize)
					{
						this->columnName = new char[sizeColumnName + 1];
						strcpy(this->columnName, this->nouaComanda.substr(0, sizeColumnName).c_str());
						ok = 1;
						for (int i = 0; i < sizeColumnName && ok == 1; i++)
						{
							if ((this->columnName[i] < 'a' && this->columnName[i] != '_' && (this->columnName[i] < 48 || this->columnName[i] > 57)) || this->columnName[i] > 'z')
							{
								ok = 0;
								break;
							}
						}
						if (ok == 0)
						{
							cout << "The column name you entered isn't correct";
						}
						else
						{
							this->nouaComanda = this->nouaComanda.substr(sizeColumnName + 3, newSize - sizeColumnName - 3);
							if (this->value != nullptr)
							{
								delete[] this->value;
							}
							this->value = new char[this->nouaComanda.size() + 1];
							strcpy(this->value, this->nouaComanda.c_str());
							if (verifyIfExists(this->tableName))
							{
								this->deleteFrom();
							}

							cout << "Delete is successful";
						}
					}
					else
					{
						cout << "Please reenter the command." << endl;
					}
				}
			}
		}
		else
			cout << endl
			<< "please enter a valid command";
	}
	~DeleteCommand()
	{
		if (this->tableName)
		{
			delete[] this->tableName;
		}
		if (this->columnName)
		{
			delete[] this->columnName;
		}
		if (this->value)
		{
			delete[] this->value;
		}
	}
	DeleteCommand(const DeleteCommand& dlt)
	{
		this->value = nullptr;
		if (dlt.tableName != nullptr)
			this->setTableName(dlt.tableName);
		if (dlt.columnName != nullptr)
			this->setColumnName(dlt.columnName);
		if (dlt.value != nullptr)
			this->setValue(dlt.value);
		this->sizeTableName = dlt.sizeTableName;
	}
	friend ostream& operator<<(ostream& console, DeleteCommand& dlt);
	friend void operator>>(istream& input, DeleteCommand& del1);
	friend bool operator==(DeleteCommand del1, DeleteCommand  del2);
	friend bool operator>=(DeleteCommand del1, DeleteCommand  del2);


	//Overloading operator "[]" 

//	char& operator[](int index) {
//		if (index >= 0 && index < strlen(this->tableName)) {
//			return this->tableName[index];
//		}
//		else {
//			cout<<"The index is wrong";
//		}
//	}


	//Overloading operator "!" - Comparing the length of the tableName with a constant value 
	bool operator!() {
		if (strlen(this->tableName) > 12) {
			return true;
		}
		else {
			return false;
		}
	}


	//Overloading operator "+=" - DONE
	DeleteCommand& operator+=(DeleteCommand& name) {
		int x = strlen(this->tableName);
		char* temp = new char[x + 1];
		strcpy(temp, this->tableName);
		if (this->tableName) {
			delete[]this->tableName;
		}
		this->tableName = new char[x + strlen(name.tableName) + 1];
		strcpy(this->tableName, temp);
		strcat(this->tableName, name.tableName);
		delete[]temp;
		return *this;
	}
	//Overloading cast - DONE
	explicit operator int() {
		return strlen(this->tableName);
	}
};
const int DeleteCommand::deleteTableMinLength = 2;
ostream& operator<<(ostream& console, DeleteCommand& dlt)
{
	console << endl
		<< "\n-------------------------------------";
	if (dlt.tableName != nullptr)
		console << endl
		<< "The name of the table you have deleted from is: " << dlt.tableName << endl;
	return console;
}
void operator>>(istream& input, DeleteCommand& del1)
{
	cout << "Which table would you like to delete from?" << endl;
	char buffer[1000];
	input >> buffer;
	if (strlen(buffer) > 1)
		del1.setTableName(buffer);
	cout << "What column would you like to delete from?" << endl;
	input >> buffer;
	if (strlen(buffer) > 1)
		del1.setColumnName(buffer);
	cout << "What value would you like to search for?" << endl;
	input >> buffer;
	if (strlen(buffer) > 1)
		del1.setValue(buffer);
}
//Overloading operator "=="
bool operator==(DeleteCommand del1, DeleteCommand del2) {
	if (strcmp(del1.tableName, del2.tableName) == 0) {
		return true;
	}
	else {
		return false;
	}
}

//Overloading operator ">="
bool operator>=(DeleteCommand del1, DeleteCommand del2) {
	if (strlen(del1.tableName) >= strlen(del2.tableName)) {
		return true;
	}
	else {
		return false;
	}
}
