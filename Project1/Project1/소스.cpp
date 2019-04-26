#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef struct tree {

	char *word;
	char *clas;
	char *meaning;
	struct tree * left;
	struct tree * right;

}node;

char buffer[5000];
char *temw, *temcla, *temmean;
node *head = NULL;
int n = 0;
int d = 0;
char *dele[3000];


void size();
node* newnod(char *w, char *cla, char *mean);
void insert(node *root, char *w, char *cla, char *mean);
void pre(node * root);
void add(node* root);
void find(node* root, char *w);
node* succer(node* root, node* per);
int delet(node* root, node *per, char* w);
void read_shuffled();
void read_to_be();
void deleteall(node* root);

int main(void) {

	int x;

	read_shuffled();

	while (1) {

		printf("숫자를 입력하시오 (1)size (2)find (3)add (4)delete (5)deleteall (0)exit:");
		scanf_s("%d", &x);
		rewind(stdin);
		if (x == 1)
			size();
		else if (x == 2)
			find(head, NULL);
		else if (x == 3)
			add(head);
		else if (x == 4)
			delet(head, NULL, NULL);
		else if (x == 5)
			deleteall(head);
		else if (x == 0)
			break;
	}


	printf("\n종료\n");
	return 0;
}

void read_shuffled() {
	FILE *fp;
	errno_t err;
	char *context = NULL;

	err = fopen_s(&fp, "shuffled_dict.txt", "r");
	if (err == 0)
		puts("shuffled_dict 파일오픈 성공(기다려 주세요)\n");
	else
		puts("shuffled_dict 파일 오픈 실패\n");

	while (fgets(buffer, 5000, fp) != NULL) {
		n++;
		temw = strtok_s(buffer, "(", &context);
		temcla = strtok_s(NULL, ")", &context);
		temmean = strtok_s(NULL, "\n", &context);
		temw[strlen(temw) - 1] = '\0'; //단어마지막공백삭제

		insert(head, temw, temcla, temmean);

	}
}

void pre(node * root) {
	if (root == NULL)return;
	pre(root->left);
	printf("%s ", root->word);
	printf("(%s) ", root->clas);
	if (root->meaning != NULL)
		printf("%s\n", root->meaning);
	pre(root->right);
}

node* newnod(char *w, char *cla, char *mean) {

	node *newnode = (node*)malloc(sizeof(node));

	newnode->word = _strdup(w);
	newnode->clas = _strdup(cla);
	newnode->meaning = _strdup(mean);
	newnode->right = NULL;
	newnode->left = NULL;


	return newnode;
}



void insert(node *root, char *w, char *cla, char *mean) {

	int cmp;


	if (islower(w[0]))w[0] = toupper(w[0]);  //첫글자 대문자로


	if (head == NULL) {
		head = newnod(w, cla, mean);
		return;
	}

	cmp = strcmp(root->word, w);

	if (cmp > 0)
	{
		if (root->left == NULL) {
			root->left = newnod(w, cla, mean);
			return;
		}
		insert(root->left, w, cla, mean);

	}
	else if (cmp < 0)
	{
		if (root->right == NULL) {
			root->right = newnod(w, cla, mean);
			return;
		}
		insert(root->right, w, cla, mean);
	}
	else {  //같은 단어가 있을시 오른쪽 노드에 추가
		node * newn = newnod(w, cla, mean);
		newn->right = root->right;
		root->right = newn;
	}

}

void add(node* root) {

	char w[1000], cl[1000] = {}, me[3000];

	printf("단어 입력:");

	scanf_s("%[^\n]s", w, sizeof(w)); // 공백포함한 문자열 받음

	if (islower(w[0]))w[0] = toupper(w[0]);  //첫글자 대문자로

	printf("\n품사 입력:");

	rewind(stdin);
	scanf_s("%[^\n]s", cl, sizeof(cl));

	printf("\n뜻 입력:");

	rewind(stdin);
	scanf_s("%[^\n]s", me, sizeof(me));

	rewind(stdin);
	insert(root, w, cl, me);

	n++;
}


void size() {
	printf("저장된 단어 갯수 : %d\n", n);
}


void find(node* root, char *w) {

	char fw[1000];
	int cmp;


	if (root == NULL) {
		printf("찾는 단어가 없습니다.\n");
		return;
	}

	if (w == NULL) {    //함수가 처음 호출됐을 때만 실행됨

		printf("찾고싶은 단어 입력:");

		scanf_s("%[^\n]s", fw, sizeof(fw));

		if (islower(fw[0]))fw[0] = toupper(fw[0]);

		w = fw;

		rewind(stdin);
	}

	cmp = strcmp(root->word, w);



	if (cmp > 0)
		find(root->left, w);
	else if (cmp < 0)
		find(root->right, w);
	else
	{
		printf("word : %s\n", root->word);
		printf("class : %s\n", root->clas);
		printf("meaning : %s\n\n", root->meaning);

	}

}



int delet(node* root, node *per, char* w) {

	int cmp;
	char fw[1000];

	if (root == NULL) {
		printf("삭제 실패\n");
		return 0;
	}

	if (w == NULL) {
		printf("삭제할 단어를 입력하시오:");

		scanf_s("%[^\n]s", fw, sizeof(fw));

		if (islower(fw[0]))fw[0] = toupper(fw[0]);

		w = fw;

		rewind(stdin);
	}

	cmp = strcmp(root->word, w);

	if (cmp > 0)
		return delet(root->left, root, w);
	else if (cmp < 0)
		return delet(root->right, root, w);
	else
	{
		if (root->left == NULL && root->right == NULL)
		{
			if (root == head) {
				head = NULL;
			}
			else {
				if (per->right == root)per->right = NULL;
				else per->left = NULL;
			}
		}
		else if (root->left == NULL || root->right == NULL)
		{
			if (root == head) {
				if (root->right == NULL)
					head = root->left;
				else
					head = root->right;
			}
			else {
				if (root->right == NULL) {
					if (per->right == root)per->right = root->left;
					else per->left = root->left;

				}
				else {
					if (per->right == root)per->right = root->right;
					else per->left = root->right;
				}
			}
		}
		else {

			if (root->right->left == NULL)
			{

				root->right->left = root->left;
				if (root == head) {
					head = root->right;
				}
				else {
					if (per->right == root)per->right = root->right;
					else per->left = root->right;
				}
			}
			else {
				node* su = succer(root->right->left, root->right);
				su->left = root->left;
				su->right = root->right;

				if (root == head) {
					head = su;
				}
				else {
					if (per->right == root)per->right = su;
					else per->left = su;
				}
			}
		}
		free(root);
		n--;
		return 1;
	}

}

node* succer(node* root, node* per) {
	if (root->left == NULL) {
		per->left = root->right;
		return root;
	}
	return succer(root->left, per);
}

void read_to_be() {

	FILE *fp;
	errno_t err;

	err = fopen_s(&fp, "to_be_deleted_words.txt", "r");
	if (err == 0)
		puts("to_be_deleted_words 파일오픈 성공(기다려 주세요)\n");
	else
		puts("to_be_deleted_words 파일 오픈 실패\n");


	while (fgets(buffer, 5000, fp) != NULL) {
		dele[d] = _strdup(buffer);
		dele[d][strlen(dele[d]) - 1] = '\0'; //단어 마지막에 들어있는 '\n'제거
		d++;
	}

}

void deleteall(node* root) {

	int total = 0;

	read_to_be();
	for (int i = 0; i < d; i++)total += delet(root, NULL, dele[i]);
	printf("%d개의 단어 삭제 성공\n", total);

}