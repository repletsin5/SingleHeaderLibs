# SingleHeaderLibs

## ConsoleInputHandler.hpp
### How to use
```c++
#include "ConsoleInputHandler.hpp"

void testCommand(vector<string> args) {
	printf("called test command\n");
}


int main(){
//args = std::string,std::vector<std::string>,std::string,void*()
CIH::addCommand("test",{"testAliases","anotherOne"},"test description"},testCommand)

string output = ">>";
//you can modify the output string while running but currently only works with initciHandlerOnNewThread

//args = std::string&
CIH::initciHandlerOnNewThread(output);

//or 

//args = std::string&
CIH::initciHandler(output)
}
```

#### you are also able to overrite commands but not the 'help' command
#### asserts if you try override used aliases or help command
#### requires c++17
