//랜덤으로 들어가야할 것. 남자의 수와 여자의 수 항상 랜덤하게 들어감.
//사용자가 엘리베이터에 탈 경우에만 최대 수용량을 넘게 랜덤 수가 나올 수 있음
//탑승객이 0명이 될 때까지 엘리베이터는 계속 움직임
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
//#define PERIOD_VALUE 30 // 사람이 타는 주기가 15초 이므로 30으로 지정

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

int admin = 0, periodValue = 30; 
// admin : 관리자 권한에 대한 변수이므로 전역변수 처리
// periodValue : 사람이 타는 주기가 15초이므로 30으로 초기화하고 전역변수로 생성

void CursorView() // cmd 크기를 조절하는 함수
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //커서 굵기 (1 ~ 100)
	cursorInfo.bVisible = FALSE; //커서 Visible TRUE(보임) FALSE(숨김)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void init(elevator_t* elva) { //초기화
	CursorView(); // cmd 크기를 변경하는 함수 실행
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
		
		int nowMan = elva[i].gender.man; // 현재 남자의 수
		int nowWoman = elva[i].gender.woman; // 현재 여자의 수
		while (nowMan != 0 || nowWoman != 0) { // 둘 다 0이 될 때까지 반복
			int nextFloor = rand() % 111; // nextMan과 nextWoman 변수들이 도착할 층수를 랜덤으로 지정
			if (nextFloor == 10) { // nextFloor == 10이면 0층이라는 소리이므로 continue 해줌
				continue;
			}
			int nextMan = rand() % (nowMan + 1); // nowMan에서 랜덤으로 남자의 수를 뽑음
			int nextWoman = rand() % (nowWoman + 1); // nowWoman에서 랜덤으로 여자의 수를 뽑음
			elva[i].info[nextFloor].man += nextMan; // nextFloor층에 내려야하므로 info[nextFloor] 위치에 nextMan을 더함
			elva[i].info[nextFloor].woman += nextWoman; // nextFloor층에 내려야하므로 info[nextFloor] 위치에 nextWoman을 더함
			nowMan -= nextMan; // nextMan만큼 어느 층에 갈 지 정해줬으므로 nextMan만큼 nowMan에서 빼줌
			nowWoman -= nextWoman; // nextWoman만큼 어느 층에 갈 지 정해줬으므로 nextWoman만큼 nowWoman에서 빼줌
		}
	}
}

void userPrint(elevator_t* elva, int elvaIdx,int upButton, int downButton, int nowFloor) { //엘리베이터 출력
	if (admin == 2) { // admin == 2 일때 관련 출력을 함
		printf("\t**관리자 모드입니다.**\n");
		printf("F3을 누르면 관리자 모드 전용 명령어를 사용할 수 있습니다.\n");
	}
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
			if (elva[j + (i * 3)].dir == -1) { // 현재 엘리베이터가 멈췄는 지
				printf("△\t");
			}
			else if (elva[j + (i * 3)].dir == 1) { // 현재 엘리베이터가 올라가는 지
				printf("▲\t");
			}
			else { // 현재 엘리베이터가 내려가는 지
				printf("△\t");
			}
		}
		printf("\n");

		//남자의 수
		for (int j = 0; j < 3; j++) { // 엘리베이터의 배치가 2행 3열이기 때문에 세번 반복
			printf("남자 : %2d  ", elva[j + (i * 3)].gender.man);
			if (elva[j + (i * 3)].dir == -1) { // 현재 엘리베이터가 멈췄는 지
				printf("▽\t");
			}
			else if (elva[j + (i * 3)].dir == 1) { // 현재 엘리베이터가 올라가는 지
				printf("▽\t");
			}
			else { // 현재 엘리베이터가 내려가는 지
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
			if (nowFloor > 0) { // 지상 층일때
				printf("\t현재 층수 : %3d", nowFloor);
			}
			else { // 지하 층일때
				printf("\t현재 층수 : B%2d", nowFloor*-1);
			}
			if (upButton) { // 위로 가는 버튼을 눌렀을 때
				printf("    ▲\n\t\t");
			}
			else { // 아래로 가는 버튼을 눌렀을 때
				printf("    △\n\t\t");
			}
			if (downButton) { // 아래로 가는 버튼을 눌렀을 때
				printf("           ▼");
			}
			else { // 위로 가는 버튼을 눌렀을 때
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

void password(char* correctPw) { // 비밀번호 입력 함수
	int pwCnt = 5;  // 틀렸을 때 주는 기회 수를 저장하는 변수
	char pw[100] = ""; // 입력받는 비밀번호를 저장할 변수
	char changePw[10] = "change"; // 비밀번호 변경할 때 사용할 명령어를 저장하는 변수
	//int flag = 0;
	printf("관리자 모드입니다.\n");
	printf("비밀번호를 입력해주세요. (8자리이상, 20자리이하만 가능합니다.)\n");
	printf("비밀번호 변경을 원한다면 \"%s\"을 입력해주세요.\n", changePw);
	scanf_s("%s", pw, 50); // 비밀번호 입력

	if (checkStr(pw, changePw)) { // pw와 changePw가 서로 동일한지 확인
		printf("비밀번호를 변경합니다.\n");
		printf("기존의 비밀번호를 입력해주세요.\n");
		scanf_s("%s", pw, 50); // 비밀번호 입력

		if (checkStr(pw, correctPw)) { // pw와 correctPw가 서로 동일한지 확인
			printf("변경하고 싶은 비밀번호를 입력해주세요. (8자리이상, 20자리이하만 가능합니다.)\n");
			scanf_s("%s", pw, 50); // 비밀번호 입력
			if (strLen(pw) >= 8 && strLen(pw) <= 20) { // pw의 길이가 8이상 20 이하라면
				changeStr(pw, correctPw);
				printf("%s로 비밀번호를 변경합니다.\n", correctPw);
			}
			else { // pw의 길이가 8이상 20이하가 아니라면
				printf("8자리이상, 20자리 이하의 비밀번호를 입력해주세요.\n");
			}
		}
		else { // pw와 correctPw가 서로 다르다면
			printf("비밀번호가 틀립니다.\n");
		}

		printf("1초 후 관리자 모드가 재시작됩니다.\n");
		Sleep(1000);
		return;
	}

	while (!checkStr(pw, correctPw) && pwCnt > 0 && strLen(pw) != 1) { // pw와 correctPw가 서로 다르면서 pwCnt가 0보다 크면서 pw의 길이가 1이 아닌 동안 반복
		system("cls");
		printf("비밀번호가 틀렸습니다.\n");
		printf("- %d번 남았습니다. -\n", pwCnt);
		printf("다시 입력해주세요. (하나만 입력하면 관리자 모드가 바로 종료됩니다.)\n");
		scanf_s("%s", pw, 50); // 비밀번호 입력
		pwCnt--; // pwCnt의 값을 하나 줄임 (초기값 : 5)
	}
	if (!checkStr(pw, correctPw)) { // 마지막까지도 pw와 correctPw가 다르다면
		admin = 0; // 관리자모드를 종료함
		printf("1초 후 관리자 모드를 종료합니다.\n");
		Sleep(1000);
		return;
	}
	else { // pw와 correctPw가 같다면
		admin = 2; // 관리자모드를 실행함
		printf("1초 후 관리자 모드를 시작합니다.\n");
		Sleep(1000);
	}
}

void behaviorAdmin(elevator_t* elva, char *correctPw, int *inspectCnt) { //관리자 모드 함수
	if (admin == 1) {
		password(correctPw); // 비밀번호 입력 함수
	}
	else {
		//admin == 2일때 F3을 눌렀다면 관리자 전용 명령어를 사용할 수 있도록 함
		int flag = 0; // 명령어를 선택하는 변수

		printf("실행할 명령어를 선택하세요.\n");
		printf("0. 명령어 선택 종료\n");
		printf("1. 엘리베이터 점검\n");
		printf("2. 엘리베이터가 가야할 층 나열\n");
		printf("3. 엘리베이터에 사람이 타는 주기 수정\n");
		scanf_s("%d",&flag); // flag 입력
		if (flag == 0) { // flag가 0이면 함수 종료
			return;
		}
		else if (flag == 1) { // flag가 1이면
			int yes = 0; // 엘리베이터 점검을 할지 말지를 저장하는 변수
			printf("엘리베이터 점검을 하시겠습니까? (현재 점검 중인 엘리베이터 : %d 개) (예: 1, 아니오: 0)\n", *inspectCnt);
			scanf_s("%d",&yes); // yes 입력
			if (yes == 1) { // yes가 1이라면
				int cnt = 0; // 점검할 대수를 저장하는 변수
				printf("점검할 대수를 선택하세요. (최대 2개)\n");
				scanf_s("%d", &cnt); // cnt 입력
				while (cnt > 2 || cnt < 0) { //cnt가 2 초과 또는 0 미만일 때
					printf("잘못된 값을 입력하셨습니다.\n"); 
					printf("0 ~ 2사이의 값만 입력해주세요.\n");
					printf("점검할 대수를 선택하세요. (최대 2개)\n");
					scanf_s("%d", &cnt); // cnt의 값은 0 이상 2 이하여야하므로 다시 입력
				}
				int select = 0; // 점검할 엘리베이터를 저장하는 변수
				for (int i = 0; i < cnt; i++) { // cnt만큼 반복
					printf("점검 여부를 선택할 엘리베이터를 골라주세요.(하나씩 선택해주세요.)\n");
					scanf_s("%d", &select); // select 입력
					if (select < 0 || select > 6) { // 엘리베이터는 6개만 존재하므로 select가 0미만 또는 6 초과일 때 계속 반복하도록 정함
						printf("엘리베이터는 1번부터 6번까지만 존재합니다.\n");
						i--; // i값을 줄여 다시 반복
						continue;
					}
					if (elva[select - 1].inspect == 0 && *inspectCnt == 2) { // 최대 점검 대수는 2개이므로 
						//(select-1) 엘리베이터가 점검 중이 아니면서 점검 중인 엘리베이터 수가 2일 때를 조건으로 정함 
						printf("점검은 2대까지만 가능합니다.\n");
						printf("다시 선택해주세요.\n");
						i--; // i값을 줄여 다시 반복
						continue;
					}
					else { // 점검이 가능할 때
						printf("%d번 엘리베이터의 점검 여부를 전환합니다.\n",select);
						if (elva[select - 1].inspect == 0) { // 점검 중이 아닐 때
							elva[select - 1].inspect = 1; // 점검 중으로 바꿈
							(*inspectCnt)++; // 점검 중인 엘리베이터가 늘어났으므로 1 증가
						}
						else { // 점검 중일 때
							elva[select - 1].inspect = 0; // 점검 중이 아님으로 바꿈
							(*inspectCnt)--; // 점검 중이 아닌 엘리베이터가 늘어났으므로 1 감소
						}
					}
				}

			}
			else { // yes가 0이라면
				return;
			}
		}
		else if (flag == 2) { // flag가 2일 때
			int select = 0; // 엘리베이터의 번호를 나타내는 변수
			printf("어떤 엘리베이터를 보고 싶나요.\n");
			scanf_s("%d", &select); // select 입력
			while (select < 1 || select > 6) { // 엘리베이터는 6개만 존재하므로 select가 0미만 또는 6 초과일 때 계속 반복하도록 정함
				printf("엘리베이터는 1번부터 6번까지만 존재합니다.\n");
				scanf_s("%d", &select);
			}
			system("mode con cols=100 lines=111"); // 엘리베이터의 지하 10층부터 지상 100층까지 전부 출력해야하므로 cmd창의 크기를 늘림
			printf("\t--%d 엘리베이터--\n", select);
			for (int i = 0; i <= TOTAL_FLOOR; i++) { // 지하 10층부터 지상 100층까지 총 109개 이므로 0부터 110까지 반복
				if (i == 10) continue; // i가 10일 때는 0층을 뜻함. 0층은 없으므로 continue
				if (elva[select - 1].info[i].man == 0 && elva[select - 1].info[i].woman == 0) continue; // 둘 다 0일 때는 출력을 하지 않고 넘어감
				if (i < 10) { // i가 10보다 작을 때는 지하를 뜻함.
					printf("B%2d층\t남자 : %3d명\t여자 : %3d명\n", (i - 10) * -1, elva[select - 1].info[i].man, elva[select - 1].info[i].woman);
				}
				else { // i가 10보다 클 때는 지상을 뜻함.
					printf("%3d층\t남자 : %3d명\t여자 : %3d명\n", i - 10, elva[select - 1].info[i].man, elva[select - 1].info[i].woman);
				}
			}
			system("pause"); // 109개의 엘리베이터를 다 보기엔 초단위로는 부족할 수 있으므로 프로그램을 잠시 정지시킴
			system("mode con cols=100 lines=30"); // 기존의 엘리베이터 화면으로 돌아가므로 cmd창의 크기를 다시 조정함
		}
		else if (flag == 3) {
			int periodTemp = 0; // 바꿀 주기를 저장할 변수 생성
			printf("엘리베이터에 인원이 탑승하는 주기를 다시 설정해주세요.\n");
			scanf_s("%d", &periodTemp); // periodTemp 저장
			printf("탑승 주기가 %d초에서 %d초로 변경되었습니다.\n", periodValue/2, periodTemp);
			printf("1초 후 돌아갑니다.\n");
			Sleep(1000); // 1 초 정지
			periodValue = periodTemp * 2; // 0.5초 기준이므로 periodTemp에서 2를 곱한거로 교환한다.
		}
		else { // 잘못된 입력을 했을 때
			printf("잘못된 입력을 하셨습니다.\n");
			printf("1초 후 명령어 실행을 다시 시작합니다.\n");
			Sleep(1000); // 1초 정지
			behaviorAdmin(elva, correctPw, inspectCnt); // behaviorAdmin()을 다시 실행
		}
	}
}

void enter(int *loop,int *gender) { // 입장 여부를 판별하기 위한 함수
	printf("성별이 어떻게 되나요?(남성: 1 , 여성: 0)\n");
	scanf_s("%d", gender); //gender 입력
	printf("엘리베이터에 입장하시겠습니까?\n");
	printf("입장을 하신 후에도 ESC를 누르면 언제든 나가실 수 있습니다.\n");
	printf("입장을 희망한다면 1, 입장을 희망하지 않는다면 0을 입력해주세요.\n");
	scanf_s("%d", loop); //loop 입력
}

void helpMessage() { // 도움말 출력 함수
	int flag = 0; //도움말 종료 여부를 나타내는 변수
	system("cls");
	printf("엘리베이터를 부르는 방법은 위쪽 방향키 또는 아래쪽 방향키를 누르면 됩니다.\n");
	printf("엘리베이터를 탑승했을 때 왼쪽 방향키를 누르면 그 즉시 문이 닫힙니다.\n");
	printf("\t\t\t오른쪽 방향키를 누르면 그 즉시 문이 열립니다.\n");
	printf("ESC를 누르면 프로그램이 종료됩니다.\n");
	printf("F2를 누르면 관리자 모드를 실행할 수 있습니다.\n");
	printf("F2를 다시 누르면 관리자 모드가 종료됩니다.\n");
	printf("도움말을 종료하시겠습니까? (예: 1 , 아니요: 0)\n");
	scanf_s("%d", &flag); // flag 입력
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
	int minDir = 1; // 가장 최적의 엘리베이터의 이동 방향을 저장할 변수
	for (int i = 0; i < 6; i++) { // 6대이므로 0부터 5까지 반복
		if (elva[i].inspect == 1) continue; // 점검 중이라면 볼 필요가 없으므로 continue
		if (elva[i].dir == -1) { // 엘리베이터가 움직이지 않고 있을 때
			if (min > abs(elva[i].floor - start)) { // 현재 최소값보다 |i번 엘리베이터의 층 수 - 현재 사용자의 층수| 가 더 작다면
				min = abs(elva[i].floor - start); // 최소값을 갱신
				minIdx = i; // 갱신한 엘리베이터의 번호도 같이 갱신
				if (elva[i].floor >= start) { // i번 엘리베이터가 현재 사용자의 위치보다 크거나 같다면
					minDir = 0; // 내려가야하므로 방향을 0으로 지정
				}
				else { // i번 엘리베이터가 현재 사용자의 위치보다 작다면
					minDir = 1; // 올라가야하므로 방향을 1로 지정
				}
			}
		}
		if (upButton) { // 위로 가는 버튼을 눌렀을 때
			if (elva[i].dir == 1 && elva[i].floor <= start) { // 위로가는 엘리베이터이면서 현재 사용자의 위치보다 작거나 같을 때
				if (min > abs(elva[i].floor - start)) { // 현재 최소값보다 |i번 엘리베이터의 층 수 - 현재 사용자의 층수| 가 더 작다면
					min = abs(elva[i].floor - start); // 최소값을 갱신
					minIdx = i; // 갱신한 엘리베이터의 번호도 같이 갱신
					minDir = elva[i].dir; // 갱신한 엘리베이터의 방향도 같이 갱신
				}
			}
			else if (elva[i].dir == 0 && elva[i].floor >= start) { // 아래로 가는 엘리베이터이면서 현재 사용자의 위치보다 크거나 같을 때
				int flag = 0; // 진행 여부를 판단할 변수
				for (int j = start+10; j >= 0; j--) { // 현재 위치+10부터 0까지 반복
					if (j == 10) continue; // j가 10이면 0층이므로 continue
					if (elva[i].info[j].man != 0 || elva[i].info[j].woman != 0) { // i번 엘리베이터에 남자 또는 여자가 j층에 하나라도 있을 때
						flag = 1; // flag를 1로 지정
						break;
					}
				}
				if (flag) continue; // flag가 1일 때는 현재 위치 밑으로 
				//i번 엘리베이터가 더 내려간다는 뜻으로, 이때는 최소값인지 확인할 필요가 없으므로 continue;
				if (min > abs(elva[i].floor - start)) { // 현재 최소값보다 |i번 엘리베이터의 층 수 - 현재 사용자의 층수| 가 더 작다면
					min = abs(elva[i].floor - start); // 최소값을 갱신
					minIdx = i; // 갱신한 엘리베이터의 번호도 갱신
					minDir = elva[i].dir; // 갱신한 엘리베이터의 방향도 같이 갱신
				}
			}
		}
		if(downButton) { //아래로 가는 버튼을 눌렀을 때
			if (elva[i].dir == 0 && elva[i].floor >= start) { // 아래로 가는 엘리베이터면서 현재 사용자의 위치보다 크거나 같을 때
				if (min > abs(elva[i].floor - start)) { // 현재 최소값보다 |i번 엘리베이터의 층 수 - 현재 사용자의 층수| 가 더 작다면
					min = abs(elva[i].floor - start); // 최소값을 갱신
					minIdx = i; // 갱신한 엘리베이터의 번호도 갱신
					minDir = elva[i].dir; // 갱신한 엘리베이터의 방향도 같이 갱신
				}
			}
			else if (elva[i].dir == 1 && elva[i].floor <= start) { // 위로 가는 엘리베이터면서 현재 사용자의 위치보다 작거나 같을 때
				int flag = 0; // 진행 여부를 판단할 변수
				for (int j = start + 10; j <= TOTAL_FLOOR; j++) { // 현재 위치+10부터 110까지 반복
					if (j == 10) continue; // j가 10이면 0층이므로 continue
					if (elva[i].info[j].man != 0 || elva[i].info[j].woman != 0) { // i번 엘리베이터에 남자 또는 여자가 j층에 하나라도 있을 때
						flag = 1; // flag를 1로 지정
						break;
					}
				}
				if (flag) continue; // flag가 1일 때는 현재 위치 밑으로 
				//i번 엘리베이터가 더 내려간다는 뜻으로, 이때는 최소값인지 확인할 필요가 없으므로 continue;
				if (min > abs(elva[i].floor - start)) { // 현재 최소값보다 |i번 엘리베이터의 층 수 - 현재 사용자의 층수| 가 더 작다면
					min = abs(elva[i].floor - start); // 최소값을 갱신
					minIdx = i; // 갱신한 엘리베이터의 번호도 갱신
					minDir = elva[i].dir; // 갱신한 엘리베이터의 방향도 같이 갱신
				}
			}
		}
	}
	if (minIdx == -1) return minIdx; // minIdx가 -1이면 minIdx를 그대로 반환
	elva[minIdx].dir = minDir; // minIdx번 엘리베이터의 방향을 minDir로 바꿈
	return minIdx; // minIdx 반환
}

void checkElevatorLoc(elevator_t *elva, int *upButton,int *downButton, int *nowFloor,
	int *moveElva,int *moveFlag,int * nextFloor,int gender) { //엘리베이터를 탑승한 직후 필요한 정보들을 물어보는 함수
	int check;
	// check : 엘리베이터 탑승 여부를 나타내는 변수
	if (*upButton || *downButton) { // 위로 가는 버튼 또는 아래로 가는 버튼을 눌렀다면
		*moveElva = checkSpeed(elva, *nowFloor, *upButton, *downButton); // 움직일 엘리베이터를 확인함
		
		if (elva[*moveElva].floor == *nowFloor) { // 부른 엘리베이터의 층수와 현재 사용자가 있는 층수가 같다면
			printf("\n%d번 엘리베이터가 도착했습니다.\n", *moveElva + 1);
			printf("타시겠습니까? (예: 1 , 아니요: 0)\n");
			scanf_s("%d", &check); // check 입력
			if (check) { // check가 1이면
				if (elva[*moveElva].gender.man * MAN_KG + elva[*moveElva].gender.woman * WOMAN_KG + (gender ? MAN_KG : WOMAN_KG) > MAX_KG) { // 사용자가 탑승했을 때 MAX_KG의 값을 초과했을 때
					printf("엘리베이터 수용 인원을 초과하였습니다.\n");
					printf("하차하십시오.\n");
					Sleep(500); // 0.5초 기다리기
					*moveElva = -1; // *moveElva 값을 -1로 바꿈
					*downButton = 0; // 아래로 가는 버튼을 0으로 바꿈
					*upButton = 0; // 위로 가는 버튼을 0으로 바꿈
					return;
				}
				printf("엘리베이터에 탑승하셨습니다.\n");
				printf("3초 후 문이 자동으로 닫힙니다.\n");
				printf("문을 닫고 싶다면 왼쪽 방향키를\n");
				printf("문을 열고 싶다면 오른쪽 방향키를 눌러주세요.\n");
				int sec=4; // 3초의 시간을 주기 위해 4로 초기화
				while (--sec) { // sec에 1을 먼저 빼면서 반복
					if (_kbhit()) { //  키입력
						char key = _getch();
						if (key == -32) { // 키 입력
							key = _getch();
						}

						if (key == LEFT) { // key의 값이 LEFT라면 바로 문을 닫음 
							printf("1초 후 문이 닫힙니다.\n"); 
							Sleep(1000);
							break;
						}
						else if (key == RIGHT) { // key의 값이 RIGHT라면 바로 문을 염
							printf("문이 열립니다.\n");
							sec = 3; // 문을 다시 열었으므로 sec의 값을 3으로 늘림
						}
					}
					printf("%d초...\n", sec);
					Sleep(1000);
				}
				printf("문이 닫힙니다.\n");
				printf("어느 층으로 가시겠습니까?(%d ~ %d까지의 층만 존재합니다.)\n", MIN_FLOOR, MAX_FLOOR);
				scanf_s("%d", nextFloor); // nextFloor 입력
				while (*nextFloor == *nowFloor || (*upButton && *nextFloor < *nowFloor && elva[*moveElva].dir==1) ||
					(*downButton && *nextFloor > *nowFloor && elva[*moveElva].dir==0) ||
					(*nextFloor < MIN_FLOOR || *nextFloor > MAX_FLOOR)) { // 현재 층과 같을 때 또는 위로 가는 버튼을 눌렀으면서 현재 층보다 낮은 층을 입력했을 떄 또는
					// 아래로 가는 버튼을 눌렀으면서 현재 층보다 높은 층을 입력했을 때 또는
					// 최소 층과 최대 층의 범위를 넘어갔을 때인 동안 계속 반복
					if (*nextFloor == 0) { // 0일 때
						printf("0층은 존재하지 않습니다.\n");
					}
					else if (*nextFloor < MIN_FLOOR || *nextFloor > MAX_FLOOR) { // 최소 층과 최대 층의 범위를 넘어갔을 때
						printf("%d ~ %d까지의 층만 존재합니다.\n", MIN_FLOOR, MAX_FLOOR);
					}
					else if (*nextFloor == *nowFloor) { // 현재 층과 동일한 값을 입력했을 때
						printf("현재 있으신 층과 동일한 층을 입력하셨습니다.\n");
					}
					else if (*upButton && *nextFloor < *nowFloor) { // 위로 가는 버튼을 눌렀으면서 현재 층보다 낮은 층을 입력했을 때
						printf("현재 층 수보다 높은 층만 입력 가능합니다.\n");
					}
					else { // 아래로 가는 버튼을 눌렀으면서 현재 층보다 높은 층을 입력했을 때
						printf("현재 층 수보다 낮은 층만 입력 가능합니다.\n");
					}
					printf("다시 입력해주세요.\n");
					scanf_s("%d", nextFloor); // nextFloor 입력
				}

				*moveFlag = 1; // moveFlag의 값을 1로 바꿈
				if (gender) { // gender가 1이면 남자이므로
					elva[*moveElva].gender.man++; // 남자의 수를 하나 늘림
				}
				else { // gender가 0이면 여자이므로
					elva[*moveElva].gender.woman++; // 여자의 수를 하나 늘림
				}
				if (*nextFloor - *nowFloor > 0) { // (가야할 층 - 현재 층)이 양수라면
					elva[*moveElva].dir = 1; // 방향을 1로 바꿈
				}
				else { // (가야할 층 - 현재 층)이 0보다 작거나 같다면
					elva[*moveElva].dir = 0; // 방향을 0으로 바꿈
				}
			}
			else { // check가 0이면
				printf("엘리베이터에 탑승하지 않으셨습니다.\n");
				Sleep(500);
				*moveElva = -1; // *moveElva를 -1로 초기화
			}
			*downButton = 0; // 아래로 가는 버튼을 0으로 바꿈
			*upButton = 0; // 위로 가는 버튼을 0으로 바꿈
		}
		else { // 엘리베이터의 탑승을 했다면
			if (elva[*moveElva].dir == 1) { // 엘리베이터가 위로 가고 있을 때
				elva[*moveElva].floor++; // 층수를 하나씩 증가시킴
				if (elva[*moveElva].floor == 0) { // 0층은 없으므로 하나 더 증가
					elva[*moveElva].floor++;
				}
			}
			else if(elva[*moveElva].dir == 0) { // 엘리베이터가 아래로 가고 있을 때
				elva[*moveElva].floor--; // 층수를 하나씩 감소시킴
				if (elva[*moveElva].floor == 0) { // 0층은 없으므로 하나 더 감소
					elva[*moveElva].floor--;
				}
			}
		}
	}
}

void elevatorMove(elevator_t* elva, int* moveFlag, int *moveElva, int* nowFloor,
	int *nextFloor,int gender) { //탑승한 엘리베이터의 이동을 담당하는 함수
	if (elva[*moveElva].floor == *nextFloor) { // 탄 엘리베이터의 층 수와 가야할 층 수가 같을 때
		if (*nowFloor > 0) { // 지상 층이면
			printf("\n%d층에 도착했습니다.\n", *nowFloor);
		}
		else { // 지하 층일 때
			printf("\nB%d층에 도착했습니다.\n", *nowFloor * -1);
		}
		Sleep(1000);
		printf("문이 열립니다.\n");
		Sleep(1000);
		*moveFlag = 0; // *moveFlag를 0으로 바꿈
		if (gender) { // gender가 1이면 남자 수를 하나 줄임
			elva[*moveElva].gender.man--;
		}
		else { // gender가 0이면 여자 수를 하나 줄임
			elva[*moveElva].gender.woman--;
		}
		*moveElva = -1; // *moveElva를 -1로 바꿈
	}
	else { // 탄 엘리베이터의 층 수와 가야할 층 수가 다를 때
		if (elva[*moveElva].dir) { // 방향이 1이면
			elva[*moveElva].floor++; // 층 수 증가
			if (elva[*moveElva].floor == 0) { // 0층일 때는 1 추가로 증가
				elva[*moveElva].floor++; 
			}
		}
		else { // 방향이 0이면
			elva[*moveElva].floor--; // 층 수 감소 
			if (elva[*moveElva].floor == 0) { //0층일 때는 1 추가로 감소
				elva[*moveElva].floor--;
			}
		}

		*nowFloor = elva[*moveElva].floor; // 현재 층 수를 elva[*moveElva].floor로 갱신
	}

}

void randBoard(elevator_t *elva) { //탑승 수 랜덤으로 추가
	int randomE = rand() % 6; // 탑승할 엘리베이터를 지정
	if (elva[randomE].inspect == 1) return;
	int randomM = rand() % 10; // 남자 수를 랜덤으로 지정
	int randomW = rand() % 10; // 여자 수를 랜덤으로 지정
	info_t nowGender;
	nowGender.man = elva[randomE].gender.man + randomM; // 현재 엘리베이터 위치에서 탑승
	nowGender.woman = elva[randomE].gender.woman + randomW; //현재 엘리베이터 위치에서 탑승

	if (admin == 2) { // admin이 2일 때 ( 관리자 모드일 때 )
		if (elva[randomE].floor>=0) { // 엘리베이터의 현재 층이 양수일 때
			printf("\n%d번 엘리베이터에서 %3d층에 남자 %d명과 여자 %d명이 탑승했습니다.\n", randomE + 1, elva[randomE].floor, randomM, randomW);
		}
		else { // 엘리베이터의 현재 층이 음수일 때
			printf("\n%d번 엘리베이터에서 B%2d층에 남자 %d명과 여자 %d명이 탑승했습니다.\n", randomE + 1, elva[randomE].floor * -1, randomM, randomW);
		}
		printf("%d번 엘리베이터는 현재 %dkg입니다.\n", randomE + 1, nowGender.man * MAN_KG + nowGender.woman * WOMAN_KG);
		Sleep(2000);
	}
	
	while(nowGender.man*MAN_KG + nowGender.woman*WOMAN_KG >= MAX_KG) { //만원일때
		int flag = rand() % 2; // 내릴 성별을 랜덤으로 지정 (0: 여자, 1: 남자)
		if (flag) { // flag가 1일 때 ( 남자라면 )
			nowGender.man--; // nowGender.man의 값을 하나 줄임
			randomM--; // randomM의 값을 하나 줄임
			if (admin == 2) { // admin이 2일 때
				// 만원일 때 누가 내렸는 지 출력
				printf("엘리베이터 수용 인원을 초과하여 남자 1명이 하차하였습니다.\n");
				Sleep(1000);
			}
		}
		else { // flag가 0일 때
			nowGender.woman--; // nowGender.woman의 값을 하나 줄임
			randomW--; // randomW의 값을 하나 줄임
			if (admin == 2) { // admin이 2일 때
				// 만원일 때 누가 내렸는 지 출력
				printf("엘리베이터 수용 인원을 초과하여 여자 1명이 하차하였습니다.\n");
				Sleep(1000);
			}
		}
	}

	//위 while문이 끝났다면 무조건 nowGender의 값을 만원이 안뜨도록 조정했으므로 현재 탑승한 인원으로 추가
	elva[randomE].gender.man = nowGender.man; // elva[randomE].gender.man 에 nowGender.man을 저장
	elva[randomE].gender.woman = nowGender.woman; // elva[randomE].gender.woman 에 nowGender.woman을 저장

	//현재 위치에서 탑승한 사람들이 어느 층으로 가고 싶은지를 난수로 받음
	info_t now; // 현재 위치에서 탑승한 사람들을 저장하는 구조체
	now.man = randomM; // randomM만큼 now.man에 저장
	now.woman = randomW; // randomW만큼 now.woman에 저장
	while (now.man > 0 || now.woman > 0) { // now.man 또는 now.woman이 0보다 큰 동안
		int nextFloor = rand() % 111; // now명의 사람이 가야할 층을 저장하는 변수로 총 0~110 나오도록 랜덤을 돌림
		if (nextFloor == 10) { // 0층은 없으므로 continue
			continue;
		}
		info_t next = now; // now명 중 몇명이나 nextFloor 층에 내릴 지 저장하는 구조체
		if (now.man > 0) { // now.man이 0보다 클 때
			next.man = rand() % (now.man + 1); // next.man을 0~now.man 중 랜덤으로 저장
		}
		if (now.woman > 0) { // now.woman이 0보다 클 때
			next.woman = rand() % (now.woman + 1); // next.woman을 0~now.woman 중 랜덤으로 저장
		}
		elva[randomE].info[nextFloor].man += next.man; // randomE번 엘리베이터의 nextFloor층에서 내려야할 남자 수를 next.man만큼 추가
		elva[randomE].info[nextFloor].woman += next.woman; // randomE번 엘리베이터의 nextFloor층에서 내려야할 여자 수를 next.woman만큼 추가
		now.man -= next.man; // now.man에서 next.man만큼 뺌
		now.woman -= next.woman; // now.woman에서 next.woman만큼 뺌
	}
	
}

void checkHuman(elevator_t* elva,int now) { // 현재 층에 내릴 사람이 있는지 확인하는 함수
	
	int nowFloor = elva[now].floor+10; // -10 ~ -1 && 1 ~ 100  ->  0 ~9 && 11 ~ 110
	int nowM = elva[now].info[nowFloor].man; // nowM은 now번 엘리베이터의 nowFloor에서 내려야할 남자 수를 저장
	int nowW = elva[now].info[nowFloor].woman; // nowW은 now번 엘리베이터의 nowFloor에서 내려야할 여자 수를 저장

	if (nowM == 0 && nowW == 0) return; // nowM과 nowW가 모두 0이면 바로 종료

	//무조건 내리므로 전부 0으로 초기화
	elva[now].info[nowFloor].man = 0; // now번 엘리베이터의 nowFloor에서 내려야할 남자 수를 0으로 저장
	elva[now].info[nowFloor].woman = 0; // now번 엘리베이터의 nowFloor에서 내려야할 여자 수를 0으로 저장

	elva[now].gender.man -= nowM; // now번 엘리베이터의 탑승 중인 남자 수를 nowM만큼 뺌
	elva[now].gender.woman -= nowW; // now번 엘리베이터의 탑승 중인 여자 수를 nowW만큼 뺌

	if (elva[now].gender.man == 0 && elva[now].gender.woman == 0) { // 탑승 중인 사람의 수가 0이면
		elva[now].dir = -1; // 엘리베이터를 멈춤
	}
	else { // 탑승 중인 사람이 있다면
		if (admin == 2) { // admin이 2일 때
			// 몇 명 내렸는 지 출력해서 보여줌
			printf("\n%d번 엘리베이터에서 %d층에 남자 %d명과 여자 %d명이 내렸습니다.\n", now+1, nowFloor, nowM, nowW);
			Sleep(1000);
		}
	}

}

void checkDirection(elevator_t* elva,int now,int moveElva) { // 엘리베이터의 방향을 확인하는 함수
	if ((elva[now].gender.man == 0 && elva[now].gender.woman == 0) && moveElva != now) { // 엘리베이터의 사람 수가 0이면서 부른 엘리베이터와 다른 엘리베이터일 때
		elva[now].dir = -1; // 엘리베이터를 멈춤
		return;
	}
	if (elva[now].dir == 1) { // 엘리베이터가 올라가고 있을 때
		int flag = 0; // 올라가야하는 지 말아야하는 지 판단하는 변수
		int nFloor = elva[now].floor + 10; // -10 ~ -1 && 1 ~ 100  ->  0 ~9 && 11 ~ 110 이므로 현재 층에 + 10한 것을 저장하는 변수
		while (nFloor <= TOTAL_FLOOR) { // nFloor가 110보다 작거나 같은 동안
			if (elva[now].info[nFloor].man > 0 || elva[now].info[nFloor].woman > 0) { // 아직 내릴 사람이 있다면
				flag = 1; // flag를 1로 저장 ( 계속 올라가야하기 때문에 )
				break; // while문을 빠져나옴
			}
			nFloor++; // nFloor의 값을 하나 증가
			if (nFloor == 10) { // 0층이므로 하나 추가로 증가
				nFloor++;
			}
		}
		if (!flag) { // flag가 0일 때
			elva[now].dir = 0; // 엘리베이터의 방향을 하강으로 바꿈
		}
	}
	else if (elva[now].dir == 0) { // 엘리베이터가 내려가고 있을 때
		int flag = 0; // 내려가야하는 지 말아야하는 지 판단하는 변수
		int nFloor = elva[now].floor+10; // -10 ~ -1 && 1 ~ 100 -> 0 ~ 9 && 11 ~ 110 이므로 현재 층에 + 10한 것을 저장하는 변수
		while (nFloor >= 0) { // nFloor가 0보다 크거나 같은 동안
			if (elva[now].info[nFloor].man > 0 || elva[now].info[nFloor].woman > 0) { // 아직 내릴 사람이 있다면
				flag = 1; // flag를 1로 저장 ( 계속 내려가야하기 때문에 )
				break; // while문을 빠져나옴
			}
			nFloor--; //nFloor의 값을 하나 감소
			if (nFloor == 10) { // 0층이므로 하나 추가로 감소
				nFloor--;
			}
		}
		if (!flag) { // flag가 0일 때
			elva[now].dir = 1; // 엘리베이터의 방향을 상승으로 바꿈
		}
	}
	else { // 엘리베이터가 멈춰있을 때
		int flag = 0; // 가는 방향에 사람이 있는지 판단하는 변수
		int nFloor = elva[now].floor + 10; // -10 ~ -1 && 1 ~ 100 -> 0 ~ 9 && 11 ~ 110 이므로 현재 층에 + 10한 것을 저장하는 변수
		// 먼저 올라가는 방향으로 사람이 있는 지 확인
		while (nFloor <= TOTAL_FLOOR) { // nFloor가 110보다 작거나 같은 동안
			if (elva[now].info[nFloor].man > 0 || elva[now].info[nFloor].woman > 0) { // 아직 내릴 사람이 있다면
				flag = 1; // flag를 1로 저장 ( 계속 내려가야하기 때문에 )
				break; // while문을 빠져나옴
			}
			nFloor++; // nFloor의 값을 하나 증가
			if (nFloor == 10) { // 0층이므로 하나 추가로 증가
				nFloor++;
			}
		}
		if (flag) { // flag가 1일 때
			elva[now].dir = 1; // 올라가는 방향으로 사람이 있다는 소리이므로 방향을 1로 바꿈
			return; // 함수 종료
		}
		flag = 0; // flag를 0으로 초기화
		nFloor = elva[now].floor + 10; // -10 ~ -1 && 1 ~ 100 -> 0 ~ 9 && 11 ~ 110 이므로 현재 층에 + 10한 것을 저장하는 변수
		// 내려가는 방향으로 사람이 있는 지 확인
		while (nFloor >= 0) { // nFloor가 0보다 크거나 같은 동안
			if (elva[now].info[nFloor].man > 0 || elva[now].info[nFloor].woman > 0) { // 아직 내릴 사람이 있다면
				flag = 1; // flag를 1로 저장 ( 계속 내려가야하기 때문에 )
				break; // while문을 빠져나옴
			}
			nFloor--; //nFloor의 값을 하나 감소
			if (nFloor == 10) { // 0층이므로 하나 추가로 감소
				nFloor--;
			}
		}
		if (flag) { // flag가 1일 때
			elva[now].dir = 0; // 올라가는 방향으로 사람이 있다는 소리이므로 방향을 1로 바꿈
			return; // 함수 종료
		}
	}
}

void autoMove(elevator_t* elva, int moveElva) { // 엘리베이터를 자동으로 움직이게 하는 함수
	for (int i = 0; i < 6; i++) { // 엘리베이터가 6개 이므로 0 ~ 5까지 반복
		if (elva[i].inspect == 1) continue; // 점검 중이라면 continue
		if (elva[i].floor <= MAX_FLOOR && elva[i].floor >= MIN_FLOOR) { // 엘리베이터의 층이 100보다 작거나 같고 -10보다 크거나 같을 때
			if (elva[i].gender.man == 0 && elva[i].gender.woman == 0) { // 탄 사람이 없다면
				elva[i].dir = -1; // 엘리베이터를 멈춤
				continue;
			}
			if (moveElva != i) { // 사용자가 탄 엘리베이터와 현재 확인할 엘리베이터가 다를 때
				if (elva[i].dir == 1 && elva[i].floor < MAX_FLOOR) { // 올라가면서 110보다 작을 때
					elva[i].floor++; // i번 엘리베이터의 층을 1씩 증가
					if (elva[i].floor == 0) { // 0층일 때는 1 추가로 증가
						elva[i].floor++;
					}
				}
				else if (elva[i].dir == 0 && elva[i].floor > MIN_FLOOR) { // 내려가면서 -10보다 클 때
					elva[i].floor--; // i번 엘리베이터의 층을 1씩 감소
					if (elva[i].floor == 0) { // 0층일 때는 1 추가로 감소
						elva[i].floor--;
					}
				}

				checkDirection(elva, i, moveElva); // 방향을 확인하는 함수를 실행
			}
			checkHuman(elva,i); // 현재 층에 내릴 사람이 있는 지 확인하는 함수를 실행
		}
	}
}

int main()
{
	srand((unsigned int)time(NULL)); //랜덤을 위한 시드값으로 시간을 사용함
	system("mode con cols=100 lines=30 | title 엘리베이터 프로그램");
	elevator_t elva[6] = { 0, }; //6대 이므로 6짜리 구조체 배열 생성
	int upButton=0,downButton=0,nowFloor=1,loop=0,moveFlag = 0,moveElva=-1;
	//upButton : 위로 향하는 버튼의 상태를 저장하는 변수 (on: 1, off: 0)
	//downButton : 아래로 향하는 버튼의 상태를 저장하는 변수 (on: 1, off: 0)
	//nowFloor : 현재 사용자의 층수를 저장하는 변수
	//loop : 루프를 돌릴지 말지를 저장하는 변수 (루프 ○ : 1 , 루프 X : 0)
	//moveFlag : 엘리베이터가 움직일지 안움직일지를 저장하는 변수 (움직임: 1 , 안움직임: 0)
	//moveElva : 움직이는 엘리베이터의 번호를 저장하는 변수
	int check = 0, nextFloor = 0, door = 0, gender = 0, help = 0, inspectCnt = 0;
	//check : 엘리베이터 탑승 여부를 저장하는 변수 (탑승: 1 , 탑승거부: 0)
	//nextFloor : 가고자 하는 층을 저장하는 변수
	//door : 엘리베이터의 문을 닫을지 말지를 저장하는 변수 (닫음: 1 , 닫지 않음: 0)
	//gender : 사용자의 성별을 저장하는 변수 (남성: 1 , 여성: 0)
	//help : 도움말 활성화 여부를 나타내는 변수 (활성화: 1 , 비활성화: 0)
	//admin : 관리자 모드 활성화 여부를 나타내는 변수 (활성화: 1 , 비활성화: 0)
	//inspectCnt : 점검 중인 엘리베이터의 개수를 저장하는 변수
	int period = periodValue;
	// period : 엘리베이터의 사람이 탑승하는 주기를 나타내는 변수
	char correctPw[100] = "1234567*"; //관리자모드 비밀번호


	init(elva); //초기화

	//Call by reference를 위해 함수의 매개변수는 대부분 포인터 변수임
	do {
		if (loop == 0) { //맨 처음 실행되었을 때
			enter(&loop, &gender); //맨 처음 실행되었을 때 필요한 정보를 받기 위한 함수
			system("cls"); // 화면을 지움
		}
		else {
			if (_kbhit()) { // 키가 눌러져있다면
				char key = _getch(); // key 입력
				if (key == -32) { // key가 -32일 때
					key = _getch(); // key 입력
				}
				else if (key == 0) { // key가 0일 때
					key = _getch(); // key 입력
				}

				if (key == UP && !moveFlag) { //위쪽 방향키를 눌렀을 때
					upButton = !upButton; // upButton의 값을 반대로 바꿈
					moveElva = -1; // moveElva 값을 -1로 저장
				}
				else if (key == DOWN && !moveFlag) { //아래쪽 방향키를 눌렀을 때
					downButton = !downButton; // downButton의 값을 반대로 바꿈
					moveElva = -1; // moveElva 값을 -1로 저장
				}
				else if (key == F1) { //F1을 눌렀을 때
					help = 1; // help의 값을 1로 저장
				}
				else if (key == F2) { //F2를 눌렀을 때
					admin = !admin; // admin의 값을 반대로 바꿈
				}
				else if (key == F3 && admin == 2) { //관리자 모드 상태에서 F3을 누를 시
					behaviorAdmin(elva,correctPw,&inspectCnt); // 관리자 모드 명령어들을 실행 
				}
				else if (key == ESC) { //ESC를 눌렀을 때
					loop = 0; // loop을 0으로 저장
				}
			}
			
			if (help) {
				helpMessage(); // 도움말 UI 출력 함수 실행
				system("cls"); // 화면을 지움
				help = 0; // help를 0으로 저장
				continue; // continue
			}

			if (admin==1) { // admin이 1일 때
				behaviorAdmin(elva, correctPw, &inspectCnt); // 관리자 모드 용 UI 출력 함수 실행
				system("cls"); // 화면을 지움
				continue; // continue
			}
			else {
				userPrint(elva, moveElva, upButton, downButton, nowFloor); //기본 UI 출력 함수 실행
			}

			if (moveFlag) { // moveFlag가 1일 때
				elevatorMove(elva, &moveFlag, &moveElva, &nowFloor, &nextFloor, gender); // 사용자가 엘리베이터에 탔을 때 움직이는 함수 실행
			}
			else { // moveFlag가 0일 때
				checkElevatorLoc(elva, &upButton, &downButton, &nowFloor, &moveElva, &moveFlag, &nextFloor, gender); // 엘리베이터가 도착했을 때의 정보를 입력하는 함수 실행
			}

			autoMove(elva,moveElva); // 자동으로 움직이는 함수 실행 

			if (period <= 0) { // period가 0보다 작거나 같을 때
				randBoard(elva); // 탑승하는 함수를 실행
				period = periodValue; // period를 다시 PERIOD_VALUE로 지정
			}
			period--; // period의 값을 1씩 감소
			Sleep(500); //0.5초동안 프로그램을 멈춤
			system("cls"); //cmd화면에 이전 출력 결과를 지워줌
		}
	} while (loop == 1); // loop값이 1인 동안 계속 반복
	printf("엘리베이터 프로그램을 종료합니다.\n");
	return 0;
}