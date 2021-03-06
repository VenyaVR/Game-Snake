
#include       "C:\dllBridge\Lego\WinMain.h"  
using namespace Lego;

#define  body 1
#define apple 10

#include <time.h>

const short int FieldX    = 24;
const short int FieldY    = 24;
const short int size     = 500;
const short int SizeCells = 20;

HPEN     pen;
HBRUSH brush;

const int Red   = RGB(255, 51, 51);
const int Green = RGB(  0, 204, 0);
const int Gray  = RGB( 96, 96, 96);
const int Black = RGB(  0,  0,  0);

const short int SizeArray = 25; 
int    Field[SizeArray][SizeArray];
int OldField[SizeArray][SizeArray];

short int tail;
short int head;

short int NApples;
short int length  = 1;
short int score   = 0;

short int Press;

bool CreateApple = true;

int OldKeyPressed;

void Square(int x, int y, int Color)
{
	brush = CreateSolidBrush(Color);	
	SelectObject(hdc, brush);	
	
	Rectangle(hdc, x, y, x + SizeCells, y + SizeCells);	
}

struct Coordinates
{
	short int x;
	short int y;
		
};

int NfCV;
int NfC;

Coordinates Cells[SizeArray/* + 2*/][SizeArray/* + 2*/];
Coordinates CSnake[1000];
Coordinates CApple[1000];

void SetCoordinatesForCells()
{	
	int Fx = FieldX, 
		Fy = FieldY;
	
	for(int a = 0; a < SizeArray; a++)
	{	
		for(int b = 0; b < SizeArray; b++)
		{	
			for(int x = Fx; x < (Fx + SizeCells); x++)
				for(int y = Fy; y < (Fy + SizeCells); y++)
				{	
					Cells[a][b].x = Fx;
					Cells[a][b].y = Fy;	
				}
					
			Fx += SizeCells;
		}	
		Fy += SizeCells;
		Fx = FieldX;
	}	
}

void ChecksCells(int Color)
{	
	hdc = GetDC(hWnd);
	
	for(int a = 0; a < SizeArray; a++)	
		for(int b = 0; b < SizeArray; b++)
			if(OldField[a][b] != Field[a][b])
			{
				Square(Cells[a][b].x, Cells[a][b].y, Color);
				OldField[a][b] = Field[a][b];	
			}
					
	ReleaseDC(hWnd, hdc);
}

void DrawingField(int x, int y, int size)
{
			
	pen = CreatePen(PS_SOLID, 1, Gray);	
	SelectObject (hdc, pen);		
		
	x -= 5;
	y -= 5;
	size += 10;
	
	for(int i = 0; i < 2; i++)
	{
		MoveToEx(hdc, x, y + (i * size), NULL); 
		LineTo(hdc, x + size, y + (i * size));		
		
		MoveToEx(hdc, x + (i * size), y, NULL); 
		LineTo(hdc, x + (i * size), y + size);			
	}
	
	SetCoordinatesForCells();
	
	for(int i = 1; i < SizeArray - 1; i++)
	{	
		Square(Cells[0 ][i].x, Cells[0 ][i].y, Gray);
		Square(Cells[24][i].x, Cells[24][i].y, Gray);
		Square(Cells[i ][0].x, Cells[i ][0].y, Gray);
		Square(Cells[i][24].x, Cells[i][24].y, Gray);
	}
	
}


class Snake
{
public:
	
	int x;
	int y;
	
private:						
	
	bool stand;
	
	void Restart()
	{
		x = (SizeArray / 2) - 1;	
		y = (SizeArray / 2) - 1;	
		
		NApples = rand()%100;
		
		length = 1;
		score  = 0;
		tail   = 0;
		head   = 0;
		
		Key_Pressed = 0;
		
		NfC    = 0;
		NfCV   = 0;
	
		for(int i = 0; i < 1000; i++)
		{	
			CSnake[i].x = CApple[i].x = 0;
			CSnake[i].y = CApple[i].y = 0;
		}	
		
		hdc = GetDC(hWnd);
		
		for(int a = 0; a < SizeArray; a++)
			for(int b = 0; b < SizeArray; b++)
			{	
				Square(Cells[a][b].x, Cells[a][b].y, Black);	
			}
	
		DrawingField(FieldX, FieldY, size);
				
		ReleaseDC(hWnd, hdc);
			
		CreateApple = true;
	}
	
	void GameOver(int Color)
	{	
		hdc = GetDC(hWnd);

		for(int a = 1; a < SizeArray - 1; a++)
			for(int b = 1; b < SizeArray - 1; b++)	
			{	
				Field[a][b] = Color;
				Square(Cells[a][b].x, Cells[a][b].y, Field[a][b]);
		   	    Sleep(1);
			}
		
		Restart();
		
		ReleaseDC(hWnd, hdc);
	}	
	
	void Collision()
	{
		for(int i = tail; i < head; i++)
			if(CSnake[i].x == CSnake[NfCV].x && CSnake[i].y == CSnake[NfCV].y)
			{	
				GameOver(Red);
				Restart();		
			}
	}
	
	void AteApple()
	{			
		if(x == CApple[NfC].x && y == CApple[NfC].y )
		{	
			Beep(500, 50);
			length++;
			score += 5;
			
			NApples--;
								
			Field[x][y] = 0;
			CreateApple = true;
		}
	}
	
	
	void Drwing(int x, int y)
	{	
		hdc = GetDC(hWnd);

		Square(Cells[x][y].x, Cells[x][y].y, Green);	
		Field[x][y] = body;
		
		ReleaseDC(hWnd, hdc);
	}
	
	void Boundaries()
	{
		if(x <= 0 || x >= 24)
		{	
			Restart();	
			GameOver(Red);
		}	
		if(y <= 0 || y >= 24)
		{
			Restart();	
			GameOver(Red);
		}
	}
	
	void Motion()
	{	
		hdc = GetDC(hWnd);
						
		NfCV++;
		Press++;
			
		CSnake[NfCV].x = x;
		CSnake[NfCV].y = y;	
		
		head++; 
		tail = (head - (length - 1)) - 1;
		
		for(int i = tail + 1; i <= head; i++)
			Square(Cells[CSnake[i].x][CSnake[i].y].x, Cells[CSnake[i].x][CSnake[i].y].y, Green);	
		
		Square(Cells[CSnake[tail].x][CSnake[tail].y].x, Cells[CSnake[tail].x][CSnake[tail].y].y, Black);
		
		ReleaseDC(hWnd, hdc);
		
	}	
	
	
	void Control()
	{ 	
		for(int i = 37; i <= 40; i++)
			if(Key_Pressed == i)
				Motion();
		
		if(Key_Pressed == 37 || Key_Pressed == 38 || Key_Pressed == 39 || Key_Pressed == 40)
			OldKeyPressed = Key_Pressed;
		
		switch(Key_Pressed)
		{
			case 37: y--; break;	
			case 38: x--; break;
			case 39: y++; break;
			case 40: x++; break;
		}
	}
	
public:
	
	Snake()
	{
		x = (SizeArray / 2) - 1;	
		y = (SizeArray / 2) - 1;
	}
	
	void main()
	{	
		if(NApples == 0)
			GameOver(Green);
		
		Collision();
		Control();
		Drwing(x, y);
		AteApple();
		Boundaries();
	}

};
Snake snake;

class Apple
{

private:
	
	int randX,
		randY;

public: 
	
	void Drawing()
	{	
		hdc = GetDC(hWnd);
		Square(Cells[randX][randY].x, Cells[randX][randY].y, Red);
		ReleaseDC(hWnd, hdc);				
	}

private:
	
	void Coordinates()
	{	
		Repeat:
			
		srand(time(0));
		
		randX = rand()%SizeArray,
		randY = rand()%SizeArray;
		
		if(randX < 1 || randY < 1 || randX > 23 || randY > 23)
			goto Repeat;
				
		NfC++;
		Field[randX][randY] = apple; 
		CApple[NfC].x = randX;
		CApple[NfC].y = randY;
		
		Drawing();
	}

public:
	
	void main()
	{
		Coordinates();
	}
};

Apple apples;

class ControlBar
{
	
private:
	
	void CreateItems()
	{
		Button btn[10];	
		EditBox edt[10];
	}

	void Parameters()
	{
		Print_Set_Font(7);
		
		Print(550, 35, "Score: " , score);
		Print(550, 70, "Length: ", length);
		
		Print(550, 100, "Number of apples");
		Print(550, 120, "remaining: ", NApples);
		Print(546, 150, "(it's a random number)");
		
		Print(540, 185, "------Command-keys-----");		
		Print(540, 320, "--------Top-players-------");
		
		Print(550, 215, "In order to stop the");
		Print(550, 235, "game click 'space'");
		
		Print(550, 265, "To save, press 'F1'");
	}	

public:
	
	void Drawing(int x, int y)
	{	
		pen = CreatePen(PS_SOLID, 1, Gray);	
		SelectObject (hdc, pen);
		
		for(int i = 0; i < 2; i++)
		{
			MoveToEx(hdc, x + (i * 205), y, NULL); 
			LineTo(hdc, x + (i * 205), y + 510);		
			
			MoveToEx(hdc, x, y + (i * 510), NULL); 
			LineTo(hdc, x + 205, y + (i * 510));		
		}			
		
	}

	void main()
	{	
		Parameters();
		if(Timer_CLK < 5)
		{			
			CreateItems();	
		}
	}
	
}Bar;

void INIT()
{	
	srand(time(0));
	NApples = rand()%100;
}

int Pause = 1;

void CUTK()
{
	if(Key_Pressed == 32)
	{	
		Pause++;
		Key_Pressed = OldKeyPressed;
	}

}

void START()
{  	
	Bar.main();
	
	CUTK();
	
	if(Pause % 2 != 0)
	{	
		Print(550, 215, "In order to stop the");
		
		SetCoordinatesForCells(); 
			
		snake.main();
				
		if(CreateApple == true)
		{	
			apples.main();
			CreateApple = false;
		}
				
		if(CreateApple)
			apples.main();
			
		apples.Drawing();
	}
	else
		Print(550, 215, "In order to start the");
}













































//			                                                                   (   )       
//													                          (    )
//													                           (    )
//													                          (    )
//													                            )  )
//													                           (  (                  /\
//													                            (_)                 /  \  /\
//													                    ________[_]________      /\/    \/  \
//													           /\      /\        ______    \    /   /\/\  /\/\
//													          /  \    //_\       \    /\    \  /\/\/    \/    \
//													   /\    / /\/\  //___\       \__/  \    \/
//													  /  \  /\/    \//_____\       \ |[]|     \
//													 /\/\/\/       //_______\       \|__|      \
//													/      \      /XXXXXXXXXX\                  \
//													        \    /_I_II  I__I_\__________________\
//													               I_I|  I__I_____[]_|_[]_____I
//													               I_II  I__I_____[]_|_[]_____I
//													               I II__I  I     XXXXXXX     I
//					                                             E-mail:    dllbridge@gmail.com







//     � ���� ������� �� ������ ��������. �������� ��� �������� �������� BeginPaint() � ����� ��������� �������������.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////     void mainPAINT()    /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void mainPAINT()
{	
	Bar.Drawing(FieldX + 515, FieldY - 5);
	DrawingField(FieldX, FieldY, size);	
}








//     ��� ������� ���������� �� ������� ��������� WndProc(), ���� ������������ ������:
// 1)  �����-���� ����� ����.
// 2)  ����� ������.	

////////////////////////////////////////////////////            
void ScanButtonsAndMenuItems(int item)            //                  
{
	


 
}






