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

int main() {
	srand(time(NULL));
	Gun* gun = (Gun*)malloc(sizeof(Gun));
	initGun(gun);
	for (int i = 0; i < MAX_SIZE_GUN; i++) {
		enqueueGun(gun, createBullet());
	}
	for (int i = 0; i < MAX_SIZE_GUN; i++) {
		printf("%d\n", gun->front->dmg);
		gun->front = gun->front->link;
	}

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



