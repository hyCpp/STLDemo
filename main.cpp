#include "list/list.h"
#include <stddef.h>
#include <iterator>
#include <array>
#include "vector/vector.h"
#include <string>
#include <algorithm>
#include "set/set.h"
#include "map/map.h"
#include "t_br_tree.h"
#include "hashtable/hashtable.h"
#include "Graph/GraphInk.h"
#include "Graph/GraphMtx.h"
#include "deque.h"
#include <stdio.h>
#include <math.h>
#include <heap/heap.h>



int test1(int t)
{
    std::cout<<"partition"<<std::endl;
    t = t + 4;
    return t;
}

void test(int n, int x, int y) {
    while (y - x > 16) {
        if (n == 0) {
            std::cout<<"return"<<std::endl;
            return;
        }
        int tmp = test1(x);
        std::cout<<"before n = "<<n<<", x = "<<x<<", y = "<<y<<std::endl;
        --n;

        test(n, tmp, y);
        y = tmp;
        std::cout<<"after n = "<<n<<", x = "<<x<<", y = "<<y<<std::endl;
    }
}

void listTest()
{
    int arr[3] = {11, 22, 33};
    STL::list<int> ll;
    ll.push_back(11);
    ll.push_back(22);
    ll.push_back(33);

    STL::list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(102);
    l.push_back(3);
    l.push_back(98);
    l.push_back(4);

    l.remove(102);
    l.remove(98);
    STL::list<int>::iterator i;
    for (i = l.begin(); i != l.end(); ++i) {
        std::cout << (*i) <<std::endl;
    }
    std::cout <<"-----list------"<<std::endl;

    l.splice(find(l.begin(), l.end(), 3), ll);
    for (i = l.begin(); i != l.end(); ++i) {
        std::cout << (*i) <<std::endl;
    }
    std::cout <<"-----------"<<std::endl;
    l.splice(find(l.begin(), l.end(), 1), l, find(l.begin(), l.end(), 4));
    for (i = l.begin(); i != l.end(); ++i) {
        std::cout << (*i) <<std::endl;
    }
    std::cout <<"-----------"<<std::endl;
    l.splice(find(l.begin(), l.end(), 4), l, find(l.begin(), l.end(), 11), find(l.begin(), l.end(), 3));
    for (i = l.begin(); i != l.end(); ++i) {
        std::cout << (*i) <<std::endl;
    }
    std::cout <<"-----------"<<std::endl;
    l.reverse();
    for (i = l.begin(); i != l.end(); ++i) {
        std::cout << (*i) <<std::endl;
    }

    std::cout <<"-----------"<<std::endl;
    STL::list<int> lll(arr, arr + 3);
    for (i = lll.begin(); i != lll.end(); ++i) {
        std::cout << (*i) <<std::endl;
    }
}

void hashtableTest()
{
    hashtable<int, int, hash<int>, identity<int>, equal_to<int> > iht(100, hash<int>(), equal_to<int>());
    std::cout<<"hashtable size : "<<iht.size()<<std::endl;
    std::cout<<"hashtable bucket count : "<<iht.bucket_count()<<std::endl;
    std::cout<<"hashtable max bucket count : "<<iht.max_buckets_count()<<std::endl;
    std::vector<int> t;
    iht.insert_unique(59);
    iht.insert_unique(63);
    iht.insert_unique(108);
    iht.insert_unique(2);
    iht.insert_unique(46);
    iht.insert_unique(87);
    std::cout<<"hashtable size : "<<iht.size()<<std::endl;
    hashtable<int, int, hash<int>, identity<int>, equal_to<int> >::iterator hashIter;

    for (hashIter = iht.begin(); hashIter != iht.end(); ++ hashIter) {
        std::cout<<*hashIter<<std::endl;
    }
    std::cout<<"hashtable size : "<<iht.size()<<std::endl;
    std::cout<<"hashtable bucket count : "<<iht.bucket_count()<<std::endl;

    for (int i = 0; i < iht.bucket_count(); ++i) {
        int n = iht.elems_in_bucket(i);
        if (n != 0) {
            std::cout<<"bucket["<<i<<"] has "<<n<<" elems."<<std::endl;
        }
    }

    for (int i = 200; i < 250; ++i) {
        iht.insert_unique(i);
    }

    std::cout<<"hashtable size : "<<iht.size()<<std::endl;
    std::cout<<"hashtable bucket count : "<<iht.bucket_count()<<std::endl;
}

void vectorTest()
{
    int arrVec[9] = {11, 22, 33, 44, 55, 66, 77, 88, 99};
    STL::vector<int> vec1(arrVec, arrVec + 9);
    STL::vector<int>::iterator vecIter;

    STL::fill(vec1.begin(), vec1.end(), 7);
    std::for_each(vec1.begin(), vec1.end(), display<int>());
    std::cout <<"-----------"<<std::endl;
    STL::fill_n(vec1.begin(), 3, 888);
    std::for_each(vec1.begin(), vec1.end(), display<int>());

    std::cout <<"-----------"<<std::endl;
    int ia[5] = {0, 1, 2, 3, 4};
    STL::copy(ia, ia + 5, ia);
    std::for_each(ia, ia + 5, display<int>());
    std::cout <<"-----------"<<std::endl;

    STL::list<int> ilist(ia, ia + 5);
    STL::list<int> iilist;
    iilist.push_back(1);
    iilist.push_back(1);
    iilist.push_back(1);
    iilist.push_back(1);
    iilist.push_back(1);
    STL::copy(ilist.begin(), ilist.end(), iilist.begin());
    std::for_each(iilist.begin(), iilist.end(), display<int>());
    std::cout <<"-----------"<<std::endl;
    STL::vector<int> ivec(ia, ia + 5);
    STL::vector<int> iivec(5, 7);
    STL::copy(ivec.begin(), ivec.end(), iivec.begin());
    std::for_each(iivec.begin(), iivec.end(), display<int>());
}

void vectorTest2()
{
    STL::vector<int> vec;
    vec.push_back(11);
    std::cout << "vec.capacity = "<<vec.capacity() <<std::endl;
    vec.push_back(22);
    std::cout << "vec.capacity = "<<vec.capacity() <<std::endl;
    vec.push_back(33);
    std::cout << "vec.capacity = "<<vec.capacity() <<std::endl;
    vec.push_back(44);
    std::cout << "vec.capacity = "<<vec.capacity() <<std::endl;
    vec.push_back(55);
    std::cout << "vec.capacity = "<<vec.capacity() <<std::endl;
    std::cout << "vec.size = "<<vec.size() <<std::endl;
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] <<std::endl;
    }
    std::cout <<"-----------"<<std::endl;
    vec.erase(vec.begin(), vec.end());
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] <<std::endl;
    }
    std::cout << "vec.capacity = "<<vec.capacity() <<std::endl;
    std::cout << "vec.size = "<<vec.size() <<std::endl;
    std::cout <<"-----------"<<std::endl;

    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec.at(i) <<std::endl;
    }
    std::cout << "vec.capacity = "<<vec.capacity() <<std::endl;
    std::cout << "vec.size = "<<vec.size() <<std::endl;
}

void mapTest()
{
    STL::map<std::string, int> tmap;
    tmap["test2"] = 2;
    tmap["test4"] = 4;
    tmap["test3"] = 3;
    tmap.insert(STL::pair<const std::string, int>("test5", 5));
    tmap["test1"] = 1;
    tmap.insert(STL::map<std::string, int>::value_type("test6", 6));
    std::cout <<"map size = "<<tmap.size()<<std::endl;
    STL::map<std::string, int>::iterator iterMap;

    for (iterMap = tmap.begin(); iterMap != tmap.end(); ++iterMap) {
        std::cout<<"map = ["<<iterMap->first<<" , "<<iterMap->second<<"]"<<std::endl;
    }

    iterMap = tmap.find("test3");
    std::cout<<"map 3 = ["<<(*iterMap).first<<" , "<<(*iterMap).second<<"]"<<std::endl;

    iterMap->second = 33;
    std::cout<<"map 3 = ["<<(*iterMap).first<<" , "<<(*iterMap).second<<"]"<<std::endl;
}

void setTest()
{
    int setArr[5] = {0, 1, 2, 3, 4};
    STL::set<int>::iterator setIter;
    STL::set<int> set(setArr, setArr + 5);
    for (setIter = set.begin(); setIter != set.end(); setIter++) {
        std::cout <<"set value : "<<*setIter<<std::endl;
    }
    set.insert(3);
    std::cout <<"set count : "<<set.count(3)<<std::endl;
    set.insert(5);
    set.insert(4);
    set.insert(6);
    set.insert(2);
    std::cout <<"set size : "<<set.size()<<std::endl;
    std::cout <<"set empty : "<<set.empty()<<std::endl;

    for (setIter = set.begin(); setIter != set.end(); setIter++) {
        std::cout <<"set value : "<<*setIter<<std::endl;
    }
}

void treeTest()
{
    rb_tree<int, int, identity<int>, std::less<int> > itree;
    itree.insert_unique(15);
    itree.insert_unique(13);
    itree.insert_unique(12);
    itree.insert_unique(11);
    itree.insert_unique(10);
    itree.insert_unique(14);
    itree.insert_unique(17);
    itree.insert_unique(16);

    std::cout <<"br_tree size : "<<itree.size()<<std::endl;
    rb_tree<int, int, identity<int>, std::less<int> >::iterator iter;
    __rb_tree_base_iterator brtite;
    for (iter = itree.begin(); iter != itree.end(); ++iter) {
        brtite = __rb_tree_base_iterator(iter);
        std::cout <<*iter<<"("<<brtite.node->color<<")"<<std::endl;
    }

    itree.clear();
    std::cout <<"br_tree size : "<<itree.size()<<std::endl;
    for (iter = itree.begin(); iter != itree.end(); ++iter) {
        brtite = __rb_tree_base_iterator(iter);
        std::cout <<*iter<<"("<<brtite.node->color<<")"<<std::endl;
    }
}

void GraphInkTest()
{
    std::cout <<"-----Grapglnk Test------"<<std::endl;
    char ch1,ch2;
    int weight;
    GraphInk<char, int> g(30);
    g.inputGraph();     //构造图
    g.outputGraph();    //显示图
    std::cout<<"边数："<<g.NumberOfEdes()<<" , 顶点数:"<<g.NumberOfVertices()<<std::endl;

    std::cout<<"插入顶点:";
    std::cin >>ch1;
    g.insertVertex(ch1);
    std::cout<<"插入边："<<std::endl;
    std::cin >>ch1>>ch2>>weight;
    g.insertEdge(g.getVertexPos(ch1),g.getVertexPos(ch2),weight);//插入边
    g.outputGraph();

    std::cout<<"删除边：";
    std::cin >>ch1>>ch2;
    g.removeEdge(g.getVertexPos(ch1),g.getVertexPos(ch2)); //删除边
    g.outputGraph();

    std::cout<<"删除点：";
    std::cin>>ch1;
    g.removeVertex(g.getVertexPos(ch1));
    g.outputGraph();
}

void GraphMtxTest()
{
    std::cout <<"-----GrapgMtx Test------"<<std::endl;
    char ch1,ch2;
    int weight;
    Graphmtx<char, int> g(30);
    g.inputGraph();     //构造图
    g.outputGraph();    //显示图
    std::cout<<"边数："<<g.NumberOfEdes()<<" , 顶点数:"<<g.NumberOfVertices()<<std::endl;

    std::cout<<"插入顶点:";
    std::cin >>ch1;
    g.insertVertex(ch1);
    std::cout<<"插入边："<<std::endl;
    std::cin >>ch1>>ch2>>weight;
    g.insertEdge(g.getVertexPos(ch1),g.getVertexPos(ch2),weight);//插入边
    g.outputGraph();

    std::cout<<"删除边：";
    std::cin >>ch1>>ch2;
    g.removeEdge(g.getVertexPos(ch1),g.getVertexPos(ch2)); //删除边
    g.outputGraph();

    std::cout<<"删除点：";
    std::cin>>ch1;
    g.removeVertex(g.getVertexPos(ch1));
    g.outputGraph();
}

template<typename T,typename E>
void createGraph(Graph<T, E>* g)
{
//    g->insertVertex("v1");
//    g->insertVertex("v2");
//    g->insertVertex("v3");
//    g->insertVertex("v4");
//    g->insertVertex("v5");
//    g->insertVertex("v6");
//    g->insertVertex("v7");
//    g->insertVertex("v8");
//    int i = 0,k;

//    i = g->getVertexPos("v1");
//    k = g->getVertexPos("v2");
//    g->insertEdge(i, k, 3);

//    i = g->getVertexPos("v2");
//    k = g->getVertexPos("v4");
//    g->insertEdge(i, k, 6);

//    i = g->getVertexPos("v4");
//    k = g->getVertexPos("v8");
//    g->insertEdge(i, k, 12);

//    i = g->getVertexPos("v8");
//    k = g->getVertexPos("v5");
//    g->insertEdge(i, k, 13);

//    i = g->getVertexPos("v5");
//    k = g->getVertexPos("v2");
//    g->insertEdge(i, k, 7);

//    i = g->getVertexPos("v1");
//    k = g->getVertexPos("v3");
//    g->insertEdge(i, k, 4);

//    i = g->getVertexPos("v3");
//    k = g->getVertexPos("v6");
//    g->insertEdge(i, k, 9);

//    i = g->getVertexPos("v6");
//    k = g->getVertexPos("v7");
//    g->insertEdge(i, k, 11);

//    i = g->getVertexPos("v7");
//    k = g->getVertexPos("v3");
//    g->insertEdge(i, k, 10);

        g->insertVertex("A");
        g->insertVertex("B");
        g->insertVertex("C");
        g->insertVertex("D");
        int i = 0,k;

        i = g->getVertexPos("A");
        k = g->getVertexPos("C");
        g->insertEdge(k, i, 3, false);

        i = g->getVertexPos("B");
        k = g->getVertexPos("C");
        g->insertEdge(k, i, 4, false);
        g->insertEdge(i, k, 4, false);

        i = g->getVertexPos("D");
        k = g->getVertexPos("C");
        g->insertEdge(k, i, 5, false);

        i = g->getVertexPos("D");
        k = g->getVertexPos("B");
        g->insertEdge(k, i, 1, false);
}

void GraphMtxDFS()
{
    Graph<char*, int>* g = new Graphmtx<char*, int>(30);
    createGraph(g);
//    g->inputGraph();
    g->outputGraph();
    g->DFS();

}

void GraphInkDFS()
{
    Graph<char*, int>* g = new GraphInk<char*, int>(30);
    createGraph(g);
//    g->inputGraph();
    g->outputGraph();
    g->DFS();
}

void GraphInkBFS()
{
    GraphInk<char*, int>* g = new GraphInk<char*, int>(30);
    createGraph(g);
//    g->inputGraph();
    g->outputGraph();
    g->BFS();
    g->printDistance();
}

inline const char* GetDigitsLut() {
    static const char cDigitsLut[200] = {
        '0','0','0','1','0','2','0','3','0','4','0','5','0','6','0','7','0','8','0','9',
        '1','0','1','1','1','2','1','3','1','4','1','5','1','6','1','7','1','8','1','9',
        '2','0','2','1','2','2','2','3','2','4','2','5','2','6','2','7','2','8','2','9',
        '3','0','3','1','3','2','3','3','3','4','3','5','3','6','3','7','3','8','3','9',
        '4','0','4','1','4','2','4','3','4','4','4','5','4','6','4','7','4','8','4','9',
        '5','0','5','1','5','2','5','3','5','4','5','5','5','6','5','7','5','8','5','9',
        '6','0','6','1','6','2','6','3','6','4','6','5','6','6','6','7','6','8','6','9',
        '7','0','7','1','7','2','7','3','7','4','7','5','7','6','7','7','7','8','7','9',
        '8','0','8','1','8','2','8','3','8','4','8','5','8','6','8','7','8','8','8','9',
        '9','0','9','1','9','2','9','3','9','4','9','5','9','6','9','7','9','8','9','9'
    };
    return cDigitsLut;
}


inline char* u32toa(uint32_t value, char* buffer) {
    const char* cDigitsLut = GetDigitsLut();

    if (value < 10000) {
        const uint32_t d1 = (value / 100) << 1;
        const uint32_t d2 = (value % 100) << 1;

        if (value >= 1000)
            *buffer++ = cDigitsLut[d1];

        printf("cDigitsLut is %c \n", cDigitsLut[d1]);
        printf("buffer is %c \n", *buffer);
        if (value >= 100)
            *buffer++ = cDigitsLut[d1 + 1];

        if (value >= 10)
            *buffer++ = cDigitsLut[d2];

        *buffer++ = cDigitsLut[d2 + 1];

        printf("buffer is %s \n", buffer);
    }
    else if (value < 100000000) {
        // value = bbbbcccc
        const uint32_t b = value / 10000;
        const uint32_t c = value % 10000;

        const uint32_t d1 = (b / 100) << 1;
        const uint32_t d2 = (b % 100) << 1;

        const uint32_t d3 = (c / 100) << 1;
        const uint32_t d4 = (c % 100) << 1;

        if (value >= 10000000)
            *buffer++ = cDigitsLut[d1];
        if (value >= 1000000)
            *buffer++ = cDigitsLut[d1 + 1];
        if (value >= 100000)
            *buffer++ = cDigitsLut[d2];
        *buffer++ = cDigitsLut[d2 + 1];

        *buffer++ = cDigitsLut[d3];
        *buffer++ = cDigitsLut[d3 + 1];
        *buffer++ = cDigitsLut[d4];
        *buffer++ = cDigitsLut[d4 + 1];
    }
    else {
        // value = aabbbbcccc in decimal

        const uint32_t a = value / 100000000; // 1 to 42
        value %= 100000000;

        if (a >= 10) {
            const unsigned i = a << 1;
            *buffer++ = cDigitsLut[i];
            *buffer++ = cDigitsLut[i + 1];
        }
        else
            *buffer++ = static_cast<char>('0' + static_cast<char>(a));

        const uint32_t b = value / 10000; // 0 to 9999
        const uint32_t c = value % 10000; // 0 to 9999

        const uint32_t d1 = (b / 100) << 1;
        const uint32_t d2 = (b % 100) << 1;

        const uint32_t d3 = (c / 100) << 1;
        const uint32_t d4 = (c % 100) << 1;

        *buffer++ = cDigitsLut[d1];
        *buffer++ = cDigitsLut[d1 + 1];
        *buffer++ = cDigitsLut[d2];
        *buffer++ = cDigitsLut[d2 + 1];
        *buffer++ = cDigitsLut[d3];
        *buffer++ = cDigitsLut[d3 + 1];
        *buffer++ = cDigitsLut[d4];
        *buffer++ = cDigitsLut[d4 + 1];
    }
    return buffer;
}

void dequeTest()
{
    STL::deque<int> de(10, 7);
    printf("deque size %d\n", de.size());
    for (int i : de) {
        printf(" %d ", i);
    }
    printf("\n");

    for (int i = 0; i < 10; ++i) {
        de[i] = i;
    }
    for (int i : de) {
        printf(" %d ", i);
    }
    printf("\n");

    de.push_back(99);
    de.push_back(999);
    de.push_back(9999);
    printf("deque size %d\n", de.size());
    for (int i : de) {
        printf(" %d ", i);
    }
    printf("\n");

    de.push_front(11);
    de.push_front(111);
    de.push_front(1111);
    printf("deque size %d\n", de.size());
    for (int i : de) {
        printf(" %d ", i);
    }
    printf("\n");
    de.pop_back();
    de.pop_front();
    printf("deque size %d\n", de.size());
    for (int i : de) {
        printf(" %d ", i);
    }
    printf("\n");

    deque<int>::iterator iter;
    iter = find(de.begin(), de.end(), 99);
    printf("%d %d\n", *iter, *(iter.cur));

    de.erase(iter);
    printf("deque size %d\n", de.size());
    for (iter = de.begin(); iter != de.end(); ++iter) {
        printf(" %d ", *iter);
    }
    printf("\n");

    de.clear();
    printf("deque size %d\n", de.size());
}

void heapTest()
{
    int arr[10] = {68, 31, 65, 21, 24, 32, 26, 19, 16, 13};
    vector<int> test(arr, arr + 10);

    for (int i : test) {
        std::cout<< i << ",";
    }

    std::cout<<std::endl;
    test.push_back(50);
    push_heap(test.begin(), test.end());
    for (int i : test) {
        std::cout<< i <<",";
    }
    std::cout<<std::endl;
    pop_heap(test.begin(), test.end());

    test.pop_back();
    for (int i : test) {
        std::cout<< i <<",";
    }
    std::cout<<std::endl;

    sort_heap(test.begin(), test.end());
    for (int i : test) {
        std::cout<< i <<",";
    }
    std::cout<<std::endl;
}

#include <linux/mempolicy.h>

class A
{
public:
    A():a(nullptr){}
    void test(){
        std::cout<<"test"<<std::endl;
    }
    A *a;
};

int main()
{

//    int n = 5;
//    test(__lg(39) * 2, 1, 40);

    int arr[40] = {3, 1, 25, 10, 40, 17, 21, 28, 19, 11, 36, 33, 39, 8, 16, 22, 20, 2, 27, 13, 18, 9, 35, 7, 38, 12, 23, 24, 31, 6, 34, 32, 15, 4, 30, 14, 5, 26, 29, 37};
//    STL::vector<int> vec(arr, arr+40);
//    STL::sort(vec.begin(), vec.end());
//    std::cout <<"size : "<< vec.size() <<std::endl;
//    STL::for_each(vec.begin(), vec.end(), display<int>());

    //vectorTest2();
//    treeTest();
//    setTest();
//    mapTest();
//    hashtableTest();
    //vectorTest();
//    GraphMtxTest();
//    GraphMtxDFS();
//    GraphInkDFS();
//    GraphInkBFS();
//    dequeTest();
//    heapTest();

//    std::vector<int , allocator<int> > vec = {1, 2, 3, 4, 5};

//    STL::for_each(vec.begin(), vec.end(), display<int>());


//    STL::for_each(vec.begin(), vec.end(), display<int>());

}

