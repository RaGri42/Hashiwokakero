// Copyright 2021, Spaichingen
// Grimm-Engineering
// Author: Raphael Grimm <info@grimm-engineering.de>

#include "./Hashi.h"
#include <ncurses.h>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <deque>
// ____________________________________________________________
void Hashi::initializeScreen() {
  initscr();
  cbreak();
  noecho();
  curs_set(false);
  if (has_colors() == TRUE) {
    start_color();  //  Farben
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    clear();
    attron(COLOR_PAIR(0));
    }
  nodelay(stdscr, true);
  keypad(stdscr, true);
  mousemask(ALL_MOUSE_EVENTS, NULL);

//_NullY = (LINES / 2) - (3 * (_maxYFeld / 2)) ;
//_NullX = (COLS / 2) - (3 * (_maxXFeld / 2));
  _NullY = 0;
  _NullX = 0;
  _brueckenDeque.resize(4);
  std::vector<int> initVec = { 33, 33, 33, 33 };
  _brueckenDeque.push_front(initVec);
  
   std::vector<int> initVec2 = { 33, 33, 33};
  _inselDeque1.resize(3);
  _inselDeque1.push_front(initVec2);
  _inselDeque2.push_front(initVec2);
}
// ____________________________________________________________

void Hashi::processUserInput(int key) {
  MEVENT event;
   std::deque<int> test;
  switch (key) {
    case 's':
    mvprintw(15, 10, "Teste deque");
     test.push_front(12);
     test.push_front(13);
     test.push_front(14);
     test.push_front(15);
     if(test.size() > 2) {
       test.pop_back();
     }
     test.push_front(16);
    for (size_t i = 0; i < test.size(); i++) {
    mvprintw(16 + i, 12,"%d", test[i]);
    }
    break;
    case 'u':
    //undoMove();
    break;

    case KEY_MOUSE:
    if (getmouse(&event) == OK) {
      if (event.bstate & BUTTON1_CLICKED) {
          int lastClickedX = event.x - _NullX;
          int lastClickedY = event.y - _NullY;
//       mvprintw(12, 10, "%d,%d", lastClickedY, lastClickedX);
            checkBridges(lastClickedX, lastClickedY);
     }
    }
    break;
  }
  return;
}
// ____________________________________________________________
void Hashi::readFile() {
  _inputFileName = "i044-n009-s10x09.xy";
  std::ifstream file(_inputFileName.c_str());
  if (!file.is_open()) {
    std::cerr << "Can't open file!"<< _inputFileName << std::endl;
    exit(1);
  }
  std::string line;
  // erste Zeile hier steht die Feldgrösse drin
  std::getline(file, line);
  size_t linecount = 0;
  std::vector<int> xyVektor;
  xyVektor.resize(2);
  while (true) {
    if (file.eof()) { break; }
    // erste Zeile? Lese Feld Koordinaten aus
    if (linecount == 0) {
      size_t pos0 = line.find(":");
      std::string s0 = line.substr(1, pos0-1);
      std::string s01 = line.substr(pos0 + 1, 2);
      _maxXFeld = atoi(s0.c_str());
      _maxYFeld = atoi(s01.c_str());
      linecount++;
    }
    std::getline(file, line);
    size_t pos1 = line.find(",");
    size_t pos2 = line.find(",", pos1 + 1);
    std::string s1 = line.substr(0, pos1);
    std::string s2 = line.substr(pos1 + 1, pos2-pos1-1);
    std::string s3 = line.substr(pos2 + 1, line.npos);
    // Int aus den Substrings extrahieren x, y, Bruecken
    int x = atoi(s1.c_str());
    int y = atoi(s2.c_str());
    int b = atoi(s3.c_str());
    // Test ob letzte Zeile eingelesen wurde
    if (x == 0 && y == 0 && b == 0) { break; }
    // Werte in eine unordererd Map einlesen
    // x und y als key
    _xislands[x].push_back(y);
    _yislands[y].push_back(x);
    // Triple x, b, z für y-Inseln
    _triple = {x , b , 0};
    _YIslands[y].push_back(_triple);
    _triple.clear();
    // Schreibe das x,y double in Vektor
    xyVektor = {x, y};
    // _alleWerte.push_back(xyVector);
    _allIslands[b].push_back(xyVektor);
}
}
// ____________________________________________________________
void Hashi::getBridges() {
// x-Bridges bestimmen
for (auto& pair : _xislands) {
  std::vector<int> v = pair.second;
  for (size_t j = 0; j < (v.size())-1; j++) {
  // Berechne Differenz zwischen 2 Werten in der unordered map
  int diff = v[j + 1] - v[j];
  // Berechne die Stellen wo Brücken in x-Richtung vorkommen können
    for (int i = 1; i <= diff -1; i++) {
        int bridgevalue = v[j] + i;
        _bridge.push_back(bridgevalue);
     }
  // Am Ende des Brückenvektors steht der Zustand, default = 0
    _bridge.push_back(0);
    _allXBridges[pair.first].push_back(_bridge);
    _bridge.clear();
  }
}
// y-Bridges bestimmen
for (auto& pair : _yislands) {
  std::vector<int> v =  pair.second;
  for (size_t j = 0; j < (v.size())-1; j++) {
  // Berechne Differenz zwischen 2 Werten in der unordered map
  int diff = v[j + 1] - v[j];
  // Berechne die Stellen wo Brücken in x-Richtung vorkommen können
  for (int i = 1; i <= diff -1; i++) {
    int bridgevalue = v[j] + i;
    _bridge.push_back(bridgevalue);
     }
        _bridge.push_back(0);
        _allYBridges[pair.first].push_back(_bridge);
        _bridge.clear();
  }
  }
}
// ____________________________________________________________
void Hashi::printIslands() {
  // durchlaufe alle Inselkoordinaten
  // gib Inselkoordinaten zu Häufigkeiten aus
  for (auto& pair : _YIslands) {
    for (auto& z : pair.second) {
      int color = 0;
      if(z[1] - z[2] == 0) { color = 1; }
      else if(z[1] - z[2] < 0) { color = 2; }
      else { color = 0; }
      attron(COLOR_PAIR(color)); 
      mvprintw (pair.first + _NullY , z[0] + _NullX , "%d" , z[1]);
      attroff(COLOR_PAIR(color));
      
      //  for (int dx = -1; dx <= 1; dx++) {
//    for (int dy = -1; dy <= 1; dy++) {
//      if (!(dx == 0 && dy ==0)) {
//       attron(A_REVERSE);
//        mvprintw((pair.first + _NullY + dy), (z[0] +_NullX  + dx), " ");
//      }
//      }
//  }
 }
//  }
}
//attroff(A_REVERSE);
}
// ____________________________________________________________
void Hashi::printBridges() {
  const char* bridge ="";
  // durchlaufe alle Inselkoordinaten
  // gib Inselkoordinaten zu Häufigkeiten aus
  for (auto& pair : _allXBridges) {
    for (auto& z : pair.second) {
      if (z[z.size()-1] == 0) {bridge =" ";}
      else if (z[z.size()-1] == 1) {bridge = "|";}
      else if (z[z.size()-1] == 2) {bridge = "H";}

      for (size_t i = 0; i < z.size() -1; i++) {
      mvprintw(z[i] + _NullY , pair.first + _NullX , bridge);
  }
    }
}
  for (auto& pair : _allYBridges) {
    for (auto& z : pair.second) {
      if (z[z.size()-1] == 0) {bridge =" ";}
      else if (z[z.size()-1] == 1) {bridge = "-";}
      else if (z[z.size()-1] == 2) {bridge = "=";}
      for (size_t i = 0; i < z.size() - 1; i++) {
      mvprintw(pair.first + _NullY, z[i] + _NullX, bridge);
  }
  }
}
}
// ____________________________________________________________
void Hashi::checkBridges(int x, int y) {
  // Suche in den X-Brücken (momentan ist x fix) nach dem Wert
  if (_allXBridges.count(x) > 0) {
    for (size_t j = 0; j < _allXBridges[x].size(); j++) {
      for (size_t i = 0; i < _allXBridges[x][j].size()-1; i++) {
        if (_allXBridges[x][j][i] == y) {
       int undoZustand;
       int zustand;
       // erhöhe den Zustand der zugehörigen XInseln
    // y-Wert der Inse
  switch (_allXBridges[x][j][_allXBridges[x][j].size()-1]) {
    case 0:
      _allXBridges[x][j][_allXBridges[x][j].size()-1] = 1;
      // für undoMove
      _undoVec = {0, x, j, 0}; 
      zustand = 1;
      undoZustand = -1;
      break;
    case 1:
      _allXBridges[x][j][_allXBridges[x][j].size()-1] = 2;
      _undoVec = {0, x, j, 1}; 
      zustand = 1;
      undoZustand = -1;
      break;
    case 2:
      _allXBridges[x][j][_allXBridges[x][j].size()-1] = 0;
      _undoVec = {0, x, j, 2}; 
      zustand = -2;
      undoZustand = 2;
      break;
  }

        
      _brueckenDeque.push_front(_undoVec);
      _brueckenDeque.pop_back();
      _undoVec.clear();
        
        int yInselOben = _allXBridges[x][j][0] - 1;
        int yInselUnten = _allXBridges[x][j][_allXBridges[x][j].size()-2] + 1;
        changeStateIsland(x, yInselOben, zustand);
    //    setUndoCache(x, yInselOben, undoZustand, 0);
        changeStateIsland(x, yInselUnten, zustand);
    //    setUndoCache(x, yInselUnten, undoZustand, 1);
  // ____________________________________________________________
        }
        }
        }
    } else if (_allYBridges.count(y) > 0) {
    for (size_t j = 0; j < _allYBridges[y].size(); j++) {
      for (size_t i = 0; i < _allYBridges[y][j].size()-1; i++) {
        if (_allYBridges[y][j][i] == x) {
      int zustand;
      int undoZustand;
          // ____________________________________________________________
  switch (_allYBridges[y][j][_allYBridges[y][j].size()-1]) {
    case 0:
      _allYBridges[y][j][_allYBridges[y][j].size()-1] = 1;
      _undoVec = {1, y, j, 0};
      undoZustand = -1;
      zustand = 1;
      break;
    case 1:
       _allYBridges[y][j][_allYBridges[y][j].size()-1] = 2;
      _undoVec = {1, y, j, 1}; 
      undoZustand = -1;
      zustand = 1;
      break;
    case 2:
      _allYBridges[y][j][_allYBridges[y][j].size()-1] = 0;
      _undoVec = {1, y, j, 2}; 
      undoZustand = 2;
      zustand = -2;
      break;
  }

      _brueckenDeque.push_front(_undoVec);
      _brueckenDeque.pop_back();
        _undoVec.clear();
        int xInsellinks = _allYBridges[y][j][0] - 1;
        int xInselrechts = _allYBridges[y][j][_allYBridges[y][j].size()-2] + 1;
        
        changeStateIsland(xInsellinks, y, zustand);
  //      setUndoCache(xInsellinks, y, undoZustand, 0);
        
        changeStateIsland(xInselrechts, y, zustand );
//        setUndoCache(xInselrechts, y, undoZustand, 1);
        }
        }
        }
  }
        }
// ____________________________________________________________
void Hashi::changeStateIsland(int x, int y, int z ) {
  if (_YIslands.count(y) > 0) {
  for (size_t j = 0; j < _YIslands[y].size(); j++) {
    if (_YIslands[y][j][0] == x) {
      
      _YIslands[y][j][2] += z;
  //  mvprintw(22,16, "Zustand der Insel %d"   ,_YIslands[y][j][2]);
    }
  }
  } else { std::cout << " Bruecke nicht vorhanden!" << std::endl;}
}

// ____________________________________________________________
//void Hashi::setUndoCache(int x, int y, int z, int step) {
//      
//      if (step == 0) {
//      _undoVecIsland = { x, y, z };
//      _inselDeque1.push_front(_undoVecIsland);
//      _inselDeque1.pop_back();
//      _undoVecIsland.clear();
//      }
//      if (step == 1) {
//      _undoVecIsland = { x, y, z };
//      _inselDeque2.push_front(_undoVecIsland);
//      _inselDeque2.pop_back();
//      _undoVecIsland.clear();
//      }
//}
//  
//// ____________________________________________________________
//void Hashi::undoMove() {
//  
//  std::vector<int> v = _brueckenDeque.at(0);
//  std::vector<int> v1;
//  std::vector<int> v2;
//    v1 = _inselDeque1.at(0);
//    v2 = _inselDeque2.at(0);
//  std::vector<int> nullVec = { 33, 33, 33, 33 };
//  std::vector<int> nullVec1 = { 33, 33, 33 };
//
//  if(_brueckenDeque.empty() || _brueckenDeque[0][0] == 33) {
//  mvprintw(8,20, "kein Undo Move mehr uebrig!");
//  return;
//  }
//  
//
//  if(v.at(0) == 0) {
//      // setzte X-Bruecken zurueck auf undoVector Werte
//      _allXBridges[v[1]][v[2]][_allXBridges[v[1]][v[2]].size()-1] = v[3];
//      // check die Brucken mit x   y und altem Zustand     
//       changeStateIsland(v1[0], v1[1], v1[2]);
//       changeStateIsland(v2[0], v2[1], v2[2]);
//  
//  }
//    if(v.at(0) == 1) {
//      // setzte Y-Bruecken zurueck auf undoVector Werte
//      _allYBridges[v[1]][v[2]][_allYBridges[v[1]][v[2]].size()-1] = v[3];
//       changeStateIsland(v1[0], v1[1], v1[2]);
//       changeStateIsland(v2[0], v2[1], v2[2]);
//     }
//      
//      
//      
// //     _YIslands[v1[0]][v1[1]][2] = v1[2];
// //      mvprintw(14,12,"_YIslands[v1[0]][v1[1]][2] = %d",_YIslands[v1[0]][v1[1]][2] = v1[2]);
//      
// //     _YIslands[v2[0]][v1[1]][2] = v2[2];
// //     mvprintw(14,12,"_YIslands[v2[0]][v2[1]][2] = %d",_YIslands[v2[0]][v2[1]][2] = v2[2]);
// //printBridges();
//
//     _brueckenDeque.pop_front();
//    _brueckenDeque.push_back(nullVec);
//    
////      _inselDeque1.pop_front();
////      _inselDeque1.push_back(nullVec1);
//////    
////      _inselDeque2.pop_front();
////      _inselDeque2.push_back(nullVec1);
//}
// ____________________________________________________________
void Hashi::playGame() {
  
  initializeScreen();
  while (true) {
  int key  = getch();
  processUserInput(key);
  printBridges();
  mvprintw(18, 22, "refrsh der Bridges");
  printIslands();
  mvprintw(18, 22, "XXX");
  refresh();
//  mvprintw(12, 20, "%d:%d", _NullY, _NullX);
//  mvprintw(13, 20, "%d:%d", COLS, LINES);
//  mvprintw(14, 20, "%d:%d", _maxYFeld, _maxXFeld);
}
endwin();
}
