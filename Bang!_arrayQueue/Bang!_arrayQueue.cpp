#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>


#define MAX_SIZE_GUN 6
#define MAX_HEALTH 5

typedef struct bullet {
    int dmg;
    int doAIknow;
} Bullet;

typedef struct gun {
    int livenum;
    int blanknum;
    int bulletnum;
    Bullet bullets[MAX_SIZE_GUN];
    int front;
    int rear;
    int count;
} Gun;

typedef struct actor {
    char name[5];
    int HP;
    int canActorMove;
    int Cigarette;
    int Dotbogi;
    int Handcuffs;
    int Money;
    int wincount;
} Actor;

void printAsciiArt(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("ASCII 아트 파일을 열 수 없습니다.\n");
        return;
    }

    char line[4896]; // 한 줄씩 읽기 위한 버퍼
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

void initGun(Gun* gun) {
    gun->front = gun->rear = gun->count = 0;
    gun->livenum = gun->blanknum = gun->bulletnum = 0;
}

int is_empty(Gun* gun) {
    return gun->count == 0;
}

int is_full(Gun* gun) {
    return gun->count == MAX_SIZE_GUN;
}
void initActor(Actor* actor);

Bullet createBullet() {
    Bullet bullet;
    bullet.dmg = rand() % 2;
    bullet.doAIknow = 0;
    return bullet;
}

void enqueueGun(Gun* gun, Bullet bullet) {
    if (is_full(gun)) return;
    gun->bullets[gun->rear] = bullet;
    gun->rear = (gun->rear + 1) % MAX_SIZE_GUN;
    gun->count++;
}

int dequeueGun(Gun* gun) {
    if (is_empty(gun)) exit(1);
    int dmg = gun->bullets[gun->front].dmg;
    gun->front = (gun->front + 1) % MAX_SIZE_GUN;
    gun->count--;
    return dmg;
}

int peakGun(Gun* gun) {
    return gun->bullets[gun->front].dmg;
}

Actor* Game(Actor* player, int ai) {
    Sleep(500);
    system("cls");
    Gun gun;
    initGun(&gun);
    int myhealth = player->HP;

    Actor enemy;
    initActor(&enemy);
    if (ai == 3) {
        enemy.Cigarette = 2;
        enemy.Dotbogi = 2;
        enemy.Handcuffs = 2;
    }

    int enemyhealth = enemy.HP;
    printf("%d단계\n", ai);

    printf("게임 시작\n");
    printf("현재 나의 체력: %d 상대 체력: %d\n", myhealth, enemyhealth);
    int select = 0;
    int enemyselect = 0;
    Sleep(1000);

    while ((myhealth > 0) && (enemyhealth > 0)) {
        if (is_empty(&gun)) {
            printf("\n새 탄창!\n");
            Sleep(1000);
            int temp = (rand() % MAX_SIZE_GUN) + 1;
            int livesum = 0;
            for (int i = 0; i < temp; i++) {
                Bullet b = createBullet();
                enqueueGun(&gun, b);
                if (b.dmg) livesum++;
            }
            gun.bulletnum = temp;
            gun.livenum = livesum;
            gun.blanknum = temp - livesum;
            printf("[남은 총알 수: %d발 실탄: %d발 공포탄: %d발]\n", gun.bulletnum, gun.livenum, gun.blanknum);
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
                printf("탕!...\n");
                printAsciiArt("revolber.txt");
                printf("\n");
                Sleep(1000);
                int hit = dequeueGun(&gun);
                if (select == 2) {
                    if (hit) {
                        printf("실탄이었다!\n");
                        enemyhealth--;
                        gun.livenum--;
                    } else {
                        printf("공포탄이었다...\n");
                        gun.blanknum--;
                    }
                    break;
                } else {
                    if (hit) {
                        printf("윽! 실탄이었다..\n");
                        myhealth--;
                        gun.livenum--;
                        break;
                    } else {
                        printf("휴! 공포탄이었다~\n");
                        gun.blanknum--;
                    }
                }
            } else if (select == 2) {
                printf("무슨 아이템을 사용할까?\n");
                printf("현재 지닌 아이템 목록\n");
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
                } else if ((select == 2) && (player->Dotbogi > 0)) {
                    printf("%s\n", peakGun(&gun) == 1 ? "실탄이다." : "공포탄이다.");
                    gun.bullets[gun.front].doAIknow = 1;
                    player->Dotbogi--;
                } else if ((select == 3) && (player->Handcuffs > 0)) {
                    enemy.canActorMove = 0;
                    printf("상대의 손에 수갑을 채웠다.\n");
                    player->Handcuffs--;
                } else {
                    printf("아이템을 가지고 있지 않는 듯 하다.");
                }
            } else if (select == 9) {
                enemyhealth = 0;
            }
            Sleep(500);
            gun.bulletnum = gun.livenum + gun.blanknum;
            printf("\n현재 나의 체력: %d 상대 체력: %d\n", myhealth, enemyhealth);
            printf("[남은 총알 수 : %d발]\n", gun.bulletnum);
            if (enemyhealth == 0 || myhealth == 0 || is_empty(&gun)) break;
        }
        if (!player->canActorMove) {
            printf("수갑을 풀어내느라 총을 쏘지 못했다.\n");
            player->canActorMove = 1;
        }
        if (enemyhealth <= 0 || myhealth <= 0 || is_empty(&gun)) continue;

        printf("\n상대의 차례!\n");
        while (enemy.canActorMove) {
            Sleep(1000);
            if (ai == 1) enemyselect = 1;
            else if (ai == 2) enemyselect = (gun.livenum >= gun.blanknum) ? 1 : 2;
            else {
                Bullet* frontBullet = &gun.bullets[gun.front];
                if (frontBullet->doAIknow) {
                    enemyselect = frontBullet->dmg ? 1 : 2;
                } else if ((enemy.Dotbogi > 0) && (gun.livenum > 0) && (gun.blanknum > 0)) {
                    enemyselect = 5;
                } else if ((enemy.Handcuffs > 0) && (player->canActorMove == 1) && (gun.livenum > 0)) {
                    enemyselect = 4;
                } else if ((enemy.Cigarette > 0) && (enemyhealth < MAX_HEALTH)) {
                    enemyselect = 3;
                } else {
                    enemyselect = (gun.livenum >= gun.blanknum) ? 1 : 2;
                }
            }
            if (enemyselect == 1) {
                printAsciiArt("revolber.txt");
                printf("상대는 이쪽으로 총구를 겨눴다.\n탕!...\n");
                int hit = dequeueGun(&gun);
                if (hit) { printf("윽! 실탄이었다..\n"); myhealth--; gun.livenum--; break; }
                else { printf("휴! 공포탄이었다~\n"); gun.blanknum--; break; }
            } else if (enemyselect == 2) {
                printAsciiArt("revolber.txt");
                printf("상대는 자신의 머리에 총구를 겨눴다.\n탕!...\n");
                int hit = dequeueGun(&gun);
                if (hit) { printf("ㅋㅋ\n"); enemyhealth--; gun.livenum--; break; }
                else { printf("이런 공포탄이었다...\n"); gun.blanknum--; }
            } else if (enemyselect == 3) {
                printf("상대는 담배를 피기 시작했다.\n");
                enemyhealth++;
                enemy.Cigarette--;
            } else if (enemyselect == 4) {
                printf("상대는 내 손에 수갑을 채웠다.\n");
                Sleep(1000);
                player->canActorMove = 0;
                enemy.Handcuffs--;
            } else if (enemyselect == 5) {
                printf("상대는 돋보기로 총을 살피기 시작했다.\n");
                Sleep(1000);
                gun.bullets[gun.front].doAIknow = 1;
                enemy.Dotbogi--;
            }
            Sleep(500);
            if (enemyhealth == 0 || myhealth == 0 || is_empty(&gun)) break;
        }
        if (!enemy.canActorMove) {
            printf("상대는 수갑을 풀어내느라 총을 쏘지 못했다.\n");
            enemy.canActorMove = 1;
        }
        gun.bulletnum = gun.livenum + gun.blanknum;
        printf("\n현재 나의 체력: %d 상대 체력: %d\n", myhealth, enemyhealth);
        printf("[남은 총알 수 : %d발]\n", gun.bulletnum);
    }

    if (myhealth > enemyhealth) {
        printf("승리!\n");
        if (ai == 3) player->Money += 10000;
        else if (ai == 2) { player->Money += 5000; player->wincount = 2; }
        else { player->Money += 1000; player->wincount = 1; }
    } else {
        printf("패배했다...\n");
    }
    return player;
}

void initActor(Actor* actor) {
    actor->canActorMove = 1;
    actor->Cigarette = 1;
    actor->Dotbogi = 1;
    actor->Handcuffs = 1;
    actor->HP = MAX_HEALTH;
    actor->Money = 1000;
    actor->wincount = 0;
}

void showInventory(Actor* actor) {
    printf("현재 지닌 아이템 목록\n");
    printf("1. 담배 : %d개\n", actor->Cigarette);
    printf("2. 돋보기 : %d개\n", actor->Dotbogi);
    printf("3. 수갑 : %d개\n", actor->Handcuffs);
}

Actor* Shop(Actor* actor) {
    Sleep(500);
    system("cls");
    int select = 0;
    while (select != 5){
        printf("현재 소지 금액: %d원\n", actor->Money);
        printf("\n");
        printAsciiArt("tabaco.txt");
        printf("\n");
        printf("1. 담배 : 500원\n");
        printAsciiArt("rens.txt");
        printf("\n");
        printf("2. 돋보기 : 1000원\n");
        printAsciiArt("sugap.txt");
        printf("\n");
        printf("3. 수갑 : 1500원\n");
        printf("4. 현재 지닌 물건 확인하기.\n");
        printf("5. 구매 종료\n");
        scanf("%d", &select);
        switch (select) {
        case 1:
            if (actor->Money >= 500) { actor->Money -= 500; actor->Cigarette++; printf("담배를 구매했다.\n"); }
            else { printf("돈이 부족하다..."); }
            break;
        case 2:
            if (actor->Money >= 1000) { actor->Money -= 1000; actor->Dotbogi++; printf("돋보기를 구매했다.\n"); }
            else { printf("돈이 부족하다..."); }
            break;
        case 3:
            if (actor->Money >= 1500) { actor->Money -= 1500; actor->Handcuffs++; printf("돋보기를 구매했다.\n"); }
            else { printf("돈이 부족하다...\n"); }
            break;
        case 4:
            showInventory(actor);
            break;
        default: break;
        }
        Sleep(500);
        system("cls");
    }
    return actor;
}

int main() {
    srand(time(NULL));
    Actor player;
    initActor(&player);
    int select = 0;
    while (select != 3) {
        printf("무엇을 할까?\n");
        printf("1. 상대에게 도전한다.\n");
        printf("2. 상점에 방문한다.\n");
        printf("3. 종료\n");
        scanf("%d", &select);
        if (select == 1) {
            printf("난이도를 선택하세용가리치킨\n1.쉬움 2. 보통 3.어려움\n");
            scanf("%d", &select);
            if (select == 1) {
                Game(&player, 1);
            }
            else if (select == 2) {
                if (player.wincount < 1) {
                    printf("아직 이 상대에게 도전하지 못할 것 같다.\n");
                    Sleep(500);
                    system("cls");
                    continue;
                }
                Game(&player, 2);
            }
            else if (select == 3) {
                if (player.wincount < 2) {
                    printf("아직 이 상대에게 도전하지 못할 것 같다.\n");
                    Sleep(500);
                    system("cls");
                    select = 0;
                    continue;
                }
                Game(&player, 3);
                select = 0;
            }
        }
        else if (select == 2) {
            Shop(&player);
        }
    }
    return 0;
}
