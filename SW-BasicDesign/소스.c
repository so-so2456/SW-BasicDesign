#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

void ShowGameBoard();
int SetPlayers();
void SelectMap();
int IsTurnEnd();
int IsGameOver();
void ShowMenu();
void ProcessKeyInput();

void ShowMap();
void ShowPlayerInfo();
void ShowGameInfo();

void ArrowUp();
void ArrowDown();
void SelectMenu();
void CancelMenu();

void RollDice();
int ShowItem();
void UseItem(item);
void BuyItemOrStar();
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
                if (IsTurnEnd())
                ShowMenu();
                ProcessKeyInput();
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
            case Z:
                SelectMenu();
                break;
            case X:
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
    case DICE:
        RollDice();
        break;
    case ITEM:
        item = ShowItem();
        UseItem(item);
        break;
    case BUY:
        BuyItemOrStar();
        break;
    case SAVE:
        SaveGame();
        break;
    }
}