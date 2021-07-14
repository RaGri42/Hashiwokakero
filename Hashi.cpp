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
// ____________________________________________________________
void Hashi::initializeScreen() {
  initscr();
  cbreak();
  noecho();
  curs_set(false);
  nodelay(stdscr, true);
  keypad(stdscr, true);
  mousemask(ALL_MOUSE_EVENTS, NULL);

  _NullY = (LINES / 2) - (_maxYFeld / 2) ;
  _NullX = (COLS / 2) - (_maxXFeld / 2);
}
// ____________________________________________________________

void Hashi::processUserInput(int key) {
  MEVENT event;
  switch (key) {
    case 's':
    mvprintw(15, 10, "Solver!");
    break;
    case KEY_MOUSE:
    if (getmouse(&event) == OK) {
      if (event.bstate & BUTTON1_CLICKED) {
          int lastClickedX = event.x - _NullX;
          int lastClickedY = event.y - _NullY;
       mvprintw(12, 10, "%d,%d", lastClickedX, lastClickedY);
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
      mvprintw (pair.first + _NullY , z[0] + _NullX , "%d" , z[1]);
//   for (int dx = -1; dx <= 1; dx++) {
//     for (int dy = -1; dy <= 1; dy++) {
//       if (!(dx == 0 && dy ==0)) {
//        attron(A_REVERSE);
//         mvprintw((z[1] + dy), (z[0]  + dx), " ");
//       }
//       }
//   }
//  attroff(A_REVERSE);
    //}
  }
}
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
    mvprintw(20, 10, "Ja man da ist das Ding!!");
    for (size_t j = 0; j < _allXBridges[x].size(); j++) {
      for (size_t i = 0; i < _allXBridges[x][j].size()-1; i++) {
        if (_allXBridges[x][j][i] == y) {
  // ____________________________________________________________
  if (_allXBridges[x][j][_allXBridges[x][j].size()-1] < 2) {
  // erhöhe den Zustand der Brücke wenn < 2, sonst setze 0
        _allXBridges[x][j][_allXBridges[x][j].size() - 1]++;
      } else {_allXBridges[x][j][_allXBridges[x][j].size()-1] = 0;}
  // erhöhe den Zustand der zugehörigen XInseln
  // y-Wert der Inse
        int yInselOben = _allXBridges[x][j][0] - 1;
        int yInselUnten = _allXBridges[x][j][_allXBridges[x][j].size()-2] + 1;
        int z = _allXBridges[x][j][_allXBridges[x][j].size()-1];
        changeStateIsland(x, yInselOben, z);
        changeStateIsland(x, yInselUnten, z);
  // ____________________________________________________________
        }
        }
        }
    } else if (_allYBridges.count(y) > 0) {
    for (size_t j = 0; j < _allYBridges[y].size(); j++) {
      for (size_t i = 0; i < _allYBridges[y][j].size()-1; i++) {
        if (_allYBridges[y][j][i] == x) {
        // ____________________________________________________________
      if (_allYBridges[y][j][_allYBridges[y][j].size()-1] < 2) {
      // erhöhe den Zustand der Brücke wenn < 2, sonst setze 0
        _allYBridges[y][j][_allYBridges[y][j].size()-1]++;
      } else {_allYBridges[y][j][_allYBridges[y][j].size()-1] = 0;}
      // erhöhe den Zustand der zugehörigen XInseln
      // y-Wert der Inse
        int xInsellinks = _allYBridges[y][j][0] - 1;
        int xInselrechts = _allYBridges[y][j][_allYBridges[y][j].size()-2] + 1;
        int z = _allYBridges[y][j][_allYBridges[y][j].size() - 1];
        changeStateIsland(xInsellinks, y, z);
        changeStateIsland(xInselrechts, y, z);
      // ____________________________________________________________
        }
        }
        }
  }
        }
// ____________________________________________________________
void Hashi::changeStateIsland(int x, int y, int z) {
  if (_YIslands.count(y) > 0) {
  for (size_t j = 0; j < _YIslands[y].size(); j++) {
    if (_YIslands[y][j][0] == x) {
      _YIslands[y][j][2] += z;
    }
  }
  } else { std::cout << " Bruecke nicht vorhanden!" << std::endl;}
}

// ____________________________________________________________
void Hashi::playGame() {
  
  initializeScreen();
  while (true) {
  int key  = getch();
  processUserInput(key);
  printBridges();
  refresh();
  printIslands();
  refresh();
}
endwin();
}
