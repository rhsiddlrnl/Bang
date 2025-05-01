#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <malloc.h>
#define MAX_SIZE_GUN 6				//한 탄창 당 담을 수 있는 총 탄 수

typedef struct bullet {
	int dmg;						//총알 데미지(공포탄이면 0, 실탄이면 1, 톱아이템으로 데미지 수정가능)
	bool doIknow;					//ai구현용 상대가 공포탄인지 실탄인지 알고있는지 판단용
	struct bullet* link;
}Bullet;

typedef struct gun {
	int livenum;					//실탄 수
	int blanknum;					//공포탄 수
	int bulletnum;					//남은 총 탄 수(실탄+공포탄)
	Bullet* front;
	Bullet* rear;
}Gun;

void initGun(Gun* gun);
int is_empty(Gun* gun);
int is_full(Gun* gun);
Bullet createBullet();
void enqueueGun(Gun* gun, Bullet bullet);
int dequeueGun(Gun* gun);
void print_Gun(Gun* gun, int temp);

int main() {
	srand(time(NULL));
	Gun* gun = (Gun*)malloc(sizeof(Gun));
	initGun(gun);
	/*for (int i = 0; i < MAX_SIZE_GUN; i++) {
		enqueueGun(gun, createBullet());
	}
	print_Gun(gun);*/

	int myhealth = 5;
	int enemyhealth = 5;

	printf("게임 시작\n");
	printf("현재 나의 체력: %d 상대 체력: %d\n", myhealth, enemyhealth);
	Sleep(1000);
	while (myhealth && enemyhealth) {
		if (gun->rear == NULL) {
			printf("\n새 탄창!\n");
			Sleep(1000);
			int temp = (rand() % MAX_SIZE_GUN)+1;
			for (int i = 0; i < temp; i++) {
				enqueueGun(gun, createBullet());
			}
			print_Gun(gun, temp);
		}
		int select = 0;
		int enemyselect = 0;
		printf("나의 차례\n");
		Sleep(500);
		printf("무엇을 할 것인가?\n1.격발 2.아이템\n");
		scanf("%d", &select);
		if (select == 1) {
			printf("탕!...\n");
			Sleep(1000);
			if (dequeueGun(gun)) {
				printf("실탄이었다!\n");
				enemyhealth--;
			}
			else {
				printf("공포탄이었다...\n");
			}
		}
		else if (select == 2) {

		}
		Sleep(1000);
		printf("\n현재 나의 체력: %d 상대 체력: %d\n", myhealth, enemyhealth);
		if (enemyhealth == 0) break;
		if (myhealth == 0) break;
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
	bullet.doIknow = FALSE;
	return bullet;
}

void enqueueGun(Gun* gun, Bullet bullet)
{
	Bullet* temp = (Bullet*)malloc(sizeof(Bullet));
	temp->dmg = bullet.dmg;
	temp->doIknow = bullet.doIknow;
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



