#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class CreateCommand:public Useful,RemoveSpaces,Parse,DataManipCommands
{
	string initialCommand = "";
	string nouaComanda;
	char* columnName = nullptr;
	char* tableName;
	int sizeTableName;
	char* indexName;
	char* columnList = nullptr;
	int defaultInt;
	float defaultFloat;
	char* defaultText = nullptr;
	string type = "";
	string list = "";
	static const int createTableMinLength;
	bool exists = 0;
	int i;
	int length;

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

	void printAsInFile()
	{
		string numeFisier = string(this->tableName) + ".txt";
		ofstream output(numeFisier, ios::out | ios::app);
		if (output.is_open())
		{
			string const delims{ "()," };
			int beginning, pos = 0, i = 1;
			while ((beginning = this->list.find_first_not_of(delims, pos)) != string::npos)
			{

				if (i <= 4)
				{
					pos = this->list.find_first_of(delims, beginning + 1);
					output << this->list.substr(beginning, pos - beginning);
					if (i < 4)
					{
						output << " ";
					}
					i++;
				}
				if (i == 5 && this->i < this->length)
				{
					i = 1;
					output << endl;
				}
			}
		}
		else
		{
			cout << endl
				<< "The file was not created";
		}
		output.close();
	}

	void savingTables()
	{

		ofstream raport("listOfTables.txt", ios::app);
		if (raport.is_open())
		{
			raport << this->tableName << '\n';
			raport.close();
		}
		else
		{
			cout << "The file was not created.";
		}
	}
	CreateCommand()
	{
		this->defaultText = nullptr;
		this->columnName = nullptr;
		this->tableName = nullptr;
		this->sizeTableName = 0;
		this->indexName = nullptr;
		this->columnList = nullptr;
	}
	CreateCommand(const char* tableName, int sizeTableName, const char* indexName, const char* columnList)
	{
		this->tableName = nullptr;
		this->indexName = nullptr;
		if (strlen(indexName) > 1)
		{
			this->setIndexName(indexName);
			if (strcmp(tableName, "") == 0)
			{
				this->initialCommand = " new CREATE INDEX command";
			}
		}
		else
		{
			if (strlen(tableName) > CreateCommand::createTableMinLength)
			{
				this->setTableName(tableName);
				this->initialCommand = " new CREATE TABLE command";
			}
		}
	}

	void setTableName(const char* tableName)
	{
		if (CreateCommand::createTableMinLength <= strlen(tableName))
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
		if (strlen(indexName) >= CreateCommand::createTableMinLength)
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
	void setColumnList(const char* columnList)
	{
		if (strlen(columnList) > 1)
		{
			if (this->columnList != nullptr)
			{
				delete[] this->columnList;
			}
			this->columnList = new char[strlen(columnList) + 1];
			strcpy(this->columnList, columnList);
		}
		else
		{
			cout << "you need to enter a valid column list" << endl;
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
		this->nouaComanda = this->nouaComanda.substr(7, newSize - 7);

		if (this->nouaComanda.find("TABLE") == 0)
		{
			newSize = this->nouaComanda.size();
			this->nouaComanda = this->nouaComanda.substr(6, newSize - 6);
			newSize = this->nouaComanda.size();
			this->sizeTableName = this->nouaComanda.find_first_of("(") - 1;
			if (this->sizeTableName < 0 || this->sizeTableName >= newSize || this->sizeTableName < CreateCommand::createTableMinLength)
			{
				cout << endl
					<< "You need to add the columns";
			}
			else
			{
				this->tableName = new char[sizeTableName + 1];
				this->tableName = strcpy(this->tableName, this->nouaComanda.substr(0, sizeTableName).c_str());
				bool ok = 1;
				for (int i = 0; i < sizeTableName && ok == 1; i++)
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
					if (verifyIfExists(this->tableName))
					{
						cout << "This table already exists";
					}
					else
					{
						string name = string(this->tableName);
						this->nouaComanda = this->nouaComanda.substr(sizeTableName + 1, newSize);
						removeSpaces(this->nouaComanda);

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
						this->columnList = new char[this->nouaComanda.size() + 1];
						strcpy(this->columnList, this->nouaComanda.c_str());
						int noLeftP = 0;
						int noRightP = 0;
						int commaCount = 0;
						int v;
						for (int i = 1; i < strlen(this->columnList) - 1; i++)
						{
							if (this->columnList[i] == ',')
								commaCount++;
							if (this->columnList[i] == '(')
								noLeftP++;
							if (this->columnList[i] == ')')
								noRightP++;
							v = i;
						}
						if (noLeftP == noRightP && ((commaCount + 1) % noRightP == 0) && this->columnList[0] == '(' && this->columnList[strlen(this->columnList) - 2] == ')')
						{
							if (this->columnList[0] == '(' && this->columnList[strlen(this->columnList) - 1] == ')')
							{
								this->columnList[strlen(this->columnList) - 1] = '\0';
								this->columnList = this->columnList + 1;
							}
							int firstL;
							int firstR;
							int lastR;
							string paranth = "";
							bool wrongInput = 1;
							bool lastColumn = 0;
							this->length = strlen(this->columnList) + 1;
							this->i = 1;
							while (wrongInput)
							{
								this->list = string(this->columnList);
								firstL = 0;
								firstR = 0;
								lastR = (this->list.find_last_of(')'));
								if (this->list.find_first_of('(') == 0)
									firstL = this->list.find_first_of('(');
								if (this->list.find_first_of(')') > 0)
									firstR = this->list.find_first_of(')');
								if (paranth == "")
								{
									paranth = this->list.substr(1, firstR - 1);
								}
								int firstC = 0;
								if (paranth.find_first_of(',') > 0)
									firstC = paranth.find_first_of(',');
								if (this->columnName)
									delete[] this->columnName;
								this->columnName = new char[firstC + 1];
								strcpy(this->columnName, paranth.substr(0, firstC).c_str());

								ok = 1;
								for (int i = 0; i < strlen(this->columnName) && ok == 1; i++)
								{
									if ((this->columnName[i] < 'a' && this->columnName[i] != '_' && (this->columnName[i] < 48 || this->columnName[i] > 57)) || this->columnName[i] > 'z')
									{
										ok = 0;
										break;
									}
								}
								if (ok == 0)
								{
									cout << "the name you entered for the column isn't correct";
									wrongInput = 0;
								}
								else
								{
									paranth = paranth.substr(firstC + 1, paranth.size() - firstC - 1);
									if (paranth.find_first_of(',') > 0)
										firstC = paranth.find_first_of(',');
									this->type = paranth.substr(0, firstC);
									if (this->type != "text" && this->type != "integer" && this->type != "float")
									{
										cout << "wrong type" << endl;
										wrongInput = 0;
									}
									else
									{
										int length = 0;
										paranth = paranth.substr(firstC + 1, paranth.size() - firstC - 1);
										if (paranth.find_first_of(',') != string::npos)
											firstC = paranth.find_first_of(',');
										length = stoi(paranth.substr(0, firstC));
										if (this->type == "integer")
										{
											paranth = paranth.substr(firstC + 1, paranth.size() - firstC - 1);
											this->defaultInt = stoi(paranth.substr(0, paranth.size()));
											this->list = this->list.substr(0, firstR + 1);
											this->i += this->list.size();
											this->printAsInFile();
											if (strlen(this->columnList + firstR + 1) > 0)
											{
												this->i++;
												this->columnList = this->columnList + firstR + 2;

												if (this->i >= length)
													lastColumn = 1;
											}
											else
											{
												this->columnList = nullptr;
											}
										}
										else
										{
											if (this->type == "float")
											{
												paranth = paranth.substr(firstC + 1, paranth.size() - firstC - 1);
												this->defaultFloat = stof(paranth.substr(0, paranth.size()));
												this->list = this->list.substr(0, firstR + 1);
												this->i += this->list.size();
												this->printAsInFile();
												if (strlen(this->columnList + firstR + 1) > 0)
												{
													this->i++;
													this->columnList = this->columnList + firstR + 2;
													if (this->i >= length)
														lastColumn = 1;
												}
												else
												{
													this->columnList = nullptr;
												}
											}
											else
											{
												if (this->type == "text")
												{
													paranth = paranth.substr(firstC + 1, paranth.size() - firstC - 1);
													if (this->defaultText != nullptr)
														delete[] this->defaultText;
													this->defaultText = new char[paranth.size() + 1];
													strcpy(this->defaultText, paranth.c_str());
													this->list = this->list.substr(0, firstR + 1);
													i += this->list.size();
													this->printAsInFile();
													if (strlen(this->columnList + firstR + 1) > 0)
													{

														this->columnList = this->columnList + firstR + 2;
														this->i++;
														if (this->i >= this->length)
															lastColumn = 1;
													}
													else
													{
														this->columnList = nullptr;
													}
												}
												else
												{
													cout << "Wrong input";
													wrongInput = 0;
												}
											}
										}
										if (this->columnList == nullptr)
										{
											wrongInput = 0;
											savingTables();
										}

										else
										{
											paranth = string(this->columnList);
											paranth = paranth.substr(1, paranth.size() - 1);
										}
									}
								}
							}
						}
						else
							cout << "wrong column list, try again";
					}
				}
			}
		}
		else
		{
			if (this->nouaComanda.find("INDEX") == 0)
			{
				this->nouaComanda = this->nouaComanda.substr(6, newSize - 6).c_str();
				newSize = nouaComanda.size();
				this->sizeTableName = this->nouaComanda.find("ON") - 1;
				if (this->sizeTableName < 0 || this->sizeTableName >= newSize)
				{
					cout << endl
						<< "You need to add the column";

				}
				else
				{
					this->indexName = new char[this->sizeTableName + 1];
					strcpy(this->indexName, this->nouaComanda.substr(0, this->sizeTableName).c_str());

					bool ok = 1;
					for (int i = 0; i < this->sizeTableName && ok == 1; i++)
					{
						if ((this->indexName[i] < 'a' && this->indexName[i] != '_' && (this->indexName[i] < 48 || this->indexName[i] > 57)) || this->indexName[i] > 'z')
						{
							ok = 0;
							break;
						}
					}
					if (ok == 0)
						cout << "the index name you entered isn't correct";
					else
					{
						cout << this->indexName << endl
							<< "ON" << endl;

						this->nouaComanda = this->nouaComanda.substr(sizeTableName + 4, newSize);
						this->sizeTableName = this->nouaComanda.find("(") - 1;
						newSize = this->nouaComanda.size();
						if (this->sizeTableName < 0 || this->sizeTableName >= newSize)
						{
							cout << endl
								<< "You need to add the columns";
						}
						else
						{
							this->tableName = new char[this->sizeTableName + 1];
							this->tableName = strcpy(this->tableName, this->nouaComanda.substr(0, this->sizeTableName).c_str());
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
								cout << "the table name you entered isn't correct";
							else
							{
								cout << this->tableName << endl;
								this->nouaComanda = this->nouaComanda.substr(this->sizeTableName + 1, newSize);
								newSize = this->nouaComanda.size();
								this->columnList = new char[newSize - 1];
								strcpy(this->columnList, this->nouaComanda.substr(1, newSize - 2).c_str());
								bool ok = 1;
								for (int j = 0; (j < (newSize - 2) && ok == 1); j++)
								{
									if ((this->columnList[j] < 'a' && this->columnList[j] != '_' && (this->columnList[j] < 48 || this->columnList[j] > 57)) || this->columnList[j] > 'z')
									{
										ok = 0;
										break;
									}
								}
								if (ok == 0)
									cout << "the column name you entered isn't correct";
								else
								{
									if (this->nouaComanda.find(")"))
										cout << this->nouaComanda << endl;
								}
							}
						}
					}
				}
			}
			else
				cout << "Enter a valid command";
		}
	}
	~CreateCommand()
	{
		if (this->tableName)
		{
			delete[] this->tableName;
		}
		if (this->indexName)
		{
			delete[] this->indexName;
		}
		if (this->columnName)
		{
			delete[] this->columnName;
		}
		if (this->defaultText)
		{
			delete[] this->defaultText;
		}
	}

	CreateCommand(const CreateCommand& create)
	{
		this->tableName = nullptr;
		this->indexName = nullptr;
		this->columnName = nullptr;
		this->defaultText = nullptr;
		if (strcmp(create.tableName, "") != 0)
			this->setTableName(create.tableName);
		else if (strcmp(create.indexName, "") != 0)
			this->setIndexName(create.indexName);
		if (strcmp(create.columnList, "") != 0)
			this->setColumnList(create.columnList);
	}
	friend void operator>>(istream& input, CreateCommand& cc1);
	friend ostream& operator<<(ostream& console, CreateCommand& create);
	friend bool operator==(CreateCommand cc1, CreateCommand cc2);
	friend bool operator>=(CreateCommand cc1, CreateCommand cc2);
	//Overloading operator "[]" 

	char& operator[](int index) {
		if (index >= 0 && index < strlen(this->tableName)) {
			return this->tableName[index];
		}
		else {
			cout<<"The index is wrong";
		}
	}


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
	CreateCommand& operator+=(CreateCommand& name) {
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
const int CreateCommand::createTableMinLength = 2;
void operator>>(istream& input, CreateCommand& cc1)
{
	cout << endl
		<< "What would you like to create?" << endl;
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
			cc1.setTableName(buffer);
		cout << "Enter the column list" << endl;
		input >> buffer;
		if (strlen(buffer) > 1)
		{
			cc1.setColumnList(buffer);
		}
		cout << "Table created" << endl;
		break;
	}
	case 2:
	{
		cout << "Now please enter the name of the index" << endl;
		char buffer[1000];
		input >> buffer;
		if (strlen(buffer) > 1)
			cc1.setIndexName(buffer);
		cout << "In which table would you like to create the index?" << endl;
		input >> buffer;
		if (strlen(buffer) > 1)
			cc1.setTableName(buffer);
		cout << "On which column should the index be placed?" << endl;
		input >> buffer;
		if (strlen(buffer) > 1)
		{
			cc1.setColumnList(buffer);
		}
		cout << "Index created" << endl;
		break;
	}
	default:
	{
		cout << "Please enter a valid option" << endl;
		break;
	}
	}
}
ostream& operator<<(ostream& console, CreateCommand& create)
{
	//CREATE INDEX index1 ON table2 (column3)
	console << endl
		<< "-------------------------------------";

	if (create.initialCommand.find("TABLE") > 0 && create.initialCommand.find("TABLE") < create.initialCommand.size() && create.tableName != nullptr)

	{
		console << endl
			<< "The name of the table you have created is: " << create.tableName << endl;
	}
	else if (create.initialCommand.find("INDEX") > 0 && create.initialCommand.find("INDEX") < create.initialCommand.size() && create.indexName != nullptr)
	{
		console << endl
			<< "The name of the index you have created is: " << create.indexName << endl;
	}
	return console;
}
//Overloading operator "=="
bool operator==(CreateCommand cc1, CreateCommand cc2) {
	if (strcmp(cc1.tableName, cc2.tableName) == 0) {
		return true;
	}
	else {
		return false;
	}
}

//Overloading operator ">="
bool operator>=(CreateCommand cc1, CreateCommand cc2) {
	if (strlen(cc1.tableName) >= strlen(cc2.tableName)) {
		return true;
	}
	else {
		return false;
	}
}
