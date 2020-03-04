#include "p5.hpp"
#include <stdio.h>
#include <string.h>

#include "tempdata.hpp"
#include "Queue.hpp"

Queue<cityTemp> tempsQ;
char * name = "RENTON MUNICIPAL AIRPORT, WA US";

struct predicate {
    virtual bool operator()(const cityTemp &tmp)=0;
};

// Simple example of a filter based on generic predicates
struct namePredicate : public predicate {
    const char *fName;    
    namePredicate(const char * name)
    :fName(name)
    {}

    bool operator()(const cityTemp &tmp){
        if (!strcmp(tmp.name, fName)) {
            return true;
        } 

        return false;
    }
};

// This iterator is essentially a filter/predicate
class PredicateFilter {
    QueueIterator<cityTemp> &fSource;
    predicate &fPredicate;

public:
    PredicateFilter(predicate &pred, QueueIterator<cityTemp> &source)
        :fPredicate(pred),
        fSource(source)
    {}

    bool moveNext(){
        while (fSource.moveNext()) {
            if (fPredicate(fSource.getCurrent()) {
                return true;
            }
        }
        return false;
    }

    cityTemp getCurrent() {return fSource.getCurrent();}
    void reset() {fSource.reset();}
};




// This iterator is essentially a filter/predicate
class nameFilter {
    QueueIterator<cityTemp> &fSource;
    const char *fName;

public:
    nameFilter(const char * locName, QueueIterator<cityTemp> &source)
        :fName(locName),
        fSource(source)
    {}

    bool moveNext(){
        while (fSource.moveNext()) {
            if (!strcmp(fSource.getCurrent().name, fName)) {
                return true;
            }
        }
        return false;
    }

    cityTemp getCurrent() {return fSource.getCurrent();}
    void reset() {fSource.reset();}
};

struct printTemp {
    void operator()(const cityTemp &tmp) {
        printf("Low: %d  High: %d\n", tmp.low, tmp.high);
    }
};

void setup()
{
    createCanvas(400, 400);
    
    int nTemps = sizeof(temps)/sizeof(temps[0]);
    // Fill the queue with temperature data
    for (int i=1; i<=nTemps; i++)
        tempsQ.enqueue(temps[i-1]);

    QueueIterator<cityTemp> tempsqi(tempsQ);
    nameFilter nf(name, tempsqi);
    printTemp pt;

    while (nf.moveNext()) {
        pt(nf.getCurrent());
    }
    
}