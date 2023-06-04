#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/queue.h>


struct ble_ll_sched_item
{
    uint32_t start_time;
    uint32_t end_time;
    uint8_t sched_type;
};

typedef struct ble_ll_sched_node                             /* 链表节点结构体 */
{
    SLIST_ENTRY(ble_ll_sched_node) field;
    struct ble_ll_sched_item entry;
} ble_ll_sched_node_t;



typedef SLIST_HEAD(ble_ll_sched_head, ble_ll_sched_node) ble_ll_sched_head_t;    /* 链表头结构体 */

/* 合并两个链表 */
static ble_ll_sched_node_t *merge(ble_ll_sched_node_t *node1, ble_ll_sched_node_t *node2)
{
    if (node1 == NULL) {return node2;}
    if (node2 == NULL) {return node1;}

    /* res 保存头 */
    ble_ll_sched_node_t *res, *ptr;

    if(node1->entry.start_time < node2->entry.start_time) {
        res = node1;
        node1 = SLIST_NEXT(node1, field);
    }
    else {
        res = node2;
        node2 = SLIST_NEXT(node2, field);
    }

    ptr = res;

    while (node1 != NULL && node2 != NULL) {
        if (node1->entry.start_time < node2->entry.start_time) {
            SLIST_NEXT(ptr, field) = node1;
            node1 = SLIST_NEXT(node1, field);
        }
        else {
            SLIST_NEXT(ptr, field) = node2;
            node2 = SLIST_NEXT(node2, field);
        }
        ptr = SLIST_NEXT(ptr, field);
    }

    if      (node1 != NULL) {SLIST_NEXT(ptr, field) = node1;}
    else if (node2 != NULL) {SLIST_NEXT(ptr, field) = node2;}

    return res;
}

/* 对链表节点进行归并排序 */
static ble_ll_sched_node_t *mergeSort(ble_ll_sched_node_t *node)
{
    if (node==NULL || SLIST_NEXT(node,field) == NULL) { 
        return node;
    }

    ble_ll_sched_node_t *fast = node;
    ble_ll_sched_node_t *slow = node;
    while (SLIST_NEXT(fast, field) != NULL) {
        if (SLIST_NEXT(SLIST_NEXT(fast,field), field) == NULL) {
            break;
        }
        fast = SLIST_NEXT(SLIST_NEXT(fast,field), field);
        slow = SLIST_NEXT(slow, field);
    }

    fast = slow;
    slow = SLIST_NEXT(slow, field);
    SLIST_NEXT(fast,field) = NULL;
    fast = mergeSort(node);
    slow = mergeSort(slow);
    return merge(fast, slow);
}

/* 对链表头进行归并排序 */
void num_mergeSort(ble_ll_sched_head_t *head)
{
    SLIST_FIRST(head) = mergeSort(SLIST_FIRST(head));
}

int main(void)
{
    /* 从堆空间申请链表表头并初始化 */
    ble_ll_sched_head_t *ble_ll_sched_head = malloc(sizeof(ble_ll_sched_head_t));
    SLIST_INIT(ble_ll_sched_head);

    /* 为链表添加10个0~100的随机数 */
    ble_ll_sched_node_t *elm;
    printf("插入顺序：\n");
    for (int i=0; i<10; ++i) {
        elm = malloc(sizeof(ble_ll_sched_node_t));
        elm->entry.start_time = rand() % 100;
        printf("addr = %p, num = %d\n", elm, elm->entry.start_time);
        SLIST_INSERT_HEAD(ble_ll_sched_head, elm, field);
    }

    /* 打印排序前的链表 */
    printf("Before sort:\n");
    SLIST_FOREACH (elm, ble_ll_sched_head, field) {
        printf("addr = %p, num = %d\n", elm, elm->entry.start_time);
    }

    /* 归并排序函数 */
    num_mergeSort(ble_ll_sched_head);

    /* 打印排序后的链表 */
    printf("After sort:\n");
    SLIST_FOREACH (elm, ble_ll_sched_head, field) {
        printf("addr = %p, num = %d\n", elm, elm->entry.start_time);
    }

    /* 销毁链表释放空间 */
    while (!SLIST_EMPTY(ble_ll_sched_head)) {
        ble_ll_sched_node_t *ptr = SLIST_FIRST(ble_ll_sched_head);
        SLIST_REMOVE_HEAD(ble_ll_sched_head, field);
        free(ptr);
    }
    free(ble_ll_sched_head);

    return 0;
}
