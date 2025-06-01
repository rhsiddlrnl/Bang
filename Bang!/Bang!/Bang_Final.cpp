
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
    Bullet bullets[MAX_SIZE_GUN];
    int front;
    int rear;

} Gun;

typedef struct actor {
    char name[5];
    int HP;
    int canActorMove;
    int Cigarette;
    int Dotbogi;
    int Handcuffs;
    int Phone;
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
    gun->front = gun->rear = -1;
    gun->blanknum = gun->livenum = 0;

}

int is_empty(Gun* gun) {
    return gun->front == gun->rear;
}

int is_full(Gun* gun) {
    return gun->rear == MAX_SIZE_GUN - 1;
}

Bullet createBullet() {
    Bullet bullet;
    bullet.dmg = rand() % 2;
    bullet.doAIknow = 0;
    return bullet;
}

void enqueueGun(Gun* gun, Bullet bullet) {
    if (is_full(gun)) return;
    gun->rear++;
    gun->bullets[gun->rear] = bullet;
    if (bullet.dmg > 0) gun->livenum++;
    else gun->blanknum++;
}

Bullet dequeueGun(Gun* gun) {
    if (is_empty(gun)) exit(1);
    gun->front++;
    Bullet bullet = gun->bullets[gun->front];
    if (bullet.dmg > 0) gun->livenum--;
    else gun->blanknum--;
    return bullet;
}

Bullet peekGun(Gun* gun) {
    return gun->bullets[gun->front];
}

void initActor(Actor* actor) {
    actor->canActorMove = 1;
    actor->Cigarette = 1;
    actor->Dotbogi = 1;
    actor->Handcuffs = 1;
    actor->Phone = 1;
    actor->HP = MAX_HEALTH;
    actor->Money = 1000;
    actor->wincount = 0;
}
void Phone_item_func(Gun* gun) {
    int N = (rand() % (gun->rear - gun->front)) + gun->front + 1;
    printf("%d번째 총알은 ", gun->rear - gun->front + 1);
    printf("%s\n", gun->bullets[N].dmg == 1 ? "실탄입니다." : "공포탄입니다.");
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
            initGun(&gun);
            int temp = (rand() % MAX_SIZE_GUN) + 1;
            for (int i = 0; i < temp; i++) {
                Bullet b = createBullet();
                enqueueGun(&gun, b);
            }
            printf("[남은 총알 수: %d발 실탄: %d발 공포탄: %d발]\n", (gun.rear - gun.front), gun.livenum, gun.blanknum);
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
                Bullet hit = dequeueGun(&gun);
                if (select == 2) {
                    if (hit.dmg) {
                        printf("실탄이었다!\n");
                        enemyhealth--;
                    }
                    else {
                        printf("공포탄이었다...\n");
                    }
                    break;
                }
                else {
                    if (hit.dmg) {
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
                printf("무슨 아이템을 사용할까?\n");
                printf("현재 지닌 아이템 목록\n");
                printf("1. 담배 : %d개\n", player->Cigarette);
                printf("2. 돋보기 : %d개\n", player->Dotbogi);
                printf("3. 수갑 : %d개\n", player->Handcuffs);
                printf("4. 휴대폰 : %d\n", player->Phone);
                scanf("%d", &select);
                if ((select == 1) && (player->Cigarette > 0)) {
                    printf("뻐끔뻐끔\n");
                    Sleep(1000);
                    printf("체력을 회복했다.\n");
                    myhealth++;
                    player->Cigarette--;
                }
                else if ((select == 2) && (player->Dotbogi > 0)) {
                    printf("%s\n", peekGun(&gun).dmg == 1 ? "실탄이다." : "공포탄이다.");
                    gun.bullets[gun.front].doAIknow = 1;
                    player->Dotbogi--;
                }
                else if ((select == 3) && (player->Handcuffs > 0)) {
                    enemy.canActorMove = 0;
                    printAsciiArt("sugap.txt");

                    printf("상대의 손에 수갑을 채웠다.\n");
                    Sleep(1000);
                    player->Handcuffs--;
                }
                else if ((select == 4) && (player->Phone > 0)) {
                    Phone_item_func(&gun);
                    player->Phone--;
                }
                else {
                    printf("아이템을 가지고 있지 않는 듯 하다.");
                }
            }
            else if (select == 9) {
                enemyhealth = 0;
            }
            Sleep(500);

            printf("\n현재 나의 체력: %d 상대 체력: %d\n", myhealth, enemyhealth);
            printf("[남은 총알 수 : %d발]\n", (gun.rear - gun.front));
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
                Bullet* frontBullet = &gun.bullets[++gun.front];
                if (frontBullet->doAIknow) {
                    enemyselect = frontBullet->dmg ? 1 : 2;
                }
                else if ((enemy.Dotbogi > 0) && (gun.livenum > 0) && (gun.blanknum > 0)) {
                    enemyselect = 5;
                }
                else if ((enemy.Handcuffs > 0) && (player->canActorMove == 1) && (gun.livenum > 0)) {
                    enemyselect = 4;
                }
                else if ((enemy.Cigarette > 0) && (enemyhealth < MAX_HEALTH)) {
                    enemyselect = 3;
                }
                else {
                    enemyselect = (gun.livenum >= gun.blanknum) ? 1 : 2;
                }
            }
            if (enemyselect == 1) {
                printf("상대는 이쪽으로 총구를 겨눴다.\n탕!...\n");
                printAsciiArt("revolber.txt");
                printf("\n");
                Bullet hit = dequeueGun(&gun);
                if (hit.dmg) { printf("윽! 실탄이었다..\n"); myhealth--;  break; }
                else { printf("휴! 공포탄이었다~\n");  break; }
            }
            else if (enemyselect == 2) {
                printf("상대는 자신의 머리에 총구를 겨눴다.\n탕!...\n");
                printAsciiArt("revolber.txt");
                printf("\n");
                Bullet hit = dequeueGun(&gun);
                if (hit.dmg) { printf("ㅋㅋ\n"); enemyhealth--;  break; }
                else { printf("이런 공포탄이었다...\n"); }
            }
            else if (enemyselect == 3) {
                printAsciiArt("tabaco.txt");
                printf("상대는 담배를 피기 시작했다.\n");
                Sleep(1000);
                enemyhealth++;
                enemy.Cigarette--;
            }
            else if (enemyselect == 4) {
                printAsciiArt("sugap.txt");
                printf("상대는 내 손에 수갑을 채웠다.\n");

                Sleep(1000);
                player->canActorMove = 0;
                enemy.Handcuffs--;
            }
            else if (enemyselect == 5) {
                printAsciiArt("rens.txt");
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

        printf("\n현재 나의 체력: %d 상대 체력: %d\n", myhealth, enemyhealth);
        printf("[남은 총알 수 : %d발]\n", (gun.rear - gun.front));
    }

    if (myhealth > enemyhealth) {
        printf("승리!\n");
        if (ai == 3) player->Money += 10000;
        else if (ai == 2) { player->Money += 5000; player->wincount = 2; }
        else { player->Money += 1000; player->wincount = 1; }
    }
    else {
        printf("패배했다...\n");
    }
    return player;
}



void showInventory(Actor* actor) {
    printf("현재 지닌 아이템 목록\n");
    printf("1. 담배 : %d개\n", actor->Cigarette);
    printf("2. 돋보기 : %d개\n", actor->Dotbogi);
    printf("3. 수갑 : %d개\n", actor->Handcuffs);
    printf("4. 핸드폰: %d개\n", actor->Phone);
}

Actor* Shop(Actor* actor) {
    Sleep(500);
    system("cls");
    int select = 0;
    while (select != 6) {
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
        printf("\n");
        printf("4. 휴대폰: 2000\n");
        printf("5. 현재 지닌 물건 확인하기.\n");
        printf("6. 구매 종료\n");
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
            if (actor->Money >= 2000) { actor->Money -= 2000; actor->Phone++; printf("휴대폰을 구매했다.\n"); }
            break;
        case 5:
            showInventory(actor);
            break;
        case 6:
            printf("구매할게 없는 것 같다.\n");
            break;
        default:
            printf("다시 선택하세요.");
            break;
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
            printf("난이도를 선택하세요.\n1.쉬움 2. 보통 3.어려움\n");
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
