// Copyright 2021, Spaichingen
// Grimm-Engineering
// Author: Raphael Grimm <info@grimm-engineering.de>


#ifndef HASHI_H_
#define HASHI_H_

#include <ncurses.h>
#include <string>
#include <vector>
#include <unordered_map>
class Hashi {
 public:
  // Membervariablen
  int _maxXFeld;
  int _maxYFeld;
  // NullPunkt X des Spiels relativ zur Mitte
  int _NullX;
  // NullPunkt Y des Spiels relativ zur Mitte
  int _NullY;
  std::string _inputFileName;
  // neu
  std::unordered_map<int, std::vector<std::vector<int>>> _YIslands;
  std::vector<int> _triple;
  std::vector<int> _bridge;
  std::unordered_map<int, std::vector<int>> _xislands;
  std::unordered_map<int, std::vector<int>> _yislands;
  std::unordered_map<int, std::vector<int>> _ybridges;
  std::unordered_map<int, std::vector<int>> _xbridges;
  // Speichert das gesamte Spielfeld mit Brückenanzahl als key
  std::unordered_map<int, std::vector<std::vector<int>>> _allIslands;
  std::unordered_map<int, std::vector<std::vector<int>>> _allXBridges;
  std::unordered_map<int, std::vector<int>> _allXBridges2;
  std::unordered_map<int, std::vector<std::vector<int>>> _allYBridges;
  std::vector<std::vector<int>> _alleWerte;
  // Memberfunktionen
  void playGame();
  void initializeScreen();
  void processUserInput(int key);
  void printIslands();
  void printBridges();
  void printField();
  void readFile();
  void getBridges();
  void checkBridges(int x, int y);
  void changeStateIsland(int x, int y, int z);
};
#endif  //  HASHI_H_
