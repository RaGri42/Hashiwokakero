// Copyright 2021, Spaichingen
// Grimm-Engineering
// Author: Raphael Grimm <info@grimm-engineering.de>

#include "./Hashi.h"
#include <ncurses.h>
#include <getopt.h>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <deque>

// ____________________________________________________________
/* Funktion um Programm zu starten
 * Ein Inputfile im Format *.xy muss angegeben werden
 * Anzahl von Undoschritten kann festgelegt werden
*/
//

void Hashi::parseCommandLineArguments(int argc, char** argv) {
struct option options[] = {
  {"undo-steps", 1, NULL, 'u'},
  {NULL, 0, NULL, 0}
};
  optind = 1;
  // default
  _undoSteps = 5;
  _fileName = "";

  while (true) {
    char c = getopt_long(argc, argv, "u:", options, NULL);
      if (c == -1) { break; }
      switch (c) {
        case 'u':
          _undoSteps = atof(optarg);
          break;
        case '?':
        default:
          std::cout << "bloek" << std::endl;
      }
  }
  if (optind + 1 != argc) {
    printUsageAndExit();
    exit(1);
  }
  _fileName = argv[optind];
}
// ____________________________________________________________
/* Funktion gibt Fehlemeldung aus, wenn Programm mit falschen Parametern
 * aufgerufen wurde
*/
// ____________________________________________________________

void Hashi::printUsageAndExit() const {
  std::cerr << "Usage ./HashiMain [options] <inputfile>" << std::endl;
}
// ____________________________________________________________
/* Screen initialisieren für ncurses
*/
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
  // _NullX = (COLS / 2) - _maxXFeld;
  // _NullY = (LINES / 2) - _maxYFeld;
  _NullY = 0;
  _NullX = 0;
  _sizeFactor = 3;
  // Deques für die undo Funktion
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
    checkSolution();
    break;
    case 'u':
    // undoMove();
    break;
    case KEY_MOUSE:
    if (getmouse(&event) == OK) {
      if (event.bstate & BUTTON1_CLICKED) {
          int lastClickedX = ((event.x  - _NullX) / _sizeFactor);
          int lastClickedY = ((event.y  - _NullY) / _sizeFactor);
          mvprintw(22, 60, "y = %d,x = %d", lastClickedY, lastClickedX);
            checkBridges(lastClickedX, lastClickedY);
     }
    }
    break;
  }
  return;
}
// ____________________________________________________________
/* File einlesen wie in Kommandozeile angegeben
 */
// ____________________________________________________________

void Hashi::readFile() {
  // _fileName = "i044-n009-s10x09.xy";
  std::size_t found = _fileName.find("xy");
  int einleseCode;
  if (found != std::string::npos) {
    einleseCode = 0;
  }
  found = _fileName.find("plain");
  if (found != std::string::npos) {
    einleseCode = 1;
  }
  found = _fileName.find("xy.solution");
  if (found != std::string::npos) {
    einleseCode = 2;
  }



 
  std::ifstream file(_fileName.c_str());
  if (!file.is_open()) {
  std::cerr << "Can't open file!"<< _fileName << std::endl;
  exit(1);
  }
  std::string line;
  // erste Zeile hier steht die Feldgrösse drin
  std::getline(file, line);
  size_t linecount = 0;
  std::vector<int> xyVektor;
  xyVektor.resize(2);

  if (einleseCode == 0) {
  while (true) {
    if (file.eof()) { break;}
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

if(einleseCode == 1) {
    std::cout << "plain-Datei!" << std::endl;
    
    int x;
    int y;
    int b;
    int yline = 0;
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
    for(size_t i = 0; i < line.length(); ++i) {
      if (line.at(i) !=' ') {
      char wert = line.at(i);
      x = i;
      y = yline;
      b = wert - 48;
    // Test ob letzte Zeile eingelesen wurde
    // if (x == 0 && y == 0 && b == 0) { break; }
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
    yline++;
  }
}
}
// ____________________________________________________________
void Hashi::writeFile() {
  std::ofstream myfile;
  myfile.open ("solution.xy");
  
  myfile << "# (xy.solution)" << std::endl;
  myfile << "# x1, y1, x2, y2" << std::endl;
  for (auto& pair : _solBridges) {
    if (pair.first == 1) {
      myfile << "# Single bridge" << std::endl;
    }
    if (pair.first == 2) {
      myfile << "# Double bridge" << std::endl;
    }
    for (auto& vector : pair.second) {
        for (int j = 0; j < 4 ; j++) {
        myfile << vector.at(j) << (j < 3 ? "," : "");  
      }
        myfile << std::endl;
    }
  }

      myfile.close();
}





// ____________________________________________________________
/* Berechnet alle Brücken zwischen den Inseln
 * speichert diese für x und y in einer unordered_map
 * XBridges -> x = fix, YBridges -> y = fix
*/ 
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
      if (z[1] - z[2] == 0) { color = 1; }
      else if (z[1] - z[2] < 0) { color = 2; }
      else { color = 0; }
      attron(COLOR_PAIR(color));
      mvprintw (_sizeFactor * pair.first + _NullY , _sizeFactor * z[0] + _NullX , "%d" , z[1]);
      attroff(COLOR_PAIR(color));
      for (int dx = -1; dx <= 1; dx++) {
      for (int dy = -1; dy <= 1; dy++) {
      if (!(dx == 0 && dy ==0)) {
      // attron(A_REVERSE);
      mvprintw((_sizeFactor * pair.first + _NullY + dy), 
          (_sizeFactor * z[0] +_NullX  + dx), "+");
      }
      }
  }
 }
 }
}
// ____________________________________________________________
/* Zeichne angeklickte Bruecken
*/
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
      for (size_t j = 0; j < z.size() - 1 ; j++) {
      for (int i = -1; i < 3 ; ++i) {      
        mvprintw(_sizeFactor * z[j] + i + _NullY , _sizeFactor *  pair.first +_NullX, bridge);
      }
   }
  }
}
for (auto& pair : _allYBridges) {
  for (auto& z : pair.second) {
      if (z[z.size()-1] == 0) {bridge =" ";}
      else if (z[z.size()-1] == 1) {bridge = "-";}
      else if (z[z.size()-1] == 2) {bridge = "=";}
      for (size_t j = 0; j < z.size() - 1; j++) {
      mvprintw(_sizeFactor * pair.first + _NullY, _sizeFactor * z[j] + _NullX , bridge);
        }
    }
  }
for (auto& pair : _allCrossings) {
  for (auto& z : pair.second) {
      mvprintw(_sizeFactor * z + _NullY, _sizeFactor * pair.first + _NullX , "X");
    }
  }
}
// ____________________________________________________________
/* Findet Kreuzungen zwischen Brücken
 */
// ____________________________________________________________
void Hashi::checkCrossing() {
  for (auto& pair : _allXBridges) {                 // durchlaufe alle XBridges, 
      for (auto& y1 : pair.second) {                  // schaue ob die y-Werte der XBridges
        for (size_t j = 0; j < y1.size() - 1; j++) {
        int y2 = y1.at(j);                            // schaue ob die y-Werte der XBridges
        if (_allYBridges.count(y2)) {                 // als YBridges gibt
            _XCrossings[pair.first].push_back(y2);          // speichere gefundene y in unsorted_map
        }
      }
    }
 }
  for (auto& pair : _XCrossings) {                      // nimm die gefundenen Werte und schaue
    for (auto& ya : pair.second) {                      // ob der X-Wert auch in der Y-Bruecke vorkommt
      for (size_t j = 0; j < _allYBridges[ya].size(); j++) {  
        for (size_t i = 0; i < _allYBridges[ya][j].size() - 1; i++) {
             int xa = _allYBridges[ya][j][i];
             if (xa == pair.first) {
      // x kommt auch in YBridge vor
      // jetzt wird die Kreuzung in einer unordered_map gespeichert mit x als
      // key
      _allCrossings[pair.first].push_back(ya);
      _YCrossings[ya].push_back(pair.first);
          }
        }
      }
    }
  }
}
// ____________________________________________________________
/* Diese Funktion checkt ob eine Bruecke bei gegebenem x erlaubt ist
 * oder ob es schon bereits eine YBridge an dieser Stelle gibt
 * wenn es eine YBridge gibt, dann wuerden sich die Bruecken kreuzen
 * da dies verboten ist, kann die XBridge an dieser Stelle nicht gezichnte
 * werden
 */
// ____________________________________________________________
int Hashi::bridgeAllowedX(int x) {
  int sum = 0; 
  if (_allCrossings.count(x)) {
    for (auto& ywert : _allCrossings[x]) { 
      if (_allYBridges.count(ywert)) {
        for (int j = 0; j < _allYBridges[ywert].size(); j++) {
          for(int i = 0; i < _allYBridges[ywert][j].size() - 1; i++) {
            if (x == _allYBridges[ywert][j][i]) {
            // _XBridges[x][j][i] ist die Brücke die kreuzt
            if (_allYBridges[ywert][j][_allYBridges[ywert][j].size() - 1] == 0) {
            // Brueckenstatus == 0 --> Bruecke darf gebaut werden  
              sum += 0;
            } else {
            // Brueckenstatus der YBridge ist nicht null --> Bruecke darf nicht
            // gebaut werden
              sum += 1;
              }
            }
          }
        }
      }
    }
  }
  return sum;
}
// ____________________________________________________________
int Hashi::bridgeAllowedY(int y) {
  int sum = 0; 
  if (_YCrossings.count(y)) {
    for (auto& xwert : _YCrossings[y]) { 
      if (_allXBridges.count(xwert)) {
        for (int j = 0; j < _allXBridges[xwert].size(); j++) {
          for (int i = 0; i < _allXBridges[xwert][j].size() - 1; i++) {
            if (y == _allXBridges[xwert][j][i]) {
            // _XBridges[x][j][i] ist die Brücke die kreuzt
            if (_allXBridges[xwert][j][_allXBridges[xwert][j].size() - 1] == 0) {
            // Brueckenstatus == 0 --> Bruecke darf gebaut werden  
              sum += 0;
            } else {
            // Brueckenstatus der YBridge ist nicht null --> Bruecke darf nicht
            // gebaut werden
              sum += 1;
              }
            }
          }
        }
      }
    }
  }
  return sum;
}
// ____________________________________________________________
void Hashi::checkBridges(int x, int y) {
  int x1 = x;
  int check = 0; 
  // Pruefen ob Bruecken sich kreuzen
  // Vergroessteten Klickbereich abfragen
  if (_allXBridges.count(x)) {; }
   else if (_allXBridges.count(x-1)) { x = x -1; }
   else if ( _allXBridges.count(x + 1)) { x = x + 1; }
    
  if (_allXBridges.count(x)) {
     
  int rueck = 0;
    for (size_t j = 0; j < _allXBridges[x].size(); j++) {
      for (size_t i = 0; i < _allXBridges[x][j].size()-1; i++) {
        // checken ob das geklickte y zu einer XBridge gehoert.
        if (_allXBridges[x][j][i] == y) {
          // checken ob es eine kreuzende YBridge gibt
          rueck = bridgeAllowedX(x);
          // check dient dazu, dass die Routine für YBruecken nur aufgerufen
          // wird wenn es kein x, y Match für XBridges gibt
          check = 1;     
       int undoZustand;
       int zustand;
  // Aendere XBridges und dazugehörige Inseln
  if (rueck == 0) {
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
      // setUndoCache(x, yInselOben, undoZustand, 0);
      changeStateIsland(x, yInselUnten, zustand);
      }
    } 
  }
 }
}
// wird nur ausgeführt wenn kein x, y Paar bei allXBridges gefunden wurde  
  if (check == 0) {
int rueck = 0;
   if (_allYBridges.count(y) > 0 ) {;}
   else if (_allYBridges.count(y-1) > 0 ) { y = y -1; }
   else if ( _allYBridges.count(y + 1) > 0 ) { y = y + 1; }

   if ( _allYBridges.count(y) > 0 ) {
    for (size_t j = 0; j < _allYBridges[y].size(); j++) {
      for (size_t i = 0; i < _allYBridges[y][j].size()-1; i++) {
        if (_allYBridges[y][j][i] == x) {
          int zustand;
          int undoZustand;
          rueck = bridgeAllowedY(y);
          if (rueck != 0) {
          mvprintw(2, 50, "Bruecke verboten -> kreuzt");
          }
          // Zusstand der Bruecken in Abhaengigkeit vom Ist-Zustand veraendern
  if (rueck == 0) {
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
      changeStateIsland(xInselrechts, y, zustand);
            }
          }
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
    }
  }
  } else { std::cout << " Bruecke nicht vorhanden!" << std::endl;}
}
// ____________________________________________________________
void Hashi::checkSolution() {
  std::vector<int> zu;
  int sum = 0;
  for (auto& pair : _YIslands) {
    for (auto& islands : pair.second) {
    zu = islands;
    sum += (zu[2] - zu[1]);
  }
  }
  if (sum == 0) {
    mvprintw(4, 50 , "DAS SPIEL WURDE GELOEST!");
    getSolution();
    writeFile();
  } else { mvprintw(5, 50 , "DIESE LOESUNG IST LEIDER FALSCH!");}
}
// ____________________________________________________________
void Hashi::getSolution() {
  // alleInseln in unordered map schreiben
  // sortiert nach einfach, doppel
  // x1, y1, x2, y2
  std::vector<int> solVector;
  for (auto& ym : _allXBridges) {
    for (auto& yv : ym.second) {
      if (yv.at(yv.size() -1) == 1) {
      solVector.push_back(ym.first);
      solVector.push_back(yv.at(0) - 1);
      solVector.push_back(ym.first);
      solVector.push_back(yv.at(yv.size() - 2) + 1);
      
      _solBridges[1].push_back(solVector);
      solVector.clear();
      }  
      
      if (yv.at(yv.size() -1) == 2) {
      solVector.push_back(ym.first);
      solVector.push_back(yv.at(0) - 1);
      solVector.push_back(ym.first);
      solVector.push_back(yv.at(yv.size() - 2) + 1);
      _solBridges[2].push_back(solVector);
      solVector.clear();
      }  
    }
  }
  for (auto& xm : _allYBridges) {
    for (auto& xv : xm.second) {
      if (xv.at(xv.size() -1) == 1) {
      solVector.push_back(xm.first);
      solVector.push_back(xv.at(0) - 1);
      solVector.push_back(xm.first);
      solVector.push_back(xv.at(xv.size() - 2) + 1);
      
      _solBridges[1].push_back(solVector);
      solVector.clear();
      }  
      
      if (xv.at(xv.size() -1) == 2) {
      solVector.push_back(xm.first);
      solVector.push_back(xv.at(0) - 1);
      solVector.push_back(xm.first);
      solVector.push_back(xv.at(xv.size() - 2) + 1);
      _solBridges[2].push_back(solVector);
      solVector.clear();
      }  
    }
  }
}
// ____________________________________________________________
void Hashi::playGame() {
  initializeScreen();
  while (true) {
  int key  = getch();
  processUserInput(key);
  printBridges();
  printIslands();
  refresh();
//  mvprintw(12, 20, "%d:%d", _NullY, _NullX);
//  mvprintw(13, 20, "%d:%d", COLS, LINES);
//  mvprintw(14, 20, "%d:%d", _maxYFeld, _maxXFeld);
}
endwin();
}
