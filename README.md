<img src="icon.png" align="right"/>

# SQL Database Simulator

> A C++ program which claims the principles of object-oriented programming.

  This project simulates a SQL database by either receiving commands from the console, or from a file passed as an argument to the program. The commands are `case-sensitive`, and while all the keywords are written in uppercase, the name of the tables and columns will be written in lowercase. The input types have been narrowed down to `integer, float, text`.
  As soon as the program starts, the user will be provided with a menu consisting of the following options:
- Enter a command
- Import commands from a .txt file — more specifically, the one passed as an argument
- Take a brief look at the syntax
- Close the application
  
  
### Examples
```sql
1. CREATE TABLE students ((id, integer, 1000, 0), (name, text, 128, ’’), (group, text,50,’1000’)) 
```
   - the program will first parse the command
   - it will check if the table already exists in the the list of table names generated automatically when the first table is created, saved as a .txt file
   - the column description will be assigned to a text file, whose name maches the one of table.
  
```sql
2. DROP TABLE students 
```
   - after parsing the command, the program will check if the table exists
   - if so, `students.txt` will be deleted and so will all the associated files
```sql
3. DISPLAY TABLE products
```
   - similar to the **desc** command in SQL, this command displays the description of the `products` table both to the console and in a `display.txt` file which is automatically created when the first **DISPLAY** command is run, in order to keep record of all the tables displayed.
```sql  
4. INSERT INTO products VALUES (1,”Pepsi”,2.35,”summer”)
```
   - as soon as the table's existence is verified, the program will check if the types of the values passed in the list match the ones of the table's columns
   - in such event, a binary file with the name of the table will be created, in which the data will be stored
    
```sql
5. SELECT (name) FROM pharmacy_products WHERE id = 1
   SELECT (id, customer_name, discount) FROM orders WHERE id = 1
   SELECT (id, name, group_number)  FROM  students
   SELECT ALL FROM warehouse WHERE id = 1
   SELECT ALL FROM students
```
   - akin to the **SELECT** command in SQL, the existence of the table is evaluated first
   - in the event of `selection by columns`, the program begins by checking if the columns match the ones in the table description. In this case, it will search in the binary file for the value (if there's a **WHERE** clause) in the same order as they were passed in the column list
   - after the first **SELECT** command, a `select.txt` log file will be generated, and the query results will be displayed both in the file and to the console.
```sql
6. DELETE FROM customers WHERE name = “John”
```
   - if the table's existence is verified, the searching process begins by validating the column name, then looking for the value in the binary file.
   - it should be mentioned that all the records, except for the one that should be deleted, will be stored in a temporary binary file, which will later on be renamed to the original file's name.

```sql
7. UPDATE suppliers SET name = “HelpNet” WHERE id = 1001
```
   - in such manner as the previous command, the program validates the existence of the table and the columns, and uses a temporary binary file to store both the values which don't need to be modified, and the ones updated by the command.
   
```sql
8. IMPORT exam_grades exam_grades.csv
   ```
   -allows the user to insert data from a `.csv` file into the associated binary file, if the table already exists and the values from the excel file match the column types


   
