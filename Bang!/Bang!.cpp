#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <malloc.h>
#define MAX_SIZE_GUN 6				//한 탄창 당 담을 수 있는 총 탄 수
#define MAX_HEALTH 5				//처음 체력(이거 담배 써도 넘으면 안되는건가)
#define MAX_ITEM 3					//최대 아이템 수

typedef struct bullet {
	int dmg;						//총알 데미지(공포탄이면 0, 실탄이면 1, 톱아이템으로 데미지 수정가능)
	bool doAIknow;					//ai구현용 상대가 공포탄인지 실탄인지 알고있는지 판단용
	struct bullet* link;
}Bullet;

typedef struct gun {
	int livenum;					//실탄 수
	int blanknum;					//공포탄 수
	int bulletnum;					//남은 총 탄 수(실탄+공포탄)
	Bullet* front;
	Bullet* rear;
}Gun;

typedef struct actor {
	char name[5];
	int HP;
	bool canActorMove;
	int Saw;
	int Cigarette;
	int Medicine;
	int Handcuffs;
	int Dotbogi;
}Actor;


void initGun(Gun* gun);
int is_empty(Gun* gun);
int is_full(Gun* gun);
Bullet createBullet();
void enqueueGun(Gun* gun, Bullet bullet);
int dequeueGun(Gun* gun);
int peakGun(Gun* gun);
void print_Gun(Gun* gun, int temp);
void initActor(Actor* actor);
void Game(Actor* player, int ai);

int main() {
	srand(time(NULL));
	Actor* player= (Actor*)malloc(sizeof(Actor));
	initActor(player);
	int select = 0;
	printf("난이도를 선택하세용가리치킨\n1.쉬움 2. 보통 3.어려움");
	scanf("%d", &select);
	Game(player, select);
	return 0;
}


void initGun(Gun* gun)
{
	gun->front = NULL;
	gun->rear = NULL;
}

int is_empty(Gun* gun)
{
	return(gun->front == NULL);
}

int is_full(Gun* gun)
{
	return 0;//흠 full일 경우가 있나
}

Bullet createBullet()
{
	Bullet bullet;
	bullet.dmg = rand() % 2;
	bullet.doAIknow = FALSE;
	return bullet;
}

void enqueueGun(Gun* gun, Bullet bullet)
{
	Bullet* temp = (Bullet*)malloc(sizeof(Bullet));
	temp->dmg = bullet.dmg;
	temp->doAIknow = bullet.doAIknow;
	temp->link = NULL;
	if (is_empty(gun)) {
		gun->front = temp;
		gun->rear = temp;
	}
	else {
		gun->rear->link = temp;
		gun->rear = temp;
	}
}

int dequeueGun(Gun* gun)
{
	Bullet* temp = gun->front;
	int dmg;
	if (is_empty(gun)) {
		exit(1);
	}
	else {
		dmg = temp->dmg;
		gun->front = gun->front->link;
		if (gun->front == NULL) gun->rear = NULL;
		free(temp);
		return dmg;
	}
}

int peakGun(Gun* gun)
{
	return gun->front->dmg;
}

void print_Gun(Gun* gun, int num)
{
	Bullet* temp = gun->front;
	for (int i = 0; i < num; i++) {
		printf("%d", gun->front->dmg);
		gun->front = gun->front->link;
	}
	printf("\n");
	gun->front = temp;
}

void initActor(Actor* actor)
{
	actor->canActorMove = TRUE;
	actor->Cigarette = 1;
	actor->Dotbogi = 1;
	actor->Handcuffs = 1;
	actor->HP = MAX_HEALTH;
	actor->Medicine = 0;
	actor->Saw = 0;
}

void Game(Actor* player, int ai)
{
	Gun* gun = (Gun*)malloc(sizeof(Gun));
	initGun(gun);
	int myhealth = player->HP;

	Actor* enemy = (Actor*)malloc(sizeof(Actor));
	initActor(enemy);
	if (ai == 3) {
		enemy->Cigarette = 2;
		enemy->Dotbogi = 2;
		enemy->Handcuffs = 2;
	}
	else if (ai == 2) {

	}
	else {

	}
	int enemyhealth = enemy->HP;
	printf("%d단계\n", ai);

	printf("게임 시작\n");
	printf("현재 나의 체력: %d 상대 체력: %d\n", myhealth, enemyhealth);
	int select = 0;
	int enemyselect = 0;
	Sleep(1000);
	while ((myhealth > 0) && (enemyhealth > 0)) {
		if (gun->rear == NULL) {
			printf("\n새 탄창!\n");
			Sleep(1000);
			int temp = (rand() % MAX_SIZE_GUN) + 1;
			int livesum = 0;
			for (int i = 0; i < temp; i++) {
				enqueueGun(gun, createBullet());
				livesum += gun->rear->dmg;
			}
			//print_Gun(gun, temp);
			gun->bulletnum = temp;
			gun->livenum = livesum;
			gun->blanknum = temp - livesum;
			printf("[남은 총알 수: %d발 실탄: %d발 공포탄: %d발]\n", gun->bulletnum, gun->livenum, gun->blanknum);
		}

		printf("나의 차례\n");
		while (player->canActorMove) {
			Sleep(500);
			printf("무엇을 할 것인가?\n1.격발 2.아이템\n");
			Sleep(500);
			scanf("%d", &select);
			if (select == 1) {
				printf("누굴 쏠 것 인가?\n1.나 자신 2. 상대\n");
				Sleep(500);
				scanf("%d", &select);
				if (select == 2) {
					printf("탕!...\n");
					Sleep(1000);
					if (dequeueGun(gun)) {
						printf("실탄이었다!\n");
						enemyhealth--;
						gun->livenum--;
					}
					else {
						printf("공포탄이었다...\n");
						gun->blanknum--;
					}
					break;
				}
				else {
					printf("탕!...\n");
					if (dequeueGun(gun)) {
						printf("윽! 실탄이었다..\n");
						myhealth--;
						gun->livenum--;
						break;
					}
					else {
						printf("휴! 공포탄이었다~\n");
						gun->blanknum--;
					}
				}
			}
			else if (select == 2) {
				printf("무슨 아이템을 사용할까?\n");
				printf("1. 담배 : %d개\n", player->Cigarette);
				printf("2. 돋보기 : %d개\n", player->Dotbogi);
				printf("3. 수갑 : %d개\n", player->Handcuffs);
				scanf("%d", &select);
				if ((select == 1) && (player->Cigarette > 0)) {
					printf("뻐끔뻐끔\n");
					Sleep(1000);
					printf("체력을 회복했다.\n");
					myhealth++;
					player->Cigarette--;
				}
				else if ((select == 2) && (player->Dotbogi > 0)) {
					if (peakGun(gun) == 1) {
						printf("실탄이다.\n");
					}
					else {
						printf("공포탄이다.\n");
					}
					player->Dotbogi--;
				}
				else if ((select == 3) && (player->Handcuffs > 0)) {
					enemy->canActorMove = FALSE;
					printf("상대의 손에 수갑을 채웠다.\n");
					player->Handcuffs--;
				}
				else {
					printf("아이템을 가지고 있지 않는 듯 하다.");
				}
			}
			else if (select == 9) {
				enemyhealth = 0;
			}
			Sleep(1000);
			gun->bulletnum = gun->livenum + gun->blanknum;
			printf("\n현재 나의 체력: %d 상대 체력: %d\n", myhealth, enemyhealth);
			printf("[남은 총알 수 : %d발]\n", gun->bulletnum);
			if (enemyhealth == 0) break;
			if (myhealth == 0) break;
			if (gun->rear == NULL) break;
		}
		if (!player->canActorMove) {
			printf("수갑을 풀어내느라 총을 쏘지 못했다.\n");
			player->canActorMove = TRUE;
		}
		if (enemyhealth <= 0) break;
		if (myhealth <= 0) break;
		if (gun->rear == NULL) continue;

		printf("\n상대의 차례!\n");
		while (enemy->canActorMove) {
			Sleep(1000);
			if (ai == 1) {
				enemyselect = 1;
			}
			else if (ai == 2) {
				if (gun->livenum >= gun->blanknum) {
					enemyselect = 1;
				}
				else {
					enemyselect = 2;
				}
			}
			else if (ai == 3) {
				if (gun->front->doAIknow) {
					if (gun->front->dmg) {
						enemyselect = 1;
					}
					else {
						enemyselect = 2;
					}
				}
				else if ((enemy->Dotbogi > 0) && (gun->livenum > 0) && (gun->blanknum>0)) {
					enemyselect = 5;
				}
				else if ((enemy->Handcuffs > 0) && (player->canActorMove == TRUE) && (gun->livenum>0)) {
					enemyselect = 4;
				}
				else if ((enemy->Cigarette > 0)&&(enemyhealth<MAX_HEALTH)) {
					enemyselect = 3;
				}
				else if (gun->livenum >= gun->blanknum) {
					enemyselect = 1;
				}
				else {
					enemyselect = 2;
				}
			}
			if (enemyselect == 1) {
				printf("상대는 이쪽으로 총구를 겨눴다.\n");
				Sleep(1000);
				printf("탕!...\n");
				if (dequeueGun(gun)) {
					printf("윽! 실탄이었다..\n");
					myhealth--;
					gun->livenum--;
					break;
				}
				else {
					printf("휴! 공포탄이었다~\n");
					gun->blanknum--;
					break;
				}

			}
			else if (enemyselect == 2) {
				printf("상대는 자신의 머리에 총구를 겨눴다.\n");
				Sleep(1000);
				printf("탕!...\n");
				if (dequeueGun(gun)) {
					printf("ㅋㅋ\n");
					enemyhealth--;
					gun->livenum--;
					break;
				}
				else {
					printf("이런 공포탄이었다...\n");
					gun->blanknum--;
				}
			}
			else if (enemyselect == 3) {
				printf("상대는 담배를 피기 시작했다.\n");
				enemyhealth++;
				enemy->Cigarette--;
			}
			else if (enemyselect == 4) {
				printf("상대는 내 손에 수갑을 채웠다.\n");
				Sleep(1000);
				player->canActorMove = FALSE;
				enemy->Handcuffs--;
			}
			else if (enemyselect == 5) {
				printf("상대는 돋보기로 총을 살피기 시작했다.\n");
				Sleep(1000);
				gun->front->doAIknow = TRUE;
				enemy->Dotbogi--;
			}
			Sleep(500);
			if (enemyhealth == 0) break;
			if (myhealth == 0) break;
			if (gun->rear == NULL) break;
		}
		if (!enemy->canActorMove) {
			printf("상대는 수갑을 풀어내느라 총을 쏘지 못했다.\n");
			enemy->canActorMove = TRUE;
		}
		gun->bulletnum = gun->livenum + gun->blanknum;
		printf("\n현재 나의 체력: %d 상대 체력: %d\n", myhealth, enemyhealth);
		printf("[남은 총알 수 : %d발]\n", gun->bulletnum);
		if (enemyhealth == 0) break;
		if (myhealth == 0) break;

	}
	free(gun);

	if (myhealth > enemyhealth) {
		printf("승리!");
	}
	else {
		printf("으하하 너의 패배다\nGAME OVER");
	}
}



