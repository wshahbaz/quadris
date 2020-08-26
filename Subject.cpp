#include "Subject.h"

Subject::Subject() {}
// Subject::~Subject() {}

void Subject::attach(Observer *o) {
  observers.emplace_back(o);
}

void Subject::detach(Observer *o) {
  for (auto it = observers.begin(); it != observers.end();) {
    if (*it == o) {
      observers.erase(it);
    } else { ++it; }
  }
}

void Subject::notifyObservers() {
  for (auto ob : observers) ob->update();
}
