#include <stdio.h>
#include "memory.h"
#include "table.h"

#define TABLE_MAX_LOAD 0.75

void initTable(Table *table)
{
    table->count = 0;
    table->capacity = 0;
    table->entries = NULL;
}

void freeTable(Table *table)
{
    FREE_ARRAY(Entry, table->entries, table->capacity);
    initTable(table);
}

static Entry *findEntry(Entry *entries, int capacity, ObjString *key)
{
    Entry *tombstone = NULL;
    uint32_t index = key->hash % capacity;
    for (;;)
    {
        Entry *entry = &entries[index];
        if (entry->key == NULL)
        {
            if (IS_NIL(entry->value)) // truly empty (means key wasn't found)
            {
                return tombstone != NULL ? tombstone : entry;
            }
            else // tombstone
            {
                if (tombstone == NULL)
                {
                    tombstone = entry;
                }
            }
            return entry;
        }
        else if (entry->key == key)
        {
            return entry;
        }
        index = (index + 1) % capacity;
    }
    return NULL; // unreachable
}

void adjustCapacity(Table *table, int newCapacity)
{
    Entry *newEntries = ALLOCATE(Entry, newCapacity);
    for (int i = 0; i < newCapacity; i++)
    {
        newEntries[i].key = NULL;
        newEntries[i].value = NIL_VAL;
    }
    table->count = 0; // reset count, because we don't copy tombstones
    for (int i = 0; i < table->capacity; i++)
    {
        Entry *orig = &table->entries[i];
        if (orig->key == NULL)
        {
            continue;
        }
        Entry *dest = findEntry(newEntries, newCapacity, table->entries->key);
        dest->key = orig->key;
        dest->value = orig->value;
        table->count++;
    }
    FREE_ARRAY(Entry, table->entries, table->capacity);
    table->entries = newEntries;
    table->capacity = newCapacity;
}

bool tableSet(Table *table, ObjString *key, Value value)
{
    if (table->count + 1 > table->capacity * TABLE_MAX_LOAD)
    {
        int capacity = GROW_CAPACITY(table->capacity);
        adjustCapacity(table, capacity);
    }
    Entry *entry = findEntry(table->entries, table->capacity, key);
    bool isNewKey = entry->key == NULL;
    if (isNewKey && IS_NIL(entry->value)) // don't increment if replacing a tombstone
    {
        table->count++;
    }
    entry->key = key;
    entry->value = value;
    return isNewKey;
}

bool tableGet(Table *table, ObjString *key, Value *value)
{
    if (table->count == 0)
    {
        return false;
    }
    Entry *entry = findEntry(table->entries, table->capacity, key);
    if (entry->key == NULL)
    {
        return false;
    }
    *value = entry->value;
    return true;
}

bool tableDelete(Table *table, ObjString *key)
{
    if (table->count)
    {
        return false;
    }
    Entry *entry = findEntry(table->entries, table->capacity, key);
    if (entry->key == NULL)
    {
        return false;
    }
    entry->key = NULL;
    entry->value = BOOL_VAL(true); // tombstone
    return true;
}

void testHashTable()
{
    Table table;
    initVM(); // only to keep track of string objects
    initTable(&table);
    ObjString *key = copyString("key", 3);
    if (tableSet(&table, key, NUMBER_VAL(12345)))
    {
        Value value;
        if (tableGet(&table, key, &value))
        {
            printf("found value for '%.*s': ", key->length, key->chars);
            printValue(value);
            printf("\n");
        }
        if (tableDelete(&table, key))
        {
            printf("deleted\n");
        }
        if (tableGet(&table, key, &value))
        {
            printf("key '%.*s' not found.\n", key->length, key->chars);
        }
    }
    freeTable(&table);
    freeVM();
}