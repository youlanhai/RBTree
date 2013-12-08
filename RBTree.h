/**
    红黑树的性质：
    1.一个结点，不是红色就是黑色
    2.根结点必须是黑色
    3.若结点为红色，其子结点必须是黑色（反之，若结点为红，其父结点必须是黑色）
    4.任一结点，到其叶NULL结点（叶结点视为有两个空结点）的每一天路径中，黑色结点数必须相同。
*/
#pragma once

#include <algorithm>

#define DEBUG_RBTREE

///红黑树
namespace rbtree
{

    const bool color_red = true;    ///<红色
    const bool color_black = false; ///<黑色
    
    ///红黑树结点
    template<typename T>
    struct Node
    {
        typedef T value_type;

        Node(const value_type & v)
            : value(v)
            , left(nullptr)
            , right(nullptr)
            , parent(nullptr)
            , color(color_red) //新插入结点的颜色是红的
        {}

        Node *  left;
        Node *  right;
        Node *  parent;
        bool    color;
        value_type value;
    };

    ///红黑树
    template<typename T, typename Pred = std::less<T>>
    class RBTree
    {
    public:
        typedef T       value_type;
        typedef Node<T> node_type;
        typedef Pred    compare_type;

        RBTree()
            : m_root(nullptr)
            , m_compare(compare_type())
        {}

        ~RBTree()
        {
            destry();
        }

        ///树是否为空
        bool empty() const { return nullptr == m_root; }

        ///树的最大深度
        int maxDepth() const{ return maxDepth(m_root); }

        ///销毁树
        void destry()
        {
            destroy(m_root);
            m_root = nullptr;
        }

        ///插入一个值
        void insert(const value_type & v)
        {
            node_type * p = newNode(v);
            if (nullptr == m_root)
            {
                p->color = color_black; //根结点的颜色是黑色的
                m_root = p;
                return;
            }

            node_type * parent = findParent(m_root, v);
            p->parent = parent;
            if (m_compare(v, parent->value)) parent->left = p;
            else parent->right = p;

            balance(p);
        }

        ///遍历树
        template<typename Fun>
        void traverse(Fun f)
        {
            traverse_mid(m_root, f);
        }

        ///遍历树
        template<typename Fun>
        void traverseDebug(Fun f)
        {
            traverse_mid_debug(m_root, f);
        }

    private:

        ///让结点p平衡
        void balance(node_type *p)
        {
            balanceNode(p);
            m_root->color = color_black; //根结点必须是黑色的
        }

        ///让结点p平衡
        void balanceNode(node_type *p);

        ///左旋，以p为中心，将右子逆时针旋转。
        void rotateLeft(node_type * p);

        ///右旋，以p为中心，将左子顺时针旋转。
        void rotateRight(node_type * p);


        ///查找v的父结点。
        node_type * findParent(node_type * p, const value_type & v)
        {
            if (m_compare(v, p->value)) return p->left ? findParent(p->left, v) : p;
            else return p->right ? findParent(p->right, v) : p;
        }

        ///中序遍历
        template<typename Fun>
        void traverse_mid(node_type * p, Fun f)
        {
            if (!p) return;

            traverse_mid(p->left, f);
            f(p->value);
            traverse_mid(p->right, f);
        }

        template<typename Fun>
        void traverse_mid_debug(node_type *p, Fun f)
        {
            if (!p) return;

            traverse_mid_debug(p->left, f);
            f(p);
            traverse_mid_debug(p->right, f);
        }

        int maxDepth(node_type *p) const
        {
            if (!p) return 0;
            return 1 + std::max(maxDepth(p->left), maxDepth(p->right));
        }

        node_type * newNode(const value_type & v)
        {
            return new node_type(v);
        }

        void delNode(node_type * p)
        {
            delete p;
        }

        void destroy(node_type * p)
        {
            if (nullptr == p) return;
            destroy(p->left);
            destroy(p->right);
            delNode(p);
        }

    private:

        node_type * m_root; ///< 根结点
        compare_type m_compare; ///< 比较器
    };

    template<typename T, typename Pred>
    void RBTree<T, Pred>::balanceNode(node_type *p)
    {
        //本来就是平衡的
        if (p == m_root || p->parent->color == color_black) return;

#ifdef DEBUG_RBTREE
        std::cout << "balance: " << p->value << std::endl;
#endif

        node_type * parent = p->parent;
        node_type * grandpa = parent->parent;

        if (parent == grandpa->left)
        {
            node_type * uncle = grandpa->right;
            if (uncle && uncle->color == color_red)
            {
                //将父跟叔结点都改成黑色，这样两端的黑色结点数就相同了。
                parent->color = uncle->color = color_black;

                //对爷爷结点进行判断
                grandpa->color = color_red;
                balanceNode(grandpa);
            }
            else
            {
                if (p == parent->right) //内插，需要先左旋，变成外插模式
                {
                    p = parent;
                    rotateLeft(p);
                }

                p->parent->color = color_black;
                p->parent->parent->color = color_red;
                rotateRight(p->parent->parent);
            }

        }
        else
        {
            node_type * uncle = grandpa->left;
            if (uncle && uncle->color == color_red)
            {
                parent->color = uncle->color = color_black;

                grandpa->color = color_red;
                balanceNode(grandpa);
            }
            else
            {
                if (p == parent->left)//内插，需要右旋
                {
                    p = parent;
                    rotateRight(p);
                }

                p->parent->color = color_black;
                p->parent->parent->color = color_red;
                rotateLeft(p->parent->parent);
            }
        }
    }

    ///左旋，以p为中心，将右子逆时针旋转。
    template<typename T, typename Pred>
    void RBTree<T, Pred>::rotateLeft(node_type * p)
    {
#ifdef DEBUG_RBTREE
        std::cout << "rotate left: (" << p->value << "," << p->right->value << ")" << std::endl;
#endif

        node_type *right = p->right;
        p->right = right->left;
        if (p->right) p->right->parent = p;

        right->parent = p->parent;

        if (m_root == p)
        {
            m_root = right;
        }
        else
        {
            if (p == p->parent->left)
                p->parent->left = right;
            else
                p->parent->right = right;
        }
        right->left = p;
        p->parent = right;
    }

    ///右旋，以p为中心，将左子顺时针旋转。
    template<typename T, typename Pred>
    void RBTree<T, Pred>::rotateRight(node_type * p)
    {
#ifdef DEBUG_RBTREE
        std::cout << "rotate right: (" << p->value << "," << p->left->value << ")" << std::endl;
#endif

        node_type *left = p->left;
        p->left = left->right;
        if (p->left) p->left->parent = p;

        left->parent = p->parent;
        if (p == m_root)
        {
            m_root = left;
        }
        else
        {
            if (p == p->parent->left)
                p->parent->left = left;
            else
                p->parent->right = left;
        }
        left->right = p;
        p->parent = left;
    }

}//end namespace rbtree