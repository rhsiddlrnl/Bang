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
void print_Gun(Gun* gun, int temp);
void initActor(Actor* actor);

int main() {
	srand(time(NULL));
	Gun* gun = (Gun*)malloc(sizeof(Gun));
	initGun(gun);

	Actor* player= (Actor*)malloc(sizeof(Actor));
	initActor(player);
	int myhealth = player->HP;

	Actor* enemy = (Actor*)malloc(sizeof(Actor));
	initActor(enemy);
	int enemyhealth = enemy->HP;

	printf("게임 시작\n");
	printf("현재 나의 체력: %d 상대 체력: %d\n", myhealth, enemyhealth);
	int select = 0;
	int enemyselect = 0;
	Sleep(1000);
	while ((myhealth>0) && (enemyhealth>0)) {
		if (gun->rear == NULL) {
			printf("\n새 탄창!\n");
			Sleep(1000);
			int temp = (rand() % MAX_SIZE_GUN)+1;
			int livesum = 0;
			for (int i = 0; i < temp; i++) {
				enqueueGun(gun, createBullet());
				livesum += gun->rear->dmg;
			}
			print_Gun(gun, temp);
			gun->bulletnum = temp;
			gun->livenum = livesum;
			gun->blanknum = temp - livesum;
			printf("%d %d %d\n", gun->bulletnum, gun->livenum, gun->blanknum);
		}

		printf("나의 차례\n");
		while (1) {
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
					}
					else {
						printf("공포탄이었다...\n");
					}
					break;
				}
				else {
					printf("탕!...\n");
					if (dequeueGun(gun)) {
						printf("윽! 실탄이었다..\n");
						myhealth--;
						break;
					}
					else {
						printf("휴! 공포탄이었다~\n");
					}
				}
			}
			else if (select == 2) {
				printf("맥주를 사용했다.\n");
				myhealth++;
			}
			Sleep(1000);
			printf("\n현재 나의 체력: %d 상대 체력: %d\n", myhealth, enemyhealth);
			if (enemyhealth == 0) break;
			if (myhealth == 0) break;
			if (gun->rear == NULL) break;
		}
		if (enemyhealth <= 0) break;
		if (myhealth <= 0) break;
		if (gun->rear == NULL) continue;

		printf("\n상대의 차례!\n");
		Sleep(1000);
		enemyselect = 1;
		if (enemyselect == 1) {
			printf("상대는 이쪽으로 총구를 겨눴다.\n");
			Sleep(1000);
			printf("탕!...\n");
			if (dequeueGun(gun)) {
				printf("윽! 실탄이었다..\n");
				myhealth--;
			}
			else {
				printf("휴! 공포탄이었다~\n");
			}

		}
		Sleep(500);
		printf("\n현재 나의 체력: %d 상대 체력: %d\n", myhealth, enemyhealth);
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
	actor->Cigarette = 0;
	actor->Dotbogi = 0;
	actor->Handcuffs = 0;
	actor->HP = MAX_HEALTH;
	actor->Medicine = 0;
	actor->Saw = 0;
}



