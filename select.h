#include <iostream>
#include <fstream>
using namespace std;
class SelectCommand:public Useful,public Parse,public DataManipCommands
{
	char* columnName;
	int firstParanthesis;
	int lastParanthesis;
	string columnList = "";
	char* column = nullptr;
	string type = "";
	string list = "";
	static const int createTableMinLength;
	bool existsColumn = 0;
	bool found = 0;
	static const int selectTableMinLength;
	char* value = nullptr;
	bool dummy = 0;


public:

	bool verifyIfExists(char* tableName) {
		bool temp = this->Useful::verifyIfExists(this->tableName);
		cout << "Searching for the table.." << endl;
		return temp;
	}
	void openingTableFile()
	{
		string NumeFisier = string(this->tableName);
		NumeFisier = NumeFisier.append(".txt");
		ifstream filename(NumeFisier, ios::in);
		ofstream display("Select.txt", ios::out | ios::app);
		this->found = 0;
		if (filename.is_open())
		{
			while (!filename.eof())
			{
				string nameOfExistingColumn;
				char buffer[1000];
				filename.getline(buffer, 1000);
				nameOfExistingColumn = string(buffer);
				if (nameOfExistingColumn.find(" ") != string::npos)
				{
					this->existsColumn = 1;
					nameOfExistingColumn = nameOfExistingColumn.substr(0, nameOfExistingColumn.find(" "));
				}
				if (this->column == nameOfExistingColumn)
				{
					this->found = 1;
					break;
				}
			}
			if (this->found == 0)
			{
				cout << endl
					<< "The selected column doesn't exists." << endl;
			}
			else
			{
				filename.close();
				string numeFisier = string(this->tableName) + ".bin";
				ifstream input(numeFisier, ios::in | ios::binary);
				bool ok = 0;
				int pos = 0;
				if (input.is_open())
				{
					input.seekg(0, input.end);
					int length = input.tellg(); //*sizeof(int);
					input.seekg(0, input.beg);
					while (pos < length)
					{
						ifstream filename(NumeFisier, ios::in);
						if (filename.is_open())
						{
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
									if (this->column == this->currentColumn)
									{
										cout << currentInt << " ";
										if (display.is_open())
										{
											display << currentInt << " ";
										}
										ok = 1;
									}
									pos += sizeof(currentInt);
								}
								else if (this->currentType == "float")
								{
									float currentFloat;
                                    input.read(reinterpret_cast<char*>(&currentFloat),sizeof(float));
									if (this->column == this->currentColumn)
									{
										cout << currentFloat << " ";
										if (display.is_open())
										{
											display << currentFloat << " ";
										}
										ok = 1;
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
									if (this->column == this->currentColumn)
									{
										cout << Text << " ";
										if (display.is_open())
										{
											display << Text << " ";
										}
										ok = 1;
									}
									pos += sizeof(char) * currentTextLength + sizeof(int);
								}
								if (ok == 1 && filename.eof())
								{
									filename.close();
									ok = 0;
								}
							}
						}
						if (pos == length)
						{
							input.close();
						}
					}
				}
			}
		}
		display << endl;
		display.close();
	}

	void allFrom()
	{
		string NumeFisier = string(this->tableName);
		NumeFisier = NumeFisier.append(".txt");
		ifstream filename(NumeFisier, ios::in);
		if (filename.is_open())
		{
			while (!filename.eof())
			{

				char buffer[1000];
				filename.getline(buffer, 1000);
				this->allFromColumn = string(buffer);
				this->allFromColumn = this->allFromColumn.substr(0, this->allFromColumn.find_first_of(" "));
				if (this->column)
				{
					delete[] this->column;
				}
				this->column = new char[this->allFromColumn.size() + 1];
				strcpy(this->column, this->allFromColumn.c_str());
				if (this->allFromColumn != "")
				{
					if (this->value != nullptr && this->dummy == 0) {
						this->allWithWhere();
						cout << endl;
					}
					if (this->value == nullptr) {
						this->openingTableFile();
						cout << endl;
					}

				}
				else
				{
					break;
				}
			}
			filename.close();
		}
	}

	void withWhere()
	{
		string NumeFisier = string(this->tableName);
		NumeFisier = NumeFisier.append(".txt");
		ifstream filename(NumeFisier, ios::in);
		ofstream display("Select.txt", ios::out | ios::app);
		this->found = 0;
		if (filename.is_open())
		{
			while (!filename.eof())
			{
				string nameOfExistingColumn;
				char buffer[1000];
				filename.getline(buffer, 1000);
				nameOfExistingColumn = string(buffer);
				if (nameOfExistingColumn.find(" ") != string::npos)
				{
					this->existsColumn = 1;
					nameOfExistingColumn = nameOfExistingColumn.substr(0, nameOfExistingColumn.find(" "));
				}
				if (this->column == nameOfExistingColumn)
				{
					this->found = 1;
					break;
				}
			}
			if (this->found == 0)
			{
				cout << endl
					<< "The selected column doesn't exists." << endl;
			}
			else
			{
				filename.close();

				string numeFisier = string(this->tableName) + ".bin";
				ifstream input(numeFisier, ios::in | ios::binary);
				bool ok = 0;
				int pos = 0;
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
									if (this->column == this->currentColumn)
									{
										this->columnBinary = this->columnBinary.append(to_string(currentInt));
										this->columnBinary = this->columnBinary.append(" ");

										ok = 1;
									}
									if (this->value == to_string(currentInt))
									{
										works = 1;

									}
									pos += sizeof(currentInt);
								}
								else if (this->currentType == "float")
								{
									float currentFloat;
									//input.read((char*)&currentFloat, sizeof(float));
                                    input.read(reinterpret_cast<char*>(&currentFloat),sizeof(float));
									if (this->column == this->currentColumn)
									{
										this->columnBinary = this->columnBinary.append(to_string(currentFloat));
										this->columnBinary = this->columnBinary.append(" ");
										ok = 1;
									}
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

									if (this->column == this->currentColumn)
									{

										this->columnBinary = this->columnBinary.append(Text);
										this->columnBinary = this->columnBinary.append(" ");
										ok = 1;
									}
									if (strcmp(this->value, bufferText) == 0)
									{
										works = 1;

									}

									pos += sizeof(char) * currentTextLength + sizeof(int);
								}
								if (ok == 1 && filename.eof())
								{
									filename.close();
									ok = 0;
								}
							}
							if (works == 1)
							{
								if (display.is_open()) {
									display << this->columnBinary << endl;
									display << "-------------------" << endl;
								}
								cout << this->columnBinary << endl;

								works = 0;
							}
							columnBinary = "";
						}
						if (pos == length)
							input.close();
					}
				}
			}
		}
		display.close();
	}
	void allWithWhere()
	{
		string NumeFisier = string(this->tableName);
		NumeFisier = NumeFisier.append(".txt");
		ifstream filename(NumeFisier, ios::in);
		ofstream display("Select.txt", ios::out | ios::app);
		this->found = 0;
		if (filename.is_open())
		{
			while (!filename.eof())
			{
				string nameOfExistingColumn;
				char buffer[1000];
				filename.getline(buffer, 1000);
				nameOfExistingColumn = string(buffer);
				if (nameOfExistingColumn.find(" ") != string::npos)
				{
					this->existsColumn = 1;
					nameOfExistingColumn = nameOfExistingColumn.substr(0, nameOfExistingColumn.find(" "));
				}
				if (this->column == nameOfExistingColumn)
				{
					this->found = 1;
					break;
				}
			}
			if (this->found == 0)
			{
				cout << endl
					<< "The selected column doesn't exists." << endl;
			}
			else
			{
				filename.close();

				string numeFisier = string(this->tableName) + ".bin";
				ifstream input(numeFisier, ios::in | ios::binary);
				bool ok = 0;
				int pos = 0;
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
									this->columnBinary = this->columnBinary.append(to_string(currentInt));
									this->columnBinary = this->columnBinary.append(" ");
									if (this->value == to_string(currentInt))
									{
										works = 1;

									}
									if (this->column == this->currentColumn)
									{
										ok = 1;
									}

									pos += sizeof(currentInt);
								}
								else if (this->currentType == "float")
								{
									float currentFloat;
                                    input.read(reinterpret_cast<char*>(&currentFloat),sizeof(float));
									this->columnBinary = this->columnBinary.append(to_string(currentFloat));
									this->columnBinary = this->columnBinary.append(" ");
									if (this->value == to_string(currentFloat))
									{
										works = 1;

									}
									if (this->column == this->currentColumn)
									{
										ok = 1;
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
									this->columnBinary = this->columnBinary.append(Text);
									this->columnBinary = this->columnBinary.append(" ");
									if (strcmp(this->value, bufferText) == 0)
									{
										works = 1;
									}
									if (this->column == this->currentColumn)
									{
										ok = 1;
									}

									pos += sizeof(char) * currentTextLength + sizeof(int);
								} 
								if (ok == 1 && filename.eof())
								{
									filename.close();
									ok = 0;
								}
							}
							if (works == 1)
							{
								if (display.is_open()) {
									display << this->columnBinary << endl;
									display << "-------------------" << endl;
								}
								cout << this->columnBinary << endl;
								works = 0;
								this->dummy = 1;
							}
							columnBinary = "";
						}
						if (pos == length)
						{
							input.close();
						}

					}
				}
			}
		}
		display.close();
	}

	SelectCommand()
	{
		this->tableName = nullptr;
		this->sizeTableName = 0;
		this->columnName = nullptr;
	}
	SelectCommand(const char* tableName, int sizeTableName, const char* columnName, string columnList)
	{
		this->tableName = nullptr;
		this->columnName = nullptr;
		cout << "new SELECT command" << endl;
		if (tableName != nullptr)
		{
			this->setTableName(tableName);
			this->sizeTableName = sizeTableName;
			this->setColumnName(columnName);
			this->columnList = columnList;
		}
		else
		{
			cout << "Please enter valid values" << endl;
		}
	}
	void setTableName(const char* tableName)
	{
		if (SelectCommand::selectTableMinLength <= strlen(tableName))
		{
			if (this->tableName != nullptr)
			{
				delete[] this->tableName;
			}
			this->tableName = new char[strlen(tableName) + 1];
			strcpy(this->tableName, tableName);
		}
	}

	void setColumnName(const char* columnName)
	{
		if (this->columnName != nullptr)
		{
			delete[] this->columnName;
			this->columnName = new char[strlen(columnName) + 1];
			strcpy(this->columnName, columnName);
		}
		else
		{
			cout << "you need to enter a valid column name" << endl;
		}
	}
	void setColumn(const char* column)
	{
		if (this->column != nullptr)
		{
			delete[] this->column;
			this->column = new char[strlen(column) + 1];
			strcpy(this->column, column);
		}
		else
		{
			cout << "you need to enter a valid column list" << endl;
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
		int backr = 0;
		if (this->nouaComanda.find('\r') != string::npos)
		{
			backr = this->nouaComanda.find('\r');
			this->nouaComanda = this->nouaComanda.substr(0, backr);
		}
		if (this->nouaComanda.find("ALL FROM") == 0)
		{
			this->nouaComanda = this->nouaComanda.substr(9, newSize - 9);
			newSize = this->nouaComanda.size();
			this->sizeTableName = this->nouaComanda.find_first_of("WHERE") - 1;
			if (this->sizeTableName > 0 && this->sizeTableName <= newSize)
			{
				this->tableName = new char[this->sizeTableName + 1];
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
					cout << "The name you entered isn't correct";
				}
				else
				{
					this->nouaComanda = this->nouaComanda.substr(this->sizeTableName + 7, newSize - this->sizeTableName - 7);
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
								this->allFrom();
							}

							cout << "Select is successful";
						}
					}
					else
						cout << "Please enter a valid command";
				}
			}
			else if (this->sizeTableName == -2)
			{
				this->tableName = new char[newSize + 1];
				strcpy(this->tableName, this->nouaComanda.c_str());
				bool ok = 1;
				for (int i = 0; i < newSize - 1 && ok == 1; i++)
				{
					if ((this->tableName[i] < 'a' && this->tableName[i] != '_' && (this->tableName[i] < 48 || this->tableName[i] > 57)) || this->tableName[i] > 'z')
					{
						ok = 0;
						break;
					}
				}
				if (ok == 0)
				{
					cout << "The table name you entered isn't correct";
				}
				else
				{
					if (verifyIfExists(this->tableName))
					{
						this->allFrom();
					}
				}
			}
			else
				cout << "Please enter a valid command";
		}
		else
		{
			if (this->nouaComanda.find("(") == 0)
			{
				this->firstParanthesis = 0;
				this->lastParanthesis = this->nouaComanda.find_last_of(")");
				if (!(this->lastParanthesis != 0) || !(this->firstParanthesis > -1) || !(this->lastParanthesis > this->firstParanthesis))
				{
					cout << endl
						<< "You need to select at least one column.";
				}
				else
				{
					this->columnList = this->nouaComanda.substr(this->firstParanthesis + 1, this->lastParanthesis - this->firstParanthesis - 1);
					this->nouaComanda = this->nouaComanda.substr(this->lastParanthesis + 2, newSize - this->lastParanthesis - 2);

					if (this->nouaComanda.find("FROM") == 0)
					{
						this->nouaComanda = this->nouaComanda.substr(5, newSize - 5);
						newSize = this->nouaComanda.size();
						this->sizeTableName = this->nouaComanda.find_first_of("WHERE") - 1;
						if (this->sizeTableName > 0 && this->sizeTableName <= newSize)
						{
							this->tableName = new char[this->sizeTableName + 1];
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
								cout << "The name you entered isn't correct";
							}
							else
							{
								if (verifyIfExists(this->tableName)) {

									this->nouaComanda = this->nouaComanda.substr(this->sizeTableName + 7, newSize - this->sizeTableName - 7);
									newSize = this->nouaComanda.size();
									int sizeColumnName;
									sizeColumnName = this->nouaComanda.find("=") - 1;
									if (sizeColumnName > 0 || sizeColumnName <= newSize)
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
												const string delims{ ", " };
												int beginning, pos = 0;
												while ((beginning = this->columnList.find_first_not_of(delims, pos)) != string::npos)
												{
													pos = this->columnList.find_first_of(delims, beginning);

													if (this->column)
													{
														delete[] this->column;
													}
													if (pos == -1 && beginning >= 0)
													{
														int length = this->columnList.size();
														this->column = new char[length - beginning + 1];
														strcpy(this->column, this->columnList.substr(beginning, length - beginning + 1).c_str());
													}
													else
													{
														this->column = new char[pos - beginning + 1];
													}
													strcpy(this->column, this->columnList.substr(beginning, pos - beginning).c_str());
													this->withWhere();
													if (this->found == 0)
													{
														break;
													}
												}
											}
											if (this->found)
											{
												cout << endl
													<< "Select is successful" << endl;
											}
											else
											{
												cout << endl
													<< "Wrong input ";
											}
										}

									}
									else
										cout << "Please enter a valid command";
								}
								else {
									cout << "The table doesn't exists.";
								}
							}
						}
						else if (this->sizeTableName == -2)
						{
							this->tableName = new char[newSize + 1];
							strcpy(this->tableName, this->nouaComanda.c_str());
							bool ok = 1;
							for (int i = 0; i < newSize - 1 && ok == 1; i++)
							{
								if ((this->tableName[i] < 'a' && this->tableName[i] != '_' && (this->tableName[i] < 48 || this->tableName[i] > 57)) || this->tableName[i] > 'z')
								{
									ok = 0;
									break;
								}
							}
							if (ok == 0)
							{
								cout << "The table name you entered isn't correct";
							}
							else
							{
								if (verifyIfExists(this->tableName))
								{
									const string delims{ ", " };
									int beginning, pos = 0;
									while ((beginning = this->columnList.find_first_not_of(delims, pos)) != string::npos)
									{
										pos = this->columnList.find_first_of(delims, beginning);

										if (this->column)
										{
											delete[] this->column;
										}
										if (pos == -1 && beginning >= 0)
										{
											int length = this->columnList.size();
											this->column = new char[length - beginning + 1];
											strcpy(this->column, this->columnList.substr(beginning, length - beginning + 1).c_str());
										}
										else
										{
											this->column = new char[pos - beginning + 1];
										}
										strcpy(this->column, this->columnList.substr(beginning, pos - beginning).c_str());
										this->openingTableFile();
										cout << endl;

										if (this->found == 0)
										{
											break;
										}

									}
								}
								else
								{
									cout << "The table doesn't exists";
								}
							}
						}
						else
							cout << "Please enter a valid command";
					}
				}
			}
			else
				cout << "Please enter a valid command";
		}
	}
	~SelectCommand()
	{
		if (this->tableName)
		{
			delete[] this->tableName;
		}
		if (this->columnName)
		{
			delete[] this->columnName;
		}
		if (this->column)
		{
			delete[] this->column;
		}
		if (this->value)
		{
			delete[] this->value;
		}
	};

	SelectCommand(const SelectCommand& sel)
	{
		this->tableName = nullptr;
		this->columnName = nullptr;
		this->column = nullptr;
		this->value = nullptr;
		if (sel.tableName != nullptr)
			this->setTableName(sel.tableName);
		this->sizeTableName = sel.sizeTableName;
		if (sel.columnName != nullptr)
			this->setColumnName(sel.columnName);
		this->columnList = sel.columnList;
	}
	friend ostream& operator<<(ostream& console, SelectCommand& select);
	friend void operator>>(istream& input, SelectCommand& sel);
	friend bool operator==(SelectCommand sel1, SelectCommand  sel2);
	friend bool operator>=(SelectCommand sel1, SelectCommand  sel2);
	//Overloading operator "[]" - DONE

//	char& operator[](int index) {
//		if (index >= 0 && index < strlen(this->tableName)) {
//			return this->tableName[index];
//		}
//		else {
//			cout<<"The index is wrong";
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
	SelectCommand& operator+=(SelectCommand& name) {
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
const int SelectCommand::selectTableMinLength = 2;
ostream& operator<<(ostream& console, SelectCommand& select)
{
	console << endl
		<< "-------------------------------------";
	if (select.tableName != nullptr)
		console << endl
		<< "The name of the table you have selected from is: " << select.tableName << endl;

	return console;
}
void operator>>(istream& input, SelectCommand& sel)
{
	cout << "Which table would you like to select from?" << endl;
	char buffer[1000];
	input >> buffer;
	if (strlen(buffer) > 1)
		sel.setTableName(buffer);
	cout << "Successfully selected" << endl;
}
bool operator==(SelectCommand sel1, SelectCommand sel2) {
	if (strcmp(sel1.tableName, sel2.tableName) == 0) {
		return true;
	}
	else {
		return false;
	}
}

//Overloading operator ">="
bool operator>=(SelectCommand sel1, SelectCommand sel2) {
	if (strlen(sel1.tableName) >= strlen(sel2.tableName)) {
		return true;
	}
	else {
		return false;
	}
}
