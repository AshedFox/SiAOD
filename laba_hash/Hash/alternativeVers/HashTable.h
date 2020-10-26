#pragma once

#include <iostream>

#include <vector>
#include <string>
#include <set>
#include <list>

#include <algorithm>

#include <fstream>

#include <iterator>

static const std::string separator = "-----------------------------------------";

template <class T, class THash1, class THash2>
class HashTable {
    static const size_t defaultSize = 16; 
    constexpr static const double coefToResize = 0.7;
    size_t numOfElements;
    size_t numOfElementsWithDeleted;
    struct node {
        std::string value;
        bool state;
        HashTable<std::string, THash1, THash2> subterms;
        std::list<size_t> pages;
        node(const std::string& _value, const size_t _page, const bool _state = true) :value(_value), state(_state) {
            pages.push_back(_page);
        }
        node(const std::string& _value, const std::list<size_t> _pages, const bool _state = true) :value(_value), state(_state), pages(_pages) {}
        node(const node* _node) : value(_node->value), state(_node->state), subterms(_node->subterms), pages(_node->pages) {}
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

    node* getNode(const T& term) {
        unsigned int pos = Search(term);
        return pos == -1 ? nullptr
            : nodesArray.at(pos);
    }


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

    static bool CheckForSort(node* operand1, node* operand2) {
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
    static bool CheckForSortPages(node* operand1, node* operand2) {
        if (operand1 && operand2) {
            if (operand1->state && operand2->state) {
                size_t a = *operand1->pages.begin(), b = *operand2->pages.begin();
                return (a < b);
            }
        }
        return false;
    }

public:

    HashTable() {
        numOfElements = 0;
        numOfElementsWithDeleted = 0;
        nodesArray.resize(defaultSize);
    }
    ~HashTable() {
        nodesArray.~vector();
    }


    friend std::fstream& operator << (std::fstream& o, const HashTable& table) {
        o << table.numOfElements << " " << table.nodesArray.size() << '\n';
        for (auto& element : table.nodesArray) {
            if (element && element->state) {
                o << element->value << "\n" << element->state << " "
                    << element->subterms.numOfElements
                    << " " << element->subterms.nodesArray.size() << " " << element->pages.size() << '\n';
                for (size_t page : element->pages) {
                    o << page << '\n';
                }
                for (auto& subelement : element->subterms.nodesArray) {
                    if (subelement && subelement->state) {
                        o << '\t';
                        o << subelement->value << "\n\t" << subelement->state << " "
                            << subelement->subterms.numOfElements
                            << " " << subelement->subterms.nodesArray.size() << " " << subelement->pages.size() << '\n';
                        for (size_t page : subelement->pages) {
                            o << '\t' << page << '\n';
                        }
                        for (auto& subelement2 : subelement->subterms.nodesArray) {
                            if (subelement2 && subelement2->state) {
                                o << "\t\t";
                                o << subelement2->value << "\n\t\t" << subelement2->state << " "
                                    << subelement2->subterms.numOfElements
                                    << " " << subelement2->subterms.nodesArray.size() << " " << subelement2->pages.size() << '\n';
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
            char VAL[1000];
            bool S;
            is.getline(VAL, 1);
            is.getline(VAL,1000);
            is >> S >> NOE >> size >> pagesSize >> page;
            element = new node(VAL, page, S);
            element->subterms.numOfElements = NOE;
            element->subterms.numOfElementsWithDeleted = NOE;
            element->subterms.nodesArray.resize(size);
            for (size_t n = 0; n < pagesSize - 1; n++) {
                is >> page;
                element->pages.push_back(page);
            }
            for (size_t j = 0; j < element->subterms.numOfElements; j++) {
                auto& subelement = element->subterms.nodesArray.at(j);
                is.getline(VAL, 10);
                is.getline(VAL, 10, '\t');
                is.getline(VAL, 1000);
                is >> S >> NOE >> size >> pagesSize >> page;
                subelement = new node(VAL, page, S);
                subelement->subterms.numOfElements = NOE;
                subelement->subterms.numOfElementsWithDeleted = NOE;
                subelement->subterms.nodesArray.resize(size);
                for (size_t n = 0; n < pagesSize - 1; n++) {
                    is >> page;
                    subelement->pages.push_back(page);
                }
                for (size_t k = 0; k < subelement->subterms.numOfElements; k++) {
                    auto& subelement2 = subelement->subterms.nodesArray.at(k);
                    is.getline(VAL, 10);
                    is.getline(VAL, 10, '\t');
                    is.getline(VAL, 10, '\t');
                    is.getline(VAL, 1000);
                    is >> S >> NOE >> size >> pagesSize >> page;
                    subelement2 = new node(VAL, page, S);
                    subelement2->subterms.numOfElements = NOE;
                    subelement2->subterms.numOfElementsWithDeleted = NOE;
                    subelement2->subterms.nodesArray.resize(size);
                    for (size_t n = 0; n < pagesSize - 1; n++) {
                        is >> page;
                        subelement2->pages.push_back(page);
                    }
                }
            }
        }
        table.Rehash();
        return is;
    }

    void PreShowSort() {
        for (auto& element : nodesArray) {
            if (element && element->state) {
                element->pages.sort();
                for (auto& subelement : element->subterms.nodesArray) {
                    if (subelement && subelement->state) {
                        subelement->pages.sort();
                        for (auto& subelement2 : subelement->subterms.nodesArray) {
                            if (subelement2 && subelement2->state) {
                                subelement2->pages.sort();
                            }
                        }
                    }
                }
            }
        }
    }

    void ShowSorted() {
        quickSort(nodesArray.begin(), nodesArray.end(), &CheckForSort);
        for (auto& element : nodesArray) {
            if (element) {
                quickSort(element->subterms.nodesArray.begin(), element->subterms.nodesArray.end(),
                    &CheckForSort);
                for (auto& subelement : element->subterms.nodesArray) {
                    if (subelement) {
                        quickSort(subelement->subterms.nodesArray.begin(), subelement->subterms.nodesArray.end(),
                            &CheckForSort);
                    }
                }
            }
        }
        Show();
        Rehash();
    }
    void ShowSortedPages() {
        PreShowSort();
        quickSort(nodesArray.begin(), nodesArray.end(), &CheckForSortPages);
        for (auto& element : nodesArray) {
            if (element) {
                quickSort(element->subterms.nodesArray.begin(), element->subterms.nodesArray.end(),
                    &CheckForSortPages);
                for (auto& subelement : element->subterms.nodesArray) {
                    if (subelement) {
                        quickSort(subelement->subterms.nodesArray.begin(), subelement->subterms.nodesArray.end(),
                            &CheckForSortPages);                      
                    }
                }
            }
        }
        Show();
        Rehash();
    }

    void Show() {
        std::cout << separator << std::endl;
        bool isAnyElement = false;
        for (auto& element : nodesArray) {
            if (element && element->state) {
                isAnyElement = true;
                std::cout << "Term: " << element->value;
                element->pages.sort();
                for (size_t page : element->pages) {
                    std::cout << " " << page;
                }
                std::cout << std::endl;
                for (auto& subelement : element->subterms.nodesArray) {
                    if (subelement && subelement->state) {
                        std::cout << "\tSubterm(lvl 1): " << subelement->value;
                        subelement->pages.sort();
                        for (size_t page : subelement->pages) {
                            std::cout << " " << page;
                        }
                        std::cout << std::endl;
                        for (auto& subelement2 : subelement->subterms.nodesArray) {
                            if (subelement2 && subelement2->state) {
                                std::cout << "\t\tSubterm(lvl 2): " << subelement2->value;
                                subelement2->pages.sort();
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
        size_t firstDeleted = -1;
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
        else if (numOfElementsWithDeleted > numOfElements * 2)
            Rehash();
        size_t h1 = hash1(value, nodesArray.size());
        size_t h2 = hash2(value, nodesArray.size());
        size_t i = 0;
        size_t firstDeleted = -1;
        while (nodesArray.at(h1) != nullptr && i < nodesArray.size())
        {
            if (nodesArray.at(h1)->value == value && nodesArray.at(h1)->state) {
                auto itFound = std::find(nodesArray.at(h1)->pages.begin(), nodesArray.at(h1)->pages.end(), page);
                if (itFound == nodesArray.at(h1)->pages.end()) {
                    nodesArray.at(h1)->pages.push_back(page);
                    //nodesArray.at(h1)->pages.sort();
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
            nodesArray.at(firstDeleted)->pages.push_back(page);
            //nodesArray.at(firstDeleted)->pages.sort();
        }
        ++numOfElements;
        return true;
    }
    bool Add(const T& value, const std::list<size_t> pages, const THash1 hash1 = THash1(), const THash2 hash2 = THash2()) {
        if (numOfElements > coefToResize * nodesArray.size() - 1)
            Resize();
        else if (numOfElementsWithDeleted > numOfElements * 2)
            Rehash();
        size_t h1 = hash1(value, nodesArray.size());
        size_t h2 = hash2(value, nodesArray.size());
        size_t i = 0;
        size_t firstDeleted = -1;
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
        }
        return false;
    }
    bool AddSubterm(const T& value, const T& addValue, const std::list<size_t> addValuePages) {
        node* term = getNode(value);
        if (term) {
            return term->subterms.Add(addValue, addValuePages);
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
    bool AddSubterm(const T& value, const T& subValue, const T& addValue, const std::list<size_t> addValuePages) {
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
        bool isAnyFound = false;
        std::cout << separator << std::endl;
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
        node* buff = getNode(oldValue);
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

    bool ChangeSubterm(const T& oldValueTerm, const T& oldValue, const T& newValue) {
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
            auto itFound = std::find(buff->pages.begin(), buff->pages.end(), oldPage);
            if (itFound != buff->pages.end() && oldPage != newPage) {
                buff->pages.push_back(newPage);
                buff->pages.erase(itFound);
                return true;
            }
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
    bool DeleteSubterm(const T& value, const T& deleteValue) {
        node* term = getNode(value);
        if (term) {
            return term->subterms.Delete(deleteValue);
        }
        return false;
    }


    bool DeletePage(const T& value, const size_t page, const THash1 hash1 = THash1(), const THash2 hash2 = THash2()) {
        size_t h1 = hash1(value, nodesArray.size());
        size_t h2 = hash2(value, nodesArray.size());
        size_t i = 0;
        while (nodesArray.at(h1) != nullptr && i < nodesArray.size())
        {
            auto itFound = std::find(nodesArray.at(h1)->pages.begin(), nodesArray.at(h1)->pages.end(), page);
            if (nodesArray.at(h1)->value == value && nodesArray.at(h1)->state
              && itFound != nodesArray.at(h1)->pages.end()) {
                nodesArray.at(h1)->pages.erase(itFound);
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
    bool DeleteSubtermPage(const T& value, const T& deleteValue, const size_t deletePage) {
        node* term = getNode(value);
        if (term) {
            return term->subterms.DeletePage(deleteValue, deletePage);
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

