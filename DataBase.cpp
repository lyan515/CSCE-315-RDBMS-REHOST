/****************************************************************************************************************
*Filename: DataBase.cpp 	  										*
*Initial Author: Andrew Meserole										*
*Project: RDBMS													*
*Description:													*
*Source file for DataBase.h 											*
*A decription of the class and functions can be found in the header file.					*
*****************************************************************************************************************/

#include "DataBase.h"

DataBase::DataBase()
{
	dataBaseHashTable = {};
	viewHashTable = {};
}

void DataBase::createTable(string tableName, vector<pair<string, int>> attributes, vector<string> primaryKeys)
{
	unordered_map<string, Table>::const_iterator checkNameUniq = dataBaseHashTable.find(tableName);
	
	if(checkNameUniq != dataBaseHashTable.end())
	{
		Table newTable = Table(tableName, attributes, primaryKeys);
		dataBaseHashTable[tableName] = newTable;
		newTable.writeToDisk();
	}
	else
		throw "Table name already in use.";
}

void DataBase::dropTable(string tableName)
{
	unordered_map<string, Table>::const_iterator checkNameExist = dataBaseHashTable.find(tableName);
	
	if(checkNameExist != dataBaseHashTable.end())
	{
		dataBaseHashTable.erase(checkNameExist);
	}
	else
		throw "Table does not exist.";
}

Table DataBase::getTable(string tableName)
{
	unordered_map<string, Table>::const_iterator getTable = dataBaseHashTable.find(tableName);
	
	if(getTable != dataBaseHashTable.end())
	{
		return getTable->second;
	}
	else
		throw "Table does not exist.";
}

Table DataBase::setUnion(string tableName1, string tableName2)
{
	Table t = Table();	//Not how function will work just for compilation purposes
	return t;
}

Table DataBase::setDifference(string tableName1, string tableName2)
{
	Table t = Table();	//Not how function will work just for compilation purposes
	return t;	
}

Table DataBase::cossProduct(string tableName1, string tableName2)
{
	Table t = Table();	//Not how function will work just for compilation purposes
	return t;
}

void readTableFromDisk(string fileName)
{
	
}
