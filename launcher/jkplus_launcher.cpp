/*
===================== Jedi Knight Plus Launcher =====================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2018
=====================================================================
[Description]: Main application functionality to launch the server.
=====================================================================
*/

//System headers
#include <windows.h>
#include <direct.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//Main header
#include "jkplus_launcher.h"

//Welcome message
#define MOD_WELCOME R"!(
__________________________________________________________________________
| _______________________________________________________________________ |
||                      [Jedi Knight Plus Launcher]                      ||
||        _  _  __       _                             _                 ||
||       | || |/ / _    | |                           | |                ||
||       | || ' /_| |_  | |      __ _ _   _ _ __   ___| |__   ___  _ __  ||
||   _   | ||  <|_   _| | |     / _` | | | | '_ \ / __| '_ \ / _ \| '__| ||
||  | |__| || . \ |_|   | |____| (_| | |_| | | | | (__| | | |  __/| |    ||
||   \____/ |_|\_\      |______|\__,_|\__,_|_| |_|\___|_| |_|\___||_|    ||
||                                                                       ||
|| - Simple tool to launch Jedi Knight Plus Mod dedicated server         ||
|| - Developed by Tr!Force                                               ||
|| - https://jk2mp.com/mods/jkplus                                       ||
||_______________________________________________________________________||
|_________________________________________________________________________|
)!"

//Default settings
#define MOD_DEFAULT R"!(
Server Binary:binary=jk2mvded.exe
Server Port:net_port=28070
Server Dedicated:dedicated=2
Server Version:mv_serverversion=1.04
Server Mod:fs_game=JediKnightPlus_1.0_Beta
Server Config:exec=jkplus_server.cfg
Enable Downloads:sv_allowdownload=1
Enable HTTP Downloads:mv_httpdownloads=1
)!"

//Using declarations
using namespace std;

//Parse settings structure
struct getSetting{
	std::string title;
	std::string name;
	std::string value;

	getSetting(std::string const& titleGet, std::string const& nameGet, std::string const& valueGet):
		title(titleGet),
		name(nameGet),
		value(valueGet)
	{}
};

//Check file existence
inline bool fileExist(const std::string& name){
	ifstream infile(name);
	return infile.good();
}

//Check file empty
inline bool fileEmpty(const std::string& name){
	ifstream infile(name);
	return infile.peek() == std::ifstream::traits_type::eof();
}

//Get working directory
string getCurrentPath(void){
	char buff[FILENAME_MAX];
	_getcwd(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}

//Create default file
void generateDefault(void){
	ofstream defaultFile;
	string defaultText = 1 + MOD_DEFAULT;
	defaultFile.open("jkplus_launcher.ini");
	defaultFile << defaultText.substr(0, defaultText.size() - 1);
	defaultFile.close();
	return;
}

//Main function
int main()
{
	while (true){
		//Set console title
		SetConsoleTitle(TEXT(VER_PRODUCTNAME_STR));

		//Set mod path
		string modPath = getCurrentPath();

		//Check settings file
		if (!fileExist("jkplus_launcher.ini")){
			system("cls");
			printf("\n-------------------------------- [ Error ] --------------------------------\n\n");
			printf("Can't find 'jkplus_launcher.ini'. Starting to create a new one...\n");
			printf("\n---------------------------------------------------------------------------\n\n");
			system("pause");
			generateDefault();
			//Restart
			system("cls");
			continue;
		}
		
		//Check empty file
		if (fileEmpty("jkplus_launcher.ini")){
			system("cls");
			printf("\n-------------------------------- [ Error ] --------------------------------\n\n");
			printf("File 'jkplus_launcher.ini' is empty. Giving default values...\n");
			printf("\n---------------------------------------------------------------------------\n\n");
			system("pause");
			generateDefault();
			//Restart
			system("cls");
			continue;
		}

		//Show welcome message
		printf("%s\n", 1 + MOD_WELCOME);

		//Get and parse settings
		ifstream getSettingFile("jkplus_launcher.ini");
		vector<getSetting> getCvars;
		string getCvarTitle;
		string getCvarName;
		string getCvarValue;

		getline(getSettingFile, getCvarTitle, ':');
		getline(getSettingFile, getCvarName, '=');
		getline(getSettingFile, getCvarValue, '\n');

		while (getSettingFile){
			getCvars.push_back(getSetting(getCvarTitle, getCvarName, getCvarValue));
			getline(getSettingFile, getCvarTitle, ':');
			getline(getSettingFile, getCvarName, '=');
			getline(getSettingFile, getCvarValue, '\n');
		}

		//Set launch options
		string launchBin = "jk2mp.exe";
		string launchCvars;

		for (getSetting cvar : getCvars){
			//Set
			if (cvar.name == "binary"){
				launchBin = cvar.value;
			}
			else{
				launchCvars = launchCvars + (cvar.name == "exec" ? "+exec " : "+set " + cvar.name) + " " + cvar.value + " ";
			}
			//Show
			printf("- %-32s: %s\n", cvar.title.c_str(), cvar.value.c_str());
		}

		//User input
		string userOption;
		printf("\nType [1] to start the server [2] to modify settings [3] to open mod folder: ");
		getline(cin, userOption);

		//User options
		if (userOption == "1"){
			//Go to parent folder
			SetCurrentDirectory(TEXT(".."));
			//Check server binary
			if (!fileExist(launchBin)){
				system("cls");
				printf("\n-------------------------------- [ Error ] --------------------------------\n\n");
				printf("Can't find '%s'. Place it in 'GameData' folder and try again.\n", launchBin.c_str());
				printf("\n---------------------------------------------------------------------------\n\n");
				//Back to mod path
				TCHAR *modPathBack = new TCHAR[modPath.size() + 1];
				modPathBack[modPath.size()] = 0;
				copy(modPath.begin(), modPath.end(), modPathBack);
				SetCurrentDirectory(modPathBack);
				system("pause");
				//Restart
				system("cls");
				continue;
			}
			else{
				//Launch server binary
				system(("start " + launchBin + " " + launchCvars).c_str());
				return EXIT_SUCCESS;
			}
		}
		else if (userOption == "2"){
			system("cls");
			printf("\n--------------------------- [ Modify Settings ] ---------------------------\n\n");
			printf("Opening settings file... Edit it and launch the server again.\n");
			printf("\n---------------------------------------------------------------------------\n\n");
			system("jkplus_launcher.ini");
			//Restart
			system("cls");
			continue;
		}
		else if (userOption == "3"){
			system("cls");
			printf("\n--------------------------- [ Open Mod Folder ] ---------------------------\n\n");
			printf("Opening mod folder... Now you can back to main menu.\n");
			printf("\n---------------------------------------------------------------------------\n\n");
			//Open mod directory
			system(("explorer " + modPath).c_str());
			system("pause");
			//Restart
			system("cls");
			continue;
		}
		else{
			//Restart
			system("cls");
			continue;
		}
	}
}
