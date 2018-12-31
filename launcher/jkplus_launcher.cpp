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
|| - https://jkplus.github.io                                            ||
||_______________________________________________________________________||
|_________________________________________________________________________|
)!"

//Default settings
#define MOD_DEFAULT R"!(
// Server Binary
jk2mvded.exe
// Server Port
set net_port 28070
// Server Dedicated
set dedicated 2
// Server Version
set mv_serverversion 1.04
// Server Mod
set fs_game JediKnightPlus
// Server Config
exec jkplus_server.cfg
)!"

//Using declarations
using namespace std;

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
	defaultFile.open("jkplus_launcher.cfg");
	defaultFile << defaultText.substr(0, defaultText.size() - 1);
	defaultFile.close();
	return;
}

//Main function
int main(){
	while (true){
		//Set console title
		SetConsoleTitle(TEXT(VER_PRODUCTNAME_STR));

		//Set console size
		const int consoleWidth = 645;
		const int consoleHeight = 570;
		SetWindowPos(GetConsoleWindow(), NULL, GetSystemMetrics(SM_CXSCREEN) / 2 - consoleWidth / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - consoleHeight / 2, consoleWidth, consoleHeight, SWP_SHOWWINDOW);

		//Set mod path
		string modPath = getCurrentPath();

		//Delete auto generated config
		if (fileExist("jk2mpserver.cfg")){
			system("del jk2mpserver.cfg");
			continue;
		}
		if (fileExist("jk2mvserver.cfg")){
			system("del jk2mvserver.cfg");
			continue;
		}

		//Check settings file
		if (!fileExist("jkplus_launcher.cfg")){
			system("cls");
			printf("\n-------------------------------- [ Error ] --------------------------------\n\n");
			printf("Can't find 'jkplus_launcher.cfg'. Starting to create a new one...\n");
			printf("\n---------------------------------------------------------------------------\n\n");
			system("pause");
			generateDefault();
			//Restart
			system("cls");
			continue;
		}
		
		//Check empty file
		if (fileEmpty("jkplus_launcher.cfg")){
			system("cls");
			printf("\n-------------------------------- [ Error ] --------------------------------\n\n");
			printf("File 'jkplus_launcher.cfg' is empty. Giving default values...\n");
			printf("\n---------------------------------------------------------------------------\n\n");
			system("pause");
			generateDefault();
			//Restart
			system("cls");
			continue;
		}

		//Show welcome message
		printf("%s", 1 + MOD_WELCOME);
		printf("\n-------------------------- [ Current Settings ] ---------------------------\n\n");

		//Get and parse settings
		ifstream getSettingFile("jkplus_launcher.cfg");
		string getSettingLine;
		string getSettingTitle;

		string launchBin;
		string launchCvars;
		string launchServerCFG;

		while (getline(getSettingFile, getSettingLine))
		{
			char *getFirst;
			char *getLast = strtok_s(_strdup(getSettingLine.c_str()), " ", &getFirst);
			vector<string> getCvars;
			//Parse
			while (getLast)
			{
				getCvars.push_back(getLast);
				getLast = strtok_s(NULL, " ", &getFirst);
			}
			if (strstr(getSettingLine.c_str(), "//")){
				//Set Title
				getSettingTitle = getSettingLine.c_str();
			}
			else{
				//Check title
				if (getSettingTitle == ""){
					getSettingTitle = "// " + getCvars[1];
				}
				//Set Cvars
				if (strstr(getSettingLine.c_str(), ".exe")){
					launchBin = getCvars[0].c_str();
					printf("%-35s: %s\n", getSettingTitle.c_str(), getCvars[0].c_str());
				}
				else if (strstr(getSettingLine.c_str(), ".cfg")){
					launchServerCFG = getCvars[1].c_str();
					launchCvars = launchCvars + "+" + getCvars[0].c_str() + " " + getCvars[1].c_str() + " ";
					printf("%-35s: %s\n", getSettingTitle.c_str(), getCvars[1].c_str());
				}
				else{
					launchCvars = launchCvars + "+" + getCvars[0].c_str() + " " + getCvars[1].c_str() + " " + getCvars[2].c_str() + " ";
					if (getCvars[1] == "dedicated"){
						string type = getCvars[2] == "2" ? "(Internet)" : "(LAN)";
						printf("%-35s: %s\n", getSettingTitle.c_str(), (getCvars[2] + " " + type).c_str());
					}
					else{
						printf("%-35s: %s\n", getSettingTitle.c_str(), getCvars[2].c_str());
					}
				}
				getSettingTitle = "";
			}
		}

		//User input
		string userOption;
		string userOptionText[][2] = { 
										{ "[1] Start the server",			"[4] Open mod folder" },
										{ "[2] Modify launch settings",		"[5] Open config generator" },
										{ "[3] Open server config file",	"[6] Visit JKPlus Website" },
									 };

		int userOptionRows = (sizeof(userOptionText) / sizeof(*userOptionText));
		int userOptionCols = (sizeof(userOptionText) / sizeof(userOptionText[0][0]));

		printf("\n-------------------------- [ Choose an option ] ---------------------------\n\n");

		for (int i = 0; i < userOptionRows; i++){
			printf("%-34s %s\n", userOptionText[i][0].c_str(), userOptionText[i][1].c_str());
		}

		printf("\nType the number of your option: ");
		getline(cin, userOption);

		int userOptionNum = stoi(userOption);

		//User options
		switch (userOptionNum){
			case 1:
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
				break;
			case 2:
				system("cls");
				printf("\n--------------------------- [ Modify Settings ] ---------------------------\n\n");
				printf("Opening settings file... Edit it and launch the server again.\n");
				printf("\n---------------------------------------------------------------------------\n\n");
				system("jkplus_launcher.cfg");
				system("pause");
				//Restart
				system("cls");
				continue;
				break;
			case 3:
				system("cls");
				printf("\n----------------------- [ Open Server Config File ] -----------------------\n\n");
				printf("Opening server configuration file.. Now you can back to main menu.\n");
				printf("\n---------------------------------------------------------------------------\n\n");
				//Open mod directory
				system((launchServerCFG).c_str());
				system("pause");
				//Restart
				system("cls");
				continue;
				break;
			case 4:
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
				break;
			case 5:
				//Open mod directory
				SetCurrentDirectory(TEXT("tools"));
				system("jkplus_server.html");
				if (!fileExist(launchBin)){
					system("cls");
					printf("\n------------------------ [ Open Server Generator ] ------------------------\n\n");
					printf("Opening server generator... Now you can back to main menu.\n");
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
				break;
			case 6:
				system("cls");
				printf("\n------------------------- [ Open JKPlus Website ] -------------------------\n\n");
				printf("Opening JKPlus website... Now you can back to main menu.\n");
				printf("\n---------------------------------------------------------------------------\n\n");
				//Open mod directory
				system("start https://jkplus.github.io");
				system("pause");
				//Restart
				system("cls");
				continue;
				break;
			default:
				//Restart
				system("cls");
				continue;
				break;
		}
	}
}
