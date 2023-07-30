#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// Node for linked list
struct Node
{
    struct RecordType record;
    struct Node* next;
};

// HashType (separate chaining structure)
struct HashType
{
    struct Node* head;
};

// Compute the hash function
int hash(int x, int hashSz)
{
    return x % hashSz;
}

int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// Function to insert a record into the hash table
void insertRecord(struct HashType* pHashArray, int hashSz, struct RecordType record)
{
    int index = hash(record.id, hashSz);
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        printf("Cannot allocate memory for new record node\n");
        exit(-1);
    }

    newNode->record = record;
    newNode->next = NULL;

    if (pHashArray[index].head == NULL)
    {
        pHashArray[index].head = newNode;
    }
    else
    {
        struct Node* current = pHashArray[index].head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Function to display records in the hash structure
// Skip the indices which are free
// The output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    for (int i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i].head != NULL)
        {
            printf("index %d ->", i);

            struct Node* current = pHashArray[i].head;
            while (current != NULL)
            {
                printf(" %d %c %d ->", current->record.id, current->record.name, current->record.order);
                current = current->next;
            }

            printf(" NULL\n");
        }
    }
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);

    // Assuming the hash size to be recordSz for simplicity
    int hashSz = recordSz;

    // Create the hash table
    struct HashType* pHashArray = (struct HashType*)malloc(sizeof(struct HashType) * hashSz);
    if (pHashArray == NULL)
    {
        printf("Cannot allocate memory for the hash table\n");
        exit(-1);
    }

    // Initialize the hash table
    for (int i = 0; i < hashSz; ++i)
    {
        pHashArray[i].head = NULL;
    }

    // Insert records into the hash table
    for (int i = 0; i < recordSz; ++i)
    {
        insertRecord(pHashArray, hashSz, pRecords[i]);
    }

    // Display records in the hash table
    displayRecordsInHash(pHashArray, hashSz);

    // Free allocated memory
    for (int i = 0; i < hashSz; ++i)
    {
        struct Node* current = pHashArray[i].head;
        while (current != NULL)
        {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(pHashArray);
    free(pRecords);

    return 0;
}
