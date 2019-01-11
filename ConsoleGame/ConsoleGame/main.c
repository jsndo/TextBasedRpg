#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <math.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define CHARACTER_ATTRIBUTES_SIZE 50
#define ENEMY_ATTRIBUTES_SIZE 30


struct player
{
	char* playerName;
	char* playerArmor;
	char* playerWeapon;
	int playerHealth;
	int playerCurrentHealth;
	int playerLevel;
	int playerExpUntilNextLevel;
	int playerAttack;
	int playerDefense;
	int playerSpeed;
};

struct enemy
{
	char* enemyName;
	int enemyHealth;
	int enemyAttack;
	int enemyDefense;
	int enemySpeed;
};


void printIntroduction(void);

void displayMenuItems(void);

struct player* createPlayer();

struct enemy* createEnemy();

void battleMonster(struct player* player, struct enemy* enemy);


int main(void)
{
	
	printIntroduction();

	struct player* player = malloc(sizeof(struct player));
	player = createPlayer();

	//Creating and initializing attributes for the enemy
	struct enemy* enemy = malloc(sizeof(struct enemy));
	enemy = createEnemy();
	

	fgets(player->playerName, CHARACTER_ATTRIBUTES_SIZE + 1, stdin);

	system("cls");

	printf("Welcome, %sWhat would you like to do:\n\n", player->playerName);
	displayMenuItems();

	int menuChoice;
	bool gameIsRunning = true;
	scanf_s("%d", &menuChoice);
	
	while (gameIsRunning)
	{
		switch (menuChoice)
		{
			case 1:
				system("cls");
				printf("A monster has appeared! Brace yourself.\n");
				
				battleMonster(player, enemy);
				break;
			case 2:
				printf("You have a %s and %s.", player->playerArmor, player->playerWeapon);
				break;
			case 3:
				printf("You have 3 potions, each healing 20 HP");
				break;
			case 4:
				printf("\nName: %sLevel: %d\nHP: %d\nAttack: %d\nDefense: %d\nSpeed: %d\nUntil next level: %d"
					,player->playerName, player->playerLevel, player->playerHealth, player->playerAttack
					,player->playerDefense, player->playerSpeed, player->playerExpUntilNextLevel);
				break;
			case 5:
				printf("Goodbye...");
				gameIsRunning = false;
				break;
			default:
				break;
		}

		if (gameIsRunning == true)
		{
			printf("\n\nEnter any key to go back to the menu...");
			getch();
			system("cls");
			displayMenuItems();
			scanf_s("%d", &menuChoice);
		}
		

	}
	

	getch();

	//Deallocating memory for the player and enemy

	free(player);
	player = NULL;

	free(enemy);
	enemy = NULL;


	return 0;

}



void printIntroduction(void)
{
	printf("Welcome to the Console RPG Game. This game will be full of monsters in which"
		" you will be able to battle.\n");

	printf("But first, what is your name: ");


}



void displayMenuItems(void)
{
	printf("1). Battle monsters\n"
		"2). Check equipment\n"
		"3). Browse item\n"
		"4). Check stats\n"
		"5). Exit game\n\n");
}


struct player* createPlayer()
{
	//Creating and initializing attributes for the player
	struct player* player = malloc(sizeof(struct player));
	player->playerName = (char*)malloc(CHARACTER_ATTRIBUTES_SIZE + 1 * sizeof(char));
	player->playerArmor = (char*)malloc(CHARACTER_ATTRIBUTES_SIZE + 1 * sizeof(char));
	player->playerWeapon = (char*)malloc(CHARACTER_ATTRIBUTES_SIZE + 1 * sizeof(char));

	player->playerArmor = "Wooden Shield";
	player->playerWeapon = "Wooden Sword";
	player->playerHealth = 100;
	player->playerCurrentHealth = player->playerHealth;
	player->playerLevel = 1;
	player->playerExpUntilNextLevel = 30;
	player->playerAttack = 10;
	player->playerDefense = 12;
	player->playerSpeed = 8;

	if (player == NULL)
	{
		printf("Could not allocate memory for player!");
		return -1;
	}

	return player;

}


struct enemy* createEnemy()
{
	//Creating and initializing attributes for the player
	struct enemy* enemy = malloc(sizeof(struct enemy));
	enemy->enemyName = (char*)malloc(ENEMY_ATTRIBUTES_SIZE + 1 * sizeof(char));
	enemy->enemyHealth = 60;
	enemy->enemyAttack = 8;
	enemy->enemyDefense = 8;
	enemy->enemySpeed = 12;

	if (enemy == NULL)
	{
		printf("Could not allocate memory for enemy!");
		return -1;
	}

	return enemy;

}

void battleMonster(struct player* player, struct enemy* enemy)
{
	//Credit to Aaron Krogh for producing the song titled "Leviathan"
	PlaySound(TEXT("Leviathan.wav"), NULL, SND_LOOP | SND_ASYNC);
	bool battleIsOver = false, playerTurn = false, playerIsFaster = false;

	Sleep(3000);

	//Calculates damage inflicted on the entities, depending on who attacked who
	int dmgToEnemy, dmgToPlayer, playerMaxHp, potion = 20;

	dmgToEnemy = player->playerAttack * (float)(player->playerAttack / (float)enemy->enemyDefense);
	dmgToPlayer = enemy->enemyAttack * (float)(enemy->enemyAttack / (float)player->playerDefense);

	//Check and see which player is faster
	if (player->playerSpeed > enemy->enemySpeed)
	{
		playerTurn = true;
		playerIsFaster = true;
	}

	while (battleIsOver == false)
	{
		printf("Enemy HP: %d\t\t\tPlayer HP: %d\n\n", enemy->enemyHealth, player->playerCurrentHealth);

		if (playerTurn == true)
		{
			printf("What will you do?\n\n1). Attack\n2.) Use potion\n3.) Run\n\n");
			int battleChoice = 0;

			scanf_s("%d", &battleChoice);
			switch (battleChoice)
			{
			case 1:
				enemy->enemyHealth -= dmgToEnemy;
				printf("You just inflicted %d damage to the enemy!\n", dmgToEnemy);

				if (enemy->enemyHealth <= 0)
				{
					printf("You defeated the enemy!\n");

					PlaySound(TEXT("Pause Sound"), NULL, 0, NULL);
					enemy->enemyHealth = 60;
					battleIsOver = true;
				}
				break;
			case 2:
				if (player->playerHealth + potion >= 100)
				{
					//This caps off HP at its maximum value
					player->playerCurrentHealth = player->playerHealth;
					printf("You are full healed!\n");
				}
				else
				{
					player->playerCurrentHealth += potion;
					printf("You replenlished %d HP!\n", potion);
				}
				break;
			case 3:
				if (!playerIsFaster)
				{
					printf("You are too slow! You have to continue fighting.\n");
				}
				else
				{
					printf("You fled the enemy successfully!\n");
					enemy->enemyHealth = 60;
					battleIsOver = true;
				}
				break;

			default:
				break;
			}
		}
		else
		{
			player->playerCurrentHealth -= dmgToPlayer;
			printf("Enemy attacked you!!\nYou lost %d health!\n", dmgToPlayer);

			if (player->playerCurrentHealth <= 0)
			{
				printf("You just died!\n");
				enemy->enemyHealth = 60;
				battleIsOver = true;
			}
		}

		Sleep(4000);
		system("cls");
		playerTurn = !playerTurn;

	}
}


