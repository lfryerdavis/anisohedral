//
//  anisohedral.hpp
//
//  Created by Lee Fryer-Davis on 2025-06-28.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <cstdlib> // used for random to display a random path - eventually cut
#include <ctime>
#include "common.hpp"
#include "laves.hpp"
#include "action.hpp"
#include "picture.hpp"
#include "shape.hpp"

using namespace std;

string outname = "out.tex"; // default output filename

size_t maxdepth = 1;
size_t debugLevel = 0;
size_t aniDebugSimple = 1;
size_t aniDebugExtra = 3;

ofstream outFile;

shapes *gShapes;
actions *gActions;
pictures *gPictures;

void debug(size_t level, string out)
{
    if (level <= debugLevel)
        cout << out;
}

string boolToStr(bool value)
{
    if (value)
        return "true";
    return "false";
}

int main( int argc, char **argv )
{
    gShapes = new shapes;
    gActions = new actions;
    gPictures = new pictures;

    srand(time(0)); // use for random output - eventually cut

    // need to switch over to a list of bools with corresponding option strings
    // it will clean this code up significantly
    
	size_t idx = 1;
	while( idx < argc ) {
        if( !strcmp( argv[idx], "-depth" ) ) {
            maxdepth = atoi( argv[idx+1] );
            ++idx;
            debug(aniDebugSimple, "arguments depth = " + to_string(maxdepth) + "\n");
        } else if ( !strcmp( argv[idx], "-debug" ) ) {
            debugLevel = atoi( argv[idx+1] );
            ++idx;
            debug(aniDebugSimple, "arguments debug level = " + to_string(debugLevel) + "\n");
        } else if ( !strcmp( argv[idx], "-o" ) ) {
            ++idx;
            outname = argv[idx];
            debug(aniDebugSimple, "arguments output name = " + outname + "\n");
        } else if ( !strcmp( argv[idx], "-standalone" ) ) {
            isStandalonePicture = true;
            debug(aniDebugSimple, "arguments standalone = " + boolToStr(isStandalonePicture) + "\n");
        } else if ( !strcmp( argv[idx], "-laves" ) ) {
            isLaves = true;
            debug(aniDebugSimple, "arguments laves tiles = " + boolToStr(isLaves) + "\n");
        } else if ( !strcmp( argv[idx], "-hex" ) ) {
            isHex = true;
            debug(aniDebugSimple, "arguments hex laves = " + boolToStr(isHex) + "\n");
        } else if ( !strcmp( argv[idx], "-floretleft" ) ) {
            isFloretLeft = true;
            debug(aniDebugSimple, "arguments floret left laves = " + boolToStr(isFloretLeft) + "\n");
        } else if ( !strcmp( argv[idx], "-floretright" ) ) {
            isFloretRight = true;
            debug(aniDebugSimple, "arguments floret right laves = " + boolToStr(isFloretRight) + "\n");
        } else if ( !strcmp( argv[idx], "-prismaticpentagonal" ) ) {
            isPrismaticPentagonal = true;
            debug(aniDebugSimple, "arguments prismatic pentagonal laves = " + boolToStr(isPrismaticPentagonal) + "\n");
        } else if ( !strcmp( argv[idx], "-cairo" ) ) {
            isCairo = true;
            debug(aniDebugSimple, "arguments cairo laves = " + boolToStr(isCairo) + "\n");
        } else if ( !strcmp( argv[idx], "-kite" ) ) {
            isKite = true;
            debug(aniDebugSimple, "arguments kite laves = " + boolToStr(isKite) + "\n");
        } else if ( !strcmp( argv[idx], "-lozenge" ) ) {
            isLozenge = true;
            debug(aniDebugSimple, "arguments lozenge laves = " + boolToStr(isLozenge) + "\n");
        } else if ( !strcmp( argv[idx], "-triakistriangular" ) ) {
            isTriakisTriangular = true;
            debug(aniDebugSimple, "arguments triakis triangular laves = " + boolToStr(isTriakisTriangular) + "\n");
        } else if ( !strcmp( argv[idx], "-square" ) ) {
            isSquare = true;
            debug(aniDebugSimple, "arguments square laves = " + boolToStr(isSquare) + "\n");
        } else if ( !strcmp( argv[idx], "-kisrhombille" ) ) {
            isKisrhombille = true;
            debug(aniDebugSimple, "arguments kisrhombille laves = " + boolToStr(isKisrhombille) + "\n");
        } else if ( !strcmp( argv[idx], "-tetrakissquare" ) ) {
            isTetrakisSquare = true;
            debug(aniDebugSimple, "arguments tetrakis square laves = " + boolToStr(isTetrakisSquare) + "\n");
        } else if ( !strcmp( argv[idx], "-triangular" ) ) {
            isTriangular = true;
            debug(aniDebugSimple, "arguments triangular laves = " + boolToStr(isTriangular) + "\n");
        } else if ( !strcmp( argv[idx], "-tests" ) ) {
            isTests = true;
            debug(aniDebugSimple, "arguments all tests = " + boolToStr(isTests) + "\n");
        } else if ( !strcmp( argv[idx], "-testanchor1" ) ) {
            isTileAnchorTest1 = true;
            debug(aniDebugSimple, "arguments test anchor1 test = " + boolToStr(isTileAnchorTest1) + "\n");
        } else if ( !strcmp( argv[idx], "-testanchor2" ) ) {
            isTileAnchorTest2 = true;
            debug(aniDebugSimple, "arguments test anchor2 test = " + boolToStr(isTileAnchorTest2) + "\n");
        } else if ( !strcmp( argv[idx], "-testanchor3" ) ) {
            isTileAnchorTest3 = true;
            debug(aniDebugSimple, "arguments test anchor3 test = " + boolToStr(isTileAnchorTest3) + "\n");
        } else if ( !strcmp( argv[idx], "-testanchor4" ) ) {
            isTileAnchorTest4 = true;
            debug(aniDebugSimple, "arguments test anchor4 test = " + boolToStr(isTileAnchorTest4) + "\n");
        } else if ( !strcmp( argv[idx], "-testanchor5" ) ) {
            isTileAnchorTest5 = true;
            debug(aniDebugSimple, "arguments test anchor5 test = " + boolToStr(isTileAnchorTest5) + "\n");
        } else if ( !strcmp( argv[idx], "-testanchor6" ) ) {
            isTileAnchorTest6 = true;
            debug(aniDebugSimple, "arguments test anchor6 test = " + boolToStr(isTileAnchorTest6) + "\n");
        } else if ( !strcmp( argv[idx], "-testanchor7" ) ) {
            isTileAnchorTest7 = true;
            debug(aniDebugSimple, "arguments test anchor7 test = " + boolToStr(isTileAnchorTest7) + "\n");
        } else if ( !strcmp( argv[idx], "-testanchor8" ) ) {
            isTileAnchorTest8 = true;
            debug(aniDebugSimple, "arguments test anchor8 test = " + boolToStr(isTileAnchorTest8) + "\n");
        } else if ( !strcmp( argv[idx], "-testanchor9" ) ) {
            isTileAnchorTest9 = true;
            debug(aniDebugSimple, "arguments test anchor9 test = " + boolToStr(isTileAnchorTest9) + "\n");
        } else if ( !strcmp( argv[idx], "-testsplit" ) ) {
            isTileSplit = true;
            debug(aniDebugSimple, "arguments test split = " + boolToStr(isTileSplit) + "\n");
        } else if ( !strcmp( argv[idx], "-testtriangle" ) ) {
            isTileSplitTriangle = true;
            debug(aniDebugSimple, "arguments test triangle = " + boolToStr(isTileSplitTriangle) + "\n");
        } else if ( !strcmp( argv[idx], "-testquad" ) ) {
            isTileSplitQuad = true;
            debug(aniDebugSimple, "arguments test quadrilateral = " + boolToStr(isTileSplitQuad) + "\n");
        } else if ( !strcmp( argv[idx], "-testpent" ) ) {
            isTileSplitPent = true;
            debug(aniDebugSimple, "arguments test pentagon = " + boolToStr(isTileSplitPent) + "\n");
        } else if ( !strcmp( argv[idx], "-testbent" ) ) {
            isTileSplitBent = true;
            debug(aniDebugSimple, "arguments test bent tile = " + boolToStr(isTileSplitBent) + "\n");
        } else if ( !strcmp( argv[idx], "-teststar" ) ) {
            isTileSplitStar = true;
            debug(aniDebugSimple, "arguments test star tile = " + boolToStr(isTileSplitStar) + "\n");
        } else if ( !strcmp( argv[idx], "-showsplitgraphs" ) ) {
            isShowSplitGraphs = true;
            debug(aniDebugSimple, "arguments show split graphs = " + boolToStr(isShowSplitGraphs) + "\n");
        } else if ( !strcmp( argv[idx], "-showsplitpaths" ) ) {
            isShowSplitPaths = true;
            debug(aniDebugSimple, "arguments show split paths = " + boolToStr(isShowSplitPaths) + "\n");
        } else if ( !strcmp( argv[idx], "-showneighbours" ) ) {
            isIncludeNeighbours = true;
            debug(aniDebugSimple, "arguments show neighbours for laves tiles = " + boolToStr(isIncludeNeighbours) + "\n");
        } else if ( !strcmp( argv[idx], "-showinternalgraphandboundaries" ) ) {
            showInternalGraphAndBoundaries = true;
            debug(aniDebugSimple, "arguments show internal graph and boundaries = " + boolToStr(showInternalGraphAndBoundaries) + "\n");
        } else if ( !strcmp( argv[idx], "-showinternalgraphonly" ) ) {
            showInternalGraphOnly = true;
            debug(aniDebugSimple, "arguments show internal graph only = " + boolToStr(showInternalGraphOnly) + "\n");
        } else if ( !strcmp( argv[idx], "-showvertexindices" ) ) {
            showVertexIndices = true;
            debug(aniDebugSimple, "arguments show vertex indices = " + boolToStr(showVertexIndices) + "\n");
        } else if ( !strcmp( argv[idx], "-showtriangulation" ) ) {
            showTriangulation = true;
            debug(aniDebugSimple, "arguments show triangulation = " + boolToStr(showTriangulation) + "\n");
        } else if ( !strcmp( argv[idx], "-boundaryonly" ) ) {
            isBoundaryOnly = true;
            debug(aniDebugSimple, "arguments is boundary only = " + boolToStr(isBoundaryOnly) + "\n");
        } else {
			cout << "Unrecognized parameter \"" << argv[idx] << "\""
				<< endl;
			exit(0);
		}
		++idx;
        debug(aniDebugExtra, "arguments idx = " + to_string(idx) + "\n");
	}
    
    gPictures->createPictures(outname, isStandalonePicture);

    createLaves();
    createTests();
        
    delete gShapes;
    delete gActions;
    delete gPictures;
    
    return 0;
}
