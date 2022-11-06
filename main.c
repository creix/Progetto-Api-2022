#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NEW_GAME "+n"
#define START_INSERT 'i'
#define END_INSERT "+i"
#define REMAINING 's'
#define COMMAND '+'
#define MAX_COMMAND_SIZE 17
#define ASCII_RANGE 78
#define RED 1
#define BLACK 0

struct stack_node
{
    char *word;
    struct stack_node *next;
};

struct rb_node
{
    struct rb_node *parent;
    struct rb_node *left;
    struct rb_node *right;
    bool color;
    char word[];
};

struct rb_node T_Nil_Node;
struct rb_node* T_Nil = &T_Nil_Node;

bool notInWord[ASCII_RANGE];
char *correctPosition;
int exactCount[ASCII_RANGE];
int minimumCount[ASCII_RANGE];
bool (*possiblePosition)[ASCII_RANGE];

void push(struct stack_node **head, char *word)
{
    struct stack_node *tmp = malloc(sizeof(struct stack_node));

    tmp->word = word;
    tmp->next = (*head);
    (*head) = tmp;
}

void divideList(struct stack_node *dictionary, struct stack_node **head, struct stack_node **tail)
{
    struct stack_node *ptr1;
    struct stack_node *ptr2;

    ptr2 = dictionary;
    ptr1 = dictionary->next;

    while(ptr1 != NULL)
    {
        ptr1 = ptr1->next;

        if(ptr1 != NULL)
        {
            ptr2 = ptr2->next;
            ptr1 = ptr1->next;
        }
    }

    *head = dictionary;
    *tail = ptr2->next;
    ptr2->next = NULL;
}

struct stack_node *merge(struct stack_node *stack1, struct stack_node *stack2)
{
    struct stack_node *complete = NULL;
    struct stack_node *tmp = NULL;

    if(strcmp(stack1->word, stack2->word) < 0)
    {
        complete = stack1;
        stack1 = stack1->next;
    }
    else
    {
        complete = stack2;
        stack2 = stack2->next;
    }

    tmp = complete;

    while(stack1 != NULL && stack2 != NULL)
    {
        if(strcmp(stack1->word, stack2->word) < 0)
        {
            tmp->next = stack1;
            stack1 = stack1->next;
        }
        else
        {
            tmp->next = stack2;
            stack2 = stack2->next;
        }

        tmp = tmp->next;
    }

    if(stack1 != NULL)
        tmp->next = stack1;

    if(stack2 != NULL)
        tmp->next = stack2;

    return complete;
}

void mergeSort(struct stack_node **dictionary)
{
    struct stack_node *ptr1;
    struct stack_node *ptr2;

    if(dictionary == NULL || (*dictionary)->next == NULL)
        return;

    divideList(*dictionary, &ptr1, &ptr2);

    mergeSort(&ptr1);
    mergeSort(&ptr2);

    *dictionary = merge(ptr1, ptr2);
}

struct rb_node* rb_createNode(char *word, int wordLength)
{
    struct rb_node* newNode = (struct rb_node*)malloc(sizeof(struct rb_node) + (wordLength + 1));
    strcpy(newNode->word, word);
    newNode->parent = T_Nil;
    newNode->left = T_Nil;
    newNode->right = T_Nil;
    newNode->color = RED;

    return (newNode);
}

void rbRotateLeft(struct rb_node **dictionary, struct rb_node *rotationNode)
{
    struct rb_node *tmp  = rotationNode->right;
    rotationNode->right = tmp->left;

    if (tmp->left != T_Nil)
        tmp->left->parent = rotationNode;

    tmp->parent = rotationNode->parent;

    if (rotationNode->parent == T_Nil)
        *dictionary = tmp;
    else if (rotationNode == rotationNode->parent->left)
        rotationNode->parent->left = tmp;
    else
        rotationNode->parent->right = tmp;

    tmp->left   = rotationNode;
    rotationNode->parent = tmp;
}

void rbRightRotate(struct rb_node **dictionary, struct rb_node *rotationNode)
{
    struct rb_node *tmp  = rotationNode->left;
    rotationNode->left  = tmp->right;

    if (tmp->right != T_Nil)
        tmp->right->parent = rotationNode;

    tmp->parent = rotationNode->parent;

    if (rotationNode->parent == T_Nil)
        *dictionary = tmp;
    else if (rotationNode == rotationNode->parent->right)
        rotationNode->parent->right = tmp;
    else
        rotationNode->parent->left  = tmp;

    tmp->right  = rotationNode;
    rotationNode->parent = tmp;
}

void rbInsertFixup(struct rb_node **dictionary, struct rb_node *node)
{
    struct rb_node *tmp;

    while (node->parent->color == RED)
    {
        if (node->parent == node->parent->parent->left)
        {
            tmp = node->parent->parent->right;

            if (tmp->color == RED)
            {
                node->parent->color = BLACK;
                tmp->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->right)
                {
                    node = node->parent;
                    rbRotateLeft(dictionary, node);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rbRightRotate(dictionary, node->parent->parent);
            }
        }
        else
        {
            tmp = node->parent->parent->left;

            if (tmp->color == RED)
            {
                node->parent->color = BLACK;
                tmp->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->left)
                {
                    node = node->parent;
                    rbRightRotate(dictionary, node);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rbRotateLeft(dictionary, node->parent->parent);
            }
        }
    }

    (*dictionary)->color = BLACK;
}

struct rb_node *rbInsert(struct rb_node **dictionary, char *word, int wordLength)
{
    struct rb_node* newNode = rb_createNode(word, wordLength);
    struct rb_node *pre =  T_Nil;
    struct rb_node *curr = *dictionary;

    while (curr != T_Nil) {
        pre = curr;
        if (strcmp(word, curr->word) < 0)
            curr = curr->left;
        else
            curr = curr->right;
    }

    newNode->parent = pre;

    if (pre == T_Nil)
        *dictionary = newNode;
    else if (strcmp(word, pre->word) < 0)
        pre->left  = newNode;
    else
        pre->right = newNode;

    rbInsertFixup(dictionary, newNode);

    return newNode;
}

bool searchDictionary(struct rb_node *curr, char *word)
{
    if(curr == T_Nil)
        return false;
    else if(strcmp(curr->word, word) == 0)
        return true;

    if(strcmp(word, curr->word) > 0)
        return searchDictionary(curr->right, word);
    else
        return searchDictionary(curr->left, word);
}

void checkWord(int wordLength, char *ordered, char *orderedRef, int originalIndex[], int originalIndexRef[], int count[], int countRef[])
{
    int i=0;
    int j=0;
    int k,z;
    int sum = 0;
    int sumRef = 0;
    int match = 0;
    int remaining;
    char output[wordLength+1];
    int currentMinimumCount;
    bool found;

    for(k=0; k<wordLength; k++)
        output[k] = '*';

    output[wordLength] = '\0';

    while(i<wordLength && j<wordLength)
    {
        if(ordered[i] == orderedRef[j])
        {
            sum = i + count[ordered[i] - '-'];
            sumRef = j + countRef[ordered[i] - '-'];
            match = 0;
            currentMinimumCount = 0;

            for(k = i; k < sum; k++)
            {
                found = false;

                for(z = j; z < sumRef && found == false; z++)
                {
                    if(originalIndex[k] == originalIndexRef[z])
                    {
                        output[originalIndex[k]] = '+';
                        correctPosition[originalIndex[k]] = ordered[i];
                        currentMinimumCount++;
                        match++;
                        found = true;
                    }
                }
            }

            remaining = countRef[ordered[i] - '-'] - match;

            while(i<sum)
            {
                if(output[originalIndex[i]] != '+')
                {
                    if(remaining > 0) {
                        output[originalIndex[i]] = '|';
                        possiblePosition[originalIndex[i]][ordered[i] - '-'] = 0;
                        currentMinimumCount++;
                        remaining--;
                    }
                    else
                    {
                        output[originalIndex[i]] = '/';
                        exactCount[ordered[i] - '-'] = countRef[ordered[i] - '-'];
                        possiblePosition[originalIndex[i]][ordered[i] - '-'] = 0;
                    }
                }

                i++;
            }

            if(currentMinimumCount > minimumCount[orderedRef[j] - '-'])
                minimumCount[orderedRef[j] - '-'] = currentMinimumCount;

            j = sumRef;

        }
        else if(ordered[i]<orderedRef[j])
        {
            output[originalIndex[i]] = '/';
            notInWord[ordered[i] - '-'] = true;
            i++;
        }
        else
            j++;
    }

    if(j>=wordLength)
    {
        while(i<wordLength)
        {
            output[originalIndex[i]] = '/';
            notInWord[ordered[i] - '-'] = true;
            i++;
        }
    }

    printf("%s\n", output);
}

void countingWord(char *word, char *ordered, int wordLength, int originalIndex[], int count[])
{
    int i;
    int sum[ASCII_RANGE+1] = {0};

    memset(count, 0, ASCII_RANGE * sizeof(int));

    for(i = 0; i < wordLength; i++) {
        count[word[i] - '-']++;
        sum[word[i] - '-']++;
    }

    if(ordered != NULL)
    {
        for (i = 1; i <= ASCII_RANGE; i++) {
            sum[i] += sum[i - 1];
        }

        for (i = wordLength - 1; i >= 0; i--) {
            ordered[sum[word[i] - '-'] - 1] = word[i];
            originalIndex[sum[word[i] - '-'] - 1] = i;
            sum[word[i] - '-']--;
        }
    }
}

char* customRead(char *buffer, int maxLength)
{
    char *end;

    end = fgets(buffer, maxLength+2, stdin);

    if(end != NULL)
        buffer[strcspn(buffer, "\n")] = 0;

    return end;
}

void clearList(struct stack_node **dictionary)
{
    struct stack_node *tmp = *dictionary;

    while(tmp != NULL)
    {
        *dictionary = (*dictionary)->next;
        free(tmp);
        tmp = *dictionary;
    }
}

void setupNewGame(char *end, char *buffer, char *ref, int *maxWord, int maxLength, char *orderedRef, int originalIndexRef[], int wordLength, int countRef[], struct stack_node **dictionary)
{
    end = customRead(buffer, maxLength);
    strcpy(ref, buffer);

    end = customRead(buffer, maxLength);
    *maxWord = (int)strtol(buffer, &end, 10);

    countingWord(ref, orderedRef, wordLength, originalIndexRef, countRef);

    memset(minimumCount, 0, ASCII_RANGE * sizeof(int));
    memset(exactCount, 0, ASCII_RANGE * sizeof(int));
    memset(notInWord, 0, ASCII_RANGE * sizeof(bool));
    memset(possiblePosition, 1, wordLength * sizeof(bool[ASCII_RANGE]));
    memset(correctPosition, '*', wordLength * sizeof(char));
}

bool isCompatible(char *word, int wordLength)
{
    int i;
    int countWordNode[ASCII_RANGE];

    for(i=0; i<wordLength; i++)
    {
        if(notInWord[word[i] - '-'] == true || (word[i] != correctPosition[i] && correctPosition[i] != '*'))
            return false;
        else if(possiblePosition[i][word[i] - '-'] == 0)
            return false;
    }

    countingWord(word, NULL, wordLength, NULL, countWordNode);

    for(i=0; i<ASCII_RANGE; i++)
    {
        if(exactCount[i] != 0)
        {
            if(countWordNode[i] != exactCount[i])
                return false;
        }
        else if(countWordNode[i] < minimumCount[i])
            return false;

    }

    return true;
}

void filterCompatible(struct stack_node **dictionary, int wordLength, int *remainingCompatible)
{
    struct stack_node *curr = *dictionary;
    struct stack_node *prev = NULL;
    struct stack_node *succ;

    while(curr != NULL)
    {
        succ = curr->next;

        if(!isCompatible(curr->word, wordLength))
        {
            if(prev == NULL)
                *dictionary = curr->next;
            else
                prev->next = curr->next;

            free(curr);
            curr = succ;
            (*remainingCompatible)--;
        }
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }
}

void printList(struct stack_node **dictionary)
{
    struct stack_node *tmp;

    mergeSort(dictionary);

    tmp = *dictionary;

    while(tmp != NULL)
    {
        printf("%s\n", tmp->word);
        tmp = tmp->next;
    }
}

void listInsertCompatible(struct rb_node *curr, struct stack_node **dictionary, int wordLength, int *remainingCompatible)
{
    if(curr == T_Nil)
        return;
    else
    {
        if(isCompatible(curr->word, wordLength))
        {
            push(dictionary, curr->word);
            (*remainingCompatible)++;
        }
    }

    listInsertCompatible(curr->left, dictionary, wordLength, remainingCompatible);
    listInsertCompatible(curr->right, dictionary, wordLength, remainingCompatible);
}

void rbInorderVisit(struct rb_node *curr)
{
    if(curr == T_Nil)
        return;

    rbInorderVisit(curr->left);
    printf("%s\n", curr->word);
    rbInorderVisit(curr->right);
}

void rbClearTree(struct rb_node *curr)
{
    if(curr == T_Nil)
        return;

    rbClearTree(curr->left);
    rbClearTree(curr->right);

    free(curr);
}

int main() {
    int maxLength, maxWord, wordLength;
    char *ref;
    char *end;
    char *buffer;
    char *orderedRef;
    char *orderedWord;
    int *originalIndexRef;
    int *originalIndexWord;
    int countRef[ASCII_RANGE] = {0};
    int countWord[ASCII_RANGE] = {0};
    struct stack_node *dictionary = NULL;
    struct rb_node *completeDictionary = T_Nil;
    struct rb_node *tmp = T_Nil;
    int remainingCompatible;
    int totalWord;
    bool firstWord = false;
    bool gameStarted = false;

    if(!scanf("%d\n", &wordLength))
        printf("Errore lettura lunghezza parole");

    ref = (char*) malloc((wordLength+1) * sizeof(char));
    orderedRef =  calloc(wordLength+1, sizeof(char));
    originalIndexRef = (int*) malloc((wordLength+1) * sizeof(int));

    orderedWord = calloc(wordLength+1, sizeof(char));
    originalIndexWord = (int*) malloc((wordLength+1) * sizeof(int));

    possiblePosition = malloc(wordLength * sizeof(bool[ASCII_RANGE]));
    correctPosition = malloc(wordLength + 1 * sizeof(char));

    totalWord = 0;


    if(wordLength < MAX_COMMAND_SIZE)
        maxLength = MAX_COMMAND_SIZE;
    else
        maxLength = wordLength;

    buffer = (char*) malloc((maxLength+2) * sizeof(char));

    end = customRead(buffer, maxLength);

    while(buffer[0] != '+' && end != NULL)
    {
        tmp = rbInsert(&completeDictionary, buffer, wordLength);
        totalWord++;

        end = customRead(buffer, maxLength);
    }


    while(end != NULL)
    {
        if(buffer[0] == COMMAND)
        {
            if(strncmp(buffer, NEW_GAME, 2) == 0)
            {
                setupNewGame(end, buffer, ref, &maxWord, maxLength, orderedRef, originalIndexRef, wordLength, countRef, &dictionary);
                remainingCompatible = 0;
                firstWord = true;
                gameStarted = true;
            }
            else if(buffer[1] == START_INSERT)
            {
                end = customRead(buffer, maxLength);

                while(strncmp(buffer, END_INSERT, 2)  != 0)
                {
                    tmp = rbInsert(&completeDictionary, buffer, wordLength);
                    totalWord++;

                    if(!firstWord && gameStarted && isCompatible(buffer, wordLength))
                    {
                        push(&dictionary, tmp->word);
                        remainingCompatible++;
                    }

                    end = customRead(buffer, maxLength);
                }
            }
            else if(buffer[1] == REMAINING)
            {
                if(dictionary == NULL)
                    rbInorderVisit(completeDictionary);
                else
                    printList(&dictionary);
            }
        }
        else
        {
            if(strcmp(buffer, ref) == 0)
            {
                printf("ok\n");
                clearList(&dictionary);
                gameStarted = false;
            }
            else if(searchDictionary(completeDictionary, buffer))
            {
                countingWord(buffer, orderedWord, wordLength, originalIndexWord, countWord);
                checkWord(wordLength, orderedWord, orderedRef, originalIndexWord, originalIndexRef, countWord, countRef);

                if(firstWord)
                {
                    listInsertCompatible(completeDictionary, &dictionary, wordLength, &remainingCompatible);
                    firstWord = false;
                }
                else
                    filterCompatible(&dictionary, wordLength, &remainingCompatible);

                maxWord--;

                printf("%d\n", remainingCompatible);

                if(maxWord == 0)
                {
                    printf("ko\n");
                    clearList(&dictionary);
                    gameStarted = false;
                }
            }
            else
                printf("not_exists\n");

        }

        end = customRead(buffer, maxLength);
    }


    rbClearTree(completeDictionary);
    free(ref);
    free(orderedRef);
    free(originalIndexRef);
    free(orderedWord);
    free(originalIndexWord);
    free(possiblePosition);
    free(correctPosition);
    free(buffer);

    return 0;
}