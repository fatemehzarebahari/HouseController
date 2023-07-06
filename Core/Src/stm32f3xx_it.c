/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LiquidCrystal.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define MAXROWS 8
#define MAXCOLS 20
#define MAXCONTENT 100
// home stuff:
int lum = 0;
int temperature = 9;
int someOneClose = 0;

int lumThreshold = 10;
int temThreshold = 35;
int Lights[4] = {0,0,0,0};
int temAlarm = 1;
int securityAlarm = 1;
int securityAlert = 1;
int addedScenarios[4][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
int lastScenarioIndex = 0;
char startMassage[20]="";


int newScenarioLights[4] = {0,0,0,0};
int newSceneTemThreshold = 10;
int newSceneTemAlarm = 1;
int newScenesecurityAlarm = 1;
int newSceneSecurityAlert = 1;

int minFrequency = 200;
int maxFrequency = 4000;
int vol = 100;
int systemPos = 0;
int lcdPos = 0;
uint64_t lastPress =0;
int col = -1;
int row = -1;
char currentMenu[12][20]={">> ControlPanel","","","","","","","","","","",""};
int CurrentMenuOption = 0;
int MenuoptionCounts = 0;
int page = 0;
int generateMenu = 0;
int inMenu = 1;
int typingCount = 1;
int lastKeyPressed = 0;
int sameKeyPressed = 0;
int lastId;
int lastPagesId;
int startState = 0; //0:shut down,1: start menu massage,2: started and generated menu,3:shutdown massage but not gone yet
int startStateLastTime = 0;
int mSecond = 0;
int shown = 0;


struct menuScreen{
	int id;
	int optionCount;
	int isMenu;
	char menuItems[MAXROWS][MAXCOLS];
	char content[MAXCONTENT];
};
struct TreeNode {
    struct TreeNode* child1;
    struct TreeNode* child2;
    struct TreeNode* child3;
    struct TreeNode* child4;
    struct TreeNode* child5;
    struct TreeNode* child6;
    struct TreeNode* child7;
    struct TreeNode* child8;
    struct TreeNode* parent;
    struct menuScreen screen;
};

struct TreeNode* createNode(struct menuScreen screen, struct TreeNode* parent) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->child1 = NULL;
    newNode->child2 = NULL;
    newNode->child3 = NULL;
    newNode->child4 = NULL;
    newNode->child5 = NULL;
    newNode->child6 = NULL;
    newNode->child7 = NULL;
    newNode->child8 = NULL;
    newNode->parent = parent;
    newNode->screen = screen;
    return newNode;
}
struct TreeNode* root;
struct TreeNode* node1;
struct TreeNode* node2;
struct TreeNode* node3;
struct TreeNode* node4;
struct TreeNode* node5;
struct TreeNode* node6;
struct TreeNode* node7;
struct TreeNode* node8;
struct TreeNode* node9;
struct TreeNode* node10;
struct TreeNode* node11;
struct TreeNode* node12;
struct TreeNode* node13;
struct TreeNode* node14;
struct TreeNode* node15;
struct TreeNode* node16;
struct TreeNode* node17;
struct TreeNode* node18;
struct TreeNode* node19;
struct TreeNode* node_1;

struct TreeNode* currentNode;

void GenerateMenu(){
	struct menuScreen mainMenu = {0, 1, 1, {">> ControlPanel   "}, ""};
    struct menuScreen controlPanel = {1, 7, 1, {">> Temperature   ", "> Security       ", "> Lights       ", "> Scenarios    ", "> StartSetting   ","> AddScenario   ","> ShutDown   "}, ""};
    struct menuScreen temperature = {2, 2, 1, {">> Threshold    ", "> AlarmSound   "}, ""};
    struct menuScreen security = {3, 2, 1, {">> Activation   ", "> AlarmSound   "}, ""};
    struct menuScreen lightsControl = {4, 0, 0, {""}, "X  X  X  X"};
    struct menuScreen scenarios = {5, 2, 1, {">> Scenario 1   ", "> Scenario 2   "}, ""};
    struct menuScreen nameSet = {6, 1, 0, {"write your phrase"}," " };
    struct menuScreen temThresh = {7, 0, 0, {""}, "<  35  >"};
    struct menuScreen temAlarm = {8, 0, 0, {""}, "<  on  >"};
    struct menuScreen securityActivation = {9, 0, 0, {""}, "<  on  >"};
    struct menuScreen securityAlarm = {10, 0, 0, {""}, "<  on  >"};
    struct menuScreen scenario1 = {11, 8, 0, {"temTHreshold=22","temAlarm=on","securityAlarm=off","securityAlert=on","light1=on","light2=on","light3=off","light4=off"}, "<  off  >"};
    struct menuScreen scenario2 = {12, 8, 0, {"temTHreshold=35","temAlarm=on","securityAlarm=on","securityAlert=on","light1=off","light2=off","light3=off","light4=off"}, "<  off  >"};

    struct menuScreen AddScenario= {13, 6, 1, {">> temTHreshold    ","> temAlarm    ","> securityAlarm   ","> securityAlert   ","> lights        ","> create         "}, ""};
    struct menuScreen scenTemThreshold = {14,0,0, {""}, "<  10  >"};
    struct menuScreen scenTemAlarm = {15, 0, 0, {""}, "<  on  >"};
	struct menuScreen scenSecAlarm = {16, 0, 0, {""}, "<  on  >"};
	struct menuScreen scenSecAlert = {17, 0, 0, {""}, "<  on  >"};
	struct menuScreen scenLight = {18, 0, 0, {""}, "X  X  X  X"};
	struct menuScreen create = {19, 0, 0, {""}, "new scenario created"};

	struct menuScreen shutDown = {-1, 0, 0, {""}, "Turning Off... "};


    lastId = 19;
    lastPagesId = 19;

    root = createNode(mainMenu, NULL);
    node1 = createNode(controlPanel, root);
    node2 = createNode(temperature, node1);
    node3 = createNode(security, node1);
    node4 = createNode(lightsControl, node1);
    node5 = createNode(scenarios, node1);
    node6 = createNode(nameSet, node1);
    node7 = createNode(temThresh, node2);
    node8 = createNode(temAlarm, node2);
    node9 = createNode(securityActivation, node3);
    node10 = createNode(securityAlarm, node3);
    node11 = createNode(scenario1, node5);
    node12 = createNode(scenario2, node5);

    node13 = createNode(AddScenario, node1);
    node14 = createNode(scenTemThreshold, node13);
    node15 = createNode(scenTemAlarm, node13);
    node16 = createNode(scenSecAlarm, node13);
    node17 = createNode(scenSecAlert, node13);
    node18 = createNode(scenLight, node13);
    node19 = createNode(create, node13);

    node_1 = createNode(shutDown, node1);


    root->child1 = node1;
    node1->child1 = node2;
    node1->child2 = node3;
    node1->child3 = node4;
    node1->child4 = node5;
    node1->child5 = node6;
    node1->child6 = node13;
    node1->child7 = node_1;


    node2->child1 = node7;
    node2->child2 = node8;

    node3->child1 = node9;
    node3->child2 = node10;

    node5->child1 = node11;
    node5->child2 = node12;

    node13->child1 = node14;
    node13->child2 = node15;
    node13->child3 = node16;
    node13->child4 = node17;
    node13->child5 = node18;
    node13->child6 = node19;

    currentNode = root;

}
void createScenario() {
    lastId++;

    char str1[20] ="";
	char str2[20]="";
	char str3[20]="";
	char str4[20]="";
    char str5[4][20]={"","","",""};
    sprintf(str1, "temThreshold=%d", newSceneTemThreshold);

    if (newSceneTemAlarm == 1)
        strcpy(str2, "temAlarm=on");
    else
        strcpy(str2, "temAlarm=off");

    if (newScenesecurityAlarm == 1)
        strcpy(str3, "securityAlarm=on");
    else
        strcpy(str3, "securityAlarm=off");

    if (newSceneSecurityAlert == 1)
        strcpy(str4, "securityAlert=on");
    else
        strcpy(str4, "securityAlert=off");

    for (int i = 0; i < 4; i++) {
        if (newScenarioLights[i] == 1)
            sprintf(str5[i], "Light%d=on", i);
        else
            sprintf(str5[i], "Light%d=off", i);
    }

    addedScenarios[lastScenarioIndex][0] = newSceneTemThreshold;
    addedScenarios[lastScenarioIndex][1] = newSceneTemAlarm;
    addedScenarios[lastScenarioIndex][2] = newScenesecurityAlarm;
    addedScenarios[lastScenarioIndex][3] = newSceneSecurityAlert;
    addedScenarios[lastScenarioIndex][4] = newScenarioLights[0];
    addedScenarios[lastScenarioIndex][5] = newScenarioLights[1];
    addedScenarios[lastScenarioIndex][6] = newScenarioLights[2];
    addedScenarios[lastScenarioIndex][7] = newScenarioLights[3];

    char strArray[8][20]; // Array to hold str1 to str4

    strcpy(strArray[0], str1);
    strcpy(strArray[1], str2);
    strcpy(strArray[2], str3);
    strcpy(strArray[3], str4);
    strcpy(strArray[4], str5[0]);
    strcpy(strArray[5], str5[1]);
    strcpy(strArray[6], str5[2]);
    strcpy(strArray[7], str5[3]);

    struct menuScreen newScenario = {lastId, 8, 0};

    for (int i = 0; i < 8; i++) {
        strcpy(newScenario.menuItems[i], strArray[i]);
    }

    strcpy(newScenario.content, "<  off  >");

//    struct menuScreen newScenario = {lastId, 8, 0, strArray, "<  off  >"};

    struct TreeNode* newNode = createNode(newScenario, node5);

    switch (lastScenarioIndex) {
        case 0:
            node5->child3 = newNode;
            break;
        case 1:
            node5->child4 = newNode;
            break;
        case 2:
            node5->child5 = newNode;
            break;
        case 3:
            node5->child6 = newNode;
            break;
    }
    char newStr[20];
    sprintf(newStr,"> scenario %d",lastScenarioIndex+3);
    strcpy(node5->screen.menuItems[node5->screen.optionCount],newStr);
    node5->screen.optionCount++;
    lastScenarioIndex++;
}

void ShowMenu(){
	int p = page * 4;
//    clear();
	char temp[20] = "                   ";
    setCursor(0,0);
    print(currentMenu[p]);
    setCursor(0,1);
    if(p+1 >= MenuoptionCounts)
        print(temp);
    else
    	print(currentMenu[p+1]);
    setCursor(0,2);
    if(p+2 >= MenuoptionCounts)
        print(temp);
    else
    	print(currentMenu[p+2]);
    setCursor(0,3);
    if(p+3 >= MenuoptionCounts)
        print(temp);
    else
    	print(currentMenu[p+3]);
    display();
}
void ShowInfo(struct menuScreen info){
	int p = page * 3;
    clear();
	char temp[20] = "                   ";
    setCursor(0,0);
    print(info.content);
    setCursor(0,1);
    print(info.menuItems[p]);
    setCursor(0,2);
    if(p+1 >= MenuoptionCounts)
        print(temp);
	else
		print(info.menuItems[p+1]);
    setCursor(0,3);
    if(p+2 >= MenuoptionCounts)
        print(temp);
	else
		print(info.menuItems[p+2]);
    display();
}
void changeOption(int down){
	char newStr[20] = ">";
	strcat(newStr,currentMenu[CurrentMenuOption]);
	strcpy(currentMenu[CurrentMenuOption],newStr);
	if(down == 1){
		if(CurrentMenuOption > 0){
			int length  = strlen(currentMenu[CurrentMenuOption - 1]) - 1;
			for (int i = 0; i< length ;i++)
			{
				currentMenu[CurrentMenuOption - 1][i] = currentMenu[CurrentMenuOption - 1][i+1];
			}
			currentMenu[CurrentMenuOption - 1][length] = '\0';
		}
	}
	else{
		if(CurrentMenuOption < MenuoptionCounts){
			int length  = strlen(currentMenu[CurrentMenuOption + 1]) - 1;
			for (int i = 0; i< length ;i++)
			{
				currentMenu[CurrentMenuOption + 1][i] = currentMenu[CurrentMenuOption + 1][i+1];
			}
			currentMenu[CurrentMenuOption + 1][length] = '\0';
		}
	}
	ShowMenu();
}
void SetMenu(char MenuItems[][20]){
	for(int i = 0; i < 12 ; i++){
	    strcpy(currentMenu[i],"");
	}
	for(int i = 0; i < MenuoptionCounts; i++){
	    strcpy(currentMenu[i],MenuItems[i]);
	}
}
void messageSetter(struct TreeNode* node, char* newChar){
	if(strcmp(newChar,"null") != 0 && strcmp(newChar,"dell") != 0){
		int len = strlen(node->screen.content);
		if(HAL_GetTick()- lastPress < 800  && sameKeyPressed){
			node->screen.content[len - 1] = newChar[0];
		}
		else{
			node->screen.content[len] = newChar[0];
			node->screen.content[len+1] = '\0';
		}
	}
	else if(strcmp(newChar,"dell") == 0){
		int len = strlen(node->screen.content);
		if(len >0){
			node->screen.content[len - 1] = '\0';
		}
	}
	strcpy(startMassage,node->screen.content);
}
void setOn(struct menuScreen* screen){
	if(strcmp(screen->content,"<  on  >") != 0 ){
		strcpy(screen->content,"<  on  >");
		if(screen->id > lastPagesId){
			int index = screen->id - lastPagesId - 1;
			setScenario(addedScenarios[index][0], addedScenarios[index][1], addedScenarios[index][2], addedScenarios[index][3], addedScenarios[index][4], addedScenarios[index][5], addedScenarios[index][6], addedScenarios[index][7]);
		}
		switch(screen->id){
			case 8:
				temAlarm = 1;
				break;
			case 9:
				securityAlert = 1;
				break;
			case 10:
				securityAlarm = 1;
				break;
			case 11: // scenario 1: entering the house
				setScenario(22, 1, 0, 1, 1, 1, 0, 0);
				break;
			case 12:// scenario 2: leaving the house
				setScenario(35, 1, 1, 1, 0, 0, 0, 0);
				break;
			case 15:
				newSceneTemAlarm = 1;
				break;
			case 16:
				newScenesecurityAlarm = 1;
				break;
			case 17:
				newSceneSecurityAlert = 1;
				break;
		}

	}
	else{
		strcpy(screen->content,"<  off  >");
		switch(screen->id){
			case 8:
				temAlarm = 0;
				break;
			case 9:
				securityAlert = 0;
				break;
			case 10:
				securityAlarm = 0;
				break;
			case 11:
				break;
			case 12:
				break;
			case 15:
				newSceneTemAlarm = 0;
				break;
			case 16:
				newScenesecurityAlarm = 0;
				break;
			case 17:
				newSceneSecurityAlert = 0;
				break;
		}
	}
}
void setExactOn(struct menuScreen* screen,int value){
	if(value == 1){
			strcpy(screen->content,"<  on  >");
			switch(screen->id){
				case 8:
					temAlarm = 1;
					break;
				case 9:
					securityAlert = 1;
					break;
				case 10:
					securityAlarm = 1;
					break;
				case 15:
					newSceneTemAlarm = 1;
					break;
				case 16:
					newScenesecurityAlarm = 1;
					break;
				case 17:
					newSceneSecurityAlert = 1;
					break;
			}
		}
		else{
			strcpy(screen->content,"<  off  >");
			switch(screen->id){
				case 8:
					temAlarm = 0;
					break;
				case 9:
					securityAlert = 0;
					break;
				case 10:
					securityAlarm = 0;
					break;
				case 15:
					newSceneTemAlarm = 0;
					break;
				case 16:
					newScenesecurityAlarm = 0;
					break;
				case 17:
					newSceneSecurityAlert = 0;
					break;
			}
		}
}
void setThreshold(struct menuScreen* screen, int increase, int temperId){
	if(temperId == 0){
		if(increase && temThreshold > 0)
			temThreshold++;
		else
			temThreshold--;

	    sprintf(screen->content,"<  %d  >", temThreshold);

	}
	else if(temperId == 1){
		if(increase && newSceneTemThreshold > 0)
			newSceneTemThreshold++;
		else
			newSceneTemThreshold--;
	    sprintf(screen->content,"<  %d  >", newSceneTemThreshold);

	}

}
void setExcactThreshold(struct menuScreen* screen, int threshold){
	temThreshold = threshold;
    sprintf(screen->content,"<  %d  >", temThreshold);
}
void setLigthToggle(struct menuScreen* screen, int index,int* lights){
	lights[index] = 1 - lights[index];
	if(lights[index] == 1)
		screen->content[index * 3] = 'O';
	else
		screen->content[index * 3] = 'X';

}
void setExactLight(struct menuScreen* screen, int index,int value){
	Lights[index] = value;
	if(Lights[index] == 1)
			screen->content[index * 3] = 'O';
		else
			screen->content[index * 3] = 'X';
}
void setScenario(int temperatureThreshold, int temperatureAlarm,int secAlarm, int secAlert, int l1,int l2, int l3, int l4){
	// calling previous functions
	setExcactThreshold(&(node7->screen),temperatureThreshold);
	setExactLight(&(node4->screen),0,l1);
	setExactLight(&(node4->screen),1,l2);
	setExactLight(&(node4->screen),2,l3);
	setExactLight(&(node4->screen),3,l4);
	setExactOn(&(node8->screen),temperatureAlarm);
	setExactOn(&(node10->screen),secAlarm);
	setExactOn(&(node9->screen),secAlert);
}
char* charSelector(int key, int count) {
    switch (key) {
        case 1:
            switch (count) {
                case 1:
                    return "0";
                case 2:
                    return "1";
                case 3:
                    return " ";
            }
            break;
        case 2:
            switch (count) {
                case 1:
                    return "2";
                case 2:
                    return "a";
                case 3:
                    return "b";
                case 4:
                    return "c";
            }
            break;
        case 3:
            switch (count) {
                case 1:
                    return "3";
                case 2:
                    return "d";
                case 3:
                    return "e";
                case 4:
                    return "f";
            }
            break;
        case 5:
            switch (count) {
                case 1:
                    return "4";
                case 2:
                    return "g";
                case 3:
                    return "h";
                case 4:
                    return "i";
            }
            break;
        case 6:
            switch (count) {
                case 1:
                    return "5";
                case 2:
                    return "j";
                case 3:
                    return "k";
                case 4:
                    return "l";
            }
            break;
        case 7:
            switch (count) {
                case 1:
                    return "6";
                case 2:
                    return "m";
                case 3:
                    return "n";
                case 4:
                    return "o";
            }
            break;
        case 9:
            switch (count) {
                case 1:
                    return "7";
                case 2:
                    return "p";
                case 3:
                    return "q";
                case 4:
                    return "r";
                case 5:
                    return "s";
            }
            break;
        case 10:
            switch (count) {
                case 1:
                    return "8";
                case 2:
                    return "t";
                case 3:
                    return "u";
                case 4:
                    return "v";
            }
            break;
        case 11:
            switch (count) {
                case 1:
                    return "9";
                case 2:
                    return "w";
                case 3:
                    return "x";
                case 4:
                    return "y";
                case 5:
                    return "z";
            }
            break;
        case 14:
            return "dell";
    }

    return "null";
}
int stateCountsForEachKey(int key){
		switch(key){
		case 1:
			return 3;
		case 2:
			return 4;
		case 3:
			return 4;
		case 5:
			return 4;
		case 6:
			return 4;
		case 7:
			return 4;
		case 9:
			return 5;
		case 10:
			return 4;
		case 11:
			return 5;

	}
		return 0;
}
void nextCurrentNode(){
	switch(CurrentMenuOption){
		case 0:
			currentNode = currentNode->child1;
			break;
		case 1:
			currentNode = currentNode->child2;
			break;
		case 2:
			currentNode = currentNode->child3;
			break;
		case 3:
			currentNode = currentNode->child4;
			break;
		case 4:
			currentNode = currentNode->child5;
			break;
		case 5:
			currentNode = currentNode->child6;
			break;
		case 6:
			currentNode = currentNode->child7;
			break;
		case 7:
			currentNode = currentNode->child8;
			break;
	}
}
void CuationMessage(char* str){
	setCursor(0,0);
	print(str);
	setCursor(0,1);
	print("                   ");
	setCursor(0,2);
	print("                   ");
	setCursor(0,3);
	print("                   ");
	display();
}


void keyHandler(){
	int key = row * 4 + col +1;
	if(startState == 2){
		if(HAL_GetTick()- lastPress<300 ) return;
		if(!generateMenu){
			GenerateMenu();
			generateMenu = 1;
		}

		if(currentNode->screen.id == 19){
			createScenario();
		}
		else if(currentNode->screen.id == 6){
			if(key != lastKeyPressed){
				lastKeyPressed = key;
				typingCount = 1;
				sameKeyPressed = 0;
			}
			else{
				sameKeyPressed = 1;
				typingCount++;
				typingCount = typingCount % (stateCountsForEachKey(key)+1);
			}
			messageSetter(currentNode, charSelector(key, typingCount));
			ShowInfo(currentNode->screen);
		}
		switch (key) {
			case 2: // up key
				if(inMenu && CurrentMenuOption > 0){
					if(inMenu && CurrentMenuOption == 4)
						page--;
					CurrentMenuOption--;
					changeOption(0);
				}
				if(!inMenu && page>0){
					page--;
					ShowInfo(currentNode->screen);
				}
				break;

			case 4:
				if(currentNode->screen.id == 4 ){
					setLigthToggle(&(currentNode->screen),0,Lights);
					ShowInfo(currentNode->screen);
				}
				else if(currentNode->screen.id == 18){
					setLigthToggle(&(currentNode->screen),0,newScenarioLights);
					ShowInfo(currentNode->screen);
				}
				break;

			case 5: // left key
				if(currentNode->screen.id == 8 ||currentNode->screen.id == 9 || currentNode->screen.id == 10 || currentNode->screen.id == 11 || currentNode->screen.id == 12 || currentNode->screen.id == 15 || currentNode->screen.id == 16 || currentNode->screen.id == 17 ||currentNode->screen.id >lastPagesId){
					setOn(&(currentNode->screen));
					ShowInfo(currentNode->screen);
				}
				else if(currentNode->screen.id == 7 ){
					setThreshold(&(currentNode->screen), 0,0);
					ShowInfo(currentNode->screen);
				}
				else if(currentNode->screen.id == 14){
					setThreshold(&(currentNode->screen), 0,1);
					ShowInfo(currentNode->screen);
				}
				break;
			case 6: // down key
				if(inMenu && CurrentMenuOption < MenuoptionCounts - 1) {
					if(inMenu && CurrentMenuOption == 3)
						page = 1;
					CurrentMenuOption++;
					changeOption(1);
				}
				if(!inMenu && page < (currentNode->screen.optionCount-1) / 3){
					page++;
					ShowInfo(currentNode->screen);

				}

				break;
			case 7: // right key

				if(currentNode->screen.id == 8 ||currentNode->screen.id == 9 || currentNode->screen.id == 10 || currentNode->screen.id == 11 || currentNode->screen.id == 12 || currentNode->screen.id == 15 || currentNode->screen.id == 16 || currentNode->screen.id == 17 ||currentNode->screen.id >lastPagesId){
					setOn(&(currentNode->screen));
					ShowInfo(currentNode->screen);

				}
				else if(currentNode->screen.id == 7){
					setThreshold(&(currentNode->screen), 1,0);
					ShowInfo(currentNode->screen);
				}
				else if(currentNode->screen.id == 14){
					setThreshold(&(currentNode->screen), 1,1);
					ShowInfo(currentNode->screen);
				}
				break;
			case 8:
				if(currentNode->screen.id == 4){
					setLigthToggle(&(currentNode->screen),1,Lights);
					ShowInfo(currentNode->screen);
				}
				else if(currentNode->screen.id == 18){
					setLigthToggle(&(currentNode->screen),1,newScenarioLights);
					ShowInfo(currentNode->screen);
				}
				break;
			case 12:
				if(currentNode->screen.id == 4){
					setLigthToggle(&(currentNode->screen),2,Lights);
					ShowInfo(currentNode->screen);
				}
				else if(currentNode->screen.id == 18){
					setLigthToggle(&(currentNode->screen),2,newScenarioLights);
					ShowInfo(currentNode->screen);
				}
				break;
			case 13: // back key
				if(currentNode != root){
					clear();
					page = 0;
					MenuoptionCounts = currentNode->screen.optionCount;
					currentNode = currentNode->parent;
					if(currentNode->screen.isMenu){
						CurrentMenuOption = 0;
						MenuoptionCounts = currentNode->screen.optionCount;
						SetMenu(currentNode->screen.menuItems);
						ShowMenu();
						inMenu = 1;
					}
					else{
						ShowInfo(currentNode->screen);
						inMenu = 0;
					}
				}

				break;
			case 15: // okay key
				if(inMenu){
					page = 0;
					nextCurrentNode();
					MenuoptionCounts = currentNode->screen.optionCount;
					if(currentNode->screen.isMenu){
						CurrentMenuOption = 0;
						SetMenu(currentNode->screen.menuItems);
						ShowMenu();
						inMenu = 1;
					}
					else{
						clear();
						ShowInfo(currentNode->screen);
						inMenu = 0;
						if(currentNode->screen.id == -1){
							startState = 3;
							startStateLastTime = HAL_GetTick();
						}
					}

				}
				break;
			case 16:
				if(currentNode->screen.id == 4){
					setLigthToggle(&(currentNode->screen),3,Lights);
					ShowInfo(currentNode->screen);
				}
				else if(currentNode->screen.id == 18){
					setLigthToggle(&(currentNode->screen),3,newScenarioLights);
					ShowInfo(currentNode->screen);
				}

			break;

		}
	}else if(startState == 0 && key == 16){ // blue push button action
		setCursor(0,0);
		if(strlen(startMassage) > 0)
			print(startMassage);
		else
			print("WELLCOME       ");
		display();
		startState = 1;
		startStateLastTime = HAL_GetTick();
	}

}

extern TIM_HandleTypeDef *buzzerPwmTimer;
extern uint32_t buzzerPwmChannel;
extern TIM_HandleTypeDef htim2;
void buzzerChangeTone(uint16_t freq, uint16_t volume) {
    if (freq == 0 || freq > 20000) {
        __HAL_TIM_SET_COMPARE(buzzerPwmTimer, buzzerPwmChannel, 0);
    } else {
        const uint32_t internalClockFreq = HAL_RCC_GetSysClockFreq();
        const uint32_t prescaler = 1 + internalClockFreq / freq / 60000;
        const uint32_t timerClock = internalClockFreq / prescaler;
        const uint32_t periodCycles = timerClock / freq;
        const uint32_t pulseWidth = volume * periodCycles / 1000 / 2;

        buzzerPwmTimer->Instance->PSC = prescaler - 1;
        buzzerPwmTimer->Instance->ARR = periodCycles - 1;
        buzzerPwmTimer->Instance->EGR = TIM_EGR_UG;

        __HAL_TIM_SET_COMPARE(buzzerPwmTimer, buzzerPwmChannel, pulseWidth);
    }
}
void alarm(int position ){
    static int time = 0 ;
    int frequency;
    int period = 1001;
    switch(position){
        case 0:
            frequency = 0;
            break;
        case 1: frequency =(int)((maxFrequency-minFrequency)*((sin(2*3.14*((float)(time))/(float)(period))+1)/2)+minFrequency);
            break;
        case 2://sqr
            if(time <period/2){
                frequency = maxFrequency;
            } else{
                frequency = minFrequency;
            }
            break;
        case 3://triangie
            frequency =(int) (minFrequency + (((maxFrequency - minFrequency)*((float )time )/ (float )period)));
            break;
    }
    buzzerChangeTone(frequency,vol);


    time ++;
    if (time >= period)time = 0 ;

}
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;
extern UART_HandleTypeDef huart2;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */
	if(HAL_GetTick() - lastPress >200){

				col =0;
				for (int i = 0 ; i < 4 ; i++ ){
					HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,GPIO_PIN_RESET);
					switch (i) {
						case 0:
							   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
							if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)) row = 0 ;
							break;
						case 1:
							HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_SET);
							if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)) row = 1 ;
							break;
						case 2:
							HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
							if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)) row = 2 ;
							break;
						case 3:
							HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET);
							if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)) row = 3 ;
							break;

					}
				}

				keyHandler();
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,GPIO_PIN_SET);

				lastPress = HAL_GetTick();

			}
  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(keyPad_Pin);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */
    if (HAL_GetTick() - lastPress > 200) {

        col = 1;
        for (int i = 0; i < 4; i++) {
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);
            switch (i) {
                case 0:
                    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
                    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)) row = 0;
                    break;
                case 1:
                    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
                    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)) row = 1;
                    break;
                case 2:
                    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
                    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)) row = 2;
                    break;
                case 3:
                    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
                    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)) row = 3;
                    break;

            }
        }

        keyHandler();
        lastPress = HAL_GetTick();
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_SET);
    }
	while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1));

  /* USER CODE END EXTI1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
  /* USER CODE BEGIN EXTI1_IRQn 1 */

  /* USER CODE END EXTI1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line2 and Touch Sense controller.
  */
void EXTI2_TSC_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_TSC_IRQn 0 */
		if (HAL_GetTick() - lastPress > 200) {

			col = 2;
			for (int i = 0; i < 4; i++) {
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);
				switch (i) {
					case 0:
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
						if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)) row = 0;
						break;
					case 1:
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
						if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)) row = 1;
						break;
					case 2:
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
						if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)) row = 2;
						break;
					case 3:
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
						if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)) row = 3;
						break;

				}
			}

			keyHandler();
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_SET);

			lastPress = HAL_GetTick();

		}
		while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));

  /* USER CODE END EXTI2_TSC_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
  /* USER CODE BEGIN EXTI2_TSC_IRQn 1 */

  /* USER CODE END EXTI2_TSC_IRQn 1 */
}

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */
        if (HAL_GetTick() - lastPress > 200) {

            col = 3;
            for (int i = 0; i < 4; i++) {
                HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);
                switch (i) {
                    case 0:
                        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
                        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)) row = 0;
                        break;
                    case 1:
                        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
                        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)) row = 1;
                        break;
                    case 2:
                        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
                        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)) row = 2;
                        break;
                    case 3:
                        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
                        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)) row = 3;
                        break;

                }
            }

            keyHandler();
            lastPress = HAL_GetTick();
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_SET);


        }
        while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3));

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */
someOneClose = 1;
mSecond = 0;
HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
  /* USER CODE END EXTI4_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(PIR_Pin);
  /* USER CODE BEGIN EXTI4_IRQn 1 */

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
  * @brief This function handles ADC1 and ADC2 interrupts.
  */
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */
    static int counter  =  0 ;
    static int temSum = 0;
//    static int lumSum = 0 ;
    counter ++;
    if(counter %100== 0){

//        lumSum +=  (int) ((HAL_ADC_GetValue(&hadc1) * 99) / 1000);
        int Voltage_mv = (int)(HAL_ADC_GetValue(&hadc2) * 3300 / 4095);
        temSum += Voltage_mv / 10;

        if(counter == 900){

//            int lumCopy = lum;
            int temCopy = temperature;
//            lum = lumSum/10;
            temperature = temSum / 100;
            counter = 0 ;
            temSum = 0 ;
//            lumSum = 0 ;

//            if(lum < lumCopy)HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,1);
            if(temperature > temSum)HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,0);
        }

    }




    HAL_ADC_Start_IT(&hadc1);
    HAL_ADC_Start_IT(&hadc2);
  /* USER CODE END ADC1_2_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  HAL_ADC_IRQHandler(&hadc2);
  /* USER CODE BEGIN ADC1_2_IRQn 1 */

  /* USER CODE END ADC1_2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */
	HAL_TIM_Base_Start_IT(&htim2);

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
//	if(temperature>temThreshold && !shown1){
//		setCursor(0,0);
//		print("caution     ");
//		display();
//		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
//		shown1 = 1;
//
//	}
  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */
	HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles Timer 6 interrupt and DAC underrun interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */
static int alarmPosition = 0;
//setCursor(0,0);
char temp2[2] = " ";
sprintf(temp2,"%d",temperature);
//print(temp2);
		if(startState == 1 && HAL_GetTick() - startStateLastTime >= 1000){
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10,1);
			clear();
			setCursor(0,0);
			print(">> ControlPanel  ");
			GenerateMenu();
			generateMenu = 1;
			startState = 2;
			display();
		}
		if(startState == 3 && HAL_GetTick() - startStateLastTime >= 1000){
			startState = 0;
			generateMenu = 0;
			clear();
		}
		if(temperature <= temThreshold && !someOneClose)
			alarmPosition = 0;
		if(temperature > temThreshold || someOneClose){
			if(securityAlarm == 1 && someOneClose &&temAlarm==1 && temperature > temThreshold){
				alarmPosition = 1;
			}
			else if(securityAlarm == 1 && someOneClose){
				alarmPosition = 2;
			}
			else if(temAlarm==1 && temperature > temThreshold){
				alarmPosition = 3;
			}
			// this part can be changed by animation
			char temp[1];
			sprintf(temp,"%d",mSecond);
			setCursor(17,0);
			print(temp);
			if(mSecond < 10){
				setCursor(0,0);
				if(temperature > temThreshold){
					print("High Temper     ");
					if(temAlarm){
					// activate temperature alarm
					}
				}else{
					print("                   ");
				}
				setCursor(0,1);
				if(someOneClose && securityAlert){
					print("Some One Near By   ");
					if(securityAlarm){
					// activate security alarm
					}
				}else{
					print("                   ");
				}
				setCursor(0,2);
				print("                   ");
				setCursor(0,3);
				print("                   ");
				shown = 1;
				if(mSecond %3 ==0)
					noDisplay();
				else
					display();
			}
			else{
				shown = 0;
				clear();
				if(startState == 2){
					if(inMenu)
						ShowMenu();
					else
						ShowInfo(currentNode->screen);
				}
			}

		}else if(shown){
			alarmPosition = 0;
			shown = 0;
			clear();
			if(startState == 2){
				if(inMenu)
					ShowMenu();
				else
					ShowInfo(currentNode->screen);
			}
		}
		mSecond++;
		if(mSecond>=20)
		{
			mSecond =0;
			someOneClose = 0;
		}
  alarm(alarmPosition);
  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */
	HAL_TIM_Base_Start_IT(&htim6);

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
