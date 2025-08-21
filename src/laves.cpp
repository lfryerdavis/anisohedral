//
//  laves.cpp
//
//
//  Created by Lee Fryer-Davis on 2025-06-28.
//

#include <vector>
#include "laves.hpp"

using namespace std;

const double sqrt2 = 1.414213562373095;
const double sqrt3 = 1.732050807568877;

bool isLaves = false;

bool isHex = false;
bool isFloretLeft = false;
bool isFloretRight = false;
bool isPrismaticPentagonal = false;
bool isCairo = false;
bool isKite = false;
bool isLozenge = false;
bool isTriakisTriangular = false;
bool isSquare = false;
bool isKisrhombille = false;
bool isTetrakisSquare = false;
bool isTriangular = false;

bool isTests = false;

bool isTileAnchorTest1 = false;
bool isTileAnchorTest2 = false;
bool isTileAnchorTest3 = false;
bool isTileAnchorTest4 = false;
bool isTileAnchorTest5 = false;
bool isTileAnchorTest6 = false;
bool isTileAnchorTest7 = false;
bool isTileAnchorTest8 = false;
bool isTileAnchorTest9 = false;
bool isTileSplit = false;
bool isTileSplitTriangle = false;
bool isTileSplitQuad = false;
bool isTileSplitPent = false;
bool isTileSplitBent = false;
bool isTileSplitStar = false;
bool isTileBroken = false;
bool isTileSquareBroken = false;

bool isStandalonePicture = false;
bool isShowSplitGraphs = false;
bool isShowSplitPaths = false;
bool isIncludeNeighbours = false;

// Laves shapes
size_t hexTile = -1;
size_t floretLeft = -1;
size_t floretRight = -1;
size_t prismaticPentagonal = -1;
size_t cairo = -1;
size_t kite = -1;
size_t lozenge = -1;
size_t triakisTriangular = -1;
size_t square = -1;
size_t kisrhombille = -1;
size_t kisrhombilleFlipped = -1;
size_t tetrakisSquare = -1;
size_t tetrakisSquareFlipped = -1;
size_t triangular = -1;

// test shapes
size_t tileAnchorTest1 = -1;
size_t tileAnchorTest2 = -1;
size_t tileAnchorTest3 = -1;
size_t tileAnchorTest4 = -1;
size_t tileAnchorTest5 = -1;
size_t tileAnchorTest6 = -1;
size_t tileAnchorTest7 = -1;
size_t tileAnchorTest8 = -1;
size_t tileAnchorTest9 = -1;
size_t tileSplit = -1;
size_t tileSplitTriangle = -1;
size_t tileSplitQuad = -1;
size_t tileSplitPent = -1;
size_t tileSplitBent = -1;
size_t tileSplitStar = -1;
size_t tileBroken = -1;
size_t tileSquareBroken = -1;

string centralThickness("thick");
string neighbourThickness("ultra thin");

size_t lavesDebugSimple = 1;
size_t lavesDebugExtra = 2;

void createHex()
{
    hexTile = gShapes->getNewID("hexLaves");
    
    debug(lavesDebugSimple, "Hex shape id = " + to_string(hexTile) + "\n");
    
    gShapes->addVertex(hexTile, 0, 0);
    gShapes->addVertex(hexTile, -0.5, sqrt3/2.);
    gShapes->addVertex(hexTile, 0, sqrt3);
    gShapes->addVertex(hexTile, 1, sqrt3);
    gShapes->addVertex(hexTile, 1.5, sqrt3/2.);
    gShapes->addVertex(hexTile, 1, 0);
    gShapes->addBoundaryEdges(hexTile);
    size_t hexCentre = gActions->getNewID(hexTile, gShapes->getName(hexTile), "colorBorder", centralThickness, 1.5, sqrt3/2.);
    size_t pictHex = gPictures->createPicture();
    gPictures->addShape(pictHex, hexTile);
    gPictures->addAction(pictHex, hexCentre);
    if (isIncludeNeighbours)
    {
        size_t hexTopLeft = gActions->getNewID(hexTile, gShapes->getName(hexTile), "colorNeighbour", neighbourThickness, 0, sqrt3);
        size_t hexTop = gActions->getNewID(hexTile, gShapes->getName(hexTile), "colorNeighbour", neighbourThickness, 1.5, 3*sqrt3/2.);
        size_t hexTopRight = gActions->getNewID(hexTile, gShapes->getName(hexTile), "colorNeighbour", neighbourThickness, 3, sqrt3);
        size_t hexBottomLeft = gActions->getNewID(hexTile, gShapes->getName(hexTile), "colorNeighbour", neighbourThickness, 0, 0);
        size_t hexBottomRight = gActions->getNewID(hexTile, gShapes->getName(hexTile), "colorNeighbour", neighbourThickness, 3, 0);
        size_t hexBottom = gActions->getNewID(hexTile, gShapes->getName(hexTile), "colorNeighbour", neighbourThickness, 1.5, -sqrt3/2.);
        gPictures->addAction(pictHex, hexTopLeft);
        gPictures->addAction(pictHex, hexTop);
        gPictures->addAction(pictHex, hexTopRight);
        gPictures->addAction(pictHex, hexBottomLeft);
        gPictures->addAction(pictHex, hexBottomRight);
        gPictures->addAction(pictHex, hexBottom);
    }
    gShapes->split(hexTile);
    gPictures->write(pictHex);
}

void createFloretLeft()
{
    floretLeft = gShapes->getNewID("floretLeftLaves");
    
    debug(lavesDebugSimple, "Floret Left shape id = " + to_string(floretLeft) + "\n");
    
    gShapes->addVertex(floretLeft, 0, 0);
    gShapes->addVertex(floretLeft, sqrt3, 1);
    gShapes->addVertex(floretLeft, 1.5*sqrt3, 0.5);
    gShapes->addVertex(floretLeft, 1.5*sqrt3, -0.5);
    gShapes->addVertex(floretLeft, sqrt3, -1);
    gShapes->addBoundaryEdges(floretLeft);
    size_t pictFloretLeft = gPictures->createPicture();
    size_t floretLeftCentre = gActions->getNewID(floretLeft, gShapes->getName(floretLeft), "colorBorder", centralThickness, 0, 0, 0);
    gPictures->addShape(pictFloretLeft, floretLeft);
    gPictures->addAction(pictFloretLeft, floretLeftCentre);
    if (isIncludeNeighbours)
    {
        size_t floretLeft1 = gActions->getNewID(floretLeft, gShapes->getName(floretLeft), "colorNeighbour", neighbourThickness, 0, 0, 60);
        size_t floretLeft2 = gActions->getNewID(floretLeft, gShapes->getName(floretLeft), "colorNeighbour", neighbourThickness, 0, 0, 120);
        size_t floretLeft3 = gActions->getNewID(floretLeft, gShapes->getName(floretLeft), "colorNeighbour", neighbourThickness, 0, 0, 180);
        size_t floretLeft4 = gActions->getNewID(floretLeft, gShapes->getName(floretLeft), "colorNeighbour", neighbourThickness, 0, 0, 240);
        size_t floretLeft5 = gActions->getNewID(floretLeft, gShapes->getName(floretLeft), "colorNeighbour", neighbourThickness, 0, 0, 300);
        size_t floretLeft6 = gActions->getNewID(floretLeft, gShapes->getName(floretLeft), "colorNeighbour", neighbourThickness, 2.5*sqrt3, 1.5, 180);
        size_t floretLeft7 = gActions->getNewID(floretLeft, gShapes->getName(floretLeft), "colorNeighbour", neighbourThickness, 2.5*sqrt3, 1.5, 240);
        size_t floretLeft8 = gActions->getNewID(floretLeft, gShapes->getName(floretLeft), "colorNeighbour", neighbourThickness, 2*sqrt3, -3, 120);
        gPictures->addAction(pictFloretLeft, floretLeft1);
        gPictures->addAction(pictFloretLeft, floretLeft2);
        gPictures->addAction(pictFloretLeft, floretLeft3);
        gPictures->addAction(pictFloretLeft, floretLeft4);
        gPictures->addAction(pictFloretLeft, floretLeft5);
        gPictures->addAction(pictFloretLeft, floretLeft6);
        gPictures->addAction(pictFloretLeft, floretLeft7);
        gPictures->addAction(pictFloretLeft, floretLeft8);
    }
    gShapes->split(floretLeft);
    gPictures->write(pictFloretLeft);
}

void createFloretRight()
{
    floretRight = gShapes->getNewID("floretRightLaves");
    
    debug(lavesDebugSimple, "Floret Right shape id = " + to_string(floretRight) + "\n");
    
    gShapes->addVertex(floretRight, 0, 0);
    gShapes->addVertex(floretRight, sqrt3, 1);
    gShapes->addVertex(floretRight, 1.5*sqrt3, 0.5);
    gShapes->addVertex(floretRight, 1.5*sqrt3, -0.5);
    gShapes->addVertex(floretRight, sqrt3, -1);
    gShapes->addBoundaryEdges(floretRight);
    size_t floretRightCentre = gActions->getNewID(floretRight, gShapes->getName(floretRight), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictfloretRight = gPictures->createPicture();
    gPictures->addShape(pictfloretRight, floretRight);
    gPictures->addAction(pictfloretRight, floretRightCentre);
    if (isIncludeNeighbours)
    {
        size_t floretRight1 = gActions->getNewID(floretRight, gShapes->getName(floretRight), "colorNeighbour", neighbourThickness, 0, 0, 60);
        size_t floretRight2 = gActions->getNewID(floretRight, gShapes->getName(floretRight), "colorNeighbour", neighbourThickness, 0, 0, 120);
        size_t floretRight3 = gActions->getNewID(floretRight, gShapes->getName(floretRight), "colorNeighbour", neighbourThickness, 0, 0, 180);
        size_t floretRight4 = gActions->getNewID(floretRight, gShapes->getName(floretRight), "colorNeighbour", neighbourThickness, 0, 0, 240);
        size_t floretRight5 = gActions->getNewID(floretRight, gShapes->getName(floretRight), "colorNeighbour", neighbourThickness, 0, 0, 300);
        size_t floretRight6 = gActions->getNewID(floretRight, gShapes->getName(floretRight), "colorNeighbour", neighbourThickness, 2*sqrt3, 3, 240);
        size_t floretRight7 = gActions->getNewID(floretRight, gShapes->getName(floretRight), "colorNeighbour", neighbourThickness, 2.5*sqrt3, -1.5, 120);
        size_t floretRight8 = gActions->getNewID(floretRight, gShapes->getName(floretRight), "colorNeighbour", neighbourThickness, 2.5*sqrt3, -1.5, 180);
        gPictures->addAction(pictfloretRight, floretRight1);
        gPictures->addAction(pictfloretRight, floretRight2);
        gPictures->addAction(pictfloretRight, floretRight3);
        gPictures->addAction(pictfloretRight, floretRight4);
        gPictures->addAction(pictfloretRight, floretRight5);
        gPictures->addAction(pictfloretRight, floretRight6);
        gPictures->addAction(pictfloretRight, floretRight7);
        gPictures->addAction(pictfloretRight, floretRight8);
    }
    gShapes->split(floretRight);
    gPictures->write(pictfloretRight);
}

void createPrismaticPentagonal()
{
    prismaticPentagonal = gShapes->getNewID("prismaticPentagonalLaves");
    
    debug(lavesDebugSimple, "Prismatic Pentagonal shape id = " + to_string(prismaticPentagonal) + "\n");
    
    gShapes->addVertex(prismaticPentagonal, 0, 0);
    gShapes->addVertex(prismaticPentagonal, 0, 1);
    gShapes->addVertex(prismaticPentagonal, 0.5, 1+0.5/sqrt2);
    gShapes->addVertex(prismaticPentagonal, 1, 1);
    gShapes->addVertex(prismaticPentagonal, 1, 0);
    gShapes->addBoundaryEdges(prismaticPentagonal);
    size_t pictPrismaticPentagonal = gPictures->createPicture();
    size_t prismaticPentagonalCentre = gActions->getNewID(prismaticPentagonal, gShapes->getName(prismaticPentagonal), "colorBorder", centralThickness, 0, 0, 0);
    gPictures->addShape(pictPrismaticPentagonal, prismaticPentagonal);
    gPictures->addAction(pictPrismaticPentagonal, prismaticPentagonalCentre);
    if (isIncludeNeighbours)
    {
        size_t prismaticPentagonal1 = gActions->getNewID(prismaticPentagonal, gShapes->getName(prismaticPentagonal), "colorNeighbour", neighbourThickness, -1, 0, 0);
        size_t prismaticPentagonal2 = gActions->getNewID(prismaticPentagonal, gShapes->getName(prismaticPentagonal), "colorNeighbour", neighbourThickness, 0, 0, 180);
        size_t prismaticPentagonal3 = gActions->getNewID(prismaticPentagonal, gShapes->getName(prismaticPentagonal), "colorNeighbour", neighbourThickness, 1, 0, 0);
        size_t prismaticPentagonal4 = gActions->getNewID(prismaticPentagonal, gShapes->getName(prismaticPentagonal), "colorNeighbour", neighbourThickness, 1, 0, 180);
        size_t prismaticPentagonal5 = gActions->getNewID(prismaticPentagonal, gShapes->getName(prismaticPentagonal), "colorNeighbour", neighbourThickness, 2, 0, 180);
        size_t prismaticPentagonal6 = gActions->getNewID(prismaticPentagonal, gShapes->getName(prismaticPentagonal), "colorNeighbour", neighbourThickness, 0.5, 2+0.5/sqrt2, 180);
        size_t prismaticPentagonal7 = gActions->getNewID(prismaticPentagonal, gShapes->getName(prismaticPentagonal), "colorNeighbour", neighbourThickness, 1.5, 2+0.5/sqrt2, 180);
        gPictures->addAction(pictPrismaticPentagonal, prismaticPentagonal1);
        gPictures->addAction(pictPrismaticPentagonal, prismaticPentagonal2);
        gPictures->addAction(pictPrismaticPentagonal, prismaticPentagonal3);
        gPictures->addAction(pictPrismaticPentagonal, prismaticPentagonal4);
        gPictures->addAction(pictPrismaticPentagonal, prismaticPentagonal5);
        gPictures->addAction(pictPrismaticPentagonal, prismaticPentagonal6);
        gPictures->addAction(pictPrismaticPentagonal, prismaticPentagonal7);
    }
    gShapes->split(prismaticPentagonal);
    gPictures->write(pictPrismaticPentagonal);
}

void createCairo()
{
    cairo = gShapes->getNewID("cairoLaves");
    
    debug(lavesDebugSimple, "Cairo shape id = " + to_string(cairo) + "\n");
    
    gShapes->addVertex(cairo, 0, 0);
    gShapes->addVertex(cairo, -0.5, 0.5*sqrt3);
    gShapes->addVertex(cairo, 0.5*sqrt3-0.5, 0.5+0.5*sqrt3);
    gShapes->addVertex(cairo, sqrt3-0.5, 0.5*sqrt3);
    gShapes->addVertex(cairo, -1+sqrt3, 0);
    gShapes->addBoundaryEdges(cairo);
    size_t cairoCentre = gActions->getNewID(cairo, gShapes->getName(cairo), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictCairo = gPictures->createPicture();
    gPictures->addShape(pictCairo, cairo);
    gPictures->addAction(pictCairo, cairoCentre);
    if (isIncludeNeighbours)
    {
        size_t cairo1 = gActions->getNewID(cairo, gShapes->getName(cairo), "colorNeighbour", neighbourThickness, -0.5+1.5*sqrt3, 0.5-0.5*sqrt3, 90);
        size_t cairo2 = gActions->getNewID(cairo, gShapes->getName(cairo), "colorNeighbour", neighbourThickness, -0.5-0.5*sqrt3, -0.5+0.5*sqrt3, 270);
        size_t cairo3 = gActions->getNewID(cairo, gShapes->getName(cairo), "colorNeighbour", neighbourThickness, -0.5+0.5*sqrt3, -0.5+1.5*sqrt3, 270);
        size_t cairo4 = gActions->getNewID(cairo, gShapes->getName(cairo), "colorNeighbour", neighbourThickness, -0.5+0.5*sqrt3, 0.5+0.5*sqrt3, 90);
        size_t cairo5 = gActions->getNewID(cairo, gShapes->getName(cairo), "colorNeighbour", neighbourThickness, -1+sqrt3, 0, 180);
        size_t cairo6 = gActions->getNewID(cairo, gShapes->getName(cairo), "colorNeighbour", neighbourThickness, -1+2*sqrt3, sqrt3, 180);
        size_t cairo7 = gActions->getNewID(cairo, gShapes->getName(cairo), "colorNeighbour", neighbourThickness, -1, sqrt3, 180);
        gPictures->addAction(pictCairo, cairo1);
        gPictures->addAction(pictCairo, cairo2);
        gPictures->addAction(pictCairo, cairo3);
        gPictures->addAction(pictCairo, cairo4);
        gPictures->addAction(pictCairo, cairo5);
        gPictures->addAction(pictCairo, cairo6);
        gPictures->addAction(pictCairo, cairo7);
    }
    gShapes->split(cairo);
    gPictures->write(pictCairo);
}

void createKite()
{
    kite = gShapes->getNewID("kiteLaves");
    
    debug(lavesDebugSimple, "Kite shape id = " + to_string(kite) + "\n");
    
    gShapes->addVertex(kite, 0, 0);
    gShapes->addVertex(kite, 0.5, 0.5*sqrt3);
    gShapes->addVertex(kite, 1, 1./sqrt3);
    gShapes->addVertex(kite, 1, 0);
    gShapes->addBoundaryEdges(kite);
    size_t kiteCentre = gActions->getNewID(kite, gShapes->getName(kite), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictKite = gPictures->createPicture();
    gPictures->addShape(pictKite, kite);
    gPictures->addAction(pictKite, kiteCentre);
    if (isIncludeNeighbours)
    {
        size_t kite1 = gActions->getNewID(kite, gShapes->getName(kite), "colorNeighbour", neighbourThickness, 0, 0, 60);
        size_t kite2 = gActions->getNewID(kite, gShapes->getName(kite), "colorNeighbour", neighbourThickness, 0, 0, 120);
        size_t kite3 = gActions->getNewID(kite, gShapes->getName(kite), "colorNeighbour", neighbourThickness, 0, 0, 180);
        size_t kite4 = gActions->getNewID(kite, gShapes->getName(kite), "colorNeighbour", neighbourThickness, 0, 0, 240);
        size_t kite5 = gActions->getNewID(kite, gShapes->getName(kite), "colorNeighbour", neighbourThickness, 0, 0, 300);
        size_t kite6 = gActions->getNewID(kite, gShapes->getName(kite), "colorNeighbour", neighbourThickness, 2, 0, 120);
        size_t kite7 = gActions->getNewID(kite, gShapes->getName(kite), "colorNeighbour", neighbourThickness, 2, 0, 180);
        size_t kite8 = gActions->getNewID(kite, gShapes->getName(kite), "colorNeighbour", neighbourThickness, 1, sqrt3, 180);
        size_t kite9 = gActions->getNewID(kite, gShapes->getName(kite), "colorNeighbour", neighbourThickness, 1, sqrt3, 240);
        gPictures->addAction(pictKite, kite1);
        gPictures->addAction(pictKite, kite2);
        gPictures->addAction(pictKite, kite3);
        gPictures->addAction(pictKite, kite4);
        gPictures->addAction(pictKite, kite5);
        gPictures->addAction(pictKite, kite6);
        gPictures->addAction(pictKite, kite7);
        gPictures->addAction(pictKite, kite8);
        gPictures->addAction(pictKite, kite9);
    }
    gShapes->split(kite);
    gPictures->write(pictKite);
}

void createLozenge()
{
    lozenge = gShapes->getNewID("lozengeLaves");
    
    debug(lavesDebugSimple, "Lozenge shape id = " + to_string(lozenge) + "\n");
    
    gShapes->addVertex(lozenge, 0, 0);
    gShapes->addVertex(lozenge, 0.5, 0.5*sqrt3);
    gShapes->addVertex(lozenge, 1.5, 0.5*sqrt3);
    gShapes->addVertex(lozenge, 1, 0);
    gShapes->addBoundaryEdges(lozenge);
    size_t lozengeCentre = gActions->getNewID(lozenge, gShapes->getName(lozenge), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictLozenge = gPictures->createPicture();
    gPictures->addShape(pictLozenge, lozenge);
    gPictures->addAction(pictLozenge, lozengeCentre);
    if (isIncludeNeighbours)
    {
        size_t lozenge1 = gActions->getNewID(lozenge, gShapes->getName(lozenge), "colorNeighbour", neighbourThickness, 0, 0, 60);
        size_t lozenge2 = gActions->getNewID(lozenge, gShapes->getName(lozenge), "colorNeighbour", neighbourThickness, 0, 0, 120);
        size_t lozenge3 = gActions->getNewID(lozenge, gShapes->getName(lozenge), "colorNeighbour", neighbourThickness, 0, 0, 180);
        size_t lozenge4 = gActions->getNewID(lozenge, gShapes->getName(lozenge), "colorNeighbour", neighbourThickness, 0, 0, 240);
        size_t lozenge5 = gActions->getNewID(lozenge, gShapes->getName(lozenge), "colorNeighbour", neighbourThickness, 0, 0, 300);
        size_t lozenge6 = gActions->getNewID(lozenge, gShapes->getName(lozenge), "colorNeighbour", neighbourThickness, 1.5, 0.5*sqrt3, 0);
        size_t lozenge7 = gActions->getNewID(lozenge, gShapes->getName(lozenge), "colorNeighbour", neighbourThickness, 1.5, 0.5*sqrt3, 60);
        size_t lozenge8 = gActions->getNewID(lozenge, gShapes->getName(lozenge), "colorNeighbour", neighbourThickness, 1.5, 0.5*sqrt3, 120);
        size_t lozenge9 = gActions->getNewID(lozenge, gShapes->getName(lozenge), "colorNeighbour", neighbourThickness, 1.5, 0.5*sqrt3, 240);
        size_t lozenge10 = gActions->getNewID(lozenge, gShapes->getName(lozenge), "colorNeighbour", neighbourThickness, 1.5, 0.5*sqrt3, 300);
        gPictures->addAction(pictLozenge, lozenge1);
        gPictures->addAction(pictLozenge, lozenge2);
        gPictures->addAction(pictLozenge, lozenge3);
        gPictures->addAction(pictLozenge, lozenge4);
        gPictures->addAction(pictLozenge, lozenge5);
        gPictures->addAction(pictLozenge, lozenge6);
        gPictures->addAction(pictLozenge, lozenge7);
        gPictures->addAction(pictLozenge, lozenge8);
        gPictures->addAction(pictLozenge, lozenge9);
        gPictures->addAction(pictLozenge, lozenge10);
    }
    gShapes->split(lozenge);
    gPictures->write(pictLozenge);
}

void createTriakisTriangular()
{
    triakisTriangular = gShapes->getNewID("triakisTriangularLaves");
    
    debug(lavesDebugSimple, "Triakis Triangular shape id = " + to_string(triakisTriangular) + "\n");
    
    gShapes->addVertex(triakisTriangular, 0, 0);
    gShapes->addVertex(triakisTriangular, sqrt3, 1);
    gShapes->addVertex(triakisTriangular, 2*sqrt3, 0);
    gShapes->addBoundaryEdges(triakisTriangular);
    size_t triakisTriangularCentre = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictTriakisTriangular = gPictures->createPicture();
    gPictures->addShape(pictTriakisTriangular, triakisTriangular);
    gPictures->addAction(pictTriakisTriangular, triakisTriangularCentre);
    if (isIncludeNeighbours)
    {
        size_t triakisTriangular1 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, 0, 0, 60);
        size_t triakisTriangular2 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, 0, 0, 120);
        size_t triakisTriangular3 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, 0, 0, 180);
        size_t triakisTriangular4 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, 0, 0, 240);
        size_t triakisTriangular5 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, 0, 0, 300);
        size_t triakisTriangular6 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, -2*sqrt3, 0, 0);
        size_t triakisTriangular7 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, -sqrt3, -3, 60);
        size_t triakisTriangular8 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, sqrt3, -3, 120);
        size_t triakisTriangular9 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, 2*sqrt3, 0, 180);
        size_t triakisTriangular10 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, sqrt3, 3, 240);
        size_t triakisTriangular11 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, -sqrt3, 3, 300);
        size_t triakisTriangular12 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, 2*sqrt3, 0, 60);
        size_t triakisTriangular13 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, 2*sqrt3, 0, 120);
        size_t triakisTriangular14 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, sqrt3, 3, 300);
        size_t triakisTriangular15 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, 2*sqrt3, 0, 240);
        size_t triakisTriangular16 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, 2*sqrt3, 0, 300);
        size_t triakisTriangular17 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, 2*sqrt3, 0, 0);
        size_t triakisTriangular18 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, sqrt3, -3, 60);
        size_t triakisTriangular19 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, 3*sqrt3, -3, 120);
        size_t triakisTriangular20 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, 4*sqrt3, 0, 180);
        size_t triakisTriangular21 = gActions->getNewID(triakisTriangular, gShapes->getName(triakisTriangular), "colorNeighbour", neighbourThickness, 3*sqrt3, 3, 240);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular1);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular2);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular3);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular4);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular5);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular6);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular7);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular8);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular9);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular10);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular11);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular12);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular13);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular14);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular15);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular16);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular17);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular18);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular19);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular20);
        gPictures->addAction(pictTriakisTriangular, triakisTriangular21);
    }
    gShapes->split(triakisTriangular);
    gPictures->write(pictTriakisTriangular);
}

void createSquare()
{
    square = gShapes->getNewID("squareLaves");
    
    debug(lavesDebugSimple, "Square shape id = " + to_string(square) + "\n");
    
    gShapes->addVertex(square, 0, 0);
    gShapes->addVertex(square, 0, 1);
    gShapes->addVertex(square, 1, 1);
    gShapes->addVertex(square, 1, 0);
    gShapes->addBoundaryEdges(square);
    size_t squareCentre = gActions->getNewID(square, gShapes->getName(square), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictsquare = gPictures->createPicture();
    gPictures->addShape(pictsquare, square);
    gPictures->addAction(pictsquare, squareCentre);
    if (isIncludeNeighbours)
    {
        size_t square1 = gActions->getNewID(square, gShapes->getName(square), "colorNeighbour", neighbourThickness, 1, 0, 0);
        size_t square2 = gActions->getNewID(square, gShapes->getName(square), "colorNeighbour", neighbourThickness, 1, 1, 0);
        size_t square3 = gActions->getNewID(square, gShapes->getName(square), "colorNeighbour", neighbourThickness, 0, 1, 0);
        size_t square4 = gActions->getNewID(square, gShapes->getName(square), "colorNeighbour", neighbourThickness, -1, 1, 0);
        size_t square5 = gActions->getNewID(square, gShapes->getName(square), "colorNeighbour", neighbourThickness, -1, 0, 0);
        size_t square6 = gActions->getNewID(square, gShapes->getName(square), "colorNeighbour", neighbourThickness, -1, -1, 0);
        size_t square7 = gActions->getNewID(square, gShapes->getName(square), "colorNeighbour", neighbourThickness, 0, -1, 0);
        size_t square8 = gActions->getNewID(square, gShapes->getName(square), "colorNeighbour", neighbourThickness, 1, -1, 0);
        gPictures->addAction(pictsquare, square1);
        gPictures->addAction(pictsquare, square2);
        gPictures->addAction(pictsquare, square3);
        gPictures->addAction(pictsquare, square4);
        gPictures->addAction(pictsquare, square5);
        gPictures->addAction(pictsquare, square6);
        gPictures->addAction(pictsquare, square7);
        gPictures->addAction(pictsquare, square8);
    }
    gShapes->split(square);
    gPictures->write(pictsquare);
}

void createKisrhombille()
{
    kisrhombille = gShapes->getNewID("kisrhombilleLaves");
    kisrhombilleFlipped = gShapes->getNewID("kisrhombilleLavesFlipped");
    
    debug(lavesDebugSimple, "Kisrhombille shape id = " + to_string(kisrhombille) + "\n");
    debug(lavesDebugSimple, "Kisrhombille Flipped shape id = " + to_string(kisrhombilleFlipped) + "\n");

    gShapes->addVertex(kisrhombille, 0, 0);
    gShapes->addVertex(kisrhombille, 1, -sqrt3);
    gShapes->addVertex(kisrhombille, 0, -sqrt3);
    gShapes->addBoundaryEdges(kisrhombille);
    gShapes->addVertex(kisrhombilleFlipped, 0, 0);
    gShapes->addVertex(kisrhombilleFlipped, 0, -sqrt3);
    gShapes->addVertex(kisrhombilleFlipped, -1, -sqrt3);
    gShapes->addBoundaryEdges(kisrhombilleFlipped);
    size_t kisrhombilleCentre = gActions->getNewID(kisrhombille, gShapes->getName(kisrhombille), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictKisrhombille = gPictures->createPicture();
    gPictures->addShape(pictKisrhombille, kisrhombille);
    gPictures->addShape(pictKisrhombille, kisrhombilleFlipped);
    gPictures->addBaseShape(pictKisrhombille, kisrhombille);
    gPictures->addBaseShape(pictKisrhombille, kisrhombilleFlipped);
    gPictures->addAction(pictKisrhombille, kisrhombilleCentre);
    if (isIncludeNeighbours)
    {
        size_t kisrhombille1 = gActions->getNewID(kisrhombille, gShapes->getName(kisrhombille), "colorNeighbour", neighbourThickness, 0, 0, 60);
        size_t kisrhombille2 = gActions->getNewID(kisrhombille, gShapes->getName(kisrhombille), "colorNeighbour", neighbourThickness, 0, 0, 120);
        size_t kisrhombille3 = gActions->getNewID(kisrhombille, gShapes->getName(kisrhombille), "colorNeighbour", neighbourThickness, 0, 0, 180);
        size_t kisrhombille4 = gActions->getNewID(kisrhombille, gShapes->getName(kisrhombille), "colorNeighbour", neighbourThickness, 0, 0, 240);
        size_t kisrhombille5 = gActions->getNewID(kisrhombille, gShapes->getName(kisrhombille), "colorNeighbour", neighbourThickness, 0, 0, 300);
        size_t kisrhombille6 = gActions->getNewID(kisrhombilleFlipped, gShapes->getName(kisrhombilleFlipped), "colorNeighbour", neighbourThickness, 0, 0, 0);
        size_t kisrhombille7 = gActions->getNewID(kisrhombilleFlipped, gShapes->getName(kisrhombilleFlipped), "colorNeighbour", neighbourThickness, 0, 0, 60);
        size_t kisrhombille8 = gActions->getNewID(kisrhombilleFlipped, gShapes->getName(kisrhombilleFlipped), "colorNeighbour", neighbourThickness, 0, 0, 120);
        size_t kisrhombille9 = gActions->getNewID(kisrhombilleFlipped, gShapes->getName(kisrhombilleFlipped), "colorNeighbour", neighbourThickness, 0, 0, 180);
        size_t kisrhombille10 = gActions->getNewID(kisrhombilleFlipped, gShapes->getName(kisrhombilleFlipped), "colorNeighbour", neighbourThickness, 0, 0, 240);
        size_t kisrhombille11 = gActions->getNewID(kisrhombilleFlipped, gShapes->getName(kisrhombilleFlipped), "colorNeighbour", neighbourThickness, 0, 0, 300);
        size_t kisrhombille12 = gActions->getNewID(kisrhombille, gShapes->getName(kisrhombille), "colorNeighbour", neighbourThickness, 0, -2*sqrt3, 180);
        size_t kisrhombille13 = gActions->getNewID(kisrhombille, gShapes->getName(kisrhombille), "colorNeighbour", neighbourThickness, 0, -2*sqrt3, 120);
        size_t kisrhombille14 = gActions->getNewID(kisrhombilleFlipped, gShapes->getName(kisrhombilleFlipped), "colorNeighbour", neighbourThickness, 0, -2*sqrt3, 180);
        size_t kisrhombille15 = gActions->getNewID(kisrhombille, gShapes->getName(kisrhombille), "colorNeighbour", neighbourThickness, 3, -sqrt3, 240);
        size_t kisrhombille16 = gActions->getNewID(kisrhombilleFlipped, gShapes->getName(kisrhombilleFlipped), "colorNeighbour", neighbourThickness, 3, -sqrt3, 300);
        gPictures->addAction(pictKisrhombille, kisrhombille1);
        gPictures->addAction(pictKisrhombille, kisrhombille2);
        gPictures->addAction(pictKisrhombille, kisrhombille3);
        gPictures->addAction(pictKisrhombille, kisrhombille4);
        gPictures->addAction(pictKisrhombille, kisrhombille5);
        gPictures->addAction(pictKisrhombille, kisrhombille6);
        gPictures->addAction(pictKisrhombille, kisrhombille7);
        gPictures->addAction(pictKisrhombille, kisrhombille8);
        gPictures->addAction(pictKisrhombille, kisrhombille9);
        gPictures->addAction(pictKisrhombille, kisrhombille10);
        gPictures->addAction(pictKisrhombille, kisrhombille11);
        gPictures->addAction(pictKisrhombille, kisrhombille12);
        gPictures->addAction(pictKisrhombille, kisrhombille13);
        gPictures->addAction(pictKisrhombille, kisrhombille14);
        gPictures->addAction(pictKisrhombille, kisrhombille15);
        gPictures->addAction(pictKisrhombille, kisrhombille16);
    }
    gShapes->split(kisrhombille);
    gShapes->split(kisrhombilleFlipped);
    gPictures->write(pictKisrhombille);
}

void createTetrakisSquare()
{
    tetrakisSquare = gShapes->getNewID("tetrakisSquareLaves");
    tetrakisSquareFlipped = gShapes->getNewID("tetrakisSquareFlippedLaves");
    
    debug(lavesDebugSimple, "Tetrakis Square shape id = " + to_string(tetrakisSquare) + "\n");
    debug(lavesDebugSimple, "Tetrakis Square Flipped shape id = " + to_string(tetrakisSquareFlipped) + "\n");
    
    gShapes->addVertex(tetrakisSquare, 0, 0);
    gShapes->addVertex(tetrakisSquare, 0.5, 0.5);
    gShapes->addVertex(tetrakisSquare, 1, 0);
    gShapes->addBoundaryEdges(tetrakisSquare);
    gShapes->addVertex(tetrakisSquareFlipped, 0, 0);
    gShapes->addVertex(tetrakisSquareFlipped, 1, 0);
    gShapes->addVertex(tetrakisSquareFlipped, 0.5, -0.5);
    gShapes->addBoundaryEdges(tetrakisSquareFlipped);
    size_t tetrakisSquareCentre = gActions->getNewID(tetrakisSquare, gShapes->getName(tetrakisSquare), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictTetrakisSquare = gPictures->createPicture();
    gPictures->addShape(pictTetrakisSquare, tetrakisSquare);
    gPictures->addShape(pictTetrakisSquare, tetrakisSquareFlipped);
    gPictures->addBaseShape(pictTetrakisSquare, tetrakisSquare);
    gPictures->addBaseShape(pictTetrakisSquare, tetrakisSquareFlipped);
    gPictures->addAction(pictTetrakisSquare, tetrakisSquareCentre);
    if (isIncludeNeighbours)
    {
        size_t tetrakisSquare1 = gActions->getNewID(tetrakisSquare, gShapes->getName(tetrakisSquare), "colorNeighbour", neighbourThickness, 0, 0, 90);
        size_t tetrakisSquare2 = gActions->getNewID(tetrakisSquare, gShapes->getName(tetrakisSquare), "colorNeighbour", neighbourThickness, 0, 0, 180);
        size_t tetrakisSquare3 = gActions->getNewID(tetrakisSquare, gShapes->getName(tetrakisSquare), "colorNeighbour", neighbourThickness, 0, 0, 270);
        size_t tetrakisSquare4 = gActions->getNewID(tetrakisSquareFlipped, gShapes->getName(tetrakisSquareFlipped), "colorNeighbour", neighbourThickness, 0, 0, 0);
        size_t tetrakisSquare5 = gActions->getNewID(tetrakisSquareFlipped, gShapes->getName(tetrakisSquareFlipped), "colorNeighbour", neighbourThickness, 0, 0, 90);
        size_t tetrakisSquare6 = gActions->getNewID(tetrakisSquareFlipped, gShapes->getName(tetrakisSquareFlipped), "colorNeighbour", neighbourThickness, 0, 0, 180);
        size_t tetrakisSquare7 = gActions->getNewID(tetrakisSquareFlipped, gShapes->getName(tetrakisSquareFlipped), "colorNeighbour", neighbourThickness, 0, 0, 270);
        size_t tetrakisSquare8 = gActions->getNewID(tetrakisSquare, gShapes->getName(tetrakisSquare), "colorNeighbour", neighbourThickness, 1, 0, 0);
        size_t tetrakisSquare9 = gActions->getNewID(tetrakisSquare, gShapes->getName(tetrakisSquare), "colorNeighbour", neighbourThickness, 1, 0, 90);
        size_t tetrakisSquare10 = gActions->getNewID(tetrakisSquare, gShapes->getName(tetrakisSquare), "colorNeighbour", neighbourThickness, 1, 0, 270);
        size_t tetrakisSquare11 = gActions->getNewID(tetrakisSquareFlipped, gShapes->getName(tetrakisSquareFlipped), "colorNeighbour", neighbourThickness, 1, 0, 0);
        size_t tetrakisSquare12 = gActions->getNewID(tetrakisSquareFlipped, gShapes->getName(tetrakisSquareFlipped), "colorNeighbour", neighbourThickness, 1, 0, 90);
        size_t tetrakisSquare13 = gActions->getNewID(tetrakisSquareFlipped, gShapes->getName(tetrakisSquareFlipped), "colorNeighbour", neighbourThickness, 1, 0, 270);
        size_t tetrakisSquare14 = gActions->getNewID(tetrakisSquareFlipped, gShapes->getName(tetrakisSquareFlipped), "colorNeighbour", neighbourThickness, 0, 1, 0);
        gPictures->addAction(pictTetrakisSquare, tetrakisSquare1);
        gPictures->addAction(pictTetrakisSquare, tetrakisSquare2);
        gPictures->addAction(pictTetrakisSquare, tetrakisSquare3);
        gPictures->addAction(pictTetrakisSquare, tetrakisSquare4);
        gPictures->addAction(pictTetrakisSquare, tetrakisSquare5);
        gPictures->addAction(pictTetrakisSquare, tetrakisSquare6);
        gPictures->addAction(pictTetrakisSquare, tetrakisSquare7);
        gPictures->addAction(pictTetrakisSquare, tetrakisSquare8);
        gPictures->addAction(pictTetrakisSquare, tetrakisSquare9);
        gPictures->addAction(pictTetrakisSquare, tetrakisSquare10);
        gPictures->addAction(pictTetrakisSquare, tetrakisSquare11);
        gPictures->addAction(pictTetrakisSquare, tetrakisSquare12);
        gPictures->addAction(pictTetrakisSquare, tetrakisSquare13);
        gPictures->addAction(pictTetrakisSquare, tetrakisSquare14);
    }
    gShapes->split(tetrakisSquare);
    gShapes->split(tetrakisSquareFlipped);
    gPictures->write(pictTetrakisSquare);
}
void createTriangular()
{
    triangular = gShapes->getNewID("triangular");
    
    debug(lavesDebugSimple, "Triangular shape id = " + to_string(triangular) + "\n");
    
    gShapes->addVertex(triangular, 0, 0);
    gShapes->addVertex(triangular, 2, 0);
    gShapes->addVertex(triangular, 1, 1*sqrt3);
    gShapes->addBoundaryEdges(triangular);
    size_t triangularCentre = gActions->getNewID(triangular, gShapes->getName(triangular), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictTriangular = gPictures->createPicture();
    gPictures->addShape(pictTriangular, triangular);
    gPictures->addAction(pictTriangular, triangularCentre);
    if (isIncludeNeighbours)
    {
        size_t triangular1 = gActions->getNewID(triangular, gShapes->getName(triangular), "colorNeighbour", neighbourThickness, 0, 0, 60);
        size_t triangular2 = gActions->getNewID(triangular, gShapes->getName(triangular), "colorNeighbour", neighbourThickness, 0, 0, 120);
        size_t triangular3 = gActions->getNewID(triangular, gShapes->getName(triangular), "colorNeighbour", neighbourThickness, 0, 0, 180);
        size_t triangular4 = gActions->getNewID(triangular, gShapes->getName(triangular), "colorNeighbour", neighbourThickness, 0, 0, 240);
        size_t triangular5 = gActions->getNewID(triangular, gShapes->getName(triangular), "colorNeighbour", neighbourThickness, 0, 0, 300);
        size_t triangular6 = gActions->getNewID(triangular, gShapes->getName(triangular), "colorNeighbour", neighbourThickness, 1, 1*sqrt3, 0);
        size_t triangular7 = gActions->getNewID(triangular, gShapes->getName(triangular), "colorNeighbour", neighbourThickness, 1, 1*sqrt3, 60);
        size_t triangular8 = gActions->getNewID(triangular, gShapes->getName(triangular), "colorNeighbour", neighbourThickness, 1, 1*sqrt3, 120);
        size_t triangular9 = gActions->getNewID(triangular, gShapes->getName(triangular), "colorNeighbour", neighbourThickness, 2, 0, 0);
        size_t triangular10 = gActions->getNewID(triangular, gShapes->getName(triangular), "colorNeighbour", neighbourThickness, 2, 0, 60);
        size_t triangular11 = gActions->getNewID(triangular, gShapes->getName(triangular), "colorNeighbour", neighbourThickness, 2, 0, 240);
        size_t triangular12 = gActions->getNewID(triangular, gShapes->getName(triangular), "colorNeighbour", neighbourThickness, 2, 0, 300);
        gPictures->addAction(pictTriangular, triangular1);
        gPictures->addAction(pictTriangular, triangular2);
        gPictures->addAction(pictTriangular, triangular3);
        gPictures->addAction(pictTriangular, triangular4);
        gPictures->addAction(pictTriangular, triangular5);
        gPictures->addAction(pictTriangular, triangular6);
        gPictures->addAction(pictTriangular, triangular7);
        gPictures->addAction(pictTriangular, triangular8);
        gPictures->addAction(pictTriangular, triangular9);
        gPictures->addAction(pictTriangular, triangular10);
        gPictures->addAction(pictTriangular, triangular11);
        gPictures->addAction(pictTriangular, triangular12);
    }
    gShapes->split(triangular);
    gPictures->write(pictTriangular);
}

void createTileAnchorTest1()
{
    tileAnchorTest1 = gShapes->getNewID("tileAnchorTest1");
    
    debug(lavesDebugSimple, "tileAnchorTest1 shape id = " + to_string(tileAnchorTest1) + "\n");
    
    gShapes->addVertex(tileAnchorTest1, 0, 0, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest1, 0, 1.5, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest1, 1, 2, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest1, 2, 3, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest1, 3, 3);
    gShapes->addVertex(tileAnchorTest1, 3, 0);
    gShapes->addVertex(tileAnchorTest1, 1.5, -1);
    gShapes->addBoundaryEdges(tileAnchorTest1);
    size_t tileAnchorTest1Action = gActions->getNewID(tileAnchorTest1, gShapes->getName(tileAnchorTest1), "colorBorder", centralThickness, 0, 0, 0);
    size_t picttileAnchorTest1 = gPictures->createPicture();
    gPictures->addShape(picttileAnchorTest1, tileAnchorTest1);
    gPictures->addAction(picttileAnchorTest1, tileAnchorTest1Action);
    gShapes->split(tileAnchorTest1);
    gPictures->write(picttileAnchorTest1);
}

void createTileAnchorTest2()
{
    tileAnchorTest2 = gShapes->getNewID("tileAnchorTest2");
    
    debug(lavesDebugSimple, "tileAnchorTest2 shape id = " + to_string(tileAnchorTest2) + "\n");
    
    gShapes->addVertex(tileAnchorTest2, 0, 0, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest2, 0, 1.5, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest2, 1, 2, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest2, 2, 3);
    gShapes->addVertex(tileAnchorTest2, 3, 3);
    gShapes->addVertex(tileAnchorTest2, 3, 0);
    gShapes->addVertex(tileAnchorTest2, 1.5, -1, vertexType::Knot);
    gShapes->addBoundaryEdges(tileAnchorTest2);
    size_t tileAnchorTest2Action = gActions->getNewID(tileAnchorTest2, gShapes->getName(tileAnchorTest2), "colorBorder", centralThickness, 0, 0, 0);
    size_t picttileAnchorTest2 = gPictures->createPicture();
    gPictures->addShape(picttileAnchorTest2, tileAnchorTest2);
    gPictures->addAction(picttileAnchorTest2, tileAnchorTest2Action);
    gShapes->split(tileAnchorTest2);
    gPictures->write(picttileAnchorTest2);
}

void createTileAnchorTest3()
{
    tileAnchorTest3 = gShapes->getNewID("tileAnchorTest3");
    
    debug(lavesDebugSimple, "tileAnchorTest3 shape id = " + to_string(tileAnchorTest3) + "\n");
    
    gShapes->addVertex(tileAnchorTest3, 0, 0, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest3, 0, 1.5, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest3, 1, 2);
    gShapes->addVertex(tileAnchorTest3, 2, 3);
    gShapes->addVertex(tileAnchorTest3, 3, 3);
    gShapes->addVertex(tileAnchorTest3, 3, 0, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest3, 1.5, -1, vertexType::Knot);
    gShapes->addBoundaryEdges(tileAnchorTest3);
    size_t tileAnchorTest3Action = gActions->getNewID(tileAnchorTest3, gShapes->getName(tileAnchorTest3), "colorBorder", centralThickness, 0, 0, 0);
    size_t picttileAnchorTest3 = gPictures->createPicture();
    gPictures->addShape(picttileAnchorTest3, tileAnchorTest3);
    gPictures->addAction(picttileAnchorTest3, tileAnchorTest3Action);
    gShapes->split(tileAnchorTest3);
    gPictures->write(picttileAnchorTest3);
}

void createTileAnchorTest4()
{
    tileAnchorTest4 = gShapes->getNewID("tileAnchorTest4");
    
    debug(lavesDebugSimple, "tileAnchorTest4 shape id = " + to_string(tileAnchorTest4) + "\n");
    
    gShapes->addVertex(tileAnchorTest4, 0, 0, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest4, 0, 1.5);
    gShapes->addVertex(tileAnchorTest4, 1, 2);
    gShapes->addVertex(tileAnchorTest4, 2, 3);
    gShapes->addVertex(tileAnchorTest4, 3, 3, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest4, 3, 0, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest4, 1.5, -1, vertexType::Knot);
    gShapes->addBoundaryEdges(tileAnchorTest4);
    size_t tileAnchorTest4Action = gActions->getNewID(tileAnchorTest4, gShapes->getName(tileAnchorTest4), "colorBorder", centralThickness, 0, 0, 0);
    size_t picttileAnchorTest4 = gPictures->createPicture();
    gPictures->addShape(picttileAnchorTest4, tileAnchorTest4);
    gPictures->addAction(picttileAnchorTest4, tileAnchorTest4Action);
    gShapes->split(tileAnchorTest4);
    gPictures->write(picttileAnchorTest4);
}

void createTileAnchorTest5()
{
    tileAnchorTest5 = gShapes->getNewID("tileAnchorTest5");
    
    debug(lavesDebugSimple, "tileAnchorTest5 shape id = " + to_string(tileAnchorTest5) + "\n");
    
    gShapes->addVertex(tileAnchorTest5, 0, 0);
    gShapes->addVertex(tileAnchorTest5, 0, 1.5);
    gShapes->addVertex(tileAnchorTest5, 1, 2);
    gShapes->addVertex(tileAnchorTest5, 2, 3, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest5, 3, 3, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest5, 3, 0, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest5, 1.5, -1, vertexType::Knot);
    gShapes->addBoundaryEdges(tileAnchorTest5);
    size_t tileAnchorTest5Action = gActions->getNewID(tileAnchorTest5, gShapes->getName(tileAnchorTest5), "colorBorder", centralThickness, 0, 0, 0);
    size_t picttileAnchorTest5 = gPictures->createPicture();
    gPictures->addShape(picttileAnchorTest5, tileAnchorTest5);
    gPictures->addAction(picttileAnchorTest5, tileAnchorTest5Action);
    gShapes->split(tileAnchorTest5);
    gPictures->write(picttileAnchorTest5);
}

void createTileAnchorTest6()
{
    tileAnchorTest6 = gShapes->getNewID("tileAnchorTest6");
    
    debug(lavesDebugSimple, "tileAnchorTest6 shape id = " + to_string(tileAnchorTest6) + "\n");
    
    gShapes->addVertex(tileAnchorTest6, 0, 0);
    gShapes->addVertex(tileAnchorTest6, 0, 1.5);
    gShapes->addVertex(tileAnchorTest6, 1, 2, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest6, 2, 3, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest6, 3, 3, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest6, 3, 0, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest6, 1.5, -1);
    gShapes->addBoundaryEdges(tileAnchorTest6);
    size_t tileAnchorTest6Action = gActions->getNewID(tileAnchorTest6, gShapes->getName(tileAnchorTest6), "colorBorder", centralThickness, 0, 0, 0);
    size_t picttileAnchorTest6 = gPictures->createPicture();
    gPictures->addShape(picttileAnchorTest6, tileAnchorTest6);
    gPictures->addAction(picttileAnchorTest6, tileAnchorTest6Action);
    gShapes->split(tileAnchorTest6);
    gPictures->write(picttileAnchorTest6);
}

void createTileAnchorTest7()
{
    tileAnchorTest7 = gShapes->getNewID("tileAnchorTest7");
    
    debug(lavesDebugSimple, "tileAnchorTest7 shape id = " + to_string(tileAnchorTest7) + "\n");
    
    gShapes->addVertex(tileAnchorTest7, 0, 0);
    gShapes->addVertex(tileAnchorTest7, 0, 1.5);
    gShapes->addVertex(tileAnchorTest7, 1, 2, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest7, 2, 3, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest7, 3, 3);
    gShapes->addVertex(tileAnchorTest7, 3, 0);
    gShapes->addVertex(tileAnchorTest7, 1.5, -1);
    gShapes->addBoundaryEdges(tileAnchorTest7);
    size_t tileAnchorTest7Action = gActions->getNewID(tileAnchorTest7, gShapes->getName(tileAnchorTest7), "colorBorder", centralThickness, 0, 0, 0);
    size_t picttileAnchorTest7 = gPictures->createPicture();
    gPictures->addShape(picttileAnchorTest7, tileAnchorTest7);
    gPictures->addAction(picttileAnchorTest7, tileAnchorTest7Action);
    gShapes->split(tileAnchorTest7);
    gPictures->write(picttileAnchorTest7);
}

void createTileAnchorTest8()
{
    tileAnchorTest8 = gShapes->getNewID("tileAnchorTest8");
    
    debug(lavesDebugSimple, "tileAnchorTest8 shape id = " + to_string(tileAnchorTest8) + "\n");
    
    gShapes->addVertex(tileAnchorTest8, 0, 0, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest8, 0, 1.5);
    gShapes->addVertex(tileAnchorTest8, 1, 2);
    gShapes->addVertex(tileAnchorTest8, 2, 3);
    gShapes->addVertex(tileAnchorTest8, 3, 3);
    gShapes->addVertex(tileAnchorTest8, 3, 0);
    gShapes->addVertex(tileAnchorTest8, 1.5, -1);
    gShapes->addBoundaryEdges(tileAnchorTest8);
    size_t tileAnchorTest8Action = gActions->getNewID(tileAnchorTest8, gShapes->getName(tileAnchorTest8), "colorBorder", centralThickness, 0, 0, 0);
    size_t picttileAnchorTest8 = gPictures->createPicture();
    gPictures->addShape(picttileAnchorTest8, tileAnchorTest8);
    gPictures->addAction(picttileAnchorTest8, tileAnchorTest8Action);
    gShapes->split(tileAnchorTest8);
    gPictures->write(picttileAnchorTest8);
}

void createTileAnchorTest9()
{
    tileAnchorTest9 = gShapes->getNewID("tileAnchorTest9");
    
    debug(lavesDebugSimple, "tileAnchorTest9 shape id = " + to_string(tileAnchorTest9) + "\n");
    
    gShapes->addVertex(tileAnchorTest9, 0, 0, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest9, 0, 1.5);
    gShapes->addVertex(tileAnchorTest9, 1, 2);
    gShapes->addVertex(tileAnchorTest9, 2, 3, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest9, 3, 3, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest9, 3, 0, vertexType::Knot);
    gShapes->addVertex(tileAnchorTest9, 1.5, -1, vertexType::Knot);
    gShapes->addBoundaryEdges(tileAnchorTest9);
    size_t tileAnchorTest9Action = gActions->getNewID(tileAnchorTest9, gShapes->getName(tileAnchorTest9), "colorBorder", centralThickness, 0, 0, 0);
    size_t picttileAnchorTest9 = gPictures->createPicture();
    gPictures->addShape(picttileAnchorTest9, tileAnchorTest9);
    gPictures->addAction(picttileAnchorTest9, tileAnchorTest9Action);
    gShapes->split(tileAnchorTest9);
    gPictures->write(picttileAnchorTest9);
}

void createTileSplit()
{
    tileSplit = gShapes->getNewID("tileSplit");
    
    debug(lavesDebugSimple, "tileSplit shape id = " + to_string(tileSplit) + "\n");
    
    gShapes->addVertex(tileSplit, 0, 0);
    gShapes->addVertex(tileSplit, 3./3., 3./6., vertexType::Knot);
    gShapes->addVertex(tileSplit, 1.5, 1.5, vertexType::Knot);
    gShapes->addVertex(tileSplit, 3./3., 15./8., vertexType::Knot);
    gShapes->addVertex(tileSplit, 0, 1.5);
    gShapes->addVertex(tileSplit, 0, 3);
    gShapes->addVertex(tileSplit, 3, 3);
    gShapes->addVertex(tileSplit, 3, 0);
    gShapes->addBoundaryEdges(tileSplit);
    size_t tileSplitAction = gActions->getNewID(tileSplit, gShapes->getName(tileSplit), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictTileSplit = gPictures->createPicture();
    gPictures->addShape(pictTileSplit, tileSplit);
    gPictures->addAction(pictTileSplit, tileSplitAction);
    gShapes->split(tileSplit);
    gPictures->write(pictTileSplit);
}

void createTileSplitTriangle()
{
    tileSplitTriangle = gShapes->getNewID("tileSplitTriangle");
    
    debug(lavesDebugSimple, "tileSplitTriangle shape id = " + to_string(tileSplitTriangle) + "\n");
    
    gShapes->addVertex(tileSplitTriangle, 0, 0);
    gShapes->addVertex(tileSplitTriangle, 1, 1*sqrt3);
    gShapes->addVertex(tileSplitTriangle, 2, 0);
    gShapes->addBoundaryEdges(tileSplitTriangle);
    size_t tileSplitTriangleAction = gActions->getNewID(tileSplitTriangle, gShapes->getName(tileSplitTriangle), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictTileSplitTriangle = gPictures->createPicture();
    gPictures->addShape(pictTileSplitTriangle, tileSplitTriangle);
    gPictures->addAction(pictTileSplitTriangle, tileSplitTriangleAction);
    gShapes->split(tileSplitTriangle);
    gPictures->write(pictTileSplitTriangle);
}

void createTileSplitQuad()
{
    size_t tileSplitQuad = gShapes->getNewID("tileSplitQuad");
    
    debug(lavesDebugSimple, "tileSplitQuad shape id = " + to_string(tileSplitQuad) + "\n");
    
    gShapes->addVertex(tileSplitQuad, 0, 0);
    gShapes->addVertex(tileSplitQuad, -1, 1);
    gShapes->addVertex(tileSplitQuad, 0, 2);
    gShapes->addVertex(tileSplitQuad, 2, 2);
    gShapes->addBoundaryEdges(tileSplitQuad);
    size_t tileSplitQuadAction = gActions->getNewID(tileSplitQuad, gShapes->getName(tileSplitQuad), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictTileSplitQuad = gPictures->createPicture();
    gPictures->addShape(pictTileSplitQuad, tileSplitQuad);
    gPictures->addAction(pictTileSplitQuad, tileSplitQuadAction);
    gShapes->split(tileSplitQuad);
    gPictures->write(pictTileSplitQuad);
}

void createTileSplitPent()
{
    tileSplitPent = gShapes->getNewID("tileSplitPent");
    
    debug(lavesDebugSimple, "tileSplitPent shape id = " + to_string(tileSplitPent) + "\n");
    
    gShapes->addVertex(tileSplitPent, -1, 1);
    gShapes->addVertex(tileSplitPent, 0, 0);
    gShapes->addVertex(tileSplitPent, 2, 0);
    gShapes->addVertex(tileSplitPent, 2, 1);
    gShapes->addVertex(tileSplitPent, 2, 2);
    gShapes->addVertex(tileSplitPent, 0, 2);
    gShapes->addBoundaryEdges(tileSplitPent);
    size_t tileSplitPentAction = gActions->getNewID(tileSplitPent, gShapes->getName(tileSplitPent), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictTileSplitPent = gPictures->createPicture();
    gPictures->addShape(pictTileSplitPent, tileSplitPent);
    gPictures->addAction(pictTileSplitPent, tileSplitPentAction);
    gShapes->split(tileSplitPent);
    gPictures->write(pictTileSplitPent);
}

void createTileSplitBent()
{
    tileSplitBent = gShapes->getNewID("tileSplitBent");
    
    debug(lavesDebugSimple, "tileSplitBent shape id = " + to_string(tileSplitBent) + "\n");
    
    gShapes->addVertex(tileSplitBent, 0, 0);
    gShapes->addVertex(tileSplitBent, 2.5, 2.5);
    gShapes->addVertex(tileSplitBent, 4, 4.5);
    gShapes->addVertex(tileSplitBent, 5.5, 0.5);
    gShapes->addVertex(tileSplitBent, 5.5, 3.5);
    gShapes->addVertex(tileSplitBent, 6.5, 2.5);
    gShapes->addVertex(tileSplitBent, 7.5, 1);
    gShapes->addVertex(tileSplitBent, 6.25, 4);
    gShapes->addVertex(tileSplitBent, 4.75, 5);
    gShapes->addVertex(tileSplitBent, 4.25, 5);
    gShapes->addVertex(tileSplitBent, 3, 5.5);
    gShapes->addVertex(tileSplitBent, 0, 4);
    gShapes->addBoundaryEdges(tileSplitBent);
    size_t tileSplitBentAction = gActions->getNewID(tileSplitBent, gShapes->getName(tileSplitBent), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictTileSplitBent = gPictures->createPicture();
    gPictures->addShape(pictTileSplitBent, tileSplitBent);
    gPictures->addAction(pictTileSplitBent, tileSplitBentAction);
    gShapes->split(tileSplitBent);
    gPictures->write(pictTileSplitBent);
}

void createTileSplitStar()
{
    tileSplitStar = gShapes->getNewID("tileSplitStar");
    
    debug(lavesDebugSimple, "tileSplitStar shape id = " + to_string(tileSplitStar) + "\n");
    
    gShapes->addVertex(tileSplitStar, 0, 0);
    gShapes->addVertex(tileSplitStar, -2, 2);
    gShapes->addVertex(tileSplitStar, 0, 3);
    gShapes->addVertex(tileSplitStar, -1, 4);
    gShapes->addVertex(tileSplitStar, 2, 5);
    gShapes->addVertex(tileSplitStar, 1, 2);
    gShapes->addVertex(tileSplitStar, 4, 4);
    gShapes->addVertex(tileSplitStar, 3, 2);
    gShapes->addVertex(tileSplitStar, 5, 0);
    gShapes->addVertex(tileSplitStar, 2, 1);
    gShapes->addVertex(tileSplitStar, 2, 2);
    gShapes->addBoundaryEdges(tileSplitStar);
    size_t tileSplitStarAction = gActions->getNewID(tileSplitStar, gShapes->getName(tileSplitStar), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictTileSplitStar = gPictures->createPicture();
    gPictures->addShape(pictTileSplitStar, tileSplitStar);
    gPictures->addAction(pictTileSplitStar, tileSplitStarAction);
    gShapes->split(tileSplitStar);
    gPictures->write(pictTileSplitStar);
}

void createTileBroken()
{
    tileBroken = gShapes->getNewID("tileBroken");
    
    debug(lavesDebugSimple, "tileBroken shape id = " + to_string(tileBroken) + "\n");

    gShapes->addVertex(tileBroken, 0, 0);
    gShapes->addVertex(tileBroken, 1./6., sqrt3/6., vertexType::Knot);
    gShapes->addVertex(tileBroken, -0.125, sqrt3/8.);
    gShapes->addVertex(tileBroken, -0.25, sqrt3/4., vertexType::Knot);
    gShapes->addVertex(tileBroken, -0.375, 3*sqrt3/8., vertexType::Knot);
    gShapes->addVertex(tileBroken, -0.5, sqrt3/2.);
    gShapes->addVertex(tileBroken, -0.375, 5*sqrt3/8., vertexType::Knot);
    gShapes->addVertex(tileBroken, -0.25, 3*sqrt3/4., vertexType::Knot);
    gShapes->addVertex(tileBroken, -0.125, 7*sqrt3/8., vertexType::Knot);
    gShapes->addVertex(tileBroken, 0, sqrt3);
    gShapes->addVertex(tileBroken, 0.25, sqrt3, vertexType::Knot);
    gShapes->addVertex(tileBroken, 0.5, sqrt3, vertexType::Knot);
    gShapes->addVertex(tileBroken, 0.75, sqrt3, vertexType::Knot);
    gShapes->addVertex(tileBroken, 1, sqrt3);
    gShapes->addVertex(tileBroken, 1.125, 7*sqrt3/8., vertexType::Knot);
    gShapes->addVertex(tileBroken, 1.25, 3*sqrt3/4., vertexType::Knot);
    gShapes->addVertex(tileBroken, 1.375, 5*sqrt3/8., vertexType::Knot);
    gShapes->addVertex(tileBroken, 1.5, sqrt3/2.);
    gShapes->addVertex(tileBroken, 1.375, 3*sqrt3/8., vertexType::Knot);
    gShapes->addVertex(tileBroken, 1.25, sqrt3/4., vertexType::Knot);
    gShapes->addVertex(tileBroken, 1.125, sqrt3/8., vertexType::Knot);
    gShapes->addVertex(tileBroken, 1, 0);
    gShapes->addVertex(tileBroken, 0.75, 0, vertexType::Knot);
    gShapes->addVertex(tileBroken, 0.5, 0, vertexType::Knot);
    gShapes->addVertex(tileBroken, 0.25, 0, vertexType::Knot);
    gShapes->addBoundaryEdges(tileBroken);
    size_t tileBrokenAction = gActions->getNewID(tileBroken, gShapes->getName(tileBroken), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictTileBroken = gPictures->createPicture();
    gPictures->addShape(pictTileBroken, tileBroken);
    gPictures->addAction(pictTileBroken, tileBrokenAction);
    gShapes->split(tileBroken);
    gPictures->write(pictTileBroken);
}

void createTileSquareBroken()
{
    tileSquareBroken = gShapes->getNewID("tileSquareBroken");
    
    debug(lavesDebugSimple, "tileSquareBroken shape id = " + to_string(tileSquareBroken) + "\n");

    gShapes->addVertex(tileSquareBroken, 0.25, 1);
    gShapes->addVertex(tileSquareBroken, 0.5, 1, vertexType::Knot);
    gShapes->addVertex(tileSquareBroken, 0.75, 1, vertexType::Knot);
    gShapes->addVertex(tileSquareBroken, 1, 1);
    gShapes->addVertex(tileSquareBroken, 1, 0.75, vertexType::Knot);
    gShapes->addVertex(tileSquareBroken, 1, 0.5, vertexType::Knot);
    gShapes->addVertex(tileSquareBroken, 1, 0.25, vertexType::Knot);
    gShapes->addVertex(tileSquareBroken, 1, 0);
    gShapes->addVertex(tileSquareBroken, 0.75, 0, vertexType::Knot);
    gShapes->addVertex(tileSquareBroken, 0.5, 0, vertexType::Knot);
    gShapes->addVertex(tileSquareBroken, 0.25, 0, vertexType::Knot);
    gShapes->addVertex(tileSquareBroken, 0, 0);
    gShapes->addVertex(tileSquareBroken, 0, 0.25);
    gShapes->addVertex(tileSquareBroken, 1./3., 1./3., vertexType::Knot);
    gShapes->addVertex(tileSquareBroken, 0.5, 0.5, vertexType::Knot);
    gShapes->addVertex(tileSquareBroken, 2./3., 2./3., vertexType::Knot);
    gShapes->addBoundaryEdges(tileSquareBroken);
    size_t tileSquareBrokenAction = gActions->getNewID(tileSquareBroken, gShapes->getName(tileSquareBroken), "colorBorder", centralThickness, 0, 0, 0);
    size_t pictTileSquareBroken = gPictures->createPicture();
    gPictures->addShape(pictTileSquareBroken, tileSquareBroken);
    gPictures->addAction(pictTileSquareBroken, tileSquareBrokenAction);
    gShapes->split(tileSquareBroken);
    gPictures->write(pictTileSquareBroken);
}


void createLaves()
{
    if (isLaves || isHex) createHex();
    if (isLaves || isFloretLeft) createFloretLeft();
    if (isLaves || isFloretRight) createFloretRight();
    if (isLaves || isPrismaticPentagonal) createPrismaticPentagonal();
    if (isLaves || isCairo) createCairo();
    if (isLaves || isKite) createKite();
    if (isLaves || isLozenge) createLozenge();
    if (isLaves || isTriakisTriangular) createTriakisTriangular();
    if (isLaves || isSquare) createSquare();
    if (isLaves || isKisrhombille) createKisrhombille();
    if (isLaves || isTetrakisSquare) createTetrakisSquare();
    if (isLaves || isTriangular) createTriangular();
}

void createTests()
{
    if (isTests || isTileAnchorTest1) createTileAnchorTest1();
    if (isTests || isTileAnchorTest2) createTileAnchorTest2();
    if (isTests || isTileAnchorTest3) createTileAnchorTest3();
    if (isTests || isTileAnchorTest4) createTileAnchorTest4();
    if (isTests || isTileAnchorTest5) createTileAnchorTest5();
    if (isTests || isTileAnchorTest6) createTileAnchorTest6();
    if (isTests || isTileAnchorTest7) createTileAnchorTest7();
    if (isTests || isTileAnchorTest8) createTileAnchorTest8();
    if (isTests || isTileAnchorTest9) createTileAnchorTest9();
    if (isTests || isTileSplit) createTileSplit();
    if (isTests || isTileSplitTriangle) createTileSplitTriangle();
    if (isTests || isTileSplitQuad) createTileSplitQuad();
    if (isTests || isTileSplitPent) createTileSplitPent();
    if (isTests || isTileSplitBent) createTileSplitBent();
    if (isTests || isTileSplitStar) createTileSplitStar();
    if (isTests || isTileBroken) createTileBroken();
    if (isTests || isTileSquareBroken) createTileSquareBroken();
}
