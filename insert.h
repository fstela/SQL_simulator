#include <iostream>
#include <fstream>
using namespace std;
class InsertCommand:public Useful,RemoveSpaces,Parse,DataManipCommands
{
protected:
	string nouaComanda;
	char* tableName;
	int sizeTableName;
	char* values;
	int contor;
	int firstParanthesis;
	int lastParanthesis;
	static const int insertTableMinLength;
	string column = "";
	char* presentValue = nullptr;
	bool corresp = 0;
	bool exists;
	int number = 0;

public:
	bool verifyIfExists(char* tableName) {
		bool temp = this->Useful::verifyIfExists(this->tableName);
		cout << "Searching for the table..";
		return temp;
	}
	void removeSpaces(string nouaComanda) {
		this->RemoveSpaces::removeSpaces(this->nouaComanda);
		cout << "Spaces removed.";
	}
	virtual void printAsInFile()
	{
		string numeFisierText = string(this->tableName) + ".txt";
		ifstream input(numeFisierText, ios::in);
		if (input.is_open())
		{
			string const delim{ "," };
			int beginning, pos = 0;
			while ((beginning = this->column.find_first_not_of(delim, pos)) != string::npos)
			{
				pos = this->column.find_first_of(delim, beginning);
				if (this->presentValue)
				{
					delete[] this->presentValue;
				}
				if (pos == -1 && beginning >= 0)
				{

					int length = this->column.size();
					this->presentValue = new char[length - beginning + 1];
					strcpy(this->presentValue, this->column.substr(beginning, length - beginning).c_str());
				}
				else
				{
					this->presentValue = new char[pos - beginning + 1];
				}
				strcpy(this->presentValue, this->column.substr(beginning, pos - beginning).c_str());
				string type;
				char buffer[1000];
				input.getline(buffer, 1000);
				type = string(buffer);
				if (type.find_first_of(" ") != string::npos)
				{
					type = type.substr(type.find_first_of(" ") + 1, type.size() - type.find_first_of(" "));
				}
				if (type.find_first_of(" ") != string::npos)
				{
					type = type.substr(0, type.find_first_of(" "));
				}
				if (type == "integer")
				{
					if (!atoi(this->presentValue))
					{
						cout << "Values don't correspond";
						break;
					}
				}
				else if (type == "float")
				{
					if (!atof(this->presentValue))
					{
						cout << "Values don't correspond";
						break;
					}
				}
				else if (type == "text")
				{
					if (!((this->presentValue[0] == '\"') && (this->presentValue[strlen(this->presentValue) - 1] == '\"')))
					{	cout << "Values don't correspond";
						break;
					}
				}
			}
			if (input.eof())
			{
				corresp = 1;
			}
			else
			{
				cout << "You haven't entered enough values";
			}

			input.close();
		}
		if (corresp == 1)
		{
			number++;
			string numeFisier = string(this->tableName) + ".bin";
			ofstream output(numeFisier, ios::app | ios::binary);
			if (output.is_open())
			{
				ifstream input(numeFisierText, ios::in);
				if (input.is_open())
				{
					string const delim{ ", " };
					int beginning, pos = 0;
					while ((beginning = this->column.find_first_not_of(delim, pos)) != string::npos)
					{
						pos = this->column.find_first_of(delim, beginning);

						if (this->presentValue)
						{
							delete[] this->presentValue;
						}
						if (pos == -1 && beginning >= 0)
						{

							int length = this->column.size();
							this->presentValue = new char[length - beginning + 1];
							strcpy(this->presentValue, this->column.substr(beginning, length - beginning).c_str());
						}
						else
						{
							this->presentValue = new char[pos - beginning + 1];
						}
						strcpy(this->presentValue, this->column.substr(beginning, pos - beginning).c_str());
						string type;
						char buffer[1000];
						input.getline(buffer, 1000);
						type = string(buffer);
						if (type.find_first_of(" ") != string::npos)
						{
							type = type.substr(type.find_first_of(" ") + 1, type.size() - type.find_first_of(" "));
						}
						if (type.find_first_of(" ") != string::npos)
						{
							type = type.substr(0, type.find_first_of(" "));
						}
						if (type == "integer")
						{
							int nrOfCharacters = atoi(this->presentValue);
							output.write((char*)&nrOfCharacters, sizeof(int));
						}
						else if (type == "float")
						{
							float nrOfCharacters = atof(this->presentValue);
							output.write((char*)&nrOfCharacters, sizeof(float));
						}
						else if (type == "text")
						{
							int nrOfCharacters = strlen(this->presentValue);
							output.write((char*)&nrOfCharacters, sizeof(int));
							output.write(this->presentValue, sizeof(char) * nrOfCharacters);
						}
					}
                    cout << endl
                        << "Values successfully inserted." << endl;
					input.close();
				}
				else
				{
					cout << endl
						<< "The file was not created";
				}
				output.close();
			}
		}
	}
	InsertCommand()
	{
		this->tableName = nullptr;
		this->sizeTableName = 999;
		this->values = nullptr;
	};

	InsertCommand(const char* tableName, int sizeTableName, const char* values)
	{
		this->tableName = nullptr;
		this->values = nullptr;
		if (strlen(tableName) > 1)
		{
			this->setTableName(tableName);
			this->sizeTableName = sizeTableName;
			if (values != nullptr)
				this->setValues(values);
		}
	}

	void setTableName(const char* tableName)
	{
		if (InsertCommand::insertTableMinLength <= strlen(tableName))
		{
			if (this->tableName != nullptr)
			{
				delete[] this->tableName;
			}
			this->tableName = new char[strlen(tableName) + 1];
			strcpy(this->tableName, tableName);
		}
	}
	void setValues(const char* values)
	{
		if (values != nullptr)
		{
			if (this->values)
			{
				cout << this->values;
				delete[] this->values;
			}
			this->values = new char[strlen(values) + 1];
			strcpy(this->values, values);
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
		this->nouaComanda = this->nouaComanda.substr(7, newSize - 7);
		int lRightP = 0;
		lRightP = this->nouaComanda.find_last_of(")");
		int backr = 0;

		if (this->nouaComanda.find('\r') != string::npos)
		{
			backr = this->nouaComanda.find('\r');
			this->nouaComanda = this->nouaComanda.substr(0, backr);
		}
		if (lRightP < this->nouaComanda.size() - 1)
			this->nouaComanda = this->nouaComanda.substr(0, lRightP);
		if (this->nouaComanda.find("INTO") == 0)
		{
			newSize = this->nouaComanda.size();
			this->nouaComanda = this->nouaComanda.substr(5, newSize - 5);
			this->sizeTableName = this->nouaComanda.find("VALUES") - 1;
			if (this->sizeTableName < 0 || this->sizeTableName >= newSize)
			{
				cout << endl
					<< "You forgot to add the table name";
			}
			else
			{
				this->tableName = new char[this->sizeTableName + 1];
				strcpy(this->tableName, this->nouaComanda.substr(0, this->sizeTableName).c_str());
				bool ok = 1;
				for (int i = 0; i < this->sizeTableName && ok == 1; i++)
				{
					if ((this->tableName[i] < 'a' && tableName[i] != '_' && (tableName[i] < 48 || tableName[i] > 57)) || tableName[i] > 'z')
					{
						ok = 0;
						break;
					}
				}
				if (ok == 0)
					cout << "the name you entered isn't correct";
				else
				{
					if (!verifyIfExists(this->tableName))
					{
						cout << "This table doesn't exists";
					}
					else
					{
						this->nouaComanda = this->nouaComanda.substr(this->sizeTableName + 8, newSize);
						removeSpaces(this->nouaComanda);
						this->lastParanthesis = this->nouaComanda.find(")");
						if (this->lastParanthesis > -2)
						{
							if (this->values != nullptr)
								delete[] values;
							this->values = new char[this->nouaComanda.size() + 1];
							strcpy(this->values, this->nouaComanda.c_str());

							this->column = string(this->values);
							this->column = this->column.substr(1, this->column.size() - 2);
							this->printAsInFile();
						}
						else
							cout << endl
							<< "you need to enter the values" << endl;
					}
				}
			}
		}
		else
			cout << endl
			<< "Please enter a valid command" << endl;
	}
	InsertCommand(const InsertCommand& insert)
	{
		this->tableName = nullptr;
		this->values = nullptr;
		this->presentValue = nullptr;
		if (insert.tableName != nullptr)
			this->setTableName(insert.tableName);
		this->sizeTableName = insert.sizeTableName;
		if (insert.values != nullptr)
			this->setValues(insert.values);
	}
	~InsertCommand()
	{
		if (this->tableName)
		{
			delete[] this->tableName;
		}
		if (this->values)
		{
			delete[] this->values;
		}
		if (this->presentValue)
		{
			delete[] this->presentValue;
		}
	}
	friend ostream& operator<<(ostream& console, InsertCommand& insert);
	friend void operator>>(istream& input, InsertCommand& ins);
	friend bool operator==(InsertCommand ins1, InsertCommand  ins2);
	friend bool operator>=(InsertCommand ins1, InsertCommand  ins2);


	//Overloading operator "[]" - DONE

//	char& operator[](int index) {
//		if (index >= 0 && index < strlen(this->tableName)) {
//			return this->tableName[index];
//		}
//		else {
//            throw new exception("The index is wrong");
//		}
//	}


	//Overloading operator "!" - Comparing the length of the tableName with a constant value - DONE
	bool operator!() {
		if (strlen(this->tableName) > 12) {
			return true;
		}
		else {
			return false;
		}
	}


	//Overloading operator "+=" - DONE
	InsertCommand& operator+=(InsertCommand& name) {
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

	//Overloading operator "++" - Preincrement
	InsertCommand operator++() {
		this->sizeTableName += 1;
		return *this;
	}
};
const int InsertCommand::insertTableMinLength = 2;
void operator>>(istream& input, InsertCommand& ins)
{
	cout << "Into which table would you like to insert?" << endl;
	char buffer[1000];
	input >> buffer;
	if (strlen(buffer) > 1)
	{
		ins.setTableName(buffer);
		cout << "What values would you like to insert?" << endl;
		input >> buffer;
		cout << "Values inserted" << endl;
	}
}
ostream& operator<<(ostream& console, InsertCommand& insert)
{

	console << endl
		<< "-------------------------------------";
	if (insert.tableName != nullptr)
		console << endl
		<< "You have inserted the values into the table : " << insert.tableName << endl;
	return console;
}
class InsertingValues:public InsertCommand {
protected:
	int counter = 0;
public:
	InsertingValues(){}
	void printAsInFile()
	{
		string numeFisierText = string(this->tableName) + ".txt";
		ifstream input(numeFisierText, ios::in);
		if (input.is_open())
		{
			string const delim{ "," };
			int beginning, pos = 0;
			while ((beginning = this->column.find_first_not_of(delim, pos)) != string::npos)
			{
				pos = this->column.find_first_of(delim, beginning);
				if (this->presentValue)
				{
					delete[] this->presentValue;
				}
				if (pos == -1 && beginning >= 0)
				{

					int length = this->column.size();
					this->presentValue = new char[length - beginning + 1];
					strcpy(this->presentValue, this->column.substr(beginning, length - beginning).c_str());
				}
				else
				{
					this->presentValue = new char[pos - beginning + 1];
				}
				strcpy(this->presentValue, this->column.substr(beginning, pos - beginning).c_str());
				string type;
				char buffer[1000];
				input.getline(buffer, 1000);
				type = string(buffer);
				if (type.find_first_of(" ") != string::npos)
				{
					type = type.substr(type.find_first_of(" ") + 1, type.size() - type.find_first_of(" "));
				}
				if (type.find_first_of(" ") != string::npos)
				{
					type = type.substr(0, type.find_first_of(" "));
				}
				if (type == "integer")
				{
					if (!atoi(this->presentValue))
					{
						cout << "Values don't correspond";
						break;
					}
				}
				else if (type == "float")
				{
					if (!atof(this->presentValue))
					{
						cout << "Values don't correspond";
						break;
					}
				}
				else if (type == "text")
				{
					if (!((this->presentValue[0] == '\"') && (this->presentValue[strlen(this->presentValue) - 1] == '\"')))
					{
						cout << "Values don't correspond";
						break;
					}
				}
			}
			if (input.eof())
			{
				corresp = 1;
			}
			else
			{
				cout << "You haven't entered enough values";
			}

			input.close();
		}
		if (corresp == 1)
		{
			number++;
			string numeFisier = string(this->tableName) + ".bin";
			ofstream output(numeFisier, ios::app | ios::binary);
			if (output.is_open())
			{
				ifstream input(numeFisierText, ios::in);
				if (input.is_open())
				{
					string const delim{ ", " };
					int beginning, pos = 0;
					while ((beginning = this->column.find_first_not_of(delim, pos)) != string::npos)
					{
						pos = this->column.find_first_of(delim, beginning);

						if (this->presentValue)
						{
							delete[] this->presentValue;
						}
						if (pos == -1 && beginning >= 0)
						{

							int length = this->column.size();
							this->presentValue = new char[length - beginning + 1];
							strcpy(this->presentValue, this->column.substr(beginning, length - beginning).c_str());
						}
						else
						{
							this->presentValue = new char[pos - beginning + 1];
						}
						strcpy(this->presentValue, this->column.substr(beginning, pos - beginning).c_str());
						string type;
						char buffer[1000];
						input.getline(buffer, 1000);
						type = string(buffer);
						if (type.find_first_of(" ") != string::npos)
						{
							type = type.substr(type.find_first_of(" ") + 1, type.size() - type.find_first_of(" "));
						}
						if (type.find_first_of(" ") != string::npos)
						{
							type = type.substr(0, type.find_first_of(" "));
						}
						if (type == "integer")
						{
							int nrOfCharacters = atoi(this->presentValue);
							output.write((char*)&nrOfCharacters, sizeof(int));
							counter++;
						}
						else if (type == "float")
						{
							float nrOfCharacters = atof(this->presentValue);
							output.write((char*)&nrOfCharacters, sizeof(float));
							counter++;
						}
						else if (type == "text")
						{
							int nrOfCharacters = strlen(this->presentValue);
							output.write((char*)&nrOfCharacters, sizeof(int));
							output.write(this->presentValue, sizeof(char) * nrOfCharacters);
							counter++;
						}
					}
					cout << endl << " You have succesfully inserted " << counter << " values.";
					input.close();
				}
				else
				{
					cout << endl
						<< "The file was not created";
				}
				output.close();
			}
		}
	}
};

//Overloading operator "=="
bool operator==(InsertCommand ins1, InsertCommand ins2) {
	if (strcmp(ins1.tableName, ins2.tableName) == 0) {
		return true;
	}
	else {
		return false;
	}
}

//Overloading operator ">="
bool operator>=(InsertCommand ins1, InsertCommand ins2) {
	if (strlen(ins1.tableName) >= strlen(ins2.tableName)) {
		return true;
	}
	else {
		return false;
	}
}
