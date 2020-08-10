//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"


#define GRIDW 35
#define BLACK  0
#define WHITE  1
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------






void __fastcall TFormMain::FormCreate(TObject *Sender)
{

      NewGame();
      GameStart=True;
}
//---------------------------------------------------------------------------
void TFormMain::NewGame()
{

      imgBoard->Picture->Assign(_imgBoard->Picture->Graphic);
      Chess.Init();
      GameStart=True;

}



void TFormMain::MakeMove(int i,int j,int StoneType)
{
      if(StoneType==BLACK)
        imgBoard->Canvas->StretchDraw(TRect(GRIDW*i+GRIDW/2,GRIDW*j+GRIDW/2,GRIDW*i+GRIDW/2+GRIDW,GRIDW*j+GRIDW/2+GRIDW),imgBlack->Picture->Bitmap);
      else
        imgBoard->Canvas->StretchDraw(TRect(GRIDW*i+GRIDW/2,GRIDW*j+GRIDW/2,GRIDW*i+GRIDW/2+GRIDW,GRIDW*j+GRIDW/2+GRIDW),imgWhite->Picture->Bitmap);
}

void TFormMain::UnMakeMove(int i,int j)
{

     imgBoard->Canvas->CopyRect(TRect(GRIDW*i+GRIDW/2,GRIDW*j+GRIDW/2,GRIDW*i+GRIDW/2+GRIDW,GRIDW*j+GRIDW/2+GRIDW)
          ,_imgBoard->Canvas,TRect(TRect(GRIDW*i+GRIDW/2,GRIDW*j+GRIDW/2,GRIDW*i+GRIDW/2+GRIDW,GRIDW*j+GRIDW/2+GRIDW)));

}

void __fastcall TFormMain::imgBoardMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
      if(GameStart)
      {
          if(X>GRIDW/2&&X<imgBoard->Width-GRIDW/2&&Y>GRIDW/2&&Y<imgBoard->Height-GRIDW/2)
          {
            int i= (X-GRIDW/2)/GRIDW;
            int j= (Y-GRIDW/2)/GRIDW;
            if(Chess.HasStone(i,j))return;
            MakeMove(i,j,!Chess.Bot);
            Chess.MakeMove(i,j,!Chess.Bot);
            if(Chess.GameIsOver())
            {
              ShowMessage("你胜了！");
              GameStart=False;
              return ;
            }
            aMove *bestmove=new aMove;
            Chess.SearchBestMove(bestmove);
            Chess.MakeMove(bestmove->i,bestmove->j,Chess.Bot);
            MakeMove(bestmove->i,bestmove->j,Chess.Bot);
            if(Chess.GameIsOver())
            {
              ShowMessage("你被电脑打败了！");
              GameStart=False;
              return ;
            }
            delete bestmove;
          }
      }
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::btnStartGameClick(TObject *Sender)
{
     NewGame();
     if(rb2->Checked)
     {
       Chess.Bot=BLACK;
       Chess.MakeMove(7,7,BLACK);
       MakeMove(7,7,BLACK);
     }
     else
       Chess.Bot=WHITE;
}
//---------------------------------------------------------------------------


