#ifndef QTREE_H // includes guard
#define QTREE_H

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400
#define SIZE 100


namespace QuadTree
{
    // Used to hold details of a point
    struct Point
    {
        int x;
        int y;
        Point(int _x, int _y);
        Point();
    };
    
    // The objects that we want stored in the quadtree
    struct Node
    {
        Point pos;
        int data1;
        int data2;
        int data3;
        Node(Point _pos, int _data1, int _data2, int _data3);
        Node();
    };
    
    // The main quadtree class
    class Quad
    {
        // Hold details of the boundary of this node
        Point topLeft;
        Point botRight;
    
        // Contains details of node
        Node *n;
    
        // Children of this tree
        Quad *topLeftTree;
        Quad *topRightTree;
        Quad *botLeftTree;
        Quad *botRightTree;
    
    public:
        Quad();
        Quad(Point topL, Point botR);
        void make(int red[SCREEN_WIDTH][SCREEN_HEIGHT],
                  int green[SCREEN_WIDTH][SCREEN_HEIGHT],
                  int blue[SCREEN_WIDTH][SCREEN_HEIGHT],
                  int size); // makes a QuadTree
    
        // Insert a node into the quadtree
        void insert(Node *node);
        
        // Find a node in a quadtree
        Node* search(Point p);
        
        // Check if current quadtree contains the point
        bool inBoundary(Point p);
    };
}

#endif