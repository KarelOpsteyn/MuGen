#include <iostream>
#include <vector>
#include "scale.h"
#include "song.h"
#include "grade.h"
using namespace std;

int main()
{
    /*
    Scale scl(64, 1);
    Scale scl2(64,0);
    cout<<scl.getmodus()<<' '<<scl.getscalenote(3)<<endl;
    scl.print();
    scl2.print();
    */
/*
    vector<Grade> Gradeprogression(4);
    vector<Grade>& vectorref = Gradeprogression;
    Grade start(1);
    vectorref[0]=start;
    for(int i = 1; i< 4; i++){
        vectorref[i] = Gradeprogression[i-1].addgrade();
    }

    for(int i = 0; i< int(Gradeprogression.size());i++){
        vectorref[i].print();
    }
*/
    /*
    Grade gr1(1);
    Grade gr2 = gr1.addgrade();
    Grade gr3 = gr2.addgrade();
    Grade gr4 = gr3.addgrade();
    gr1.print();
    gr2.print();
    gr3.print();
    gr4.print();
    */

    Song newsong(10);
    newsong.generate();
    newsong.printgradeprogression();

    return 0;
}
