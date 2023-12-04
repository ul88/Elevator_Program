//�������� ������ ��. ������ ���� ������ �� �׻� �����ϰ� ��.
//����ڰ� ���������Ϳ� Ż ��쿡�� �ִ� ���뷮�� �Ѱ� ���� ���� ���� �� ����
//ž�°��� 0���� �� ������ ���������ʹ� ��� ������
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
//#define PERIOD_VALUE 30 // ����� Ÿ�� �ֱⰡ 15�� �̹Ƿ� 30���� ����

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

int admin = 0, periodValue = 30; 
// admin : ������ ���ѿ� ���� �����̹Ƿ� �������� ó��
// periodValue : ����� Ÿ�� �ֱⰡ 15���̹Ƿ� 30���� �ʱ�ȭ�ϰ� ���������� ����

void CursorView() // cmd ũ�⸦ �����ϴ� �Լ�
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //Ŀ�� ���� (1 ~ 100)
	cursorInfo.bVisible = FALSE; //Ŀ�� Visible TRUE(����) FALSE(����)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void init(elevator_t* elva) { //�ʱ�ȭ
	CursorView(); // cmd ũ�⸦ �����ϴ� �Լ� ����
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
		
		int nowMan = elva[i].gender.man; // ���� ������ ��
		int nowWoman = elva[i].gender.woman; // ���� ������ ��
		while (nowMan != 0 || nowWoman != 0) { // �� �� 0�� �� ������ �ݺ�
			int nextFloor = rand() % 111; // nextMan�� nextWoman �������� ������ ������ �������� ����
			if (nextFloor == 10) { // nextFloor == 10�̸� 0���̶�� �Ҹ��̹Ƿ� continue ����
				continue;
			}
			int nextMan = rand() % (nowMan + 1); // nowMan���� �������� ������ ���� ����
			int nextWoman = rand() % (nowWoman + 1); // nowWoman���� �������� ������ ���� ����
			elva[i].info[nextFloor].man += nextMan; // nextFloor���� �������ϹǷ� info[nextFloor] ��ġ�� nextMan�� ����
			elva[i].info[nextFloor].woman += nextWoman; // nextFloor���� �������ϹǷ� info[nextFloor] ��ġ�� nextWoman�� ����
			nowMan -= nextMan; // nextMan��ŭ ��� ���� �� �� ���������Ƿ� nextMan��ŭ nowMan���� ����
			nowWoman -= nextWoman; // nextWoman��ŭ ��� ���� �� �� ���������Ƿ� nextWoman��ŭ nowWoman���� ����
		}
	}
}

void userPrint(elevator_t* elva, int elvaIdx,int upButton, int downButton, int nowFloor) { //���������� ���
	if (admin == 2) { // admin == 2 �϶� ���� ����� ��
		printf("\t**������ ����Դϴ�.**\n");
		printf("F3�� ������ ������ ��� ���� ��ɾ ����� �� �ֽ��ϴ�.\n");
	}
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
			if (elva[j + (i * 3)].dir == -1) { // ���� ���������Ͱ� ����� ��
				printf("��\t");
			}
			else if (elva[j + (i * 3)].dir == 1) { // ���� ���������Ͱ� �ö󰡴� ��
				printf("��\t");
			}
			else { // ���� ���������Ͱ� �������� ��
				printf("��\t");
			}
		}
		printf("\n");

		//������ ��
		for (int j = 0; j < 3; j++) { // ������������ ��ġ�� 2�� 3���̱� ������ ���� �ݺ�
			printf("���� : %2d  ", elva[j + (i * 3)].gender.man);
			if (elva[j + (i * 3)].dir == -1) { // ���� ���������Ͱ� ����� ��
				printf("��\t");
			}
			else if (elva[j + (i * 3)].dir == 1) { // ���� ���������Ͱ� �ö󰡴� ��
				printf("��\t");
			}
			else { // ���� ���������Ͱ� �������� ��
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
			if (nowFloor > 0) { // ���� ���϶�
				printf("\t���� ���� : %3d", nowFloor);
			}
			else { // ���� ���϶�
				printf("\t���� ���� : B%2d", nowFloor*-1);
			}
			if (upButton) { // ���� ���� ��ư�� ������ ��
				printf("    ��\n\t\t");
			}
			else { // �Ʒ��� ���� ��ư�� ������ ��
				printf("    ��\n\t\t");
			}
			if (downButton) { // �Ʒ��� ���� ��ư�� ������ ��
				printf("           ��");
			}
			else { // ���� ���� ��ư�� ������ ��
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

void password(char* correctPw) { // ��й�ȣ �Է� �Լ�
	int pwCnt = 5;  // Ʋ���� �� �ִ� ��ȸ ���� �����ϴ� ����
	char pw[100] = ""; // �Է¹޴� ��й�ȣ�� ������ ����
	char changePw[10] = "change"; // ��й�ȣ ������ �� ����� ��ɾ �����ϴ� ����
	//int flag = 0;
	printf("������ ����Դϴ�.\n");
	printf("��й�ȣ�� �Է����ּ���. (8�ڸ��̻�, 20�ڸ����ϸ� �����մϴ�.)\n");
	printf("��й�ȣ ������ ���Ѵٸ� \"%s\"�� �Է����ּ���.\n", changePw);
	scanf_s("%s", pw, 50); // ��й�ȣ �Է�

	if (checkStr(pw, changePw)) { // pw�� changePw�� ���� �������� Ȯ��
		printf("��й�ȣ�� �����մϴ�.\n");
		printf("������ ��й�ȣ�� �Է����ּ���.\n");
		scanf_s("%s", pw, 50); // ��й�ȣ �Է�

		if (checkStr(pw, correctPw)) { // pw�� correctPw�� ���� �������� Ȯ��
			printf("�����ϰ� ���� ��й�ȣ�� �Է����ּ���. (8�ڸ��̻�, 20�ڸ����ϸ� �����մϴ�.)\n");
			scanf_s("%s", pw, 50); // ��й�ȣ �Է�
			if (strLen(pw) >= 8 && strLen(pw) <= 20) { // pw�� ���̰� 8�̻� 20 ���϶��
				changeStr(pw, correctPw);
				printf("%s�� ��й�ȣ�� �����մϴ�.\n", correctPw);
			}
			else { // pw�� ���̰� 8�̻� 20���ϰ� �ƴ϶��
				printf("8�ڸ��̻�, 20�ڸ� ������ ��й�ȣ�� �Է����ּ���.\n");
			}
		}
		else { // pw�� correctPw�� ���� �ٸ��ٸ�
			printf("��й�ȣ�� Ʋ���ϴ�.\n");
		}

		printf("1�� �� ������ ��尡 ����۵˴ϴ�.\n");
		Sleep(1000);
		return;
	}

	while (!checkStr(pw, correctPw) && pwCnt > 0 && strLen(pw) != 1) { // pw�� correctPw�� ���� �ٸ��鼭 pwCnt�� 0���� ũ�鼭 pw�� ���̰� 1�� �ƴ� ���� �ݺ�
		system("cls");
		printf("��й�ȣ�� Ʋ�Ƚ��ϴ�.\n");
		printf("- %d�� ���ҽ��ϴ�. -\n", pwCnt);
		printf("�ٽ� �Է����ּ���. (�ϳ��� �Է��ϸ� ������ ��尡 �ٷ� ����˴ϴ�.)\n");
		scanf_s("%s", pw, 50); // ��й�ȣ �Է�
		pwCnt--; // pwCnt�� ���� �ϳ� ���� (�ʱⰪ : 5)
	}
	if (!checkStr(pw, correctPw)) { // ������������ pw�� correctPw�� �ٸ��ٸ�
		admin = 0; // �����ڸ�带 ������
		printf("1�� �� ������ ��带 �����մϴ�.\n");
		Sleep(1000);
		return;
	}
	else { // pw�� correctPw�� ���ٸ�
		admin = 2; // �����ڸ�带 ������
		printf("1�� �� ������ ��带 �����մϴ�.\n");
		Sleep(1000);
	}
}

void behaviorAdmin(elevator_t* elva, char *correctPw, int *inspectCnt) { //������ ��� �Լ�
	if (admin == 1) {
		password(correctPw); // ��й�ȣ �Է� �Լ�
	}
	else {
		//admin == 2�϶� F3�� �����ٸ� ������ ���� ��ɾ ����� �� �ֵ��� ��
		int flag = 0; // ��ɾ �����ϴ� ����

		printf("������ ��ɾ �����ϼ���.\n");
		printf("0. ��ɾ� ���� ����\n");
		printf("1. ���������� ����\n");
		printf("2. ���������Ͱ� ������ �� ����\n");
		printf("3. ���������Ϳ� ����� Ÿ�� �ֱ� ����\n");
		scanf_s("%d",&flag); // flag �Է�
		if (flag == 0) { // flag�� 0�̸� �Լ� ����
			return;
		}
		else if (flag == 1) { // flag�� 1�̸�
			int yes = 0; // ���������� ������ ���� ������ �����ϴ� ����
			printf("���������� ������ �Ͻðڽ��ϱ�? (���� ���� ���� ���������� : %d ��) (��: 1, �ƴϿ�: 0)\n", *inspectCnt);
			scanf_s("%d",&yes); // yes �Է�
			if (yes == 1) { // yes�� 1�̶��
				int cnt = 0; // ������ ����� �����ϴ� ����
				printf("������ ����� �����ϼ���. (�ִ� 2��)\n");
				scanf_s("%d", &cnt); // cnt �Է�
				while (cnt > 2 || cnt < 0) { //cnt�� 2 �ʰ� �Ǵ� 0 �̸��� ��
					printf("�߸��� ���� �Է��ϼ̽��ϴ�.\n"); 
					printf("0 ~ 2������ ���� �Է����ּ���.\n");
					printf("������ ����� �����ϼ���. (�ִ� 2��)\n");
					scanf_s("%d", &cnt); // cnt�� ���� 0 �̻� 2 ���Ͽ����ϹǷ� �ٽ� �Է�
				}
				int select = 0; // ������ ���������͸� �����ϴ� ����
				for (int i = 0; i < cnt; i++) { // cnt��ŭ �ݺ�
					printf("���� ���θ� ������ ���������͸� ����ּ���.(�ϳ��� �������ּ���.)\n");
					scanf_s("%d", &select); // select �Է�
					if (select < 0 || select > 6) { // ���������ʹ� 6���� �����ϹǷ� select�� 0�̸� �Ǵ� 6 �ʰ��� �� ��� �ݺ��ϵ��� ����
						printf("���������ʹ� 1������ 6�������� �����մϴ�.\n");
						i--; // i���� �ٿ� �ٽ� �ݺ�
						continue;
					}
					if (elva[select - 1].inspect == 0 && *inspectCnt == 2) { // �ִ� ���� ����� 2���̹Ƿ� 
						//(select-1) ���������Ͱ� ���� ���� �ƴϸ鼭 ���� ���� ���������� ���� 2�� ���� �������� ���� 
						printf("������ 2������� �����մϴ�.\n");
						printf("�ٽ� �������ּ���.\n");
						i--; // i���� �ٿ� �ٽ� �ݺ�
						continue;
					}
					else { // ������ ������ ��
						printf("%d�� ������������ ���� ���θ� ��ȯ�մϴ�.\n",select);
						if (elva[select - 1].inspect == 0) { // ���� ���� �ƴ� ��
							elva[select - 1].inspect = 1; // ���� ������ �ٲ�
							(*inspectCnt)++; // ���� ���� ���������Ͱ� �þ���Ƿ� 1 ����
						}
						else { // ���� ���� ��
							elva[select - 1].inspect = 0; // ���� ���� �ƴ����� �ٲ�
							(*inspectCnt)--; // ���� ���� �ƴ� ���������Ͱ� �þ���Ƿ� 1 ����
						}
					}
				}

			}
			else { // yes�� 0�̶��
				return;
			}
		}
		else if (flag == 2) { // flag�� 2�� ��
			int select = 0; // ������������ ��ȣ�� ��Ÿ���� ����
			printf("� ���������͸� ���� �ͳ���.\n");
			scanf_s("%d", &select); // select �Է�
			while (select < 1 || select > 6) { // ���������ʹ� 6���� �����ϹǷ� select�� 0�̸� �Ǵ� 6 �ʰ��� �� ��� �ݺ��ϵ��� ����
				printf("���������ʹ� 1������ 6�������� �����մϴ�.\n");
				scanf_s("%d", &select);
			}
			system("mode con cols=100 lines=111"); // ������������ ���� 10������ ���� 100������ ���� ����ؾ��ϹǷ� cmdâ�� ũ�⸦ �ø�
			printf("\t--%d ����������--\n", select);
			for (int i = 0; i <= TOTAL_FLOOR; i++) { // ���� 10������ ���� 100������ �� 109�� �̹Ƿ� 0���� 110���� �ݺ�
				if (i == 10) continue; // i�� 10�� ���� 0���� ����. 0���� �����Ƿ� continue
				if (elva[select - 1].info[i].man == 0 && elva[select - 1].info[i].woman == 0) continue; // �� �� 0�� ���� ����� ���� �ʰ� �Ѿ
				if (i < 10) { // i�� 10���� ���� ���� ���ϸ� ����.
					printf("B%2d��\t���� : %3d��\t���� : %3d��\n", (i - 10) * -1, elva[select - 1].info[i].man, elva[select - 1].info[i].woman);
				}
				else { // i�� 10���� Ŭ ���� ������ ����.
					printf("%3d��\t���� : %3d��\t���� : %3d��\n", i - 10, elva[select - 1].info[i].man, elva[select - 1].info[i].woman);
				}
			}
			system("pause"); // 109���� ���������͸� �� ���⿣ �ʴ����δ� ������ �� �����Ƿ� ���α׷��� ��� ������Ŵ
			system("mode con cols=100 lines=30"); // ������ ���������� ȭ������ ���ư��Ƿ� cmdâ�� ũ�⸦ �ٽ� ������
		}
		else if (flag == 3) {
			int periodTemp = 0; // �ٲ� �ֱ⸦ ������ ���� ����
			printf("���������Ϳ� �ο��� ž���ϴ� �ֱ⸦ �ٽ� �������ּ���.\n");
			scanf_s("%d", &periodTemp); // periodTemp ����
			printf("ž�� �ֱⰡ %d�ʿ��� %d�ʷ� ����Ǿ����ϴ�.\n", periodValue/2, periodTemp);
			printf("1�� �� ���ư��ϴ�.\n");
			Sleep(1000); // 1 �� ����
			periodValue = periodTemp * 2; // 0.5�� �����̹Ƿ� periodTemp���� 2�� ���Ѱŷ� ��ȯ�Ѵ�.
		}
		else { // �߸��� �Է��� ���� ��
			printf("�߸��� �Է��� �ϼ̽��ϴ�.\n");
			printf("1�� �� ��ɾ� ������ �ٽ� �����մϴ�.\n");
			Sleep(1000); // 1�� ����
			behaviorAdmin(elva, correctPw, inspectCnt); // behaviorAdmin()�� �ٽ� ����
		}
	}
}

void enter(int *loop,int *gender) { // ���� ���θ� �Ǻ��ϱ� ���� �Լ�
	printf("������ ��� �ǳ���?(����: 1 , ����: 0)\n");
	scanf_s("%d", gender); //gender �Է�
	printf("���������Ϳ� �����Ͻðڽ��ϱ�?\n");
	printf("������ �Ͻ� �Ŀ��� ESC�� ������ ������ ������ �� �ֽ��ϴ�.\n");
	printf("������ ����Ѵٸ� 1, ������ ������� �ʴ´ٸ� 0�� �Է����ּ���.\n");
	scanf_s("%d", loop); //loop �Է�
}

void helpMessage() { // ���� ��� �Լ�
	int flag = 0; //���� ���� ���θ� ��Ÿ���� ����
	system("cls");
	printf("���������͸� �θ��� ����� ���� ����Ű �Ǵ� �Ʒ��� ����Ű�� ������ �˴ϴ�.\n");
	printf("���������͸� ž������ �� ���� ����Ű�� ������ �� ��� ���� �����ϴ�.\n");
	printf("\t\t\t������ ����Ű�� ������ �� ��� ���� �����ϴ�.\n");
	printf("ESC�� ������ ���α׷��� ����˴ϴ�.\n");
	printf("F2�� ������ ������ ��带 ������ �� �ֽ��ϴ�.\n");
	printf("F2�� �ٽ� ������ ������ ��尡 ����˴ϴ�.\n");
	printf("������ �����Ͻðڽ��ϱ�? (��: 1 , �ƴϿ�: 0)\n");
	scanf_s("%d", &flag); // flag �Է�
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
	int minDir = 1; // ���� ������ ������������ �̵� ������ ������ ����
	for (int i = 0; i < 6; i++) { // 6���̹Ƿ� 0���� 5���� �ݺ�
		if (elva[i].inspect == 1) continue; // ���� ���̶�� �� �ʿ䰡 �����Ƿ� continue
		if (elva[i].dir == -1) { // ���������Ͱ� �������� �ʰ� ���� ��
			if (min > abs(elva[i].floor - start)) { // ���� �ּҰ����� |i�� ������������ �� �� - ���� ������� ����| �� �� �۴ٸ�
				min = abs(elva[i].floor - start); // �ּҰ��� ����
				minIdx = i; // ������ ������������ ��ȣ�� ���� ����
				if (elva[i].floor >= start) { // i�� ���������Ͱ� ���� ������� ��ġ���� ũ�ų� ���ٸ�
					minDir = 0; // ���������ϹǷ� ������ 0���� ����
				}
				else { // i�� ���������Ͱ� ���� ������� ��ġ���� �۴ٸ�
					minDir = 1; // �ö󰡾��ϹǷ� ������ 1�� ����
				}
			}
		}
		if (upButton) { // ���� ���� ��ư�� ������ ��
			if (elva[i].dir == 1 && elva[i].floor <= start) { // ���ΰ��� �����������̸鼭 ���� ������� ��ġ���� �۰ų� ���� ��
				if (min > abs(elva[i].floor - start)) { // ���� �ּҰ����� |i�� ������������ �� �� - ���� ������� ����| �� �� �۴ٸ�
					min = abs(elva[i].floor - start); // �ּҰ��� ����
					minIdx = i; // ������ ������������ ��ȣ�� ���� ����
					minDir = elva[i].dir; // ������ ������������ ���⵵ ���� ����
				}
			}
			else if (elva[i].dir == 0 && elva[i].floor >= start) { // �Ʒ��� ���� �����������̸鼭 ���� ������� ��ġ���� ũ�ų� ���� ��
				int flag = 0; // ���� ���θ� �Ǵ��� ����
				for (int j = start+10; j >= 0; j--) { // ���� ��ġ+10���� 0���� �ݺ�
					if (j == 10) continue; // j�� 10�̸� 0���̹Ƿ� continue
					if (elva[i].info[j].man != 0 || elva[i].info[j].woman != 0) { // i�� ���������Ϳ� ���� �Ǵ� ���ڰ� j���� �ϳ��� ���� ��
						flag = 1; // flag�� 1�� ����
						break;
					}
				}
				if (flag) continue; // flag�� 1�� ���� ���� ��ġ ������ 
				//i�� ���������Ͱ� �� �������ٴ� ������, �̶��� �ּҰ����� Ȯ���� �ʿ䰡 �����Ƿ� continue;
				if (min > abs(elva[i].floor - start)) { // ���� �ּҰ����� |i�� ������������ �� �� - ���� ������� ����| �� �� �۴ٸ�
					min = abs(elva[i].floor - start); // �ּҰ��� ����
					minIdx = i; // ������ ������������ ��ȣ�� ����
					minDir = elva[i].dir; // ������ ������������ ���⵵ ���� ����
				}
			}
		}
		if(downButton) { //�Ʒ��� ���� ��ư�� ������ ��
			if (elva[i].dir == 0 && elva[i].floor >= start) { // �Ʒ��� ���� ���������͸鼭 ���� ������� ��ġ���� ũ�ų� ���� ��
				if (min > abs(elva[i].floor - start)) { // ���� �ּҰ����� |i�� ������������ �� �� - ���� ������� ����| �� �� �۴ٸ�
					min = abs(elva[i].floor - start); // �ּҰ��� ����
					minIdx = i; // ������ ������������ ��ȣ�� ����
					minDir = elva[i].dir; // ������ ������������ ���⵵ ���� ����
				}
			}
			else if (elva[i].dir == 1 && elva[i].floor <= start) { // ���� ���� ���������͸鼭 ���� ������� ��ġ���� �۰ų� ���� ��
				int flag = 0; // ���� ���θ� �Ǵ��� ����
				for (int j = start + 10; j <= TOTAL_FLOOR; j++) { // ���� ��ġ+10���� 110���� �ݺ�
					if (j == 10) continue; // j�� 10�̸� 0���̹Ƿ� continue
					if (elva[i].info[j].man != 0 || elva[i].info[j].woman != 0) { // i�� ���������Ϳ� ���� �Ǵ� ���ڰ� j���� �ϳ��� ���� ��
						flag = 1; // flag�� 1�� ����
						break;
					}
				}
				if (flag) continue; // flag�� 1�� ���� ���� ��ġ ������ 
				//i�� ���������Ͱ� �� �������ٴ� ������, �̶��� �ּҰ����� Ȯ���� �ʿ䰡 �����Ƿ� continue;
				if (min > abs(elva[i].floor - start)) { // ���� �ּҰ����� |i�� ������������ �� �� - ���� ������� ����| �� �� �۴ٸ�
					min = abs(elva[i].floor - start); // �ּҰ��� ����
					minIdx = i; // ������ ������������ ��ȣ�� ����
					minDir = elva[i].dir; // ������ ������������ ���⵵ ���� ����
				}
			}
		}
	}
	if (minIdx == -1) return minIdx; // minIdx�� -1�̸� minIdx�� �״�� ��ȯ
	elva[minIdx].dir = minDir; // minIdx�� ������������ ������ minDir�� �ٲ�
	return minIdx; // minIdx ��ȯ
}

void checkElevatorLoc(elevator_t *elva, int *upButton,int *downButton, int *nowFloor,
	int *moveElva,int *moveFlag,int * nextFloor,int gender) { //���������͸� ž���� ���� �ʿ��� �������� ����� �Լ�
	int check;
	// check : ���������� ž�� ���θ� ��Ÿ���� ����
	if (*upButton || *downButton) { // ���� ���� ��ư �Ǵ� �Ʒ��� ���� ��ư�� �����ٸ�
		*moveElva = checkSpeed(elva, *nowFloor, *upButton, *downButton); // ������ ���������͸� Ȯ����
		
		if (elva[*moveElva].floor == *nowFloor) { // �θ� ������������ ������ ���� ����ڰ� �ִ� ������ ���ٸ�
			printf("\n%d�� ���������Ͱ� �����߽��ϴ�.\n", *moveElva + 1);
			printf("Ÿ�ðڽ��ϱ�? (��: 1 , �ƴϿ�: 0)\n");
			scanf_s("%d", &check); // check �Է�
			if (check) { // check�� 1�̸�
				if (elva[*moveElva].gender.man * MAN_KG + elva[*moveElva].gender.woman * WOMAN_KG + (gender ? MAN_KG : WOMAN_KG) > MAX_KG) { // ����ڰ� ž������ �� MAX_KG�� ���� �ʰ����� ��
					printf("���������� ���� �ο��� �ʰ��Ͽ����ϴ�.\n");
					printf("�����Ͻʽÿ�.\n");
					Sleep(500); // 0.5�� ��ٸ���
					*moveElva = -1; // *moveElva ���� -1�� �ٲ�
					*downButton = 0; // �Ʒ��� ���� ��ư�� 0���� �ٲ�
					*upButton = 0; // ���� ���� ��ư�� 0���� �ٲ�
					return;
				}
				printf("���������Ϳ� ž���ϼ̽��ϴ�.\n");
				printf("3�� �� ���� �ڵ����� �����ϴ�.\n");
				printf("���� �ݰ� �ʹٸ� ���� ����Ű��\n");
				printf("���� ���� �ʹٸ� ������ ����Ű�� �����ּ���.\n");
				int sec=4; // 3���� �ð��� �ֱ� ���� 4�� �ʱ�ȭ
				while (--sec) { // sec�� 1�� ���� ���鼭 �ݺ�
					if (_kbhit()) { //  Ű�Է�
						char key = _getch();
						if (key == -32) { // Ű �Է�
							key = _getch();
						}

						if (key == LEFT) { // key�� ���� LEFT��� �ٷ� ���� ���� 
							printf("1�� �� ���� �����ϴ�.\n"); 
							Sleep(1000);
							break;
						}
						else if (key == RIGHT) { // key�� ���� RIGHT��� �ٷ� ���� ��
							printf("���� �����ϴ�.\n");
							sec = 3; // ���� �ٽ� �������Ƿ� sec�� ���� 3���� �ø�
						}
					}
					printf("%d��...\n", sec);
					Sleep(1000);
				}
				printf("���� �����ϴ�.\n");
				printf("��� ������ ���ðڽ��ϱ�?(%d ~ %d������ ���� �����մϴ�.)\n", MIN_FLOOR, MAX_FLOOR);
				scanf_s("%d", nextFloor); // nextFloor �Է�
				while (*nextFloor == *nowFloor || (*upButton && *nextFloor < *nowFloor && elva[*moveElva].dir==1) ||
					(*downButton && *nextFloor > *nowFloor && elva[*moveElva].dir==0) ||
					(*nextFloor < MIN_FLOOR || *nextFloor > MAX_FLOOR)) { // ���� ���� ���� �� �Ǵ� ���� ���� ��ư�� �������鼭 ���� ������ ���� ���� �Է����� �� �Ǵ�
					// �Ʒ��� ���� ��ư�� �������鼭 ���� ������ ���� ���� �Է����� �� �Ǵ�
					// �ּ� ���� �ִ� ���� ������ �Ѿ�� ���� ���� ��� �ݺ�
					if (*nextFloor == 0) { // 0�� ��
						printf("0���� �������� �ʽ��ϴ�.\n");
					}
					else if (*nextFloor < MIN_FLOOR || *nextFloor > MAX_FLOOR) { // �ּ� ���� �ִ� ���� ������ �Ѿ�� ��
						printf("%d ~ %d������ ���� �����մϴ�.\n", MIN_FLOOR, MAX_FLOOR);
					}
					else if (*nextFloor == *nowFloor) { // ���� ���� ������ ���� �Է����� ��
						printf("���� ������ ���� ������ ���� �Է��ϼ̽��ϴ�.\n");
					}
					else if (*upButton && *nextFloor < *nowFloor) { // ���� ���� ��ư�� �������鼭 ���� ������ ���� ���� �Է����� ��
						printf("���� �� ������ ���� ���� �Է� �����մϴ�.\n");
					}
					else { // �Ʒ��� ���� ��ư�� �������鼭 ���� ������ ���� ���� �Է����� ��
						printf("���� �� ������ ���� ���� �Է� �����մϴ�.\n");
					}
					printf("�ٽ� �Է����ּ���.\n");
					scanf_s("%d", nextFloor); // nextFloor �Է�
				}

				*moveFlag = 1; // moveFlag�� ���� 1�� �ٲ�
				if (gender) { // gender�� 1�̸� �����̹Ƿ�
					elva[*moveElva].gender.man++; // ������ ���� �ϳ� �ø�
				}
				else { // gender�� 0�̸� �����̹Ƿ�
					elva[*moveElva].gender.woman++; // ������ ���� �ϳ� �ø�
				}
				if (*nextFloor - *nowFloor > 0) { // (������ �� - ���� ��)�� ������
					elva[*moveElva].dir = 1; // ������ 1�� �ٲ�
				}
				else { // (������ �� - ���� ��)�� 0���� �۰ų� ���ٸ�
					elva[*moveElva].dir = 0; // ������ 0���� �ٲ�
				}
			}
			else { // check�� 0�̸�
				printf("���������Ϳ� ž������ �����̽��ϴ�.\n");
				Sleep(500);
				*moveElva = -1; // *moveElva�� -1�� �ʱ�ȭ
			}
			*downButton = 0; // �Ʒ��� ���� ��ư�� 0���� �ٲ�
			*upButton = 0; // ���� ���� ��ư�� 0���� �ٲ�
		}
		else { // ������������ ž���� �ߴٸ�
			if (elva[*moveElva].dir == 1) { // ���������Ͱ� ���� ���� ���� ��
				elva[*moveElva].floor++; // ������ �ϳ��� ������Ŵ
				if (elva[*moveElva].floor == 0) { // 0���� �����Ƿ� �ϳ� �� ����
					elva[*moveElva].floor++;
				}
			}
			else if(elva[*moveElva].dir == 0) { // ���������Ͱ� �Ʒ��� ���� ���� ��
				elva[*moveElva].floor--; // ������ �ϳ��� ���ҽ�Ŵ
				if (elva[*moveElva].floor == 0) { // 0���� �����Ƿ� �ϳ� �� ����
					elva[*moveElva].floor--;
				}
			}
		}
	}
}

void elevatorMove(elevator_t* elva, int* moveFlag, int *moveElva, int* nowFloor,
	int *nextFloor,int gender) { //ž���� ������������ �̵��� ����ϴ� �Լ�
	if (elva[*moveElva].floor == *nextFloor) { // ź ������������ �� ���� ������ �� ���� ���� ��
		if (*nowFloor > 0) { // ���� ���̸�
			printf("\n%d���� �����߽��ϴ�.\n", *nowFloor);
		}
		else { // ���� ���� ��
			printf("\nB%d���� �����߽��ϴ�.\n", *nowFloor * -1);
		}
		Sleep(1000);
		printf("���� �����ϴ�.\n");
		Sleep(1000);
		*moveFlag = 0; // *moveFlag�� 0���� �ٲ�
		if (gender) { // gender�� 1�̸� ���� ���� �ϳ� ����
			elva[*moveElva].gender.man--;
		}
		else { // gender�� 0�̸� ���� ���� �ϳ� ����
			elva[*moveElva].gender.woman--;
		}
		*moveElva = -1; // *moveElva�� -1�� �ٲ�
	}
	else { // ź ������������ �� ���� ������ �� ���� �ٸ� ��
		if (elva[*moveElva].dir) { // ������ 1�̸�
			elva[*moveElva].floor++; // �� �� ����
			if (elva[*moveElva].floor == 0) { // 0���� ���� 1 �߰��� ����
				elva[*moveElva].floor++; 
			}
		}
		else { // ������ 0�̸�
			elva[*moveElva].floor--; // �� �� ���� 
			if (elva[*moveElva].floor == 0) { //0���� ���� 1 �߰��� ����
				elva[*moveElva].floor--;
			}
		}

		*nowFloor = elva[*moveElva].floor; // ���� �� ���� elva[*moveElva].floor�� ����
	}

}

void randBoard(elevator_t *elva) { //ž�� �� �������� �߰�
	int randomE = rand() % 6; // ž���� ���������͸� ����
	if (elva[randomE].inspect == 1) return;
	int randomM = rand() % 10; // ���� ���� �������� ����
	int randomW = rand() % 10; // ���� ���� �������� ����
	info_t nowGender;
	nowGender.man = elva[randomE].gender.man + randomM; // ���� ���������� ��ġ���� ž��
	nowGender.woman = elva[randomE].gender.woman + randomW; //���� ���������� ��ġ���� ž��

	if (admin == 2) { // admin�� 2�� �� ( ������ ����� �� )
		if (elva[randomE].floor>=0) { // ������������ ���� ���� ����� ��
			printf("\n%d�� ���������Ϳ��� %3d���� ���� %d��� ���� %d���� ž���߽��ϴ�.\n", randomE + 1, elva[randomE].floor, randomM, randomW);
		}
		else { // ������������ ���� ���� ������ ��
			printf("\n%d�� ���������Ϳ��� B%2d���� ���� %d��� ���� %d���� ž���߽��ϴ�.\n", randomE + 1, elva[randomE].floor * -1, randomM, randomW);
		}
		printf("%d�� ���������ʹ� ���� %dkg�Դϴ�.\n", randomE + 1, nowGender.man * MAN_KG + nowGender.woman * WOMAN_KG);
		Sleep(2000);
	}
	
	while(nowGender.man*MAN_KG + nowGender.woman*WOMAN_KG >= MAX_KG) { //�����϶�
		int flag = rand() % 2; // ���� ������ �������� ���� (0: ����, 1: ����)
		if (flag) { // flag�� 1�� �� ( ���ڶ�� )
			nowGender.man--; // nowGender.man�� ���� �ϳ� ����
			randomM--; // randomM�� ���� �ϳ� ����
			if (admin == 2) { // admin�� 2�� ��
				// ������ �� ���� ���ȴ� �� ���
				printf("���������� ���� �ο��� �ʰ��Ͽ� ���� 1���� �����Ͽ����ϴ�.\n");
				Sleep(1000);
			}
		}
		else { // flag�� 0�� ��
			nowGender.woman--; // nowGender.woman�� ���� �ϳ� ����
			randomW--; // randomW�� ���� �ϳ� ����
			if (admin == 2) { // admin�� 2�� ��
				// ������ �� ���� ���ȴ� �� ���
				printf("���������� ���� �ο��� �ʰ��Ͽ� ���� 1���� �����Ͽ����ϴ�.\n");
				Sleep(1000);
			}
		}
	}

	//�� while���� �����ٸ� ������ nowGender�� ���� ������ �ȶߵ��� ���������Ƿ� ���� ž���� �ο����� �߰�
	elva[randomE].gender.man = nowGender.man; // elva[randomE].gender.man �� nowGender.man�� ����
	elva[randomE].gender.woman = nowGender.woman; // elva[randomE].gender.woman �� nowGender.woman�� ����

	//���� ��ġ���� ž���� ������� ��� ������ ���� �������� ������ ����
	info_t now; // ���� ��ġ���� ž���� ������� �����ϴ� ����ü
	now.man = randomM; // randomM��ŭ now.man�� ����
	now.woman = randomW; // randomW��ŭ now.woman�� ����
	while (now.man > 0 || now.woman > 0) { // now.man �Ǵ� now.woman�� 0���� ū ����
		int nextFloor = rand() % 111; // now���� ����� ������ ���� �����ϴ� ������ �� 0~110 �������� ������ ����
		if (nextFloor == 10) { // 0���� �����Ƿ� continue
			continue;
		}
		info_t next = now; // now�� �� ����̳� nextFloor ���� ���� �� �����ϴ� ����ü
		if (now.man > 0) { // now.man�� 0���� Ŭ ��
			next.man = rand() % (now.man + 1); // next.man�� 0~now.man �� �������� ����
		}
		if (now.woman > 0) { // now.woman�� 0���� Ŭ ��
			next.woman = rand() % (now.woman + 1); // next.woman�� 0~now.woman �� �������� ����
		}
		elva[randomE].info[nextFloor].man += next.man; // randomE�� ������������ nextFloor������ �������� ���� ���� next.man��ŭ �߰�
		elva[randomE].info[nextFloor].woman += next.woman; // randomE�� ������������ nextFloor������ �������� ���� ���� next.woman��ŭ �߰�
		now.man -= next.man; // now.man���� next.man��ŭ ��
		now.woman -= next.woman; // now.woman���� next.woman��ŭ ��
	}
	
}

void checkHuman(elevator_t* elva,int now) { // ���� ���� ���� ����� �ִ��� Ȯ���ϴ� �Լ�
	
	int nowFloor = elva[now].floor+10; // -10 ~ -1 && 1 ~ 100  ->  0 ~9 && 11 ~ 110
	int nowM = elva[now].info[nowFloor].man; // nowM�� now�� ������������ nowFloor���� �������� ���� ���� ����
	int nowW = elva[now].info[nowFloor].woman; // nowW�� now�� ������������ nowFloor���� �������� ���� ���� ����

	if (nowM == 0 && nowW == 0) return; // nowM�� nowW�� ��� 0�̸� �ٷ� ����

	//������ �����Ƿ� ���� 0���� �ʱ�ȭ
	elva[now].info[nowFloor].man = 0; // now�� ������������ nowFloor���� �������� ���� ���� 0���� ����
	elva[now].info[nowFloor].woman = 0; // now�� ������������ nowFloor���� �������� ���� ���� 0���� ����

	elva[now].gender.man -= nowM; // now�� ������������ ž�� ���� ���� ���� nowM��ŭ ��
	elva[now].gender.woman -= nowW; // now�� ������������ ž�� ���� ���� ���� nowW��ŭ ��

	if (elva[now].gender.man == 0 && elva[now].gender.woman == 0) { // ž�� ���� ����� ���� 0�̸�
		elva[now].dir = -1; // ���������͸� ����
	}
	else { // ž�� ���� ����� �ִٸ�
		if (admin == 2) { // admin�� 2�� ��
			// �� �� ���ȴ� �� ����ؼ� ������
			printf("\n%d�� ���������Ϳ��� %d���� ���� %d��� ���� %d���� ���Ƚ��ϴ�.\n", now+1, nowFloor, nowM, nowW);
			Sleep(1000);
		}
	}

}

void checkDirection(elevator_t* elva,int now,int moveElva) { // ������������ ������ Ȯ���ϴ� �Լ�
	if ((elva[now].gender.man == 0 && elva[now].gender.woman == 0) && moveElva != now) { // ������������ ��� ���� 0�̸鼭 �θ� ���������Ϳ� �ٸ� ������������ ��
		elva[now].dir = -1; // ���������͸� ����
		return;
	}
	if (elva[now].dir == 1) { // ���������Ͱ� �ö󰡰� ���� ��
		int flag = 0; // �ö󰡾��ϴ� �� ���ƾ��ϴ� �� �Ǵ��ϴ� ����
		int nFloor = elva[now].floor + 10; // -10 ~ -1 && 1 ~ 100  ->  0 ~9 && 11 ~ 110 �̹Ƿ� ���� ���� + 10�� ���� �����ϴ� ����
		while (nFloor <= TOTAL_FLOOR) { // nFloor�� 110���� �۰ų� ���� ����
			if (elva[now].info[nFloor].man > 0 || elva[now].info[nFloor].woman > 0) { // ���� ���� ����� �ִٸ�
				flag = 1; // flag�� 1�� ���� ( ��� �ö󰡾��ϱ� ������ )
				break; // while���� ��������
			}
			nFloor++; // nFloor�� ���� �ϳ� ����
			if (nFloor == 10) { // 0���̹Ƿ� �ϳ� �߰��� ����
				nFloor++;
			}
		}
		if (!flag) { // flag�� 0�� ��
			elva[now].dir = 0; // ������������ ������ �ϰ����� �ٲ�
		}
	}
	else if (elva[now].dir == 0) { // ���������Ͱ� �������� ���� ��
		int flag = 0; // ���������ϴ� �� ���ƾ��ϴ� �� �Ǵ��ϴ� ����
		int nFloor = elva[now].floor+10; // -10 ~ -1 && 1 ~ 100 -> 0 ~ 9 && 11 ~ 110 �̹Ƿ� ���� ���� + 10�� ���� �����ϴ� ����
		while (nFloor >= 0) { // nFloor�� 0���� ũ�ų� ���� ����
			if (elva[now].info[nFloor].man > 0 || elva[now].info[nFloor].woman > 0) { // ���� ���� ����� �ִٸ�
				flag = 1; // flag�� 1�� ���� ( ��� ���������ϱ� ������ )
				break; // while���� ��������
			}
			nFloor--; //nFloor�� ���� �ϳ� ����
			if (nFloor == 10) { // 0���̹Ƿ� �ϳ� �߰��� ����
				nFloor--;
			}
		}
		if (!flag) { // flag�� 0�� ��
			elva[now].dir = 1; // ������������ ������ ������� �ٲ�
		}
	}
	else { // ���������Ͱ� �������� ��
		int flag = 0; // ���� ���⿡ ����� �ִ��� �Ǵ��ϴ� ����
		int nFloor = elva[now].floor + 10; // -10 ~ -1 && 1 ~ 100 -> 0 ~ 9 && 11 ~ 110 �̹Ƿ� ���� ���� + 10�� ���� �����ϴ� ����
		// ���� �ö󰡴� �������� ����� �ִ� �� Ȯ��
		while (nFloor <= TOTAL_FLOOR) { // nFloor�� 110���� �۰ų� ���� ����
			if (elva[now].info[nFloor].man > 0 || elva[now].info[nFloor].woman > 0) { // ���� ���� ����� �ִٸ�
				flag = 1; // flag�� 1�� ���� ( ��� ���������ϱ� ������ )
				break; // while���� ��������
			}
			nFloor++; // nFloor�� ���� �ϳ� ����
			if (nFloor == 10) { // 0���̹Ƿ� �ϳ� �߰��� ����
				nFloor++;
			}
		}
		if (flag) { // flag�� 1�� ��
			elva[now].dir = 1; // �ö󰡴� �������� ����� �ִٴ� �Ҹ��̹Ƿ� ������ 1�� �ٲ�
			return; // �Լ� ����
		}
		flag = 0; // flag�� 0���� �ʱ�ȭ
		nFloor = elva[now].floor + 10; // -10 ~ -1 && 1 ~ 100 -> 0 ~ 9 && 11 ~ 110 �̹Ƿ� ���� ���� + 10�� ���� �����ϴ� ����
		// �������� �������� ����� �ִ� �� Ȯ��
		while (nFloor >= 0) { // nFloor�� 0���� ũ�ų� ���� ����
			if (elva[now].info[nFloor].man > 0 || elva[now].info[nFloor].woman > 0) { // ���� ���� ����� �ִٸ�
				flag = 1; // flag�� 1�� ���� ( ��� ���������ϱ� ������ )
				break; // while���� ��������
			}
			nFloor--; //nFloor�� ���� �ϳ� ����
			if (nFloor == 10) { // 0���̹Ƿ� �ϳ� �߰��� ����
				nFloor--;
			}
		}
		if (flag) { // flag�� 1�� ��
			elva[now].dir = 0; // �ö󰡴� �������� ����� �ִٴ� �Ҹ��̹Ƿ� ������ 1�� �ٲ�
			return; // �Լ� ����
		}
	}
}

void autoMove(elevator_t* elva, int moveElva) { // ���������͸� �ڵ����� �����̰� �ϴ� �Լ�
	for (int i = 0; i < 6; i++) { // ���������Ͱ� 6�� �̹Ƿ� 0 ~ 5���� �ݺ�
		if (elva[i].inspect == 1) continue; // ���� ���̶�� continue
		if (elva[i].floor <= MAX_FLOOR && elva[i].floor >= MIN_FLOOR) { // ������������ ���� 100���� �۰ų� ���� -10���� ũ�ų� ���� ��
			if (elva[i].gender.man == 0 && elva[i].gender.woman == 0) { // ź ����� ���ٸ�
				elva[i].dir = -1; // ���������͸� ����
				continue;
			}
			if (moveElva != i) { // ����ڰ� ź ���������Ϳ� ���� Ȯ���� ���������Ͱ� �ٸ� ��
				if (elva[i].dir == 1 && elva[i].floor < MAX_FLOOR) { // �ö󰡸鼭 110���� ���� ��
					elva[i].floor++; // i�� ������������ ���� 1�� ����
					if (elva[i].floor == 0) { // 0���� ���� 1 �߰��� ����
						elva[i].floor++;
					}
				}
				else if (elva[i].dir == 0 && elva[i].floor > MIN_FLOOR) { // �������鼭 -10���� Ŭ ��
					elva[i].floor--; // i�� ������������ ���� 1�� ����
					if (elva[i].floor == 0) { // 0���� ���� 1 �߰��� ����
						elva[i].floor--;
					}
				}

				checkDirection(elva, i, moveElva); // ������ Ȯ���ϴ� �Լ��� ����
			}
			checkHuman(elva,i); // ���� ���� ���� ����� �ִ� �� Ȯ���ϴ� �Լ��� ����
		}
	}
}

int main()
{
	srand((unsigned int)time(NULL)); //������ ���� �õ尪���� �ð��� �����
	system("mode con cols=100 lines=30 | title ���������� ���α׷�");
	elevator_t elva[6] = { 0, }; //6�� �̹Ƿ� 6¥�� ����ü �迭 ����
	int upButton=0,downButton=0,nowFloor=1,loop=0,moveFlag = 0,moveElva=-1;
	//upButton : ���� ���ϴ� ��ư�� ���¸� �����ϴ� ���� (on: 1, off: 0)
	//downButton : �Ʒ��� ���ϴ� ��ư�� ���¸� �����ϴ� ���� (on: 1, off: 0)
	//nowFloor : ���� ������� ������ �����ϴ� ����
	//loop : ������ ������ ������ �����ϴ� ���� (���� �� : 1 , ���� X : 0)
	//moveFlag : ���������Ͱ� �������� �ȿ��������� �����ϴ� ���� (������: 1 , �ȿ�����: 0)
	//moveElva : �����̴� ������������ ��ȣ�� �����ϴ� ����
	int check = 0, nextFloor = 0, door = 0, gender = 0, help = 0, inspectCnt = 0;
	//check : ���������� ž�� ���θ� �����ϴ� ���� (ž��: 1 , ž�°ź�: 0)
	//nextFloor : ������ �ϴ� ���� �����ϴ� ����
	//door : ������������ ���� ������ ������ �����ϴ� ���� (����: 1 , ���� ����: 0)
	//gender : ������� ������ �����ϴ� ���� (����: 1 , ����: 0)
	//help : ���� Ȱ��ȭ ���θ� ��Ÿ���� ���� (Ȱ��ȭ: 1 , ��Ȱ��ȭ: 0)
	//admin : ������ ��� Ȱ��ȭ ���θ� ��Ÿ���� ���� (Ȱ��ȭ: 1 , ��Ȱ��ȭ: 0)
	//inspectCnt : ���� ���� ������������ ������ �����ϴ� ����
	int period = periodValue;
	// period : ������������ ����� ž���ϴ� �ֱ⸦ ��Ÿ���� ����
	char correctPw[100] = "1234567*"; //�����ڸ�� ��й�ȣ


	init(elva); //�ʱ�ȭ

	//Call by reference�� ���� �Լ��� �Ű������� ��κ� ������ ������
	do {
		if (loop == 0) { //�� ó�� ����Ǿ��� ��
			enter(&loop, &gender); //�� ó�� ����Ǿ��� �� �ʿ��� ������ �ޱ� ���� �Լ�
			system("cls"); // ȭ���� ����
		}
		else {
			if (_kbhit()) { // Ű�� �������ִٸ�
				char key = _getch(); // key �Է�
				if (key == -32) { // key�� -32�� ��
					key = _getch(); // key �Է�
				}
				else if (key == 0) { // key�� 0�� ��
					key = _getch(); // key �Է�
				}

				if (key == UP && !moveFlag) { //���� ����Ű�� ������ ��
					upButton = !upButton; // upButton�� ���� �ݴ�� �ٲ�
					moveElva = -1; // moveElva ���� -1�� ����
				}
				else if (key == DOWN && !moveFlag) { //�Ʒ��� ����Ű�� ������ ��
					downButton = !downButton; // downButton�� ���� �ݴ�� �ٲ�
					moveElva = -1; // moveElva ���� -1�� ����
				}
				else if (key == F1) { //F1�� ������ ��
					help = 1; // help�� ���� 1�� ����
				}
				else if (key == F2) { //F2�� ������ ��
					admin = !admin; // admin�� ���� �ݴ�� �ٲ�
				}
				else if (key == F3 && admin == 2) { //������ ��� ���¿��� F3�� ���� ��
					behaviorAdmin(elva,correctPw,&inspectCnt); // ������ ��� ��ɾ���� ���� 
				}
				else if (key == ESC) { //ESC�� ������ ��
					loop = 0; // loop�� 0���� ����
				}
			}
			
			if (help) {
				helpMessage(); // ���� UI ��� �Լ� ����
				system("cls"); // ȭ���� ����
				help = 0; // help�� 0���� ����
				continue; // continue
			}

			if (admin==1) { // admin�� 1�� ��
				behaviorAdmin(elva, correctPw, &inspectCnt); // ������ ��� �� UI ��� �Լ� ����
				system("cls"); // ȭ���� ����
				continue; // continue
			}
			else {
				userPrint(elva, moveElva, upButton, downButton, nowFloor); //�⺻ UI ��� �Լ� ����
			}

			if (moveFlag) { // moveFlag�� 1�� ��
				elevatorMove(elva, &moveFlag, &moveElva, &nowFloor, &nextFloor, gender); // ����ڰ� ���������Ϳ� ���� �� �����̴� �Լ� ����
			}
			else { // moveFlag�� 0�� ��
				checkElevatorLoc(elva, &upButton, &downButton, &nowFloor, &moveElva, &moveFlag, &nextFloor, gender); // ���������Ͱ� �������� ���� ������ �Է��ϴ� �Լ� ����
			}

			autoMove(elva,moveElva); // �ڵ����� �����̴� �Լ� ���� 

			if (period <= 0) { // period�� 0���� �۰ų� ���� ��
				randBoard(elva); // ž���ϴ� �Լ��� ����
				period = periodValue; // period�� �ٽ� PERIOD_VALUE�� ����
			}
			period--; // period�� ���� 1�� ����
			Sleep(500); //0.5�ʵ��� ���α׷��� ����
			system("cls"); //cmdȭ�鿡 ���� ��� ����� ������
		}
	} while (loop == 1); // loop���� 1�� ���� ��� �ݺ�
	printf("���������� ���α׷��� �����մϴ�.\n");
	return 0;
}