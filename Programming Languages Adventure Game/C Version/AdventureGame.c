/* AdventureGame.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct location {
	char *name;
	int confInt;
	struct location *left;
	struct location *forward;
	struct location *right;		
};

struct token {
	struct location *place;
};

// Location variables

struct location valley;
struct location path;
struct location cliff;
struct location forkLoc;
struct location mountaintop;
struct location maze0;
struct location maze1;
struct location maze2;
struct location maze3;
struct location maze4;
struct location labyrinth0;
struct location labyrinth1;
struct location labyrinth2;
struct location labyrinth3;
struct location labyrinth4;
struct location labyrinth5;
struct location labyrinth6;
struct location labyrinth7;
struct location labyrinth8;
struct location labyrinth9;
struct location labyrinth10;
struct location labyrinth11;
struct location labyrinth12;
struct location labyrinth13;
struct location labyrinth14;
struct location labyrinth15;

// Token variables
struct token you;
struct token ogre;
struct token treasure;

// For configuration purposes
struct location *config[25];

// Flags
int ogreFlag;
int treasureFlag;
int cliffFlag;

void initialize();
void move(char *direction);
void runGame(char *input, char *configuration, char *results);
void vrRun();

void initialize() {
	// Set all connections for every location struct
	valley.forward = &path;
	valley.left = NULL;
	valley.right = NULL;
	valley.confInt = 32;
	valley.name = "valley.";
	
	path.right = &cliff;
	path.left = &cliff;
	path.forward = &forkLoc;
	path.confInt = 0;
	path.name = "path.";
	
	cliff.confInt = 1;
	
	forkLoc.forward = &labyrinth0;
	forkLoc.left = &maze0;
	forkLoc.right = &mountaintop;
	forkLoc.confInt = 2;
	forkLoc.name = "fork.";
	
	mountaintop.left = &forkLoc;
	mountaintop.right = NULL;
	mountaintop.forward = NULL;
	mountaintop.confInt = 3;
	mountaintop.name = "mountaintop";
	
	maze0.left = &maze1;
	maze0.right = &maze3;
	maze0.forward = NULL;
	maze0.confInt = 4;
	maze0.name = "maze 0";
	
	maze1.left = &maze0;
	maze1.right = &maze2;
	maze1.forward = NULL;
	maze1.confInt = 5;
	maze1.name = "maze 1.";
	
	maze2.forward = &maze4;
	maze2.left = &forkLoc;
	maze2.right = &maze0;
	maze2.confInt = 6;
	maze2.name = "maze 2.";
	
	maze3.left = &maze0;
	maze3.right = &maze3;
	maze3.forward = NULL;
	maze3.confInt = 7;
	maze3.name = "maze 3.";
	
	maze4.forward = &mountaintop;
	maze4.right = &maze4;
	maze4.left = NULL;
	maze4.confInt = 8;
	maze4.name = "maze 4.";
	
	labyrinth0.left = &labyrinth1;
	labyrinth0.right = &labyrinth15;
	labyrinth0.forward = NULL;
	labyrinth0.confInt = 9;
	labyrinth0.name = "labyrinth 0.";
	
	labyrinth1.left = &labyrinth2;
	labyrinth1.right = &labyrinth10;
	labyrinth1.forward = NULL;
	labyrinth1.confInt = 10;
	labyrinth1.name = "labyrinth 1.";
	
	labyrinth2.left = &labyrinth3;
	labyrinth2.right = &labyrinth9;
	labyrinth2.forward = NULL;
	labyrinth2.confInt = 11;
	labyrinth2.name = "labyrinth 2.";
	
	labyrinth3.right = &labyrinth4;
	labyrinth3.left = NULL;
	labyrinth3.forward = NULL;
	labyrinth3.confInt = 12;
	labyrinth3.name = "labyrinth 3.";
	
	labyrinth4.left = &labyrinth5;
	labyrinth4.right = &labyrinth9;
	labyrinth4.forward = NULL;
	labyrinth4.confInt = 13;
	labyrinth4.name = "labyrinth 4.";
	
	labyrinth5.left = &labyrinth6;
	labyrinth5.right = &labyrinth8;
	labyrinth5.forward = NULL;
	labyrinth5.confInt = 14;
	labyrinth5.name = "labyrinth 5.";
	
	labyrinth6.right = &labyrinth7;
	labyrinth6.left = NULL;
	labyrinth6.forward = NULL;
	labyrinth6.confInt = 15;
	labyrinth6.name = "labyrinth 6.";
	
	labyrinth7.left = &labyrinth8;
	labyrinth7.right = &labyrinth12;
	labyrinth7.forward = NULL;
	labyrinth7.confInt = 16;
	labyrinth7.name = "labyrinth 7.";
	
	labyrinth8.left = &labyrinth7;
	labyrinth8.right = &labyrinth9;
	labyrinth8.forward = NULL;
	labyrinth8.confInt = 17;
	labyrinth8.name = "labyrinth 8.";
	
	labyrinth9.left = &labyrinth10;	
	labyrinth9.right = &labyrinth8;
	labyrinth9.forward = NULL;
	labyrinth9.confInt = 18;
	labyrinth9.name = "labyrinth 9.";
	
	labyrinth10.left = &labyrinth9;
	labyrinth10.right = &labyrinth11;
	labyrinth10.forward = NULL;
	labyrinth10.confInt = 19;
	labyrinth10.name = "labyrinth 10.";
	
	labyrinth11.left = &labyrinth12;
	labyrinth11.right = &labyrinth10;
	labyrinth11.forward = NULL;
	labyrinth11.confInt = 20;
	labyrinth11.name = "labyrinth 11.";
	
	labyrinth12.left = &labyrinth11;
	labyrinth12.right = &labyrinth13;
	labyrinth12.forward = NULL;
	labyrinth12.confInt = 21;
	labyrinth12.name = "labyrinth 12.";
	
	labyrinth13.left = &labyrinth14;
	labyrinth13.right = NULL;
	labyrinth13.forward = NULL;
	labyrinth13.confInt = 22;
	labyrinth13.name = "labyrinth 13.";
	
	labyrinth14.left = &labyrinth15;
	labyrinth14.right = &labyrinth11;
	labyrinth14.forward = NULL;
	labyrinth14.confInt = 23;
	labyrinth14.name = "labyrinth 14.";
	
	labyrinth15.left = &forkLoc;
	labyrinth15.right = &forkLoc;
	labyrinth15.forward = NULL;
	labyrinth15.confInt = 24;
	labyrinth15.name = "labyrinth 15.";
	
	
	// Load up the configuration array
	config[0] = &path;
	config[1] = &cliff;
	config[2] = &forkLoc;
	config[3] = &mountaintop;
	config[4] = &maze0;
	config[5] = &maze1;
	config[6] = &maze2;
	config[7] = &maze3;
	config[8] = &maze4;
	config[9] = &labyrinth0;
	config[10] = &labyrinth1;
	config[11] = &labyrinth2;
	config[12] = &labyrinth3;
	config[13] = &labyrinth4;
	config[14] = &labyrinth5;
	config[15] = &labyrinth6;
	config[16] = &labyrinth7;
	config[17] = &labyrinth8;
	config[18] = &labyrinth9;
	config[19] = &labyrinth10;
	config[20] = &labyrinth11;
	config[21] = &labyrinth12;
	config[22] = &labyrinth13;
	config[23] = &labyrinth14;
	config[24] = &labyrinth15;
}

void move(char *direction) {
	
		if(strcmp(direction, "left.") == 0 && you.place->left != NULL)
			you.place = you.place->left;
			
		if(strcmp(direction, "forward.") == 0 && you.place->forward != NULL)
			you.place = you.place->forward;
			
		if(strcmp(direction, "right.") == 0 && you.place->right != NULL)
			you.place = you.place->right;
}

void runGame(char *input, char *configuration, char *results) {
		
		
		FILE *conFile = fopen(configuration, "r");
		FILE *logFile = fopen(results, "a");
		
		int scenario = 1; // counter for scenario.
		int moves; // counter for amount of moves.
		
		char line[256]; // Configuration string
		char line2[256]; // Command String
		char gameResult[256]; // String that stores that games result
		
		// Loop for the configuration file
		while(fgets(line, sizeof line, conFile) != NULL) {
			
			// Setup flags
			ogreFlag = 0;
			treasureFlag = 0;
			cliffFlag = 0;
			
			// Setup increment
			moves = 0;
			
			// Setup your, Treasure, and Orc location
			you.place = &valley;
			char* locP = strtok(line, ","); // Get the ogre location
			int oLoc = atoi(locP);
			locP = strtok(NULL, "\n"); // Get the treasure location
			int tLoc = atoi(locP);
			ogre.place = config[oLoc];
			treasure.place = config[tLoc];
			
			
			
			// open file
			FILE *inFile = fopen(input, "r");
			
			// Loop for the run of the current scenario
			while(fgets(line2, sizeof line2, inFile) != NULL
					&& ogreFlag == 0 && treasureFlag == 0 && cliffFlag == 0) {
				
				int len = strlen(line2); //This is to avoid errors.
				
				if(line2[len-1] == '\n'){
					line2[len-1] = 0;
				}
				
				move(line2);
				moves++;
				
				if(you.place->confInt == ogre.place->confInt){
					ogreFlag = 1;
				}
				
				if(you.place->confInt == treasure.place->confInt){
					treasureFlag = 1;
				}
				
				if(you.place->confInt == cliff.confInt){
					cliffFlag = 1;
				}
				
			}
			
			fclose(inFile);
			
			if(ogreFlag == 1) {
				sprintf(gameResult, "Scenario %d: Ogre sucked your brains out in %d move(s) at %s\n",
				scenario, moves, you.place->name);
				fprintf(logFile, gameResult);
			} else if(treasureFlag == 1) {
				sprintf(gameResult, "Scenario %d: Treasure found in %d move(s) at %s\n",
				scenario, moves, you.place->name);
				fprintf(logFile, gameResult);
			} else if(cliffFlag == 1) {
				sprintf(gameResult, "Scenario %d: Fell off cliff in %d move(s).\n",
				scenario, moves);
				fprintf(logFile, gameResult);
			} else {
				sprintf(gameResult, "Scenario %d: Ran out of commands.\n",
				scenario);
				fprintf(logFile,gameResult);
			}
			
			scenario++;
		}
		
		fclose(conFile);
}

void vrRun() {
		FILE *conFile = fopen("configurations.txt", "r");
		FILE *logFile = fopen("vrLog.txt", "a");
		FILE *inFile = fopen("commands_vR", "r");
		
		int scenario = 1; // counter for scenario.
		int moves; // counter for amount of moves.
		
		char line[256]; // Configuration string
		char line2[256]; // Command String
		char gameResult[256]; // String that stores that games result
		
		// Loop for the configuration file
		while(fgets(line, sizeof line, conFile) != NULL) {
			
			// Setup flags
			ogreFlag = 0;
			treasureFlag = 0;
			cliffFlag = 0;
			
			// Setup increment
			moves = 0;
			
			// Setup your, Treasure, and Orc location
			you.place = &valley;
			char* locP = strtok(line, ","); // Get the ogre location
			int oLoc = atoi(locP);
			locP = strtok(NULL, "\n"); // Get the treasure location
			int tLoc = atoi(locP);
			ogre.place = config[oLoc];
			treasure.place = config[tLoc];
			
			
			// Loop for the run of the current scenario
			while(ogreFlag == 0 && treasureFlag == 0 && cliffFlag == 0) {
				
				if(fgets(line2, sizeof line2, inFile) == NULL) {
					rewind(inFile);
					fgets(line2, sizeof line2, inFile);
				}
				
				
				int len = strlen(line2); //This is to avoid errors.
				
				if(line2[len-1] == '\n'){
					line2[len-1] = 0;
				}
				
				move(line2);
				moves++;
				
				if(you.place->confInt == ogre.place->confInt){
					ogreFlag = 1;
				}
				
				if(you.place->confInt == treasure.place->confInt){
					treasureFlag = 1;
				}
				
				if(you.place->confInt == cliff.confInt){
					cliffFlag = 1;
				}
				
			}
			
			if(ogreFlag == 1) {
				sprintf(gameResult, "Scenario %d: Ogre sucked your brains out in %d move(s) at %s\n",
				scenario, moves, you.place->name);
				fprintf(logFile, gameResult);
			} else if(treasureFlag == 1) {
				sprintf(gameResult, "Scenario %d: Treasure found in %d move(s) at %s\n",
				scenario, moves, you.place->name);
				fprintf(logFile, gameResult);
			} else if(cliffFlag == 1) {
				sprintf(gameResult, "Scenario %d: Fell off cliff in %d move(s).\n",
				scenario, moves);
				fprintf(logFile, gameResult);
			}
			
			scenario++;
		}
		
		fclose(conFile);
		fclose(inFile);
}

int main(int argc, char *argv[]) {
	initialize();
	runGame("commands_v1", "configurations.txt", "v1logfile.txt");
	runGame("commands_v2", "configurations.txt", "v2logfile.txt");
	runGame("commands_v3", "configurations.txt", "v3logfile.txt");
	runGame("commands_v4", "configurations.txt", "v4logfile.txt");
	runGame("commands_v5", "configurations.txt", "v5logfile.txt");
	vrRun();
	exit(0);
}
