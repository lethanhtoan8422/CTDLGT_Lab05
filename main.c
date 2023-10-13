#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AUTHORS 5

typedef struct
{
    char title[100];
    char authors[MAX_AUTHORS][100];
    char publisher[100];
    int year;
} Book;

typedef struct Node
{
    Book data;
    struct Node* next;
} Node;

typedef struct
{
    Node* head;
    Node* tail;
} List;

void initList(List *l)
{
    l->head = NULL;
    l->tail = NULL;
}

void saveListToFile(List* list, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Khong the mo file.\n");
        return;
    }

    Node* current = list->head;
    while (current != NULL)
    {
        fprintf(file, "Ten sach : %s\n", current->data.title);
        fprintf(file, "Tac gia : ");
        for (int i = 0; i < MAX_AUTHORS; i++)
        {
            if (strlen(current->data.authors[i]) > 0)
            {
                if(i + 1 == MAX_AUTHORS){
                    fprintf(file, "%s", current->data.authors[i]);
                }
                else{
                    fprintf(file, "%s, ", current->data.authors[i]);
                }
            }
        }
        fprintf(file, "\n");
        fprintf(file, "Nha xuat ban : %s\n", current->data.publisher);
        fprintf(file, "Nam xuat ban : %d\n", current->data.year);
        fprintf(file, "\n");
        current = current->next;
    }

    fclose(file);
}

void readListFromFile(List* list, const char* filename)
{
    initList(list);
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Khong the mo file.\n");
        return;
    }

    Book book;
    char line[500];
    while (fgets(line, sizeof(line), file))
    {
        printf("line : %s\n", line);
        if(strncmp(line, "Ten sach : ",11) == 0){
            fscanf(file, "Ten sach : %s", book.title);
            printf("title : %s\n", book.title);
        }
        else if(strncmp(line, "Tac gia : ",10) == 0){
            char tacGia[500];
            fscanf(file, "Tac gia : %[^\n]", tacGia);
            char *token = strtok(tacGia, ",");
            int i = 0;
            while(token != NULL){
                if(strcmp(token, " ") != 0){
                    strcpy(book.authors[i], token);
                    i++;
                }
                token = strtok(NULL, ",");
            }
            printf("tacGia : %s\n", tacGia);
        }
        else if(strncmp(line, "Nha xuat ban : ",15) == 0){
            fscanf(file, "Nha xuat ban : %[^\n]", book.publisher);
            printf("publisher : %s\n", book.publisher);
        }
        else if(strncmp(line, "Nam xuat ban : ",15) == 0){
            fscanf(file, "Nam xuat ban : %d", &book.year);
            printf("year : %d\n", book.year);
            addBookToList(list, book);
        }
    }

    fclose(file);
}

Book createBook()
{
    Book book;
    printf("Nhap thong tin cua quyen sach:\n");
    printf("Ten sach: \n");
    fflush(stdin);
    gets(book.title);
    printf("Tac gia(Toi da 5 tac gia, nhap 0 de ket thuc) : \n");
    char tacGia[100];
    for (int i = 0; i < MAX_AUTHORS; i++)
    {
        printf("Tac gia %d: \n", i + 1);
        fflush(stdin);
        gets(tacGia);
        if(strcmp(tacGia, "0") != 0)
        {
            strcpy(book.authors[i], tacGia);
        }
        else
        {
            break;
        }
    }
    printf("Nha xuat ban: \n");
    fflush(stdin);
    gets(book.publisher);
    printf("Nam xuat ban: \n");
    scanf("%d", &book.year);
    printf("Da them Book vao List.\n");
    return book;
}

void addBookToList(List* list, Book book)
{
    Node* newNode = malloc(sizeof(Node));
    newNode->data = book;
    newNode->next = NULL;

    if (list->head == NULL)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        list->tail->next = newNode;
        list->tail = newNode;
    }
}

int countBooksByAuthor(List* list, const char* author)
{
    int count = 0;
    Node* current = list->head;
    while (current != NULL)
    {
        for (int i = 0; i < MAX_AUTHORS; i++)
        {
            if (strcmp(current->data.authors[i], author) == 0)
            {
                count++;
            }
        }
        current = current->next;
    }
    return count;
}

void findBooksByPublisherAndYear(List* list, const char* publisher, int year)
{
    Node* current = list->head;
    while (current != NULL)
    {
        if (strcmp(current->data.publisher, publisher) == 0 && current->data.year == year)
        {
            printf("Title: %s", current->data.title);
            for (int i = 0; i < MAX_AUTHORS; i++)
            {
                if (strlen(current->data.authors[i]) > 0)
                {
                    printf("Author: %s", current->data.authors[i]);
                }
            }
            printf("Publisher: %s", current->data.publisher);
            printf("Year: %d\n\n", current->data.year);
        }
        current = current->next;
    }
}

void removeBooksByAuthor(List* list, const char* author, const char* filename)
{
    Node* current = list->head;
    Node* previous = NULL;

    while (current != NULL)
    {
        int found = 0;
        for (int i = 0; i < MAX_AUTHORS; i++)
        {
            if (strcmp(current->data.authors[i], author) == 0)
            {
                found = 1;
                break;
            }
        }

        if (found)
        {
            if (previous == NULL)
            {
                list->head = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            Node* temp = current;
            current = current->next;
            free(temp);
        }
        else
        {
            previous = current;
            current = current->next;
        }
    }

    saveListToFile(list, filename);
}

void countBooksByAuthors(List* list, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Khong the mo file.\n");
        return;
    }

    int counts[MAX_AUTHORS] = {0};

    Node* current = list->head;
    while (current != NULL)
    {
        for (int i = 0; i < MAX_AUTHORS; i++)
        {
            if (strlen(current->data.authors[i]) > 0)
            {
                counts[i]++;
            }
        }
        current = current->next;
    }

    fprintf(file, "Thong ke so luong sach cua moi tac gia:\n");
    for (int i = 0; i < MAX_AUTHORS; i++)
    {
        if (strlen(list->head->data.authors[i]) > 0)
        {
            fprintf(file, "Tac gia %d: %s - So luong sach: %d\n", i + 1, list->head->data.authors[i], counts[i]);
        }
    }

    fclose(file);
}

void printStatistics(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Khong the mo file.\n");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }

    fclose(file);
}

void freeList(List* list)
{
    Node* current = list->head;
    while (current != NULL)
    {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
    list->tail = NULL;
}

void displayList(List list)
{
    Node* current = list.head;
    printf("---------------Danh Sach Book---------------\n");
    while (current != NULL)
    {
        printf("Ten Sach : %s\n", current->data.title);
        printf("Tac gia : ");
        for(int i = 0; i < 5; i++)
        {
            if(strlen(current->data.authors[i]) > 0)
            {
                printf("%s, ", current->data.authors[i]);
            }
            else
            {
                break;
            }
        }
        printf("\n");
        printf("Nha san xuat : %s\n", current->data.publisher);
        printf("Nam san xuat : %d\n", current->data.year);
        printf("\n");
        current = current->next;
    }
    printf("---------------Ket Thuc---------------\n");
}

int main()
{
    List bookList;
    initList(&bookList);
    Book book;

    int choice;
    char author[100];
    char publisher[100];
    int year;

    do
    {
        printf("----- MENU -----\n");
        printf("1. Ghi List vao FILE library.txt\n");
        printf("2. Doc du lieu tu FILE library.txt ra List\n");
        printf("3. Them Book vao List\n");
        printf("4. Cho biet so luong cac quyen sach cua mot tac gia bat ky\n");
        printf("5. Trong nam YYYY, nha xuat ban ABC da phat hanh nhung quyen sach nao\n");
        printf("6. Xoa tat ca cac quyen sach cua tac gia X\n");
        printf("7. Thong ke so luong sach cua moi tac gia va ghi ket qua thong ke len tep\n");
        printf("8. Doc du lieu thong ke tu tep va xuat len man hinh\n");
        printf("9. Doc du lieu tu tep va dua vao danh sach\n");
        printf("10.Hien thi danh sach\n");
        printf("0. Thoat\n");
        printf("Nhap lua chon cua ban: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            saveListToFile(&bookList, "library.txt");
            printf("Da ghi List vao FILE library.txt.\n");
            break;
        case 2:
            readListFromFile(&bookList, "library.txt");
            printf("Da doc du lieu tu FILE library.txt ra List.\n");
            break;
        case 3:
            book = createBook();
            addBookToList(&bookList, book);
            break;
        case 4:
            printf("Nhap ten tac gia: ");
            scanf("\n");
            fgets(author, sizeof(author), stdin);
            author[strcspn(author, "\n")] = '\0'; // Xoa ky tu newline (\n) trong chuoi
            int count = countBooksByAuthor(&bookList, author);
            printf("So luong sach cua tac gia %s: %d\n", author, count);
            break;
        case 5:
            printf("Nhap ten nha xuat ban: ");
            scanf("\n");
            fgets(publisher, sizeof(publisher), stdin);
            publisher[strcspn(publisher, "\n")] = '\0'; // Xoa ky tu newline (\n) trong chuoi
            printf("Nhap nam xuat ban: ");
            scanf("%d", &year);
            printf("Cac quyen sach cua nha xuat ban %s trong nam %d:\n", publisher, year);
            findBooksByPublisherAndYear(&bookList, publisher, year);
            break;
        case 6:
            printf("Nhap ten tac gia: ");
            scanf("\n");
            fgets(author, sizeof(author), stdin);
            author[strcspn(author, "\n")] = '\0'; // Xoa ky tu newline (\n) trong chuoi
            removeBooksByAuthor(&bookList, author, "library.txt");
            printf("Da xoa tat ca cac quyen sach cua tac gia %s.\n", author);
            break;
        case 7:
            countBooksByAuthors(&bookList, "statistics.txt");
            printf("Da thong ke so luong sach cua moi tac gia va ghi ket qua vao FILE statistics.txt.\n");
            break;
        case 8:
            printf("Du lieu thong ke:\n");
            printStatistics("statistics.txt");
            break;
        case 9:
            readListFromFile(&bookList, "data.txt");
            printf("Da doc du lieu tu FILE data.txt ra List.\n");
            break;
        case 10:
            displayList(bookList);
            break;
        case 0:
            printf("Ket thuc chuong trinh.\n");
            break;
        default:
            printf("Lua chon khong hop le. Vui long chon lai.\n");
        }
    }
    while (choice != 0);

    freeList(&bookList);

    return 0;
}
