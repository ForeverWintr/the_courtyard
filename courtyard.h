#ifndef COURTYARD_H
#define COURTYARD_H

#include <iostream>
#include <vector>

using namespace std;

class cell{
  int dragons; //randomly move dragons
  bool hero;
  bool princess;
  bool wall;
 public:
  cell(){dragons=0; wall=hero=princess=false;}
  int D(){return dragons;}
  bool H(){return hero;}
  bool P(){return princess;}
  void addD(){ dragons++;}
  void addP(){princess=true;}
  void addH(){hero=true;}
  void subD(){dragons--; if(dragons<0) dragons=0;}
  void subP(){princess=false;}
  void subH(){hero=false;}
  void isWall(){wall=true;}
  void print(){
    if(wall)
      cout<<"WW";
    else if(hero && dragons > 0)
      cout<<"HD"; //there's a fight goin down
    else if(hero && princess)
      cout<<"HP";
    else if(hero)
      cout<<"H]";
    else if(princess)
      cout<<"[P";
    else if(dragons == 0)
      cout<<"  ";
    else if(dragons < 10)
      cout<<dragons<<"D";
    else
      cout<<dragons;
  }
};


#endif
