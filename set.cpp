// FILE: set.cxx
// CLASS IMPLEMENTED: Set (see set.h for documentation)
// INVARIANT for the Set ADT:
//   1. The items of the set are stored in a B-tree, satisfying the six
//      B-tree rules.
//   2. The number of entries in the tree's root is in the member variable
//      data_count, and the number of subtrees of the root is stored in the
//      member variable child_count.
//   3. The root's entries are stored in data[0] through data[data_count-1].
//   4. If the root has subtrees, then these subtrees are stored in the Sets
//      *subset[0] through *subset[child_count-1].
//
// FOR REFERENCE, THE SIX B-tree Rules:
//   B-tree Rule 1: Unless the whole set is empty, the root has at least one
//                  entry; every other node has at least MINIMUM entries.
//   B-tree Rule 2: The maximum number of allowed entries in a node is
//                  MAXIMUM (which is equal to 2*MINIMUM).
//   B-tree Rule 3: The entries of each node are sorted from the smallest entry
//                  (at data[0]) to the largest entry (at data[data_count-1]).
//   B-tree Rule 4: The number of subtrees below a non-leaf node is always one
//                  more than the number of entries in the node.
//   B-tree Rule 5: For any non-leaf node: (a) data[i] is greater than all the
//                  entries in subset[i], and (b) data[i] is less than all the
//                  entries in subset[i+1].
//   B-tree Rule 6: All leaves are at the same depth.

//#ifdef SET_H

#include <iostream>
#include <iomanip>
#include <cstdlib> // Provides size_t
#include "set.h"
#include "items.h"  // Has first_ge, merge, split, various *_item functions

Set::Set( )
{
	data_count = 0;
	child_count = 0;
}

Set::Set(const Set& source)
{
	size_t i;

	data_count = source.data_count;
	for (i = 0; i < data_count; i++)
		data[i] = source.data[i];
	child_count = source.child_count;
	for (i = 0; i < child_count; i++)
		subset[i] = new Set(*(source.subset[i]));
}

void Set::operator =(const Set& source)
{
	size_t i;

	if (this != &source)
	{
		clear( );
		data_count = source.data_count;
		for (i = 0; i < data_count; i++)
			data[i] = source.data[i];
		child_count = source.child_count;
		for (i = 0; i < child_count; i++)
			subset[i] = new Set(*(source.subset[i]));
	}
}

void Set::clear( )
{
	size_t i;

	for (i = 0; i < child_count; i++)
		delete subset[i];
	child_count = 0;
	data_count = 0;
}    

void Set::insert(const Item& entry)
{
	size_t i;
	Set* grow_ptr;

	loose_insert(entry);
	if (data_count > MAXIMUM)
	{
		// Split the root node into two

		grow_ptr = new Set;
		grow_ptr->data_count = data_count;
		for (i = 0; i < data_count; i++)
			grow_ptr->data[i] = data[i];
		grow_ptr->child_count = child_count;
		for (i = 0; i < child_count; i++)
			grow_ptr->subset[i] = subset[i];
		data_count = 0;
		child_count = 1;
		subset[0] = grow_ptr;
		fix_excess(0);
	}
}

void Set::remove(const Item& target)
{
	size_t i;
	Set* shrink_ptr;

	loose_remove(target);
	if (child_count == 1)
	{   // Remove the root with zero entries and one child
		shrink_ptr = subset[0];
		data_count = shrink_ptr->data_count;
		for (i = 0; i < data_count; i++)
			data[i] = shrink_ptr->data[i];
		child_count = shrink_ptr->child_count;
		for (i = 0; i < child_count; i++)
			subset[i] = shrink_ptr->subset[i];
		shrink_ptr->child_count = 0;
		delete shrink_ptr;
	}
}

bool Set::contains(const Item& target) const
{
	size_t i;
	bool found;

	i = first_ge(data, data_count, target);
	found = (i < data_count) && (target == data[i]);

	if (found)
		return true;
	else if (is_leaf( ))
		return false;
	else
		return subset[i]->contains(target);
}

void Set::print(int indent) const
{
	size_t i;
	if (child_count > 0)
		subset[child_count-1]->print(indent+4);
	for (i = data_count; i > 0; i--)
	{
		std::cout << std::setw(indent) << "" << data[i-1] << std::endl;
		if (child_count > 0)
			subset[i-1]->print(indent+4);
	}
}

// HELPER FUNCTIONS 
// The helper functions are below with precondition/postcondition contracts.

void Set::loose_insert(const Item& entry)
// Precondition:
//   The entire B-tree is valid.
// Postcondition:
//   If entry was already in the set, then the set is unchanged. Otherwise,
//   entry has been added to the set, and the entire B-tree is still valid
//   EXCEPT that the number of entries in the root of this set might be one
//   more than the allowed maximum.
{
	size_t i;
	bool found;

	i = first_ge(data, data_count, entry);
	found = (i < data_count) && (entry == data[i]);

	if ((!found) && is_leaf( ))
		insert_item(data, i, data_count, entry);
	else if (!found)
	{
		subset[i]->loose_insert(entry);
		if (subset[i]->data_count > MAXIMUM)
			fix_excess(i);
	}
}

void Set::loose_remove(const Item& target)
// Precondition:
//   The entire B-tree is valid.
// Postcondition:
//   If target was in the set, then it has been removed from the set; otherwise
//   the set is unchanged. The entire B-tree is still valid EXCEPT that the
//   number of entries in the root of this set might be one less than the
//   allowed minimum.
{
	size_t i;
	Item temp_entry;
	bool found;

	i = first_ge(data, data_count, target);
	found = (i < data_count) && (data[i] == target);

	if (is_leaf( ) && found)
		delete_item(data, i, data_count, temp_entry);
	else if (!is_leaf( ))
	{
		if (found)
			subset[i]->remove_biggest(data[i]);
		else
			subset[i]->loose_remove(target);
		if (subset[i]->data_count < MINIMUM)
			fix_shortage(i);
	}
}

void Set::remove_biggest(Item& removed_entry)
// Precondition: 
//   (data_count > 0) and the entire B-tree is valid.
// Postcondition:
//   The largest item in the set has been removed, and removed_entry has been
//   set equal to a copy of this removed item. The B-tree is still valid EXCEPT
//   that the number of entries in the root of this set might be one less than
//   the allowed minimum.
{
	size_t i;

	if (is_leaf( ))
		detach_item(data, data_count, removed_entry);
	else
	{   // Continue search for biggest item down the rightmost branch
		i = child_count-1;
		subset[i]->remove_biggest(removed_entry);
		if (subset[i]->data_count < MINIMUM)
			fix_shortage(i);
	}
}

void Set::fix_excess(size_t i)
// Precondition: 
//   (i < child_count) and the entire B-tree is valid EXCEPT that
//   subset[i] has MAXIMUM + 1 entries. Also, the root is allowed to have
//   zero entries and one child.
// Postcondition: 
//   The tree has been rearranged so that the entire B-tree is valid EXCEPT
//   that the number of entries in the root of this set might be one more than
//   the allowed maximum.
{
	Item temp_entry;

	// Insert a new subset at subset[i+1], and split subset[i]'s subsets
	insert_item(subset, i+1, child_count, new Set);
	split(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);

	// Split subset[i]'s data
	split(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);

	// Move the median entry from subset[i] up to data[i].
	detach_item(subset[i]->data, subset[i]->data_count, temp_entry);
	insert_item(data, i, data_count, temp_entry);
}

void Set::fix_shortage(size_t i)
// Precondition: 
//   (i < child_count) and the entire B-tree is valid EXCEPT that
//   subset[i] has only MINIMUM - 1 entries.
// Postcondition: 
//   The tree has been rearranged so that the entire B-tree is valid EXCEPT
//   that the number of entries in the root of this set might be one less than
//   the allowed minimum.
{
	if ((i > 0) && (subset[i-1]->data_count > MINIMUM))
		transfer_right(i-1);
	else if ((i+1 < child_count) && (subset[i+1]->data_count > MINIMUM))
		transfer_left(i+1);
	else if (i+1 < child_count)
		merge_with_next_subset(i);   // Merge subset[i] with subset[i+1]
	else
		merge_with_next_subset(--i); // Merge subset[i-1] with subset[i]
}      

void Set::merge_with_next_subset(size_t i)
// Precondition: 
//   (i+1 < child_count) and the entire B-tree is valid EXCEPT that the total
//   number of entries in subset[i] and subset[i+1] is 2*MINIMUM - 1.
// Postcondition: 
//   subset[i] and subset[i+1] have been merged into one subset (now at
//   subset[i]), and data[i] has been passed down to be the median entry of the
//   new subset[i]. As a result, the entire B-tree is valid EXCEPT that the
//   number of entries in the root of this set might be one less than the
//   allowed minimum.
{
	Item temp_entry;
	Set* temp_ptr;

	// Shift an entry from data[i] down to end of subset[i]
	delete_item(data, i, data_count, temp_entry);
	attach_item(subset[i]->data, subset[i]->data_count, temp_entry);

	// Shift all entries from subset i+1 to right end of subset i;
	merge(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);

	// Shift all subsets of subset i+1 to right end of subset i;
	merge(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);

	// Get rid of subset i+1 in my own subset array
	delete_item(subset, i+1, child_count, temp_ptr);
	delete temp_ptr;
}

void Set::transfer_right(size_t i)
// Precondition: 
//   (i+1 < child_count) and (subset[i]->data_count > MINIMUM)
//   and the entire B-tree is valid EXCEPT that
//   subset[i] has only MINIMUM - 1 entries.
// Postcondition: One entry has been shifted from the end of subset[i] up to
//   data[i], and the original data[i] has been shifted down to the first entry
//   of subset[i+1]. Also, if subset[i] is not a leaf, then its last subset has
//   been transfered over to be the first subset of subset[i+1].
//   As a result, the entire B-tree is now valid.
{
	// If necessary, shift last subset of subset[i] to front of subset[i+1]
	if (!subset[i]->is_leaf( ))
	{
		Set* grandchild;
		detach_item(subset[i]->subset, subset[i]->child_count, grandchild);
		insert_item(subset[i+1]->subset, (unsigned int)0, subset[i+1]->child_count, grandchild);
	}

	// Copy an entry from data[i] down to front of subset[i+1]
	insert_item(subset[i+1]->data, (unsigned int)0, subset[i+1]->data_count, data[i]);

	// Transfer last entry of subset[i] up to replace data[i]
	detach_item(subset[i]->data, subset[i]->data_count, data[i]);
}

void Set::transfer_left(size_t i)
// Precondition: 
//   (0 < i < child_count) and (subset[i]->data_count > MINIMUM)
//   and the entire B-tree is valid EXCEPT that
//   subset[i-1] has only MINIMUM - 1 entries.
// Postcondition:
//   One entry has been shifted from the front of subset[i] up to
//   data[i-1], and the original data[i-1] has been shifted down to the last
//   entry of subset[i-1]. Also, if subset[i] is not a leaf, then its first
//   subset has been transfered over to be the last subset of subset[i-1].
//   As a result, the entire B-tree is now valid.
{
	// If necessary, shift first subset of subset[i] to end of subset[i-1]
	if (!subset[i]->is_leaf( ))
	{
		Set* grandchild;
		delete_item(subset[i]->subset, 0, subset[i]->child_count, grandchild);
		attach_item(subset[i-1]->subset, subset[i-1]->child_count, grandchild);
	}

	// Copy an entry from data[i] down to end of subset[i-1]
	attach_item(subset[i-1]->data, subset[i-1]->data_count, data[i-1]);

	// Transfer first entry of subset[i] up to replace data[i-1]
	delete_item(subset[i]->data, 0, subset[i]->data_count, data[i-1]);
}

//#endif

