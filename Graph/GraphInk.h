#ifndef GRAPHINK
#define GRAPHINK
#include "Graph.h"
#include <queue>
#include "vector/vector.h"

//边界点的定义
template<typename T, typename E>
struct Edge
{
    int dest; //边的另一顶点位置
    E cost; //权值
    Edge<T, E> *link; //下一条边链指针

    Edge(){}
    Edge(int num , E weight) :
        dest(num),
        cost(weight),
        link(NULL) {}

    bool operator != (Edge<T, E> &R) const {
        return (dest != R.dest) ? true : false;
    }
};

//顶点的定义
template<typename T, typename E>
struct Vertex
{
    T data; //顶点名字
    Edge<T ,E> *adj;  //边链表的头指针
};

template<typename T, typename E>
class GraphInk : public Graph<T, E>
{
public:
    GraphInk(int sz = DefaultVertices)
    {
        this->maxVertices = sz;
        this->numVertices = 0;
        this->numEdges = 0;

        m_pNodeTable = new Vertex<T, E>[this->maxVertices];

        if (m_pNodeTable == NULL) {
            printf("error !");
            return;
        }

        for (int i = 0; i < this->maxVertices; ++i) {
            m_pNodeTable[i].adj = NULL;
        }
    }

    ~GraphInk()
    {
        for (int i = 0; i < this->numVertices; i++) {
            Edge<T, E> *p = m_pNodeTable[i].adj;
            while (p) {
                m_pNodeTable[i].adj = p->link;
                delete p;
                p = m_pNodeTable[i].adj;
            }
        }
    }

    T getValue(int i) {
        return (i >= 0 && i < this->numVertices) ? m_pNodeTable[i].data : 0;
    }

    E getWeight(int v1, int v2)
    {
        if (v1 > -1 && v2 > -1) {
            Edge<T, E> *_p = m_pNodeTable[v1].adj;
            while (_p != NULL && _p->dest != v2) {
                _p = _p->link;
            }

            if (_p) {
                return _p->cost;
            }
        }

        return 0;
    }

    int getFirstNeighbor(int v)
    {
        if (v > -1) {
            Edge<T, E> *_p = m_pNodeTable[v].adj;
            if (_p) {
                return _p->dest;
            }
        }
        return -1;
    }

    int getNextNeighbor(int v, int w)
    {
        if (v > -1) {
            Edge<T, E> *_p = m_pNodeTable[v].adj;
            while (_p && _p->dest != w) {
                _p = _p->link;
            }

            if (_p && _p->link) {
                return _p->link->dest;
            }
        }
        return -1;
    }

    bool insertVertex(const T &vertex)
    {
        if (this->numVertices == this->maxVertices) {
            printf("insert vertex error!\n");
            return false;
        }

        m_pNodeTable[this->numVertices].data = vertex;
        this->numVertices++;
        return true;
    }

    bool removeVertex(int v)
    {
        if (this->numVertices <= 1 || v >= this->numVertices)
            return false;

        Edge<T, E> *_p, *_s, *_t;
        int _k;

        while (m_pNodeTable[v].adj != NULL) { // 删除该顶点，以及与之邻接的顶点中的记录
             _p = m_pNodeTable[v].adj;
             _k = _p->dest;
             _s = m_pNodeTable[_k].adj; // 以找对称存放的边节点
             _t = NULL;

            while (_s && _s->dest != v) { // 在对称点的邻接表里面找v，删除掉
                _t = _s;
                _s = _s->link;
            }

            if (_s) {
                if (_t == NULL) { // 第一个邻接顶点就是v
                    m_pNodeTable[_k].adj = _s->link;
                }
                else {
                    _t->link = _s->link;
                }

                delete _s;
            }

            m_pNodeTable[v].adj = _p->link;
            delete _p;
            this->numEdges--;
        }

        this->numVertices--;

        m_pNodeTable[v].data = m_pNodeTable[this->numVertices].data;
        _p = m_pNodeTable[v].adj = m_pNodeTable[this->numVertices].adj; // 最后位置的链表替换删除的链表

        while (_p) { // 邻接定点中的dest修改位dest
            _s = m_pNodeTable[_p->dest].adj;
            while (_s) {
                if (_s->dest == this->numVertices) {
                    _s->dest = v;
                    break;
                }
                else {
                    _s = _s->link;
                }
            }
            _p = _p->link;
        }

        return true;
    }

    bool insertEdge(int v1, int v2, E weight, bool flags = true)
    {
        if (v1 >= 0 && v1 < this->numVertices && v2 >= 0 && v2 < this->numVertices) {
            Edge<T, E> *_q, *_p = m_pNodeTable[v1].adj;

            while (_p && _p->dest != v2) { //先检查该边是否已经存在
                _p = _p->link;
            }

            if (_p) {
                printf("insert Edge error! \n");
                return false;
            }

            if (flags) {
                _p = new Edge<T, E>;
                _p->dest = v2;
                _p->cost = weight;
                _p->link = m_pNodeTable[v1].adj;
                m_pNodeTable[v1].adj = _p;

                _q = new Edge<T, E>;
                _q->dest = v1;
                _q->cost = weight;
                _q->link = m_pNodeTable[v2].adj;
                m_pNodeTable[v2].adj = _q;
            }
            else {
                _p = new Edge<T, E>;
                _p->dest = v2;
                _p->cost = weight;
                _p->link = m_pNodeTable[v1].adj;
                m_pNodeTable[v1].adj = _p;
            }

            this->numEdges++;

            return true;
        }

        return false;
    }

    bool removeEdge(int v1, int v2)
    {
        if (v1 >= 0 && v1 < this->numVertices && v2 >= 0 && v2 < this->numVertices) {
            Edge<T, E> *_p = m_pNodeTable[v1].adj, *_q = NULL, *_s = _p;

            while (_p && _p->dest != v2) {  // 先找该边
                _q = _p;
                _p = _p->link;
            }

            if (_p) { // 找到该边
                if (_p == _s) {
                    m_pNodeTable[v1].adj = _p->link;
                }
                else {
                    _q->link = _p->link;
                }
                delete _p;
            }
            else {
                return false;
            }

            _p = m_pNodeTable[v2].adj;
            _q = NULL;
            _s = _p;

            while (_p && _p->dest != v1) {
                _q = _p;
                _p = _p->link;
            }

            if (_p) {
                if (_p == _s) {
                    m_pNodeTable[v2].adj = _p->link;
                }
                else {
                    _q->link = _p->link;
                }
            }

            delete _p;

            this->numEdges--;

            return true;
        }

        return false;
    }

    void addNumVertice() {
        this->numVertices++;
    }

    void inputGraph()
    {
        int j, k, m, n;
        T e1, e2;
        E weight;
        printf("请输入顶点数和边数(空格隔开): \n");
        std::cin >> n >> m;
        printf("请依次输入顶点: \n");
        for (int i = 0; i < n; ++i) {
            std::cin>>e1;
            this->insertVertex(e1);
        }

        printf("请依次输入边，形如 v1 v2 weight ：\n");
        int i = 0;

        while (i < m) {
            std::cin >> e1 >> e2 >> weight;
            j = this->getVertexPos(e1);
            k = this->getVertexPos(e2);

            if (j < 0 || k < 0) {
                printf("input vertext error!\n");
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
            if (m_pNodeTable[i].data == vertex) {
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
                visitVex(i);
            }
        }
        std::cout<<std::endl;
    }

    void BFS()
    {
        for (int i = 0; i < this->numVertices; ++i) {
            this->m_isVisit[i] = false;
        }
        visitVexByBFS();
//        visitVexByBFS2();
    }

    void printDistance()
    {
        std::cout<<"Min number of edges : "<<m_numMinEdge.size()<<std::endl;
    }

private:
    void visitVex(int v) {
        this->m_isVisit[v] = true;
        std::cout<<"->"<<m_pNodeTable[v].data;

        for (int i = getFirstNeighbor(v); i >= 1; i = getNextNeighbor(v, i)) {
            if (this->m_isVisit[i] == false) {
                visitVex(i);
            }
        }
    }

    void visitVexByBFS()
    {
        this->m_isVisit[0] = true;

        for (int i = 0; i < this->numVertices; ++i) {
            std::cout<<"["<<m_pNodeTable[i].data<<" ->";
                for (int j = getFirstNeighbor(i); j > 0; j = getNextNeighbor(i, j)) {
                    if (this->m_isVisit[j] == false) {
                        this->m_isVisit[j] = true;
                        m_numMinEdge.push_back(getWeight(i, j));
                        std::cout<< " " << m_pNodeTable[j].data;
                    }
                }
                std::cout<<" ], ";
        }
        std::cout<<std::endl;
    }

    void visitVexByBFS2()
    {
        std::queue<int> q;
        q.push(0);

        this->m_isVisit[0] = true;

        while (!q.empty()) {
            int cur = q.front();
            Edge<T, E>* _p = m_pNodeTable[cur].adj;
            std::cout<<"["<<m_pNodeTable[cur].data<<" ->";
            q.pop();

            // first :
            for (int j = getFirstNeighbor(cur); j > 0; j = getNextNeighbor(cur, j)) {
                if (!this->m_isVisit[j]) {
                    this->m_isVisit[j] = true;
                    q.push(j);
                    m_numMinEdge.push_back(getWeight(cur, j));
                    std::cout<< " " << m_pNodeTable[j].data;
                }
            }

            // second:
            //            while (_p) {
            //                if (!this->m_isVisit[_p->dest]) {
            //                    q.push(_p->dest);
            //                    this->m_isVisit[_p->dest] = true;
            //                    std::cout<< " " << m_pNodeTable[_p->dest].data;
            //                }
            //                _p = _p->link;

            //            }

            std::cout<<" ], "<<std::endl;
        }
    }


private:
    Vertex<T ,E> *m_pNodeTable; //顶点表
    STL::vector<int> m_numMinEdge;

};
#endif // GRAPHINK

