// C++ Implementation of Quad Tree
#include <iostream>
#include <cmath>
#include <qtree.hpp>
using namespace std;
  
// Used to hold details of a point
    QuadTree::Point::Point(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    QuadTree::Point::Point()
    {
        x = 0;
        y = 0;
    }
  
// The objects that we want stored in the quadtree
    QuadTree::Node::Node(QuadTree::Point _pos, int _data1, int _data2, int _data3)
    {
        pos = _pos;
        data1 = _data1;
        data2 = _data2;
        data3 = _data3;
    }
    QuadTree::Node::Node()
    {
        data1 = 0;
        data2 = 0;
        data3 = 0;
    }
  
// The main quadtree class
    QuadTree::Quad::Quad()
    {
        topLeft = Point(0, 0);
        botRight = Point(0, 0);
        n = NULL;
        topLeftTree  = NULL;
        topRightTree = NULL;
        botLeftTree  = NULL;
        botRightTree = NULL;
    }
    QuadTree::Quad::Quad(Point topL, Point botR)
    {
        n = NULL;
        topLeftTree  = NULL;
        topRightTree = NULL;
        botLeftTree  = NULL;
        botRightTree = NULL;
        topLeft = topL;
        botRight = botR;
    }
  
// Insert a node into the quadtree
void QuadTree::Quad::insert(Node *node)
{
    if (node == NULL)
        return;
  
    // Current quad cannot contain it
    if (!inBoundary(node->pos))
        return;
  
    // We are at a quad of unit area
    // We cannot subdivide this quad further
    if (abs(topLeft.x - botRight.x) <= 1 &&
        abs(topLeft.y - botRight.y) <= 1)
    {
        if (n == NULL)
            n = node;
        return;
    }
  
    if ((topLeft.x + botRight.x) / 2 >= node->pos.x)
    {
        // Indicates topLeftTree
        if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
        {
            if (topLeftTree == NULL)
                topLeftTree = new Quad(
                    Point(topLeft.x, topLeft.y),
                    Point((topLeft.x + botRight.x) / 2,
                        (topLeft.y + botRight.y) / 2));
            topLeftTree->insert(node);
        }
  
        // Indicates botLeftTree
        else
        {
            if (botLeftTree == NULL)
                botLeftTree = new Quad(
                    Point(topLeft.x,
                        (topLeft.y + botRight.y) / 2),
                    Point((topLeft.x + botRight.x) / 2,
                        botRight.y));
            botLeftTree->insert(node);
        }
    }
    else
    {
        // Indicates topRightTree
        if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
        {
            if (topRightTree == NULL)
                topRightTree = new Quad(
                    Point((topLeft.x + botRight.x) / 2,
                        topLeft.y),
                    Point(botRight.x,
                        (topLeft.y + botRight.y) / 2));
            topRightTree->insert(node);
        }
  
        // Indicates botRightTree
        else
        {
            if (botRightTree == NULL)
                botRightTree = new Quad(
                    Point((topLeft.x + botRight.x) / 2,
                        (topLeft.y + botRight.y) / 2),
                    Point(botRight.x, botRight.y));
            botRightTree->insert(node);
        }
    }
}
  
// Find a node in a quadtree
QuadTree::Node* QuadTree::Quad::search(Point p)
{
    // Current quad cannot contain it
    if (!inBoundary(p))
        return NULL;
  
    // We are at a quad of unit length
    // We cannot subdivide this quad further
    if (n != NULL)
        return n;
  
    if ((topLeft.x + botRight.x) / 2 >= p.x)
    {
        // Indicates topLeftTree
        if ((topLeft.y + botRight.y) / 2 >= p.y)
        {
            if (topLeftTree == NULL)
                return NULL;
            return topLeftTree->search(p);
        }
  
        // Indicates botLeftTree
        else
        {
            if (botLeftTree == NULL)
                return NULL;
            return botLeftTree->search(p);
        }
    }
    else
    {
        // Indicates topRightTree
        if ((topLeft.y + botRight.y) / 2 >= p.y)
        {
            if (topRightTree == NULL)
                return NULL;
            return topRightTree->search(p);
        }
  
        // Indicates botRightTree
        else
        {
            if (botRightTree == NULL)
                return NULL;
            return botRightTree->search(p);
        }
    }
};
  
// Check if current quadtree contains the point
bool QuadTree::Quad::inBoundary(Point p)
{
    return (p.x >= topLeft.x &&
        p.x <= botRight.x &&
        p.y >= topLeft.y &&
        p.y <= botRight.y);
}

void QuadTree::Quad::make(int red[SCREEN_WIDTH][SCREEN_HEIGHT],
                    int green[SCREEN_WIDTH][SCREEN_HEIGHT],
                    int blue[SCREEN_WIDTH][SCREEN_HEIGHT], int size)
{
    if(size>1)
    {
        Point a(topLeft.x+ 0,topLeft.y + 0);
        Point b(topLeft.x+size,topLeft.y + size/2);
        Point c(topLeft.x+size,topLeft.y + size);
        Point d(topLeft.x+size/2,topLeft.y + 0);
        Point e(topLeft.x+0,topLeft.y + size/2);
        Point f(topLeft.x+size/2,topLeft.y + size);
        Point centre(topLeft.x+size/2,topLeft.y + size/2);
        topLeftTree = new Quad(a,centre);
        topRightTree = new Quad(d,b);
        botRightTree = new Quad(centre,c);
        botLeftTree = new Quad(e,f);

        topLeftTree->make(red, green, blue, size/2);
        topLeftTree->make(red, green, blue, size/2);
        topLeftTree->make(red, green, blue, size/2);
        topLeftTree->make(red, green, blue, size/2);
    }
    else
    {
        n = new Node(topLeft, red[topLeft.x][topLeft.y], green[topLeft.x][topLeft.y], blue[topLeft.x][topLeft.y]);
    }
}
