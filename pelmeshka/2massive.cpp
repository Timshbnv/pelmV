
#include "libs\TXLib.h"
#include "libs\Personages.h"

enum { WALL = 'X', SPACE = '-', WIN = '!', KNIFE = '|', NYAM = '+'};

const int SIZEB = 30;
const int MASSSIZE = 24;
const int MASSLAYER = 3;
const int MASSLEVEL = 2;

#define XM x / SIZEB
#define YM y / SIZEB

/*-----------------------------------------------PROTOTYPES----------------------------------------------------------*/

// typedef char [MASSSIZE][MASSSIZE] map_layer_t;
// typedef map_layer_t [MASSLAYER] map_t;
// void ReadKarta (map_t map, int lvl);
// void DrawKarta (int x, int y, int dx, int dy, map_layer_t map_layer);

void block (int x, int y, COLORREF color = TX_WHITE);
void ReadKarta (char map [MASSLAYER][MASSSIZE][MASSSIZE], int lvl);
void DrawKarta (int x0, int y0, int dx, int dy, char map[MASSLAYER][MASSSIZE][MASSSIZE], int layer, HDC FoodIMG, HDC WallIMG, HDC DoorIMG, HDC FloorIMG, HDC KnifIMG);
void MovePelByXY (int* x, int* y, int* vx, int* vy, char map [MASSLAYER][MASSSIZE][MASSSIZE], int layer);
int MovingOn(char allMap [MASSLAYER][MASSSIZE][MASSSIZE], int layer, int* pos, int x, int y, int* nyamka);
void Xfor (int koor, int how);

//choosing color for portals here:
const COLORREF MEOW_COLOR = TX_BLUE;
//every portal will be painted in this color




int main()
    {

	

    txCreateWindow (MASSSIZE * SIZEB, MASSSIZE * SIZEB);
    char map [MASSLAYER][MASSSIZE][MASSSIZE] = {};
	int lvl = 1;
	int winlose = 0;
	int lakomka = 0;

	while (true)
	{


		ReadKarta(map, lvl);

		int layer = 0;
		int pos = 1;
		int x = SIZEB * 4 - SIZEB / 2;
		int y = SIZEB * 4 - SIZEB / 2;
		txBegin();
		while (true)
		{

			int whrgo = MovingOn(map, layer, &pos, x, y, &lakomka);
			printf ("%d",lakomka);

			switch (pos)
			{
			case 1: x = SIZEB * MASSSIZE / 2;           y = SIZEB * MASSSIZE - SIZEB * 1.5; break;
			case 2: x = 1.5 * SIZEB;                    y = SIZEB * MASSSIZE / 2; break;
			case 3: x = SIZEB * MASSSIZE / 2;           y = SIZEB + SIZEB * 0.5; break;
			case 4: x = SIZEB * MASSSIZE - SIZEB * 1.5; y = SIZEB * MASSSIZE / 2; break;

			}

			switch (whrgo)
			{
			case 2: layer = 0; break;
			case 3: layer = 1; break;
			case 4: layer = 2; break;
			default: layer = 0;
			}

			if (whrgo == 0) { winlose = 0;  break; }
			else if (whrgo == 1) { winlose = 1;  break; }

		}
		txEnd();
		if (winlose == 0) { txMessageBox("LOSSE", "LOSSE2"); lakomka = 0; }
		if (winlose == 1) {txMessageBox("WINNY", "WINNY2"); lvl++;}
	}

}

int MovingOn (/* const */char allMap [MASSLAYER][MASSSIZE][MASSSIZE], int layer, int* pos, int x, int y, int* nyamka)
    {

	HDC WallImage = txLoadImage("images/DoorMetall.bmp");
	HDC DoorImage = txLoadImage("images/spinportal.bmp");
	HDC FloorImage = txLoadImage("images/plitkafloor.bmp");
	HDC FoodImage = txLoadImage("images/edaplitka.bmp");
	HDC KnifeImage = txLoadImage("images/plasma-bulb.bmp");

	int vy = 0;
    int vx = 0;
    int t = 0;
	int colo = TX_BLACK;
	HDC KNIF = KnifeImage;

    txBegin ();



    while (true)

        {
         txSetFillColor (MEOW_COLOR);
         txClear();

         DrawKarta (0, 0, 1 * SIZEB, 1 * SIZEB, allMap, layer, FoodImage, WallImage, DoorImage, FloorImage, KNIF);

		 if (t % 16 == 1)
		 {
			 if (KNIF == KnifeImage) KNIF = FloorImage;
			 else if (KNIF == FloorImage) KNIF = KnifeImage;
		 }

         MovePelByXY (&x, &y, &vx, &vy, allMap, layer);

         pelmen (x, y, 15, 6, 6, t % 15, t % 15);

         if      (allMap [layer][YM][XM] == 'w') *pos = 1;
         else if (allMap [layer][YM][XM] == 'v') *pos = 2;
         else if (allMap [layer][YM][XM] == 'y') *pos = 3;
         else if (allMap [layer][YM][XM] == 'z') *pos = 4;

		 if (allMap[layer][YM][XM] == NYAM)
		 {
			 allMap[layer][YM][XM] = SPACE;
			 *nyamka = *nyamka += 1;
		 }




         if (allMap [layer][YM][XM] == WIN) return 1;
         if (allMap [layer][YM][XM] == 'A') return 2;
         if (allMap [layer][YM][XM] == 'B') return 3;
         if (allMap [layer][YM][XM] == 'C') return 4;

		 if (allMap[layer][YM][XM] == KNIFE) 
			 if (KNIF == KnifeImage) return 0;
		 

         t++;
         txSleep (30);
        }



    txEnd();
	txDeleteDC (WallImage);
	txDeleteDC (DoorImage);
	txDeleteDC (FloorImage);
	txDeleteDC (FoodImage);
	txDeleteDC (KnifeImage);
    }

void ReadKarta (char map [MASSLAYER][MASSSIZE][MASSSIZE], int lvl)
    {


    char fname [50] = "";
    sprintf (fname, "levels/mapLvl%d.txt", lvl);  // curr dir

    FILE* f = fopen (fname, "r");
    if (!f) { printf ("%s: error: cannot open level file\n", fname); return; }

    int sizeX = 0, sizeY = 0, verMajor = 0, verMinor = 0;


    fscanf (f, "MEOW %d.%d %d %d", &verMajor, &verMinor, &sizeX, &sizeY);
    if (verMajor != 1 || verMinor != 0 || sizeX != MASSSIZE || sizeY != MASSSIZE)
        {
        printf ("%s: error: cannot open meowing header in the level file, "
                "format not recognized\n", fname);
        fclose (f);
        return;
        }
        for (int layer = 0; layer < 4; layer ++)
        {
            for (int y = 0; y < MASSSIZE; y++ )
            {
                for (int x = 0; x < MASSSIZE; x++ )
                {
                assert (0 <= x && x < MASSSIZE);
                assert (0 <= y && y < MASSSIZE);


                fscanf (f, " %c", &map [layer][y][x]); // fgetc



                }


            }
        }
    fclose (f);
    }


void DrawKarta (int x0, int y0, int dx, int dy, char map [MASSLAYER][MASSSIZE][MASSSIZE], int layer, HDC FoodIMG, HDC WallIMG, HDC DoorIMG, HDC FloorIMG, HDC KnifIMG)
    {

    for (int y = 0; y < MASSSIZE; y++ )
        for (int x = 0; x < MASSSIZE; x++ )
            {
            if (map [layer][y][x] == SPACE) txBitBlt(txDC(), x0 + x * dx, y0 + y * dy, x0 + x * dx + SIZEB, y0 + y * dy + SIZEB, FloorIMG, 0, 0);
            if (map [layer][y][x] == WALL)  txBitBlt(txDC(), x0 + x * dx, y0 + y * dy, x0 + x * dx + SIZEB, y0 + y * dy + SIZEB, WallIMG, 0, 0);
            if (map [layer][y][x] == WIN)   block (x0 + x * dx, y0 + y * dy, TX_LIGHTBLUE);
			if (map [layer][y][x] == KNIFE) txBitBlt(txDC(), x0 + x * dx, y0 + y * dy, x0 + x * dx + SIZEB, y0 + y * dy + SIZEB, KnifIMG, 0, 0);
			if (map [layer][y][x] == NYAM)  txBitBlt(txDC(), x0 + x * dx, y0 + y * dy, x0 + x * dx + SIZEB, y0 + y * dy + SIZEB, FoodIMG, 0, 0);

            if (map [layer][y][x] == 'w'  ||
                map [layer][y][x] == 'v'  ||
                map [layer][y][x] == 'y'  ||
                map [layer][y][x] == 'z') txBitBlt(txDC(), x0 + x * dx, y0 + y * dy, x0 + x * dx + SIZEB, y0 + y * dy + SIZEB, FloorIMG, 0, 0);




            if (map [layer][y][x] == 'A' ||
                map [layer][y][x] == 'B' ||
                map [layer][y][x] == 'C')  txBitBlt(txDC(), x0 + x * dx, y0 + y * dy, x0 + x * dx + SIZEB, y0 + y * dy + SIZEB, DoorIMG, 0, 0);
            
            }
    }








void MovePelByXY(int* x, int* y, int* vx, int* vy, char map[MASSLAYER][MASSSIZE][MASSSIZE], int layer)

{
#define RIGHTY GetAsyncKeyState(VK_RIGHT)
#define LEFTY  GetAsyncKeyState(VK_LEFT)
#define DOWNY  GetAsyncKeyState(VK_DOWN)
#define UPPY   GetAsyncKeyState(VK_UP)

	*x = *vx * 1 + *x;
	*y = *vy * 1 + *y;

	*vy = *vy * 0.8;
	*vx = *vx * 0.8;



	if (RIGHTY && map[layer][*y / SIZEB][*x / SIZEB + 1] != WALL) *vx += 2;
	else if (LEFTY && map[layer][*y / SIZEB][*x / SIZEB - 1] != WALL) *vx -= 2;
	else if (DOWNY && map[layer][*y / SIZEB + 1][*x / SIZEB] != WALL) *vy += 2;
	else if (UPPY && map[layer][*y / SIZEB - 1][*x / SIZEB] != WALL) *vy -= 2;

	if (*vx > 0) if (map[layer][*y / SIZEB][(*x + 14) / SIZEB] == WALL) *vx = -*vx;
	if (*vx < 0) if (map[layer][*y / SIZEB][(*x - 14) / SIZEB] == WALL) *vx = -*vx;
	if (*vy > 0) if (map[layer][(*y + 14) / SIZEB][*x / SIZEB] == WALL) *vy = -*vy;
	if (*vy < 0) if (map[layer][(*y - 14) / SIZEB][*x / SIZEB] == WALL) *vy = -*vy;


//	if (RIGHTY && UPPY || RIGHTY && DOWNY)



}

void Xfor(int koor, int how)
{
	for (int i = 0; i < 5; i++)
	{
		koor = koor + how;
		txSleep(6);
	}

}

void block (int x, int y, COLORREF color)
    {
    txSetColor (color);
    txSetFillColor (color);
    txRectangle (x, y, x+1*SIZEB, y+1*SIZEB);
    }
