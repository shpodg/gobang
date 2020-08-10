# 智能五子棋
开发语言：C++

开发工具：C++ Builder 6

## 算法
核心算法基于[Alpha-Beta](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning)剪枝。位于ChessFive.cpp的ChessFive::AlphaBeta中
```cpp
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

```

## 效果图
<div align="center">
    <img src=docs/gaming.jpg width=350 />
</div>