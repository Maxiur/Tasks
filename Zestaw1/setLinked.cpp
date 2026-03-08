#include <iostream>

struct ListNode {
    ListNode *next;
    int val;

    ListNode(const int x, ListNode *next) : next(next), val(x) {}
    ListNode(const int x) : next(nullptr), val(x) {}
    ListNode() : next(nullptr), val(0) {}
};

class setLinked {
    public:
    ListNode *head = new ListNode();

    void insert(int val) {
        ListNode *prev = find_last(val);

        // if exists
        if (prev->next != nullptr && prev->next->val == val)
            return;

        prev->next = new ListNode(val, prev->next);
    }

    void remove(int val) {

    }

private:

    // finding prev pointer of desired value
    ListNode* find_last(int val) const {
        ListNode *curr = head;
        while (curr->next != nullptr && curr->next->val < val) {
            curr = curr->next;
        }
        return curr;
    }
};