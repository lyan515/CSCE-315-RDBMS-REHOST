#include "Parser.h"
//should we include DataBase.h here or in Parser.h?

using namespace std;

void printVector (vector<string> vec)
{	
	cout << "{ ";
	for (int i = 0; i < vec.size(); ++i)
	{
		if (i != vec.size() - 1)
			cout << vec[i] << " , ";
		else
			cout << vec[i] << " }";
	}
	cout << endl;
}

void printVector (vector<pair<string, int>> vec)
{	
	cout << "{ ";
	for (int i = 0; i < vec.size(); ++i)
	{
		if (i != vec.size() - 1)
			cout << vec[i].first << " , " << vec[i].second << " | ";
		else
			cout << vec[i].first << " , " << vec[i].second << " }";
	}
	cout << endl;
}

void Parser::runOnCommandLine()
{
	db = new DataBase();
	string command;
	while(1)
	{
		cout << ">";
		getline(cin, command);
		commandOrQuery(command);
	}
}

vector<string> Parser::stringToTokens (string boolExpression) 
{
	// result
	vector<string> tokens;
	// remove new lines, tabs, and spaces
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), '\r'), boolExpression.end());
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), '\n'), boolExpression.end());
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), '\t'), boolExpression.end());
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), '\"'), boolExpression.end());
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), ' '), boolExpression.end());
	// convert string to tokens
	int current = 0;
	string token = "";
	while (current != boolExpression.length()) {
		 if (boolExpression[current] != '&' && boolExpression[current] != '|'
		 	&& boolExpression[current] != '(' && boolExpression[current] != ')')
		 {
		 	token += boolExpression[current++];
		 }
		 else
		 {
		 	if (token.length() > 0)
		 	{
		 		tokens.push_back(token);
		 	}
		 	token = "";
		 	if (boolExpression[current] == '(' || boolExpression[current] == ')')
		 	{
		 		token += boolExpression[current++];
		 		tokens.push_back(token);
		 		token = "";
		 	}
		 	else 
		 	{
		 		token += boolExpression[current++];
		 		token += boolExpression[current++];
		 		tokens.push_back(token);
		 		token = "";
		 	}
		 }
	}
	if (token.length() > 0)
	{
		tokens.push_back(token);
	}
	return tokens;
}

vector<string> Parser::convertBoolExpression (string boolExpression)
{
	vector<string> tokens = stringToTokens(boolExpression);
	vector<string> postfix;
	stack<string> opStack;
	for (int i = 0; i < tokens.size(); ++i)
	{
		if (tokens[i][0] != '&' && tokens[i][0] != '|'
			&& tokens[i][0] != '(' && tokens[i][0] != ')')
		{
			postfix.push_back(tokens[i]);
		}
		else {
			if (tokens[i] == "(")
			{
				opStack.push(tokens[i]);
			}
			else if (tokens[i] == ")")
			{
				while (!opStack.empty() && opStack.top() != "(")
				{
					string op = opStack.top();
					opStack.pop();
					postfix.push_back(op);
				}
				if (!opStack.empty())
				{
					opStack.pop();
				}				
			}
			else if (!opStack.empty() && opStack.top() != "(")
			{
				string op = opStack.top();
				opStack.pop();
				opStack.push(tokens[i]);
				postfix.push_back(op);
			}
			else
			{
				opStack.push(tokens[i]);
			}
		}
	}
	// check if any operators are left on the opStack
	while (!opStack.empty())
	{
		postfix.push_back(opStack.top());
		opStack.pop();
	}
	return postfix;
}

void Parser::commandOrQuery(string instruction)
{
	instruction.erase(remove(instruction.begin(), instruction.end(), '\r'), instruction.end());
	instruction.erase(remove(instruction.begin(), instruction.end(), '\n'), instruction.end());
	instruction.erase(remove(instruction.begin(), instruction.end(), '\t'), instruction.end());
	instruction.erase(remove(instruction.begin(), instruction.end(), ';'), instruction.end());
	instruction.erase(remove(instruction.begin(), instruction.end(), ' '), instruction.end());
	if(instruction.find("<-") != string::npos)// <- found 
	{
		queryParse(instruction);
	}
	else
	{
		commandParse(instruction);
	}
}	


void Parser::commandParse(string instruction)//parses a command
{
	if(instruction.find("OPEN") != string::npos)// <- found 
	{
		instruction.erase(0,5);
		commandOpen(instruction);// may change if the "OPEN " part of the string needs to be removed
	}
	else if(instruction.find("CLOSE") != string::npos)
	{
		instruction.erase(0,6);
		commandClose(instruction);// may change if the "CLOSE " part of the string needs to be removed
	}
	else if(instruction.find("WRITE") != string::npos)
	{
		instruction.erase(0,6);
		commandWrite(instruction);// may change if the "WRITE " part of the string needs to be removed
	}
	else if(instruction.find("SHOW") != string::npos)
	{
		instruction.erase(0,4);
		commandShow(instruction);// may change if the "SHOW " part of the string needs to be removed
	}
	else if(instruction.find("EXIT") != string::npos)
	{
		instruction.erase(0,5);
		commandExit();// may change if the "EXIT " part of the string needs to be removed
	}
	else if(instruction.find("CREATETABLE") != string::npos)
	{
		instruction.erase(0,11);
		commandCreate(instruction);
	}
	else if(instruction.find("UPDATE") != string::npos)
	{
		instruction.erase(0,6);
		commandUpdate(instruction);
	}
	else if(instruction.find("INSERTINTO") != string::npos)//since whitespace will be eliminated
	{
		instruction.erase(0,10);//removes INSERT INTO
		commandInsert(instruction);
	}
	else if(instruction.find("DROP") != string::npos)
	{
		instruction.erase(0,5);
		commandDrop(instruction);// may change if the "DROP " part of the string needs to be removed
	}
	else if(instruction.find("DELETE") != string::npos)
	{
		instruction.erase(0,6);
		commandDelete(instruction);
	}
	else
	{
		cout << "Not a valid command\n";
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////COMMAND FUNCTIONS////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
void Parser::commandOpen(string filename)
{
	filename = filename.substr(0, filename.size()-1);//eliminates semicolon at end of command
	db->readTableFromDisk(filename);//under the impression that the filename string has no ""
}

void Parser::commandClose(string tablename)
{
	tablename = tablename.substr(0, tablename.size()-1);//eliminates semicolon at end of command
	db->dropTable(tablename);//works the exact same way as commandDrop()
}

void  Parser::commandWrite(string tableName)
{
	tableName = tableName.substr(0, tableName.size()-1);//eliminates semicolon at end of command
	db->writeTableToDisk(tableName);
}

void Parser::commandExit()//For Travis
{
	int success = db->exit();
	if (success != -1)
	{
		cout << "Saved " << success << " database tables" << endl;
		exit(EXIT_SUCCESS);
	}
	else
	{
		cout << "Failed to save database tables" << endl;
		exit(EXIT_FAILURE);
	}
}

void Parser::commandShow(string tablename)
{
	//tablename = tablename.substr(0, tablename.size()-1);//eliminates semicolon at end of command
	cout << db->showTable(tablename) << endl;
}

vector<string> Parser::commandPrimKeys(string instr)//example input (name,kind)
{
	instr.erase(0,1);//eliminates left parenthese
	instr = instr.substr(0,instr.size()-1);//eliminate right parenthese
	
	stringstream inss(instr);
	string key;
	vector<string> keyList;
	
	while(getline(inss, key, ','))
	{
		keyList.push_back(key);
	}
	
	return keyList;
	
}

vector< pair<string, int> > Parser::commandAttributes(string instr)
{
	instr = instr.erase(0,1);//eliminates left parenthese
	instr = instr.substr(0,instr.size()-1);//eliminate right parenthese
	
	stringstream inss(instr);
	string pair;
	string eOne;//element 1 of pair
	int eTwo;//element 2 of pair
	vector< std::pair<string, int> > atList;
	
	while(getline(inss, pair, ','))
	{
		if(pair.find("INTEGER") != string::npos)
		{
			
			eOne = pair.substr(0,pair.find("INTEGER")); //get the name of attribute
			eTwo = -1;//signifies that the attribute is an integer
			std::pair<string, int> attr = make_pair(eOne,eTwo);
			atList.push_back(attr);
			
		}
		else if(pair.find("VARCHAR") != string::npos)
		{
			eOne = pair.substr(0,pair.find("VARCHAR")); //get the name of attribute
			eTwo = stoi(pair.substr(pair.find("(")+1,pair.find(")")-1));
			std::pair<string, int> attr = make_pair(eOne,eTwo);
			atList.push_back(attr);
			
		}
	}
	
	return atList;
}

void Parser::commandCreate(string instr)// We'll need 
{
	string instruction = instr;
	string name;
	vector<std::pair<string, int> > attributes;
	vector<string> primKeys;
	name = instruction.substr(0, instruction.find("("));//get name of table
	instruction.erase(0,instruction.find("("));//erases section of the line containing the name
	//function here for getting attributes
	int pIndex = instruction.find("PRIMARYKEY");
	string aInstr = instruction.substr(0, pIndex);//sub string of attribute list
	instruction.erase(0,pIndex + 10);//erase everything up until PRIMARYKEY
	attributes = commandAttributes(aInstr);//get attributes
	
	
	if (pIndex == string::npos)
	{
		throw "PRIMARYKEY not found";
	}
	//primKeys = commandPrimKeys(instruction);//get the primary keys
	primKeys = extractAttributes(instruction);
	db->createTable(name, attributes, primKeys);
	
}



// split function from here: http://code.runnable.com/VHb0hWMZp-ws1gAr/splitting-a-string-into-a-vector-for-c%2B%2B
vector<string> split(string str, char delimiter)
{
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
  
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
  
  return internal;
}

void Parser::commandUpdate(string instr)
{
	string name;//name of relation
	
	name = instr.substr(0, instr.find("SET"));//get name of table
	instr.erase(0, instr.find("SET")+3);//erase previous part of string
	
	string sInstr = instr.substr(0, instr.find("WHERE"));//get set commands
	instr.erase(0, instr.find("WHERE")+5);//erase previous part of string
	
	sInstr.erase(remove(sInstr.begin(), sInstr.end(), '('), sInstr.end()); 
	sInstr.erase(remove(sInstr.begin(), sInstr.end(), ')'), sInstr.end());
	
	vector<string> sets = split(sInstr, ',');//splits line into sets for parsing below
	vector<string> desired;//desired attributes
	vector<string> values;//values to be changed
	for(int i = 0; i < sets.size(); i++)
	{
		desired.push_back(sets[i].substr(0, sets[i].find("=")));//gets our desired attributes
		values.push_back(sets[i].substr(sets[i].find("=")+1, sets[i].length()-1));//gets our values
	}
	instr.erase(remove(instr.begin(), instr.end(), '('), instr.end()); //both lines eliminate parentheses 
	instr.erase(remove(instr.begin(), instr.end(), ')'), instr.end());
	instr.erase(remove(instr.begin(), instr.end(), '"'), instr.end());//eliminates quotes
	vector<string> cond = convertBoolExpression(instr);//condition
	
	db->updateTableRecord(name, desired, values, cond);
}

vector<string> Parser::extractAttributes (string attributeList)
{
	
	attributeList.erase(remove(attributeList.begin(), attributeList.end(), '('), attributeList.end()); 
	attributeList.erase(remove(attributeList.begin(), attributeList.end(), ')'), attributeList.end());
	attributeList.erase(remove(attributeList.begin(), attributeList.end(), '\"'), attributeList.end());
	vector<string> attributes = split(attributeList, ',');
	return attributes;
}

void Parser::commandInsert(string instr)
{
	instr.erase(remove(instr.begin(), instr.end(), ';'), instr.end());
	string name;
	vector<string> attributes;
	int attributesIndex = instr.find("VALUESFROMRELATION");
	if (attributesIndex != string::npos)
	{
		name = instr.substr(0, attributesIndex);
		attributesIndex += 18;
		string expression = instr.substr(attributesIndex);
		Table *result = queryParseHelper(expression);
		db->getTable(name)->insertRecord(result);
	}
	else
	{
		attributesIndex = instr.find("VALUESFROM");
		if (attributesIndex == string::npos)
		{
			throw "VALUESFROM not found";
		}
		else
		{
			name = instr.substr(0, attributesIndex);
			attributesIndex += 10;
			attributes = extractAttributes(instr.substr(attributesIndex));
			db->getTable(name)->insertRecord(attributes);
		}
	}
}

int getConditionEnd (string instr)
{
	int conditionEnd = 0;
	int parenCount = 0;
	do
	{
		if (instr[conditionEnd] == '(')
		{
			parenCount++;
		}
		else if (instr[conditionEnd] == ')')
		{
			parenCount--;
		}
		conditionEnd++;
	} while (parenCount > 0);
	return conditionEnd;
}

void Parser::queryParse(string instr)
{
	string name;
	if (instr.find("<-") != string::npos)
	{
		name = instr.substr(0, instr.find("<-"));
	}
	else
	{
		name = "tmp";
	}
	if (instr.find("select") != string::npos)
	{
		instr.erase(0, instr.find("select") + 6);
		int conditionEnd = getConditionEnd(instr);
		cout << instr << endl;
		cout << conditionEnd << endl;
		string conditionString = instr.substr(1, conditionEnd - 2);
		cout << "conditions: " << conditionString << endl;
		vector<string> conditions = convertBoolExpression(conditionString);
		instr.erase(0, conditionEnd);
		cout << instr << endl;
		Table *tmp = queryParseHelper(instr);
		Table *result = tmp->select(name, conditions);
		db->createView(result);
		delete tmp;
	}
	else if (instr.find("project") != string::npos)
	{
		instr.erase(0, instr.find("project") + 7);
		cout << instr << endl;
		int openParen = instr.find("(");
		int closeParen = instr.find(")");
		string attributeListString = instr.substr(openParen + 1, closeParen - openParen - 1);
		cout << "attributeListString: " << attributeListString << endl;
		vector<string> attributes = split(attributeListString, ',');
		cout << "attributes: " << endl;
		printVector(attributes);
		instr.erase(0, closeParen + 1);
		cout << instr << endl;
		Table *tmp = queryParseHelper(instr);
		Table *result = tmp->project(name, attributes);
		db->createView(result);
		delete tmp;
	}
	else if (instr.find("rename") != string::npos)
	{
		instr.erase(0, instr.find("rename") + 6);
		cout << instr << endl;
		int openParen = instr.find("(");
		int closeParen = instr.find(")");
		string newNameListString = instr.substr(openParen + 1, closeParen - openParen - 1);
		cout << "newNameListString: " << newNameListString << endl;
		vector<string> newNames = split(newNameListString, ',');
		cout << "newNames: " << endl;
		printVector(newNames);
		instr.erase(0, closeParen + 1);
		cout << instr << endl;
		Table *tmp = queryParseHelper(instr);
		Table *result = tmp->rename(name, newNames);
		db->createView(result);
		delete tmp;
	}
	else if (instr.find("+") != string::npos)
	{

	}
	else if (instr.find("-") != string::npos)
	{

	}
	else if (instr.find("*") != string::npos)
	{

	}
	else if (instr.find("JOIN") != string::npos)
	{

	}
	else
	{
		cout << "Not a valid command" << endl;
	}
}

Table* Parser::queryParseHelper(string instr)
{
	cout << "queryParseHelper passed string " << instr << endl;
	string name;
	if (instr.find("<-") != string::npos)
	{
		name = instr.substr(0, instr.find("<-"));
	}
	else
	{
		name = "tmp";
	}
	if (instr.find("select") != string::npos)
	{

		instr.erase(0, instr.find("select") + 6);
		int conditionEnd = getConditionEnd(instr);
		cout << instr << endl;
		cout << conditionEnd << endl;
		string conditionString = instr.substr(1, conditionEnd - 2);
		cout << "conditions: " << conditionString << endl;
		vector<string> conditions = convertBoolExpression(conditionString);
		instr.erase(0, conditionEnd);
		cout << instr << endl;
		Table *tmp = queryParseHelper(instr);
		Table *result = tmp->select("tmp", conditions);
		delete tmp;
		return result;
	}
	else if (instr.find("project") != string::npos)
	{
		instr.erase(0, instr.find("project") + 7);
		cout << instr << endl;
		int openParen = instr.find("(");
		int closeParen = instr.find(")");
		string attributeListString = instr.substr(openParen + 1, closeParen - openParen - 1);
		cout << "attributeListString: " << attributeListString << endl;
		vector<string> attributes = split(attributeListString, ',');
		cout << "attributes: " << endl;
		printVector(attributes);
		instr.erase(0, closeParen + 1);
		cout << instr << endl;
		Table *tmp = queryParseHelper(instr);
		Table *result = tmp->project(name, attributes);
		delete tmp;
		return result;
	}
	else if (instr.find("rename") != string::npos)
	{
		instr.erase(0, instr.find("rename") + 6);
		cout << instr << endl;
		int openParen = instr.find("(");
		int closeParen = instr.find(")");
		string newNameListString = instr.substr(openParen + 1, closeParen - openParen - 1);
		cout << "newNameListString: " << newNameListString << endl;
		vector<string> newNames = split(newNameListString, ',');
		cout << "newNames: " << endl;
		printVector(newNames);
		instr.erase(0, closeParen + 1);
		cout << instr << endl;
		Table *tmp = queryParseHelper(instr);
		Table *result = tmp->rename(name, newNames);
		delete tmp;
		return result;
	}
	else if (instr.find("+") != string::npos)
	{

	}
	else if (instr.find("-") != string::npos)
	{

	}
	else if (instr.find("*") != string::npos)
	{

	}
	else if (instr.find("JOIN") != string::npos)
	{

	}
	else
	{
		if(db->containsTable(instr))
		{
			return db->getTable(instr);
		}
		else if(db->containsView(instr))
		{
			return db->getView(instr);
		}
		else
		{
			throw "Query Parse failed";
		}
	}
}

void Parser::commandDrop(string tablename)
{
	tablename = tablename.substr(0, tablename.size()-1);//eliminates semicolon at end of command
	db->dropTable(tablename);
}

void Parser::commandDelete(string instr)
{
	string name;
	name = instr.substr(0, instr.find("WHERE"));//get name of table
	instr.erase(0, instr.find("("));
	vector<string> delVect = convertBoolExpression(instr);
	db->deleteFromTable(name, delVect);
}

