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


 class CommandHandler
{

public:
	 CommandHandler();
	 ~CommandHandler();
	 inline static void initCmdHandler(string & output);
	 inline static void* initCmdHandler(void* output);
	 inline static int initCmdHandlerOnNewThread(string & output );
	 std::vector<commandStruct> commands;
	 static void addCommand(string cmd, vector<string> aliases, string description, void (*callback)(vector<string>));
private:
	 std::string inputText = "";
	 std::string currentCommand = "";
	 static void handleCommands(string command,vector<string>args);
	 static void helpCommand(vector<string> args);
};
static CommandHandler cmdH;
CommandHandler::CommandHandler()
{
}

CommandHandler::~CommandHandler()
{
}

int CommandHandler::initCmdHandlerOnNewThread(string& output) {

	pthread_t thread;
	int ret;
	ret = pthread_create(&thread, NULL, initCmdHandler, (void*)&output);
	if (ret != 0) {
		printf("pthread_create() failed\n");
		exit(0);
	}
	return ret;
}
void* CommandHandler::initCmdHandler(void* output) {

	CommandHandler::addCommand("help", { "" }, "", helpCommand);
	string outputString = *(static_cast<string*>(output));

	cout << outputString;
	while (true)
	{
		std::this_thread::sleep_for(80ms);
		cmdH.currentCommand = "";

		getline(cin, cmdH.inputText);
		for (auto c : cmdH.inputText) {
			if (c != ' ')
				cmdH.currentCommand += c;
			else
				if (cmdH.currentCommand.size() > 0)
					break;
		}

		string temp;
		int toRemove = 0;

		for (auto c : cmdH.currentCommand) {
			if (c == ' ')
				toRemove++;
			else
				break;
		}
		if (toRemove > 0) {
			cmdH.currentCommand.erase(cmdH.inputText.begin(), cmdH.inputText.begin() + (toRemove - 1));
		}
		temp = cmdH.currentCommand + " ";
		cmdH.inputText.erase(cmdH.inputText.begin(), cmdH.inputText.begin() + (temp.size() - 1));
		temp = "";
		auto args = cmdH.inputText;
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
		handleCommands(cmdH.currentCommand, argsArray);

		cout << outputString;
	}
}

void CommandHandler::initCmdHandler(string & output) {

	CommandHandler::addCommand("help", { "" }, "", helpCommand);
	cout << output;
	while (true)
	{
		cmdH.currentCommand = "";

		getline(cin, cmdH.inputText);
		for (auto c : cmdH.inputText) {
			if (c != ' ')	
				cmdH.currentCommand += c;
			else
				if(cmdH.currentCommand.size() > 0)
					break;
		}

		string temp;
		temp = cmdH.currentCommand + " ";
		cmdH.inputText.erase(cmdH.inputText.begin(), cmdH.inputText.begin() + (temp.size()-1));
		temp = "";
		int toRemove = 0;
		for (auto c : cmdH.currentCommand) {
			if (c == ' ')
				toRemove++;
			else
				break;
		}
		if (toRemove > 0) {
			cmdH.currentCommand.erase(cmdH.inputText.begin(), cmdH.inputText.begin() + (toRemove -1));
		}
		auto args = cmdH.inputText;
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
		handleCommands(cmdH.currentCommand,argsArray);

		cout << output;
	}

}

void CommandHandler::handleCommands(string command,vector<string>args) {
	for (auto c : cmdH.commands) {
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

void CommandHandler::addCommand(string cmd,vector<string> aliases, string description, void (*callback)(vector<string>)) {
	if (!(cmdH.commands.empty())) {
		for (auto c : cmdH.commands) {
			string temp = "";
			if (c.commandName == "help") {
				cout << "Cannnot overrite help command" << endl;
				assert(!(c.commandName == "help"));
			}
			else if (c.commandName == cmd) {
				cout << "replacing command" << endl;

				commandStruct command;
				command.commandName = cmd;
				command.aliases = aliases;
				command.description = description;
				command.callback = callback;
				for (int i = 0; i < cmdH.commands.size(); i++) {

					if (cmdH.commands[i].commandName == c.commandName) {
						cmdH.commands[i].description = description;
						cmdH.commands[i].aliases = aliases;
						cmdH.commands[i].callback = callback;
					}
				}
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
				cmdH.commands.push_back(command);
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
		cmdH.commands.push_back(command);
		return;
	}
	
}

void CommandHandler::helpCommand(vector<string> args) {
	for (auto c : cmdH.commands) {
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