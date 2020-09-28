#include <optional>
#include <map>
#include <list>
template <class CharSequence, class UserData, class Compare = std::less<typename CharSequence::value_type>>
class Trie {
    using CharType = typename CharSequence::value_type;
    class Node {
        std::list<UserData> m_userData;
        std::map<CharType, Node, Compare> m_children;
        std::size_t m_fullWordsCount = 0;

    public:
        template<class CharIter>
        bool insert(CharIter begin, CharIter end, const UserData &data) {
            bool ret = true;
            if(begin == end) {
                if(!m_userData.empty())
                   ret = false;
                m_userData.push_back(data);
            } else {
                Node * node = nullptr;
                if(auto f = m_children.find(*begin); f != m_children.end()) {
                    node = &f->second;
                } else {
                    node = &m_children[*begin];
                }
                if(!node->insert(std::next(begin), end, data))
                    ret = false;

            }
            if(ret) m_fullWordsCount ++;
            return ret;
        }
        Node * getChild(CharType ch) {
            if(auto sr = m_children.find(ch); sr != m_children.end()) {
                return &sr->second;
            }
            return nullptr;
        }
        std::size_t WordsCount() {
            return m_fullWordsCount;
        }
        std::list<UserData> GetWordByIndex(std::size_t i) {
            if(!m_userData.empty()) {
                if(i == 0) return m_userData;
                i --;
            }
            for(auto & [_, ch]: m_children) {
                auto wc = ch.WordsCount();
                if(i < wc) {
                    return ch.GetWordByIndex(i);
                } else {
                    i -= wc;
                }
            }
            return {};
        }

    };
    Node m_root;
public:
    bool insert(const CharSequence & word, const UserData & userData) {
        return m_root.insert(word.begin(), word.end(), userData);
    }

    class FilteringResult {
        Node * m_root;
    public:
        std::size_t size() {
            if(!m_root) return 0;
            return m_root->WordsCount();
        }
        FilteringResult(Node * root): m_root(root) {

        }
        std::list<UserData> operator[](std::size_t i) {
            return m_root->GetWordByIndex(i);
        }
    };

    FilteringResult search(const CharSequence & prefix) {
        Node * currentNode = &m_root;
        for(const auto & ch: prefix) {
            currentNode = currentNode->getChild(ch);
            if(!currentNode) break;
        }
        return FilteringResult(currentNode);
    }
};
