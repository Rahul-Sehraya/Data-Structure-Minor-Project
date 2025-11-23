#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUFFIX_LEN  20
#define MAX_TYPE_LEN    80
#define MAX_WORD_LEN    100

typedef struct SuffixNode {
    char suffix[MAX_SUFFIX_LEN];      // The suffix string
    char category[MAX_TYPE_LEN];      // Description / grammatical category
    struct SuffixNode *next;
} SuffixNode;

// Function declaration
SuffixNode* createNode(const char *suffix, const char *category);
void insertSuffix(SuffixNode **head, const char *suffix, const char *category);
void displaySuffixes(SuffixNode *head);
void freeList(SuffixNode *head);
int hasSuffix(const char *word, const char *suffix);
SuffixNode* findMatchingSuffix(SuffixNode *head, const char *word);
void analyzeWord(SuffixNode *head, const char *word);

// Create a new node
SuffixNode* createNode(const char *suffix, const char *category) {
    SuffixNode *newNode = (SuffixNode*)malloc(sizeof(SuffixNode));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    strncpy(newNode->suffix, suffix, MAX_SUFFIX_LEN - 1);
    newNode->suffix[MAX_SUFFIX_LEN - 1] = '\0';

    strncpy(newNode->category, category, MAX_TYPE_LEN - 1);
    newNode->category[MAX_TYPE_LEN - 1] = '\0';

    newNode->next = NULL;
    return newNode;
}

// Insert at end of linked list
void insertSuffix(SuffixNode **head, const char *suffix, const char *category) {
    SuffixNode *newNode = createNode(suffix, category);

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    SuffixNode *temp = *head;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = newNode;
}

// Suffix display function
void displaySuffixes(SuffixNode *head) {
    if (head == NULL) {
        printf("No suffixes in the list.\n");
        return;
    }

    printf("\nCurrent Suffix List:\n");
    printf("--------------------\n");
    int index = 1;
    while (head != NULL) {
        printf("%2d. %-10s -> %s\n", index, head->suffix, head->category);
        head = head->next;
        index++;
    }
    printf("\n");
}

// Free linked list / Memory
void freeList(SuffixNode *head) {
    SuffixNode *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Check if `suffix` is actually a suffix of `word`
int hasSuffix(const char *word, const char *suffix) {
    int lw = (int)strlen(word);
    int ls = (int)strlen(suffix);

    if (ls > lw) return 0;

    // Compare the end of `word` with `suffix`
    return strcmp(word + (lw - ls), suffix) == 0;
}

// Find the longest matching suffix for a given word
SuffixNode* findMatchingSuffix(SuffixNode *head, const char *word) {
    SuffixNode *bestMatch = NULL;
    int bestLen = 0;

    while (head != NULL) {
        if (hasSuffix(word, head->suffix)) {
            int currentLen = (int)strlen(head->suffix);
            // Prefer the longest matching suffix
            if (currentLen > bestLen) {
                bestLen = currentLen;
                bestMatch = head;
            }
        }
        head = head->next;
    }

    return bestMatch;
}

// Analyze a word: split into stem + suffix and classify
void analyzeWord(SuffixNode *head, const char *word) {
    if (word == NULL || strlen(word) == 0) {
        printf("Empty word.\n");
        return;
    }

    SuffixNode *match = findMatchingSuffix(head, word);

    if (match == NULL) {
        printf("Word: %s\n", word);
        printf("No known suffix found in the list.\n\n");
        return;
    }

    int wordLen = (int)strlen(word);
    int sufLen  = (int)strlen(match->suffix);
    int stemLen = wordLen - sufLen;

    char stem[MAX_WORD_LEN];

    if (stemLen <= 0) {
        strcpy(stem, "(no stem)");
    } else {
        strncpy(stem, word, stemLen);
        stem[stemLen] = '\0';
    }

    printf("Word           : %s\n", word);
    printf("Identified stem: %s\n", stem);
    printf("Identified suffix: %s\n", match->suffix);
    printf("Category       : %s\n\n", match->category);
}

int main() {
    SuffixNode *head = NULL;

    // Pre-added common Sanskrit suffixes (in simple transliteration)
    insertSuffix(&head, "ah",   "Nominative singular masculine (e.g. 'Ramah')");
    insertSuffix(&head, "am",   "Accusative singular / neuter nominative");
    insertSuffix(&head, "ena",  "Instrumental singular (e.g. 'ramena')");
    insertSuffix(&head, "asya", "Genitive singular (e.g. 'devasya')");
    insertSuffix(&head, "e",    "Locative singular / vocative variation");
    insertSuffix(&head, "esu",  "Locative plural (e.g. 'vanesu')");
    insertSuffix(&head, "bhih", "Instrumental plural");
    insertSuffix(&head, "su",   "Locative plural (alternative)");
    insertSuffix(&head, "tva",  "Abstract noun forming suffix (e.g. 'satyatva')");
    insertSuffix(&head, "ka",   "Diminutive / derivative suffix");
    insertSuffix(&head, "ta",   "Past participle / abstract noun suffix");
    insertSuffix(&head, "yah",  "Future passive participle or derivative ending");

    int choice;
    char word[MAX_WORD_LEN];
    char suffixInput[MAX_SUFFIX_LEN];
    char categoryInput[MAX_TYPE_LEN];

    while (1) {
        printf("===================================\n");
        printf(" Sanskrit Suffix Identifier\n");
        printf("===================================\n");
        printf("1. Show all known suffixes\n");
        printf("2. Analyze a word\n");
        printf("3. Add a new suffix\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            // Clear invalid input
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Try again.\n\n");
            continue;
        }

        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        switch (choice) {
            case 1:
                displaySuffixes(head);
                break;

            case 2:
                printf("Enter a Sanskrit word (transliterated, no spaces): ");
                if (fgets(word, sizeof(word), stdin) == NULL) {
                    printf("Error reading word.\n\n");
                    break;
                }
                word[strcspn(word, "\n")] = '\0';
                analyzeWord(head, word);
                break;

            case 3:
                printf("Enter new suffix (e.g. 'ena', 'asya'): ");
                if (fgets(suffixInput, sizeof(suffixInput), stdin) == NULL) {
                    printf("Error reading suffix.\n\n");
                    break;
                }
                suffixInput[strcspn(suffixInput, "\n")] = '\0';

                printf("Enter category / description: ");
                if (fgets(categoryInput, sizeof(categoryInput), stdin) == NULL) {
                    printf("Error reading category.\n\n");
                    break;
                }
                categoryInput[strcspn(categoryInput, "\n")] = '\0';

                insertSuffix(&head, suffixInput, categoryInput);
                printf("Suffix added successfully.\n\n");
                break;

            case 4:
                freeList(head);
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid choice. Try again.\n\n");
        }
    }

    return 0;
}
