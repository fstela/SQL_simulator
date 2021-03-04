#include <iostream>
#include <fstream>
using namespace std;
class UpdateCommand:public Useful,Parse,DataManipCommands
{
	char* columnName1;
	char* columnName2;
	char* value1;
	char* value2;
	static const int updateTableMinLength;

public:
	bool verifyIfExists(char* tableName) {
		bool temp = this->Useful::verifyIfExists(this->tableName);
		cout << "Searching for the table..";
		return temp;
	}
	void updateWhere()
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
							if (this->value2 == to_string(currentInt))
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
							if (this->value2 == to_string(currentFloat))
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
							char bufferText[10000];
							input.read(bufferText, currentTextLength * sizeof(char));
							bufferText[currentTextLength] = '\0';
							Text = string(bufferText);
							bufferLength[indexBuffer++] = currentTextLength;
							delBufferText[indexText++] = Text;
							if (strcmp(this->value2, bufferText) == 0)
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
									int currentTextLength = bufferLength[indexBuffer++];
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
                                    if (this->currentColumn == this->columnName1)
									{
										int dummy = atoi(this->value1);
										temp.write((char*)&dummy, sizeof(int));
									}
                                    else
										temp.write((char*)&currentInt, sizeof(int));
								}
								else if (this->currentType == "float")
								{
									float currentFloat = delBufferFloat[indexFloat++];
                                    if (this->currentColumn == this->columnName1)
									{
										int dummy = atof(this->value1);
										temp.write((char*)&dummy, sizeof(float));
									}
                                    else
                                        temp.write((char*)&currentFloat, sizeof(float));
								}
								else if (this->currentType == "text")
								{
									int currentTextLength = bufferLength[indexBuffer++];
									string Text = delBufferText[indexText++];
									if (this->currentColumn == this->columnName1) {
										currentTextLength = strlen(this->value1);
										Text = string(this->value1);
										temp.write((char*)&currentTextLength, sizeof(int));
										temp.write(Text.c_str(), sizeof(char) * currentTextLength);
									}
                                    else{
										temp.write((char*)&currentTextLength, sizeof(int));
										temp.write(Text.c_str(), sizeof(char) * currentTextLength);
                                    }
								}
								if (filename.eof())
								{
									filename.close();
									ok = 0;
								}
							}
						}
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
	UpdateCommand()
	{
		this->tableName = nullptr;
		this->columnName1 = nullptr;
		this->columnName2 = nullptr;
		this->value1 = nullptr;
		this->value2 = nullptr;
		this->sizeTableName = 0;
	}
	UpdateCommand(const char* tableName, const char* columnName1, const char* columnName2, const char* value1, const char* value2, int sizeTableName)
	{
		this->tableName = nullptr;
		this->columnName1 = nullptr;
		this->columnName2 = nullptr;
		this->value1 = nullptr;
		this->value2 = nullptr;
		if (strlen(tableName) > 1)
		{
			this->setTableName(tableName);
			this->sizeTableName = sizeTableName;
		}
		else
		{
			cout << "Please enter valid values" << endl;
		}
	}
	void setTableName(const char* newTableName)
	{
		if (UpdateCommand::updateTableMinLength <= strlen(newTableName))
		{
			if (this->tableName != nullptr)
			{
				delete[] this->tableName;
			}
			this->tableName = new char[strlen(newTableName) + 1];
			strcpy(this->tableName, newTableName);
		}
	}
	void setColumnName1(const char* newColumnName1)
	{
		if (newColumnName1 != nullptr)
		{
			if (this->columnName1 != nullptr)
			{
				delete[] this->columnName1;
			}
			this->columnName1 = new char[strlen(newColumnName1) + 1];
			strcpy(this->columnName1, newColumnName1);
		}
		else
		{
			cout << "you need to enter a valid column name" << endl;
		}
	}
	void setColumnName2(const char* newColumnName2)
	{
		if (newColumnName2 != nullptr)
		{
			if (this->columnName2 != nullptr)
			{
				delete[] this->columnName2;
			}
			this->columnName2 = new char[strlen(newColumnName2) + 1];
			strcpy(this->columnName2, newColumnName2);
		}
		else
		{
			cout << "you need to enter a valid column name" << endl;
		}
	}
	void setValue1(const char* newValue1)
	{
		if (newValue1 != nullptr)
		{
			if (this->value1 != nullptr)
			{
				delete[] this->value1;
			}
			this->value1 = new char[strlen(newValue1) + 1];
			strcpy(this->value1, newValue1);
		}
		else
		{
			cout << "you need to enter a valid value" << endl;
		}
	}
	void setValue2(const char* newValue2)
	{
		if (newValue2 != nullptr)
		{
			if (this->value2 != nullptr)
			{
				delete[] this->value2;
			}
			this->value2 = new char[strlen(newValue2) + 1];
			strcpy(this->value2, newValue2);
		}
		else
		{
			cout << "you need to enter a valid value" << endl;
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
		this->sizeTableName = this->nouaComanda.find_first_of("SET") - 1;
		if (this->sizeTableName < 0 || this->sizeTableName >= newSize)
		{
			cout << endl
				<< "You didn't provide the correct syntax. Please recheck";
		}
		else
		{
			this->tableName = new char[this->sizeTableName];
			strcpy(this->tableName, this->nouaComanda.substr(0, sizeTableName).c_str());
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
			{
				cout << endl
					<< "The name you entered is incorrect";
			}
			else
			{
				this->nouaComanda = this->nouaComanda.substr(this->sizeTableName + 1);
				newSize = this->nouaComanda.size();
				if (this->nouaComanda.find("SET") == 0)
				{
					this->nouaComanda = this->nouaComanda.substr(4, newSize - 4);
					newSize = this->nouaComanda.size();
					int index;
					if (this->nouaComanda.find_first_of("="))
					{
						index = this->nouaComanda.find_first_of("=");
						this->columnName1 = new char[index];
						strcpy(this->columnName1, this->nouaComanda.substr(0, index - 1).c_str());
						int i;
						for (i = 0; i < strlen(this->columnName1); i++)
						{
							if ((this->columnName1[i] < 'a' && this->columnName1[i] != '_' && (this->columnName1[i] < 48 || this->columnName1[i] > 57)) || this->columnName1[i] > 'z')
								break;
						}
						if (i != strlen(this->columnName1))
						{
							cout << endl
								<< "Please re-enter the command with a valid column name";
						}
						else
						{
							this->nouaComanda = this->nouaComanda.substr(index + 2, newSize - index - 1);
							index = this->nouaComanda.find("WHERE");
							this->value1 = new char[index];
							strcpy(this->value1, this->nouaComanda.substr(0, index - 1).c_str());
							int wclause;
							wclause = nouaComanda.find("WHERE");
							if (wclause > -2 && wclause < newSize)
							{
								this->nouaComanda = this->nouaComanda.substr(wclause + 6, newSize - wclause - 6);
								newSize = this->nouaComanda.size();
								int sizeColumnName;
								sizeColumnName = this->nouaComanda.find("=") - 1;
								if (sizeColumnName > 0 && sizeColumnName <= newSize)
								{
									this->columnName2 = new char[sizeColumnName + 1];
									strcpy(this->columnName2, this->nouaComanda.substr(0, sizeColumnName).c_str());
									ok = 1;
									for (int i = 0; i < sizeColumnName && ok == 1; i++)
									{
										if ((this->columnName2[i] < 'a' && this->columnName2[i] != '_' && (this->columnName2[i] < 48 || this->columnName2[i] > 57)) || this->columnName2[i] > 'z')
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
										newSize = this->nouaComanda.size();
										this->nouaComanda = this->nouaComanda.substr(strlen(this->columnName2) + 3, newSize - strlen(this->columnName2) - 2);
										newSize = this->nouaComanda.size();
										this->value2 = new char[newSize];
										strcpy(this->value2, this->nouaComanda.c_str());
//										if (this->value2[0] == '\"' && this->value2[strlen(this->value2) - 1] == '\"')
//										{
//											this->value2[0] = '\'';
//											this->value2[strlen(this->value2) - 1] = '\'';
//										}
										if (verifyIfExists(this->tableName))
										{
											this->updateWhere();
											cout << endl
												<< "Update is successful";
										}
										else
										{
											cout << "The table you entered doesn't exist";
										}
									}
								}
								else
									cout << endl
									<< "Please enter a valid command";
							}
							else
								cout << endl
								<< "Please enter a WHERE clause";
						}
					}
					else
						cout << endl
						<< "Please enter a valid command";
				}
				else
					cout << endl
					<< "Please enter a valid command";
			}
		}
	}
	
	UpdateCommand(const UpdateCommand& updt)
	{
		this->columnName1 = nullptr;
		this->columnName2 = nullptr;
		this->value1 = nullptr;
		this->value2 = nullptr;
		if (updt.tableName != nullptr)
			this->setTableName(updt.tableName);

		if (updt.columnName1 != nullptr)
			this->setColumnName1(updt.columnName1);
		if (updt.columnName2 != nullptr)
			this->setColumnName2(updt.columnName2);
		if (updt.value1 != nullptr)
			this->setValue1(updt.value1);
		if (updt.value2 != nullptr)
			this->setValue2(updt.value2);
		this->sizeTableName = updt.sizeTableName;
	}
	friend void operator>>(istream& input, UpdateCommand& upd);
	friend ostream& operator<<(ostream& console, UpdateCommand& updt);
	friend bool operator==(UpdateCommand updt1, UpdateCommand  updt2);
	friend bool operator>=(UpdateCommand updt1, UpdateCommand  updt2);


	//Overloading operator "[]" - DONE

//	char& operator[](int index) {
//		if (index >= 0 && index < strlen(this->tableName)) {
//			return this->tableName[index];
//		}
//		else {
//			throw new exception("The index is wrong");
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
	UpdateCommand& operator+=(UpdateCommand& name) {
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
const int UpdateCommand::updateTableMinLength = 2;
ostream& operator<<(ostream& console, UpdateCommand& updt)
{
	console << endl
		<< "-------------------------------------";
	if (updt.tableName != nullptr)
		console << endl
		<< "The name of the table you have updated is: " << updt.tableName;
	return console;
}
void operator>>(istream& input, UpdateCommand& upd)
{
	cout << "Which table would you like to update?" << endl;
	char buffer[1000];
	input >> buffer;
	if (strlen(buffer) > 1)
		upd.setTableName(buffer);
	cout << "What column would you like to update?" << endl;
	input >> buffer;
	if (strlen(buffer) > 1)
		upd.setColumnName1(buffer);
	cout << "What value would you like this column to take?" << endl;
	input >> buffer;
	if (strlen(buffer) > 1)
		upd.setValue1(buffer);
	cout << "What column would you like to take as a pivot?" << endl;
	input >> buffer;
	if (strlen(buffer) > 1)
		upd.setColumnName2(buffer);
	cout << "What value would you like to search for?" << endl;
	input >> buffer;
	if (strlen(buffer) > 1)
		upd.setValue2(buffer);
}
//Overloading operator "=="
bool operator==(UpdateCommand updt1, UpdateCommand updt2) {
	if (strcmp(updt1.tableName, updt2.tableName) == 0) {
		return true;
	}
	else {
		return false;
	}
}

//Overloading operator ">="
bool operator>=(UpdateCommand updt1, UpdateCommand updt2) {
	if (strlen(updt1.tableName) >= strlen(updt2.tableName)) {
		return true;
	}
	else {
		return false;
	}
}
