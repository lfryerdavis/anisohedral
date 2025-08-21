//
//  shape.hpp
//
//
//  Created by Lee Fryer-Davis on 2025-06-28.
//

#ifndef shape_hpp
#define shape_hpp

#include <string>
#include <vector>
#include <utility> // pair

using namespace std;

typedef pair<size_t, size_t> edge;

extern bool isShowSplitGraphs;
extern bool isShowSplitPaths;
extern bool showInternalGraphAndBoundaries;
extern bool showInternalGraphOnly;
extern bool showTriangulation;
extern bool showVertexIndices;
extern bool showVertexCoordinates;
extern bool isBoundaryOnly;

bool isCollinear(double ax, double ay, double bx, double by, double cx, double cy);

struct shape {
    shape(string name = "", size_t currDepth = 0);
    ~shape();

    void addVertex(double x, double y, vertexType type = vertexType::Vertex);
    void addVertex(vertex vx);
    void changeVertexType(size_t id, vertexType type = vertexType::Vertex);
    void addAnchor(double x, double y);

    void addBoundaryEdges();

    void write();
        
    void addGraphVertex(double x, double y, vertexType type = vertexType::Vertex);

    size_t getNearestInternalNeighbourFromBoundary(size_t vertexID);
    size_t getNumVertices();

    void addThisID(size_t id);
    
    string printGraphVertices();
    string printGraphInternalVertices();
    string printAnchorVertices();
    string printBoundaryVertices();
    string printGraphWeights();
    string printPath(size_t vstart, size_t vend);
    string printAllPaths();

    bool mHasSplit;
    bool mHaveFoundAnchors;
    size_t thisShapeID;
    string mName;
    size_t mCurrDepth;
    size_t mNumCopies;
        
    vector<vertex> graphVertices;
    vector<size_t> graphInternalVertexIndices;
    size_t numBoundaryVertices;
    vector<vertex> anchorVertices; // need this since there is a reordering and duplication when adding anchors to the regular graph
    vector<size_t> anchorIndices;
    vector<bool> collinearBoundaryVertexIndices;
    vector<vector<size_t>> weights;
    vector<vector<vector<size_t> > > allPairsPaths;
    vector<edge> edges;
    vector<pair<size_t, size_t>> subshapeIDs; // list of <leftID, rightID> subtiles
    vector<bool> foundBoundaryAnchors;
    vector<size_t> triangulationIndices;
    vector<size_t> internalGraphIndicesFromEarcutVertices;
    size_t numPossibleEdges;
    vector<size_t> edgeIntersections;
    size_t initNumVertices;
    vector<array<size_t,4> > triangleList;
    vector<size_t> mActions;
    size_t mParentID;
    size_t mPictureID;
};

class shapes {
public:
    shapes();
    ~shapes();
    
    // can't just use copy constructor since there are different ways of copying
    size_t cloneBoundary(size_t id);
    size_t cloneSplits(size_t id);
    size_t addChildShape(size_t id, string & name);
    size_t getParentID(size_t id);
    void addAction(size_t shapeID, size_t actionID);
    void addPicture(size_t shapeID, size_t pictureID);

    size_t getNewID(string name = "", size_t currDepth = 0);
    
    void addName(size_t id, string name);
    const string getName(size_t id);
    void addVertex(size_t id, double x, double y, vertexType type = vertexType::Vertex);
    void addVertex(size_t id, vertex vx);
    void addBoundaryEdges(size_t id);
    shape * getShape(size_t id);
    void write(size_t id);
    void writeAll();
    void split(size_t id);

private:
    void addCentroidVertex(size_t id, size_t leftID, size_t rightID, size_t newID);
    void addEdgeVertex(size_t id, size_t leftID, size_t rightID, size_t centroid0, size_t centroid1);
    void addSplitEdges(size_t id, size_t leftID, size_t rightID);
    void populateAnchors(size_t id);
    void triangulateBorderAndCreateCentroids(size_t id);
    void createInternalGraph(size_t id);
    void createAllPairsPaths(size_t id);
    void searchFromVertex(size_t id, size_t startVertex);
    void createChildShapes(size_t id);
    void addBoundaryEdge(size_t id, size_t startVertex, size_t endVertex);
    void addBoundaryAnchors(size_t id, size_t leftID, size_t rightID);
    void addAnchor(size_t id, double x, double y);
    size_t getCurrentDepth(size_t id);

    vector<shape *> mShapes;
};

extern shapes *gShapes;

#endif /* shape_hpp */
