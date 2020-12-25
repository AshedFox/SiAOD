#include <iostream>
#include "queue.h"
#include <list>

using namespace std;


int main()
{
    list<pair<list<int>, int>> initList{ { {4, 3, 1, 8, 6, 5, 2, 3}, 1},
                                         { {3,2,4,7,2,6,8},1 },
                                         { {3,1,3,1,5,4,3,2},1 },
                                         { {3,2,1,3,2,4,3},2 },
                                         { {1,2,1,4,2,6},3 },
                                         { {2,6,3,2,1,3,2,3},3} };


    Queue queue{initList};
    vector<string> queueInputs;

    queueInputs.resize(110);
    for (int i = 0; i <= 10; i++) {
        cout << "Tick time||Input time||Tt |Tz |K" << endl;
        for (int j = 1; j <= 10; j++) {
            printf("    %2d   ||    %2d    ||", j, i);
            queue = { initList, j,i };
            queue.processAllQueue();
            queue.getTaskOutput();
            cout << endl;
        }
        cout << "--------------------------------------" << endl;
    }
    cout << endl;

    //queue = { initList, 3, 3 };
    //queue.processAllQueue();
    //queue.getTaskOutput();
    /*initList.clear();
    initList = { { {5,3,4},1 }, {{4,2,3},2}, {{3,3,2},3} };
    Queue qqq{ initList, 3, 4 };
    qqq.processAllQueue();
    qqq.getTaskOutput();*/

    return 0;
}