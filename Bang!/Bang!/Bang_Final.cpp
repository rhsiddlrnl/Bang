
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
        printf("ASCII ��Ʈ ������ �� �� �����ϴ�.\n");
        return;
    }

    char line[4896]; // �� �پ� �б� ���� ����
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
    printf("%d��° �Ѿ��� ", gun->rear - gun->front + 1);
    printf("%s\n", gun->bullets[N].dmg == 1 ? "��ź�Դϴ�." : "����ź�Դϴ�.");
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
    printf("%d�ܰ�\n", ai);

    printf("���� ����\n");
    printf("���� ���� ü��: %d ��� ü��: %d\n", myhealth, enemyhealth);
    int select = 0;
    int enemyselect = 0;
    Sleep(1000);

    while ((myhealth > 0) && (enemyhealth > 0)) {
        if (is_empty(&gun)) {
            printf("\n�� źâ!\n");
            Sleep(1000);
            int temp = (rand() % MAX_SIZE_GUN) + 1;
            for (int i = 0; i < temp; i++) {
                Bullet b = createBullet();
                enqueueGun(&gun, b);
            }
            printf("[���� �Ѿ� ��: %d�� ��ź: %d�� ����ź: %d��]\n", (gun.rear - gun.front), gun.livenum, gun.blanknum);
        }

        printf("���� ����\n");
        while (player->canActorMove) {
            Sleep(500);
            printf("������ �� ���ΰ�?\n1.�ݹ� 2.������\n");
            Sleep(500);
            scanf("%d", &select);
            if (select == 1) {
                printf("���� �� �� �ΰ�?\n1.�� �ڽ� 2. ���\n");
                Sleep(500);
                scanf("%d", &select);
                printf("��!...\n");
                printAsciiArt("revolber.txt");
                printf("\n");
                Sleep(1000);
                Bullet hit = dequeueGun(&gun);
                if (select == 2) {
                    if (hit.dmg) {
                        printf("��ź�̾���!\n");
                        enemyhealth--;
                    }
                    else {
                        printf("����ź�̾���...\n");
                    }
                    break;
                }
                else {
                    if (hit.dmg) {
                        printf("��! ��ź�̾���..\n");
                        myhealth--;
                        break;
                    }
                    else {
                        printf("��! ����ź�̾���~\n");
                    }
                }
            }
            else if (select == 2) {
                printf("���� �������� ����ұ�?\n");
                printf("���� ���� ������ ���\n");
                printf("1. ��� : %d��\n", player->Cigarette);
                printf("2. ������ : %d��\n", player->Dotbogi);
                printf("3. ���� : %d��\n", player->Handcuffs);
                printf("4. �޴��� : %d\n", player->Phone);
                scanf("%d", &select);
                if ((select == 1) && (player->Cigarette > 0)) {
                    printf("��������\n");
                    Sleep(1000);
                    printf("ü���� ȸ���ߴ�.\n");
                    myhealth++;
                    player->Cigarette--;
                }
                else if ((select == 2) && (player->Dotbogi > 0)) {
                    printf("%s\n", peekGun(&gun).dmg == 1 ? "��ź�̴�." : "����ź�̴�.");
                    gun.bullets[gun.front].doAIknow = 1;
                    player->Dotbogi--;
                }
                else if ((select == 3) && (player->Handcuffs > 0)) {
                    enemy.canActorMove = 0;
                    printAsciiArt("sugap.txt");

                    printf("����� �տ� ������ ä����.\n");
                    Sleep(1000);
                    player->Handcuffs--;
                }
                else if ((select == 4) && (player->Phone > 0)) {
                    Phone_item_func(&gun);
                    player->Phone--;
                }
                else {
                    printf("�������� ������ ���� �ʴ� �� �ϴ�.");
                }
            }
            else if (select == 9) {
                enemyhealth = 0;
            }
            Sleep(500);

            printf("\n���� ���� ü��: %d ��� ü��: %d\n", myhealth, enemyhealth);
            printf("[���� �Ѿ� �� : %d��]\n", (gun.rear - gun.front));
            if (enemyhealth == 0 || myhealth == 0 || is_empty(&gun)) break;
        }
        if (!player->canActorMove) {
            printf("������ Ǯ����� ���� ���� ���ߴ�.\n");
            player->canActorMove = 1;
        }
        if (enemyhealth <= 0 || myhealth <= 0 || is_empty(&gun)) continue;

        printf("\n����� ����!\n");
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
                printf("���� �������� �ѱ��� �ܴ���.\n��!...\n");
                printAsciiArt("revolber.txt");
                printf("\n");
                Bullet hit = dequeueGun(&gun);
                if (hit.dmg) { printf("��! ��ź�̾���..\n"); myhealth--;  break; }
                else { printf("��! ����ź�̾���~\n");  break; }
            }
            else if (enemyselect == 2) {
                printf("���� �ڽ��� �Ӹ��� �ѱ��� �ܴ���.\n��!...\n");
                printAsciiArt("revolber.txt");
                printf("\n");
                Bullet hit = dequeueGun(&gun);
                if (hit.dmg) { printf("����\n"); enemyhealth--;  break; }
                else { printf("�̷� ����ź�̾���...\n"); }
            }
            else if (enemyselect == 3) {
                printAsciiArt("tabaco.txt");
                printf("���� ��踦 �Ǳ� �����ߴ�.\n");
                Sleep(1000);
                enemyhealth++;
                enemy.Cigarette--;
            }
            else if (enemyselect == 4) {
                printAsciiArt("sugap.txt");
                printf("���� �� �տ� ������ ä����.\n");

                Sleep(1000);
                player->canActorMove = 0;
                enemy.Handcuffs--;
            }
            else if (enemyselect == 5) {
                printAsciiArt("rens.txt");
                printf("���� ������� ���� ���Ǳ� �����ߴ�.\n");
                Sleep(1000);
                gun.bullets[gun.front].doAIknow = 1;
                enemy.Dotbogi--;
            }
            Sleep(500);
            if (enemyhealth == 0 || myhealth == 0 || is_empty(&gun)) break;
        }
        if (!enemy.canActorMove) {
            printf("���� ������ Ǯ����� ���� ���� ���ߴ�.\n");
            enemy.canActorMove = 1;
        }

        printf("\n���� ���� ü��: %d ��� ü��: %d\n", myhealth, enemyhealth);
        printf("[���� �Ѿ� �� : %d��]\n", (gun.rear - gun.front));
    }

    if (myhealth > enemyhealth) {
        printf("�¸�!\n");
        if (ai == 3) player->Money += 10000;
        else if (ai == 2) { player->Money += 5000; player->wincount = 2; }
        else { player->Money += 1000; player->wincount = 1; }
    }
    else {
        printf("�й��ߴ�...\n");
    }
    return player;
}



void showInventory(Actor* actor) {
    printf("���� ���� ������ ���\n");
    printf("1. ��� : %d��\n", actor->Cigarette);
    printf("2. ������ : %d��\n", actor->Dotbogi);
    printf("3. ���� : %d��\n", actor->Handcuffs);
    printf("4. �ڵ���: %d��\n", actor->Phone);
}

Actor* Shop(Actor* actor) {
    Sleep(500);
    system("cls");
    int select = 0;
    while (select != 6) {
        printf("���� ���� �ݾ�: %d��\n", actor->Money);
        printf("\n");
        printAsciiArt("tabaco.txt");
        printf("\n");
        printf("1. ��� : 500��\n");
        printAsciiArt("rens.txt");
        printf("\n");
        printf("2. ������ : 1000��\n");
        printAsciiArt("sugap.txt");
        printf("\n");
        printf("3. ���� : 1500��\n");
        printf("\n");
        printf("4. �޴���: 2000\n");
        printf("5. ���� ���� ���� Ȯ���ϱ�.\n");
        printf("6. ���� ����\n");
        scanf("%d", &select);
        switch (select) {
        case 1:
            if (actor->Money >= 500) { actor->Money -= 500; actor->Cigarette++; printf("��踦 �����ߴ�.\n"); }
            else { printf("���� �����ϴ�..."); }
            break;
        case 2:
            if (actor->Money >= 1000) { actor->Money -= 1000; actor->Dotbogi++; printf("�����⸦ �����ߴ�.\n"); }
            else { printf("���� �����ϴ�..."); }
            break;
        case 3:
            if (actor->Money >= 1500) { actor->Money -= 1500; actor->Handcuffs++; printf("�����⸦ �����ߴ�.\n"); }
            else { printf("���� �����ϴ�...\n"); }
            break;
        case 4:
            if (actor->Money >= 2000) { actor->Money -= 2000; actor->Phone++; printf("�޴����� �����ߴ�.\n"); }
            break;
        case 5:
            showInventory(actor);
            break;
        case 6:
            printf("�����Ұ� ���� �� ����.\n");
            break;
        default:
            printf("�ٽ� �����ϼ���.");
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
        printf("������ �ұ�?\n");
        printf("1. ��뿡�� �����Ѵ�.\n");
        printf("2. ������ �湮�Ѵ�.\n");
        printf("3. ����\n");
        scanf("%d", &select);
        if (select == 1) {
            printf("���̵��� �����ϼ���.\n1.���� 2. ���� 3.�����\n");
            scanf("%d", &select);
            if (select == 1) {
                Game(&player, 1);
            }
            else if (select == 2) {
                if (player.wincount < 1) {
                    printf("���� �� ��뿡�� �������� ���� �� ����.\n");
                    Sleep(500);
                    system("cls");
                    continue;
                }
                Game(&player, 2);
            }
            else if (select == 3) {
                if (player.wincount < 2) {
                    printf("���� �� ��뿡�� �������� ���� �� ����.\n");
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
