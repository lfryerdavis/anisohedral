//
//  picture.cpp
//
//
//  Created by Lee Fryer-Davis on 2025-06-28.
//

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <queue>
#include <utility> // pair
#include "common.hpp"
#include "picture.hpp"

using namespace std;

extern ofstream outFile;
extern shapes *gShapes;
extern actions *gActions;

size_t pictureDebugSimple = 1;
size_t pictureDebugExtra = 2;

picture::picture()
{
}

picture::~picture()
{
}

void picture::addShape(size_t shapeID)
{
    mShapes.push_back(shapeID);
}

void picture::addBaseShape(size_t shapeID)
{
    mBaseShapes.push_back(shapeID);
}

void picture::addAction(size_t actionID)
{
    mActions.push_back(actionID);
}

void picture::write()
{
    debug(pictureDebugSimple, "Picture: to write \n");

    bool baseGraph = isBoundaryOnly
        or showInternalGraphAndBoundaries
        or showInternalGraphOnly
        or showVertexIndices
        or showTriangulation
        or isShowSplitGraphs
    or isShowSplitPaths;

    // write out each shape definition
    vector<size_t> writingShapes;
    if (baseGraph)
    {
        debug(pictureDebugExtra, "Only write base graph\n");
        if (mBaseShapes.size() == 0)
        {
            writingShapes.push_back(mShapes[0]);
        }
        else
        {
            debug(pictureDebugExtra, "More than one base graph\n");
            for (size_t i = 0; i < mBaseShapes.size(); i++)
            {
                writingShapes.push_back(mBaseShapes[i]);
            }
        }
    }
    else
    {
        debug(pictureDebugExtra, "Don't write just base graphs\n");
        // need to fill out the writing of a random middle set of shapes
        queue<size_t> openShapes;
        
        openShapes.push(mShapes[0]);
        debug(pictureDebugExtra, "Starting queue\n");

        while (!openShapes.empty())
        {
            size_t u = openShapes.front();
            openShapes.pop();
            
            debug(pictureDebugExtra, "Picture write shape u = " + to_string(u) + "\n");

            shape * ps = gShapes->getShape(u);
            
            size_t numChildren = ps->subshapeIDs.size();
            debug(pictureDebugExtra, "Subshape ids = ");
            for (size_t i = 0; i < numChildren; i++)
            {
                debug(pictureDebugExtra, "( " + to_string(ps->subshapeIDs[i].first) + ", " + to_string(ps->subshapeIDs[i].second) + ") ");
            }
            debug(pictureDebugExtra, "\n");

            if (numChildren == 0)
            {
                writingShapes.push_back(u);
                continue;
            }

            // include no option or one of the child pairs
            size_t pick = rand() % (numChildren + 1);
            if (pick == numChildren) // pick no children
            {
                writingShapes.push_back(u);
            }
            else
            {
                openShapes.push(ps->subshapeIDs[pick].first);
                openShapes.push(ps->subshapeIDs[pick].second);
            }

            debug(pictureDebugExtra, "Picture: accumulating writingShapes = ");
            for (size_t i = 0; i < writingShapes.size(); i++)
            {
                debug(pictureDebugExtra, to_string(writingShapes[i]) + " ");
            }
            debug(pictureDebugExtra, "\n");

        }
    }

    debug(pictureDebugExtra, "Picture: writingShapes = ");
    for (size_t i = 0; i < writingShapes.size(); i++)
    {
        debug(pictureDebugExtra, to_string(writingShapes[i]) + " ");
    }
    debug(pictureDebugExtra, "\n");

    for (size_t i = 0; i < writingShapes.size(); i++)
    {
        gShapes->write(writingShapes[i]);
    }
        
    outFile << endl << "\\begin{tikzpicture}" << endl;
    // write each shape with appropriate set of actions
    for (size_t i = 0; i < writingShapes.size(); i++)
    {
        for (size_t j = gShapes->getShape(writingShapes[i])->mActions.size() - 1; j < gShapes->getShape(writingShapes[i])->mActions.size(); j--)
        {
            debug(pictureDebugExtra, "Picture: j = " + to_string(j) + " shape = " + to_string(writingShapes[i]) + " action = " + to_string(gShapes->getShape(writingShapes[i])->mActions[j]) + "\n");
            gActions->write(gShapes->getShape(writingShapes[i])->mActions[j]);
        }
    }
    outFile << "\\end{tikzpicture}" << endl;
}

pictures::pictures()
{
    
}

pictures::~pictures()
{
    // end the output file
    outFile << endl << "\\end{document}" << endl << endl;
    // close tikz output file
    outFile.close();
}

size_t pictures::createPicture()
{
    picture pict;
    mPictures.push_back(pict);
    return mPictures.size() - 1;
}

void pictures::createPictures(string outname, bool isStandalonePicture)
{
    mIsStandalonePicture = isStandalonePicture;
    if (outname == "")
    {
        cout << "No file to open" << endl;
        return;
    }

    mOutname = outname;
    // open tikz output file
    outFile.open(mOutname, fstream::out);
    
    if (!outFile.is_open())
    {
        cout << "Cannot open file: " << mOutname << endl;
        exit(1);
    }
    
    if (mIsStandalonePicture)
        writeStandalonePreamble();
    else
        writeEmbeddedPreamble();
}


void pictures::writeStandalonePreamble()
{
    outFile << "\\documentclass[tikz, border = 0.3cm]{standalone} " << endl << endl;
    writeCommonPreamble();
}

void pictures::writeEmbeddedPreamble()
{
    outFile << "\\documentclass{article} " << endl << endl;
    writeCommonPreamble();
}

void pictures::writeCommonPreamble()
{
    outFile << "\\usepackage{tikz}" << endl;
    outFile << "\\usetikzlibrary{math, calc, arrows.meta}" << endl << endl;
    outFile << "\\begin{document}" << endl << endl;
    outFile << "\\def\\gap{\\vspace{2cm}}" << endl << endl;
    outFile << "\\definecolor{colorBorder}{HTML}{000000}" << endl;
    outFile << "\\definecolor{color1}{HTML}{FEC0B3}" << endl;
    outFile << "\\definecolor{color2}{HTML}{FFD5E9}" << endl;
    outFile << "\\definecolor{colorNeighbour}{HTML}{D6BDE4}" << endl;
    outFile << "\\definecolor{color4}{HTML}{A020F0}" << endl;
    outFile << "\\definecolor{color5}{HTML}{CE90C8}" << endl;
    outFile << "\\definecolor{colorKnot}{HTML}{1212C8}" << endl;
    outFile << "\\definecolor{colorAnchor}{HTML}{15C812}" << endl;
    outFile << "\\definecolor{colorAnchorKnot}{HTML}{03FC30}" << endl;
    outFile << "\\definecolor{colorCentroid}{HTML}{EB4034}" << endl;
    outFile << "\\definecolor{colorGraphEdge}{HTML}{98A4D8}" << endl;
    outFile << "\\def\\reg{6}" << endl;
    outFile << "\\def\\regs{3}" << endl;
    outFile << "\\def\\rad{0.15}" << endl;
    outFile << "\\def\\radk{0.1}" << endl << endl;
}

void pictures::addShape(size_t pict, size_t shapeID)
{
    mPictures[pict].addShape(shapeID);
    gShapes->addPicture(shapeID, pict);
}

void pictures::addBaseShape(size_t pict, size_t shapeID)
{
    mPictures[pict].addBaseShape(shapeID);
}

void pictures::addAction(size_t pict, size_t actionID)
{
    mPictures[pict].addAction(actionID);
}

void pictures::write(size_t pict)
{
    // write all definitions, then actions on each shape
    outFile << "\\gap" << endl;
    mPictures[pict].write();
}

void pictures::writeAll()
{
    for (size_t i = 0; i < mPictures.size(); i++)
    {
        outFile << "\\gap" << endl;
        mPictures[i].write();
    }
}
