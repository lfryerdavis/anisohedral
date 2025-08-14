//
//  shape.cpp
//
//
//  Created by Lee Fryer-Davis on 2025-06-28.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <iomanip>
#include <algorithm>
#include <array>
#include <format>
#include <cstdlib> // used for random to display a random path - eventually cut
#include "common.hpp"
#include "shape.hpp"
#include "action.hpp"
#include "picture.hpp"
#include "earcut.hpp"

using namespace std;

extern ofstream outFile;

size_t shapeDebugSimple = 1;
size_t shapeDebugExtra = 2;
size_t shapeDebugDetailed = 3;

bool showInternalGraphAndBoundaries = false;
bool showInternalGraphOnly = false;
bool showVertexIndices = false;
bool showTriangulation = false;
bool isBoundaryOnly = false;

string vertex::print()
{
    string ret;
    ret = std::format("( {:g}, {:g}) type ", mX, mY);
//    ret = std::format("( {:.10f}, {:.10f}) type ", mX, mY);
    
    if (mType == vertexType::Vertex)
        ret += "Vertex";
    else if (mType == vertexType::Knot)
        ret += "Knot";
    else if (mType == vertexType::Anchor)
        ret += "Anchor";
    else if (mType == vertexType::AnchorKnot)
        ret += "AnchorKnot";
    else if (mType == vertexType::Centroid)
        ret += "Centroid";

    if (mNeighbourVertexIDs.size() != 0)
    {
        ret += " NhbIDs ";
        for (size_t i = 0; i < mNeighbourVertexIDs.size(); i++)
            ret += to_string(mNeighbourVertexIDs[i]) + " ";
    }

    if (mSplitGraphNeighbourVertexIDs.size() != 0)
    {
        ret += " splitGraphNbhIDs ";
        for (size_t i = 0; i < mSplitGraphNeighbourVertexIDs.size(); i++)
            ret += to_string(mSplitGraphNeighbourVertexIDs[i]) + " ";
    }

    if (mSplitGraphNeighbourVertexWithBoundaryIDs.size() != 0)
    {
        ret += " splitGraphNbhWithBdyIDs ";
        for (size_t i = 0; i < mSplitGraphNeighbourVertexWithBoundaryIDs.size(); i++)
            ret += to_string(mSplitGraphNeighbourVertexWithBoundaryIDs[i]) + " ";
    }

    ret += "\n";
    return ret;
}

shape::shape(string name, size_t currDepth)
{
    mName = name;
    debug(shapeDebugExtra, "shape name = " + mName + "\n");

    mHasSplit = false;
    mHaveFoundAnchors = false;
    mNumCopies = 0;
    mCurrDepth = currDepth;
    
    numBoundaryVertices = 0;
    graphVertices.clear(); // boundary plus internal graph
    graphInternalVertexIndices.clear();
    anchorVertices.clear();
    anchorIndices.clear();
    weights.clear();
    allPairsPaths.clear();
    edges.clear();
    subshapeIDs.clear(); // list of <leftID, rightID> subtiles
    foundBoundaryAnchors.clear();
    mParentID = -1;
    mPictureID = -1;
}

shape::~shape()
{
}

void shape::addVertex(double x, double y, vertexType type)
{
    vertex vx;
    vx.mX = x;
    vx.mY = y;
    vx.mType = type;
    
    graphVertices.push_back(vx);
    debug(shapeDebugExtra, "Added vertexID = " + to_string(graphVertices.size()-1) + " " + vx.print());
}

void shape::addVertex(vertex vx)
{
    vertex newVx;
    
    newVx.mX = vx.mX;
    newVx.mY = vx.mY;
    newVx.mType = vx.mType;
    
    graphVertices.push_back(newVx);
    debug(shapeDebugExtra, "Added vertexID = " + to_string(graphVertices.size()-1) + " " + newVx.print());
}

void shape::addThisID(size_t id)
{
    thisShapeID = id;
}

void shape::changeVertexType(size_t id, vertexType type)
{
    if (id > graphVertices.size())
    {
        cout << "Error: vertex id out of pool!" << endl;
        exit(1);
    }

    graphVertices[id].mType = type;
}

size_t shape::getNumVertices()
{
    return graphVertices.size();
}

size_t shape::getNearestInternalNeighbourFromBoundary(size_t vertexID)
{
    debug(shapeDebugExtra, "Get nearest internal neighbour from boundary vertex " + to_string(vertexID) + " " + graphVertices[vertexID].print());
    
    // need to find nearest internal graph
    if (graphVertices[vertexID].mType == vertexType::Knot ||
        graphVertices[vertexID].mType == vertexType::Vertex ||
        graphVertices[vertexID].mType == vertexType::AnchorKnot)
    {
        // can pick any of the neighbours
        return graphVertices[vertexID].mSplitGraphNeighbourVertexWithBoundaryIDs[0];
    } else if (graphVertices[vertexID].mType == vertexType::Anchor)
    {
        // Need to move along boundary until the vertex is one of the types that has a neighbour into the graph
        // Do this in both directions
        // Then find a common vertex - that's the nearest neighbour

        if (foundBoundaryAnchors[vertexID])
        {
            debug(shapeDebugExtra, "Nearest internal neighbour of " + to_string(vertexID) + " is " + to_string(graphVertices[vertexID].mSplitGraphNeighbourVertexWithBoundaryIDs[0]) + "\n");
            return graphVertices[vertexID].mSplitGraphNeighbourVertexWithBoundaryIDs[0];
        }
        foundBoundaryAnchors[vertexID] = true;
        
        size_t rightNeighbour = (vertexID - 1) % numBoundaryVertices;
        vertex rightNeighbourVertex = graphVertices[rightNeighbour];
        while (! (rightNeighbourVertex.mType == vertexType::Knot ||
                  rightNeighbourVertex.mType == vertexType::Vertex ||
                  rightNeighbourVertex.mType == vertexType::AnchorKnot))
        {
            rightNeighbour = (rightNeighbour - 1) % numBoundaryVertices;
            rightNeighbourVertex = graphVertices[rightNeighbour];
        }

        size_t leftNeighbour = (vertexID + 1) % numBoundaryVertices;
        vertex leftNeighbourVertex = graphVertices[leftNeighbour];
        while (! (leftNeighbourVertex.mType == vertexType::Knot ||
                  leftNeighbourVertex.mType == vertexType::Vertex ||
                  leftNeighbourVertex.mType == vertexType::AnchorKnot))
        {
            leftNeighbour = (leftNeighbour + 1) % numBoundaryVertices;
            leftNeighbourVertex = graphVertices[leftNeighbour];
        }

        debug(shapeDebugExtra, "Nearest neighbour: " + to_string(vertexID) + " left = " + to_string(leftNeighbour) + " right = " + to_string(rightNeighbour) + "\n");

        for (size_t i = 0; i < leftNeighbourVertex.mSplitGraphNeighbourVertexWithBoundaryIDs.size(); i++)
        {
            size_t possibleleft = leftNeighbourVertex.mSplitGraphNeighbourVertexWithBoundaryIDs[i];
            debug(shapeDebugExtra, "Possible left = " + to_string(possibleleft) + "\n");
            
            for (size_t j = 0; j < rightNeighbourVertex.mSplitGraphNeighbourVertexWithBoundaryIDs.size(); j++)
            {
                debug(shapeDebugExtra, "Possible right = " + to_string(rightNeighbourVertex.mSplitGraphNeighbourVertexWithBoundaryIDs[j]) + "\n");
                size_t rightCheck = rightNeighbourVertex.mSplitGraphNeighbourVertexWithBoundaryIDs[j];
                if (rightCheck == possibleleft)
                {
                    graphVertices[possibleleft].mSplitGraphNeighbourVertexWithBoundaryIDs.push_back(vertexID);
                    graphVertices[vertexID].mSplitGraphNeighbourVertexWithBoundaryIDs.push_back(possibleleft);

                    debug(shapeDebugExtra, "Nearest neighbour: " + to_string(vertexID) + " left = " + to_string(leftNeighbour) + " right = " + to_string(rightNeighbour) + " common = " + to_string(possibleleft) + "\n");
                    
                    return possibleleft;
                }
            }
        }
        cout << "Couldn't find one!!" << endl;
        exit(1);

    } else {
        cout << "This shouldn't happen in the getting nearest internal vertex from boundary" << endl;
        exit(1);
    }

    return -1;
}

void shape::write()
{
    outFile << endl;
    // define the shape for the tikz file
    outFile << "\\tikzset{" << endl;
    outFile << "\t" << mName << "/.pic ={" << endl;
    
    debug(shapeDebugExtra, "Write shape vertex " + mName + "\n");

    // knot path first, then knots, vertices last
    // assuming at least two vertices - will break with only a single vertex
    debug(shapeDebugSimple, "\nWrite border vertices " + printBoundaryVertices());
    debug(shapeDebugExtra, "Write graph vertices " + printGraphVertices());
    debug(shapeDebugExtra, "Write internal graph vertices " + printGraphInternalVertices());
    debug(shapeDebugExtra, "Show split graphs = " + boolToStr(isShowSplitGraphs) + "\n");
    debug(shapeDebugExtra, "Show split paths = " + boolToStr(isShowSplitPaths) + "\n");
    debug(shapeDebugExtra, "Show internal graph and boundaries = " + boolToStr(showInternalGraphAndBoundaries) + "\n");
    debug(shapeDebugExtra, "Show internal graph only = " + boolToStr(showInternalGraphOnly) + "\n");
    debug(shapeDebugExtra, "Show vertex indices = " + boolToStr(showVertexIndices) + "\n");
    debug(shapeDebugExtra, "Is boundary only = " + boolToStr(isBoundaryOnly) + "\n\n");

    // external edges for shape
    for (size_t i = 0; i < edges.size(); i++)
    {
        outFile << setprecision(10) << "\t\t\\draw (\\reg*" << graphVertices[edges[i].first].mX << ", \\reg*" << graphVertices[edges[i].first].mY <<") -- (\\reg*" << graphVertices[edges[i].second].mX << ", \\reg*" << graphVertices[edges[i].second].mY <<");" << endl;
    }

    if (!isBoundaryOnly and isShowSplitGraphs)
    {
        // graph edges for split
        for (size_t i = 0; i < graphVertices.size(); i++)
        {
            if (showInternalGraphAndBoundaries)
            {
                debug(shapeDebugExtra, "Write split graphs with boundary: index " + to_string(i) + "\n");

                for (size_t j = 0; j < graphVertices[i].mSplitGraphNeighbourVertexWithBoundaryIDs.size(); j++)
                {
                    vertex vstart = graphVertices[i];
                    vertex vend = graphVertices[graphVertices[i].mSplitGraphNeighbourVertexWithBoundaryIDs[j]];
                    debug(shapeDebugExtra, "vertices " + to_string(i) + " to " + to_string(graphVertices[i].mSplitGraphNeighbourVertexWithBoundaryIDs[j]) + "\n");
                    outFile << setprecision(10) << "\t\t\\draw[color=colorGraphEdge] (\\reg*" << vstart.mX << ", \\reg*" << vstart.mY <<") -- (\\reg*" << vend.mX << ", \\reg*" << vend.mY <<");" << endl;
                }
            }
        
            if (showInternalGraphOnly)
            {
                debug(shapeDebugExtra, "Write split graphs with boundary: index " + to_string(i) + "\n");

                for (size_t j = 0; j < graphVertices[i].mSplitGraphNeighbourVertexIDs.size(); j++)
                {
//                    cout << "    vertex = " << i << " adj: " << j << " = " << vertices[i].mSplitGraphNeighbourVertexIDs[j] << endl;
                    vertex vstart = graphVertices[i];
                    vertex vend = graphVertices[graphVertices[i].mSplitGraphNeighbourVertexIDs[j]];
                    debug(shapeDebugExtra, "vertices " + to_string(i) + " to " + to_string(graphVertices[i].mSplitGraphNeighbourVertexWithBoundaryIDs[j]) + "\n");
                    outFile << setprecision(10) << "\t\t\\draw[color=colorGraphEdge] (\\reg*" << vstart.mX << ", \\reg*" << vstart.mY <<") -- (\\reg*" << vend.mX << ", \\reg*" << vend.mY <<");" << endl;
                }
            }
        }
    }
    
    for (size_t i = 0; i < graphVertices.size(); i++)
    {
        if (graphVertices[i].mType == vertexType::Vertex)
            outFile << setprecision(10) << "\t\t\\filldraw (\\reg*" << graphVertices[i].mX << ", \\reg*" << graphVertices[i].mY <<") circle(\\rad);" << endl;
        else if (graphVertices[i].mType == vertexType::Knot)
            outFile << setprecision(10) << "\t\t\\filldraw[color = colorKnot] (\\reg*" << graphVertices[i].mX << ", \\reg*" << graphVertices[i].mY <<") circle(\\radk);" << endl;
        else if (graphVertices[i].mType == vertexType::Anchor)
            outFile << setprecision(10) << "\t\t\\filldraw[color = colorAnchor] (\\reg*" << graphVertices[i].mX << ", \\reg*" << graphVertices[i].mY <<") circle(\\radk);" << endl;
        else if (graphVertices[i].mType == vertexType::AnchorKnot)
            outFile << setprecision(10) << "\t\t\\filldraw[color = colorAnchorKnot] (\\reg*" << graphVertices[i].mX << ", \\reg*" << graphVertices[i].mY <<") circle(\\radk);" << endl;
        // this is used to debug the vertex indices on top of each dot
        // - it's not pretty but it works
//        outFile << setprecision(10) << "\t\t\\node[color = color1, align=left] at (\\reg*" << graphVertices[i].mX << ", \\reg*" << graphVertices[i].mY <<") {" << i << "};" << endl;
    }

    // graph boundaries to internal vertices
//    for (size_t i = 0; i < vertices.size(); i++)
//    {
//        vertex vstart = graphVertices[i];
//        if (vstart.mType != vertexType::Anchor)
//            continue;
//        vertex vend = graphVertices[gShapepool.getNearestInternalNeighbourFromBoundary(shapepoolID, i)];
//            outFile << setprecision(10) << "\t\t\\draw[color=colorGraphEdge] (\\reg*" << vstart.mPoint.first << ", \\reg*" << vstart.mPoint.second <<") -- (\\reg*" << vend.mPoint.first << ", \\reg*" << vend.mPoint.second <<");" << endl;
//    }

    if (!isBoundaryOnly and isShowSplitGraphs)
    {
        for (size_t i = 0; i < graphVertices.size(); i++)
        {
            if (graphVertices[i].mType == vertexType::Centroid)
            {
                outFile << setprecision(10) << "\t\t\\filldraw[color = colorCentroid] (\\reg*" << graphVertices[i].mX << ", \\reg*" << graphVertices[i].mY <<") circle(\\radk);" << endl;
                
                // uncomment the next line to get the centroid vertices indices
//                outFile << setprecision(10) << "\t\t\\node[color = color1, align=left] at (\\reg*" << graphVertices[i].mPoint.first << ", \\reg*" << graphVertices[i].mPoint.second <<") {" << i << "};" << endl;
            }
        }
    }

    // if an individual vertex needed to be debugged - change the appropriate vertices index
//    outFile << setprecision(10) << "\t\t\\filldraw[color=color4] (\\reg*" << vertices[5].mPoint.first << ", \\reg*" << vertices[5].mPoint.second <<") circle(\\rad);" << endl;

    if (!isBoundaryOnly and isShowSplitPaths)
    {
        size_t numAnchors = anchorIndices.size();
        size_t startAnchor = anchorIndices[rand() % numAnchors];
        size_t endAnchor = anchorIndices[rand() % numAnchors];
        while (endAnchor == startAnchor)
        {
            endAnchor = anchorIndices[rand() % numAnchors];
        }
        vector<size_t> path = allPairsPaths[startAnchor][endAnchor]; // for testing only
        if (path.size() != 0)
        {
            for (size_t i = 0; i < path.size() - 1; i++)
            {
                vertex vstart = graphVertices[path[i]];
                vertex vend = graphVertices[path[i+1]];
                outFile << setprecision(10) << "\t\t\\draw[color=color4] (\\reg*" << vstart.mX << ", \\reg*" << vstart.mY <<") -- (\\reg*" << vend.mX << ", \\reg*" << vend.mY <<");" << endl;
            }
        }
    }
    
    // close up the shape definition
    outFile << "\t}" << endl << "}" << endl;
}

string shape::printGraphVertices()
{
    string ret = "Graph Vertices\n";
    
    for (size_t i = 0; i < graphVertices.size(); i++)
    {
        vertex vx = graphVertices[i];
        ret += "vertex " + to_string(i) + " : " + vx.print();
    }
    return ret;
}

string shape::printGraphInternalVertices()
{
    string ret = "Internal Graph Vertices\n";
    
    for (size_t i = 0; i < graphInternalVertexIndices.size(); i++)
    {
        size_t index = graphInternalVertexIndices[i];
        vertex vx = graphVertices[index];
        ret += "vertex " + to_string(i) + " index " + to_string(index) + " : " + vx.print();
    }
    return ret;
}

string shape::printAnchorVertices()
{
    string ret = "Anchor Vertices\n";
    
    for (size_t i = 0; i < anchorIndices.size(); i++)
    {
        size_t index = anchorIndices[i];
        vertex vx = graphVertices[index];
        ret += "vertex " + to_string(i) + " index " + to_string(index) + " : " + vx.print();
    }
    return ret;
}

string shape::printBoundaryVertices()
{
    string ret = "Boundary Vertices\n";
    
    for (size_t i = 0; i < numBoundaryVertices; i++)
    {
        vertex vx = graphVertices[i];
        ret += "vertex " + to_string(i) + " : " + vx.print();
    }
    return ret;
}

string shape::printGraphWeights()
{
    string ret = "Graph Weights\n";

    for (size_t i = numBoundaryVertices; i < graphInternalVertexIndices.size(); i++)
    {
        vertex vx = graphVertices[i];
        ret += "vertex " + to_string(i) + " : ";
        
        for (size_t j = 0; j < graphInternalVertexIndices.size(); j++)
        {
            size_t weight = weights[i][j];
            if (weight == -1)
                ret += "- ";
            else
                ret += to_string(weight) + " ";
        }
        
        ret += "\n";
    }
    return ret;
}

string shape::printPath(size_t vstart, size_t vend)
{
    if (vstart == vend)
        return "";
    
    string ret = "path from " + to_string(vstart) + " to " + to_string(vend) + " : ";
    
    vector<size_t> path = allPairsPaths[vstart][vend];
    
    for (size_t i = 0; i < path.size(); i++)
    {
        size_t index = path[i];
        ret += to_string(index) + " ";
    }
    
    ret += "\n";
    
    return ret;
}

string shape::printAllPaths()
{
    string ret = "All paths\n";
    
    for (size_t i = 0; i < anchorIndices.size(); i++)
    {
        for (size_t j = 0; j < anchorIndices.size(); j++)
        {
            if (i == j) continue;
            
            ret += printPath(i, j);
        }
    }

    return ret;
}

void shape::addAnchor(double x, double y)
{
    vertex vx;
    vx.mX = x;
    vx.mY = y;
    vx.mType = vertexType::Anchor;
    
    anchorVertices.push_back(vx);
}

shapes::shapes()
{
}

shapes::~shapes()
{
    for (size_t i = 0; i < mShapes.size(); i++)
        delete mShapes[i];
    
    mShapes.clear();
}

size_t shapes::cloneBoundary(size_t id)
{
    shape * ps = mShapes[id]; // now conduct the split on this shape
    
    ps->mNumCopies++;
    // when creating this new shape, we are not showing the splitgraphs or the splitpath
    shape * pShape = new shape(ps->mName + "_" + to_string(ps->mNumCopies), ps->mCurrDepth);
    
    // this is still todo
    // get copy of existing boundary and add it to the new one
    // don't use splits
    
    mShapes.push_back(pShape);
    
    return mShapes.size() - 1;
}

size_t shapes::cloneSplits(size_t id)
{
    shape * ps = mShapes[id]; // now conduct the split on this shape
    
    ps->mNumCopies++;

    // this is the existing shape including any splits or paths or subshapes
    shape * pShape = new shape(ps->mName + "_" + to_string(ps->mNumCopies), ps->mCurrDepth);
    
    // this is still todo
    // get copy of existing boundary and add it to the new one
    // use existing splits and paths if already part of the existing one

    mShapes.push_back(pShape);
    
    return mShapes.size() - 1;
}

size_t shapes::addChildShape(size_t id, string & name)
{
    shape * ps = mShapes[id]; // now conduct the split on this shape
    
    ps->mNumCopies++;
    debug(shapeDebugSimple, "Start Add child shape from id " + to_string(id) + "\n");

    // this is the existing shape including any splits or paths or subshapes
    name = ps->mName + "_"  + to_string(ps->mNumCopies);
    shape * pShape = new shape(name, ps->mCurrDepth + 1);
    debug(shapeDebugSimple, "Created child shape from id " + to_string(id) + "\n");

    mShapes.push_back(pShape);
    size_t newShapeID = mShapes.size() - 1;
    pShape->mParentID = id;
    gPictures->addShape(ps->mPictureID, newShapeID);
    pShape->mPictureID = ps->mPictureID;

    debug(shapeDebugSimple, "New child shape from id " + to_string(id) + " newShapeID = " + to_string(newShapeID) +  " parentID = " + to_string(pShape->mParentID) + " pictureID = " + to_string(pShape->mPictureID) + "\n");

    for (size_t i = 0; i < ps->mActions.size(); i++)
    {
        debug(shapeDebugSimple, "Adding action from id " + to_string(id) + " i = " + to_string(i) + "\n");
        size_t newActionID = gActions->getNewIDFromParent(id, newShapeID, i);
        pShape->mActions.push_back(newActionID);
        gPictures->addAction(ps->mPictureID, newActionID);
        debug(shapeDebugSimple, "New child shape from id " + to_string(id) + " added new action i = " + to_string(i) +  " actionID = " + to_string(newActionID) + "\n");
    }
    debug(shapeDebugSimple, "Add child shape from id " + to_string(id) + " new name = " + pShape->mName + " depth = " + to_string(pShape->mCurrDepth) + "\n");
    
    return newShapeID;
}

size_t shapes::getNewID(string name, size_t currDepth)
{
//    cout << "shapes getnewid name = " << name << endl;
    shape *pShape = new shape(name, currDepth);
    mShapes.push_back(pShape);
    return mShapes.size()-1;
}

void shapes::addName(size_t id, string name)
{
    if (id > mShapes.size())
    {
        cout << "Error: shape id out of pool!" << endl;
        exit(1);
    }
    mShapes[id]->mName = name;
}

const string shapes::getName(size_t id)
{
    if (id > mShapes.size())
    {
        cout << "Error: shape id out of pool!" << endl;
        exit(1);
    }
    return mShapes[id]->mName;
}

void shapes::addAction(size_t shapeID, size_t actionID)
{
    if (shapeID > mShapes.size())
    {
        cout << "Error: shape id out of pool!" << endl;
        exit(1);
    }
    mShapes[shapeID]->mActions.push_back(actionID);
}

void shapes::addPicture(size_t shapeID, size_t pictureID)
{
    if (shapeID > mShapes.size())
    {
        cout << "Error: shape id out of pool!" << endl;
        exit(1);
    }
    mShapes[shapeID]->mPictureID = pictureID;
}

void shapes::addVertex(size_t id, double x, double y, vertexType type)
{
    if (id > mShapes.size())
    {
        cout << "Error: shape id out of pool!" << endl;
        exit(1);
    }
    mShapes[id]->addVertex(x, y, type);
}

void shapes::addVertex(size_t id, vertex vx)
{
    if (id > mShapes.size())
    {
        cout << "Error: shape id out of pool!" << endl;
        exit(1);
    }
    mShapes[id]->addVertex(vx);
}

void shapes::write(size_t id)
{
    if (id > mShapes.size())
    {
        cout << "Error: shape id out of pool!" << endl;
        exit(1);
    }
    mShapes[id]->write();
}

void shapes::split(size_t id)
{
    if (isBoundaryOnly) // not going to perform splits
        return;
        
    if (id > mShapes.size())
    {
        cout << "Error: shape id out of pool!" << endl;
        exit(1);
    }
    shape * ps = mShapes[id]; // now conduct the split on this shape
    
    debug(shapeDebugSimple, "Split for id " + to_string(id) + " parent = " + to_string(ps->mParentID) + " picture = " + to_string(ps->mPictureID) + " actions = ");
    for (size_t i = 0; i < ps->mActions.size(); i++)
    {
        debug(shapeDebugSimple, to_string(ps->mActions[i]) + " ");
    }
    debug(shapeDebugSimple, "\n");

    debug(shapeDebugExtra, "Split for id " + to_string(id) + " currDepth = " + to_string(getCurrentDepth(id)) + "\n");

    if (getCurrentDepth(id) >= maxdepth)
    {
        debug(shapeDebugExtra, "Reached maximum depth for shape " + to_string(id) +"\n");
        return;
    }
        
    // can only split once
    if (ps->mHasSplit)
        return;
    ps->mHasSplit = true;

    debug(shapeDebugExtra, "Starting split vertices: \n" + ps->printGraphVertices());
        
    // Process to split a tile
    // 1. Add anchors on boundaries using knots and vertices
    // 1.a. Any knots that are left can't form paths - they are ignored
    // 1.b. But those knots do need to be kept in the boundary for the earcut
    // 2. Triangulate shapepool using existing vertices and knots only
    // 2.a. Ignore any anchors that weren't already knots
    // 3. Create centroids and edge centroids on resulting triangulation
    // 4. Create internal graph using only these centroids
    // 4.a. But we need to know where all knots and vertices connect into the internal graph
    // 4.b. Each centroid includes lists of possible boundary locations
    // 5. Perform n Dijkstra calls on all internal graph vertices
    // 6. Create all pairs paths from anchors and vertices to all anchors and vertices
    // 7. Saving the split in an atlas and recurse as needed

    // ***********************************************************************************
    // 1. Add anchors on boundaries using knots and vertices
    // 1.a. Any knots that are left can't form paths - they are ignored
    // 1.b. But those knots do need to be kept in the boundary for the earcut
    // ***********************************************************************************

    // graphVertices is all that's populated
    // it's only composed of the boundary so far
    
    populateAnchors(id);
        
    // ***********************************************************************************
    // 2. Triangulate shapepool using existing vertices and knots only
    // 2.a. Ignore any anchors that weren't already knots
    // 3. Create centroids and edge centroids on resulting triangulation
    // ***********************************************************************************

    triangulateBorderAndCreateCentroids(id);

    // ***********************************************************************************
    // 4. Create internal graph using only these centroids
    // 4.a. But we need to know where all knots and vertices connect into the internal graph
    // 4.b. Each centroid includes lists of possible boundary locations
    // ***********************************************************************************

    createInternalGraph(id);

    // ***********************************************************************************
    // 5. Perform n Dijkstra calls on all internal graph vertices
    // ***********************************************************************************
    // ***********************************************************************************
    // 6. Create all pairs paths from anchors and vertices to all anchors and vertices
    // ***********************************************************************************

    createAllPairsPaths(id);
        
    // ***********************************************************************************
    // 7. Saving the split in an atlas and recurse as needed
    // ***********************************************************************************

    // form Catalan-style recursion for topology - need to save all split shapepools on each side of path
    // create "atlas" - equivalent to dynamic programming
    
    createChildShapes(id);
    
    debug(shapeDebugSimple, "Completed paths and new shapes for shape id " + to_string(id) + "\n\n\n");
}

void shapes::populateAnchors(size_t id)
{
    shape * ps = mShapes[id];
    
    if (ps->mHaveFoundAnchors)
    {
        cout << "Cannot find anchors if anchors have already been found!" << endl;
        exit(1);
    }
    
    debug(shapeDebugExtra, "Start populate anchors for shape id " + to_string(id) + "\n");

    ps->mHaveFoundAnchors = true;
     
    // the boundaries are contiguous and clockwise order
    size_t numBoundaryVertices = ps->graphVertices.size();
    // each vertex neighbour needs to be cleared
    for (size_t i = 0; i < numBoundaryVertices; i++)
    {
        ps->graphVertices[i].mNeighbourVertexIDs.clear();
        ps->graphVertices[i].mSplitGraphNeighbourVertexIDs.clear();
        ps->graphVertices[i].mSplitGraphNeighbourVertexWithBoundaryIDs.clear();
    }
    vertex vxInitial = ps->graphVertices[0]; // to assist with renumbering vertices later
    
    // Find first and last vertices
    size_t first = 0;
    size_t last = numBoundaryVertices - 1;
    
    // Insert three anchors between each vertex - can convert knots if necessary
    while(ps->graphVertices[first].mType != vertexType::Vertex)
        first++;
    debug(shapeDebugDetailed, "Populate Anchors shape id = " + to_string(id) + " first = " + to_string(first) + "\n");
    
    while(ps->graphVertices[last].mType != vertexType::Vertex)
        last--;
    debug(shapeDebugDetailed, "Populate Anchors shape id = " + to_string(id) + " last = " + to_string(last) + "\n");

    debug(shapeDebugDetailed, "Before boundary modifications, shape id = " + to_string(id) + "\n");
    debug(shapeDebugDetailed, ps->printGraphVertices());

    addBoundaryAnchors(id, last, first); // must be clockwise
    
    // Loop across all vertices
    // The types are Vertex, Knot, Anchor, Centroid
    // If Vertex already, then keep it
    // Can have a string of Knots between Vertices
    // Take the first ones and call them AnchorKnots
    // Between any two Vertices needs to be three Anchors
    // If no Knots, then create new Anchors between the Vertices
    for (size_t i = first; i < last; i++)
    {
        if (ps->graphVertices[i].mType == vertexType::Vertex)
        {
            ps->anchorVertices.push_back(ps->graphVertices[i]);
            // find next vertex
            size_t next = i + 1;
            while (ps->graphVertices[next].mType != vertexType::Vertex)
                next++;
            addBoundaryAnchors(id, i, next);
            i = next - 1;
        }
    }
    // graphVertices[last] is the last Vertex type on the boundary
    ps->anchorVertices.push_back(ps->graphVertices[last]);
    
    // re-number the list so it's back to the original locations
    // this helps when we are taking the vertex lists with tiles and combining the splits again
    size_t currVxIndex = 0;
    for (size_t i = 0; i < ps->anchorVertices.size(); i++)
    {
        vertex current = ps->anchorVertices[i];
        if (current.mX == vxInitial.mX && current.mY == vxInitial.mY)
        {
            currVxIndex = i;
            break;
        }
    }
    
    debug(shapeDebugDetailed, "After anchors added shape id = " + to_string(id) + "\n");
    debug(shapeDebugDetailed, ps->printGraphVertices());
    debug(shapeDebugDetailed, ps->printAnchorVertices());

    // After all Anchors have been created, replace the mVertices with the Anchors for the appropriate splits
    // Renumber all vertices in graph so the boundary vertices are contiguous and clockwise
    ps->graphVertices.clear();
    ps->edges.clear();
    size_t numAnchors = ps->anchorVertices.size();
    for (size_t i = 0; i < numAnchors; i++)
    {
        ps->graphVertices.push_back(ps->anchorVertices[currVxIndex]);
        currVxIndex = (currVxIndex + 1) % numAnchors;
    }
    
    debug(shapeDebugDetailed, "After anchors redoing graph vertices shape id = " + to_string(id) + "\n");
    debug(shapeDebugDetailed, ps->printGraphVertices());

    addBoundaryEdges(id);

    // now add the anchor indices back to the anchors vector so it can be used in the graph

    for (size_t i = 0; i < ps->graphVertices.size(); i++)
    {
        if (ps->graphVertices[i].mType == vertexType::Vertex
            || ps->graphVertices[i].mType == vertexType::Anchor
            || ps->graphVertices[i].mType == vertexType::AnchorKnot)
        {
            ps->anchorIndices.push_back(i);
        }
    }
    
    // used for saving time lookup requirement later
    for (size_t i = 0; i < ps->graphVertices.size(); i++)
    {
        ps->foundBoundaryAnchors.push_back(false);
    }
    
    ps->numBoundaryVertices = ps->graphVertices.size();
    
    debug(shapeDebugDetailed, "After populate anchors information for shape id = " + to_string(id) + "\n");
    debug(shapeDebugDetailed, ps->printGraphVertices());
    debug(shapeDebugDetailed, ps->printAnchorVertices());

    debug(shapeDebugExtra, "Completed populate anchors for shape id " + to_string(id) + "\n");
}

void shapes::addBoundaryAnchors(size_t id, size_t leftID, size_t rightID)
{
    shape * ps = mShapes[id];

    size_t numExistingKnots = 0;
    if (leftID > rightID) // crosses over the origin vertex
    {
        size_t maxID = ps->getNumVertices() - 1; // still no internal graph - only boundary
        numExistingKnots = maxID - leftID + rightID;
        debug(shapeDebugDetailed, "Shape id " + to_string(id) + " cross over existing knots " + to_string(numExistingKnots) + "\n");
    }
    else
    {
        numExistingKnots = rightID - leftID - 1;
        debug(shapeDebugDetailed, "Shape id " + to_string(id) + " not cross over existing knots " + to_string(numExistingKnots) + "\n");
    }
    size_t currIndex = leftID % ps->getNumVertices();
    
//    // this commented out section is for one anchor between vertices instead of three
//    if (numExistingKnots >= 1) // no knots is dealt with in the else clause
//    {
//        cout << "left = " << leftID << " right = " << rightID << " currIndex = " << currIndex << " numVertices = " << getNumVertices() << endl;
//        currIndex = (currIndex + 1) % si[id]->getNumVertices();
//        vertex cv = si[id]->vertices[currIndex];
//        cv.mType = vertexType::Anchor;
//        si[id]->anchors.push_back(cv);
//        cout << "currIndex = " << currIndex << " push anchor " << endl;
//
//        size_t remainingKnots = numExistingKnots - 1;
//
//        // no more anchors needed
//        // remaining knots are just added to the anchor list as-is
//        for (size_t i = 0; i < remainingKnots; i++)
//        {
//            currIndex = (currIndex + 1) % si[id]->getNumVertices();
//            cout << "i = " << i << " currIndex = " << currIndex << endl;
//            vertex vx = si[id]->vertices[currIndex];
//            si[id]->anchors.push_back(vx);
//        }
//    } else { // no existing knots
//        double startx = si[id]->vertices[leftID].mPoint.first;
//        double starty = si[id]->vertices[leftID].mPoint.second;
//        double deltax = (si[id]->vertices[rightID].mPoint.first - si[id]->vertices[leftID].mPoint.first) /2.;
//        double deltay = (si[id]->vertices[rightID].mPoint.second - si[id]->vertices[leftID].mPoint.second) /2.;
//
//        ps->addAnchor(startx + deltax, starty + deltay);
//    }

    // this is all really icky and should be cleaned up at some point
    if (numExistingKnots == 1)
    {
        currIndex = (currIndex + 1) % ps->getNumVertices();
        vertex cv = ps->graphVertices[currIndex];
        cv.mType = vertexType::AnchorKnot;
        ps->anchorVertices.push_back(cv);
        
        // two other anchors need to be added
        double startx = cv.mX;
        double starty = cv.mY;
        double deltax = (ps->graphVertices[rightID].mX - cv.mX) /3.;
        double deltay = (ps->graphVertices[rightID].mY - cv.mY) /3.;
        
        ps->addAnchor(startx + deltax, starty + deltay);
        ps->addAnchor(startx + 2*deltax, starty + 2*deltay);
        
    } else if (numExistingKnots == 2)
    {
        currIndex = (currIndex + 1) % ps->getNumVertices();
        vertex cv = ps->graphVertices[currIndex];
        cv.mType = vertexType::AnchorKnot;
        ps->anchorVertices.push_back(cv);

        currIndex = (currIndex + 1) % ps->getNumVertices();
        cv = ps->graphVertices[currIndex];
        cv.mType = vertexType::AnchorKnot;
        ps->anchorVertices.push_back(cv);
        
        // one other anchor needs to be added
        double startx = cv.mX;
        double starty = cv.mY;
        double deltax = (ps->graphVertices[rightID].mX - cv.mX) /2.;
        double deltay = (ps->graphVertices[rightID].mY - cv.mY) /2.;
        
        ps->addAnchor(startx + deltax, starty + deltay);
        
    } else if (numExistingKnots >= 3)
    {
        debug(shapeDebugDetailed, "Shape id " + to_string(id) + " add boundary anchors: left " + to_string(leftID) + " right " + to_string(rightID) + " currIndex "  + to_string(currIndex) + "\n");

        currIndex = (currIndex + 1) % ps->getNumVertices();
        vertex cv = ps->graphVertices[currIndex];
        cv.mType = vertexType::AnchorKnot;
        ps->anchorVertices.push_back(cv);
        debug(shapeDebugDetailed, "Shape id " + to_string(id) + " add boundary anchors: push anchor, currIndex " + to_string(currIndex) + "\n");

        currIndex = (currIndex + 1) % ps->getNumVertices();
        cv = ps->graphVertices[currIndex];
        cv.mType = vertexType::AnchorKnot;
        ps->anchorVertices.push_back(cv);
        debug(shapeDebugDetailed, "Shape id " + to_string(id) + " add boundary anchors: push anchor, currIndex " + to_string(currIndex) + "\n");

        currIndex = (currIndex + 1) % ps->getNumVertices();
        cv = ps->graphVertices[currIndex];
        cv.mType = vertexType::AnchorKnot;
        ps->anchorVertices.push_back(cv);
        debug(shapeDebugDetailed, "Shape id " + to_string(id) + " add boundary anchors: push anchor, currIndex " + to_string(currIndex) + "\n");

        // no more anchors needed
        // remaining knots are just added to the anchor list as-is
        size_t remainingKnots = numExistingKnots - 3;
    
        // no more anchors needed
        // remaining knots are just added to the anchor list as-is
        for (size_t i = 0; i < remainingKnots; i++)
        {
            currIndex = (currIndex + 1) % ps->getNumVertices();
            debug(shapeDebugDetailed, "Shape id " + to_string(id) + " i " + to_string(i) + " currIndex " + to_string(currIndex) + "\n");
            vertex vx = ps->graphVertices[currIndex];
            ps->anchorVertices.push_back(vx);
        }

    } else { // no existing knots
        double startx = ps->graphVertices[leftID].mX;
        double starty = ps->graphVertices[leftID].mY;
        double deltax = (ps->graphVertices[rightID].mX - ps->graphVertices[leftID].mX) /4.;
        double deltay = (ps->graphVertices[rightID].mY - ps->graphVertices[leftID].mY) /4.;
        
        ps->addAnchor(startx + deltax, starty + deltay);
        ps->addAnchor(startx + 2*deltax, starty + 2*deltay);
        ps->addAnchor(startx + 3*deltax, starty + 3*deltay);
    }
}

void shapes::triangulateBorderAndCreateCentroids(size_t id)
{
    shape * ps = mShapes[id];

    debug(shapeDebugExtra, "Start triangulation for shape id " + to_string(id) + "\n");

    // act on a copy of the boundary vertices
    // print out all of the vertex information known before splitting
    debug(shapeDebugExtra, "About to triangulate all vertices for shape " + to_string(id) + "\n");
    debug(shapeDebugDetailed, ps->printGraphVertices());
    debug(shapeDebugDetailed, ps->printBoundaryVertices());
        
    // Using /mapbox/earcut.hpp from https://github.com/mapbox/earcut.hpp
    // Uses the ISC license 2015
    //
    // ISC License
    //
    //    Copyright (c) 2015, Mapbox
    //
    //    Permission to use, copy, modify, and/or distribute this software for any purpose
    //    with or without fee is hereby granted, provided that the above copyright notice
    //    and this permission notice appear in all copies.
    //
    //    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
    //    REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
    //    FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
    //    INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
    //    OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
    //    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
    //    THIS SOFTWARE.

    // From here to the earcut line are directly from the sample/usage code on the earcut
    // website. Populate the vertex list for the earcut, then extract into the indices vector
    using Point = array<double, 2>;
    vector<vector<Point>> polygon;

    // The first polyline defines the main polygon.
    // The polygon can have holes by populating the second element of the polygon vector
    vector<Point> vertexList;
    size_t numBoundaryVertices = ps->graphVertices.size();
    for (size_t i = 0; i < numBoundaryVertices; i++)
    {
        // AnchorKnots we still want as part of the earcut
        // But ignore Anchors
        if (ps->graphVertices[i].mType == vertexType::Anchor)
            continue;

        double currVertexX = ps->graphVertices[i].mX;
        double currVertexY = ps->graphVertices[i].mY;

        Point pt;
        pt[0] = currVertexX;
        pt[1] = currVertexY;
        vertexList.push_back(pt);
        ps->internalGraphIndicesFromEarcutVertices.push_back(i);
        
        debug(shapeDebugDetailed, "VertexList for earcut i = " + to_string(i) + " x = " + to_string(pt[0]) + " y = " + to_string(pt[1]) + "\n");
    }
    polygon.push_back(vertexList);

    // Returns array of indices that refer to the vertices of the input polygon.
    // Three subsequent indices form a triangle. Output triangles are clockwise.
    // The indices vector points to the index in vertexList, which isn't the same index in graphVertices
    
    ps->triangulationIndices.clear();
    ps->triangulationIndices = mapbox::earcut<size_t>(polygon);
    
    // shape has now been triangulated - done with mapbox earcut code
    debug(shapeDebugExtra, "Completed triangulation for shape id " + to_string(id) + "\n");
    debug(shapeDebugExtra, "Start to create centroids for shape id " + to_string(id) + "\n");

    ps->triangleList.clear(); // include a spot for the centroid vertex

    ps->initNumVertices = ps->graphVertices.size();
    ps->numPossibleEdges = ps->initNumVertices * ps->initNumVertices + ps->initNumVertices + 1;

    debug(shapeDebugExtra, "numPossibleEdges =  " + to_string(ps->numPossibleEdges) + " initNumVertices = " + to_string(ps->initNumVertices) + "\n");

    ps->edgeIntersections.clear();

    for (size_t i = 0; i < ps->numPossibleEdges; i++)
    {
        ps->edgeIntersections.push_back(0);
    }

    // create all the centroids of the triangulation
    for (size_t i = 0; i < ps->triangulationIndices.size() / 3; i++)
    {
        size_t first = ps->internalGraphIndicesFromEarcutVertices[ps->triangulationIndices[3*i]];
        size_t second = ps->internalGraphIndicesFromEarcutVertices[ps->triangulationIndices[3*i + 1]];
        size_t third = ps->internalGraphIndicesFromEarcutVertices[ps->triangulationIndices[3*i + 2]];
        debug(shapeDebugDetailed, "Centroids: first = " + to_string(first) + " second = " + to_string(second) + " third = " + to_string(third) +"\n");
        
        if (first < second)
            ps->edgeIntersections[first * ps->initNumVertices + second]++;
        else
            ps->edgeIntersections[second * ps->initNumVertices + first]++;

        if (first < third)
            ps->edgeIntersections[first * ps->initNumVertices + third]++;
        else
            ps->edgeIntersections[third * ps->initNumVertices + first]++;

        if (second < third)
            ps->edgeIntersections[second * ps->initNumVertices + third]++;
        else
            ps->edgeIntersections[third * ps->initNumVertices + second]++;

        addCentroidVertex(id, first, second, third); // adds centroid to internal graph
        ps->triangleList.push_back({first, second, third, ps->graphVertices.size() - 1});
        debug(shapeDebugDetailed, "Centroids first = " + to_string(first) + " second = " + to_string(second) + " third = " + to_string(third) + " centroid = " + to_string(ps->graphVertices.size()-1) + "\n");
    }
    
    debug(shapeDebugExtra, "Completed creating centroids for shape id " + to_string(id) + "\n");
}

// this function adds a centroid of each triangle in the triangulation to the internal graph
void shapes::addCentroidVertex(size_t id, size_t leftID, size_t rightID, size_t newID)
{
    shape * ps = mShapes[id];

    double x = (ps->graphVertices[leftID].mX + ps->graphVertices[rightID].mX + ps->graphVertices[newID].mX) /3.;
    double y = (ps->graphVertices[leftID].mY + ps->graphVertices[rightID].mY + ps->graphVertices[newID].mY) /3.;
    ps->addVertex(x, y, vertexType::Centroid);
    size_t createdVertex = ps->graphVertices.size() - 1;
    addSplitEdges(id, createdVertex, leftID);
    addSplitEdges(id, createdVertex, rightID);
    addSplitEdges(id, createdVertex, newID);
}

// this adds edges between any graph edges
// there are two different graphs
// - SplitGraphNeighbourVertexWithBoundary
//      A duplicate of the other one but includes the boundary connections to Vertices, Knots, and Anchor Knots
//      Anchors are never included
// - SplitGraphNeighbourVertex
//      Just the internal graph generated by centroids of triangles and midpoints of common triangulation triangles
void shapes::addSplitEdges(size_t id, size_t leftID, size_t rightID)
{
    shape * ps = mShapes[id];

    // add vertices to graph with boundaries
    // we don't do path finding on this - it's added afterwards
    
    // check on left - if not, then add
    vector<size_t>::iterator it = find(ps->graphVertices[leftID].mSplitGraphNeighbourVertexWithBoundaryIDs.begin(), ps->graphVertices[leftID].mSplitGraphNeighbourVertexWithBoundaryIDs.end(), rightID);
    if (it == ps->graphVertices[leftID].mSplitGraphNeighbourVertexWithBoundaryIDs.end()) // not added yet
    {
        debug(shapeDebugDetailed, "Shape id " + to_string(id) + " shape boundary add split edges: " + to_string(leftID) + " - " + to_string(rightID) + " " +  "\n");
        ps->graphVertices[leftID].mSplitGraphNeighbourVertexWithBoundaryIDs.push_back(rightID);
        debug(shapeDebugDetailed, "Vertices: " + to_string(leftID) + " - " + ps->graphVertices[leftID].print());
        ps->graphVertices[rightID].mSplitGraphNeighbourVertexWithBoundaryIDs.push_back(leftID);
        debug(shapeDebugDetailed, "Vertices: " + to_string(rightID) + " - " + ps->graphVertices[rightID].print());
    }

    // now add to the internal graph - this is what's used for path finding
    // confirm neither id is on the boundary
    vertexType leftType = ps->graphVertices[leftID].mType;
    vertexType rightType = ps->graphVertices[rightID].mType;

    if (leftType == vertexType::Vertex ||
        leftType == vertexType::Knot ||
        leftType == vertexType::Anchor ||
        leftType == vertexType::AnchorKnot ||
        rightType == vertexType::Vertex ||
        rightType == vertexType::Knot ||
        rightType == vertexType::Anchor ||
        rightType == vertexType::AnchorKnot)
    {
        return;
    }
    
    // check on left - if not, then add
    it = find(ps->graphVertices[leftID].mSplitGraphNeighbourVertexIDs.begin(), ps->graphVertices[leftID].mSplitGraphNeighbourVertexIDs.end(), rightID);
    if (it == ps->graphVertices[leftID].mSplitGraphNeighbourVertexIDs.end()) // not added yet
    {
        debug(shapeDebugDetailed, "Add split edges: " + to_string(leftID) + " - " + to_string(rightID) + "\n");
        ps->graphVertices[leftID].mSplitGraphNeighbourVertexIDs.push_back(rightID);
        debug(shapeDebugDetailed, "LeftID: " + to_string(leftID) + ps->graphVertices[leftID].print());
        ps->graphVertices[rightID].mSplitGraphNeighbourVertexIDs.push_back(leftID);
        debug(shapeDebugDetailed, "RightID: " + to_string(rightID) + ps->graphVertices[rightID].print());
    }
}

void shapes::createInternalGraph(size_t id)
{
    shape * ps = mShapes[id];

    debug(shapeDebugExtra, "Started to create internal graphs for shape id " + to_string(id) + "\n");

    // for each matching triangle edge, create another vertex in that edge
    // and join the centroids to that new vertex
    for (size_t i = 0; i < ps->numPossibleEdges; i++)
    {
        if (ps->edgeIntersections[i] == 2) // we have a matching edge
        {
            size_t first = i / ps->initNumVertices;
            size_t second = i % ps->initNumVertices;
            debug(shapeDebugExtra, "Shape id " + to_string(id) + " create internal graph first = " + to_string(first) + " second = " + to_string(second) + "\n");
            
            // need to find the corresponding centroids
            vector<size_t> centroids;
            for (size_t i = 0; i < ps->triangleList.size(); i++)
            {
                array<size_t,4> vertices = ps->triangleList[i];
                if ( (vertices[0] == first || vertices[1] == first || vertices[2] == first) && (vertices[0] == second || vertices[1] == second || vertices[2] == second) ) // this is one of the triangles
                    centroids.push_back(vertices[3]);
            }
            debug(shapeDebugDetailed, "Shape id " + to_string(id) + " create internal graph centroids[0] = " + to_string(centroids[0]) + " centroids[1] = " + to_string(centroids[1]) + "\n");
            addEdgeVertex(id, first, second, centroids[0], centroids[1]);
        }
    }
        
    size_t numBoundary = ps->numBoundaryVertices;
    size_t numInternal = ps->graphVertices.size() - numBoundary;
    
    for (size_t i = 0; i < ps->graphVertices.size(); i++)
    {
        vector<vector<size_t>> row;
        for (size_t j = 0; j < ps->graphVertices.size(); j++)
        {
            vector<size_t> path;
            row.push_back(path);
        }
        ps->allPairsPaths.push_back(row);
    }
    
    for (size_t i = 0; i < numInternal; i++)
    {
        vector<size_t> row;
        for (size_t j = 0; j < numInternal; j++)
        {
            row.push_back(-1);
        }
        ps->weights.push_back(row);
    }

    // setting up the weights for the graph
    for (size_t i = 0; i < ps->graphVertices.size(); i++)
    {
        vertex vx = ps->graphVertices[i];
        debug(shapeDebugDetailed, "vertex " + to_string(i) + " " + vx.print());

        for (size_t j = 0; j < vx.mSplitGraphNeighbourVertexIDs.size(); j++)
        {
            ps->weights[i - numBoundary][vx.mSplitGraphNeighbourVertexIDs[j] - numBoundary] = 1;
        }
    }
    debug(shapeDebugDetailed, ps->printGraphVertices());

    // creating the small internal graph
    for (size_t i = 0; i < numInternal; i++)
    {
        vertex vx = ps->graphVertices[i+numBoundary];
//        ps->graphInternalVertices.push_back(vx);
        ps->graphInternalVertexIndices.push_back(i+numBoundary);
    }

    debug(shapeDebugDetailed, ps->printGraphInternalVertices());

    // before adding anchor neighbours
    for (size_t i = 0; i < ps->numBoundaryVertices; i++)
    {
        vertex vx = ps->graphVertices[i];
        size_t j = ps->getNearestInternalNeighbourFromBoundary(i);
        UNUSED(j); // this variable is not used - the nearest neighbour function needs cleanup
        debug(shapeDebugDetailed, "Nearest neighbour " + to_string(i) + " is " + to_string(j) + "\n");
    }
    
    // print out the entire set of vertices and neighbours
    debug(shapeDebugExtra, "Right before searching shape id " + to_string(id) + "\n");
    debug(shapeDebugExtra, ps->printBoundaryVertices());
    debug(shapeDebugExtra, ps->printGraphVertices());
    debug(shapeDebugExtra, ps->printGraphInternalVertices());

    debug(shapeDebugExtra, "Completed creating internal graphs for shape id " + to_string(id) + "\n");
}

void shapes::createAllPairsPaths(size_t id)
{
    shape * ps = mShapes[id];

    debug(shapeDebugExtra, "Started to create all pairs paths for shape id " + to_string(id) + "\n");

    // use Dijkstra = O(V(V+V^2)logV) from each point to all other points
    // Section 22.3 Introduction to Algorithms 4e

    // Create the all-pairs paths through the search function

    debug(shapeDebugExtra, "AnchorIndices shape id " + to_string(id) + "\n" + ps->printAnchorVertices());
    
    for (size_t start = 0; start < ps->anchorIndices.size(); start++)
    {
        searchFromVertex(id, ps->anchorIndices[start]);
    }
    
    debug(shapeDebugExtra, "Completed creating all pairs paths for shape id " + to_string(id) + "\n");
}

void shapes::searchFromVertex(size_t id, size_t startVertex)
{
    shape * ps = mShapes[id];

    debug(shapeDebugExtra, "Started to search for shape id " + to_string(id) + " from " + to_string(startVertex) + "\n");

    size_t numVertices = ps->graphInternalVertexIndices.size();
    size_t numBoundary = ps->numBoundaryVertices;

    size_t startInternalVertex = ps->getNearestInternalNeighbourFromBoundary(startVertex);
    if (startInternalVertex == -1)
    {
        startInternalVertex = ps->graphVertices[startVertex].mSplitGraphNeighbourVertexWithBoundaryIDs[0];
    }
    startInternalVertex -= numBoundary;
    debug(shapeDebugExtra, "Shape id " + to_string(id) + " using internal vertex " + to_string(startInternalVertex) + "\n");

    size_t NO_PARENT = -1;
    
    // adapted from https://www.geeksforgeeks.org/dsa/printing-paths-dijkstras-shortest-path-algorithm/
    
    // shortestDistances[i] will hold the
    // shortest distance from src to i
    vector<size_t> shortestDistances(numVertices);

    // added[i] will true if vertex i is
    // included / in shortest path tree
    // or shortest distance from src to
    // i is finalized
    vector<bool> added(numVertices);

    // Initialize all distances as
    // INFINITE and added[] as false
    for (size_t vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
    {
        shortestDistances[vertexIndex] = -1;
        added[vertexIndex] = false;
    }

    // Distance of source vertex from
    // itself is always 0
    shortestDistances[startInternalVertex] = 0;

    // Parent array to store shortest
    // path tree
    vector<int> parents(numVertices);

    // The starting vertex does not
    // have a parent
    parents[startInternalVertex] = NO_PARENT;

    // Find shortest path for all
    // vertices
    for (size_t i = 1; i < numVertices; i++)
    {
        debug(shapeDebugDetailed, "Search: shape id " + to_string(id) + " i = " + to_string(i) + " to a max of " + to_string(numVertices) + "\n");

        // Pick the minimum distance vertex
        // from the set of vertices not yet
        // processed. nearestVertex is
        // always equal to startNode in
        // first iteration.
        size_t nearestVertex = -1;
        size_t shortestDistance = -1;
        for (size_t vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
        {
            if (!added[vertexIndex] && shortestDistances[vertexIndex] < shortestDistance)
            {
                nearestVertex = vertexIndex;
                shortestDistance = shortestDistances[vertexIndex];
            }
        }

        // Mark the picked vertex as
        // processed
        added[nearestVertex] = true;

        // Update dist value of the
        // adjacent vertices of the
        // picked vertex.
        for (size_t vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
        {
            size_t edgeDistance = ps->weights[nearestVertex][vertexIndex];
            if (ps->weights[nearestVertex][vertexIndex] == -1)
                debug(shapeDebugDetailed, "Search shape id " + to_string(id) + " vertexIndex = " + to_string(vertexIndex) + " weights[" + to_string(nearestVertex) + "][" + to_string(vertexIndex) + "] = - \n");
            else
                debug(shapeDebugDetailed, "Search shape id " + to_string(id) + " vertexIndex = " + to_string(vertexIndex) + " weights[" + to_string(nearestVertex) + "][" + to_string(vertexIndex) + "] = " + to_string(ps->weights[nearestVertex][vertexIndex])+ "\n");
            
            if (edgeDistance != -1 && ((shortestDistance + edgeDistance) < shortestDistances[vertexIndex]))
            {
                parents[vertexIndex] = nearestVertex;
                shortestDistances[vertexIndex] = shortestDistance + edgeDistance;
            }
        }
    }

    // debug for distances and parents
    debug(shapeDebugDetailed, "Shape id " + to_string(id) + " shortest distances ");
    for (size_t i = 0; i < shortestDistances.size(); i++)
    {
        debug(shapeDebugDetailed, to_string(shortestDistances[i]) + " ");
    }
    debug(shapeDebugDetailed, "\n");

    debug(shapeDebugDetailed, "parents ");
    for (size_t i = 0; i < parents.size(); i++)
    {
      debug(shapeDebugDetailed, to_string(parents[i]) + " ");
    }
    debug(shapeDebugDetailed, "\n");

    // creates a path for each end-to-end set of vertices/anchors
    // stores the result, since it will be used again and again with recursive splits
    for (size_t i = 0; i < ps->anchorIndices.size(); i++)
    {
        size_t currVertex = ps->anchorIndices[i];
        if (currVertex == startVertex)
            continue;
        debug(shapeDebugDetailed, "Shape id " + to_string(id) + " path from " + to_string(startVertex) + " to " + to_string(currVertex) + " : \n");
        vector<size_t> path;
        path.push_back(currVertex);
        debug(shapeDebugDetailed, "Shape id " + to_string(id) + " pushback " + to_string(currVertex) + "\n");

        // need nearest internal vertex from each endpoint
        currVertex = ps->getNearestInternalNeighbourFromBoundary(currVertex);
        path.push_back(currVertex);
        debug(shapeDebugDetailed, "Shape id " + to_string(id) + " pushback " + to_string(currVertex) + "\n");

        currVertex = parents[currVertex - numBoundary];
        while (currVertex != -1)
        {
            vertex prev = ps->graphVertices[path[path.size() - 1]];
            vertex curr = ps->graphVertices[currVertex + numBoundary];
            if (prev.mX != curr.mX or prev.mY != curr.mY)
                path.push_back(currVertex + numBoundary);
            debug(shapeDebugDetailed, "Shape id " + to_string(id) + " pushback " + to_string(currVertex + numBoundary) + "\n");
            currVertex = parents[currVertex];
        }
        path.push_back(startVertex);
        debug(shapeDebugDetailed, "Shape id " + to_string(id) + " pushback " + to_string(startVertex) + "\n");

        reverse(path.begin(),path.end());

        debug(shapeDebugDetailed, "Shape id " + to_string(id) + " path " + to_string(startVertex) + " to " + to_string(i) + " has length " + to_string(path.size()) + " : ");

        if (path.size() == 0)
        {
            cout << "path " << startVertex << " to " << i << "has length zero!" << endl;
            exit(1);
        }
        
        ps->allPairsPaths[startVertex][ps->anchorIndices[i]] = path;
        ps->printPath(startVertex, i);
    }
    
    debug(shapeDebugExtra, "Completed searching for shape id " + to_string(id) + " from " + to_string(startVertex) + "\n");
}

void shapes::createChildShapes(size_t id)
{
    shape * ps = mShapes[id];

    size_t totalNumPaths = ps->anchorIndices.size() * ps->anchorIndices.size() - ps->anchorIndices.size();

    debug(shapeDebugSimple, "\nStarted to create child shapes for shape id " + to_string(id) + " numpaths = " + to_string(totalNumPaths) + "\n");

    size_t count = 0;

    // debug all the paths
    for (size_t i = 0; i < ps->anchorIndices.size(); i++)
    {
        for (size_t j = 0; j < ps->anchorIndices.size(); j++)
        {
            size_t vstart = ps->anchorIndices[i];
            size_t vend = ps->anchorIndices[j];
            if (vstart == vend) // not creating a path to itself
                continue;
            
            count++;
            debug(shapeDebugExtra, "Shape id " + to_string(id) + " path number = " + to_string(count) + " : split from " + to_string(vstart) + " to " + to_string(vend) + " " + ps->printPath(i, j));
        }
    }

    count = 0;
    for (size_t i = 0; i < ps->anchorIndices.size(); i++)
    {
        for (size_t j = 0; j < ps->anchorIndices.size(); j++)
        {
            size_t vstart = ps->anchorIndices[i];
            size_t vend = ps->anchorIndices[j];
            if (vstart == vend) // not creating a path to itself
                continue;

            count++;
            debug(shapeDebugExtra, "Shape id " + to_string(id) + " new boundary count = " + to_string(count) + " : split from " + to_string(vstart) + " to " + to_string(vend) + "\n");
            debug(shapeDebugExtra, ps->printGraphVertices());

            // create two new shapes with the boundary given by the path split and the outline to either side
            vector<size_t> path = ps->allPairsPaths[vstart][vend];
            vector<size_t> boundaryLeft;
            vector<size_t> boundaryRight;

            debug(shapeDebugExtra, "shape : " + to_string(id) + " " + ps->printPath(vstart, vend));

            // create the left boundary (clockwise boundary)
            // outside then reverse path
            size_t currLeft = vstart;
            boundaryLeft.push_back(vstart);
            while ((currLeft + 1) % ps->numBoundaryVertices != vend)
            {
                currLeft = (currLeft + 1) % ps->numBoundaryVertices; // border vertices do not include the internal graph
                boundaryLeft.push_back(currLeft);
            }
            for (size_t i = 0; i < path.size() - 1; i++)
            {
                boundaryLeft.push_back(path[path.size() - 1 - i]);
            }

            // create the right boundary (clockwise boundary)
            // forward path then outside
            boundaryRight.push_back(vstart);
            for (size_t i = 1; i < path.size(); i++) // already started with the vstart, so don't need it again
            {
                boundaryRight.push_back(path[i]);
            }
            size_t currRight = vend;
            while ((currRight + 1) % ps->numBoundaryVertices != vstart)
            {
                currRight = (currRight + 1) % ps->numBoundaryVertices; // border vertices do not include the internal graph
                boundaryRight.push_back(currRight);
            }

            // to debug the boundaries
            debug(shapeDebugExtra, "Shape id " + to_string(id) + " boundary left: ");
            for (size_t i = 0; i < boundaryLeft.size(); i++)
            {
                debug(shapeDebugExtra, to_string(boundaryLeft[i]) + " ");
            }
            debug(shapeDebugExtra, "\n");

            debug(shapeDebugExtra, "Shape id " + to_string(id) + " boundary right: ");
            for (size_t i = 0; i < boundaryRight.size(); i++)
            {
                debug(shapeDebugExtra, to_string(boundaryRight[i]) + " ");
            }
            debug(shapeDebugExtra, "\n");
            
            string leftName;
            string rightName;
            size_t leftID = addChildShape(id, leftName);
            size_t rightID = addChildShape(id, rightName);
            
            for (size_t i = 0; i < boundaryLeft.size(); i++)
            {
                size_t vIndex = boundaryLeft[i];
                vertex vx = ps->graphVertices[boundaryLeft[i]];
                if (vIndex == vstart || vIndex == vend)
                {
                    vx.mType = vertexType::Vertex;
                }
                if (vx.mType != vertexType::Vertex)
                {
                    vx.mType = vertexType::Knot;
                }
                
                vx.mNeighbourVertexIDs.clear();
                vx.mSplitGraphNeighbourVertexIDs.clear();
                vx.mSplitGraphNeighbourVertexWithBoundaryIDs.clear();

                addVertex(leftID, vx);
                
                debug(shapeDebugExtra, "Shape id " + to_string(id) + " left split vertex " + to_string(i) + " for leftID " + to_string(leftID) + " " + vx.print());
            }
            addBoundaryEdges(leftID);
            split(leftID);

            for (size_t i = 0; i < boundaryRight.size(); i++)
            {
                size_t vIndex = boundaryRight[i];
                vertex vx = ps->graphVertices[boundaryRight[i]];
                if (vIndex == vstart || vIndex == vend)
                {
                    vx.mType = vertexType::Vertex;
                }
                if (vx.mType != vertexType::Vertex)
                {
                    vx.mType = vertexType::Knot;
                }

                vx.mNeighbourVertexIDs.clear();
                vx.mSplitGraphNeighbourVertexIDs.clear();
                vx.mSplitGraphNeighbourVertexWithBoundaryIDs.clear();

                addVertex(rightID, vx);
                debug(shapeDebugExtra, "Shape id " + to_string(id) + " right split vertex " + to_string(i) + " for rightID " + to_string(rightID) + " " + vx.print());
            }
            addBoundaryEdges(rightID);
            split(rightID);
            
            ps->subshapeIDs.push_back(make_pair(leftID, rightID));            
        }
    }
    
    debug(shapeDebugExtra, "Completed creating child shapes for shape id " + to_string(id) + "\n");
}

size_t shapes::getParentID(size_t id)
{
    return mShapes[id]->mParentID;
}

shape * shapes::getShape(size_t id)
{
    return mShapes[id];
}

void shapes::addBoundaryEdges(size_t id)
{
    shape * ps = mShapes[id];
    debug(shapeDebugExtra, "Start addBoundaryEdges for shape id " + to_string(id) + "\n");

    size_t numVertices = ps->graphVertices.size();
    for (size_t i = 0; i < numVertices - 1; i++)
        addBoundaryEdge(id, i, i+1);
    addBoundaryEdge(id, numVertices - 1, 0);
    debug(shapeDebugExtra, "Completed addBoundaryEdges for shape id " + to_string(id) + "\n");
}

// this adds a vertex that I call a centroid to the midpoint of a common edge of two triangles that both
// have a centroid added from the triangulation
void shapes::addEdgeVertex(size_t id, size_t leftID, size_t rightID, size_t centroid0, size_t centroid1)
{
    shape * ps = mShapes[id];

    double x = (ps->graphVertices[leftID].mX + ps->graphVertices[rightID].mX) /2.;
    double y = (ps->graphVertices[leftID].mY + ps->graphVertices[rightID].mY) /2.;

    ps->addVertex(x, y, vertexType::Centroid);

    size_t createdVertex = ps->graphVertices.size() - 1;
    addSplitEdges(id, createdVertex, leftID);
    addSplitEdges(id, createdVertex, rightID);
    addSplitEdges(id, createdVertex, centroid0);
    addSplitEdges(id, createdVertex, centroid1);
}

void shapes::addBoundaryEdge(size_t id, size_t startVertex, size_t endVertex)
{
    shape * ps = mShapes[id];

    edge e = make_pair(startVertex, endVertex);
    ps->edges.push_back(e);
    ps->graphVertices[startVertex].mNeighbourVertexIDs.push_back(endVertex);
    ps->graphVertices[endVertex].mNeighbourVertexIDs.push_back(startVertex);
}

size_t shapes::getCurrentDepth(size_t id)
{
    shape * ps = mShapes[id];

    return ps->mCurrDepth;
}
