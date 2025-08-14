//
//  common.hpp
//
//
//  Created by Lee Fryer-Davis on 2025-06-28.
//

#ifndef common_hpp
#define common_hpp

#include <vector>

using namespace std;

#define UNUSED(expr) do { (void)(expr); } while (0)

extern size_t maxdepth;
extern size_t debugLevel;

void debug(size_t level = 0, string out = "");

string boolToStr(bool value);

// the AnchorKnot was something that was already a Knot, then became an Anchor
// to allow for easy splits
enum vertexType {Vertex, Knot, Anchor, AnchorKnot, Centroid};

class vertex {
public:
    string print();
    double mX;
    double mY;
    vertexType mType;
    vector<size_t> mNeighbourVertexIDs;
    vector<size_t> mSplitGraphNeighbourVertexIDs;
    vector<size_t> mSplitGraphNeighbourVertexWithBoundaryIDs;
};

#endif /* common_hpp */
