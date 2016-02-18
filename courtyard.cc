//By Tom Rutherford

//courtyard has cells
//cells have dragons, hero bit, wall bit 
//hero has arrows, princess, direction
//dragons have direction

//draw in text
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "courtyard.h"

using namespace std;

void update(vector<vector<cell> > &court, int &arrows, int &dragons);
bool wall(int r, int c, int d, const vector<vector<cell> > &court);
void move(int r, int c, char unit, int d, vector<vector<cell> > &court, 
	  vector<vector<cell> > &newcourt);
void print(vector<vector<cell> > &court, int &time, int &arrows, int&);
void printCourt(vector<vector<cell> > &court);
void usage(){
  cerr<<"Usage: courtyard num_dragons interval(ms)"<<endl;
}



int main(int argc, char* argv[]){
  if(argc < 3){
    usage();
    exit(1);
  }
  
  vector<vector<cell> > court;
  int dragons=atoi(argv[1]);
  int interval=atoi(argv[2])*1000;
  int arrows=2; // who only brings 2 arrows to a dragon-filled courtyard!?
  int rr, rc; //random row and column
  
  srand(time(NULL));
  
  //initialize court
  court.resize(12); //court is padded to include walls
  for(int c=0; c<court.size(); c++)
    court[c].resize(12);

  //draw walls
  for(int r=0; r<court.size(); r++)
    for(int c=0; c<court[r].size(); c++)
      if(r == 0 || c ==0 || r == 11 || c == 11)
	court[r][c].isWall();

  //distribute dragons
  for(int d=0; d<dragons; d++){
    rr=(rand()%10)+1, rc=(rand()%10)+1;
    while( (rr == 1 && rc == 1) || (rr == 11 && rc == 11))
      //reassign if we've picked the hero or princess's spot      
      rr=(rand()%10)+1, rc=(rand()%10)+1; 
    
    //cout<<"adding dragon at ("<<rr<<", "<<rc<<")"<<endl;
    court[rr][rc].addD();
  }


  //install hero and princess
  court[1][1].addH();
  court[10][10].addP();

  //print the courtyard
  int t=0;
  while(true){
    if(arrows < 0){
      cout<<"The hero has been slain!"<<endl;
      exit(0);
    }
    
    print(court, t, arrows, dragons);
    if(court[1][1].P()){
      cout<<"The princess is safe!"<<endl;
      exit(0);
    }
    cout<<endl;
    update(court, arrows, dragons);
    usleep(interval);
    t++;
  }
  return 0;
}

//print courtyard and stats.
void print(vector<vector<cell> > &court, int &time, int &arrows, int& dragons){
  system("clear 2> /dev/null"); 
  cout<<"Time: "<<time<<" | Arrows Remaining: "<<arrows<<endl;
  cout<<"There are "<<dragons<<" dragons..."<<endl;
  printCourt(court);
}

//move everything
//read thru cells and move dragons. move hero down if PH is false, up if true
void update(vector<vector<cell> > &court, int& arrows, int &dragons){
  vector<vector<cell> > newcourt=court;
  int rd=0; //random direction, between 0 and 7

  //for each cell
  for(int r=0; r<court.size(); r++)
    for(int c=0; c<court[r].size(); c++){
      //if we have dragons and hero in the same square,
      //the hero must fight the dragons
      if(court[r][c].H() && court[r][c].D() > 0){
	while(court[r][c].D() > 0){
	  if(arrows < 0){//Dragon eats hero!
	    cout<<"The hero has been slain!"<<endl;
	    break;
	  }
	  arrows--;
	  dragons--;
	  court[r][c].subD();
	  newcourt[r][c].subD();
	}
      }

      //move dragons
      while(court[r][c].D() > 0){
	rd=rand()%8;                     
	
	while(wall(r,c,rd, court))//if we hit a wall, try again
	  rd=rand()%8;
	
	//move the dragon
       	move(r, c,'d', rd, court, newcourt);
      } //*/
      
      //move hero
      //if the hero is here and the princess isn't
      if(court[r][c].H() && !court[r][c].P()){ 
	//cout<<"moving hero from "<<r<<", "<<c<<endl;
	move(r, c, 'h', 7, court, newcourt);
      }
      else if(court[r][c].H() && court[r][c].P()){
	//move hero and princess back towards start
	move(r, c, 'h', 0, court, newcourt);
	move(r, c, 'p', 0, court, newcourt);
      }
        
    }
  court=newcourt;
}
  
//given row, column, and direction, move hero or dragon
// decrement oldcourt and increment newcourt
void move(int r, int c, char unit, int d, vector<vector<cell> > &court, 
	  vector<vector<cell> > &newcourt){  
  //cout<<"moving"<<endl;
  if(unit=='h'){
    court[r][c].subH();
    newcourt[r][c].subH();
    switch(d){
    case 0: 
      newcourt[r-1][c-1].addH();
      break;
    case 1:
      newcourt[r-1][c].addH();
      break;      
    case 2:
      newcourt[r-1][c+1].addH();
      break;      
    case 3:
      newcourt[r][c-1].addH();
      break;      
    case 4:
      newcourt[r][c+1].addH();
      break;      
    case 5:
      newcourt[r+1][c-1].addH();
      break;      
    case 6:
      newcourt[r+1][c].addH();
      break;      
    case 7:
      newcourt[r+1][c+1].addH();
      break;      
    }
  }
  else if(unit =='p'){
    court[r][c].subP();
    newcourt[r][c].subP();
    switch(d){
    case 0: 
      newcourt[r-1][c-1].addP();
      break;
    case 1:
      newcourt[r-1][c].addP();
      break;      
    case 2:
      newcourt[r-1][c+1].addP();
      break;      
    case 3:
      newcourt[r][c-1].addP();
      break;      
    case 4:
      newcourt[r][c+1].addP();
      break;      
    case 5:
      newcourt[r+1][c-1].addP();
      break;      
    case 6:
      newcourt[r+1][c].addP();
      break;      
    case 7:
      newcourt[r+1][c+1].addP();
      break;      
    }
  }
  else{
    court[r][c].subD();
    newcourt[r][c].subD();
    switch(d){
    case 0: 
      newcourt[r-1][c-1].addD();
      break;
    case 1:
      newcourt[r-1][c].addD();
      break;      
    case 2:
      newcourt[r-1][c+1].addD();
      break;      
    case 3:
      newcourt[r][c-1].addD();
      break;      
    case 4:
      newcourt[r][c+1].addD();
      break;      
    case 5:
      newcourt[r+1][c-1].addD();
      break;      
    case 6:
      newcourt[r+1][c].addD();
      break;      
    case 7:
      newcourt[r+1][c+1].addD();
      break;      
    }
  }
}


//will moving in direction d from row r and column c cause us to hit a wall?
bool wall(int r, int c, int d, const vector<vector<cell> > &court){
  //cout<<"is ("<<r<<", "<<c<<"), beside a wall in direction "<<d<<"?"<<endl;
  if(r==1 && d < 3)
    return true;
  if(r==court.size()-2 && d > 4)
    return true;

  if(c==1 && (d == 0 || d == 3 || d == 5))
    return true;
  if(c==court[0].size()-2 && (d == 2 || d == 4 || d == 7))
    return true;

  //special case for tower
  if( (r==1 && c==2 && d == 3) || (r==2 && c==2 && d == 0) || (r==2 && c==1 && d == 1))
    return true;
  
  // cout<<"...no"<<endl;
  return false;
}

void printCourt(vector<vector<cell> > &court){
  for(int r=0; r<court.size(); r++){
    for(int c=0; c<court[r].size(); c++){
      court[r][c].print();
      cout<<" ";
    }
    cout<<endl;
  }
  cout<<endl;
}
