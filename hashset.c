#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <stdarg.h>

#define MAX_SIZE 17


int current_size = 0;
int deleted_size = 0;

typedef struct set
{
    int hash_table[MAX_SIZE];
    int current_size;
    int deleted_size;
}set;

void print_array(int arr[], int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("%d\n", arr[i]);
    }
    printf("---------><----------\n");
}

bool is_empty(set *s)
{
    return !(bool)current_size;
}

int cardinality(set *s)
{
    return s->current_size;
}

void create(set *s)
{
    for (int i = 0; i < MAX_SIZE; i++)
    {
        s->hash_table[i] = INT_MIN;
    }
    s->current_size = 0;
    s->deleted_size = 0;
}

int hash_value(int key)
{
    return key%MAX_SIZE;
}

bool add(set *s,int key)
{
    int hash = hash_value(key);
    int i = 1;
    //quadratic probing
    if(s->current_size == MAX_SIZE-1|| (s->deleted_size+s->current_size) == MAX_SIZE-1)  return false;
    while (s->hash_table[hash] != INT_MIN && s->hash_table[hash] != INT_MAX)
    {
        if(s->hash_table[hash] == key)
            return true;
        hash = (hash + i*i)%MAX_SIZE;
        i++;
    }
    if(s->hash_table[hash] == INT_MAX)
    {
        s->deleted_size--;
    }
    s->hash_table[hash] = key;
    s->current_size+=1;
 
    return true;
}

bool remove_element(set *s, int key)
{
    int hash = hash_value(key);
    int i = 1;
    if(s->current_size < 1) return false;
    while (s->hash_table[hash] != INT_MIN)
    {
        if(s->hash_table[hash] == key)
        {
            s->hash_table[hash] = INT_MAX;
            s->current_size--;
            s->deleted_size++;
            return true;
        }
        hash = hash + i*i;
        hash = hash%MAX_SIZE;
        i++;
    }
   
    return false;
}

int is_element_of(set *s, int key)
{
    
    int hash = hash_value(key);
    int i = 1;
    while (s->hash_table[hash] != INT_MIN)
    {
        if(s->hash_table[hash] == key)
        {
            return hash;
        }

        hash = (hash + i*i)%MAX_SIZE;
     
        i++;
    }
   
    return -1;
}

int* enumerate_set(set *s)
{
    if(s->current_size == 0)    return NULL;
    int* enumerated_array = (int*) malloc(s->current_size*sizeof(int));
    if(enumerated_array)
    {
        int i = 0, j = 0;
        while (i<MAX_SIZE)
        {
            if(s->hash_table[i] != INT_MIN && s->hash_table[i] != INT_MAX)
            {
                enumerated_array[j] = s->hash_table[i];
                j++;
            }    
            i++;
        }
        
        // print_array(enumerated_array, s->current_size);
        return enumerated_array;
    }
    return NULL;
}

set* build(int num, ...)
{
    set *s = (set*) malloc(sizeof(set));
    create(s);
    va_list list;

    va_start(list, num);

    if(num < MAX_SIZE - 1)
    {
        

        for(int i = 0; i < num; i++)
        {

            int value = va_arg(list, int);
            add(s, value);

        }
        va_end(list);

        return s;
    }
    return NULL;
}

set* union_of_set(set *s1, set *s2)
{
    set *ret_set = (set*) malloc(sizeof(set));
    
    if(ret_set)
    {
        create(ret_set);
        for(int i = 0; i < MAX_SIZE; i++)
        {
            if (s1->hash_table[i] != INT_MAX && s1->hash_table[i] != INT_MIN) 
            {
                add(ret_set, s1->hash_table[i]);
            }
            if (s2->hash_table[i] != INT_MAX && s2->hash_table[i] != INT_MIN) 
            {
                add(ret_set, s2->hash_table[i]);
            }
            
        }
        return ret_set;
    }
    return NULL;
}

set* intersection(set *s1, set *s2)
{
    set *ret_set = (set*) malloc(sizeof(set));
    
    if(ret_set)
    {
        create(ret_set);
        for(int i = 0; i < MAX_SIZE; i++)
        {
            if(s1->hash_table[i] != INT_MAX && s1->hash_table[i] != INT_MIN) 
            {
                if(is_element_of(s2, s1->hash_table[i]) != -1)
                {
                
                    add(ret_set, s1->hash_table[i]);
                }
            }
        }
        return ret_set;
    }
    
    return NULL;
}

set* difference(set *s1, set *s2)
{
    set *ret_set = (set*) malloc(sizeof(set));

    if(ret_set)
    {
        set *union_set = union_of_set(s1, s2);
        
        set *intersection_set = intersection(s1, s2);
        printf("i am here\n");
    
        for(int i = 0; i<INT_MAX; i++)
        {

            if(intersection_set->hash_table[i] != INT_MAX && intersection_set->hash_table[i] != INT_MIN)
            {
                remove_element(union_set, intersection_set->hash_table[i]);
            }
            
        }
        return union_set;
    }
    return NULL;
}

bool subset(set* s, set* t)
{
    // whether s is a subset of t
    for(int i = 0 ; i < MAX_SIZE; i++)
    {
        if(s->hash_table[i] != INT_MAX && s->hash_table[i] != INT_MIN)
        {
            if(is_element_of(t, s->hash_table[i]) == -1)
                return false;
        } 
    }
    return true;
}



int main()
{

    set s1;
    create(&s1);

    set s2;
    create(&s2);

    add(&s1, 1);
    add(&s1, 2);
    add(&s1, 3);
    add(&s1, 4);
    add(&s1, 5);
    add(&s1, 6);
    add(&s1, 7);
    remove_element(&s1, 7);
    remove_element(&s1, 4);
    add(&s2, 3);
    add(&s2, 5);
    add(&s2, 7);
    add(&s2, 11);
    add(&s2, 12);
    add(&s2, 13);

    print_array(enumerate_set(&s1), s1.current_size);
    print_array(enumerate_set(&s2), s2.current_size);
    
    

//     for(int i = 0; i < MAX_SIZE; i++)
//     {
//         printf("%d\n", s1.hash_table[i]);
//     }

//     printf("------><-------\n");

//     for(int i = 0; i< MAX_SIZE; i++)
//     {
//         printf("%d\n", s2.hash_table[i]);
//     }

//     printf("------><--------\n");
//     // printf("this is weird\n");
    
    set* union_set = union_of_set(&s1, &s2);

    print_array(enumerate_set(union_set), union_set->current_size);
//     printf("%d\n%d\n", s1.current_size, s2.current_size);
   
//    printf("----><-------\n");

//    enumerate_set(&s1);

    set* intersection_set = intersection(&s1, &s2);

    print_array(enumerate_set(intersection_set), intersection_set->current_size);

    

    set* difference_set = difference(&s1, &s2);

    
    print_array(enumerate_set(difference_set), difference_set->current_size);
    return 0;
}
