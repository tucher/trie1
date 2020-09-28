#include <optional>
#include <map>
template <class CharSequence, class UserData, class Compare = std::less<typename CharSequence::value_type>>
class Trie {
    using CharType = typename CharSequence::value_type;
    class Node {
        std::optional<UserData> m_userData;
        std::map<CharType, Node, Compare> m_children;
        std::size_t m_fullWordsCount = 0;

    public:
        template<class CharIter>
        void insert(CharIter begin, CharIter end, const UserData &data) {
            if(begin == end) {
                m_userData = data;
                m_fullWordsCount = 1;
            } else {
                m_fullWordsCount ++;
                Node * node = nullptr;
                if(auto f = m_children.find(*begin); f != m_children.end()) {
                    node = &f->second;
                } else {
                    node = &m_children[*begin];
                }
                node->insert(std::next(begin), end, data);
            }
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
        std::optional<UserData> GetWordByIndex(std::size_t i) {
            if(m_userData) {
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
    void insert(const CharSequence & word, const UserData & userData) {
        m_root.insert(word.begin(), word.end(), userData);
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
        UserData operator[](std::size_t i) {
            return *m_root->GetWordByIndex(i);
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
