// Copyright 2021, Spaichingen
// Grimm-Engineering
// Author: Raphael Grimm <info@grimm-engineering.de>

#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "./Hashi.h"

int main(int argc, char** argv) {
// ____________________________________________________________ 
  
  Hashi game;
  game.parseCommandLineArguments(argc, argv);
  game.readFile();
  game.getBridges();
  game.checkCrossing();
  //  game.getCrossing();
//  game.checkBridges(9,4);
//  game.checkBridges(9,4);
//  game.checkBridges(2,8);
//  game.checkBridges(8,8);
//  game.checkBridges(8,8);
//  game.checkBridges(5,0);
//
// ____________________________________________________________
// Fuer Testzwecke
  std::cout << "alle X-Inseln" << std::endl;
  for(auto& pair : game._xislands) {
      std::cout << "[" << pair.first << "]=";
    
      std::cout << "{";
      for(auto& x : pair.second) {
      
       //std::cout << pair.first << "x =";
       std::cout << x << ",";
     
     }
     std::cout << "}";
      std::cout << std::endl;
  }
//
// ____________________________________________________________
// ____________________________________________________________
std::cout << "alle Y-Inseln" << std::endl;
  for(auto& pair : game._yislands) {
      std::cout << "[" << pair.first << "]=";
    
      std::cout << "{";
      for(auto& x : pair.second) {
      
       //std::cout << pair.first << "x =";
       std::cout << x << ",";
     
     }
     std::cout << "}";
      std::cout << std::endl;
  }
//
// ____________________________________________________________
// ____________________________________________________________
std::cout << "alle X  Bridges" << std::endl;
  for (auto& pair : game._allXBridges) {
      std::cout << "[" << pair.first << "]=";
    for(auto& z : pair.second) {
      std::cout << "{";
      for(auto& x : z) {
       //std::cout << pair.first << "x =";
       std::cout << x << ",";
     }
     std::cout << "}";
     }
      std::cout << std::endl;
  }
  
// ____________________________________________________________
// ____________________________________________________________
std::cout << "alle Y  Bridges" << std::endl;
  for (auto& pair : game._allYBridges) {
      std::cout << "[" << pair.first << "]=";
    for(auto& z : pair.second) {
      std::cout << "{";
      for(auto& x : z) {
       //std::cout << pair.first << "x =";
       std::cout << x << ",";
     }
     std::cout << "}";
     }
      std::cout << std::endl;
  }
  


std::cout << "Inseln mit Zuständen" << std::endl; 
for (auto& pair : game._YIslands) {
    
  std::cout << "[" << pair.first << "] ="; 
  // pair.second = std::vector<std::vector<int>>  
    for(auto& z : pair.second) { 
     std::cout << "{";   
      for(size_t i = 0; i <= z.size() -1; i++) { 
     // std::cout <<"z.size()" << z.size()<< std::endl;
      std::cout  << z[i];
      std::cout << "  ";
      }
      
      std::cout << "}";
      // for (auto& n : z) {
        //std::cout << pair.first << "x =";
       
     // std::cout << z.size() << std::endl;
     // } 
     }
      std::cout << std::endl;
  }

std::cout << "All XCrossings" << std::endl; 
for (auto& pair : game._XCrossings) {
    
  std::cout << "[" << pair.first << "] ="; 
  // pair.second = std::vector<std::vector<int>>  
     std::cout << "{";   
    for(auto& z : pair.second) { 
    
     // std::cout <<"z.size()" << z.size()<< std::endl;
      std::cout <<  z;
      std::cout << ",";    
    }
      
      std::cout << "}";
      // for (auto& n : z) {
        //std::cout << pair.first << "x =";
       
     // std::cout << z.size() << std::endl;
     // } 
std::cout << std::endl;
     }


std::cout << "All YCrossings" << std::endl; 
for (auto& pair : game._YCrossings) {
    
  std::cout << "[" << pair.first << "] ="; 
  // pair.second = std::vector<std::vector<int>>  
     std::cout << "{";   
    for(auto& z : pair.second) { 
    
     // std::cout <<"z.size()" << z.size()<< std::endl;
      std::cout <<  z;
      std::cout << ",";    
    }
      
      std::cout << "}";
      // for (auto& n : z) {
        //std::cout << pair.first << "x =";
       
     // std::cout << z.size() << std::endl;
     // } 
std::cout << std::endl;
     }




std::cout << "Alle tatsaechlichen Crossings" << std::endl; 
for (auto& pair : game._allCrossings) {
    
  std::cout << "[" << pair.first << "] ="; 
  // pair.second = std::vector<std::vector<int>>  
     std::cout << "{";   
    for(auto& z : pair.second) { 
    
     // std::cout <<"z.size()" << z.size()<< std::endl;
      std::cout <<  z;
      std::cout << ",";    
    }
      
      std::cout << "}";
      // for (auto& n : z) {
        //std::cout << pair.first << "x =";
       
     // std::cout << z.size() << std::endl;
     // } 
std::cout << std::endl;
     }


std::cout << "Bruecken in X Solution" << std::endl; 
for (auto& pair : game._allXBridges) {
    
    
  for(auto& z : pair.second) { 
        
//        if (z.size() == 2) {
    if (z.size() - 1 == 1) {

    std::cout << "[single] = {";   
    std::cout << pair.first << (z.at(0) - 1) << pair.first << z.at(z.size() - 2) + 1 <<std::endl;
        }
    if (z.size() - 1 == 2) {

    std::cout << "[double] = {";   
    std::cout << pair.first << (z.at(0) - 1) << pair.first << z.at(z.size() - 2) + 1 <<std::endl;
        }

    else { std::cout << "What??" <<std::endl;
        
}      
      std::cout << "}";
      std::cout << std::endl;
      // for (auto& n : z) {
        //std::cout << pair.first << "x =";
       
     // std::cout << z.size() << std::endl;
     // } 
std::cout << std::endl;
     }
}


      game.playGame();

}
