#include <fstream>
#include <vector>
#include <algorithm>
#define BLACK  0
#define WHITE  1
#define  GRID_NUM  15
#define  NOSTONE   255
#define  STWO      1   //眠二
#define  STHREE    2   //眠三
#define  SFOUR     3   //冲四
#define  TWO       4   //活二
#define  THREE     5   //活三
#define  FOUR      6   //活四
#define  FIVE      7   //五连
#define  DFOUR     8   //双四
#define  FOURT     9   //四三
#define  DTHREE   10   //三三
#define  NOTYPE   11   //未定义
#define  ANALSISED 255  //已分析过的
#define  TOBEANALSIS 0  //未分析过的
//#include "Cell.cpp"
//#include "MoveGenerator.cpp"
using namespace std;
//ofstream log("log.txt");
int PosValue[GRID_NUM][GRID_NUM]=
{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
	{0,1,2,2,2,2,2,2,2,2,2,2,2,1,0},
	{0,1,2,3,3,3,3,3,3,3,3,3,2,1,0},
	{0,1,2,3,4,4,4,4,4,4,4,3,2,1,0},
	{0,1,2,3,4,5,5,5,5,5,4,3,2,1,0},
	{0,1,2,3,4,5,6,6,6,5,4,3,2,1,0},
	{0,1,2,3,4,5,6,7,6,5,4,3,2,1,0},
	{0,1,2,3,4,5,6,6,6,5,4,3,2,1,0},
	{0,1,2,3,4,5,5,5,5,5,4,3,2,1,0},
	{0,1,2,3,4,4,4,4,4,4,4,3,2,1,0},
	{0,1,2,3,3,3,3,3,3,3,3,3,2,1,0},
	{0,1,2,2,2,2,2,2,2,2,2,2,2,1,0},
	{0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
void memset(int *pi,int vl,int length)
{
     for(int i=0;i<length;i++)
       pi[i]=vl;
}

struct Cell
{
  Cell(int);
  ~Cell();
  int *Line,*Record;
  int Len;
  bool Analsised;
};
Cell::Cell(int length)
{
  Len=length;
  Line=new int[Len];
  Record=new int[Len];
  memset(Line,NOSTONE,Len);
  memset(Record,TOBEANALSIS,Len);
  Analsised=false;
}
Cell::~Cell()
{
  delete[]Line;
  delete[]Record;
}

struct aMove
{
  int i,j,score;
};
bool comp(aMove a,aMove b)
{
     return a.score>b.score;
}
void CreatePossibleMove(int *Board[GRID_NUM],vector<aMove> &MoveList)
{
     int i,j;
     for(i=0;i<GRID_NUM;i++)
       for(j=0;j<GRID_NUM;j++)
       if(Board[i][j]==NOSTONE)
       {
         aMove t={i,j,PosValue[i][j]};
         MoveList.push_back(t);
       }
     sort(MoveList.begin(),MoveList.end(),comp);
};
class ChessFive
{
  public:
    ChessFive();
    ~ChessFive();
    void Init();
    int Eveluate(bool bIsBlackTurn);
    void MakeMove(int i,int j,int StoneType);
    void UnMakeMove(int i,int j);
    bool HasStone(int i,int j);
    int AnalysisPos(Cell *cell, int StonePos);
    int *Board[15];
    void SearchBestMove(aMove *BestMove);
    bool GameIsOver();
    int Bot;
  private:
    int MaxDepth;
    aMove *BestMove;
    int AlphaBeta(int Depth,int Alpha,int Beta);
    void AnalysisLine(Cell *cell);
    int TypeCount[2][20];
    int Horizon,Vertical,Left,Right;
    //水平线 垂直线，左斜线，右斜线 偏移量 
    Cell *Cells[72];//所有的单元 
};

ChessFive::ChessFive()
{
  MaxDepth=3;
  Bot=WHITE;
  Horizon=0;Vertical=15;Left=30;Right=51;
  for(int i=0;i<GRID_NUM;i++)
  {
    Cells[i+Horizon]=new Cell(GRID_NUM);
    Cells[i+Vertical]=new Cell(GRID_NUM);
  }
  for(int i=0;i<10;i++)
  {
    Cells[i+Left]=new Cell(i+5);
    Cells[20-i+Left]=new Cell(i+5);
    Cells[i+Right]=new Cell(i+5);
    Cells[20-i+Right]=new Cell(i+5);
  }
  Cells[10+Left]=new Cell(15);
  Cells[10+Right]=new Cell(15);
  memset((int*)TypeCount,0, 40);
  for(int i=0;i<15;i++)
    Board[i]=Cells[i+Vertical]->Line;
  BestMove=new aMove;
}
ChessFive::~ChessFive()
{
  delete BestMove;
  for(int i=0;i<72;i++)
    delete Cells[i];
}
void ChessFive::Init()
{
   for(int i=0;i<72;i++)
   {
     memset(Cells[i]->Line,NOSTONE,Cells[i]->Len);
     Cells[i]->Analsised=false;
   }

}
bool ChessFive::GameIsOver()
{
    Eveluate(0);
    return TypeCount[0][FIVE]||TypeCount[1][FIVE];
}
void ChessFive::MakeMove(int i,int j,int StoneType)
{
  Cells[j+Horizon]->Line[i]=StoneType;
  Cells[j+Horizon]->Analsised=false;
  Cells[i+Vertical]->Line[j]=StoneType;
  Cells[i+Vertical]->Analsised=false;
  int idx=j-i;
  if(idx>=-10&&idx<=10)
  {
     Cells[10+idx+Left]->Line[(i>j?j:i)]=StoneType;
     Cells[10+idx+Left]->Analsised=false;
  }
  idx=i+j;
  if(idx>=4&&idx<=24)
  {
     Cells[idx-4+Right]->Line[(idx>14?14-i:j)]=StoneType;
     Cells[idx-4+Right]->Analsised=false;}
}
void ChessFive::UnMakeMove(int i,int j)
{
  Cells[j+Horizon]->Line[i]=NOSTONE;
  Cells[j+Horizon]->Analsised=false;
  Cells[i+Vertical]->Line[j]=NOSTONE;
  Cells[i+Vertical]->Analsised=false;
  int idx=j-i;
  if(idx>=-10&&idx<=10)
  {
     Cells[10+idx+Left]->Line[(i>j?j:i)]=NOSTONE;
     Cells[10+idx+Left]->Analsised=false;
  }
  idx=i+j;
  if(idx>=4&&idx<=24)
  {
     Cells[idx-4+Right]->Line[(idx>14?14-i:j)]=NOSTONE;
     Cells[idx-4+Right]->Analsised=false;
  }
}
void ChessFive::SearchBestMove(aMove *BMove)
{
     AlphaBeta(MaxDepth,-20000, 20000);
     BMove->i=BestMove->i;
     BMove->j=BestMove->j;
}
int ChessFive::AlphaBeta(int Depth,int Alpha,int Beta)
{
    if(Depth==0)return Eveluate((MaxDepth+Bot)%2);
    if(GameIsOver())return Eveluate((MaxDepth-Depth+Bot)%2);
    vector<aMove> MoveList;
    int vl;
    CreatePossibleMove(Board,MoveList);
    for(unsigned idx=0;idx<MoveList.size();idx++)
    {
      MakeMove(MoveList[idx].i,MoveList[idx].j,(MaxDepth-Depth+Bot)%2);
      vl=-AlphaBeta(Depth-1,-Beta,-Alpha);
      UnMakeMove(MoveList[idx].i,MoveList[idx].j);
      if(vl>Alpha)
      {
        if(Depth==MaxDepth)
        {
          BestMove->i=MoveList[idx].i;
          BestMove->j=MoveList[idx].j;
        }
        Alpha=vl;
      }
      if(vl>=Beta)return Beta;
    }
    return Alpha;
}


bool ChessFive::HasStone(int i,int j)
{
     return Board[i][j]!=NOSTONE;
}
int ChessFive::Eveluate(bool bIsBlackTurn)
{
    int i,j,nStoneType;
    memset((int*)TypeCount,0,40);
    for(i=0;i<72;i++)
      if(!Cells[i]->Analsised)
        AnalysisLine(Cells[i]);
    
    for(i=0;i<72;i++)
      for(j=0;j<Cells[i]->Len;j++)
      if((nStoneType=Cells[i]->Line[j])!= NOSTONE)
        switch(Cells[i]->Record[j])
        {
            case FIVE:
               TypeCount[nStoneType][FIVE] ++;
               break;
            case FOUR:
               TypeCount[nStoneType][FOUR] ++;
               break;
            case SFOUR:
               TypeCount[nStoneType][SFOUR] ++;
               break;
            case THREE:
               TypeCount[nStoneType][THREE] ++;
               break;
            case STHREE:
               TypeCount[nStoneType][STHREE] ++;
               break;
            case TWO:
               TypeCount[nStoneType][TWO] ++;
               break;
            case STWO:
               TypeCount[nStoneType][STWO] ++;
               break;
            default:
               break;
        }

             
     if (bIsBlackTurn)
     {
         if (TypeCount[BLACK][FIVE])
             return -9999;
         if (TypeCount[WHITE][FIVE])
             return 9999;
     }
     else
     {
         if (TypeCount[BLACK][FIVE])
             return 9999;
         if (TypeCount[WHITE][FIVE])
             return -9999;
     }
     if (TypeCount[WHITE][SFOUR] > 1)
		TypeCount[WHITE][FOUR]++;

     if (TypeCount[BLACK][SFOUR] > 1)
		TypeCount[BLACK][FOUR]++;


	int WValue=0,BValue=0;
	if (bIsBlackTurn)
	{
        if (TypeCount[WHITE][FOUR])//白胜
		{
			return 9990;
		}

		if (TypeCount[WHITE][SFOUR])//白胜
		{
			return 9980;
		}
        if (TypeCount[BLACK][FOUR] )//黑胜
		{
			return -9970;
		}
        if (TypeCount[BLACK][SFOUR] && TypeCount[BLACK][THREE])//黑胜
		{
			return -9960;
		}

		if (TypeCount[BLACK][THREE] > 1
			&&TypeCount[WHITE][SFOUR] == 0 &&
			TypeCount[WHITE][THREE] == 0 &&
			TypeCount[WHITE][STHREE] == 0 )
		{
			return -9940;
		}

		if (TypeCount[WHITE][THREE] && TypeCount[BLACK][SFOUR] == 0)
		{
			return 9950;
		}


		if (TypeCount[WHITE][THREE] > 1)
		        return 9930;       //	WValue += 2000;
		else{
			if (TypeCount[WHITE][THREE])
		           return 9920;
		}

		if (TypeCount[BLACK][THREE] > 1)
		        return -9930; //     BValue += 500;
		else{
			if (TypeCount[BLACK][THREE])
				BValue += 1000;//  100;
		}


		if (TypeCount[WHITE][STHREE])
			WValue += TypeCount[WHITE][STHREE]*10;

		if (TypeCount[BLACK][STHREE])
			BValue += TypeCount[BLACK][STHREE]*10;

		if (TypeCount[WHITE][TWO])
			WValue += TypeCount[WHITE][TWO]*4;

		if (TypeCount[BLACK][TWO])
			BValue += TypeCount[BLACK][TWO]*4;

		if (TypeCount[WHITE][STWO])
			WValue += TypeCount[WHITE][STWO];

		if (TypeCount[BLACK][STWO])
			BValue += TypeCount[BLACK][STWO];


	}
	else
	{

        if (TypeCount[BLACK][FOUR])
		{
			return 9990;
		}
		if (TypeCount[BLACK][SFOUR])
		{
			return 9980;
		}
		if (TypeCount[WHITE][FOUR])
		{
			return -9970;
		}
 	    if (TypeCount[WHITE][SFOUR] && TypeCount[WHITE][THREE])
		{
			return -9960;
		}
        if (TypeCount[WHITE][THREE] > 1 &&
             TypeCount[BLACK][SFOUR] == 0&&
             TypeCount[BLACK][THREE] == 0 &&
			 TypeCount[BLACK][STHREE] == 0)
		{
			return -9940;
		}

		if (TypeCount[BLACK][THREE] && TypeCount[WHITE][SFOUR] == 0)
		{
			return 9950;
		}

		
		if (TypeCount[BLACK][THREE] > 1)
		        return 9930;// BValue += 2000;
		else{
			if (TypeCount[BLACK][THREE])
			return 9920; //  BValue += 200;
		}

		if (TypeCount[WHITE][THREE] > 1)
		       	return -9930;//WValue += 500;
		else{
			if (TypeCount[WHITE][THREE])
				WValue +=1000; // 100;
		}

		

		if (TypeCount[WHITE][STHREE])
			WValue += TypeCount[WHITE][STHREE]*10;

		if (TypeCount[BLACK][STHREE])
			BValue += TypeCount[BLACK][STHREE]*10;

		if (TypeCount[WHITE][TWO])
			WValue += TypeCount[WHITE][TWO]*4;

		if (TypeCount[BLACK][TWO])
			BValue += TypeCount[BLACK][TWO]*4;

		if (TypeCount[WHITE][STWO])
			WValue += TypeCount[WHITE][STWO];

		if (TypeCount[BLACK][STWO])
			BValue += TypeCount[BLACK][STWO];

	}

	for (i=0; i<GRID_NUM; i++)
		for (j=0; j<GRID_NUM; j++)
		{
			nStoneType = Board[i][j];
			if (nStoneType != NOSTONE)
			{
		if (nStoneType == BLACK)
			 BValue += PosValue[i][j];
		else
			 WValue += PosValue[i][j];
				
			
			}
		}
		

	if (!bIsBlackTurn)
		return BValue - WValue;
	else
		return WValue - BValue;

}


void ChessFive::AnalysisLine(Cell *cell)
{
     memset(cell->Record, TOBEANALSIS,cell->Len);
     for(int i=0;i<cell->Len;i++)
       if(cell->Line[i]!=NOSTONE&&cell->Record[i]==TOBEANALSIS)
       AnalysisPos(cell,i);
     cell->Analsised=true;
}
int ChessFive::AnalysisPos(Cell *cell, int StonePos)
{
	int StoneType;
	int *AnalyLine=cell->Line;
	int *m_LineRecord=cell->Record;
	int nAnalyPos= StonePos;
	int LeftEdge, RightEdge;
	int LeftRange, RightRange;
	int GridNum=cell->Len;



	GridNum--;

	StoneType = AnalyLine[nAnalyPos];
	LeftEdge = nAnalyPos;
	RightEdge = nAnalyPos;

	while(LeftEdge > 0)
	{
		if (AnalyLine[LeftEdge-1] != StoneType)
			break;
		LeftEdge--;
	}

	while(RightEdge < GridNum)
	{
		if (AnalyLine[RightEdge+1] != StoneType)
			break;
		RightEdge++;
	}

	LeftRange = LeftEdge;
	RightRange = RightEdge;

	while(LeftRange > 0)
	{
		if (AnalyLine[LeftRange -1] == !StoneType)
			break;
		LeftRange--;
	}

	while(RightRange < GridNum)
	{
		if (AnalyLine[RightRange+1] == !StoneType)
			break;
		RightRange++;
	}

	if (RightRange - LeftRange < 4)
	{
		for (int k = LeftRange ; k <= RightRange; k++ )
			m_LineRecord[k] = ANALSISED;
		return false;
	}

	for (int k = LeftEdge; k <= RightEdge; k++ )
		m_LineRecord[k] = ANALSISED;

	if (RightEdge - LeftEdge > 3)//五连
	{
		m_LineRecord[nAnalyPos] = FIVE;
		return FIVE;
	}


	if (RightEdge - LeftEdge == 3)//4连
	{
		bool Leftfour = false;
		if (LeftEdge > 0 )
		{
			if (AnalyLine[LeftEdge-1] == NOSTONE)
				Leftfour = true;
		}

		if (RightEdge < GridNum)
		{
			if (AnalyLine[RightEdge+1] == NOSTONE)
			{
				if (Leftfour == true)
					m_LineRecord[nAnalyPos] = FOUR;
				else
					m_LineRecord[nAnalyPos] = SFOUR;
			}
			else
			{
				if (Leftfour == true)
					m_LineRecord[nAnalyPos] = SFOUR;
			}

		}else
		{
			if (Leftfour == true)
				m_LineRecord[nAnalyPos] = SFOUR;
		}

		return m_LineRecord[nAnalyPos];
	}



	if (RightEdge - LeftEdge == 2)//3连
	{
		bool LeftThree = false; 
		if (LeftEdge > 0 )
		{

			if (AnalyLine[LeftEdge-1] == NOSTONE)
			{
				if (LeftEdge > 1 && AnalyLine[LeftEdge-2] == AnalyLine[LeftEdge])
				{
					m_LineRecord[LeftEdge] = SFOUR;
					m_LineRecord[LeftEdge-2] = ANALSISED;
				}
				else
					LeftThree = true;
			}else
			{
				if (RightEdge == GridNum || AnalyLine[RightEdge+1] != NOSTONE)
					return false;
			}
		}

		if (RightEdge < GridNum)
		{
			if (AnalyLine[RightEdge+1] == NOSTONE)
			{
				if (RightEdge <GridNum -1 && AnalyLine[RightEdge+2] == AnalyLine[RightEdge])
				{
						m_LineRecord[RightEdge] = SFOUR;
						m_LineRecord[RightEdge+2] = ANALSISED;

				}
				else
				{
				if (LeftThree == true)
					m_LineRecord[RightEdge] = THREE;
				else
					m_LineRecord[RightEdge] = STHREE;
				}
			}
			else
			{
				if (m_LineRecord[LeftEdge] == SFOUR)
						return m_LineRecord[LeftEdge];
				if (LeftThree == true)
					m_LineRecord[nAnalyPos] = STHREE;
			}

		}
		else
		{
			if (m_LineRecord[LeftEdge] == SFOUR)
					return m_LineRecord[LeftEdge];
			if (LeftThree == true)
				m_LineRecord[nAnalyPos] = STHREE;
		
		}
		return m_LineRecord[nAnalyPos];
	}

	if (RightEdge - LeftEdge == 1)//2连
	{
		bool Lefttwo = false;
	       //	bool Leftthree = false;
		if (LeftEdge > 0 )
		{
			if (AnalyLine[LeftEdge-1] == NOSTONE)
			{
				if (LeftEdge-1 > 1 && AnalyLine[LeftEdge-2] == AnalyLine[LeftEdge])
				{
					if (AnalyLine[LeftEdge-3] == AnalyLine[LeftEdge])
					{
						m_LineRecord[LeftEdge-3] = ANALSISED;
						m_LineRecord[LeftEdge-2] = ANALSISED;
						m_LineRecord[LeftEdge] = SFOUR;
					}
					else 
						if (AnalyLine[LeftEdge-3] == NOSTONE)
						{
							m_LineRecord[LeftEdge-2] = ANALSISED;
							m_LineRecord[LeftEdge] = STHREE;
						}
				}
				else
				Lefttwo = true;
			}
		}
		if (RightEdge < GridNum)
		{
			if (AnalyLine[RightEdge+1] == NOSTONE)
			{
				if (RightEdge+1 < GridNum-1 && AnalyLine[RightEdge+2] == AnalyLine[RightEdge])
				{
					if (AnalyLine[RightEdge+3] == AnalyLine[RightEdge])
					{
							m_LineRecord[RightEdge+3] = ANALSISED;
							m_LineRecord[RightEdge+2] = ANALSISED;
							m_LineRecord[RightEdge] = SFOUR;
					}
					else 
						if (AnalyLine[RightEdge+3] == NOSTONE)
						{
							m_LineRecord[RightEdge+2] = ANALSISED;
							if (Lefttwo == true)
								m_LineRecord[RightEdge] = THREE;
							else
								m_LineRecord[RightEdge] = STHREE;
						}

				}
				else
				{
					if (m_LineRecord[LeftEdge] == SFOUR)
						return m_LineRecord[LeftEdge];
					if (m_LineRecord[LeftEdge] == STHREE)
					{
						m_LineRecord[LeftEdge] = THREE;
						return m_LineRecord[LeftEdge];
					}

					if (Lefttwo == true)
					m_LineRecord[nAnalyPos] = TWO;
				else
					m_LineRecord[nAnalyPos] = STWO;
				}
			}
			else
			{
					if (m_LineRecord[LeftEdge] == SFOUR)
						return m_LineRecord[LeftEdge];
				if (Lefttwo == true)
					m_LineRecord[nAnalyPos] = STWO;
			}

		}
		return m_LineRecord[nAnalyPos];
	}

	return 0;
}




