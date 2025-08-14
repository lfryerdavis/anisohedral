//
//  action.hpp
//
//
//  Created by Lee Fryer-Davis on 2025-06-28.
//

#ifndef action_hpp
#define action_hpp

#include <string>
#include <vector>

using namespace std;

class action {
public:
    action(size_t shapeID, string shapeName, string color = "", string thickness = "", double x = 0, double y = 0, double rot = 0, bool reflecty = false);
    ~action();
    
    void addRotation(double degrees);
    void translation(double x, double y);
    void reflecty();
    void write();
    
    string mShapeName;
    double rotationDegrees;
    double transX;
    double transY;
    string mColor;
    string mThickness;
    bool mReflecty;
    size_t mShapeID;
};

class actions {
public:
    actions();
    ~actions();
    
    size_t getNewID(size_t shapeID, string shapeName, string color = "", string thickness = "", double x = 0, double y = 0, double rot = 0, bool reflecty = false);
    size_t getNewIDFromParent(size_t parentShapeID, size_t childShapeID, size_t index);
    void addRotation(size_t actionID, double degrees);
    void translation(size_t actionID, double x, double y);
    void reflecty(size_t actionID);
    void write(size_t actionID);
    void writeAll();
private:
    vector<action *> mActions;
};

extern actions *gActions;

#endif /* action_hpp */
