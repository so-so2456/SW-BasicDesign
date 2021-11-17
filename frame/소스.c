#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

void ShowGameBoard();		//맵 + 플레이어 정보 + 게임 정보를 화면에 보여줌
int SetPlayers();		//몇명의 플레이어가 플레이할건지 정하는 함수
void SelectMap();		//맵 정하는 함수
int IsTurnEnd();		//한 사람의 턴이 끝났는지 확인
int IsGameOver();		//게임이 종료조건을 만족하는지 확인
void ShowMenu();		//메뉴를 화면에 보여줌
void ProcessKeyInput();

void ShowMap();
void ShowPlayerInfo();		//플레이어 정보 : 플레이어 순위, 코인, 별
void ShowGameInfo();		//게임 정보 : 현재 차례, 현재 턴, 턴 순서, 칸 효과

void ArrowUp();
void ArrowDown();
void SelectMenu();
void CancelMenu();

void RollDice();		//주사위 굴리기
int ShowItem();			//플레이어가 가지고 있는 아이템 목록을 보여줌
void UseItem(item);		//플레이어가 아이템을 사용한다고 했을 때 실행되는 함수
void BuyItemOrStar();		//플레이어가 아이템이나 별을 구매한다고 했을 때 실행
void SaveGame();

int main()
{
	int n = SetPlayers();
	SelectMap();
	ShowGameBoard();
    while (1)
    {
        if (IsGameOver()) break;
        for (int i = 0; i < n; i++)
        {
            while (1)
            {
                if (IsTurnEnd())	//한 사람의 턴이 끝났는지 확인
                ShowMenu();		//메뉴를 보여주고
                ProcessKeyInput();	//키 입력을 받는다
            }
        }
    }
	return 0;
}

void ShowGameBoard()
{
    ShowMap();
    ShowPlayerInfo();
    ShowGameInfo();
}

void ProcessKeyInput()
{
    int key;
    for (int i = 0; i < 20; i++)
    {
        if (_kbhit() != 0)
        {
            key = _getch();
            switch (key)
            {
            case UP:
                ArrowUp();
                break;
            case Down:
                ArrowDown();
                break;
            case Z:			//Z키로 메뉴 선택
                SelectMenu();
                break;
            case X:			//X키로 취소(이 기능 쓰려면 메뉴 선택했을때 다시 물어봐줘야 할 듯?)
                CancelMenu();
                break;
            }
        }
        Sleep(50);
    }
}

void SelectMenu()
{
    int item;
    switch (curMenu) // 현재 화살표가 가리키는 메뉴
    {
    case DICE:			//주사위를 선택한 경우
        RollDice();		//주사위를 굴린다
        break;
    case ITEM:			//아이템을 선택한 경우
        item = ShowItem();	//가지고 있는 아이템 목록을 보여주고
        UseItem(item);		//선택한 아이템을 사용한다
        break;
    case BUY:			//구매를 선택한 경우(상점 칸이나 별 칸)
        BuyItemOrStar();	//아이템이나 별을 구매한다
        break;
    case SAVE:			//세이브를 선택한 경우
        SaveGame();
        break;
    }
}
