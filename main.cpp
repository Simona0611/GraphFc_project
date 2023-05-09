#include <iostream>
#include <stdio.h>
#include <fstream>

#include <cstring>
#include <cmath>
#include <windows.h>
#include <mmsystem.h>

#include <winbgim.h>
#include <graphics.h>

#include "tinyexpr.h"
#include "esential.h"

#define ROMANA 0
#define ENGLEZA 1
#define FRANCEZA 2
#define NOT_CLICK 0
#define CLICK 1
#define LGMAX 256
#define Latime 800
#define Inaltime 600
#define Ox 400
#define Oy 300

using namespace std;

ifstream fin("functie.in");

struct punct
{
    long double x, y;
};

struct buton
{
    char text[LGMAX];
    int marime;
    int x,y;
    int x1,y1,x2,y2;
};

char t[LGMAX], t_inf[LGMAX], t_post[LGMAX];
long double inceputInterval, finalInterval;
int zoomFactor=1;
int limbaProgram=ROMANA;

void citire();
void creareSistem();
void creareGraficSimplu();
void creareGraficAsimptote();
void creareVizualizatorPuncte();
void meniuStart();
void meniuSetari();
void iesire();
void legenda();
void initializareButon(buton& b, char text0[LGMAX], int x0, int y0, int m0);
void deseneazaButon(buton b, int stil);

double valoareFunctie(double x);


int main()
{
    initwindow(Latime, Inaltime);
    citire();
    PlaySound(TEXT("muzica_fundal.wav"), NULL, SND_FILENAME | SND_ASYNC);
    meniuStart();
    return 0;
}

void meniuStart()
{
    setbkcolor(CYAN);
    cleardevice();
    clearviewport();
    buton graficSimplu, graficAsimptote, vizualizarePuncte, setari, setareInterval;
    int x,y;
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 6);
    setcolor(WHITE);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    switch(limbaProgram)
    {
        case ENGLEZA:
            outtextxy(Ox, Oy-200, "Function graph");
            initializareButon(graficSimplu, "Simple graph", Ox, Oy-40, 4);
            initializareButon(graficAsimptote, "Graph with asymptotes", Ox, Oy+20, 4);
            initializareButon(vizualizarePuncte, "Graph points visualizer", Ox, Oy+80, 4);
            initializareButon(setari, "Settings", Ox, Oy+140, 4);
            break;
        case FRANCEZA:
            outtextxy(Ox, Oy-200, "Graphique de fonction");
            initializareButon(graficSimplu, "Graphique simple", Ox, Oy-40, 4);
            initializareButon(graficAsimptote, "Graphique avec asymptotes", Ox, Oy+20, 4);
            initializareButon(vizualizarePuncte, "Visualiseur de points de graphique", Ox, Oy+80, 4);
            initializareButon(setari, "R�glages", Ox, Oy+140, 4);
             break;
        default:
            outtextxy(Ox, Oy-200, "Graficul functiei");
            initializareButon(graficSimplu, "Grafic simplu", Ox, Oy-40, 4);
            initializareButon(graficAsimptote, "Grafic cu asimptote", Ox, Oy+20, 4);
            initializareButon(vizualizarePuncte, "Vizualizare puncte din grafic", Ox, Oy+80, 4);
            initializareButon(setari, "Setari", Ox, Oy+140, 4);
    }
    do
    {
        iesire();
        x=mousex();
        y=mousey();
        if(x>=graficSimplu.x1 && x<=graficSimplu.x2 && y>=graficSimplu.y1 && y<=graficSimplu.y2)
        {
            deseneazaButon(graficSimplu, CLICK);
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                creareSistem();
                creareGraficSimplu();
            }
        }
        else
            deseneazaButon(graficSimplu, NOT_CLICK);
        if(x>=graficAsimptote.x1 && x<=graficAsimptote.x2 && y>=graficAsimptote.y1 && y<=graficAsimptote.y2)
        {
            deseneazaButon(graficAsimptote, CLICK);
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                creareSistem();
                creareGraficAsimptote();
            }
        }
        else
            deseneazaButon(graficAsimptote, NOT_CLICK);
        if(x>=vizualizarePuncte.x1 && x<=vizualizarePuncte.x2 && y>=vizualizarePuncte.y1 && y<=vizualizarePuncte.y2)
        {
            deseneazaButon(vizualizarePuncte, CLICK);
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                creareSistem();
                creareVizualizatorPuncte();
            }
        }
        else
            deseneazaButon(vizualizarePuncte, NOT_CLICK);
        if(x>=setari.x1 && x<=setari.x2 && y>=setari.y1 && y<=setari.y2)
        {
            deseneazaButon(setari, CLICK);
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                meniuSetari();
            }
        }
        else
            deseneazaButon(setari, NOT_CLICK);
        clearmouseclick(WM_LBUTTONDOWN);
        delay(50);
    }while(true);

}

void meniuSetari()
{
    cleardevice();
    clearviewport();
    buton limbaRomana, limbaEngleza, limbaFranceza, rezolutie1, rezolutie2, rezolutie3;
    int x,y;
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 6);
    setcolor(WHITE);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    switch(limbaProgram)
    {
        case ENGLEZA:
            outtextxy(Ox, Oy-200, "Settings");
            settextstyle(EUROPEAN_FONT, HORIZ_DIR, 4);
            outtextxy(Ox-200, Oy-100, "Language:");
            initializareButon(limbaRomana, "Romanian", Ox-200, Oy-50, 3);
            initializareButon(limbaEngleza, "English", Ox-200, Oy, 3);
            initializareButon(limbaFranceza, "French", Ox-200, Oy+50, 3);
            break;
        case FRANCEZA:
            outtextxy(Ox, Oy-200, "R�glages");
            settextstyle(EUROPEAN_FONT, HORIZ_DIR, 4);
            outtextxy(Ox-200, Oy-100, "Langue:");
            initializareButon(limbaRomana, "Roumaine", Ox-200, Oy-50, 3);
            initializareButon(limbaEngleza, "Anglaise", Ox-200, Oy, 3);
            initializareButon(limbaFranceza, "Fran�aise", Ox-200, Oy+50, 3);
            break;
        default:
            outtextxy(Ox, Oy-200, "Setari");
            settextstyle(EUROPEAN_FONT, HORIZ_DIR, 4);
            outtextxy(Ox-200, Oy-100, "Limba:");
            initializareButon(limbaRomana, "Romana", Ox-200, Oy-50, 3);
            initializareButon(limbaEngleza, "Engleza", Ox-200, Oy, 3);
            initializareButon(limbaFranceza, "Franceza", Ox-200, Oy+50, 3);
    }
    switch(limbaProgram)
        {
            case ENGLEZA:
                deseneazaButon(limbaEngleza, CLICK);
                break;
            case FRANCEZA:
                deseneazaButon(limbaFranceza, CLICK);
                break;
            default:
                deseneazaButon(limbaRomana, CLICK);
        }
    do
    {
        iesire();
        x=mousex();
        y=mousey();
        if(x>=limbaRomana.x1 && x<=limbaRomana.x2 && y>=limbaRomana.y1 && y<=limbaRomana.y2 && limbaProgram!=ROMANA)
        {
            deseneazaButon(limbaRomana, CLICK);
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                limbaProgram=ROMANA;
                meniuStart();
            }
        }
        else if(limbaProgram!=ROMANA)
            deseneazaButon(limbaRomana, NOT_CLICK);
        if(x>=limbaEngleza.x1 && x<=limbaEngleza.x2 && y>=limbaEngleza.y1 && y<=limbaEngleza.y2 && limbaProgram!=ENGLEZA)
        {
            deseneazaButon(limbaEngleza, CLICK);
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                limbaProgram=ENGLEZA;
                meniuStart();
            }
        }
        else if(limbaProgram!=ENGLEZA)
            deseneazaButon(limbaEngleza, NOT_CLICK);
        if(x>=limbaFranceza.x1 && x<=limbaFranceza.x2 && y>=limbaFranceza.y1 && y<=limbaFranceza.y2 && limbaProgram!=FRANCEZA)
        {
            deseneazaButon(limbaFranceza, CLICK);
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                limbaProgram=FRANCEZA;
                meniuStart();
            }
        }
        else if(limbaProgram!=FRANCEZA)
            deseneazaButon(limbaFranceza, NOT_CLICK);
        clearmouseclick(WM_LBUTTONDOWN);
        delay(50);
    }while(true);
}

void iesire()
{
    char tasta;
    if (kbhit())
    {
        tasta=getch();
        if(tasta==27)
        {
            closegraph();
            exit(0);
        }
        if(tasta==8)
        {
            meniuStart();
        }
  }
}

void initializareButon(buton& b, char text0[LGMAX], int x0, int y0, int m0)
{
    strcpy(b.text,text0);
    b.x=x0;
    b.y=y0;
    b.marime=m0;
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, b.marime);
    b.x1=x0-textwidth(b.text)/2-5;
    b.x2=x0+textwidth(b.text)/2+5;
    b.y1=y0-textheight(b.text)/2-5;;
    b.y2=y0+textheight(b.text)/2+5;
}

void deseneazaButon(buton b, int stil)
{
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, b.marime);
    setcolor(WHITE);
    setfillstyle(EMPTY_FILL,CYAN);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    if (stil==NOT_CLICK)
    {
        setbkcolor(CYAN);
        setcolor(YELLOW);
        outtextxy((b.x1+b.x2)/2,(b.y1+b.y2)/2+2,b.text);
     }
     else
     {
        setbkcolor(CYAN);
        setcolor(BLACK);
        outtextxy((b.x1+b.x2)/2+2,(b.y1+b.y2)/2+4,b.text);
     }
}

void citire()
{
    fin.getline(t, LGMAX);
    fin>>inceputInterval>>finalInterval;
}

void legenda()
{
    rectangle(10, 10, 250, 100);
    switch(limbaProgram)
    {
        case ENGLEZA:
            settextstyle(COMPLEX_FONT, HORIZ_DIR, 1);
            outtextxy(65, 15, "Legend:");
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
            outtextxy(55, 40, "Asymptote:");
            setlinestyle(1,1,1);
            setcolor(RED);
            line(100, 35, 130, 35);
            setcolor(BLACK);
            setlinestyle(0,1,1);
            outtextxy(68, 70, "Measuring unit:");
            line(130, 65, 140, 65);
            line(130, 67, 130, 63);
            line(140, 67, 140, 63);
            break;
        case FRANCEZA:
            settextstyle(COMPLEX_FONT, HORIZ_DIR, 1);
            outtextxy(65, 15, "L�gende:");
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
            outtextxy(55, 40, "Asymptote:");
            setlinestyle(1,1,1);
            setcolor(RED);
            line(100, 35, 130, 35);
            setcolor(BLACK);
            setlinestyle(0,1,1);
            outtextxy(75, 70, "Unit� de mesure:");
            line(140, 65, 150, 65);
            line(140, 67, 140, 63);
            line(150, 67, 150, 63);
            break;
        default:
            settextstyle(COMPLEX_FONT, HORIZ_DIR, 1);
            outtextxy(65, 15, "Legenda:");
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
            outtextxy(55, 40, "Asimptota:");
            setlinestyle(1,1,1);
            setcolor(RED);
            line(100, 35, 130, 35);
            setcolor(BLACK);
            setlinestyle(0,1,1);
            outtextxy(85, 70, "Unitate de masura:");
            line(160, 65, 170, 65);
            line(160, 67, 160, 63);
            line(170, 67, 170, 63);
    }

}

void creareSistem()
{
    cleardevice();
    clearviewport();
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    setcolor(BLACK);
    setlinestyle(0,1,1);
    line(100, Oy, Latime-100, Oy);
    line(Ox, 100, Ox, Inaltime-100);
    line(Ox-10, 120, Ox, 100);
    line(Ox+10, 120, Ox, 100);
    line(Latime-120, Oy-10, Latime-100, Oy);
    line(Latime-120, Oy+10, Latime-100, Oy);
    for(int i=100; i<=Latime-130; i+=10)
        line(i, Oy-2, i, Oy+2);
    for(int i=130; i<=Inaltime-110; i+=10)
        line(Ox-2, i, Ox+2, i);
    legenda();
}

void creareGraficSimplu()
{
    punct x,y;
    y.x=inceputInterval;
    y.y=valoareFunctie(inceputInterval);
    for(double i=inceputInterval+0.01;i<=finalInterval;i+=0.01)
    {
        setlinestyle(0,1,1);
        x.x=i;
        x.y=valoareFunctie(x.x);
        ///Debug
        cout<<y.x<<" si "<<y.y<<endl;
        ///
        if((Oy-(abs(y.y)*10)+(Oy-(abs(x.y)*10)))<40 && (abs(y.x)-abs(x.y))<=0.0001)
        else
            line(Ox+(y.x*10), Oy-(y.y*10), Ox+(x.x*10), Oy-(x.y*10));
        y=x;
    }
    outtextxy(Ox+15, Oy+20, "O");
    do
    {
        iesire();
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            setviewport(200, 200, 400, 400, 1);
            creareGraficSimplu();
        }
    }while(true);
}

void creareGraficAsimptote()
{
    punct x,y;
    y.x=inceputInterval;
    y.y=valoareFunctie(inceputInterval);
    for(double i=inceputInterval+0.01;i<=finalInterval;i+=0.01)
    {
        setlinestyle(0,1,1);
        x.x=i;
        x.y=valoareFunctie(x.x);
        ///Debug
        cout<<y.x<<" si "<<y.y<<endl;
        ///
        if((Oy-(abs(y.y)*10)+(Oy-(abs(x.y)*10)))<40 && (abs(y.x)-abs(x.y))<=0.0001)
        {
            setcolor(WHITE);
            setlinestyle(1,1,1);
            line(Ox+(y.x*10), Oy-(y.y*10), Ox+(x.x*10), Oy-(x.y*10));
            setcolor(BLACK);
        }
        else
            line(Ox+(y.x*10), Oy-(y.y*10), Ox+(x.x*10), Oy-(x.y*10));
        y=x;
    }
    ///Refacere asimptote verticale deasupra liniilor trecute
    y.x=inceputInterval;
    y.y=valoareFunctie(inceputInterval);
    for(double i=inceputInterval+0.01;i<=finalInterval;i+=0.01)
    {
        setlinestyle(0,1,1);
        x.x=i;
        x.y=valoareFunctie(x.x);
        if((Oy-(abs(y.y)*10)+(Oy-(abs(x.y)*10)))<40 && (abs(y.x)-abs(x.y))<=0.0001)
        {
            setcolor(RED);
            setlinestyle(1,1,1);
            line(Ox+(y.x*10), Oy-(y.y*10), Ox+(x.x*10), Oy-(x.y*10));
            setcolor(BLACK);

        }
        y=x;
    }
    outtextxy(Ox+15, Oy+20, "O");
    do
    {
        iesire();
    }while(true);
}

void creareVizualizatorPuncte()
{
    int numarAsimptoteVerticale=0;
    int AsimptotaVerticala[100];
    punct x,y;
    y.x=inceputInterval;
    y.y=valoareFunctie(inceputInterval);
    for(double i=inceputInterval+0.01;i<=finalInterval;i+=0.01)
    {
        setlinestyle(0,1,1);
        x.x=i;
        x.y=valoareFunctie(x.x);
        ///Debug
        cout<<y.x<<" si "<<y.y<<endl;
        ///
        if((Oy-(abs(y.y)*10)+(Oy-(abs(x.y)*10)))<40 && (abs(y.x)-abs(x.y))<=0.0001)
        else
            putpixel(Ox+(y.x*10), Oy-(y.y*10), WHITE);
        y=x;
    }
    outtextxy(Ox+15, Oy+20, "O");
    do
    {
        iesire();
    }while(true);
}

///Evaluator TinyExpr
double valoareFunctie(double x)
{
    te_variable vars[]={{"x", &x}};
    int err;
    te_expr *n=te_compile(t, vars, 1, &err);
    if(n)
    {
        const double h1=te_eval(n);
        double h1_copy = h1;
        if(h1==(1.0/0.0))
            h1_copy=999999;
        if(h1==-(1.0/0.0))
            h1_copy=-999999;
        ///Debug
        //cout<<err<<endl;
        ///
        return h1_copy;
        te_free(n);
    }
}
///
