#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include <vector>
#include "Observer.h"

class Subject {
    protected:
    std::vector<Observer*> observers;

    public:
    Subject();
    void attach(Observer *o);
    void detach(Observer *o);
    void notifyObservers();
};

#endif
