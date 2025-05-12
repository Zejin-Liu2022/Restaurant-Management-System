#include<stdio.h>

typedef struct People
{
	int id, identity;
	char Name[20];
	struct People *left, *right;
	int house_id;
}People;

int house_index;
int ascending_cost_house_id[101], freehouse;
struct House
{
	int id, cost;
	int purchase;//0 -> not buy, 1 -> bought
	int use;//3 -> free, 1 -> live, 2 -> lent
}house[101];

People membership_head, membership_end;

void init();

void Membership();
void add_P(People* head, int identity);
void del_P(People* head, int id);
People* find_P(People* head, int id);

void Housing_Management();
void add_H(int id);
int binary_search(int k, void (*mode)(int k, int *l, int *r));
void search_low(int k, int *l, int *r);
void search_high(int k, int *l, int *r);

void Purchase();

void Checkin_system();

void print();

int main()
{
    init();
    puts("Welcome to the Vitality Elderly Community System!\n");
    puts("Please click on the corresponding key to select your service");

    while(1)
    {
		puts("\n1 -> Membership system\t2 -> House system\t3 -> Purchase system");
    	puts("4 -> Check in system\t5 -> print to txt\t# -> Prompt message\t0 -> End");
		char key[4];
		scanf("%s", key);

		switch(key[0])
		{
			case '0' :
				return 0;
			case '1' :
				Membership();
				break;
			case '2' :
				Housing_Management();
				break;
			case '3' :
				Purchase();
				break;
			case '4' :
				Checkin_system();
				break;
            case '5' :
                print();
                break;
			case '#' :
				continue;
			default :
				puts("error input");
		}

    }
    return 0;
}

void init()
{
	membership_head.left = membership_end.right = NULL;
	membership_head.right = &membership_end;
	membership_end.left = &membership_head;
	return;
}

void Membership()
{
	while(1)
	{
		puts("\n1 -> creat\t2 -> delete");
		puts("3 -> find\t4 -> modify\n0 -> end");
		int key;scanf("%d", &key);
		if(key == 1)
        {
            add_P(&membership_head, 1);
            puts("Add Successfully");
        }
		else if(key == 2)
		{
			puts("input he/she id");
			int id;scanf("%d", &id);
			del_P(&membership_head, id);
			puts("Delete Successfully");
		}
		else if(key == 3)
		{
			puts("input he/she id");
			int id;scanf("%d", &id);
			People* t = find_P(&membership_head, id);
			if(t == NULL) puts("Not find");
			else if(t->identity == 1)puts("He/She is member");
			else puts("He/She is NOT member");
		}
		else if(key == 4)
		{
			puts("input he/she id");
			int id;scanf("%d", &id);
			People* t = find_P(&membership_head, id);
			if(t == NULL)
            {
                puts("Not found");
                continue;
            }

			puts("0 -> Cancel member\t1 -> turn to member");
			int identity;scanf("%d", &identity);
			t->identity = identity;
			puts("Modified Successfully");
		}
		else return;
	}
}

void add_P(People* head, int identity)
{
	People* t = (People *)malloc(sizeof(People));

	if(t == NULL)
	{
		puts("memory error");
		return;
	}

	puts("Please input id & name");

	scanf("%d%s", &(t->id), t->Name);
	t->identity = identity;
	t->house_id = -1;

	t->right = head->right; t->left = head;
	head->right->left = t; head->right = t;

	return;
}

void del_P(People* head, int id)
{
	People* i = find_P(head, id);

	if(i == NULL) puts("404");

	i->right->left = i->left; i->left->right = i->right;

	free(i);

	return;
}

People* find_P(People* head, int id)
{
	for(People* i = head; i != NULL; i = i->right)
		if(i->id == id)
			return i;
	return NULL;
}

void Housing_Management()
{
	while(1)
	{
		puts("\n1 -> creat\t0 -> end");

		int key;scanf("%d", &key);

		if(key == 1)
		{
			puts("input house id");
			int id;scanf("%d", &id);
			add_H(id);
		}
		else return;
	}
}

void add_H(int id)
{
	struct House *t = &house[++house_index];

	puts("input house cost");
	int cost;scanf("%d", &cost);

	t->id = id; t->cost = cost;
	t->purchase = 0; t->use = 3;

	int *a = ascending_cost_house_id;
	a[house_index] = house_index;
	for(int i = house_index; i > 1; )
	{
		if(house[a[i]].cost < house[a[i - 1]].cost)
		{
			a[i] = a[i] ^ a[i - 1];
			a[i - 1] = a[i] ^ a[i - 1];
			a[i] = a[i] ^ a[i - 1];
			i--;
		}
		else break;
	}
}

void Purchase()
{
	while(1)
	{
	    if(house_index == 0)
        {
            puts("sorry NO house");
            break;
        }
		puts("\n1 -> purchase\t0 -> end");
		int k;scanf("%d", &k);
		if(!k) break;

		puts("\nPlease input your id");
		int id;scanf("%d", &id);
		People* fp = find_P(&membership_head, id);
		if(fp == NULL)
		{
			puts("find err, try again");
			continue;
		}
		if(fp->identity != 1)
        {
            puts("sorry you are NOT member");
            continue;
        }
		puts("\nPlease input your ideal cost");
		int cost;scanf("%d", &cost);

		int l = binary_search(cost, search_low);
		int r = binary_search(cost, search_high);

		int *a = ascending_cost_house_id;
		for(int i = 0, j = 1; i <= r - l; i++, j++)
        {
            printf("%d: cost %d, id %d, ", j, house[a[l + i]].cost, house[a[l + i]].id);
            if(house[a[l + i]].purchase){
				puts("owned");
			}else puts("free");
        }

		puts("which one you choose,0 -> end");

		int key;scanf("%d", &key);
		if(key + l - 1 <= r && key >= 1 && !house[a[l + key - 1]].purchase)
        {
            house[a[l + key - 1]].purchase = 1;
            fp->house_id = a[l + key - 1];
            puts("Purchase successful");
        }
		else puts("Purchase failed, please try again");
	}
	return;
}

int binary_search(int k, void (*mode)(int k,int *l, int *r))
{
	int l = 1, r = house_index;
	while(l < r)
	{
		(*mode)(k, &l, &r);
	}
	return l;
}

void search_low(int k, int *l, int *r)
{

    int *a = ascending_cost_house_id;
    int mid = (*l + *r) >> 1;
    if(house[a[mid]].cost < k) *l = mid + 1;
    else *r = mid;
}

void search_high(int k, int *l, int *r)
{
    int *a = ascending_cost_house_id;
    int mid = (*l + *r + 1) >> 1;
    if(house[a[mid]].cost <= k) *l = mid;
    else *r = mid - 1;
}

void Checkin_system()
{
	while(1)
	{
		puts("\n1 -> check in\t0 -> end");
		int k;scanf("%d", &k);
		if(!k) break;

		puts("\nPlease input your id");
		int id;scanf("%d", &id);

		People* fp = find_P(&membership_head, id);
		if(fp == NULL)
        {
            puts("find err, try again");
            continue;
        }

		if(fp->house_id != -1)
		{
			puts("What do you want to do to your house?");
			puts("1 -> check in\t2 -> rent");
			puts("3 -> free\t0 -> end");

			int key;scanf("%d", &key);
			if(!key) continue;
			house[fp->house_id].use = key;
			puts("successful");
		}
		else puts("sorry you didn't have house");
	}
	return;
}

void print()
{
    FILE* fp;
    fp = fopen("system_print.txt","w");

    if(fp == NULL)
    {
        puts("file open err");
        return;
    }
    else
    {
        fprintf(fp, "member:\n");
        for(People* i = (&membership_head)->right; i != &membership_end; i = i->right)
        {
            fprintf(fp, "\t%s id: %d, ", i->Name, i->id);
            if(i->house_id == -1)
                fprintf(fp, "house:-1\n");
            else
                fprintf(fp, "house: %d\n", house[i->house_id].id);
        }


        fprintf(fp, "house:\n");
        for(int i = 1; i <= house_index; i++)
        {
            fprintf(fp, "\tid: %d, cost: %d, ", house[i].id, house[i].cost);
            if(!house[i].purchase || house[i].use == 3)
                fprintf(fp,"free\n");
            else
            {
                if(house[i].use == 1)
                    fprintf(fp,"live\n");
                if(house[i].use == 2)
                    fprintf(fp,"lent\n");
            }
        }
    }
    fclose(fp);
    printf("Print successful");
}
