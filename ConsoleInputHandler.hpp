//BSD 3-Clause License
//Copyright(c) 2021, the SingleHeaderLibs developers
#pragma once
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <assert.h> 
#include <iterator>
#include <future>
#include <pthread.h>

using namespace std;
struct commandStruct 
{
		string commandName;
		vector<string> aliases;
		string description;
		void (*callback)(vector<string>);
};


 class CIH
{

public:
	 CIH();
	 ~CIH();
	 inline static void initciHandler(string & output);
	 inline static void* initciHandler(void* output);
	 inline static int initciHandlerOnNewThread(string & output );
	 std::vector<commandStruct> commands;
	 static void addCommand(string cmd, vector<string> aliases, string description, void (*callback)(vector<string>));
private:
	 std::string inputText = "";
	 std::string currentCommand = "";
	 static void handleCommands(string command,vector<string>args);
	 static void helpCommand(vector<string> args);
};
static CIH cih;
CIH::CIH()
{
}

CIH::~CIH()
{
}

int CIH::initciHandlerOnNewThread(string& output) {

	pthread_t thread;
	int ret;
	ret = pthread_create(&thread, NULL, initciHandler, (void*)&output);
	if (ret != 0) {
		printf("pthread_create() failed\n");
		exit(0);
	}
	return ret;
}
void* CIH::initciHandler(void* output) {

	CIH::addCommand("help", { "" }, "", helpCommand);
	string outputString = *(static_cast<string*>(output));

	cout << outputString;
	while (true)
	{
		std::this_thread::sleep_for(80ms);
		cih.currentCommand = "";

		getline(cin, cih.inputText);
		for (auto c : cih.inputText) {
			if (c != ' ')
				cih.currentCommand += c;
			else
				if (cih.currentCommand.size() > 0)
					break;
		}

		string temp;
		int toRemove = 0;

		for (auto c : cih.currentCommand) {
			if (c == ' ')
				toRemove++;
			else
				break;
		}
		if (toRemove > 0) {
			cih.currentCommand.erase(cih.inputText.begin(), cih.inputText.begin() + (toRemove - 1));
		}
		temp = cih.currentCommand + " ";
		cih.inputText.erase(cih.inputText.begin(), cih.inputText.begin() + (temp.size() - 1));
		temp = "";
		auto args = cih.inputText;
		vector<string>argsArray;

		temp = "";
		for (size_t i = 0; i < args.length(); i++) {
			char c = args[i];
			if (c == ' ') {
				if (temp.size() > 0) {
					argsArray.push_back(temp);
					temp = "";
				}
			}
			else if (c == '\"') {
				i++;
				while (args[i] != '\"') { temp += args[i]; i++; }
			}
			else {
				temp += c;
			}

		}
		argsArray.push_back(temp);
		handleCommands(cih.currentCommand, argsArray);

		cout << outputString;
	}
}

void CIH::initciHandler(string & output) {

	CIH::addCommand("help", { "" }, "", helpCommand);
	cout << output;
	while (true)
	{
		cih.currentCommand = "";

		getline(cin, cih.inputText);
		for (auto c : cih.inputText) {
			if (c != ' ')	
				cih.currentCommand += c;
			else
				if(cih.currentCommand.size() > 0)
					break;
		}

		string temp;
		temp = cih.currentCommand + " ";
		cih.inputText.erase(cih.inputText.begin(), cih.inputText.begin() + (temp.size()-1));
		temp = "";
		int toRemove = 0;
		for (auto c : cih.currentCommand) {
			if (c == ' ')
				toRemove++;
			else
				break;
		}
		if (toRemove > 0) {
			cih.currentCommand.erase(cih.inputText.begin(), cih.inputText.begin() + (toRemove -1));
		}
		auto args = cih.inputText;
		vector<string>argsArray;
		temp = "";
		for (size_t i = 0; i < args.length(); i++) {
			char c = args[i];
			if (c == ' ') {
				argsArray.push_back(temp);
				temp = "";
			}
			else if (c == '\"') {
				i++;
				while (args[i] != '\"') { temp += args[i]; i++; }
			}
			else {
				temp += c;
			}
		}
		handleCommands(cih.currentCommand,argsArray);

		cout << output;
	}

}

void CIH::handleCommands(string command,vector<string>args) {
	for (auto c : cih.commands) {
		if (c.commandName == command) {
			c.callback(args);
			return;
		}
		else
		{
			for (auto b : c.aliases) {
				if (b == command && !(b == string(""))) {
					c.callback(args);
					return;
				}
			}
		}
	}
	cout << "Unkown command '" << command << "'. Please type 'help' for list of commands\n";
	return;
}

void CIH::addCommand(string cmd,vector<string> aliases, string description, void (*callback)(vector<string>)) {
	if (!(cih.commands.empty())) {
		for (auto& c : cih.commands) {
			string temp = "";
			if (c.commandName == "help") {
				cout << "Cannnot overrite help command" << endl;
				assert(!(c.commandName == "help"));
			}
			else if (c.commandName == cmd) {
				commandStruct command;
				command.commandName = cmd;
				command.aliases = aliases;
				command.description = description;
				command.callback = callback;
				c.aliases = aliases;
				c.callback = callback;
				c.description = description;
				break;
			}
			else {
				for (auto b : c.aliases) {
					for (auto i : aliases)
					{
						if (i == b && !(b == "") ) {
							cout << "'" << cmd << "' " << "Aliase matches existing alias on command '" << c.commandName <<  "'" << endl;
							assert(!(i == b));
							exit(0);
						}
					}
				}
				commandStruct command;
				command.commandName = cmd;
				command.aliases = aliases;
				command.description = description;
				command.callback = callback;
				cih.commands.push_back(command);
				return;

			}

		}

	}
	else {
		commandStruct command;
		command.commandName = cmd;
		command.aliases = aliases;
		command.description = description;
		command.callback = callback;
		cih.commands.push_back(command);
		return;
	}
	
}

void CIH::helpCommand(vector<string> args) {
	for (auto c : cih.commands) {
		string temp = "";

		if (!(c.commandName == "help")) {
			temp = "\n" + c.commandName + "  " + c.description;
			if (c.aliases.size() > 0) {
				temp += "\n\naliases:\n";
				for (auto b : c.aliases) {
					temp += "\t" + b + "\n";
				}
			}
			cout << temp;
		}

	}
	return;
}