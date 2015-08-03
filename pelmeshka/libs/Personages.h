/*--------------------------------------------------------------------------------------------------------------------*/
//All personages for PelmenGame are drawn here, using TXLIB.h

void warrior(int x, int y, int r)
    {
    txSetColor (TX_BLACK);
    txSetFillColor (TX_RED);
    txRectangle (x, y, x+r/2, y+r);
    txCircle (x+r/4, y, r);
    txRectangle (x-r/6, y+r, x+r/6, y+r+r/3);
    txRectangle (x+r/3, y+r, x+r*2/3, y+r+r/3);
    txSetFillColor (TX_WHITE);
    txSetColor (TX_BLACK);
    txCircle (x+r/4, y, r/3);
    txSetFillColor (TX_BLACK);
    txCircle (x+r/4, y, r/6);
    }

void pelmen(int x, int y, int r, double eyeR, double eyeL, double morgR, double morgL)
    {
    txSetColor (TX_BLACK);
    txSetFillColor (TX_LIGHTGRAY);
    txCircle  (x, y, r);

    txSetFillColor (TX_RED);
    txCircle  (x-7, y-7, eyeL);
    txCircle  (x+7, y-7, eyeR);

    txSetFillColor (TX_LIGHTGRAY);
    txSetColor  (TX_LIGHTGRAY);
    txRectangle  (x-7-eyeL, y-morgL, x-7+eyeL, y+7);
    txRectangle  (x+7-eyeR, y-morgR, x+7+eyeR, y+7);



    }
