#include <iostream>

#include <string>
#include <fstream>

#include "HashTable.h"


void trim(std::string& src)
{
    src.erase(std::find_if_not(src.rbegin(), src.rend(), ::isspace).base(), src.end());
    src.erase(src.begin(), std::find_if_not(src.begin(), src.end(), ::isspace));
}


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


void menu(){
    HashTable<std::string,HashFunctionVersion1,HashFunctionVersion2> table;
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
            std::fstream fin("text.bin", std::fstream::in | std::fstream::binary);
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
            std::fstream fout("text.bin", std::fstream::out | std::fstream::binary);
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
                    char stopRepeatingChoise='n';
                    while (stopRepeatingChoise != 'y') {
                        std::string addValue;
                        size_t addPage;
                        std::cout << "Enter aditting value: ";
                        do {
                            std::getline(std::cin, addValue);
                            trim(addValue);
                        } while (addValue.empty());
                        std::cout << "Enter page: ";
                        std::cin >> addPage;
                        std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                        if (table.Add(addValue, addPage)) {
                            std::cout << "Added successfully\n" << std::endl;
                        }
                        else
                            std::cout << "Addition failed\n" << std::endl;
                        std::cout << "Stop addition?('y' to stop)\n";
                        std::cin >> stopRepeatingChoise;
                        std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                    }
                    break;
                }
                case('2'): {
                    char stopRepeatingChoise = 'n';
                    while (stopRepeatingChoise != 'y') {
                        std::string term, addValue;
                        size_t addPage;
                        std::cout << "Enter term: ";
                        std::getline(std::cin, term);
                        std::cout << "Enter aditting value: ";
                        do {
                            std::getline(std::cin, addValue);
                            trim(addValue);
                        } while (addValue.empty());
                        std::cout << "Enter page: ";
                        std::cin >> addPage;
                        std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                        if (table.AddSubterm(term, addValue, addPage)) {
                            std::cout << "Added successfully\n" << std::endl;
                        }
                        else
                            std::cout << "Addition failed\n" << std::endl;
                        std::cout << "Stop addition?('y' to stop)\n";
                        std::cin >> stopRepeatingChoise;
                        std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                    }
                    break;
                }
                case('3'): {
                    char stopRepeatingChoise = 'n';
                    while (stopRepeatingChoise != 'y') {
                        std::string term, subterm, addValue;
                        size_t addPage;
                        std::cout << "Enter term: ";
                        std::getline(std::cin, term);
                        std::cout << "Enter subterm: ";
                        std::getline(std::cin, subterm);
                        std::cout << "Enter aditting value: ";
                        do {
                            std::getline(std::cin, addValue);
                            trim(addValue);
                        } while (addValue.empty());
                        std::cout << "Enter page: ";
                        std::cin >> addPage;
                        std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                        if (table.AddSubterm(term, subterm, addValue, addPage)) {
                            std::cout << "Added successfully\n" << std::endl;
                        }
                        else
                            std::cout << "Addition failed\n" << std::endl;
                        std::cout << "Stop addition?('y' to stop)\n";
                        std::cin >> stopRepeatingChoise;
                        std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
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
                            char stopRepeatingChoise = 'n';
                            while (stopRepeatingChoise != 'y') {
                                std::string oldTerm, newTerm;
                                std::cout << "Enter old term: ";
                                std::getline(std::cin, oldTerm);
                                std::cout << "Enter new term: ";
                                do {
                                    std::getline(std::cin, newTerm);
                                    trim(newTerm);
                                } while (newTerm.empty());
                                if (table.Change(oldTerm, newTerm))
                                    std::cout << "Changed successfully\n" << std::endl;
                                else
                                    std::cout << "Change failed\n" << std::endl;
                                std::cout << "Stop changing?('y' to stop)\n";
                                std::cin >> stopRepeatingChoise;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            }
                            break;
                        }
                        case('2'): {
                            char stopRepeatingChoise = 'n';
                            while (stopRepeatingChoise != 'y') {
                                std::string oldTerm, oldSubterm, newSubterm;
                                std::cout << "Enter term: ";
                                std::getline(std::cin, oldTerm);
                                std::cout << "Enter old subterm: ";
                                std::getline(std::cin, oldSubterm);
                                std::cout << "Enter new subterm: ";
                                do {
                                    std::getline(std::cin, newSubterm);
                                    trim(newSubterm);
                                } while (newSubterm.empty());
                                if (table.ChangeSubterm(oldTerm, oldSubterm, newSubterm))
                                    std::cout << "Changed successfully\n" << std::endl;
                                else
                                    std::cout << "Change failed\n" << std::endl;
                                std::cout << "Stop changing?('y' to stop)\n";
                                std::cin >> stopRepeatingChoise;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            }
                            break;
                        }
                        case('3'): {
                            char stopRepeatingChoise = 'n';
                            while (stopRepeatingChoise != 'y') {
                                std::string oldTerm, oldSubterm, oldSubterm2, newSubterm2;
                                std::cout << "Enter term: ";
                                std::getline(std::cin, oldTerm);
                                std::cout << "Enter old first-level subterm: ";
                                std::getline(std::cin, oldSubterm);
                                std::cout << "Enter old second-level subterm: ";
                                std::getline(std::cin, oldSubterm2);
                                std::cout << "Enter new second-level subterm: ";
                                do {
                                    std::getline(std::cin, newSubterm2);
                                    trim(newSubterm2);
                                } while (newSubterm2.empty());
                                if (table.ChangeSubterm(oldTerm, oldSubterm, oldSubterm2, newSubterm2))
                                    std::cout << "Changed successfully\n" << std::endl;
                                else
                                    std::cout << "Change failed\n" << std::endl;
                                std::cout << "Stop changing?('y' to stop)\n";
                                std::cin >> stopRepeatingChoise;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            }
                            break;
                        }
                        case('b'):
                            isExitInternal2 = true;
                            break;
                        }
                    }
                    break;
                }
                case('2'): {
                    char input3; 
                    while (!isExitInternal2) {
                        std::cout << "Choose variant:\n1 - Change term page\n2 - Change first-level subterm page\n"
                            << "3 - Change second-level subterm page\nb - Go back\n" << std::endl;
                        std::cin >> input3;
                        std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                        switch (input3) {
                        case('1'): {
                            char stopRepeatingChoise = 'n';
                            while (stopRepeatingChoise != 'y') {
                                std::string oldTerm;
                                size_t oldPage, newPage;
                                std::cout << "Enter term: ";
                                std::getline(std::cin, oldTerm);
                                std::cout << "Enter old page: ";
                                std::cin >> oldPage;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                                std::cout << "Enter new page: ";
                                std::cin >> newPage;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                                if (table.ChangePage(oldTerm, oldPage, newPage))
                                    std::cout << "Changed successfully\n" << std::endl;
                                else
                                    std::cout << "Change failed\n" << std::endl;
                                std::cout << "Stop changing?('y' to stop)\n";
                                std::cin >> stopRepeatingChoise;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            }
                            break;
                        }
                        case('2'): {
                            char stopRepeatingChoise = 'n';
                            while (stopRepeatingChoise != 'y') {
                                std::string oldTerm, oldSubterm;
                                size_t oldPage, newPage;
                                std::cout << "Enter term: ";
                                std::getline(std::cin, oldTerm);
                                std::cout << "Enter old subterm: ";
                                std::getline(std::cin, oldSubterm);
                                std::cout << "Enter old page: ";
                                std::cin >> oldPage;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                                std::cout << "Enter new page: ";
                                std::cin >> newPage;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                                if (table.ChangeSubtermPage(oldTerm, oldSubterm, oldPage, newPage))
                                    std::cout << "Changed successfully\n" << std::endl;
                                else
                                    std::cout << "Change failed\n" << std::endl;
                                std::cout << "Stop changing?('y' to stop)\n";
                                std::cin >> stopRepeatingChoise;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            }
                            break;
                        }
                        case('3'): {
                            char stopRepeatingChoise = 'n';
                            while (stopRepeatingChoise != 'y') {
                                std::string oldTerm, oldSubterm, oldSubterm2;
                                size_t oldPage, newPage;
                                std::cout << "Enter term: ";
                                std::getline(std::cin, oldTerm);
                                std::cout << "Enter old first-level subterm: ";
                                std::getline(std::cin, oldSubterm);
                                std::cout << "Enter old second-level subterm: ";
                                std::getline(std::cin, oldSubterm2);
                                std::cout << "Enter old page: ";
                                std::cin >> oldPage;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                                std::cout << "Enter new page: ";
                                std::cin >> newPage;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                                if (table.ChangeSubtermPage(oldTerm, oldSubterm, oldSubterm2, oldPage, newPage))
                                    std::cout << "Changed successfully\n" << std::endl;
                                else
                                    std::cout << "Change failed\n" << std::endl;
                                std::cout << "Stop changing?('y' to stop)\n";
                                std::cin >> stopRepeatingChoise;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            }
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
                            char stopRepeatingChoise = 'n';
                            while (stopRepeatingChoise != 'y') {
                                std::string deleteTerm;
                                std::cout << "Enter deleting term: ";
                                std::getline(std::cin, deleteTerm);
                                if (table.Delete(deleteTerm))
                                    std::cout << "Deleted successfully\n" << std::endl;
                                else
                                    std::cout << "Deletion failed\n" << std::endl;
                                std::cout << "Stop deleting?('y' to stop)\n";
                                std::cin >> stopRepeatingChoise;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            }
                            break;
                        }
                        case('2'): {
                            char stopRepeatingChoise = 'n';
                            while (stopRepeatingChoise != 'y') {
                                std::string term, deleteSubterm;
                                std::cout << "Enter term: ";
                                std::getline(std::cin, term);
                                std::cout << "Enter deleting subterm: ";
                                std::getline(std::cin, deleteSubterm);
                                if (table.DeleteSubterm(term, deleteSubterm))
                                    std::cout << "Deleted successfully\n" << std::endl;
                                else
                                    std::cout << "Deletion failed\n" << std::endl;
                                std::cout << "Stop deleting?('y' to stop)\n";
                                std::cin >> stopRepeatingChoise;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            }
                            break;
                        }
                        case('3'): {
                            char stopRepeatingChoise = 'n';
                            while (stopRepeatingChoise != 'y') {
                                std::string term, subterm, deleteSubterm2;
                                std::cout << "Enter term: ";
                                std::getline(std::cin, term);
                                std::cout << "Enter first-level subterm: ";
                                std::getline(std::cin, subterm);
                                std::cout << "Enter deleting second-level subterm: ";
                                std::getline(std::cin, deleteSubterm2);
                                if (table.DeleteSubterm(term, subterm, deleteSubterm2))
                                    std::cout << "Deleted successfully\n" << std::endl;
                                else
                                    std::cout << "Deletion failed\n" << std::endl;
                                std::cout << "Stop deleting?('y' to stop)\n";
                                std::cin >> stopRepeatingChoise;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            }
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
                            char stopRepeatingChoise = 'n';
                            while (stopRepeatingChoise != 'y') {
                                std::string term;
                                size_t deletePage;                               
                                std::cout << "Enter term: ";
                                std::getline(std::cin, term);
                                std::cout << "Enter deleting page: ";
                                std::cin >> deletePage;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                                if (table.DeletePage(term, deletePage))
                                    std::cout << "Deleted successfully\n" << std::endl;
                                else
                                    std::cout << "Deletion failed\n" << std::endl;
                                std::cout << "Stop deleting?('y' to stop)\n";
                                std::cin >> stopRepeatingChoise;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            }
                            break;
                        }
                        case('2'): {
                            char stopRepeatingChoise = 'n';
                            while (stopRepeatingChoise != 'y') {
                                std::string term, subterm;
                                size_t deletePage;
                                std::cout << "Enter term: ";
                                std::getline(std::cin, term);
                                std::cout << "Enter subterm: ";
                                std::getline(std::cin, subterm);                            
                                std::cout << "Enter deleting page: ";
                                std::cin >> deletePage;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                                if (table.DeleteSubtermPage(term, subterm, deletePage))
                                    std::cout << "Deleted successfully\n" << std::endl;
                                else
                                    std::cout << "Deletion failed\n" << std::endl;
                                std::cout << "Stop deleting?('y' to stop)\n";
                                std::cin >> stopRepeatingChoise;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            }
                            break;
                        }
                        case('3'): {
                            char stopRepeatingChoise = 'n';
                            while (stopRepeatingChoise != 'y') {
                                std::string term, subterm, subterm2;
                                size_t deletePage;
                                std::cout << "Enter term: ";
                                std::getline(std::cin, term);
                                std::cout << "Enter first-level subterm: ";
                                std::getline(std::cin, subterm);
                                std::cout << "Enter second-level subterm: ";
                                std::getline(std::cin, subterm2);
                                std::cout << "Enter deleting page: ";
                                std::cin >> deletePage;;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                                if (table.DeleteSubtermPage(term, subterm, subterm2, deletePage))
                                    std::cout << "Deleted successfully\n" << std::endl;
                                else
                                    std::cout << "Deletion failed\n" << std::endl;
                                std::cout << "Stop deleting?('y' to stop)\n";
                                std::cin >> stopRepeatingChoise;
                                std::cin.clear(); std::cin.ignore(INT_MAX, '\n');
                            }
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
                            std::getline(std::cin, term);
                            table.SearchByTerm(term);
                            break;
                        }
                        case('2'): {
                            std::string subterm;
                            std::cout << "Enter first-level subterm: ";
                            std::getline(std::cin,subterm);
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
                            std::getline(std::cin, subterm);
                            table.SearchBySubterm(subterm, 1);
                            break;
                        }
                        case('2'): {
                            std::string subterm2;
                            std::cout << "Enter second-level subterms: ";
                            std::getline(std::cin, subterm2);
                            table.SearchBySubterm(subterm2, 2);
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

int main()
{
    menu();
    
    return 0;
}
