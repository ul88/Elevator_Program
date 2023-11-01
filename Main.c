//�������� ������ ��. ������ ���� ������ �� �׻� �����ϰ� ��.
//����ڰ� ���������Ϳ� Ż ��쿡�� �ִ� ���뷮�� �Ѱ� ���� ���� ���� �� ����
//ž�°��� 0���� �� ������ ���������ʹ� �� �������θ� �����
#include<stdio.h> //ǥ�� ����� ��� ����
#include<time.h> //���� ���� �õ带 ���� ��� ����
#include<math.h> //���� �Լ��� ���� ��� ����
#include<stdlib.h> //Sleep �Լ��� ���� �Լ��� ���� ��� ����
#include<windows.h> //system �Լ��� ����ϱ� ���� ��� ����
#include<conio.h> //Ű���� �Է��� ���� ��� ����
#define UP 72 //���� ����Ű�� �ƽ�Ű �ڵ�� 224 72 �̹Ƿ� 72�� ��ó��
#define DOWN 80 //�Ʒ��� ����Ű�� �ƽ�Ű �ڵ�� 224 80 �̹Ƿ� 80�� ��ó��
#define RIGHT 77 //������ ����Ű�� �ƽ�Ű �ڵ�� 224 77 �̹Ƿ� 77�� ��ó��
#define LEFT 75 //���� ����Ű�� �ƽ�Ű �ڵ�� 224 75 �̹Ƿ� 75�� ��ó��
#define ESC 27 //ESC�� �ƽ�Ű �ڵ�� 27�̹Ƿ� 27�� ��ó��
#define F1 59 //F1�� �ƽ�Ű �ڵ�� 0 59�̹Ƿ� 59�� ��ó��
#define F2 60 //F2�� �ƽ�Ű �ڵ�� 0 60�̹Ƿ� 60�� ��ó��
#define F3 61 //F3�� �ƽ�Ű �ڵ�� 0 61�̹Ƿ� 61�� ��ó��
#define TOTAL_FLOOR 110 //���� 100�� + ���� 10�� = 110 �̹Ƿ� 110���� ����
#define MAX_FLOOR 100 //������ �� 100������ �����Ƿ� 100�� �ִ밪���� ����
#define MIN_FLOOR -10//���ϴ� �� 10������ �����Ƿ� -10�� �ּҰ����� ����
#define MAX_KG 1450 //�ִ� ž�� �ο��� 1450kg�̹Ƿ� 1450���� ����
#define MAN_KG 75 //������ �����Դ� 75kg�̹Ƿ� 75�� ����
#define WOMAN_KG 55 //������ �����Դ� 55kg�̹Ƿ� 55�� ����

typedef struct infoHuman{ //���������� �� ������ �������ϴ� ����� ���� ��Ÿ��
	int man; // ������ ��
	int woman; // ������ ��
}info_t;

typedef struct elevator { //�迭�����δ� �ʿ��� �����͸� �� �����ϱ⿡�� �����ϹǷ� ����ü ����
	int floor; //�� ��
	info_t gender; // �ش� ������������ ������ ���� ������ ��
	int dir; //���� (��: 1 , �Ʒ�: 0 , ����: -1)
	int inspect; //���������� ���� ���θ� ��Ÿ�� (���� ��: 1, ���� ��: 0)
	info_t info[TOTAL_FLOOR+1]; // ���� ����� ��ġ�� �����ϱ� ���� ������ �������� �迭�� �߰��� ����
}elevator_t;


void init(elevator_t* elva) { //�ʱ�ȭ
	for (int i = 0; i < 6; i++) {
		int randomF = rand() % 110; // �� ���� �������� �ʱ�ȭ
		int randomD = rand() % 2; // ������ �������� �ʱ�ȭ
		int randomM = rand() % (MAX_KG/MAN_KG); // ���� ���� �������� �ʱ�ȭ
		int randomW = rand() % ((MAX_KG - randomM*MAN_KG)/WOMAN_KG); // ���� ���� �������� �ʱ�ȭ
		elva[i].floor = randomF - 10; // ������ ����, ����� ����
		if (elva[i].floor >= 0) { // 0�� ���� ���̱� ������ 0�� ����� ���� ����
			elva[i].floor++; // 0~109������ ���� 1�� ���� floor���� -10 ~ -1 && 1 ~ 110���� �� ����
		}
		elva[i].dir = randomD; //������ dir�� ����
		elva[i].gender.man = randomM; // ������ ���� ����
		elva[i].gender.woman = randomW; // ������ ���� ����
		elva[i].inspect = 0; // ���� ����
		
		int nowMan = elva[i].gender.man;
		int nowWoman = elva[i].gender.woman;
		while (nowMan != 0 || nowWoman != 0) {
			int nextFloor = rand() % 111;
			if (nextFloor == 10) {
				continue;
			}
			int nextMan = rand() % (nowMan + 1);
			int nextWoman = rand() % (nowWoman + 1);
			elva[i].info[nextFloor].man += nextMan;
			elva[i].info[nextFloor].woman += nextWoman;
			nowMan -= nextMan;
			nowWoman -= nextWoman;
		}
	}
}

void userPrint(elevator_t* elva, int elvaIdx,int upButton, int downButton, int nowFloor) { //���������� ���
	printf("���� ����Ű�� ������ �������� ���ϴ� ���������͸� ȣ���� �� �ֽ��ϴ�.\n");
	printf("�Ʒ��� ����Ű�� ������ �������� ���ϴ� ���������͸� ȣ���� �� �ֽ��ϴ�.\n");
	printf("ESC�� ������ ���α׷��� ������ �� �ֽ��ϴ�.\n");
	printf("F1�� ������ ������ ���ɴϴ�.\n");
	//��ø�ݺ����� ��������� 2���� �迭�� �ƴ� 1���� �迭�̱� ������ [j + i * 3]�� ����ߴ�.
	for (int i = 0; i < 2; i++) { // ������������ ��ġ�� 2�� 3���̱� ������ �ι� �ݺ�

		//������������ ��ȣ
		for (int j = 0; j < 3; j++) { // ������������ ��ġ�� 2�� 3���̱� ������ ���� �ݺ�
			printf("==== %d ====", (j + 1) + (i * 3)); 
			if (elvaIdx == j + (i * 3)) { // ����ڿ��Է� ���� ��������������
										// ����ڿ��� ���� ���������Ͷ�� ��ĥ�� �� �ƴ϶�� �� ������ ��Ÿ����.
				printf("��\t");
			}
			else {
				printf("��\t");
			}
		}
		printf("\n");

		//���� ���� �Ǵ� ���� ����
		//������ ���� ���̶�� �����̾�� �� �ʿ䰡 �����Ƿ� �� �������� ó��
		for (int j = 0; j < 3; j++) { // ������������ ��ġ�� 2�� 3���̱� ������ ���� �ݺ�
			if (elva[j + (i * 3)].inspect == 1) { //���� ���� Ȯ��
				printf("   ������\t");
			}
			else if (elva[j + (i * 3)].gender.man * MAN_KG + 
					elva[j + (i * 3)].gender.woman * WOMAN_KG >= MAX_KG - WOMAN_KG) { // ���� ���� Ȯ��
				printf("   �� ��\t");
			}
			else {
				printf("\t\t");
			}
		}
		printf("\n");

		//����
		for (int j = 0; j < 3; j++) { // ������������ ��ġ�� 2�� 3���̱� ������ ���� �ݺ�
			if (elva[j + (i * 3)].floor >= 0) { // �������̸� -�� �ƴ� B�� ������ �ϱ� ���� ����
				printf("   -%3d-   ", elva[j + (i * 3)].floor);
			}
			else {
				printf("   -B%2d-   ", elva[j + (i * 3)].floor * -1);
			}
			if (elva[j + (i * 3)].dir == -1) {
				printf("��\t");
			}
			else if (elva[j + (i * 3)].dir == 1) { // ���� ���������Ͱ� �ö󰡴� ��
				printf("��\t");
			}
			else {
				printf("��\t");
			}
		}
		printf("\n");

		//������ ��
		for (int j = 0; j < 3; j++) { // ������������ ��ġ�� 2�� 3���̱� ������ ���� �ݺ�
			printf("���� : %2d  ", elva[j + (i * 3)].gender.man);
			if (elva[j + (i * 3)].dir == -1) {
				printf("��\t");
			}
			else if (elva[j + (i * 3)].dir == 1) { // ���� ���������Ͱ� �������� ��
				printf("��\t");
			}
			else {
				printf("��\t");
			}
		}
		printf("\n");

		//������ ��
		for (int j = 0; j < 3; j++) { // ������������ ��ġ�� 2�� 3���̱� ������ ���� �ݺ�
			printf("���� : %2d \t", elva[j + (i * 3)].gender.woman);
		}
		printf("\n");

		//�� ų��
		for (int j = 0; j < 3; j++) { // ������������ ��ġ�� 2�� 3���̱� ������ ���� �ݺ�
			int kg = elva[j + (i * 3)].gender.man * MAN_KG + elva[j + (i * 3)].gender.woman * WOMAN_KG; // kg ���
			printf("  %4dkg \t", kg);
		}

		printf("\n============    ============    ============");
		
		// �߰��� ���� ������ ��� ��ư�� ������ ���� ��Ÿ����
		if (i == 0) { // ù���� ������ ������ �Ǿ� �ϱ� ����
			printf("\n\n");
			if (nowFloor > 0) {
				printf("\t���� ���� : %3d", nowFloor);
			}
			else {
				printf("\t���� ���� : B%2d", nowFloor*-1);
			}
			if (upButton) {
				printf("    ��\n\t\t");
			}
			else {
				printf("    ��\n\t\t");
			}
			if (downButton) {
				printf("           ��");
			}
			else {
				printf("           ��");
			}
			printf("\n\n");
		}
	}

}

int strLen(char* str) { // ���ڿ� ���� Ȯ�� �Լ�
	int len = -1; // ���� �����ڸ� ����߱� ������ 0�� �ƴ� -1�� �ʱ�ȭ
	while (str[++len] != '\0'); // NULL���ڸ� ���� ������ len�� ���� 1�� �����ϸ鼭 Ȯ��
	return len; //len ����
}  

int checkStr(char* str, char* comStr) { //���ڿ� �� �Լ�
	if (strLen(str) != strLen(comStr)) return 0; // ���ڿ��� ���̰� ���� �ٸ��ٸ� ���� �ʿ䰡 �����Ƿ� �ٷ� 0�� ����
	for (int i = 0; i < strLen(str); i++) { // ���� ���ڿ��� ���̸�ŭ �ݺ���
		if (str[i] != comStr[i]) { // ���� �ٸ��ٸ� �ٷ� 0�� ����
			return 0;
		}
	}
	return 1; //�ݺ����� ����� �����ٸ� ���� ���ٴ� �Ҹ��̹Ƿ� 1�� ����
}

void changeStr(char* str, char* comStr) { // ���ڿ� ��ȯ �Լ�
	for (int i = 0; i <= strLen(str); i++) { // str�� ���̸�ŭ �ݺ�
		comStr[i] = str[i]; // comStr�� str������ ����
	}
}

void password(int* admin, char* correctPw) { // ��й�ȣ �Է� �Լ�
	int pwCnt = 5; 
	char pw[100] = "";
	char changePw[10] = "change";
	int flag = 0;
	printf("������ ����Դϴ�.\n");
	printf("��й�ȣ�� �Է����ּ���. (8�ڸ��̻�, 20�ڸ����ϸ� �����մϴ�.)\n");
	printf("��й�ȣ ������ ���Ѵٸ� \"%s\"�� �Է����ּ���.\n", changePw);
	scanf_s("%s", pw, 50);

	if (checkStr(pw, changePw)) {
		printf("��й�ȣ�� �����մϴ�.\n");
		printf("������ ��й�ȣ�� �Է����ּ���.\n");
		scanf_s("%s", pw, 50);

		if (checkStr(pw, correctPw)) {
			printf("�����ϰ� ���� ��й�ȣ�� �Է����ּ���. (8�ڸ��̻�, 20�ڸ����ϸ� �����մϴ�.)\n");
			scanf_s("%s", pw, 50);
			if (strLen(pw) >= 8 && strLen(pw) <= 20) {
				changeStr(pw, correctPw);
				printf("%s�� ��й�ȣ�� �����մϴ�.\n", correctPw);
			}
			else {
				printf("8�ڸ��̻�, 20�ڸ� ������ ��й�ȣ�� �Է����ּ���.\n");
			}
		}
		else {
			printf("��й�ȣ�� Ʋ���ϴ�.\n");
		}

		printf("1�� �� ������ ��尡 ����۵˴ϴ�.\n");
		Sleep(1000);
		return;
	}

	while (!checkStr(pw, correctPw) && pwCnt > 0 && strLen(pw) != 1) {
		system("cls");
		printf("��й�ȣ�� Ʋ�Ƚ��ϴ�.\n");
		printf("- %d�� ���ҽ��ϴ�. -\n", pwCnt);
		printf("�ٽ� �Է����ּ���. (�ϳ��� �Է��ϸ� ������ ��尡 �ٷ� ����˴ϴ�.)\n");
		scanf_s("%s", pw, 50);
		pwCnt--;
	}
	if (!checkStr(pw, correctPw)) {
		*admin = 0;
		printf("1�� �� ������ ��带 �����մϴ�.\n");
		Sleep(1000);
		return;
	}
	else {
		*admin = 2;
		printf("1�� �� ������ ��带 �����մϴ�.\n");
		Sleep(1000);
	}
}

void adminPrint(elevator_t* elva, int *admin, char *correctPw, int *inspectCnt) { //������ ��� �Լ�
	if (*admin == 1) {
		password(admin, correctPw); // ��й�ȣ �Է� �Լ�
	}
	else {
		//�ش� ���������Ϳ��� ��� ���� ���� ������ ��
		//�ش� ���������Ͱ� � ���� �湮���� �� �� ���� ����� ž���ϴ� ��
		//���������� ������ ��ų �� �־���� (2�����)
		//for (int i = 0; i < 6; i++) {
		//	int nowF = elva[i].floor + 10; //-10~100���� 0�� ���� ������ 0~99 ������ 100~109
		//	int nowM = elva[i].info[nowF].man;
		//	int nowW = elva[i].info[nowF].woman;
		//	
		//	printf("%d�� ����������\n", i + 1);
		//	printf("�� : %d\n", elva[i].floor);
		//	printf("�������ϴ� ���\n");
		//	printf("���� : %d\n", nowM);
		//	printf("���� : %d\n\n", nowW);

		//}
		for (int i = 0; i < 110; i++) {
			printf("%d�� : %d %d\n",i, elva[2].info[i].man, elva[2].info[i].woman);
		}
	}
}

void enter(int *loop,int *gender) { // ���� ���θ� �Ǻ��ϱ� ���� �Լ�
	printf("������ ��� �ǳ���?(����: 1 , ����: 0)\n");
	scanf_s("%d", gender);
	printf("���������Ϳ� �����Ͻðڽ��ϱ�?\n");
	printf("������ �Ͻ� �Ŀ��� ESC�� ������ ������ ������ �� �ֽ��ϴ�.\n");
	printf("������ ����Ѵٸ� 1, ������ ������� �ʴ´ٸ� 0�� �Է����ּ���.\n");
	scanf_s("%d", loop);
}

void helpMessage() { // ���� ��� �Լ�
	int flag = 0;
	system("cls");
	printf("���������͸� �θ��� ����� ���� ����Ű �Ǵ� �Ʒ��� ����Ű�� ������ �˴ϴ�.\n");
	printf("ESC�� ������ ���α׷��� ����˴ϴ�.\n");
	printf("F2�� ������ ������ ��带 ������ �� �ֽ��ϴ�.\n");
	printf("F2�� �ٽ� ������ ������ ��尡 ����˴ϴ�.\n");
	printf("������ �����Ͻðڽ��ϱ�? (��: 1 , �ƴϿ�: 0)\n");
	scanf_s("%d", &flag);
	if (flag == 1) { // ������ �����ϱ�� �ߴٸ�
		return; // ����
	}
	else {
		helpMessage(); // 1�� �Է��� ������ ���
	}
}

int checkSpeed(elevator_t *elva,int start,int upButton, int downButton) { // ���� ���� ������ �� �ִ� ���������͸� ã�� �� ���������� ��ȣ�� ��ȯ�ϱ� ���� int�� �Լ��� ����
	int min = TOTAL_FLOOR + 1; // ���� ������ ���������Ϳ� ���� �������� ���� ����
	// �ּҰ��� ������ ������ �ִ� �������� Ŀ���ϱ� ������ (�ִ� ���� + 1)�� ����
	int minIdx= -1; // ���� ������ ���������� ��ȣ�� ������ ����
	int minDir = 1;
	for (int i = 0; i < 6; i++) {
		if (elva[i].dir == -1) {
			if (min > abs(elva[i].floor - start)) {
				min = abs(elva[i].floor - start);
				minIdx = i;
				if (elva[i].floor >= start) {
					minDir = 0;
				}
				else {
					minDir = 1;
				}
			}
		}
		if (upButton) {
			if (elva[i].dir == 1 && elva[i].floor <= start) {
				if (min > abs(elva[i].floor - start)) {
					min = abs(elva[i].floor - start);
					minIdx = i;
					minDir = elva[i].dir;
				}
			}
			else if (elva[i].dir == 0 && elva[i].floor >= start) {
				int flag = 0;
				for (int j = start+10; j >= 0; j--) {
					if (j == 10) continue;
					if (elva[i].info[j].man != 0 || elva[i].info[j].woman != 0) {
						flag = 1;
						break;
					}
				}
				if (flag) continue;
				if (min > abs(elva[i].floor - start)) {
					min = abs(elva[i].floor - start);
					minIdx = i;
					minDir = elva[i].dir;
				}
			}
		}
		if(downButton) {
			if (elva[i].dir == 0 && elva[i].floor >= start) {
				if (min > abs(elva[i].floor - start)) {
					min = abs(elva[i].floor - start);
					minIdx = i;
					minDir = elva[i].dir;
				}
			}
			else if (elva[i].dir == 1 && elva[i].floor <= start) {
				int flag = 0;
				for (int j = start + 10; j <= TOTAL_FLOOR; j++) {
					if (j == 10) continue;
					if (elva[i].info[j].man != 0 || elva[i].info[j].woman != 0) {
						flag = 1;
						break;
					}
				}
				if (flag) continue;
				if (min > abs(elva[i].floor - start)) {
					min = abs(elva[i].floor - start);
					minIdx = i;
					minDir = elva[i].dir;
				}
			}
		}
	}
	if (minIdx == -1) return minIdx;
	elva[minIdx].dir = minDir;
	return minIdx;
}

void checkElevatorLoc(elevator_t *elva, int *upButton,int *downButton, int *nowFloor,
	int *moveElva,int *moveFlag,int * nextFloor,int gender) { //���������͸� ž���� ���� �ʿ��� �������� ����� �Լ�
	int check, door;
	if (*upButton || *downButton) {
		*moveElva = checkSpeed(elva, *nowFloor, *upButton, *downButton);
		
		if (elva[*moveElva].floor == *nowFloor) {
			printf("\n%d�� ���������Ͱ� �����߽��ϴ�.\n", *moveElva + 1);
			printf("Ÿ�ðڽ��ϱ�? (��: 1 , �ƴϿ�: 0)\n");
			scanf_s("%d", &check);
			if (check) {
				printf("���������Ϳ� ž���ϼ̽��ϴ�.\n");
				printf("3�� �� ���� �ڵ����� �����ϴ�.\n");
				printf("���� �ݰ� �ʹٸ� ���� ����Ű��\n");
				printf("���� ���� �ʹٸ� ������ ����Ű�� �����ּ���.\n");
				int sec=4;
				while (--sec) {
					if (_kbhit()) {
						char key = _getch();
						if (key == -32) {
							key = _getch();
						}

						if (key == LEFT) {
							printf("1�� �� ���� �����ϴ�.\n");
							Sleep(1000);
							break;
						}
						else if (key == RIGHT) {
							printf("���� �����ϴ�.\n");
							sec = 3;
						}
					}
					printf("%d��...\n", sec);
					Sleep(1000);
				}
				printf("���� �����ϴ�.\n");
				printf("��� ������ ���ðڽ��ϱ�?(%d ~ %d������ ���� �����մϴ�.)\n", MIN_FLOOR, MAX_FLOOR);
				scanf_s("%d", nextFloor);
				while (*nextFloor == *nowFloor || (*upButton && *nextFloor < *nowFloor && elva[*moveElva].dir==1) ||
					(*downButton && *nextFloor > *nowFloor && elva[*moveElva].dir==0) ||
					(*nextFloor < MIN_FLOOR || *nextFloor > MAX_FLOOR)) {
					if (*nextFloor == 0) {
						printf("0���� �������� �ʽ��ϴ�.\n");
					}
					else if (*nextFloor < MIN_FLOOR || *nextFloor > MAX_FLOOR) {
						printf("%d ~ %d������ ���� �����մϴ�.\n", MIN_FLOOR, MAX_FLOOR);
					}
					else if (*nextFloor == *nowFloor) {
						printf("���� ������ ���� ������ ���� �Է��ϼ̽��ϴ�.\n");
					}
					else if (*upButton && *nextFloor < *nowFloor) {
						printf("���� �� ������ ���� ���� �Է� �����մϴ�.\n");
					}
					else {
						printf("���� �� ������ ���� ���� �Է� �����մϴ�.\n");
					}
					printf("�ٽ� �Է����ּ���.\n");
					scanf_s("%d", nextFloor);
				}

				*moveFlag = 1;
				if (gender) {
					elva[*moveElva].gender.man++;
				}
				else {
					elva[*moveElva].gender.woman++;
				}
				if (*nextFloor - *nowFloor > 0) {
					elva[*moveElva].dir = 1;
				}
				else {
					elva[*moveElva].dir = 0;
				}
			}
			else {
				printf("���������Ϳ� ž������ �����̽��ϴ�.\n");
				Sleep(500);
				*moveElva = -1;
			}
			*downButton = 0;
			*upButton = 0;
		}
		else {
			if (elva[*moveElva].dir == 1) {
				elva[*moveElva].floor++;
				if (elva[*moveElva].floor == 0) {
					elva[*moveElva].floor++;
				}
			}
			else if(elva[*moveElva].dir == 0) {
				elva[*moveElva].floor--;
				if (elva[*moveElva].floor == 0) {
					elva[*moveElva].floor--;
				}
			}
		}
	}
}

void elevatorMove(elevator_t* elva, int* moveFlag, int *moveElva, int* nowFloor,
	int *nextFloor,int gender) { //ž���� ������������ �̵��� ����ϴ� �Լ�
	if (elva[*moveElva].floor == *nextFloor) {
		if (*nowFloor > 0) {
			printf("\n%d���� �����߽��ϴ�.\n", *nowFloor);
		}
		else {
			printf("\nB%d���� �����߽��ϴ�.\n", *nowFloor * -1);
		}
		Sleep(1000);
		printf("���� �����ϴ�.\n");
		Sleep(1000);
		*moveFlag = 0;
		if (gender) {
			elva[*moveElva].gender.man--;
		}
		else {
			elva[*moveElva].gender.woman--;
		}
		*moveElva = -1;
	}
	else {
		if (elva[*moveElva].dir) {
			elva[*moveElva].floor++;
			if (elva[*moveElva].floor == 0) {
				elva[*moveElva].floor++;
			}
		}
		else {
			elva[*moveElva].floor--;
			if (elva[*moveElva].floor == 0) {
				elva[*moveElva].floor--;
			}
		}

		*nowFloor = elva[*moveElva].floor;
	}

}

void randBoard(elevator_t *elva) { //ž�� �� �������� �߰�
	int randomE = rand() % 6; // ž���� ���������͸� ����
	int randomM = rand() % 10; // ���� ���� �������� ����
	int randomW = rand() % 10; // ���� ���� �������� ����
	info_t nowGender;
	nowGender.man = elva[randomE].gender.man + randomM; // ���� ���������� ��ġ���� ž��
	nowGender.woman = elva[randomE].gender.woman + randomW; //���� ���������� ��ġ���� ž��

	while(nowGender.man*MAN_KG + nowGender.woman*WOMAN_KG >= MAX_KG) { //�����϶�
		int flag = rand() % 2; // ���� ������ �������� ���� (0: ����, 1: ����)
		if (flag) {
			nowGender.man--;
			randomM--;
		}
		else {
			nowGender.woman--;
			randomW--;
		}
	}
	
	

	elva[randomE].gender.man = nowGender.man;
	elva[randomE].gender.woman = nowGender.woman;

	//���� ��ġ���� ž���� ������� ��� ������ ���� �������� ������ ����
	info_t now;
	now.man = randomM;
	now.woman = randomW;
	while (now.man > 0 || now.woman > 0) {
		int nextFloor = rand() % 111;
		if (nextFloor == 10) {
			continue;
		}
		info_t next = now;
		if (now.man > 0) {
			next.man = rand() % (now.man + 1);
		}
		if (now.woman > 0) {
			next.woman = rand() % (now.woman + 1);
		}
		elva[randomE].info[nextFloor].man += next.man;
		elva[randomE].info[nextFloor].woman += next.woman;
		now.man -= next.man;
		now.woman -= next.woman;
	}
	
}

void checkHuman(elevator_t* elva,int now) {
	
	int nowFloor = elva[now].floor+10; // -10 ~ -1 && 1 ~ 100  ->  0 ~9 && 11 ~ 110
	int nowM = elva[now].info[nowFloor].man;
	int nowW = elva[now].info[nowFloor].woman;

	if (nowM == 0 && nowW == 0) return;

	elva[now].info[nowFloor].man = 0;
	elva[now].info[nowFloor].woman = 0;

	elva[now].gender.man -= nowM;
	elva[now].gender.woman -= nowW;

	if (elva[now].gender.man == 0 && elva[now].gender.woman == 0) {
		elva[now].dir = -1;
	}
	//if (nowM != 0 || nowW != 0) {
	//	printf("\n%d�� ���������Ϳ��� ����: %d�� ����: %d���� ���Ƚ��ϴ�.\n",now+1,nowM,nowW);
	//	Sleep(500); //0.5�ʵ��� ���α׷��� ����
	//}
	
}

void checkDirection(elevator_t* elva,int now,int moveElva) {
	if ((elva[now].gender.man == 0 && elva[now].gender.woman == 0) && moveElva != now) {
		elva[now].dir = -1;
		return;
	}
	if (elva[now].dir == 1) {
		int flag = 0;
		int nFloor = elva[now].floor + 10; // -10 ~ -1 && 1 ~ 100  ->  0 ~9 && 11 ~ 110
		while (nFloor <= TOTAL_FLOOR) {
			if (elva[now].info[nFloor].man > 0 || elva[now].info[nFloor].woman > 0) {
				flag = 1;
				break;
			}
			nFloor++;
			if (nFloor == 10) {
				nFloor++;
			}
		}
		if (!flag) {
			elva[now].dir = 0;
		}
	}
	else if (elva[now].dir == 0) {
		int flag = 0;
		int nFloor = elva[now].floor+10;
		while (nFloor >= 0) {
			if (elva[now].info[nFloor].man > 0 || elva[now].info[nFloor].woman > 0) {
				flag = 1;
				break;
			}
			nFloor--;
			if (nFloor == 10) {
				nFloor--;
			}
		}
		if (!flag) {
			elva[now].dir = 1;
		}
	}
	else {
		int flag = 0;
		int nFloor = elva[now].floor + 10;
		while (nFloor <= TOTAL_FLOOR) {
			if (elva[now].info[nFloor].man > 0 || elva[now].info[nFloor].woman > 0) {
				flag = 1;
				break;
			}
			nFloor++;
			if (nFloor == 10) {
				nFloor++;
			}
		}
		if (flag) {
			elva[now].dir = 1;
			return;
		}
		flag = 0;
		nFloor = elva[now].floor + 10;
		while (nFloor >= 0) {
			if (elva[now].info[nFloor].man > 0 || elva[now].info[nFloor].woman > 0) {
				flag = 1;
				break;
			}
			nFloor--;
			if (nFloor == 10) {
				nFloor--;
			}
		}
		if (flag) {
			elva[now].dir = 0;
			return;
		}
	}
}

void autoMove(elevator_t* elva, int moveElva) {
	for (int i = 0; i < 6; i++) {
		if (elva[i].floor <= MAX_FLOOR && elva[i].floor >= MIN_FLOOR) {
			if (elva[i].gender.man == 0 && elva[i].gender.woman == 0) {
				elva[i].dir = -1;
				continue;
			}
			if (moveElva != i) {
				if (elva[i].dir == 1 && elva[i].floor < MAX_FLOOR) {
					elva[i].floor++;
					if (elva[i].floor == 0) {
						elva[i].floor++;
					}
				}
				else if (elva[i].dir == 0 && elva[i].floor > MIN_FLOOR) {
					elva[i].floor--;
					if (elva[i].floor == 0) {
						elva[i].floor--;
					}
				}

				checkDirection(elva, i, moveElva);
			}
			checkHuman(elva,i);
		}
	}
}

int main()
{
	srand(time(NULL)); //������ ���� �õ尪���� �ð��� �����
	elevator_t elva[6] = { 0, }; //6�� �̹Ƿ� 6¥�� ����ü �迭 ����
	int upButton=0,downButton=0,nowFloor=1,loop=0,moveFlag = 0,moveElva=-1;
	//upButton : ���� ���ϴ� ��ư�� ���¸� �����ϴ� ���� (on: 1, off: 0)
	//downButton : �Ʒ��� ���ϴ� ��ư�� ���¸� �����ϴ� ���� (on: 1, off: 0)
	//nowFloor : ���� ������� ������ �����ϴ� ����
	//loop : ������ ������ ������ �����ϴ� ���� (���� �� : 1 , ���� X : 0)
	//moveFlag : ���������Ͱ� �������� �ȿ��������� �����ϴ� ���� (������: 1 , �ȿ�����: 0)
	//moveElva : �����̴� ������������ ��ȣ�� �����ϴ� ����
	int check = 0, nextFloor = 0, door = 0, gender = 0, help = 0, admin = 0, inspectCnt = 0;
	//check : ���������� ž�� ���θ� �����ϴ� ���� (ž��: 1 , ž�°ź�: 0)
	//nextFloor : ������ �ϴ� ���� �����ϴ� ����
	//door : ������������ ���� ������ ������ �����ϴ� ���� (����: 1 , ���� ����: 0)
	//gender : ������� ������ �����ϴ� ���� (����: 1 , ����: 0)
	//help : ���� Ȱ��ȭ ���θ� ��Ÿ���� ���� (Ȱ��ȭ: 1 , ��Ȱ��ȭ: 0)
	//admin : ������ ��� Ȱ��ȭ ���θ� ��Ÿ���� ���� (Ȱ��ȭ: 1 , ��Ȱ��ȭ: 0)
	//inspectCnt : ���� ���� ������������ ������ �����ϴ� ����
	int period = 0;
	char correctPw[100] = "1234567*"; //�����ڸ�� ��й�ȣ


	init(elva); //�ʱ�ȭ

	//Call by reference�� ���� �Լ��� �Ű������� ��κ� ������ ������
	do {
		if (loop == 0) { //�� ó�� ����Ǿ��� ��
			enter(&loop, &gender); //�� ó�� ����Ǿ��� �� �ʿ��� ������ �ޱ� ���� �Լ�
		}
		else {
			if (_kbhit()) { //Ű�� �������ִٸ�
				char key = _getch(); //Ű �Է�
				if (key == -32) {
					key = _getch();
				}
				else if (key == 0) {
					key = _getch();
				}

				if (key == UP && !moveFlag) { //���� ����Ű�� ������ ��
					upButton = !upButton;
					moveElva = -1;
				}
				else if (key == DOWN && !moveFlag) { //�Ʒ��� ����Ű�� ������ ��
					downButton = !downButton;
					moveElva = -1;
				}
				else if (key == F1) { //F1�� ������ ��
					help = 1;
				}
				else if (key == F2) { //F2�� ������ ��
					admin = !admin;
				}
				else if (key == F3 && admin == 2) { //������ ��� ���¿��� F3�� ���� ��

				}
				else if (key == ESC) { //ESC�� ������ ��
					loop = 0;
				}
			}
			
			if (help) {
				helpMessage(); // ���� UI ��� �Լ�
				help = 0;
				continue;
			}

			if (admin) {
				adminPrint(elva, &admin, correctPw, &inspectCnt); // ������ ��� �� UI ��� �Լ�
			}
			else {
				userPrint(elva, moveElva, upButton, downButton, nowFloor); //�⺻ UI ��� �Լ�
			}

			if (moveFlag) { //���������� ž��
				elevatorMove(elva, &moveFlag, &moveElva, &nowFloor, &nextFloor, gender);
			}
			else { //���������Ͱ� �������� ���� ���� �Է�
				checkElevatorLoc(elva, &upButton, &downButton, &nowFloor, &moveElva, &moveFlag, &nextFloor, gender);
			}

			autoMove(elva,moveElva);

			if (period == 30) {
				randBoard(elva);
				period = 0;
			}
			period++;
			Sleep(500); //0.5�ʵ��� ���α׷��� ����
			system("cls"); //cmdȭ�鿡 ���� ��� ����� ������
		}
	} while (loop == 1);
	printf("���������� ���α׷��� �����մϴ�.\n");
	return 0;
}