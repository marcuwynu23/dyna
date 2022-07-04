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


std::string createFile(std::string path,std::string fileName)
{
	return "cf "+ path + "\\"+ fileName;
}




std::string createFolder(std::string folderPath)
{
	return "md " + folderPath + "\\";
}

std::string copyFileContent(std::string fileName){
	return "";
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
	int FOLDER_LIST_LENGTH = 7;
	int FILE_LIST_LENGTH = 5;
	std::string folderList[FOLDER_LIST_LENGTH] = {"views","controllers","models","databases","constants","lib","test"};
	std::string fileList[FILE_LIST_LENGTH] = {"app.js",".env","README.md",".gitignore",".autofile"};
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
	}
	else
	{
		for(int i=0;i<FOLDER_LIST_LENGTH;i++)
		{
			termExecute(createFolder(projectName)+folderList[i]);
		}
		for(int i=0;i<FILE_LIST_LENGTH;i++)
		{	
			termExecute(createFile(projectName,fileList[i]));
		}
		termExecute("git init "+projectName);
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

	if(subcommand == "-create"|| subcommand == "-c"){
		if(argList[2] == NULL){
			std::cout << "no <project-name> supplied." << std::endl;
			help();
			exit(0);
		}
		std::string argument = argList[2];
		termMessage("create project structure and other files...");
		if(argList[3] == NULL)
		{
			createProject(argument,"");
		}
		else
		{
			std::string debugValue = argList[3];
			createProject(argument,debugValue);
		}
		termMessage("project "+ argument + "structure created. ");
	}
	else if(subcommand == "-init"||subcommand == "-i")
	{
		termMessage(" initialize project...");
		termExecute("npm init -y");
	}
	else if(subcommand == "-deps"|| subcommand == "-d")
	{
		termMessage(" install project dependencies...");
		termExecute("npm i -g nodemon");
		termExecute("npm i express express-session nunjucks morgan mongoose");
	}
	else if(subcommand == "-test"||subcommand == "-t")
	{
		termMessage("test project...");
		termExecute("jest");
	}
	else if(subcommand == "-debug"||subcommand == "-u"){
		termMessage("run project debug...");
		termExecute("npm run dev");
	}
	else if(subcommand == "-run"||subcommand == "-r"){
		termMessage("run project...");
		termExecute("npm start");
	}
	else{
			help();
			exit(0);
	}
}


