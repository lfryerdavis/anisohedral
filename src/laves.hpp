//
//  laves.hpp
//
//
//  Created by Lee Fryer-Davis on 2025-06-28.
//

#ifndef laves_hpp
#define laves_hpp

#include "common.hpp"
#include "shape.hpp"
#include "picture.hpp"
#include "action.hpp"

extern bool isLaves;
extern bool isHex;
extern bool isFloretLeft;
extern bool isFloretRight;
extern bool isPrismaticPentagonal;
extern bool isCairo;
extern bool isKite;
extern bool isLozenge;
extern bool isTriakisTriangular;
extern bool isSquare;
extern bool isKisrhombille;
extern bool isTetrakisSquare;
extern bool isTriangular;
extern bool isTests;
extern bool isStandalonePicture;
extern bool isShowSplitGraphs;
extern bool isShowSplitPaths;
extern bool isIncludeNeighbours;

extern bool isTests;
extern bool isTileAnchorTest1;
extern bool isTileAnchorTest2;
extern bool isTileAnchorTest3;
extern bool isTileAnchorTest4;
extern bool isTileAnchorTest5;
extern bool isTileAnchorTest6;
extern bool isTileAnchorTest7;
extern bool isTileAnchorTest8;
extern bool isTileAnchorTest9;
extern bool isTileSplit;
extern bool isTileSplitTriangle;
extern bool isTileSplitQuad;
extern bool isTileSplitPent;
extern bool isTileSplitBent;
extern bool isTileSplitStar;

extern shapes *gShapes;
extern actions *gActions;
extern pictures *gPictures;

extern size_t hexTile;
extern size_t floretLeft;
extern size_t floretRight;
extern size_t prismaticPentagonal;
extern size_t cairo;
extern size_t kite;
extern size_t lozenge;
extern size_t triakisTriangular;
extern size_t square;
extern size_t kisrhombille;
extern size_t kisrhombilleFlipped;
extern size_t tetrakisSquare;
extern size_t tetrakisSquareFlipped;
extern size_t triangular;

extern size_t tileAnchorTest1;
extern size_t tileAnchorTest2;
extern size_t tileAnchorTest3;
extern size_t tileAnchorTest4;
extern size_t tileAnchorTest5;
extern size_t tileAnchorTest6;
extern size_t tileAnchorTest7;
extern size_t tileAnchorTest8;
extern size_t tileAnchorTest9;
extern size_t tileSplit;
extern size_t tileSplitTriangle;
extern size_t tileSplitQuad;
extern size_t tileSplitPent;
extern size_t tileSplitBent;
extern size_t tileSplitStar;

void createLaves();

void createHex();
void createFloretLeft();
void createFloretRight();
void createPrismaticPentagonal();
void createCairo();
void createKite();
void createLozenge();
void createTriakisTriangular();
void createSquare();
void createKisrhombille();
void createTetrakisSquare();
void createTriangular();

void createTests();

void createTileAnchorTest1();
void createTileAnchorTest2();
void createTileAnchorTest3();
void createTileAnchorTest4();
void createTileAnchorTest5();
void createTileAnchorTest6();
void createTileAnchorTest7();
void createTileAnchorTest8();
void createTileAnchorTest9();
void createTileSplit();
void createTileSplitTriangle();
void createTileSplitQuad();
void createTileSplitPent();
void createTileSplitBent();
void createTileSplitStar();

#endif /* laves_hpp */
