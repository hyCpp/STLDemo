#ifndef B_TREE_H
#define B_TREE_H

static const int m = 3;  //定义最小度为3
static const int key_max = 2 * m-1;//关键字最大个数
static const int key_min = m - 1;//关键字最小个数
static const int child_max = key_max + 1;//子节点最大个数(这里可以看出子节点数与关键字数有关)
static const int child_min = key_min + 1;//子节点最小个数

template <typename T>
class BTnode
{
public:
    BTnode()
    {
        m_keyNum = 0;
        m_parent = nullptr;
        m_isleaf = true;
        for (int i = 0; i < child_max; ++i) {
            m_child[i] = nullptr;
        }
        for (int i = 0; i < key_max; ++i) {
            m_keyValue[i] = '\0';
        }
    }

private:
    bool m_isleaf; //判断节点是否是叶子节点
    int m_keyNum; //关键字个数
    BTnode<T>* m_parent; //指向父节点
    BTnode<T>* m_child[child_max]; //子树指针数组
    T m_keyValue[key_max]; //关键字数组
};

template<typename T>
class BTree
{
public:
    BTree()
    {
        m_root = nullptr;
    }

    bool insert(T val)
    {
        if (m_root == nullptr) {
            m_root = new
        }
    }

private:
    BTnode<T>* m_root;
};



#endif // BTREE

