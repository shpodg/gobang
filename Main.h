//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <jpeg.hpp>

#include "ChessFive.cpp"
//---------------------------------------------------------------------------

ChessFive Chess;
class TFormMain : public TForm
{
__published:	// IDE-managed Components
        TImage *imgBG;
        TImage *imgBoard;
        TButton *btnStartGame;
        TImage *imgBlack;
        TImage *imgWhite;
        TImage *_imgBoard;
        TRadioButton *rb1;
        TRadioButton *rb2;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall imgBoardMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnStartGameClick(TObject *Sender);
private:	// User declarations
        bool GameStart;


public:		// User declarations
        __fastcall TFormMain(TComponent* Owner);
        void MakeMove(int i,int j,int StoneType);
        void UnMakeMove(int i,int j);
        void NewGame();



};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;

//---------------------------------------------------------------------------
#endif
