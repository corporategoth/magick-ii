
/* Magick IRC Services
**
** (c) 1997-2003 Preston Elder <prez@magick.tm>
** (c) 1998-2003 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#ifndef _ACE_MEMORY_H
#define _ACE_MEMORY_H
//RCSID(ace_memory_h, "@(#) $Id$");

/* ======================================================================= **
**
** For official changes (by the Magick Development Team),please
** check the ChangeLog* files that come with this distribution.
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** ======================================================================= */

/* This is written by Preston Elder, and is included
 * only until it is intergrated into ACE propper
 */

template < class T, class ACE_LOCK > class ACE_Expandable_Cached_Allocator : public ACE_New_Allocator
{
    // = TITLE
    //   Create a cached memory poll with <n_chunks> chunks each with
    //   sizeof (TYPE) size.  It will automatically expand when it
    //   runs out of room allocating <n_chunks> more elements.
    //
    // = DESCRIPTION
    //   This class enables caching of dynamically allocated,
    //   fixed-sized classes.
    //   Note, this class does NOT free up memory segments when
    //   they become unused (but it does when the class is destructed).
public:
    ACE_Expandable_Cached_Allocator(size_t n_chunks);
    // Create a cached memory poll with <n_chunks> chunks
    // each with sizeof (TYPE) size.

    ~ACE_Expandable_Cached_Allocator(void);

    // clear things up.

    void *malloc(size_t);

    // get a chunk of memory from free store.

    void free(void *);

    // return a chunk of memory back to free store.

    size_t chunks() const
    {
	return pool_.size();
    }
    size_t used() const
    {
	return (pool_.size() * n_chunks_) - free_list_.size();
    }

private:
    size_t n_chunks_;

    // how many chunks per segment.

    ACE_Locked_Free_List < ACE_Cached_Mem_Pool_Node < T >, ACE_LOCK > free_list_;
    // Maintain a cached memory free list.

    ACE_Array_Base < char * > pool_;

    // remember how we allocate the memory in the first place so
    // we can clear things up later.
};

template < class T, class ACE_LOCK > ACE_INLINE void ACE_Expandable_Cached_Allocator < T, ACE_LOCK >::free(void *ptr)
{
    ACE_OS::memset(ptr, 0, sizeof(T));
    this->free_list_.add(ptr);
}

template < class T, class ACE_LOCK > ACE_Expandable_Cached_Allocator < T,
    ACE_LOCK >::ACE_Expandable_Cached_Allocator(size_t n_chunks) : n_chunks_(n_chunks), free_list_(ACE_PURE_FREE_LIST),
pool_(1)
{
    char *temp;
    ACE_NEW(temp, char [n_chunks_ * sizeof(T)]);

    if (pool_.set(temp, 0) < 0)
    {
	delete[] temp;
	return;
    }

    ACE_OS::memset(temp, 0, n_chunks_ * sizeof(T));
    for (size_t c = 0; c < n_chunks_; c++)
    {
	void *placement = temp + c * sizeof(T);

	this->free_list_.add(placement);
    }
    // Put into free list using placement contructor, no real memory
    // allocation in the above <new>.
}

template < class T, class ACE_LOCK > ACE_Expandable_Cached_Allocator < T, ACE_LOCK >::~ACE_Expandable_Cached_Allocator(void)
{
    for (size_t c = 0; c < pool_.size(); c++)
	delete[] this->pool_[c];
}

template < class T, class ACE_LOCK > void *ACE_Expandable_Cached_Allocator < T, ACE_LOCK >::malloc(size_t nbytes)
{
    // Check if size requested fits within pre-determined size.
    if (nbytes > sizeof(T))
	return NULL;

    // Heres where we expand the buffer if needed ...
    if (!this->free_list_.size())
    {
	char *temp = NULL;
	ACE_NEW_RETURN(temp, char [n_chunks_ * sizeof(T)], 0);

	pool_.max_size(pool_.max_size() + 1);
	if (pool_.set(temp, pool_.size() - 1) < 0)
	{
	    delete[] temp;
	    return NULL;
	}
	// We could not add to the map ...
	ACE_OS::memset(temp, 0, n_chunks_ * sizeof(T));

	for (size_t c = 0; c < n_chunks_; c++)
	{
	    void *placement = temp + c * sizeof(T);

	    this->free_list_.add(placement);
	}
	// Put into free list using placement contructor, no real memory
	// allocation in the above <new>.
    }

    // addr() call is really not absolutely necessary because of the way
    // ACE_Cached_Mem_Pool_Node's internal structure arranged.
    return this->free_list_.remove()->addr();
}

template < class ACE_LOCK > class ACE_Cached_Fixed_Allocator : public ACE_New_Allocator
{
    // = TITLE
    //   Create a cached memory poll with <n_chunks> chunks each with
    //   <n_size> size.
    //
    // = DESCRIPTION
    //   This class enables caching of dynamically allocated,
    //   fixed-sized classes.
public:
    ACE_Cached_Fixed_Allocator(size_t n_size, size_t n_chunks);

    ~ACE_Cached_Fixed_Allocator(void);

    // clear things up.

    void *malloc(size_t);

    // get a chunk of memory from free store.

    void free(void *);

    // return a chunk of memory back to free store.

    size_t size() const;

    // Total size (amount of nodes available)

    size_t nodesize() const;

    // Size of each node (same as specified)

    size_t chunksize() const;

    // Amount of nodes per allocated chunk

    size_t count() const;

    // Amount of used chunks so far

private:
    size_t n_size_;
    // Size of each element

    size_t n_chunks_;

    // How many chunks per segment

    ACE_Locked_Free_List < ACE_Cached_Mem_Pool_Node < void * >, ACE_LOCK > free_list_;

    // Maintain a cached memory free list.

    char *pool_;

    // remember how we allocate the memory in the first place so
    // we can clear things up later.
};

template < class ACE_LOCK > ACE_INLINE void *ACE_Cached_Fixed_Allocator < ACE_LOCK >::malloc(size_t nbytes)
{
    // Check if size requested fits within pre-determined size.
    if (nbytes > n_size_)
	return NULL;

    // addr() call is really not absolutely necessary because of the way
    // ACE_Cached_Mem_Pool_Node's internal structure arranged.
    return this->free_list_.remove()->addr();
}

template < class ACE_LOCK > ACE_INLINE void ACE_Cached_Fixed_Allocator < ACE_LOCK >::free(void *ptr)
{
    ACE_OS::memset(ptr, 0, n_size_);
    this->free_list_.add(ptr);
}

template < class ACE_LOCK > ACE_INLINE size_t ACE_Cached_Fixed_Allocator < ACE_LOCK >::size() const
{
    return n_chunks_;
}

template < class ACE_LOCK > ACE_INLINE size_t ACE_Cached_Fixed_Allocator < ACE_LOCK >::nodesize() const
{
    return n_size_;
}

template < class ACE_LOCK > ACE_INLINE size_t ACE_Cached_Fixed_Allocator < ACE_LOCK >::chunksize() const
{
    return n_chunks_;
}

template < class ACE_LOCK > ACE_INLINE size_t ACE_Cached_Fixed_Allocator < ACE_LOCK >::count() const
{
    return size() - free_list_.size();
}

template < class ACE_LOCK > ACE_Cached_Fixed_Allocator < ACE_LOCK >::ACE_Cached_Fixed_Allocator(size_t n_size, size_t n_chunks):
n_size_(n_size), n_chunks_(n_chunks), free_list_(ACE_PURE_FREE_LIST), pool_(0)
{
    ACE_NEW(pool_, char [n_chunks_ * n_size_]);

    ACE_OS::memset(n_chunks_ * n_size_);
    for (size_t c = 0; c < n_chunks_; c++)
    {
	void *placement = pool_ + c * n_size_;

	this->free_list_.add(placement);
    }
    // Put into free list using placement contructor, no real memory
    // allocation in the above <new>.
}

template < class ACE_LOCK > ACE_Cached_Fixed_Allocator < ACE_LOCK >::~ACE_Cached_Fixed_Allocator(void)
{
    delete[] pool_;
}

template < class ACE_LOCK > class ACE_Expandable_Cached_Fixed_Allocator : public ACE_New_Allocator
{
    // = TITLE
    //   Create a cached memory poll with <n_chunks> chunks each with
    //   <n_size> size.  It will automatically expand when it
    //   runs out of room allocating <n_chunks> more elements.
    //
    // = DESCRIPTION
    //   This class enables caching of dynamically allocated,
    //   fixed-sized classes.
    //   Note, this class does NOT free up memory segments when
    //   they become unused (but it does when the class is destructed).
public:
    ACE_Expandable_Cached_Fixed_Allocator(size_t n_size, size_t n_chunks);

    ~ACE_Expandable_Cached_Fixed_Allocator(void);

    // clear things up.

    void *malloc(size_t);

    // get a chunk of memory from free store.

    void free(void *);

    // return a chunk of memory back to free store.

    size_t size() const;

    // Total size (amount of nodes available)

    size_t nodesize() const;

    // Size of each node (same as specified)

    size_t chunksize() const;

    // Amount of nodes per allocated chunk

    size_t count() const;

    // Amount of used chunks so far

private:
    size_t n_size_;
    // Size of each element

    size_t n_chunks_;

    // How many chunks per segment

    ACE_Locked_Free_List < ACE_Cached_Mem_Pool_Node < void * >, ACE_LOCK > free_list_;

    // Maintain a cached memory free list.

    ACE_Array_Base < char * > pool_;

    // remember how we allocate the memory in the first place so
    // we can clear things up later.
};

template < class ACE_LOCK > ACE_INLINE void ACE_Expandable_Cached_Fixed_Allocator < ACE_LOCK >::free(void *ptr)
{
    ACE_OS::memset(ptr, 0, n_size_);
    this->free_list_.add(static_cast < ACE_Cached_Mem_Pool_Node < void * > * > (ptr));
}

template < class ACE_LOCK > ACE_INLINE size_t ACE_Expandable_Cached_Fixed_Allocator < ACE_LOCK >::size() const
{
    return pool_.size() * n_chunks_;
}

template < class ACE_LOCK > ACE_INLINE size_t ACE_Expandable_Cached_Fixed_Allocator < ACE_LOCK >::nodesize() const
{
    return n_size_;
}

template < class ACE_LOCK > ACE_INLINE size_t ACE_Expandable_Cached_Fixed_Allocator < ACE_LOCK >::chunksize() const
{
    return n_chunks_;
}

template < class ACE_LOCK > ACE_INLINE size_t ACE_Expandable_Cached_Fixed_Allocator < ACE_LOCK >::count() const
{
    return size() - free_list_.size();
}

template < class ACE_LOCK > ACE_Expandable_Cached_Fixed_Allocator <
    ACE_LOCK >::ACE_Expandable_Cached_Fixed_Allocator(size_t n_size, size_t n_chunks) : n_size_(n_size), n_chunks_(n_chunks),
free_list_(ACE_PURE_FREE_LIST), pool_(1)
{
    char *temp;
    ACE_NEW(temp, char [n_chunks_ * n_size_]);

    if (pool_.set(temp, 0) < 0)
    {
	delete[] temp;
	return;
    }

    ACE_OS::memset(temp, 0, n_chunks_ * n_size_);
    for (size_t c = 0; c < n_chunks_; c++)
    {
	void *placement = temp + c * n_size_;
	this->free_list_.add(static_cast < ACE_Cached_Mem_Pool_Node < void * > * > (placement));
    }
    // Put into free list using placement contructor, no real memory
    // allocation in the above <new>.
}

template < class ACE_LOCK > ACE_Expandable_Cached_Fixed_Allocator < ACE_LOCK >::~ACE_Expandable_Cached_Fixed_Allocator(void)
{
    for (size_t c = 0; c < pool_.size(); c++)
	delete[] this->pool_[c];
}

template < class ACE_LOCK > void *ACE_Expandable_Cached_Fixed_Allocator < ACE_LOCK >::malloc(size_t nbytes)
{
    // Check if size requested fits within pre-determined size.
    if (nbytes > n_size_)
	return NULL;

    // Heres where we expand the buffer if needed ...
    if (!this->free_list_.size())
    {
	char *temp = NULL;
	ACE_NEW_RETURN(temp, char [n_chunks_ * n_size_], 0);

	pool_.max_size(pool_.max_size() + 1);
	if (pool_.set(temp, pool_.size() - 1) < 0)
	{
	    delete[] temp;
	    return NULL;
	}
	// We could not add to the map ...

	ACE_OS::memset(temp, 0, n_chunks_ * n_size_);
	for (size_t c = 0; c < n_chunks_; c++)
	{
	    void *placement = temp + c * n_size_;
	    this->free_list_.add(static_cast < ACE_Cached_Mem_Pool_Node < void * > * > (placement));
	}
	// Put into free list using placement contructor, no real memory
	// allocation in the above <new>.
    }

    // addr() call is really not absolutely necessary because of the way
    // ACE_Cached_Mem_Pool_Node's internal structure arranged.
    return this->free_list_.remove()->addr();
}

#endif
