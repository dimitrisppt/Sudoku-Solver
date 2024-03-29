#ifndef BEST_FIRST_SEARCH_H
#define BEST_FIRST_SEARCH_H

#include "Searchable.h"

#include <memory>
using std::unique_ptr;



#include <queue>
using std::priority_queue;

struct HeuristicCompare {
    bool operator()(const unique_ptr<Searchable>& first, const unique_ptr<Searchable>& second) {
        return first->heuristicValue() > second->heuristicValue();
    }
};

class BestFirstSearch {

protected:

    priority_queue<unique_ptr<Searchable>, vector<unique_ptr<Searchable>>, HeuristicCompare> pq;

    /// Make sure you increment this every time you 'expand' a node, by getting it successors and putting them on the queue
    int nodes = 0;


public:

    BestFirstSearch(std::unique_ptr<Searchable> && startFrom) {
        // TODO Put startFrom onto the queue:
        pq.push(std::move(startFrom));

    }

    int getNodesExpanded() const {
        return nodes;
    }

    Searchable * solve() {

        // TODO Implement search, returning a pointer to the solution (if one is found)
        while (!pq.empty()) {
            if (pq.top()->isSolution()) {
                return pq.top().get();
            }

            ++nodes;

            unique_ptr<Searchable> current(std::move(const_cast<unique_ptr<Searchable> &> (pq.top())));

            pq.pop();

            vector<unique_ptr<Searchable>> successors = current->successors();

            for (auto & successor : successors) {
                // and push each one onto the back of queue.
                pq.push(std::move(successor));
            }

        }
        return nullptr;
    }
};






// Do not edit below this line

#endif
