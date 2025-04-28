enum SquareCodes {
  EMPTY,

  PAINT0, PAINT1, PAINT2, PAINT3,
  ABILITY0, ABILITY1, ABILITY2, ABILITY3,
  DRAW0,  DRAW1,  DRAW2,  DRAW3,
  
  P0D1,   P0D2,   P0D3,
  P1D0,   P1D2,   P1D3,
  P2D0,   P2D1,   P2D3,
  P3D0,   P3D1,   P3D2,

  ERRSQUARE
};

//Takes in the drawer and the painter of a square
//Returns the code that represents such square
char sqcode(int dr, int pt, bool ability){
  switch(pt){
    case -1:{
      switch(dr){
      case -1:{return ':'+SquareCodes::EMPTY; break;}
      case 0:{return ':'+SquareCodes::DRAW0; break;}
      case 1:{return ':'+SquareCodes::DRAW1; break;}
      case 2:{return ':'+SquareCodes::DRAW2; break;}
      case 3:{return ':'+SquareCodes::DRAW3; break;}
      default:{return ':'+SquareCodes::ERRSQUARE; break;}
    }
    }
    case 0:{
      if(ability) return SquareCodes::ABILITY0;
      switch(dr){
        case -1:{return ':'+SquareCodes::PAINT0; break;}
        case 1:{return ':'+SquareCodes::P0D1; break;}
        case 2:{return ':'+SquareCodes::P0D2; break;}
        case 3:{return ':'+SquareCodes::P0D3; break;}
        default:{return ':'+SquareCodes::ERRSQUARE; break;}
      }
    }
    case 1:{
      if(ability) return SquareCodes::ABILITY1;
      switch(dr){
        case -1:{return ':'+SquareCodes::PAINT1; break;}
        case 0:{return ':'+SquareCodes::P1D0; break;}
        case 2:{return ':'+SquareCodes::P1D2; break;}
        case 3:{return ':'+SquareCodes::P1D3; break;}
        default:{return ':'+SquareCodes::ERRSQUARE; break;}
      }
    }
    case 2:{
      if(ability) return SquareCodes::ABILITY2;
      switch(dr){
        case -1:{return ':'+SquareCodes::PAINT2; break;}
        case 0:{return ':'+SquareCodes::P2D0; break;}
        case 1:{return ':'+SquareCodes::P2D1; break;}
        case 3:{return ':'+SquareCodes::P2D3; break;}
        default:{return ':'+SquareCodes::ERRSQUARE; break;}
      }
    }
    case 3:{
      if(ability) return SquareCodes::ABILITY3;
      switch(dr){
        case -1:{return ':'+SquareCodes::PAINT3; break;}
        case 0:{return ':'+SquareCodes::P3D0; break;}
        case 1:{return ':'+SquareCodes::P3D1; break;}
        case 2:{return ':'+SquareCodes::P3D2; break;}
        default:{return ':'+SquareCodes::ERRSQUARE; break;}
      }
    }
    default:{
      return ':'+SquareCodes::ERRSQUARE;
    }
  }
}

enum UnitCodes {
  OWN0,     OWN1,     OWN2,     OWN3,
  OWN0UP,   OWN1UP,   OWN2UP,   OWN3UP,
  BUBBLE0,  BUBBLE1,  BUBBLE2,  BUBBLE3,
  BONUS,
  NOTHING,

  ERRUNIT
};

//Takes in unit parameters
//Returns the code that represents that unit
char ucode(bool isUnit, int pl = -1, bool upg = false){
  if(pl >= 0 and pl <= 3){
    switch (pl){
    case 0:
      if(not isUnit) return ':'+UnitCodes::BUBBLE0;
      else if(upg) return ':'+UnitCodes::OWN0UP;
      return ':'+UnitCodes::OWN0;
      break;
    case 1:
      if(not isUnit) return ':'+UnitCodes::BUBBLE1;
      else if(upg) return ':'+UnitCodes::OWN1UP;
      return ':'+UnitCodes::OWN1;
      break;
    case 2:
      if(not isUnit) return ':'+UnitCodes::BUBBLE2;
      else if(upg) return ':'+UnitCodes::OWN2UP;
      return ':'+UnitCodes::OWN2;
      break;
    case 3:
      if(not isUnit) return ':'+UnitCodes::BUBBLE3;
      else if(upg) return ':'+UnitCodes::OWN3UP;
      return ':'+UnitCodes::OWN3;
      break;
    default:
      return ':'+UnitCodes::ERRUNIT;
      break;
    }
  }
  if(isUnit) return ':'+UnitCodes::BONUS;
  return ':'+UnitCodes::NOTHING;
}

/*
Square info:

Empty

Painted by 0
Painted by 1
Painted by 2
Painted by 3

Drawed by 0
Drawed by 1
Drawed by 2
Drawed by 3

Painted by 0 and drawed by 1
Painted by 0 and drawed by 2
Painted by 0 and drawed by 3

Painted by 1 and drawed by 0
Painted by 1 and drawed by 2
Painted by 1 and drawed by 3

Painted by 2 and drawed by 0
Painted by 2 and drawed by 1
Painted by 2 and drawed by 3

Painted by 3 and drawed by 0
Painted by 3 and drawed by 1
Painted by 3 and drawed by 2

Unit info

Player:
Owned by 0
Owned by 1
Owned by 2
Owned by 3
Owned by 0 and upgraded
Owned by 1 and upgraded
Owned by 2 and upgraded
Owned by 3 and upgraded

Bubble:
Owned by 0
Owned by 1
Owned by 2
Owned by 3

Bonus

*/