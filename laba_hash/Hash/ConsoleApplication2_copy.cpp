#include <iostream>

#include <vector>
#include <string>
#include <set>

#include <algorithm>

#include <fstream>

const std::string separator = "-----------------------------------------";


#include <iterator>


size_t HashFunction(const std::string& str, size_t TableSize, const size_t key) {
    size_t HashResult = 0;
    for (size_t i = 0; str[i] != 0; i++) {
        HashResult = (key * HashResult + str[i]) % TableSize;
    }
    
    HashResult = (2 * HashResult + 1) % TableSize;
    return HashResult;
}

struct HashFunctionVersion1 {
    size_t operator()(const std::string& str, size_t tableSize) const {
        return HashFunction(str, tableSize, tableSize - 1);
    }
};

struct HashFunctionVersion2 {
    size_t operator()(const std::string& str, size_t tableSize) const {
        return HashFunction(str, tableSize, tableSize + 1);
    }
};

template<typename BidirectionalIterator, typename Compare>
void quickSort(BidirectionalIterator first, BidirectionalIterator last, Compare cmp) {
    if (first != last) {
        BidirectionalIterator left = first;
        BidirectionalIterator right = last;
        BidirectionalIterator pivot = left++;
        while (left != right) {
            if (cmp(*left, *pivot)) {
                ++left;
            }
            else {
                while ((left != --right) && cmp(*pivot, *right));
                std::iter_swap(left, right);
            }
        }
        --left;
        std::iter_swap(first, left);
        quickSort(first, left, cmp);
        quickSort(right, last, cmp);
    }
}

template<typename BidirectionalIterator>
inline void quickSort(BidirectionalIterator first, BidirectionalIterator last) {
    quickSort(first, last,
        std::less_equal< typename std::iterator_traits<BidirectionalIterator>::value_type >()
    );
}

template <typename ftype>
bool CheckForSort(ftype* operand1, ftype* operand2) {
    if (operand1 && operand2) {
        if (operand1->state && operand2->state) {
            std::string a = operand1->value, b = operand2->value;
            std::transform(a.begin(), a.end(), a.begin(), std::tolower);
            std::transform(b.begin(), b.end(), b.begin(), std::tolower);
            return (a < b);
        }
    }
    return false;
}

template <typename ftype>
bool CheckForSortPages(ftype* operand1, ftype* operand2) {
    if (operand1 && operand2) {
        if (operand1->state && operand2->state) {
            size_t a = *operand1->pages.begin(), b = *operand2->pages.begin();
            return (a < b);
        }
    }
    return false;
}

template <class T, class THash1 = HashFunctionVersion1, class THash2 = HashFunctionVersion2>
class HashTable {
    static const size_t defaultSize = 16;
    constexpr static const double coefToResize = 0.7;
    size_t numOfElements;
    size_t numOfElementsWithDeleted;
    struct node {
        std::string value;
        bool state;
        HashTable<std::string, THash1, THash2> subterms;
        std::set<size_t> pages;
        node(const std::string& _value, const size_t _page, const bool _state = true) :value(_value), state(_state) {
            pages.insert(_page);
        }
        node(const std::string& _value, const std::set<size_t> _pages, const bool _state = true) :value(_value), state(_state), pages(_pages) {}
        node(const node* _node): value(_node->value), state(_node->state), subterms(_node->subterms), pages(_node->pages){}
    };
    std::vector<node*> nodesArray;
    void Resize() {
        numOfElements = 0;
        numOfElementsWithDeleted = 0;
        size_t oldSize = nodesArray.size();
        std::vector<node*> buffNodesArray = nodesArray;
        nodesArray.resize(buffNodesArray.size() * 2);
        for (auto& element : nodesArray) {
            element = nullptr;
        }
        for (auto& element : buffNodesArray) {
            if (element && element->state) {
                Add(element->value, element->pages);
                for (auto& subelement : element->subterms.nodesArray) {
                    if (subelement && subelement->state) {
                        AddSubterm(element->value, subelement->value, subelement->pages);
                        for (auto& subelement2 : subelement->subterms.nodesArray) {
                            if (subelement2 && subelement2->state) {
                                AddSubterm(element->value, subelement->value, subelement2->value, subelement2->pages);
                            }
                        }
                    }
                }
            }
        }
            
    }
    void Rehash() {
        numOfElements = 0;
        numOfElementsWithDeleted = 0;
        std::vector<node*> buffNodesArray = nodesArray;
        for (auto& element : nodesArray) {
            element = nullptr;
        }
        for (auto& element : buffNodesArray) {
            if (element && element->state) {
                Add(element->value, element->pages);
                for (auto& subelement : element->subterms.nodesArray) {   
                    if (subelement && subelement->state) {
                        AddSubterm(element->value, subelement->value, subelement->pages);
                        for (auto& subelement2 : subelement->subterms.nodesArray) {
                            if (subelement2 && subelement2->state) {
                                AddSubterm(element->value, subelement->value, subelement2->value, subelement2->pages);
                            }
                        }
                    }
                }
            }
        }
    }

public:

    HashTable() {
        numOfElements = 0;
        numOfElementsWithDeleted = 0;
        nodesArray.resize(defaultSize);
    }
    ~HashTable() {
        nodesArray.clear();
    }
    
    friend std::fstream& operator << (std::fstream& o, const HashTable& table) {
        o << table.numOfElements << " " << table.nodesArray.size() << '\n';
        //table.numOfElementsWithDeleted = table.numOfElements;
        for (auto& element : table.nodesArray) {
            if (element && element->state) {
                o << element->value << " " << element->state << " "
                  << element->subterms.numOfElements
                  << " " << element->subterms.nodesArray.size() << " " << element->pages.size() << '\n';
                //element->subterms.numOfElementsWithDeleted = element->subterms.numOfElements;
                for (size_t page : element->pages) {
                    o  << page << '\n';
                }
                for (auto& subelement : element->subterms.nodesArray) {
                    if (subelement && subelement->state) {
                        o << '\t';
                        o << subelement->value << " " << subelement->state << " "
                          << subelement->subterms.numOfElements
                          << " " << subelement->subterms.nodesArray.size() << " " << subelement->pages.size() << '\n';
                        //subelement->subterms.numOfElementsWithDeleted = subelement->subterms.numOfElements;
                        for (size_t page : subelement->pages) {
                            o << '\t' << page << '\n';
                        }
                        for (auto& subelement2: subelement->subterms.nodesArray) {
                            if (subelement2 && subelement2->state) {
                                o << "\t\t";
                                o << subelement2->value << " " << subelement2->state << " "
                                  << subelement2->subterms.numOfElements
                                  << " " << subelement2->subterms.nodesArray.size() << " " << subelement2->pages.size() << '\n';
                                //subelement2->subterms.numOfElementsWithDeleted = subelement2->subterms.numOfElements;
                                for (size_t page : subelement2->pages) {
                                    o << "\t\t" << page << '\n';
                                }
                            }
                        }
                    }
                }
            }
        }
        return o;
    }
    friend std::fstream& operator >> (std::fstream& is, HashTable& table) {
        size_t size, pagesSize, page;
        table.nodesArray.clear();
        is >> table.numOfElements >> size;
        table.numOfElementsWithDeleted = table.numOfElements;
        table.nodesArray.resize(size);
        for (size_t i = 0; i < table.numOfElements; i++) {
            auto& element = table.nodesArray.at(i);  
            size_t NOE;
            std::string VAL;
            bool S;
            is >> VAL >> S >> NOE  >> size >> pagesSize >> page;
            element = new node(VAL, page, S);
            element->subterms.numOfElements = NOE;
            element->subterms.numOfElementsWithDeleted = NOE;
            element->subterms.nodesArray.resize(size);
            for (size_t n = 0; n < pagesSize-1; n++) {
                is >> page;
                element->pages.insert(page);
            }
            for (size_t j = 0; j < element->subterms.numOfElements; j++) {
                auto& subelement = element->subterms.nodesArray.at(j);
                is >> VAL >> S >> NOE >> size >> pagesSize >> page;
                subelement = new node(VAL, page, S);
                subelement->subterms.numOfElements = NOE;
                subelement->subterms.numOfElementsWithDeleted = NOE;
                subelement->subterms.nodesArray.resize(size);
                for (size_t n = 0; n < pagesSize-1; n++) {
                    is >> page;
                    subelement->pages.insert(page);
                }
                for (size_t k = 0; k < subelement->subterms.numOfElements; k++) {
                    auto& subelement2 = subelement->subterms.nodesArray.at(k);
                    is >> VAL >> S >> NOE >> size >> pagesSize >> page;
                    subelement2 = new node(VAL, page, S);
                    subelement2->subterms.numOfElements = NOE;
                    subelement2->subterms.numOfElementsWithDeleted = NOE;
                    subelement2->subterms.nodesArray.resize(size);
                    for (size_t n = 0; n < pagesSize-1; n++) {
                        is >> page;
                        subelement2->pages.insert(page);
                    }
                }
            }
        }
        table.Rehash();
        return is;       
    }

    node* getNode(const T& term) {
        size_t pos = Search(term);
        return pos == -1 ? nullptr
                         : nodesArray.at(pos);
    }

    void ShowSorted() {
        std::vector<node*> buff = nodesArray; 
        quickSort(nodesArray.begin(), nodesArray.end(), &CheckForSort<node>);
        for (auto& element : nodesArray) {
            if (element) {
                quickSort(element->subterms.nodesArray.begin(), element->subterms.nodesArray.end(),
                                            &CheckForSort<node>);
                for (auto& subelement : element->subterms.nodesArray) {
                    if (subelement)
                       quickSort(subelement->subterms.nodesArray.begin(), subelement->subterms.nodesArray.end(),
                                                       &CheckForSort<node>);
                }
            }
        }
        Show();
        nodesArray = buff;
    }
    void ShowSortedPages() {
        std::vector<node*> buff = nodesArray;
        quickSort(nodesArray.begin(), nodesArray.end(), &CheckForSortPages<node>);
        for (auto& element : nodesArray) {
            if (element) {
                quickSort(element->subterms.nodesArray.begin(), element->subterms.nodesArray.end(),
                    &CheckForSortPages<node>);
                for (auto& subelement : element->subterms.nodesArray) {
                    if (subelement)
                        quickSort(subelement->subterms.nodesArray.begin(), subelement->subterms.nodesArray.end(),
                            &CheckForSortPages<node>);
                }
            }
        }
        Show();
        nodesArray = buff;
    }

    void Show(){
        std::cout << separator << std::endl;
        bool isAnyElement = false;
        for (auto& element : nodesArray) {
            if (element && element->state) {
                isAnyElement = true;             
                std::cout << "Term: " << element->value;  
                for (size_t page : element->pages) {
                    std::cout << " " << page;
                }
                std::cout << std::endl;
                for (auto& subelement : element->subterms.nodesArray) {
                    if (subelement && subelement->state) {                        
                        std::cout << "\tSubterm(lvl 1): " << subelement->value;
                        for (size_t page : subelement->pages) {
                            std::cout << " " << page;
                        }
                        std::cout << std::endl;
                        for (auto& subelement2 : subelement->subterms.nodesArray) {
                            if (subelement2 && subelement2->state) {
                                std::cout << "\t\tSubterm(lvl 2): " << subelement2->value;
                                for (size_t page : subelement2->pages) {
                                    std::cout << " " << page;
                                }
                                std::cout << std::endl;
                            }
                        }
                    }
                }
            }
        }
        if (!isAnyElement) {
            std::cout << "TABLE IS EMPTY" << std::endl;
        }
        std::cout << separator << std::endl;
    }

    bool Add(const node* newNode, const THash1 hash1 = THash1(), const THash2 hash2 = THash2()) {
        if (numOfElements > coefToResize * nodesArray.size() - 1)
            Resize();
        else if (numOfElementsWithDeleted > numOfElements * 2)
            Rehash();
        size_t h1 = hash1(newNode->value, nodesArray.size());
        size_t h2 = hash2(newNode->value, nodesArray.size());
        size_t i = 0;
        int firstDeleted = -1;
        while (nodesArray.at(h1) != nullptr && i < nodesArray.size())
        {
            if (nodesArray.at(h1)->value == newNode->value && nodesArray.at(h1)->state) {
                return false;
            }
            if (!nodesArray.at(h1)->state && firstDeleted == -1)
                firstDeleted = h1;
            h1 = (h1 + h2) % nodesArray.size();
            ++i;
        }
        if (firstDeleted == -1) {
            nodesArray.at(h1) = new node(newNode);
            ++numOfElementsWithDeleted;
        }
        else {
            nodesArray.at(firstDeleted) = new node(newNode);
        }
        ++numOfElements;
        return true;
    }

    bool Add(const T& value, const size_t page, const THash1 hash1 = THash1(), const THash2 hash2 = THash2()) {
        if (numOfElements > coefToResize * nodesArray.size() - 1)
            Resize();
        else if (numOfElementsWithDeleted > numOfElements*2)
            Rehash();
        size_t h1 = hash1(value, nodesArray.size());
        size_t h2 = hash2(value, nodesArray.size());
        size_t i = 0;
        int firstDeleted = -1;
        while (nodesArray.at(h1) != nullptr && i < nodesArray.size())
        {
            if (nodesArray.at(h1)->value == value && nodesArray.at(h1)->state) {
                if (nodesArray.at(h1)->pages.find(page) == nodesArray.at(h1)->pages.end()) {
                    nodesArray.at(h1)->pages.insert(page);
                    return true;
                }
                return false;
            }
            if (!nodesArray.at(h1)->state && firstDeleted == -1) 
                firstDeleted = h1;
            h1 = (h1 + h2) % nodesArray.size();
            ++i;
        }
        if (firstDeleted == -1) {
            nodesArray.at(h1) = new node(value, page);
            ++numOfElementsWithDeleted;
        }
        else {
            nodesArray.at(firstDeleted)->value = value;
            nodesArray.at(firstDeleted)->state = true;
            nodesArray.at(firstDeleted)->pages.insert(page);
        }
        ++numOfElements;
        return true;
    }
    bool Add(const T& value, const std::set<size_t> pages, const THash1 hash1 = THash1(), const THash2 hash2 = THash2()) {
        if (numOfElements > coefToResize * nodesArray.size() - 1)
            Resize();
        else if (numOfElementsWithDeleted > numOfElements * 2)
            Rehash();
        size_t h1 = hash1(value, nodesArray.size());
        size_t h2 = hash2(value, nodesArray.size());
        size_t i = 0;
        int firstDeleted = -1;
        while (nodesArray.at(h1) != nullptr && i < nodesArray.size())
        {
            if (nodesArray.at(h1)->value == value && nodesArray.at(h1)->state)
                return false;
            if (!nodesArray.at(h1)->state && firstDeleted == -1)
                firstDeleted = h1;
            h1 = (h1 + h2) % nodesArray.size();
            ++i;
        }
        if (firstDeleted == -1)
        {
            nodesArray.at(h1) = new node(value, pages);
            ++numOfElementsWithDeleted;
        }
        else
        {
            nodesArray.at(firstDeleted)->value = value;           
            nodesArray.at(firstDeleted)->state = true;
            nodesArray.at(firstDeleted)->pages = pages;
        }
        ++numOfElements;
        return true;
    }

    bool AddSubterm(const T& value, const T& addValue, const size_t addValuePage) {
        node* term = getNode(value);
        if (term) {
            return term->subterms.Add(addValue, addValuePage);
            //node* subterm = term->subterms.getNode(addValue);
        }
        return false;
    }
    bool AddSubterm(const T& value, const T& addValue, const std::set<size_t> addValuePages) {
        node* term = getNode(value);
        if (term) {
            return term->subterms.Add(addValue, addValuePages);
            //node* subterm = term->subterms.getNode(addValue);
             ;
        }
        return false;
    }

    bool AddSubterm(const T& value, const T& subValue, const T& addValue, const size_t addValuePage) {
        node* term = getNode(value);
        if (term && term->state) {
            node* subterm = term->subterms.getNode(subValue);
            if (subterm && subterm->state) {
                return subterm->subterms.Add(addValue, addValuePage);
            }
        }
        return false;
    }
    bool AddSubterm(const T& value, const T& subValue, const T& addValue, const std::set<size_t> addValuePages) {
        node* term = getNode(value);
        if (term && term->state) {
            node* subterm = term->subterms.getNode(subValue);
            if (subterm && subterm->state) {
                return subterm->subterms.Add(addValue, addValuePages);             
            }
        }
        return false;
    }

    size_t Search(const T& value, const THash1 hash1 = THash1(), const THash2 hash2 = THash2()) {
        size_t h1 = hash1(value, nodesArray.size());
        size_t h2 = hash2(value, nodesArray.size());
        size_t i = 0;
        while (nodesArray.at(h1) != nullptr && i < nodesArray.size())
        {
            if (nodesArray.at(h1)->value == value && nodesArray.at(h1)->state) {
                return h1;
            }
            h1 = (h1 + h2) % nodesArray.size();
            ++i;
        }
        return -1;
    }

    bool SearchByTerm(const T& value) {
        node* term = getNode(value);
        bool isAnyFound = false;
        std::cout << separator << std::endl;
        //std::cout << "Term: " << value << std::endl;
        if (term && term->state) {
            for (auto& element : term->subterms.nodesArray) {
                if (element && element->state) {
                    std::cout << "Subterm(lvl 1): " << element->value << std::endl;
                    isAnyFound = true;
                }
            }      
        }
        if (!isAnyFound) {
            std::cout << "NO SUBTERMS FOUND" << std::endl;;
        }
        std::cout << separator << std::endl;
        return isAnyFound;
    }
    bool SearchByTermLow(const T& subValue) {
        //node* term = getNode(value);
        bool isAnyFound = false;
        std::cout << separator << std::endl;
        //std::cout << "Subterm(lvl 1): " << subValue << std::endl;
        for (auto& element : nodesArray) {
            if (element && element->state) {               
                node* subterm = element->subterms.getNode(subValue);
                if (subterm && subterm->state && subterm->value == subValue) {
                    std::cout << separator << std::endl;
                    for (auto& subelement : subterm->subterms.nodesArray) {
                        if (subelement && element->state) {
                            std::cout << "Subterm(lvl 2): " << subelement->value << std::endl;
                            isAnyFound = true;
                        }
                    }
                }
            }
        }
        if (!isAnyFound) {
            std::cout << "NO SUBTERMS FOUND" << std::endl;
        }
        std::cout << separator << std::endl;
        return isAnyFound;
    }

    bool SearchBySubterm(const T& subValue, const size_t subLevel) {
        bool isAnyFound = false;
        std::cout << separator << std::endl;
        switch (subLevel)
        {
        case(1): {
            //std::cout << "Subterm(lvl 1): " << subValue << std::endl;
            for (auto& element : nodesArray) {
                if (element && element->state) {
                    if (element->subterms.getNode(subValue)) {
                        std::cout << "Term: " << element->value << std::endl;
                        isAnyFound = true;
                    }
                }
            }
            break;
        }
        case(2): {
            //std::cout << "Subterm(lvl 2): " << subValue << std::endl;
            for (auto& element : nodesArray) {
                if (element && element->state) {
                    for (auto& subelement : element->subterms.nodesArray) {
                        if (subelement && subelement->state) {
                            if (subelement->subterms.getNode(subValue)) {
                                std::cout << "Term: " << element->value << ", subterm(lvl 1): " << subelement->value << std::endl;
                                isAnyFound = true;
                            }
                        }
                    }
                }
            }
            break;
        }
        default: {
            std::cout << "Incorrect subterm level" << std::endl;
            break;
        }
        }
        if (!isAnyFound) {
            std::cout << "NO TERMS FOUND" << std::endl;
        }
        std::cout << separator << std::endl;
        return isAnyFound;
    }


    bool Change(const T& oldValue, const T& newValue) {
        node *buff = getNode(oldValue);
        if (buff && !getNode(newValue)) {
            node* newNode = buff;
            newNode->value = newValue;
            Delete(oldValue);
            Add(newNode);
            Rehash();
            return true;
        }
        return false;
    }

    bool ChangeSubterm(const T& oldValueTerm, const T& oldValue, const T& newValue){
        node* buff = getNode(oldValueTerm);
        if (buff) {
            return buff->subterms.Change(oldValue, newValue);
        }
        return false;
    }

    bool ChangeSubterm(const T& oldValueTerm, const T& oldValueSubterm, const T& oldValue, const T& newValue) {
        node* buff = getNode(oldValueTerm);
        if (buff) {
            return buff->subterms.ChangeSubterm(oldValueSubterm, oldValue, newValue);
        }
        return false;
    }

    bool ChangePage(const T& oldValue, const size_t oldPage, const size_t newPage) {
        node* buff = getNode(oldValue);
        if (buff) {
            if (buff->pages.find(newPage) == buff->pages.end() && oldPage != newPage) {
                buff->pages.insert(newPage);
                buff->pages.erase(oldPage);
                //Rehash();
                return true;
            }
            //return false;
        }
        return false;
    }

    bool ChangeSubtermPage(const T& oldValueTerm, const T& oldValue, const size_t oldPage, const size_t newPage) {
        node* buff = getNode(oldValueTerm);
        if (buff) {
            return buff->subterms.ChangePage(oldValue, oldPage, newPage);
        }
        return false;
    }

    bool ChangeSubtermPage(const T& oldValueTerm, const T& oldValueSubterm, const T& oldValue, const size_t oldPage, const size_t newPage) {
        node* buff = getNode(oldValueTerm);
        if (buff) {
            return buff->subterms.ChangeSubtermPage(oldValueSubterm, oldValue, oldPage, newPage);
        }
        return false;
    }


    bool Delete(const T& value, const THash1 hash1 = THash1(), const THash2 hash2 = THash2()) {
        size_t h1 = hash1(value, nodesArray.size());
        size_t h2 = hash2(value, nodesArray.size());
        size_t i = 0;
        while (nodesArray.at(h1) != nullptr && i < nodesArray.size())
        {
            if (nodesArray.at(h1)->value == value && nodesArray.at(h1)->state) {
                nodesArray.at(h1)->state = false;
                nodesArray.at(h1)->pages.clear();
                nodesArray.at(h1)->subterms.nodesArray.clear();
                --numOfElements;
                return true;
            }
            h1 = (h1 + h2) % nodesArray.size();
            ++i;
        }
        return false;
    }

    bool DeletePage(const T& value, const size_t page, const THash1 hash1 = THash1(), const THash2 hash2 = THash2()) {
        size_t h1 = hash1(value, nodesArray.size());
        size_t h2 = hash2(value, nodesArray.size());
        size_t i = 0;
        while (nodesArray.at(h1) != nullptr && i < nodesArray.size())
        {
            if (nodesArray.at(h1)->value == value && nodesArray.at(h1)->state
                && nodesArray.at(h1)->pages.find(page) != nodesArray.at(h1)->pages.end()) {
                nodesArray.at(h1)->pages.erase(page);
                if (nodesArray.at(h1)->pages.empty()) {
                    nodesArray.at(h1)->state = false;
                    --numOfElements;
                }
                return true;
            }
            h1 = (h1 + h2) % nodesArray.size();
            ++i;
        }
        return false;
    }

    bool DeleteSubterm(const T& value, const T& deleteValue) {
        node* term = getNode(value);
        if (term) {
            return term->subterms.Delete(deleteValue);
        }
        return false;
    }

    bool DeleteSubtermPage(const T& value, const T& deleteValue, const size_t deletePage) {
        node* term = getNode(value);
        if (term) {
            return term->subterms.DeletePage(deleteValue, deletePage);
        }
        return false;
    }


    bool DeleteSubterm(const T& value, const T& subValue, const T& deleteValue) {
        node* term = getNode(value);
        if (term) {
            node* subterm = term->subterms.getNode(subValue);
            if (subterm) {
                return subterm->subterms.Delete(deleteValue);
            }
        }
        return false;
    }

    bool DeleteSubtermPage(const T& value, const T& subValue, const T& deleteValue, const size_t deletePage) {
        node* term = getNode(value);
        if (term) {
            node* subterm = term->subterms.getNode(subValue);
            if (subterm) {
                return subterm->subterms.DeletePage(deleteValue, deletePage);
            }
        }
        return false;
    }
};


void menu(){
    HashTable<std::string> table;
    char input;
    bool isExit = false, isExitInternal = false;
    while (!isExit) {
        isExitInternal = false;
        std::cout << "Choose action:\n1 - Add\n2 - Change\n3 - Delete\n4 - Search\n5 - Show sorted\n" 
                  << "6 - Show\no - Open from file\ns - Save to file\ne - Exit\n" << std::endl;
        std::cin >> input;
        std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
        switch (input)
        {
        case('o'): {
            std::fstream fin("text.txt", std::fstream::in);
            if (fin.is_open()) {
                fin >> table;
                std::cout << "Successfully opened\n" << std::endl;
            }
            else
                std::cout << "File open error\n" << std::endl;
            fin.close();
            break;
        }
        case('s'): {
            std::fstream fout("text.txt", std::fstream::out);
            if (fout.is_open()) {
                fout << table;
                std::cout << "Successfully saved\n" << std::endl;
            }
            else
                std::cout << "File save error\n" << std::endl;
            fout.close();
            break;
        }
        case('1'): {
            while (!isExitInternal) {
                char input2;
                std::cout << "Choose variant:\n1 - Add term(or new page)\n2 - Add first-level subterm(or new page)" <<
                             "\n3 - Add second-level subterm(or new page)\nb - Go back\n" << std::endl;
                std::cin >> input2;
                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                switch (input2) {
                case('1'): {
                    std::string addValue;
                    size_t addPage;
                    std::cout << "Enter aditting value and page: ";
                    std::cin >> addValue >> addPage;
                    std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                    if (table.Add(addValue, addPage))
                        std::cout << "Added successfully\n" << std::endl;
                    else
                        std::cout << "Addition failed\n" << std::endl;
                    break;
                }
                case('2'): {
                    std::string term, addValue;
                    size_t addPage;
                    std::cout << "Enter term: ";
                    std::cin >> term;
                    std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                    std::cout << "\nEnter aditting value and page: ";
                    std::cin >> addValue >> addPage;
                    std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                    if (table.AddSubterm(term, addValue, addPage))
                        std::cout << "Added successfully\n" << std::endl;
                    else
                        std::cout << "Addition failed\n" << std::endl;
                    break;
                }
                case('3'): {
                    std::string term, subterm, addValue;
                    size_t addPage;
                    std::cout << "Enter term and subterm: ";
                    std::cin >> term >> subterm;
                    std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                    std::cout << "\nEnter aditting value and page: ";
                    std::cin >> addValue >> addPage;
                    std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                    if (table.AddSubterm(term, subterm, addValue, addPage))
                        std::cout << "Added successfully\n" << std::endl;
                    else
                        std::cout << "Addition failed\n" << std::endl;
                    break;
                }
                case('b'): {
                    isExitInternal = true;
                    break;
                }
                }
            }
            break;
        }
        case('2'): {
            while (!isExitInternal) {
                char input2;
                bool isExitInternal2 = false;
                std::cout << "Choose variant:\n1 - Change term/subterm\n2 - Change page\nb - Go back\n" << std::endl;
                std::cin >> input2;
                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                switch (input2) {
                case('1'): {
                    char input3;
                    while (!isExitInternal2) {
                        std::cout << "Choose variant:\n1 - Change term\n2 - Change first-level subterm\n3 - Change second-level subterm\nb - Go back\n" << std::endl;
                        std::cin >> input3;
                        std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                        switch (input3) {
                        case('1'): {
                            std::string oldTerm, newTerm;
                            std::cout << "Enter old term value: ";
                            std::cin >> oldTerm;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            std::cout << "Enter new value: ";
                            std::cin >> newTerm;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            if (table.Change(oldTerm, newTerm))
                                std::cout << "Changed successfully\n" << std::endl;
                            else
                                std::cout << "Change failed\n" << std::endl;
                            break;
                        }
                        case('2'): {
                            std::string oldTerm, oldSubterm, newSubterm;
                            std::cout << "Enter term and old value: ";
                            std::cin >> oldTerm >> oldSubterm;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            std::cout << "Enter new value: ";
                            std::cin >> newSubterm;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            if (table.ChangeSubterm(oldTerm, oldSubterm, newSubterm))
                                std::cout << "Changed successfully\n" << std::endl;
                            else
                                std::cout << "Change failed\n" << std::endl;
                            break;
                        }
                        case('3'): {
                            std::string oldTerm, oldSubterm, oldSubterm2, newSubterm2;
                            std::cout << "Enter term, first-level subterm and old value: ";
                            std::cin >> oldTerm >> oldSubterm >> oldSubterm2;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            std::cout << "Enter new value: ";
                            std::cin >> newSubterm2;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            if (table.ChangeSubterm(oldTerm, oldSubterm, oldSubterm2, newSubterm2))
                                std::cout << "Changed successfully\n" << std::endl;
                            else
                                std::cout << "Change failed\n" << std::endl;
                            break;
                        }
                        case('b'):
                            isExitInternal2 = true;
                            break;
                        }
                        }
                    }
                    break;
                case('2'): {
                    char input3; 
                    while (!isExitInternal2) {
                        std::cout << "Choose variant:\n1 - Change term page\n2 - Change first-level subterm page\n"
                            << "3 - Change second-level subterm page\nb - Go back\n" << std::endl;
                        std::cin >> input3;
                        std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                        switch (input3) {
                        case('1'): {
                            std::string oldTerm;
                            size_t oldPage, newPage;
                            std::cout << "Enter old term value and old page: ";
                            std::cin >> oldTerm >> oldPage;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            std::cout << "Enter new page: ";
                            std::cin >> newPage;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            if (table.ChangePage(oldTerm, oldPage, newPage))
                                std::cout << "Changed successfully\n" << std::endl;
                            else
                                std::cout << "Change failed\n" << std::endl;
                            break;
                        }
                        case('2'): {
                            std::string oldTerm, oldSubterm;
                            size_t oldPage, newPage;
                            std::cout << "Enter term, subterm and old page: ";
                            std::cin >> oldTerm >> oldSubterm >> oldPage;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            std::cout << "Enter new page: ";
                            std::cin >> newPage;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            if (table.ChangeSubtermPage(oldTerm, oldSubterm, oldPage, newPage))
                                std::cout << "Changed successfully\n" << std::endl;
                            else
                                std::cout << "Change failed\n" << std::endl;
                            break;
                        }
                        case('3'): {
                            std::string oldTerm, oldSubterm, oldSubterm2;
                            size_t oldPage, newPage;
                            std::cout << "Enter term, first-level subterm, second-level subterm and old page: ";
                            std::cin >> oldTerm >> oldSubterm >> oldSubterm2 >> oldPage;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            std::cout << "Enter new page: ";
                            std::cin >> newPage;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            if (table.ChangeSubtermPage(oldTerm, oldSubterm, oldSubterm2, oldPage, newPage))
                                std::cout << "Changed successfully\n" << std::endl;
                            else
                                std::cout << "Change failed\n" << std::endl;
                            break;
                        }
                        case('b'): {
                            isExitInternal2 = true;
                            break;
                        }
                        }
                    }
                    break;
                }
                case('b'): {
                    isExitInternal = true;
                    break;
                }
                }           
            }
            break;
        }
        case('3'): {
            while (!isExitInternal) {
                char input2;
                bool isExitInternal2 = false;
                std::cout << "Choose variant:\n1 - Delete term/subterm\n2 - Delete page\nb - Go back\n" << std::endl;
                std::cin >> input2;
                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                switch (input2) {
                case('1'): {
                    char input3;
                    while (!isExitInternal2) {
                        std::cout << "Choose variant:\n1 - Delete term\n2 - Delete first-level subterm\n3 - Delete second-level subterm\nb - Go back\n" << std::endl;
                        std::cin >> input3;
                        std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                        switch (input3) {
                        case('1'): {
                            std::string deleteValue;
                            std::cout << "Enter delete value: ";
                            std::cin >> deleteValue;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            if (table.Delete(deleteValue))
                                std::cout << "Deleted successfully\n" << std::endl;
                            else
                                std::cout << "Deletion failed\n" << std::endl;
                            break;
                        }
                        case('2'): {
                            std::string term, deleteValue;
                            std::cout << "Enter term: ";
                            std::cin >> term;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            std::cout << "Enter delete value: ";
                            std::cin >> deleteValue;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            if (table.DeleteSubterm(term, deleteValue))
                                std::cout << "Deleted successfully\n" << std::endl;
                            else
                                std::cout << "Deletion failed\n" << std::endl;
                            break;
                        }
                        case('3'): {
                            std::string term, subterm, deleteValue;
                            std::cout << "Enter term and subterm: ";
                            std::cin >> term >> subterm;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            std::cout << "Enter delete value: ";
                            std::cin >> deleteValue;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            if (table.DeleteSubterm(term, subterm, deleteValue))
                                std::cout << "Deleted successfully\n" << std::endl;
                            else
                                std::cout << "Deletion failed\n" << std::endl;
                            break;
                        }
                        case('b'): {
                            isExitInternal2 = true;
                            break;
                        }
                        }
                    }
                    break;
                }
                case('2'): {
                    char input3;
                    while (!isExitInternal2) {
                        std::cout << "Choose variant:\n1 - Delete term page\n2 - Delete first-level subterm page"
                            << "\n3 - Delete second-level subterm page\nb - Go back\n" << std::endl;
                        std::cin >> input3;
                        std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                        switch (input3) {
                        case('1'): {
                            std::string deleteValue;
                            size_t deletePage;
                            std::cout << "Enter delete value and page: ";
                            std::cin >> deleteValue >> deletePage;;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            if (table.DeletePage(deleteValue, deletePage))
                                std::cout << "Deleted successfully\n" << std::endl;
                            else
                                std::cout << "Deletion failed\n" << std::endl;
                            break;
                        }
                        case('2'): {
                            std::string term, deleteValue;
                            size_t deletePage;
                            std::cout << "Enter term: ";
                            std::cin >> term;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            std::cout << "Enter delete value and page: ";
                            std::cin >> deleteValue >> deletePage;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            if (table.DeleteSubtermPage(term, deleteValue, deletePage))
                                std::cout << "Deleted successfully\n" << std::endl;
                            else
                                std::cout << "Deletion failed\n" << std::endl;
                            break;
                        }
                        case('3'): {
                            std::string term, subterm, deleteValue;
                            size_t deletePage;
                            std::cout << "Enter term and subterm: ";
                            std::cin >> term >> subterm;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            std::cout << "Enter delete value and page: ";
                            std::cin >> deleteValue >> deletePage;;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            if (table.DeleteSubtermPage(term, subterm, deleteValue, deletePage))
                                std::cout << "Deleted successfully\n" << std::endl;
                            else
                                std::cout << "Deletion failed\n" << std::endl;
                            break;
                        }
                        case('b'): {
                            isExitInternal2 = true;
                            break;
                        }
                        }                       
                    }
                    break;
                }
                case('b'):
                    isExitInternal = true;
                    break;
                }
            
            }
            break;
        }
        case('4'): {
            while (!isExitInternal) {
                char input2;
                std::cout << "Choose variant:\n1 - Search subterms by term\n2 - Search terms by subterm\nb - Go back\n" << std::endl;
                std::cin >> input2;
                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                switch (input2) {
                case('1'): {
                    char input3;
                    bool isExitInternal2 = false;
                    while (!isExitInternal2) {
                        std::cout << "Choose variant:\n1 - Search first-level subterms\n2 - Search second-level subterms\nb - Go back\n" << std::endl;
                        std::cin >> input3;
                        std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                        switch (input3) {
                        case('1'): {
                            std::string term;
                            std::cout << "Enter term: ";
                            std::cin >> term;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            table.SearchByTerm(term);
                            break;
                        }
                        case('2'): {
                            std::string subterm;
                            std::cout << "Enter first-level subterm: ";
                            std::cin >> subterm;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            table.SearchByTermLow(subterm);
                            break;
                        }
                        case('b'): {
                            isExitInternal2 = true;
                            break;
                        }
                        }
                    }
                    break;
                }
                case('2'): {
                    char input3;
                    bool isExitInternal2 = false;
                    while (!isExitInternal2) {
                        std::cout << "Choose variant:\n1 - Search terms by first-level subterm\n2 - Search first-level subterms by second-level subterms\nb - Go back\n" << std::endl;
                        std::cin >> input3;
                        std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                        switch (input3) {
                        case('1'): {
                            std::string subterm;
                            std::cout << "Enter first-level subterm: ";
                            std::cin >> subterm;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            table.SearchBySubterm(subterm, 1);
                            break;
                        }
                        case('2'): {
                            std::string subterm;
                            std::cout << "Enter second-level subterms: ";
                            std::cin >> subterm;
                            std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            table.SearchBySubterm(subterm, 2);
                            break;
                        }
                        case('b'): {
                            isExitInternal2 = true;
                            break;
                        }
                        }
                    }
                    break;
                }
                case('b'): {
                    isExitInternal = true;
                    break;
                }
                }
            }
            break;
        }
        case('5'): {
            char input2;
            while (!isExitInternal) {
                std::cout << "Choose variant:\n1 - Show sorted by name\n2 - Show sorted by pages\nb - Go back\n" << std::endl;
                std::cin >> input2;
                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                switch (input2) {
                case('1'): {
                    table.ShowSorted();
                    break;
                }
                case('2'): {
                    table.ShowSortedPages();
                    break;
                }
                case('b'): {
                    isExitInternal = true;
                    break;
                }
                }
            }
            break;
        }
        case('6'): {
            table.Show();
            break;
        }
        case('e'): {
            isExit = true;
            break;
        }
        }
    }
    table.~HashTable();
}

unsigned int main()
{
    menu();
    
    return 0;
}
