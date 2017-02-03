#include <vector>
#include <unordered_map>
#include <iterator>
#include <functional>


template <typename ElementType, typename Hash=std::hash<ElementType>, typename KeyEqual=std::equal_to<ElementType>>
class DisjointSet {
    struct Node {
        ElementType m_value;
        unsigned m_rank;
        int m_parent;
    };

    std::unordered_map<ElementType, unsigned, Hash, KeyEqual> m_indexes;
    std::vector<Node> m_nodes;
    size_t m_setCount = 0;

    int FindInternal(int nodeIndex) {
        Node node = m_nodes[nodeIndex];

        if (node.m_parent == -1) {
            return nodeIndex;
        } else {
            int parent = FindInternal(node.m_parent);
            m_nodes[nodeIndex].m_parent = parent;
            return parent;
        }
    }

    int GetIndex(const ElementType& val) const {
        auto ret = m_indexes.find(val);
        if (ret != m_indexes.end()) {
            return ret->second;
        } else {
            return -1;
        }
    }

public:

    class DisjointSetElement {
        bool m_isValid;
        ElementType m_value;
        friend class DisjointSet;

    public:
        DisjointSetElement() : m_isValid(false) {}

        bool IsValid() const {
            return m_isValid;
        }

        ElementType Value() const {
            return m_value;
        }
    };

    bool MakeSet(const ElementType& value) {
        if (m_indexes.find(value) != m_indexes.end()) {
            return false;
        } else {
            m_indexes[value] = m_nodes.size();
            Node n;
            n.m_parent = -1;
            n.m_rank = 0;
            n.m_value = value;
            m_nodes.push_back(n);
            m_setCount += 1;
        }
    }

    bool Union(const ElementType& val1, const ElementType& val2) {
        int index1 = GetIndex(val1), index2 = GetIndex(val2);
        if (index1 == -1 || index2 == -1) {
            return false;
        }

        int parent1 = FindInternal(index1);
        int parent2 = FindInternal(index2);

        if (parent1 == parent2) {
            return false;
        }

        Node* p1Node = &m_nodes[parent1];
        Node* p2Node = &m_nodes[parent2];

        if (p1Node->m_rank == p2Node->m_rank) {
            p1Node->m_parent = parent2;
            p2Node->m_rank += 1;
        } else if (p1Node->m_rank < p2Node->m_rank) {
            p1Node->m_parent = parent2;
        } else {
            p2Node->m_parent = parent1;
        }

        m_setCount -= 1;

        return true;
    }

    /*
    DisjointSetElement Find(const ElementType& value) {
        DisjointSetElement ret;
        int index = GetIndex(value);
        if (index == -1) {
            std::cout << "invalid af" << std::endl;
            return ret;
        }
        int parentIndex = FindInternal(index);
        ret.m_value = m_nodes[parentIndex].m_value;
        ret.m_isValid = true;
        return ret;
    }
    */
    const ElementType& Find(const ElementType& value) {
        DisjointSetElement ret;
        int index = GetIndex(value);
        if (index == -1) {
            throw std::runtime_error("Element passed to DisjointSet::Find() does not exist");
        }
        int parentIndex = FindInternal(index);
        return m_nodes[parentIndex].m_value;
    }

    size_t SetCount() const {
        return m_setCount;
    }
};

