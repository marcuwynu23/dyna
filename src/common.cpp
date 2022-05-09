#include <iostream>
#include <thread>
#include <mutex>
#include <direct.h>
#include <unistd.h> 
#include "common.h"

void termExecute(std::string command)
{
	std::mutex m;
	std::thread t{[&]
		{
			std::lock_guard<std::mutex> lock{m};
			system(command.c_str());
		}
	};
	t.join();
}



void termMessage(std::string msg)
{
	std::mutex m;
	std::thread t{[&]
		{
			std::lock_guard<std::mutex> lock{m};
			std::cout << msg << std::endl;
		}
	};
	t.join();
}



void changeDir(std::string directoryName)
{
	std::mutex m;
	std::thread t{[&]
		{
			std::lock_guard<std::mutex> lock{m};
			chdir(directoryName.c_str());
		}
	};
	t.join();
}

std::string createFile(std::string path,std::string fileName)
{
	return "cf "+ path + "\\"+ fileName;
}




std::string createFolder(std::string folderPath)
{
	return "md " + folderPath + "\\";
}


void help(){
	std::cout << "dyna beta version - nodejs project builder" << std::endl;
	std::cout << "-help								show this message." << std::endl;
	std::cout << "-deps								install app dependencies." << std::endl;
	std::cout << "-init	<project-name> <debug-mode|optional>			initialize app." << std::endl;
	std::cout << "-test								test app." << std::endl;
	std::cout << "-debug								debug app." << std::endl;
	std::cout << "-run								run app." << std::endl;
}

void createProject(std::string projectName,std::string debugMode)
{
	std::string folderList[7] = {"views","controllers","models","databases","constants","lib","test"};
	std::string fileList[4] = {"app.js",".env","README.md",".gitignore"};
	if(debugMode == "debug"){
	
		for(int i=0;i<7;i++){
			termMessage(createFolder(projectName)+folderList[i]);
			termExecute(createFolder(projectName)+folderList[i]);
		}
		for(int i=0;i<4;i++){
			termMessage(createFile(projectName,fileList[i]));
			termExecute(createFile(projectName,fileList[i]));
		}
		
		termMessage("git init "+projectName);
		termExecute("git init "+projectName);
		changeDir(projectName);

	}
	else
	{
		for(int i=0;i<7;i++)
		{
			termExecute(createFolder(projectName)+folderList[i]);
		}
		for(int i=0;i<4;i++)
		{	
			termExecute(createFile(projectName,fileList[i]));
		}
		termExecute("git init "+projectName);
		changeDir(projectName);
	}

}

void commandline(int argSize,char** argList)
{	
	if(argSize == 1)
	{
		std::cout << "no subcommand supplied." << std::endl;
		help();
		exit(0);
	}
	std::string subcommand = argList[1];

	if(subcommand == "-init"){
		if(argList[2] == NULL){
			std::cout << "no <project-name> supplied." << std::endl;
			help();
			exit(0);
		}
		std::string argument = argList[2];
		termMessage("initialize project...");
		if(argList[3] == NULL)
		{
			createProject(argument,"");
		}
		else
		{
			std::string debugValue = argList[3];
			createProject(argument,debugValue);
		}
		termMessage("project "+ argument + " created. ");
	}
	else if(subcommand == "-deps")
	{
		termMessage(" install project dependencies...");
		termExecute("npm i");
	}
	else if(subcommand == "-test")
	{
		termMessage("test project...");
		termExecute("jest");
	}
	else if(subcommand == "-debug"){
		termMessage("run project debug...");
		termExecute("npm run dev");
	}
	else if(subcommand == "-run"){
		termMessage("run project...");
		termExecute("npm start");
	}
	else{
			help();
			exit(0);
	}
}


