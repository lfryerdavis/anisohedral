//
//  action.cpp
//
//
//  Created by Lee Fryer-Davis on 2025-06-28.
//

#include <fstream>
#include <iostream>
#include <iomanip>
#include "common.hpp"
#include "action.hpp"
#include "shape.hpp"

using namespace std;

extern ofstream outFile;

size_t actionDebugSimple = 1;
size_t actionDebugExtra = 1;

action::action(size_t shapeID, string shapeName, string color, string thickness, double x, double y, double rot, bool reflecty)
{
    mShapeID = shapeID;
    mShapeName = shapeName;
    rotationDegrees = rot;
    transX = x;
    transY = y;
    mColor = color;
    mThickness = thickness;
    mReflecty = reflecty;
}

action::~action()
{
}

void action::addRotation(double degrees)
{
    rotationDegrees = degrees;
}

void action::translation(double x, double y)
{
    transX = x;
    transY = y;
}

void action::reflecty()
{
    // need to do this still
}

void action::write()
{
    outFile << "\t\\path[" << mColor;
    if (mThickness != "")
        outFile << ", " << mThickness;
    outFile << "] (\\reg*" << transX << ", \\reg*" << transY << ") pic";
    if (rotationDegrees == 0.0)
        outFile << "{" << mShapeName << "};" << endl;
    else
        outFile << " [rotate = " << rotationDegrees << "] {" << mShapeName << "};" << endl;
    
    // need to account for the reflection across the y-axis
}

actions::actions()
{
    
}

actions::~actions()
{
    for (size_t i = 0; i < mActions.size(); i++)
        delete mActions[i];
    
    mActions.clear();
}

size_t actions::getNewID(size_t shapeID, string shapeName, string color, string thickness, double x, double y, double rot, bool reflecty)
{
    action *pAction = new action(shapeID, shapeName, color, thickness, x, y, rot, reflecty);
    mActions.push_back(pAction);
    size_t newActionID = mActions.size()-1;
    gShapes->addAction(shapeID, newActionID);
    return newActionID;
}

size_t actions::getNewIDFromParent(size_t parentShapeID, size_t childShapeID, size_t index)
{
    debug(actionDebugExtra, "getNewIDFromParent from id " + to_string(parentShapeID) + " child shape id = " + to_string(childShapeID) + " index = " + to_string(index) + "\n");

    size_t currAction = gShapes->getShape(parentShapeID)->mActions[index];
    debug(actionDebugExtra, "getNewIDFromParent from id " + to_string(parentShapeID) +  " currAction = " + to_string(currAction) + "\n");
    action *parentAction = mActions[currAction];
    string childName = gShapes->getShape(childShapeID)->mName;
    debug(actionDebugExtra, "getNewIDFromParent from id " + to_string(parentShapeID) +  " childName = " + childName + "\n");
    action *pAction = new action(childShapeID, childName, parentAction->mColor, parentAction->mThickness, parentAction->transX, parentAction->transY, parentAction->rotationDegrees, parentAction->mReflecty);
    mActions.push_back(pAction);
    size_t newActionID = mActions.size()-1;

    return newActionID;
}

void actions::addRotation(size_t actionID, double degrees)
{
    if (actionID > mActions.size())
    {
        cout << "Error: shape id out of pool!" << endl;
        exit(1);
    }
    mActions[actionID]->addRotation(degrees);
}

void actions::translation(size_t actionID, double x, double y)
{
    if (actionID > mActions.size())
    {
        cout << "Error: shape id out of pool!" << endl;
        exit(1);
    }
    mActions[actionID]->translation(x, y);
}

void actions::reflecty(size_t actionID)
{
    if (actionID > mActions.size())
    {
        cout << "Error: shape id out of pool!" << endl;
        exit(1);
    }
    mActions[actionID]->reflecty();
}

void actions::write(size_t actionID)
{
    if (actionID > mActions.size())
    {
        cout << "Error: action id out of pool!" << endl;
        exit(1);
    }
    mActions[actionID]->write();
}

void actions::writeAll()
{
    for (size_t i = 0; i < mActions.size(); i++)
    {
        mActions[i]->write();
    }
}

