/**
 * Definition for singly-linked list.
 */

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

struct ListNode_s
{
    int val;
    struct ListNode_s *next;
};

typedef struct ListNode_s ListNode;



ListNode* CreateNode(int value)
{
    ListNode* node = malloc(sizeof(ListNode));
    node->val = value;
    node->next = NULL;

    return node;
}

void PrintList(ListNode* head)
{
    ListNode* node = head;
    int count = 1;

    assert(head);

    while (node->next)
    {
        printf("[%d](%d)->", count, node->val);
        node = node->next;
        count++;
    }
    printf("[%d](%d)\n\n", count, node->val);

}

void FreeList(ListNode* head)
{
    if (head == NULL) return;
    FreeList(head->next);

    free(head);
}

ListNode* FlipList(ListNode* head, ListNode* after_end)
{
    if (head->next == after_end) return head;

    ListNode* new_head = FlipList(head->next, after_end);

    head->next->next = head;
    head->next = NULL;

    return new_head;
}

ListNode* reverseKGroup(ListNode* head, int k)
{
    ListNode* new_head = head;
    ListNode* node = head;
    ListNode* segment_start = head;
    ListNode* prev_segment_end = NULL;
    
    assert(head);
    assert(k > 0);

    int segment_count = 0;

    while (1)
    {
        if (segment_count == k)
        {
            ListNode* new_segment_start = FlipList(segment_start, node);
            segment_start->next = node;

            if (prev_segment_end == NULL) new_head = new_segment_start;
            else prev_segment_end->next = new_segment_start;

            prev_segment_end = segment_start;
            segment_start = node;

            segment_count = 0;
        }
        

        if (node == NULL) break;
        segment_count++;
        node = node->next;
    }
    
    return new_head;
}

int main(int argc, char const *argv[])
{

    ListNode* head = CreateNode(1);    
    ListNode* node = head;
    
    for (int i = 2; i < 5; i++)
    {
        node->next = CreateNode(i);
        node = node->next;
    }

    PrintList(head);
    head = reverseKGroup(head, 6);    
    // head = FlipList(head, NULL);
    PrintList(head);

    FreeList(head);

    return 0;
}




