#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include "adjlist.h"
#include "coord.h"
#include "DICE.h"
#include "MAP.h"

#pragma warning(disable:4996)
#define WIDTH 60
#define HEIGHT 30
#define MAP_X_MAX 180
#define MAP_Y_MAX 50
static int g_nScreenIndex;
static HANDLE g_hScreen[2];
int g_numofFPS;
clock_t CurTime, OldTime;
char* FPSTextInfo;

/*void ScreenInit()                        //���߹��۸�(�ٵ� �ȵ�)
{
	CONSOLE_CURSOR_INFO cci;

	// ȭ�� ���� 2���� �����.
	g_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	g_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	// Ŀ���� �����.
	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(g_hScreen[0], &cci);
	SetConsoleCursorInfo(g_hScreen[1], &cci);
}

void ScreenFlipping()
{
	SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
	g_nScreenIndex = !g_nScreenIndex;
}

void ScreenClear()
{
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(g_hScreen[g_nScreenIndex], ' ', 80 * 25, Coor, &dw);
}

void ScreenRelease()
{
	CloseHandle(g_hScreen[0]);
	CloseHandle(g_hScreen[1]);
}

*/

typedef struct GraphNode
{
	int tile;
	int vertex;
	struct GraphNode* link;
}GraphNode;

typedef struct GraphType
{
	int n;
	GraphNode** adjList_H;
	int* visited;
	COORD* coordList;
}GraphType;

typedef struct Belongings
{
	int star;
	int coin;
	int* itemList; // pair�� ������ �������� ����
}Belongings;

typedef struct Player
{
	int id;				// �÷��̾� ����
	int curPos;			// ���� ��ġ(���� ��ȣ)
	int isCPU;			// ��ǻ�� �÷��̾� ����
	Belongings info;	// �÷��̾� ����ǰ
}Player;

void SetCurrentCursorPos(int x, int y);
void showmap();
void showMenu();
void RemoveCursor();
void showPlayersInfo();
void showDialog();
int RollDice();
void showDice(int n);
void RenewTurn(); //�� ����
void showPlayerItemList();
int ProcessKeyInput();
void Dialog();
int IsEventBlock();       //�̺�Ʈ ĭ���� Ȯ��
void movePlayer(int id, int v, int n);
void createGraph(int n);
void setAdjList(int m);
void setCoord();
void setMap(int n);
COORD selectPath();
void setPlayer();
void showPlayer();

int player_turn = 1;       //� �÷��̾ �����ϴ� ������ Ȯ���ϴ� ����
GraphType* g;
Player p[4];
int numPlayer = 0;



int main()
{
	srand(time(NULL));
	system("mode con cols=180 lines=50 | title ���̽� ��Ƽ");
	RemoveCursor();

	setPlayer();
	setMap(62);
	showmap();
	showMenu();
	showPlayersInfo();
	showDialog();
	showPlayer();
	Dialog();

	getchar();

	return 0;
}
void SetCurrentCursorPos(int x, int y)
{
	COORD position = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}
void showmap()
{
	for (int i = 0; i < 30; ++i)
	{
		for (int j = 0; j < 60; ++j)
		{
			if (map1[i][j] == 1)
				printf("��");
			else
				printf("  ");
		}
		printf("\n");
	}
}
void RemoveCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void showMenu()
{
	int x0 = WIDTH * 2 + 5;
	int y0 = 1;
	SetCurrentCursorPos(x0 + 20, 0);
	printf("Menu bar");
	for (int y = 0; y < 41; ++y)
		for (int x = 0; x < 50; ++x)
		{
			SetCurrentCursorPos(x0 + x, y0 + y);                //�޴� ���� ���� = 25(�ܼ�â������ x2�ؼ� 50), ���� ���� = 41(��ġ: 1���� �����ؼ� 42���� ����)
			if ((x == 0) || (x == 49))
			{
				if (x == 0 && y == 0)
					printf("��");
				else if (x == 49 && y == 0)
					printf("��");
				else if (x == 0 && y == 40)
					printf("��");
				else if (x == 49 && y == 40)
					printf("��");
				else
					printf("��");
			}
			else if ((y == 0) || (y == 40))
			{

				printf("��");
			}

		}
	SetCurrentCursorPos(x0 + 2, 2);          //���� ����
	printf("���� ���� : ");

	SetCurrentCursorPos(x0 + 2, 5);         //�÷��̾� ����
	printf("�÷��̾� ���� : ");

	SetCurrentCursorPos(x0 + 2, 8);        //��(1(���� ��/15(���� ��))
	printf("��: ");

	SetCurrentCursorPos(x0 + 2, 11);
	printf("�鰡���� �ִ� ������ ��ϡ�");
	x0 += 2;
	y0 += 12;
	for (int y = 0; y < 15; ++y)
		for (int x = 0; x < 46; ++x)
		{
			SetCurrentCursorPos(x0 + x, y0 + y);
			if ((x == 0) || (x == 45))
			{
				if (x == 0 && y == 0)
					printf("��");
				else if (x == 45 && y == 0)
					printf("��");
				else if (x == 0 && y == 14)
					printf("��");
				else if (x == 45 && y == 14)
					printf("��");
				else
					printf("��");
			}
			else if ((y == 0) || (y == 14))
			{

				printf("��");
			}

		}


	SetCurrentCursorPos(x0, y0 + 16);
	printf("���ֻ�����");
	y0 += 17;
	for (int y = 0; y < 9; ++y)
		for (int x = 0; x < 18; ++x)
		{
			SetCurrentCursorPos(x0 + x, y0 + y);
			if ((x == 0) || (x == 17))
			{
				if (x == 0 && y == 0)
					printf("��");
				else if (x == 17 && y == 0)
					printf("��");
				else if (x == 0 && y == 8)
					printf("��");
				else if (x == 17 && y == 8)
					printf("��");
				else
					printf("��");
			}
			else if ((y == 0) || (y == 8))
			{

				printf("��");
			}

		}
}
void showPlayersInfo()
{
	int x0 = 0;
	int Mx = WIDTH * 2;
	int y0 = HEIGHT + 2 + 7;  //Dialog â�� ���� ���� = 8
	SetCurrentCursorPos(x0, y0 - 1);
	printf("Players Info");
	for (int y = 0; y < 8; ++y)
		for (int x = 0; x < Mx; ++x)
		{
			SetCurrentCursorPos(x0 + x, y0 + y);
			if ((x == 0) || (x == Mx - 1))
			{
				if (x == 0 && y == 0)
					printf("��");
				else if (x == Mx - 1 && y == 0)
					printf("��");
				else if (x == 0 && y == 7)
					printf("��");
				else if (x == Mx - 1 && y == 7)
					printf("��");
				else
					printf("��");
			}
			else if ((y == 0) || (y == 7))
			{

				printf("��");
			}

		}
}
void showDialog()
{
	int x0 = 0;
	int Mx = WIDTH * 2;
	int y0 = HEIGHT + 2;
	SetCurrentCursorPos(x0, y0 - 1);
	printf("Dialog");
	for (int y = 0; y < 6; ++y)
		for (int x = 0; x < Mx; ++x)
		{
			SetCurrentCursorPos(x0 + x, y0 + y);
			if ((x == 0) || (x == Mx - 1))
			{
				if (x == 0 && y == 0)
					printf("��");
				else if (x == Mx - 1 && y == 0)
					printf("��");
				else if (x == 0 && y == 5)
					printf("��");
				else if (x == Mx - 1 && y == 5)
					printf("��");
				else
					printf("��");
			}
			else if ((y == 0) || (y == 5))
			{

				printf("��");
			}

		}



}
int RollDice()
{
	int n;
	n = rand() % 6 + 1;
	return n;
}
void showDice(int n)
{
	RemoveCursor();
	int x0 = WIDTH * 2 + 9;
	int y0 = 31;
	int tmp = y0;

	SetCurrentCursorPos(x0, y0);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			SetCurrentCursorPos(x0, y0);
			tmp = y0;
			//printf("%d", j);
			for (int k = 0; k < 7; ++k)
			{

				SetCurrentCursorPos(x0, tmp);
				tmp += 1;
				for (int l = 0; l < 7; ++l)
				{
					if (DICE[j][k][l] == 0)
						printf("  ");
					else if (DICE[j][k][l] == 1)
						printf("��");
				}
			}
			Sleep(150);
		}
	}
	for (int j = 0; j < 6; ++j)
	{
		SetCurrentCursorPos(x0, y0);
		tmp = y0;
		//printf("%d", j);
		for (int k = 0; k < 7; ++k)
		{

			SetCurrentCursorPos(x0, tmp);
			tmp += 1;
			for (int l = 0; l < 7; ++l)
			{
				if (DICE[j][k][l] == 0)
					printf("  ");
				else if (DICE[j][k][l] == 1)
					printf("��");
			}
		}
		Sleep(150);
		if (j == n - 1)
			break;
	}

}
int ProcessKeyInput()
{
	int key;

	key = _getch();
	if (key == 'z')
		return 1;
	else if (key == 'x')
		return 0;
}
void Dialog()
{
	int x0 = 0;
	int y0 = HEIGHT + 2;
	int n, result;
	int event;
	for (int i = 0; i < 6; ++i)
	{
		result = -1;
		if (i == 0)
		{
			SetCurrentCursorPos(x0 + 2, y0 + 1);
			printf("Player%d�� ���Դϴ�. �����Ϸ��� zŰ�� �����ּ���", player_turn);
			while (result != 1)
			{
				result = ProcessKeyInput();
			}
		}
		if (i == 1)    //������ ��� Ȯ��(z)/���(x)
		{
			SetCurrentCursorPos(x0 + 2, y0 + 1);
			printf("�������� ����Ͻðڽ��ϱ�?");
			while (result != 0)      //x�� ���� while (result != 1 || result != 0)
			{
				result = ProcessKeyInput();
			}
		}
		if (i == 2)
		{
			SetCurrentCursorPos(x0 + 2, y0 + 1);
			printf("�ֻ����� �����ڽ��ϱ�?(Z) : ");

			result = ProcessKeyInput();
			while (result != 1)
			{
				result = ProcessKeyInput();
			}
			if (result == 1)
			{
				n = RollDice();
				showDice(n);
			}
			result = ProcessKeyInput();
			while (result != 1)
			{
				result = ProcessKeyInput();
			}
		}
		if (i == 3)
		{
			SetCurrentCursorPos(x0 + 2, y0 + 1);
			printf("%d��ŭ �̵��մϴ�.(Z)", n);
			result = ProcessKeyInput();
			while (result != 1)
			{
				result = ProcessKeyInput();
			}
			movePlayer(0, p[0].curPos, n);
		}
		if (i == 4)
		{
			event = IsEventBlock();

		}
		if (i == 5)
		{
			SetCurrentCursorPos(x0 + 2, y0 + 1);
			printf("Player%d�� ���� ����Ǿ����ϴ�. �����Ϸ��� zŰ�� �����ּ���", player_turn);
			result = ProcessKeyInput();
			while (result != 1)
			{
				result = ProcessKeyInput();
			}
		}
		SetCurrentCursorPos(x0 + 2, y0 + 1);
		printf("                                                                                  ");
		/*system("cls");

		showmap();
		showMenu();
		showPlayersInfo();
		showDialog();*/


	}
}
void showPlayerItemList()
{

}
int IsEventBlock()
{
	return 0;
}

void movePlayer(int id, int v, int n)
{
	GraphNode* w = NULL;
	for (int i = 0; i < n; i++)
	{
		w = g->adjList_H[v];
		COORD cur = g->coordList[v];
		COORD next;
		if (w->link != NULL) next = g->coordList[w->link->vertex];
		//next = selectPath();
		else next = g->coordList[w->vertex];
		SetCurrentCursorPos(2 * cur.X, cur.Y);
		printf("  ");
		SetCurrentCursorPos(2 * next.X, next.Y);
		printf("��");
		Sleep(500);
		v = w->vertex;
	}
	p[id].curPos = w->vertex;
}

void createGraph(int n)
{
	g = (GraphType*)malloc(sizeof(GraphType));
	g->n = n;
	g->adjList_H = (GraphNode**)calloc(sizeof(GraphNode*), g->n);
	g->visited = (int*)calloc(sizeof(int), g->n);
	g->coordList = (COORD*)malloc(sizeof(COORD) * g->n);
}

void setAdjList(int m)
{
	for (int i = 0; i < g->n; i++)
	{
		g->adjList_H[i] = NULL;
		for (int j = 0; j < m; j++)
		{
			if (adj[i][j] == -1) break;
			if (j == 0)
			{
				g->adjList_H[i] = (GraphNode*)malloc(sizeof(GraphNode));
				g->adjList_H[i]->vertex = adj[i][j];
				g->adjList_H[i]->link = NULL;
			}
			else
			{

				GraphNode* p = g->adjList_H[i];
				while (p->link != NULL)
					p = p->link;
				GraphNode* node = (GraphNode*)malloc(sizeof(GraphNode));
				node->vertex = adj[i][j];
				node->link = NULL;
				p->link = node;
			}
		}
	}
}

void setCoord()
{
	for (int i = 0; i < g->n; i++)
	{
		g->coordList[i].Y = coord[i][0];
		g->coordList[i].X = coord[i][1];
	}
}

void setMap(int n)
{
	createGraph(n);
	setAdjList(3);
	setCoord();
}

void setPlayer()
{
	numPlayer = 1;
	p[0].id = 1;
	p[0].curPos = 0;
	p[0].isCPU = 0;
	p[0].info = { 0, };
}

void showPlayer()
{
	for (int i = 0; i < numPlayer; i++)
	{
		int y = coord[p[i].curPos][0];
		int x = coord[p[i].curPos][1];
		SetCurrentCursorPos(2 * x, y);
		printf("��");
	}
}