
#include <stdio.h>
#include <string.h>

#include "tempdata.hpp"
#include "Queue.hpp"

Queue<cityTemp> tempsQ;
char * name = "RENTON MUNICIPAL AIRPORT, WA US";


class Predicate 
{
public:
    virtual bool operator()(const cityTemp &tmp)=0;
};

// Simple example of a filter based on generic predicates
class namePredicate : public Predicate {
    const char *fName;    
public:
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

class highTempPredicate : public Predicate {
    int target;

public:
    highTempPredicate(int value)
        : target(value)
    {}

    bool operator()(const cityTemp &tmp) {
        if (tmp.high <= target) {
            return true;
        }
        return false;
    }
};

// This iterator is essentially a filter/predicate
class PredicateFilter : public IEnumerator<cityTemp> {
    IEnumerator<cityTemp> &fSource;
    Predicate &fPredicate;

public:
    PredicateFilter(Predicate &pred, IEnumerator<cityTemp> &source)
        :fPredicate(pred),
        fSource(source)
    {}

    bool moveNext(){
        while (fSource.moveNext()) {
            bool isTrue = fPredicate(fSource.getCurrent());
            if (isTrue)
                return true;
        }
        return false;
    }

    cityTemp getCurrent() const {return fSource.getCurrent();}
    void reset() {fSource.reset();}
};


struct printTemp {
    void operator()(const cityTemp &tmp) {
        printf("Name: %s Low: %d  High: %d\n", tmp.name, tmp.low, tmp.high);
    }
};

void main()
{

    int nTemps = sizeof(temps)/sizeof(temps[0]);
    // Fill the queue with temperature data
    for (int i=1; i<=nTemps; i++)
        tempsQ.enqueue(temps[i-1]);


    PredicateFilter ff(highTempPredicate(50), 
        PredicateFilter(namePredicate(name), 
        QueueIterator<cityTemp>(tempsQ)));
        
    printTemp pt;

    while (ff.moveNext()) {
        pt(ff.getCurrent());
    }
    
}