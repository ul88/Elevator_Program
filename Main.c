//랜덤으로 들어가야할 것. 남자의 수와 여자의 수 항상 랜덤하게 들어감.
//사용자가 엘리베이터에 탈 경우에만 최대 수용량을 넘게 랜덤 수가 나올 수 있음
//탑승객이 0명이 될 때까지 엘리베이터는 한 방향으로만 진행됨
#include<stdio.h> //표준 입출력 헤더 파일
#include<time.h> //랜덤 수의 시드를 위한 헤더 파일
#include<math.h> //절댓값 함수를 위한 헤더 파일
#include<stdlib.h> //Sleep 함수와 랜덤 함수를 위한 헤더 파일
#include<windows.h> //system 함수를 사용하기 위한 헤더 파일
#include<conio.h> //키보드 입력을 위한 헤더 파일
#define UP 72 //위쪽 방향키의 아스키 코드는 224 72 이므로 72를 전처리
#define DOWN 80 //아래쪽 방향키의 아스키 코드는 224 80 이므로 80을 전처리
#define RIGHT 77 //오른쪽 방향키의 아스키 코드는 224 77 이므로 77을 전처리
#define LEFT 75 //왼쪽 방향키의 아스키 코드는 224 75 이므로 75를 전처리
#define ESC 27 //ESC의 아스키 코드는 27이므로 27를 전처리
#define F1 59 //F1의 아스키 코드는 0 59이므로 59를 전처리
#define F2 60 //F2의 아스키 코드는 0 60이므로 60을 전처리
#define F3 61 //F3의 아스키 코드는 0 61이므로 61을 전처리
#define TOTAL_FLOOR 110 //지상 100층 + 지하 10층 = 110 이므로 110으로 지정
#define MAX_FLOOR 100 //지상은 총 100층까지 있으므로 100을 최대값으로 지정
#define MIN_FLOOR -10//지하는 총 10층까지 있으므로 -10을 최소값으로 지정
#define MAX_KG 1450 //최대 탑승 인원은 1450kg이므로 1450으로 지정
#define MAN_KG 75 //남자의 몸무게는 75kg이므로 75로 지정
#define WOMAN_KG 55 //여자의 몸무게는 55kg이므로 55로 지정

typedef struct infoHuman{ //엘리베이터 각 층마다 내려야하는 사람의 수를 나타냄
	int man; // 남자의 수
	int woman; // 여자의 수
}info_t;

typedef struct elevator { //배열만으로는 필요한 데이터를 다 저장하기에는 복잡하므로 구조체 생성
	int floor; //층 수
	info_t gender; // 해당 엘리베이터의 남자의 수와 여자의 수
	int dir; //방향 (위: 1 , 아래: 0 , 정지: -1)
	int inspect; //엘리베이터 점검 여부를 나타냄 (점검 중: 1, 운행 중: 0)
	info_t info[TOTAL_FLOOR+1]; // 내릴 사람의 위치를 저장하기 위해 층수를 기준으로 배열을 추가로 생성
}elevator_t;


void init(elevator_t* elva) { //초기화
	for (int i = 0; i < 6; i++) {
		int randomF = rand() % 110; // 층 수를 랜덤으로 초기화
		int randomD = rand() % 2; // 방향을 랜덤으로 초기화
		int randomM = rand() % (MAX_KG/MAN_KG); // 남자 수를 랜덤으로 초기화
		int randomW = rand() % ((MAX_KG - randomM*MAN_KG)/WOMAN_KG); // 여자 수를 랜덤으로 초기화
		elva[i].floor = randomF - 10; // 음수는 지하, 양수는 지상
		if (elva[i].floor >= 0) { // 0은 없는 층이기 때문에 0을 지우기 위한 조건
			elva[i].floor++; // 0~109까지는 전부 1을 더해 floor에는 -10 ~ -1 && 1 ~ 110까지 들어가 있음
		}
		elva[i].dir = randomD; //방향을 dir에 저장
		elva[i].gender.man = randomM; // 남자의 수를 저장
		elva[i].gender.woman = randomW; // 여자의 수를 저장
		elva[i].inspect = 0; // 점검 여부
		
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

void userPrint(elevator_t* elva, int elvaIdx,int upButton, int downButton, int nowFloor) { //엘리베이터 출력
	printf("위쪽 방향키를 누르면 상층으로 향하는 엘리베이터를 호출할 수 있습니다.\n");
	printf("아래쪽 방향키를 누르면 하층으로 향하는 엘리베이터를 호출할 수 있습니다.\n");
	printf("ESC를 누르면 프로그램을 종료할 수 있습니다.\n");
	printf("F1을 누르면 도움말이 나옵니다.\n");
	//중첩반복문을 사용하지만 2차원 배열이 아닌 1차원 배열이기 때문에 [j + i * 3]을 사용했다.
	for (int i = 0; i < 2; i++) { // 엘리베이터의 배치가 2행 3열이기 때문에 두번 반복

		//엘리베이터의 번호
		for (int j = 0; j < 3; j++) { // 엘리베이터의 배치가 2행 3열이기 때문에 세번 반복
			printf("==== %d ====", (j + 1) + (i * 3)); 
			if (elvaIdx == j + (i * 3)) { // 사용자에게로 가는 엘리베이터인지
										// 사용자에게 가는 엘리베이터라면 색칠된 원 아니라면 빈 원으로 나타난다.
				printf("●\t");
			}
			else {
				printf("○\t");
			}
		}
		printf("\n");

		//점검 여부 또는 만원 여부
		//점검이 진행 중이라면 만원이었어도 뜰 필요가 없으므로 한 묶음으로 처리
		for (int j = 0; j < 3; j++) { // 엘리베이터의 배치가 2행 3열이기 때문에 세번 반복
			if (elva[j + (i * 3)].inspect == 1) { //점검 여부 확인
				printf("   점검중\t");
			}
			else if (elva[j + (i * 3)].gender.man * MAN_KG + 
					elva[j + (i * 3)].gender.woman * WOMAN_KG >= MAX_KG - WOMAN_KG) { // 만원 여부 확인
				printf("   만 원\t");
			}
			else {
				printf("\t\t");
			}
		}
		printf("\n");

		//층수
		for (int j = 0; j < 3; j++) { // 엘리베이터의 배치가 2행 3열이기 때문에 세번 반복
			if (elva[j + (i * 3)].floor >= 0) { // 지하층이면 -가 아닌 B가 나오게 하기 위한 조건
				printf("   -%3d-   ", elva[j + (i * 3)].floor);
			}
			else {
				printf("   -B%2d-   ", elva[j + (i * 3)].floor * -1);
			}
			if (elva[j + (i * 3)].dir == -1) {
				printf("△\t");
			}
			else if (elva[j + (i * 3)].dir == 1) { // 현재 엘리베이터가 올라가는 지
				printf("▲\t");
			}
			else {
				printf("△\t");
			}
		}
		printf("\n");

		//남자의 수
		for (int j = 0; j < 3; j++) { // 엘리베이터의 배치가 2행 3열이기 때문에 세번 반복
			printf("남자 : %2d  ", elva[j + (i * 3)].gender.man);
			if (elva[j + (i * 3)].dir == -1) {
				printf("▽\t");
			}
			else if (elva[j + (i * 3)].dir == 1) { // 현재 엘리베이터가 내려가는 지
				printf("▽\t");
			}
			else {
				printf("▼\t");
			}
		}
		printf("\n");

		//여자의 수
		for (int j = 0; j < 3; j++) { // 엘리베이터의 배치가 2행 3열이기 때문에 세번 반복
			printf("여자 : %2d \t", elva[j + (i * 3)].gender.woman);
		}
		printf("\n");

		//총 킬로
		for (int j = 0; j < 3; j++) { // 엘리베이터의 배치가 2행 3열이기 때문에 세번 반복
			int kg = elva[j + (i * 3)].gender.man * MAN_KG + elva[j + (i * 3)].gender.woman * WOMAN_KG; // kg 계산
			printf("  %4dkg \t", kg);
		}

		printf("\n============    ============    ============");
		
		// 중간에 현재 층에서 어느 버튼을 눌렀는 지를 나타내줌
		if (i == 0) { // 첫줄이 끝나고만 실행이 되야 하기 때문
			printf("\n\n");
			if (nowFloor > 0) {
				printf("\t현재 층수 : %3d", nowFloor);
			}
			else {
				printf("\t현재 층수 : B%2d", nowFloor*-1);
			}
			if (upButton) {
				printf("    ▲\n\t\t");
			}
			else {
				printf("    △\n\t\t");
			}
			if (downButton) {
				printf("           ▼");
			}
			else {
				printf("           ▽");
			}
			printf("\n\n");
		}
	}

}

int strLen(char* str) { // 문자열 길이 확인 함수
	int len = -1; // 전위 연산자를 사용했기 때문에 0이 아닌 -1로 초기화
	while (str[++len] != '\0'); // NULL문자를 만날 때까지 len의 값을 1씩 증가하면서 확인
	return len; //len 리턴
}  

int checkStr(char* str, char* comStr) { //문자열 비교 함수
	if (strLen(str) != strLen(comStr)) return 0; // 문자열의 길이가 서로 다르다면 비교할 필요가 없으므로 바로 0을 리턴
	for (int i = 0; i < strLen(str); i++) { // 현재 문자열의 길이만큼 반복함
		if (str[i] != comStr[i]) { // 서로 다르다면 바로 0을 리턴
			return 0;
		}
	}
	return 1; //반복문이 제대로 끝났다면 서로 같다는 소리이므로 1을 리턴
}

void changeStr(char* str, char* comStr) { // 문자열 교환 함수
	for (int i = 0; i <= strLen(str); i++) { // str의 길이만큼 반복
		comStr[i] = str[i]; // comStr에 str값들을 저장
	}
}

void password(int* admin, char* correctPw) { // 비밀번호 입력 함수
	int pwCnt = 5; 
	char pw[100] = "";
	char changePw[10] = "change";
	int flag = 0;
	printf("관리자 모드입니다.\n");
	printf("비밀번호를 입력해주세요. (8자리이상, 20자리이하만 가능합니다.)\n");
	printf("비밀번호 변경을 원한다면 \"%s\"을 입력해주세요.\n", changePw);
	scanf_s("%s", pw, 50);

	if (checkStr(pw, changePw)) {
		printf("비밀번호를 변경합니다.\n");
		printf("기존의 비밀번호를 입력해주세요.\n");
		scanf_s("%s", pw, 50);

		if (checkStr(pw, correctPw)) {
			printf("변경하고 싶은 비밀번호를 입력해주세요. (8자리이상, 20자리이하만 가능합니다.)\n");
			scanf_s("%s", pw, 50);
			if (strLen(pw) >= 8 && strLen(pw) <= 20) {
				changeStr(pw, correctPw);
				printf("%s로 비밀번호를 변경합니다.\n", correctPw);
			}
			else {
				printf("8자리이상, 20자리 이하의 비밀번호를 입력해주세요.\n");
			}
		}
		else {
			printf("비밀번호가 틀립니다.\n");
		}

		printf("1초 후 관리자 모드가 재시작됩니다.\n");
		Sleep(1000);
		return;
	}

	while (!checkStr(pw, correctPw) && pwCnt > 0 && strLen(pw) != 1) {
		system("cls");
		printf("비밀번호가 틀렸습니다.\n");
		printf("- %d번 남았습니다. -\n", pwCnt);
		printf("다시 입력해주세요. (하나만 입력하면 관리자 모드가 바로 종료됩니다.)\n");
		scanf_s("%s", pw, 50);
		pwCnt--;
	}
	if (!checkStr(pw, correctPw)) {
		*admin = 0;
		printf("1초 후 관리자 모드를 종료합니다.\n");
		Sleep(1000);
		return;
	}
	else {
		*admin = 2;
		printf("1초 후 관리자 모드를 시작합니다.\n");
		Sleep(1000);
	}
}

void adminPrint(elevator_t* elva, int *admin, char *correctPw, int *inspectCnt) { //관리자 모드 함수
	if (*admin == 1) {
		password(admin, correctPw); // 비밀번호 입력 함수
	}
	else {
		//해당 엘리베이터에서 어느 층에 누가 내리는 지
		//해당 엘리베이터가 어떤 층을 방문했을 때 몇 명의 사람이 탑승하는 지
		//엘리베이터 점검을 시킬 수 있어야함 (2대까지)
		//for (int i = 0; i < 6; i++) {
		//	int nowF = elva[i].floor + 10; //-10~100까지 0은 제외 상층은 0~99 하층은 100~109
		//	int nowM = elva[i].info[nowF].man;
		//	int nowW = elva[i].info[nowF].woman;
		//	
		//	printf("%d번 엘리베이터\n", i + 1);
		//	printf("층 : %d\n", elva[i].floor);
		//	printf("내려야하는 사람\n");
		//	printf("남자 : %d\n", nowM);
		//	printf("여자 : %d\n\n", nowW);

		//}
		for (int i = 0; i < 110; i++) {
			printf("%d층 : %d %d\n",i, elva[2].info[i].man, elva[2].info[i].woman);
		}
	}
}

void enter(int *loop,int *gender) { // 입장 여부를 판별하기 위한 함수
	printf("성별이 어떻게 되나요?(남성: 1 , 여성: 0)\n");
	scanf_s("%d", gender);
	printf("엘리베이터에 입장하시겠습니까?\n");
	printf("입장을 하신 후에도 ESC를 누르면 언제든 나가실 수 있습니다.\n");
	printf("입장을 희망한다면 1, 입장을 희망하지 않는다면 0을 입력해주세요.\n");
	scanf_s("%d", loop);
}

void helpMessage() { // 도움말 출력 함수
	int flag = 0;
	system("cls");
	printf("엘리베이터를 부르는 방법은 위쪽 방향키 또는 아래쪽 방향키를 누르면 됩니다.\n");
	printf("ESC를 누르면 프로그램이 종료됩니다.\n");
	printf("F2를 누르면 관리자 모드를 실행할 수 있습니다.\n");
	printf("F2를 다시 누르면 관리자 모드가 종료됩니다.\n");
	printf("도움말을 종료하시겠습니까? (예: 1 , 아니요: 0)\n");
	scanf_s("%d", &flag);
	if (flag == 1) { // 도움말을 종료하기로 했다면
		return; // 리턴
	}
	else {
		helpMessage(); // 1을 입력할 때까지 재귀
	}
}

int checkSpeed(elevator_t *elva,int start,int upButton, int downButton) { // 가장 빨리 도착할 수 있는 엘리베이터를 찾고 그 엘리베이터 번호를 반환하기 위해 int형 함수로 생성
	int min = TOTAL_FLOOR + 1; // 가장 최적의 엘리베이터에 대한 정보들을 담을 변수
	// 최소값의 층수는 무조건 최대 층수보다 커야하기 때문에 (최대 층수 + 1)을 해줌
	int minIdx= -1; // 가장 최적의 엘리베이터 번호를 저장할 변수
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
	int *moveElva,int *moveFlag,int * nextFloor,int gender) { //엘리베이터를 탑승한 직후 필요한 정보들을 물어보는 함수
	int check, door;
	if (*upButton || *downButton) {
		*moveElva = checkSpeed(elva, *nowFloor, *upButton, *downButton);
		
		if (elva[*moveElva].floor == *nowFloor) {
			printf("\n%d번 엘리베이터가 도착했습니다.\n", *moveElva + 1);
			printf("타시겠습니까? (예: 1 , 아니요: 0)\n");
			scanf_s("%d", &check);
			if (check) {
				printf("엘리베이터에 탑승하셨습니다.\n");
				printf("3초 후 문이 자동으로 닫힙니다.\n");
				printf("문을 닫고 싶다면 왼쪽 방향키를\n");
				printf("문을 열고 싶다면 오른쪽 방향키를 눌러주세요.\n");
				int sec=4;
				while (--sec) {
					if (_kbhit()) {
						char key = _getch();
						if (key == -32) {
							key = _getch();
						}

						if (key == LEFT) {
							printf("1초 후 문이 닫힙니다.\n");
							Sleep(1000);
							break;
						}
						else if (key == RIGHT) {
							printf("문이 열립니다.\n");
							sec = 3;
						}
					}
					printf("%d초...\n", sec);
					Sleep(1000);
				}
				printf("문이 닫힙니다.\n");
				printf("어느 층으로 가시겠습니까?(%d ~ %d까지의 층만 존재합니다.)\n", MIN_FLOOR, MAX_FLOOR);
				scanf_s("%d", nextFloor);
				while (*nextFloor == *nowFloor || (*upButton && *nextFloor < *nowFloor && elva[*moveElva].dir==1) ||
					(*downButton && *nextFloor > *nowFloor && elva[*moveElva].dir==0) ||
					(*nextFloor < MIN_FLOOR || *nextFloor > MAX_FLOOR)) {
					if (*nextFloor == 0) {
						printf("0층은 존재하지 않습니다.\n");
					}
					else if (*nextFloor < MIN_FLOOR || *nextFloor > MAX_FLOOR) {
						printf("%d ~ %d까지의 층만 존재합니다.\n", MIN_FLOOR, MAX_FLOOR);
					}
					else if (*nextFloor == *nowFloor) {
						printf("현재 있으신 층과 동일한 층을 입력하셨습니다.\n");
					}
					else if (*upButton && *nextFloor < *nowFloor) {
						printf("현재 층 수보다 높은 층만 입력 가능합니다.\n");
					}
					else {
						printf("현재 층 수보다 낮은 층만 입력 가능합니다.\n");
					}
					printf("다시 입력해주세요.\n");
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
				printf("엘리베이터에 탑승하지 않으셨습니다.\n");
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
	int *nextFloor,int gender) { //탑승한 엘리베이터의 이동을 담당하는 함수
	if (elva[*moveElva].floor == *nextFloor) {
		if (*nowFloor > 0) {
			printf("\n%d층에 도착했습니다.\n", *nowFloor);
		}
		else {
			printf("\nB%d층에 도착했습니다.\n", *nowFloor * -1);
		}
		Sleep(1000);
		printf("문이 열립니다.\n");
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

void randBoard(elevator_t *elva) { //탑승 수 랜덤으로 추가
	int randomE = rand() % 6; // 탑승할 엘리베이터를 지정
	int randomM = rand() % 10; // 남자 수를 랜덤으로 지정
	int randomW = rand() % 10; // 여자 수를 랜덤으로 지정
	info_t nowGender;
	nowGender.man = elva[randomE].gender.man + randomM; // 현재 엘리베이터 위치에서 탑승
	nowGender.woman = elva[randomE].gender.woman + randomW; //현재 엘리베이터 위치에서 탑승

	while(nowGender.man*MAN_KG + nowGender.woman*WOMAN_KG >= MAX_KG) { //만원일때
		int flag = rand() % 2; // 내릴 성별을 랜덤으로 지정 (0: 여자, 1: 남자)
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

	//현재 위치에서 탑승한 사람들이 어느 층으로 가고 싶은지를 난수로 받음
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
	//	printf("\n%d번 엘리베이터에서 남자: %d명 여자: %d명이 내렸습니다.\n",now+1,nowM,nowW);
	//	Sleep(500); //0.5초동안 프로그램을 멈춤
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
	srand(time(NULL)); //랜덤을 위한 시드값으로 시간을 사용함
	elevator_t elva[6] = { 0, }; //6대 이므로 6짜리 구조체 배열 생성
	int upButton=0,downButton=0,nowFloor=1,loop=0,moveFlag = 0,moveElva=-1;
	//upButton : 위로 향하는 버튼의 상태를 저장하는 변수 (on: 1, off: 0)
	//downButton : 아래로 향하는 버튼의 상태를 저장하는 변수 (on: 1, off: 0)
	//nowFloor : 현재 사용자의 층수를 저장하는 변수
	//loop : 루프를 돌릴지 말지를 저장하는 변수 (루프 ○ : 1 , 루프 X : 0)
	//moveFlag : 엘리베이터가 움직일지 안움직일지를 저장하는 변수 (움직임: 1 , 안움직임: 0)
	//moveElva : 움직이는 엘리베이터의 번호를 저장하는 변수
	int check = 0, nextFloor = 0, door = 0, gender = 0, help = 0, admin = 0, inspectCnt = 0;
	//check : 엘리베이터 탑승 여부를 저장하는 변수 (탑승: 1 , 탑승거부: 0)
	//nextFloor : 가고자 하는 층을 저장하는 변수
	//door : 엘리베이터의 문을 닫을지 말지를 저장하는 변수 (닫음: 1 , 닫지 않음: 0)
	//gender : 사용자의 성별을 저장하는 변수 (남성: 1 , 여성: 0)
	//help : 도움말 활성화 여부를 나타내는 변수 (활성화: 1 , 비활성화: 0)
	//admin : 관리자 모드 활성화 여부를 나타내는 변수 (활성화: 1 , 비활성화: 0)
	//inspectCnt : 점검 중인 엘리베이터의 개수를 저장하는 변수
	int period = 0;
	char correctPw[100] = "1234567*"; //관리자모드 비밀번호


	init(elva); //초기화

	//Call by reference를 위해 함수의 매개변수는 대부분 포인터 변수임
	do {
		if (loop == 0) { //맨 처음 실행되었을 때
			enter(&loop, &gender); //맨 처음 실행되었을 때 필요한 정보를 받기 위한 함수
		}
		else {
			if (_kbhit()) { //키가 눌러져있다면
				char key = _getch(); //키 입력
				if (key == -32) {
					key = _getch();
				}
				else if (key == 0) {
					key = _getch();
				}

				if (key == UP && !moveFlag) { //위쪽 방향키를 눌렀을 때
					upButton = !upButton;
					moveElva = -1;
				}
				else if (key == DOWN && !moveFlag) { //아래쪽 방향키를 눌렀을 때
					downButton = !downButton;
					moveElva = -1;
				}
				else if (key == F1) { //F1을 눌렀을 때
					help = 1;
				}
				else if (key == F2) { //F2를 눌렀을 때
					admin = !admin;
				}
				else if (key == F3 && admin == 2) { //관리자 모드 상태에서 F3을 누를 시

				}
				else if (key == ESC) { //ESC를 눌렀을 때
					loop = 0;
				}
			}
			
			if (help) {
				helpMessage(); // 도움말 UI 출력 함수
				help = 0;
				continue;
			}

			if (admin) {
				adminPrint(elva, &admin, correctPw, &inspectCnt); // 관리자 모드 용 UI 출력 함수
			}
			else {
				userPrint(elva, moveElva, upButton, downButton, nowFloor); //기본 UI 출력 함수
			}

			if (moveFlag) { //엘리베이터 탑승
				elevatorMove(elva, &moveFlag, &moveElva, &nowFloor, &nextFloor, gender);
			}
			else { //엘리베이터가 도착했을 때의 정보 입력
				checkElevatorLoc(elva, &upButton, &downButton, &nowFloor, &moveElva, &moveFlag, &nextFloor, gender);
			}

			autoMove(elva,moveElva);

			if (period == 30) {
				randBoard(elva);
				period = 0;
			}
			period++;
			Sleep(500); //0.5초동안 프로그램을 멈춤
			system("cls"); //cmd화면에 이전 출력 결과를 지워줌
		}
	} while (loop == 1);
	printf("엘리베이터 프로그램을 종료합니다.\n");
	return 0;
}