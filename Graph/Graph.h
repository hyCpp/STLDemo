#ifndef GRAPH
#define GRAPH
#include <stdio.h>
#include <iostream>

#define INF 1000000
const int DefaultVertices = 30;

template<typename T, class E>
class Graph
{
public:
    static const E maxWeight = INF;
    Graph(int size = DefaultVertices) {}
    ~Graph() {}

    //检查为空
    bool GraphEmpty() const {
        if (numEdges == 0)
            return true;
        else
            return false;
    }

    //检查为满
    bool GraphFull() const {
        if (numVertices == maxVertices || numEdges == maxVertices * (maxVertices - 1)/2)
            return true;
        else
            return false;
    }

    //返回当前顶点数
    int NumberOfVertices() {
        return numVertices;
    }

    //返回当前边数
    int NumberOfEdes() {
        return numEdges;
    }

    //取顶点i的值，i不合理返回0
    virtual T getValue(int i) = 0;
    //取边（v1，v2）的权值
    virtual E getWeight(int v1, int v2) = 0;
    //取顶点v的第一个邻接顶点
    virtual int getFirstNeighbor(int v) = 0;
    //取邻接顶点w的下一个邻接顶点
    virtual int getNextNeighbor(int v,int w) = 0;
    //插入一个顶点vertex
    virtual bool insertVertex(const T& vertex) = 0;
    //插入边（v1,v2）,权值cost
    virtual bool insertEdge(int v1, int v2, E weight, bool flags = true) = 0;

    //删除顶点v和所有与之关联的边
    virtual bool removeVertex(int v) = 0;
    //删除边（v1,v2）
    virtual bool removeEdge(int v1,int v2) = 0;
    virtual int getVertexPos(T vertex) = 0;

    /**
     * @brief DFS : depth-first-search
     * 顾名思义广度搜索 : 找到某节点相邻的一个节点，然后从该节点再找下去，直到为空为止，在返回某节点，在在相邻的另外一节点找下去
     */
    virtual void DFS() = 0;
    /**
     * @brief BFS : breadth-first-search
     * 顾名思义广度搜索 : 找到节点相邻的所有节点，然后找相邻的节点的所有相邻节点，一次搜索下去
     */
    virtual void BFS() = 0;
    virtual void inputGraph() = 0;
    virtual void outputGraph() = 0;
//    virtual void printfDistance() = 0;
protected:
    int numEdges;
    int maxVertices;
    int numVertices;
    bool m_isVisit[10000];
};

#endif // GRAPH

