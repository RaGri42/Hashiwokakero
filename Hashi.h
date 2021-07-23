// Copyright 2021, Spaichingen
// Grimm-Engineering
// Author: Raphael Grimm <info@grimm-engineering.de>


#ifndef HASHI_H_
#define HASHI_H_

#include <ncurses.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <deque>
class Hashi {
 public:
  // Membervariablen
  int _maxXFeld;
  int _maxYFeld;
  // NullPunkt X des Spiels relativ zur Mitte
  int _NullX;
  // NullPunkt Y des Spiels relativ zur Mitte
  int _NullY;
  int _sizeFactor;
  // maximale Undoschritte
  int _undoSteps;
  std::string _fileName;
  // neu
  std::unordered_map<int, std::vector<std::vector<int>>> _YIslands;
  std::vector<int> _triple;
  std::vector<int> _bridge;
  std::vector<int> _undoVec;
  std::vector<std::vector<int>> _returnVec;
  
  std::vector <int> _vc1;
  std::vector <int> _vc2;
  std::vector<int> _vec;
  std::vector <std::vector<int>> _vecpair;
  
  std::unordered_map<int, std::vector<int>> _xislands;
  std::unordered_map<int, std::vector<int>> _yislands;
  std::unordered_map<int, std::vector<int>> _ybridges;
  std::unordered_map<int, std::vector<int>> _xbridges;
  // Speichert das gesamte Spielfeld mit Brückenanzahl als key
  std::unordered_map<int, std::vector<std::vector<int>>> _allIslands;
  std::unordered_map<int, std::vector<std::vector<int>>> _allXBridges;
  std::unordered_map<int, std::vector<int>> _allXBridges2;
  std::unordered_map<int, std::vector<std::vector<int>>> _allYBridges;
  
  std::unordered_map <int, std::vector <int>> _YCrossings;
  std::unordered_map <int, std::vector <int>> _XCrossings;
  
  std::unordered_map <int, std::vector<std::vector <int>>> _allCrossings;
  std::vector<std::vector<int>> _alleWerte;
  std::deque<std::vector<int>> _brueckenDeque;
  std::deque<std::vector<int>> _inselDeque1;
  std::deque<std::vector<int>> _inselDeque2;
  std::vector<int> _undoVecIsland;
  // Memberfunktionen
  void parseCommandLineArguments(int argc, char** argv);
  void printUsageAndExit() const;
  void playGame();
  void initializeScreen();
  void processUserInput(int key);
  void printIslands();
  void printBridges();
  void printField();
  void readFile();
  void getBridges();
  void checkBridges(int x, int y);
  void getCrossing();
  bool checkCrossing(int x, int y, int selektor);
  void changeStateIsland(int x, int y, int z );
  void setUndoCache(int x, int y, int zustand, int step);
  void undoMove();
  void checkSolution();
};
#endif  //  HASHI_H_
