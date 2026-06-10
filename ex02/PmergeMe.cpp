#include "PmergeMe.hpp"

/* ==================== VECTOR ==================== */

void mergeInsertVector(std::vector<int> &vec)
{
    if (vec.size() <= 1)
        return;

    std::vector<int> main;
    std::vector<int> pend;
    bool hasStraggler = (vec.size() % 2 != 0);
    int  straggler    = hasStraggler ? vec.back() : 0;

    for (size_t i = 0; i + 1 < vec.size(); i += 2)
    {
        if (vec[i] > vec[i + 1]) { main.push_back(vec[i]);     pend.push_back(vec[i + 1]); }
        else                     { main.push_back(vec[i + 1]); pend.push_back(vec[i]);     }
    }

    mergeInsertVector(main);

    main.insert(main.begin(), pend[0]);

    for (size_t i = 1; i < pend.size(); i++)
    {
        std::vector<int>::iterator pos =
            std::lower_bound(main.begin(), main.end(), pend[i]);
        main.insert(pos, pend[i]);
    }

    if (hasStraggler)
    {
        std::vector<int>::iterator pos =
            std::lower_bound(main.begin(), main.end(), straggler);
        main.insert(pos, straggler);
    }

    vec = main;
}

/* ==================== DEQUE ==================== */

void mergeInsertDeque(std::deque<int> &deq)
{
    if (deq.size() <= 1)
        return;

    std::deque<int> main;
    std::deque<int> pend;
    bool hasStraggler = (deq.size() % 2 != 0);
    int  straggler    = hasStraggler ? deq.back() : 0;

    for (size_t i = 0; i + 1 < deq.size(); i += 2)
    {
        if (deq[i] > deq[i + 1]) { main.push_back(deq[i]);     pend.push_back(deq[i + 1]); }
        else                     { main.push_back(deq[i + 1]); pend.push_back(deq[i]);     }
    }

    mergeInsertDeque(main);

    main.insert(main.begin(), pend[0]);

    for (size_t i = 1; i < pend.size(); i++)
    {
        std::deque<int>::iterator pos =
            std::lower_bound(main.begin(), main.end(), pend[i]);
        main.insert(pos, pend[i]);
    }

    if (hasStraggler)
    {
        std::deque<int>::iterator pos =
            std::lower_bound(main.begin(), main.end(), straggler);
        main.insert(pos, straggler);
    }

    deq = main;
}