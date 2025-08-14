//
//  picture.hpp
//
//
//  Created by Lee Fryer-Davis on 2025-06-28.
//

#ifndef picture_hpp
#define picture_hpp

#include <string>
#include <vector>
#include "common.hpp"
#include "shape.hpp"
#include "action.hpp"

using namespace std;

class picture {
public:
    picture();
    ~picture();
    
    void addShape(size_t shapeID);
    void addBaseShape(size_t shapeID);
    void addAction(size_t actionID);
    void write();
private:
    vector<size_t> mShapes;
    vector<size_t> mActions;
    vector<size_t> mBaseShapes;
};

class pictures {
public:
    pictures();
    ~pictures();
    
    size_t createPicture();
    void createPictures(string outname, bool isStandalonePicture = false);
    void addShape(size_t pict, size_t shapeID);
    void addBaseShape(size_t pict, size_t shapeID);
    void addAction(size_t pict, size_t actionID);
    void write(size_t pict);
    void writeAll();
private:
    void writeStandalonePreamble();
    void writeEmbeddedPreamble();
    void writeCommonPreamble();

    string mOutname;
    bool mIsStandalonePicture;
    vector<picture> mPictures;
};

extern pictures *gPictures;

#endif /* picture_hpp */
