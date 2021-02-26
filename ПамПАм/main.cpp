#include "TXLib.h"
#include <iostream>

using namespace std;

//////размерр//////
const int strok = 20;
const int ryadov = 20;

int getSosedi(int i, int j, bool bombs[strok][ryadov])
{
    int sosedi = 0;
    if (i > 0 &&            j > 0 &&          bombs[i-1][j-1])
        sosedi++;
    if (i > 0 &&                              bombs[i-1][j])
        sosedi++;
    if (i > 0 &&            j < ryadov - 1 && bombs[i-1][j+1])
        sosedi++;
    if (                    j < ryadov - 1 && bombs[i][j+1])
        sosedi++;
    if (i < strok - 1 &&    j < ryadov - 1 && bombs[i+1][j+1])
        sosedi++;
    if (i < strok - 1 &&                      bombs[i+1][j])
        sosedi++;
    if (i < strok - 1 &&    j > 0          && bombs[i+1][j-1])
        sosedi++;
    if (                    j > 0          && bombs[i][j-1])
        sosedi++;

    return sosedi;
}

int main()
    {
    txCreateWindow (strok * 40, ryadov * 40 + 40);
    txTextCursor (false);

    bool bombs[strok][ryadov];
    bool opened[strok][ryadov];
    bool flag[strok][ryadov];

    bool exit = false;
    bool Start = false;
    bool bombaishere = false;

    int frame = -1;

    HDC pic = txLoadImage ("pic.bmp");

    for (int i = 0; i < strok; i++)
    for (int j = 0; j < ryadov; j++)
    {
        bombs[i][j] = false;
        opened[i][j] = false;
        flag[i][j] = false;
    }

    for (int i = 0; i < strok; i++)
    for (int j = 0; j < ryadov; j++)
        if (random (0, 100) < 5)
            bombs[i][j] = true;

    //начало
    while (!GetAsyncKeyState (VK_ESCAPE) && !Start && !exit){
        txBegin();
        txSetFillColor(RGB(192,192,192));
        txSetColor(TX_BLACK);
        //играть
        txRectangle (100, 140, 240, 180);
        txDrawText (100, 140, 240, 180, "Играть");
        //выйти
        txRectangle (100, 240, 240, 280);
        txDrawText (100, 240, 240, 280, "Выйти");
        //нажатие на играть
        if (txMouseButtons() == 1 &&
            txMouseX() >= 100 &&
            txMouseX() <= 240 &&
            txMouseY() >= 140 &&
            txMouseY() <= 180){
                Start = true;
            }
        //нажатие на выхоъ==
        if (txMouseButtons() == 1 &&
            txMouseX() >= 100 &&
            txMouseX() <= 240 &&
            txMouseY() >= 240 &&
            txMouseY() <= 280){
                exit = true;
            }
        txEnd();
        txSleep(10);
    }
    txClear();
    txTextOut (100, 100, "Погоди...");
    txSleep(2000);

    while (!GetAsyncKeyState (VK_ESCAPE) && !exit){

        int x = txMouseX() / 40;
        int y = (txMouseY() - 40)  / 40;

        txClear();
        txBegin();

        //Открытие
        if (txMouseButtons() == 1 &&
            !opened[x][y])
            opened[x][y] = !opened[x][y];
            txSleep(100);

        if (txMouseButtons() == 2 &&
            opened[x][y] == false){

            int x = txMouseX() / 40;
            int y = (txMouseY() - 40)  / 40;
            flag[x][y] = !flag[x][y];
            txSleep(100);
        }
        //открытие соседних
        for (int i = 0;i < strok;i++)
        for (int j = 0;j < ryadov;j++){
            if (opened[i][j] && getSosedi(i, j, bombs) == 0)
            {
                if (j < ryadov - 1 &&
                    !bombs[i][j + 1])
                    opened[i][j + 1] = true;

                if (i < strok - 1 &&
                    j < ryadov - 1 &&
                    !bombs[i + 1][j + 1])
                    opened[i + 1][j + 1] = true;

                if (i < strok - 1 &&
                    !bombs[i + 1][j])
                    opened[i + 1][j] = true;

                if (i < strok - 1 &&
                    j > 0 &&
                    !bombs[i + 1][j - 1])
                    opened[i + 1][j - 1] = true;

                if (j > 0 &&
                    !bombs[i][j - 1])
                    opened[i][j - 1] = true;

                if (i > 0 &&
                    j > 0 &&
                    !bombs[i - 1][j - 1])
                    opened[i - 1][j - 1] = true;

                if (i > 0 &&
                    !bombs[i - 1][j])
                    opened[i - 1][j] = true;

                if (i > 0 &&
                    j < ryadov - 1 &&
                    !bombs[i - 1][j + 1])
                    opened[i - 1][j + 1] = true;
                }
            }

        //Рисование
        for (int i = 0;i < strok;i++)
        for (int j = 0;j < ryadov;j++){
            frame = 10;
            txTransparentBlt (txDC(), 40*i, 40*j + 40, 40, 40, pic, 40 * frame, 0, TX_RED);

            if (flag[i][j] == true)
                txTransparentBlt (txDC(), 40*i, 40*j + 40, 40, 40, pic, 40  * 11, 0, TX_RED);

            else if (opened[i][j])
            {
                if (bombs[i][j])
                {
                    bombaishere = true;
                    txTransparentBlt (txDC(), 40*i, 40*j + 40, 40, 40, pic, 40  * 9, 0, TX_RED);
                }
                else
                    txTransparentBlt (txDC(), 40*i, 40*j + 40, 40, 40, pic, 40  * getSosedi(i, j, bombs), 0, TX_RED);
            }

        }

        txEnd();
        txSleep(10);
    }
    txDeleteDC (pic);
    return 0;
}

