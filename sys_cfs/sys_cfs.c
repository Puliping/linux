
#include <linux/linkage.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>  
#include <linux/sched/mm.h>

struct Node {
    int pid;
	unsigned long long virtual_time;
    struct Node* next;
};

void sortedInsert(struct Node** head_ref, struct Node* new_node) {

    struct Node* atual;

    if (*head_ref == NULL || (*head_ref)->virtual_time  >= new_node->virtual_time) {

        new_node->next = *head_ref;
        *head_ref = new_node;
    
	}
    else {

        atual = *head_ref;

        while (atual->next != NULL && atual->next->virtual_time < new_node->virtual_time) {
        
		    atual = atual->next;
        
		}

        new_node->next = atual->next;
        atual->next = new_node;

    }
}

struct Node* newNode(int new_pid, unsigned long long new_vruntime) {

    struct Node* new_node = (struct Node*)kmalloc( sizeof(struct Node), GFP_KERNEL );
 
    /* put in the data  */
    new_node->virtual_time = new_vruntime;
	new_node->pid = new_pid;
    new_node->next = NULL;

    return new_node;
}
 
void printList(struct Node* head) {

    struct Node* temp = head;
	
    while (temp != NULL) {
        pr_info("pid%d  virtualRuntime %llu \n", temp->pid, temp->virtual_time);
		
		temp = temp->next;
    }
}


asmlinkage unsigned long sys_cfs(void){

	struct task_struct* task_list;
    struct task_struct* p;
	struct Node* head = NULL;
    struct Node* new_node ;

    for_each_process_thread(task_list, p) {

        pr_info("== %s [%d]\n", task_list->comm, task_list->pid);
        new_node = newNode(p->pid, p->se.vruntime);
        sortedInsert(&head, new_node);
    }

	printList(head);

	return 1 ;
}
