
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
void DrawKarta (int x, int y, int dx, int dy, char map[MASSLAYER][MASSSIZE][MASSSIZE], int layer, COLORREF knif);
void MovePelByXY (int* x, int* y, int* vx, int* vy, char map [MASSLAYER][MASSSIZE][MASSSIZE], int layer);
int MovingOn(char allMap [MASSLAYER][MASSSIZE][MASSSIZE], int layer, int* pos, int x, int y, int* nyamka);

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


    int vx = 0;
    int vy = 0;
    int t = 0;
	int colo = TX_BLACK;

    txBegin ();



    while (true)

        {
         txSetFillColor (MEOW_COLOR);
         txClear();

         DrawKarta (0, 0, 1 * SIZEB, 1 * SIZEB, allMap, layer, colo);

		 if (t % 16 == 1)
		 {	 
			 if (colo == TX_BLACK) colo = TX_GREEN;
			 else if (colo == TX_GREEN) colo = TX_BLACK;
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
		 {
			 if (colo == TX_GREEN) return 0;
		 }

         t++;
         txSleep (30);
        }
	 


    txEnd();
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


void DrawKarta (int x0, int y0, int dx, int dy, char map [MASSLAYER][MASSSIZE][MASSSIZE], int layer, COLORREF knif)
    {

    for (int y = 0; y < MASSSIZE; y++ )
        for (int x = 0; x < MASSSIZE; x++ )
            {
            if (map [layer][y][x] == SPACE) block (x0 + x * dx, y0 + y * dy, TX_BLACK);
            if (map [layer][y][x] == WALL)  block (x0 + x * dx, y0 + y * dy, TX_WHITE);
            if (map [layer][y][x] == WIN)   block (x0 + x * dx, y0 + y * dy, TX_LIGHTBLUE);
			if (map [layer][y][x] == KNIFE) block (x0 + x * dx, y0 + y * dy, knif);
			if (map [layer][y][x] == NYAM)  block(x0 + x * dx, y0 + y * dy, TX_RED);

            if (map [layer][y][x] == 'w'  ||
                map [layer][y][x] == 'v'  ||
                map [layer][y][x] == 'y'  ||
                map [layer][y][x] == 'z') block (x0 + x * dx, y0 + y * dy, TX_BLACK);




            /*if (map [layer][y][x] == 'A')   block (x0 + x * dx, y0 + y * dy, TX_ORANGE);
            if (map [layer][y][x] == 'B')   block (x0 + x * dx, y0 + y * dy, TX_PINK);
            if (map [layer][y][x] == 'C')   block (x0 + x * dx, y0 + y * dy, TX_YELLOW);
            */
            }
    }








void MovePelByXY (int* x, int* y, int* vx, int* vy, char map [MASSLAYER][MASSSIZE][MASSSIZE], int layer)

    {
     *x = *vx * 1 + *x;
     *y = *vy * 1 + *y;

     *vy = *vy * 0.8;
     *vx = *vx * 0.8;

     // printf ("x = %d, x/SIZEB = %d + %lg\n", *x, *x/SIZEB, *x/SIZEB - round (*x/SIZEB));

     if (GetAsyncKeyState (VK_RIGHT) && map [layer][ROUND (*y/SIZEB)]     [ROUND (*x/SIZEB + 1)] != WALL) *vx += 2;
     if (GetAsyncKeyState (VK_LEFT)  && map [layer][ROUND (*y/SIZEB)]     [ROUND (*x/SIZEB - 1)] != WALL) *vx -= 2;
     if (GetAsyncKeyState (VK_DOWN)  && map [layer][ROUND (*y/SIZEB + 1)] [ROUND (*x/SIZEB)]     != WALL) *vy += 2;
     if (GetAsyncKeyState (VK_UP)    && map [layer][ROUND (*y/SIZEB - 1)] [ROUND (*x/SIZEB)]     != WALL) *vy -= 2;

    }



void block (int x, int y, COLORREF color)
    {
    txSetColor (color);
    txSetFillColor (color);
    txRectangle (x, y, x+1*SIZEB, y+1*SIZEB);
    }
