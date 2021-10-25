# SingleHeaderLibs

## ConsoleInputHandler.hpp
### How to use
```c++
#include "ConsoleInputHandler.hpp"

void testCommand(vector<string> args) {
	printf("called test command\n");
}


int main(){
//			   std::string    std::vector<std::string>		std::string		 void*()
CommandHandler::addCommand("test",       {"testAliases","anotherOne"}      ,   "desciption for help"},   testCommand)

string output = ">>";
//you can modify the output string while running but currently only works with initCmdHandlerOnNewThread

//                         		std::string&
CommandHandler::initCmdHandlerOnNewThread(output);

//or 

//		             std::string&
CommandHandler::initCmdHandler(output)
}
```

#### you are also able to overrite commands but not the 'help' command
#### asserts if you try override used aliases or help command
#### requires c++17
