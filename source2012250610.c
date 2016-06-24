#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define WHITE 0
#define GRAY  1
#define BLACK 2

typedef struct Adj {
	int n;
	struct Adj* next;
} Adj;

void Adj_init(Adj* self) {
	self->n = 0;
	self->next = NULL;
}

typedef struct {
	int color;
	int parent;
	char name[20];
	int n;
	Adj* first;
} Vertex;

void Vertex_init(Vertex* self) {
	self->color = 0;
	self->parent = -1;
	strcpy(self->name, "(none)");
	self->n = 0;
	self->first = NULL;
}

void Vertex_add(Vertex* self, int count2) {
	Adj* a = (Adj *)malloc(sizeof(Adj));
	a->n = count2;
	a->next = self->first;
	self->first = a;
}

typedef struct {
	Vertex super;
	double d;
} BFSVertex;

void BFSVertex_init(BFSVertex* self) {
	Vertex_init(&self->super);
	self->d = 1E10;
}

typedef struct {
	Vertex super;
	int d, f;
} DFSVertex;

void DFSVertex_init(DFSVertex* self) {
	Vertex_init(&self->super);
	self->d = 0;
	self->f = 0;
}

typedef struct {
	int front;
	int rear;
	int sz;
	int* buf;
} Queue;

void Queue_init(Queue* self) {
	self->front = 0;
	self->rear = 0;
	self->sz = 0;
	self->buf = NULL;
}

void Queue_create_queue(Queue* self, int sz) {
	self->sz = sz;
	self->buf = (int*)malloc(sizeof(int)*sz);
}

void Queue_enqueue(Queue* self, int val) {
	self->buf[self->rear] = val;
	self->rear = (self->rear + 1) % self->sz;
}

int Queue_dequeue(Queue* self) {
	int res = self->buf[self->front];
	self->front = (self->front + 1) % self->sz;
	return res;
}

int Queue_is_empty(Queue* self) {
	return self->front == self->rear;
}

void print_bfsvertex(BFSVertex* vertices, int n) {
	printf("%s ", vertices[n].super.name);
	printf("%d ", vertices[n].super.color);
	printf("%d ", vertices[n].super.parent);
	printf("%f:", vertices[n].d);
	for (Adj* p = vertices[n].super.first; p; p = p->next) {
		printf("%s ", vertices[p->n].super.name);
	}
	printf("\n");
}

void print_dfsvertex(DFSVertex* vertices, int n) {
	printf("%s ", vertices[n].super.name);
	printf("%d ", vertices[n].super.color);
	printf("%d ", vertices[n].super.parent);
	printf("%d ", vertices[n].d);
	printf("%d:", vertices[n].f);
	for (Adj* p = vertices[n].super.first; p; p = p->next) {
		printf("%s ", vertices[p->n].super.name);
	}
	printf("\n");
}

void bfs(BFSVertex* vertices, int nelem, int s) {
	Queue q;
	for (int u = 0; u < nelem; u++) {
		if (vertices[u].super.n != s) {
			vertices[u].super.color = WHITE;
			vertices[u].d = 1E10;
			vertices[u].super.parent = -1;
		}
	}
	vertices[s].super.color = GRAY;
	vertices[s].d = 0;
	vertices[s].super.parent = -1;
	Queue_init(&q);
	Queue_create_queue(&q, nelem);
	Queue_enqueue(&q, s);
	while (!Queue_is_empty(&q)) {
		int u = Queue_dequeue(&q);
		for (Adj* adj_v = vertices[u].super.first; adj_v; adj_v = adj_v->next) {
			if (vertices[adj_v->n].super.color == WHITE) {
				vertices[adj_v->n].super.color = GRAY;
				vertices[adj_v->n].d = vertices[u].d + 1;
				vertices[adj_v->n].super.parent = u;
				Queue_enqueue(&q, adj_v->n);
			}
		}
		vertices[u].super.color = BLACK;
	}
}

int dfs_time = 0;

void dfs_visit(DFSVertex* vertices, int nelem, int u) {
	dfs_time++;
	vertices[u].d = dfs_time;
	vertices[u].super.color = GRAY;
	for (Adj* v = vertices[u].super.first; v; v = v->next) {
		if (vertices[v->n].super.color == WHITE) {
			vertices[v->n].super.parent = u;
			dfs_visit(vertices, nelem, v->n);
		}
	}
	vertices[u].super.color = BLACK;
	dfs_time++;
	vertices[u].f = dfs_time;
}

void dfs(DFSVertex* vertices, int nelem) {
	for (int u = 0; u < nelem; u++) {
		vertices[u].super.color = WHITE;
		vertices[u].super.parent = -1;
	}
	dfs_time = 0;
	for (int u = 0; u < nelem; u++) {
		if (vertices[u].super.color == WHITE) {
			dfs_visit(vertices, nelem, u);
		}
	}
}

struct Node {
	int val1;
	int val2;
	struct Node* next;
};

struct Node* alloc_node() {
	struct Node* node = (struct Node*)malloc(sizeof(struct Node));
	node->next = NULL;
	return node;
}
struct Node* insert_node(struct Node* list, int val1, int val2) {
	struct Node* node = alloc_node();
	node->val1 = val1;
	node->val2 = val2;
	node->next = list;
	return node;
}

void DFStranspose(DFSVertex* vertices, int nelem) {

	struct Node* list = NULL;

	for (int u = 0; u < nelem; u++) {
		for (Adj* adj_v = vertices[u].super.first; adj_v; adj_v = adj_v->next) {
			list = insert_node(list, adj_v->n, u);
		}
	}
	for (int u = 0; u < nelem; u++) {
		vertices[u].super.first = NULL;
	}
	for (struct Node* p = list; p != NULL; p = p->next) {
		Vertex_add(&vertices[p->val1].super, &vertices[p->val2].super);
	}
}

#define NELEM 10000

void main() {

	int slct;
	int run_main = 1;

	char rcv_ID1[20];
	char rcv_ID2[20];
	char rcv_s[20];

	int count1;
	int count2;

	BFSVertex B_DATA[NELEM];
	DFSVertex D_DATA[NELEM];
	for (int i = 0; i < NELEM; i++) {
		BFSVertex_init(&B_DATA[i]);
	}
	for (int i = 0; i < NELEM; i++) {
		DFSVertex_init(&D_DATA[i]);
	}

	while (run_main) {

		printf("\n***** Interface\n0.0. Read data files");
		printf("\n* 0. Read data files");
		printf("\n* 1. display statistics");
		printf("\n* 2. Top 5 most tweeted words");
		printf("\n* 3. Top 5 most tweeted users");
		printf("\n* 4. Find users who tweeted a word(e.g., ¡¯¿¬¼¼´ë¡¯)");
		printf("\n* 5. Find all people who are friends of the above users");
		printf("\n* 6. Delete all mentions of a word");
		printf("\n* 7. Delete all users who mentioned a word");
		printf("\n* 8. Find strongly connected components");
		printf("\n* 9. Find shortest path from a given user");
		printf("\n* 99. Quit ");
		printf("\n***** Select Menu: ");
		scanf_s("%d", &slct);

		FILE* friend = fopen("C:\\friend.txt", "r");
		FILE* user = fopen("C:\\user.txt", "r");
		FILE* word = fopen("C:\\word.txt", "r");

		if (friend == NULL) return;

		while (!feof(friend))
		{
			fscanf(friend, "%s", rcv_ID1);
			fscanf(friend, "%s", rcv_ID2);
			printf("%s %s\n", rcv_ID1, rcv_ID2);

			for (count1 = 0; count1 < NELEM; count1++) {
				if (strcmp(B_DATA[count1].super.name, "(none)") || strcmp(B_DATA[count1].super.name, rcv_ID1)) {
					if (strcmp(B_DATA[count1].super.name, "(none)")) {
						strcpy(B_DATA[count1].super.name, rcv_ID1);
						strcpy(D_DATA[count1].super.name, rcv_ID1);
						B_DATA[count1].super.n = count1;
						D_DATA[count1].super.n = count1;
					}
					break;
				}
			}
			for (count2 = 0; count2 < NELEM; count2++) {
				if (strcmp(B_DATA[count2].super.name, "(none)") || strcmp(B_DATA[count2].super.name, rcv_ID2)) {
					if (strcmp(B_DATA[count2].super.name, "(none)")) {
						strcpy(B_DATA[count2].super.name, rcv_ID2);
						strcpy(D_DATA[count2].super.name, rcv_ID2);
						B_DATA[count2].super.n = count2;
						D_DATA[count2].super.n = count2;
					}
					break;
				}
			}
			Vertex_add(&B_DATA[count1].super, count2);

		}
		for (int i = 0; i < NELEM; i++) {
			if (strcmp(B_DATA[i].super.name, "(none)")) {
				print_bfsvertex(B_DATA, i);
			}
		}

		fclose(friend);
		fclose(user);
		fclose(word);

		switch (slct)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 99:
			run_main = 0;
			printf("\n*****End of the program...bye\n\n");
			break;
		default:
			printf("\n!error : wrong data has been entered\n\n");
			break;
		}
	}
}