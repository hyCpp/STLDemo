#ifndef GRAPHMTX
#define GRAPHMTX
#include "Graph.h"
#include <queue>

template<class T,class E>
class Graphmtx: public Graph<T,E>
{
public:
    Graphmtx(int sz = DefaultVertices)
    {
        this->maxVertices = sz;
        this->numEdges = 0;
        this->numVertices = 0;

        int i, j;
        m_pVerticesList = new T[this->maxVertices];
        m_pEdge = new E *[this->maxVertices];

        for (i = 0; i < this->maxVertices; ++i) {
            m_pEdge[i] = new E[this->maxVertices];
        }

        for (i = 0; i < this->maxVertices; ++i) {
            for (j = 0; j < this->maxVertices; ++j) {
                m_pEdge[i][j] = (i == j) ? 0 : this->maxWeight; // 对无向图而言，邻接矩阵一定是对称的，而且主对角线一定为零
            }
        }
    }

    ~Graphmtx()
    {
        delete []m_pEdge;
        delete []m_pVerticesList;
    }

    T getValue(int i)
    {

        if (i >= 0 && i < this->numVertices) {
            return m_pVerticesList[i];
        }
        else {
            return NULL;
        }
    }

    E getWeight(int v1, int v2)
    {
        if (v1 != -1 && v2 != -1) {
            return m_pEdge[v1][v2];
        }
        else {
            return 0;
        }
    }

    int getFirstNeighbor(int v)
    {
        if (v != -1) {
            for (int i = 0; i < this->maxVertices; ++i) {
                if (m_pEdge[v][i] > 0 && m_pEdge[v][i] < this->maxWeight) {
                    return i;
                }
            }
        }

        return -1;
    }

    int getNextNeighbor(int v, int w)
    {
        if (v != -1 && w != -1) {
            for (int i = w + 1; i < this->maxVertices; ++i) {
                if (m_pEdge[v][i] > 0 && m_pEdge[v][i] < this->maxWeight) {
                    return i;
                }
            }
        }
        return -1;
    }

    bool insertEdge(int v1, int v2, E weight, bool flags = true)
    {
        if (v1 > -1 && v2 > -1 && v1 < this->numVertices && v2 < this->numVertices) {
            if (m_pEdge[v1][v2] != this->maxWeight) {
                printf("insert Edge error! \n");
                return false;
            }
            else {
                if (flags) {
                    m_pEdge[v1][v2] = weight;
                    m_pEdge[v2][v1] = weight;
                }
                else {
                    m_pEdge[v1][v2] = weight;
                }

                this->numEdges++;
                return true;
            }
        }

        return false;
    }


    bool removeEdge(int v1, int v2)
    {
        if (v1 > -1 && v1 < this->numVertices && v2 > -1 && v2 < this->numVertices) {
            if (m_pEdge[v2][v1] > 0 &&m_pEdge[v2][v1] < this->maxWeight) {
                m_pEdge[v2][v1] = this->maxWeight;
                m_pEdge[v1][v2] = this->maxWeight;

                this->numEdges--;
                return true;
            }
            else {
                printf("remove Edge error !\n");
                return false;
            }
        }

        return false;
    }

    bool insertVertex(const T &vertex)
    {
        if (this->numVertices == this->maxVertices) {
            return false;
        }

        m_pVerticesList[this->numVertices++] = vertex;
        return true;
    }

    bool removeVertex(int v)
    {
        if (v < 0 || v > this->numVertices) {
            return false;
        }
        if (this->numVertices == 1) {
            return false;
        }

        m_pVerticesList[v] = m_pVerticesList[this->numVertices - 1];

        for (int i = 0; i < this->numVertices; ++i) {
            if (m_pEdge[i][v] > 0 && m_pEdge[i][v] < this->maxWeight) {
                this->numEdges--;
            }
        }

        for (int i = 0; i < this->numVertices; ++i) {
            m_pEdge[i][v] = m_pEdge[i][this->numVertices - 1];
        }

        this->numVertices--;

        for (int i = 0; i < this->numVertices; ++i) {
            m_pEdge[v][i] = m_pEdge[this->numVertices][i];
        }

        return true;
    }

    void inputGraph()
    {
        int i, j, k, m, n;
        T e1, e2;
        E weight;
        std::cout<<"请输入顶点数和边数(空格隔开):"<<std::endl;
        std::cin >> n >> m;
        std::cout<<"请依次输入顶点:"<<std::endl;
        for(i = 0; i < n; i++) {
            std::cin>>e1;
            this->insertVertex(e1);
        }

        std::cout<<"请依次输入边，形如 v1 v2 weight ："<<std::endl;
        i = 0;
        while (i < m)
        {
            std::cin >> e1 >> e2 >> weight;
            j = this->getVertexPos(e1);//查顶点号
            k = this->getVertexPos(e2);
            if( j == -1 || k == -1) {
                std::cout<<"边两端点信息有误，重新输入！"<<std::endl;
            }
            else {
                if (this->insertEdge(j, k, weight)) {
                    i++;
                }
            }
        }
    }

    void outputGraph()
    {
        int i, j, m ,n;
        T e1, e2;
        E weight;
        n = this->NumberOfVertices();
        m = this->NumberOfEdes();

        printf("顶点数: %d, 边数为：%d \n",n ,m);
        printf("各边依次为：\n");

        for (i = 0; i < n; ++i) {
            for (j = i + 1; j < n; ++j) {
                weight = getWeight(i, j);
                if (weight > 0 && weight < this->maxWeight) {
                    e1 = getValue(i);
                    e2 = getValue(j);
                    std::cout<<"["<<e1<<","<<e2<<","<<weight<<"]"<<std::endl;
                }
            }
        }
    }

    int getVertexPos(T vertex)
    {
        for (int i = 0; i < this->numVertices; ++i) {
            if (m_pVerticesList[i] == vertex) {
                return i;
            }
        }
        return -1;
    }

    void DFS()
    {
        for (int i = 0; i < this->numVertices; ++i) {
            this->m_isVisit[i] = false;
        }

        for (int i = 0; i < this->numVertices; ++i) {
            if (this->m_isVisit[i] == false) {
                visitVexDFC(i);
            }
        }
        std::cout<<std::endl;
    }

    void BFS()
    {
        for (int i = 0; i < this->numVertices; ++i) {
            this->m_isVisit[i] = false;
        }

    }


private:
    void visitVexDFC(int v) {
        this->m_isVisit[v] = true;
        std::cout<<"->"<<m_pVerticesList[v];

        for (int i = 0; i < this->numVertices; ++i) {
           if (m_pEdge[v][i] != this->maxWeight && this->m_isVisit[i] == false) {
                visitVexDFC(i);
           }
        }
    }

    void visitVexByBFS()
    {
//        this->m_isVisit[0] = true;
//        std::queue<int> q;
//        q.push(0);
//        std::cout<<"->"<<m_pVerticesList[0];
//        while (!q.empty()) {
//            int temp = q.front();
//            q.pop();

//            for (int i = 0; i < this->numVertices; ++i) {

//            }
//        }
    }

private:
    T *m_pVerticesList;
    E ** m_pEdge;
};

#endif // GRAPHMTX

