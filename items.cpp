// FILE: item_tpl.cxx
// IMPLEMENTS: The four template functions of a small toolkit to manipulate
// items (see item_tpl.h). Note that this file is not compiled on its own.
// Instead, this file is included with an include directive at the bottom
// of item_tpl.h.

#ifdef ITEMS_H

#include <assert.h>    // Provides assert
#include <stdlib.h>    // Provides size_t

template <class Item>
Item maximal(Item a, Item b)
{
    if (a > b)
        return a;
    else
    return b;
}

template <class Item>
void swap(Item& x, Item& y)
{
    Item temp;

    temp = x;
    x = y;
    y = temp;
}

template <class Item, class SizeType>
size_t index_of_maximal(Item data[ ], SizeType n)
// Library facilities used: assert.h, stdlib.h
{
    size_t answer;
    size_t i;

    assert(n > 0);
    answer = 0;

    for (i = 1; i < n; i++)
    {
        if (data[answer] < data[i])
        answer = i;
        // data[answer] is now biggest from data[0]..data[i]
    }

    return answer;
}

template <class Item, class SizeType>
void ordered_insert(Item data[ ], SizeType n, Item entry)
// Library functions used: stdlib.h
{
    size_t i;

    for (i = n; (i > 0) && (entry < data[i-1]); i--)
        data[i] = data[i-1];

    data[i] = entry;
}
    
template <class Item, class SizeType>
size_t first_ge(const Item data[ ], SizeType n, const Item& entry)
// Library functions used: stdlib.h
{
    size_t i;

    for (i = 0; (i < n) && !(data[i] >= entry); i++);
  
    return i;
}

template <class Item, class SizeType>
void attach_item(Item data[ ], SizeType& n, const Item& entry)
{
    data[n++] = entry;
}

template <class Item, class IndexType, class SizeType>
void insert_item(Item data[ ], IndexType i, SizeType& n, Item entry)
{
    size_t shift_index;

    for (shift_index = n; shift_index > i; shift_index--)
        data[shift_index] = data[shift_index-1];
    data[i] = entry;
    n++;
}

template <class Item, class SizeType>
void detach_item(Item data[ ], SizeType& n, Item& entry)
{
    entry = data[--n];
}

template <class Item, class IndexType, class SizeType>
void delete_item(Item data[ ], IndexType i, SizeType& n, Item& entry)
{
    size_t shift_index;

    entry = data[i];
    for (shift_index = i+1; shift_index < n; shift_index++)
        data[shift_index-1] = data[shift_index];

    n--;
}

template <class Item, class SizeType>
void merge(Item data1[ ], SizeType& n1, Item data2[ ], SizeType& n2)
{
    size_t i;

    for (i = 0; i < n2; i++)
        data1[n1 + i] = data2[i];
    n1 += n2;
    n2 = 0;
}

template <class Item, class SizeType>
void split(Item data1[ ], SizeType& n1, Item data2[ ], SizeType& n2)
{
    size_t i;
    size_t new_size1;

    new_size1 = n1 - n1/2;
    
    for (i = 0; i < n1/2; i++)
        data2[n2 + i] = data1[new_size1 + i];

    n2 += n1/2;
    n1 = new_size1;
}

#endif

