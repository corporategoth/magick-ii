/*  Magick IRC Services
**
** (c) 1997-2000 Preston Elder <prez@magick.tm>
** (c) 1998-2000 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#ifndef _MMEMORY_H
#define _MMEMORY_H
#include "pch.h"
static const char *ident_mmemory_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.3  2000/10/18 18:46:33  prez
** Well, mstring still coredumps, but it gets past the initial loading of
** all the STATIC (or const) strings, etc -- now its coring on loading a
** file (or possibly language.h or something).  Still investigating.
**
** Revision 1.2  2000/10/15 03:29:27  prez
** Mods to the memory system, LOTS of printf's to try and figure out why
** the damn thing coredumps on init.
**
** Revision 1.1  2000/10/14 04:25:31  prez
** Added mmemory.h -- MemCluster and the MemoryManager are now in it.
** TODO - make mstring use MemoryManager.
**
**
** ========================================================== */

#define DEF_MEMSIZE 8192

class MemoryNode
{
    friend class MemoryBlock;

    void *i_loc;
    bool i_avail;
    size_t i_size;
    MemoryNode *prev, *next;

public:
    MemoryNode(void *loc, size_t size)
    {
	init(loc, size);
    }
    void init(void *loc, size_t size)
    {
	i_loc = loc;
	i_size = size;
	i_avail = true;
	prev = next = NULL;
    }

    bool avail() { return i_avail; }
    size_t size() { return i_size; }
};

class MemoryBlock
{
    char *i_memory;
    size_t i_size;
    size_t i_avail;
    ACE_Expandable_Cached_Allocator<MemoryNode, ACE_Thread_Mutex> i_cluster;
    MemoryNode *i_nodes;

public:
    MemoryBlock *prev, *next;
    MemoryBlock(size_t size)
	: i_cluster(8)
    {
	i_avail = 0;
	i_size = size;
	i_memory = NULL;
	i_nodes = NULL;
	prev = next = NULL;
    }
    void init()
    {
	i_avail = i_size;
	i_memory = new char[i_size];
    }
    ~MemoryBlock()
    {
	MemoryNode *iter = NULL, *next = NULL;
	for (iter = i_nodes; iter != NULL;)
	{
	    next = iter->next;
	    i_cluster.free(iter);
	    iter = next;
	}
	if (i_memory != NULL)
	    delete [] i_memory;
    }

    void *alloc(size_t size)
    {
	void *retval = NULL;
	MemoryNode *iter = NULL, *useiter = NULL;
	size_t diff = 0;
	for (iter=i_nodes; iter != NULL; iter = iter->next)
	{
	    // Find smallest segment we have that fits ...
	    if ((iter->avail() && iter->size() >= size) &&
	    	(diff == 0 || iter->i_size - size < diff))
	    {
		diff = iter->i_size - size;
		useiter = iter;
	    }
	}
	iter = useiter;
	if (diff != 0)
	{
	    useiter->i_size = size;
	    // Add a new memory block if the next is 
	    iter = iter->next;
	    if (iter != NULL)
	    {
		MemoryNode *tmp = (MemoryNode *) i_cluster.malloc(sizeof(MemoryNode));
		if (tmp == NULL)
		    return NULL;
		tmp->init((void *) (((char *) iter->i_loc) - diff), diff);
		if (iter->prev != NULL)
		{
		    iter->prev->next = tmp;
		}
		tmp->prev = iter->prev;
		tmp->next = iter;
		iter->prev = tmp;
	    }
	}
	else if (useiter == NULL && size <= i_avail)
	{
	    MemoryNode *tmp = (MemoryNode *) i_cluster.malloc(sizeof(MemoryNode));
	    if (tmp == NULL)
		return NULL;
	    if (i_nodes != NULL)
	    {
		for (iter=i_nodes; iter->next != NULL; iter = iter->next) ;
		tmp->init((void *) (((char *) iter->i_loc) + iter->i_size), size);
		iter->next = tmp;
		iter->next->prev = iter;
	    }
	    else
	    {
		tmp->init((void *) i_memory, size);
		i_nodes = tmp;
	    }
	    useiter = tmp;
	    i_avail -= size;
	}
	if (useiter != NULL)
	{
	    retval = useiter->i_loc;
	    useiter->i_avail = false;
	}
	return retval;
    }
    void dealloc(void *loc)
    {
	MemoryNode *iter = NULL, *iternext = NULL;
	for (iter=i_nodes; iter!=NULL; iter = iter->next)
	{
	    if (iter->i_loc == loc)
		break;
	}

	// We found it in the for ...
	if (iter != NULL)
	{
	    // make it one bigger chunk, if we can!
	    // We do this by seeing if we can combine
	    // with the nodes before/after us.
	    iternext = iter->next;
	    if (iternext != NULL)
	    {
		// We got a chunk next, is it free ...
		if (iternext->avail())
		{
		    iter->i_size += iternext->i_size;
		    if (iternext->prev != NULL)
		    {
			iternext->prev->next = iternext->next;
		    }
		    else
		    {
			i_nodes = iternext->next;
		    }
		    if (iternext->next != NULL)
		    {
			iternext->next->prev = iternext->prev;
		    }
		    i_cluster.free(iternext);
		}
	    }
	    if (iter->prev != NULL)
	    {
		// We're not the first, see if we can do it again ...
		if (iter->prev->avail())
		{
		    iter->prev->i_size += iter->i_size;
		    iter->prev->next = iter->next;
		    if (iter->next != NULL)
		    {
			iter->next->prev = iter->prev;
		    }
		    iternext = iter;
		    iter = iter->prev;
		    i_cluster.free(iternext);
		}
	    }
	    else
	    {
		i_nodes = iter;
	    }

	    // If we are now the LAST node, we're free, so
	    // just free up this node and return it to this
	    // chunk's 'free' pool.
	    if (iter->next == NULL)
	    {
		i_avail += iter->i_size;
		if (iter->prev)
		{
		    iter->prev->next = NULL;
		}
		else
		{
		    i_nodes = NULL;
		}
		i_cluster.free(iter);
	    }
	    else
	    {
		iter->i_avail = true;
	    }
	}
    }
    bool have(size_t size)
    {
	if (size <= i_avail)
	    return true;

	MemoryNode *iter;
	for (iter=i_nodes; iter != NULL; iter = iter->next)
	{
	    if (iter->avail() && size <= iter->size())
		return true;
	}
	return false;
    }
    bool ismine(void *loc)
    {
	MemoryNode *iter;
	for (iter=i_nodes; iter != NULL; iter = iter->next)
	{
	    if (loc == iter->i_loc)
		return true;
	}
	return false;
    }
    size_t avail() { return i_avail; }
    size_t size() { return i_size; }
};

template <class ACE_LOCK> class MemoryManager
{
    MemoryBlock *i_blocks;
    size_t i_blocksize;
    char lockname[15];

public:
    MemoryManager()
    {
	i_blocks = NULL;
	i_blocksize = 0;
	memset(lockname, 0, 15);
    }
    ~MemoryManager()
    {
	MemoryBlock *iter, *next;
	for (iter=i_blocks; iter!=NULL; )
	{
	    next = iter->next;
	    delete iter;
	    iter = next;
	}
    }

    void init(size_t blocksize = DEF_MEMSIZE)
    {
	sprintf(lockname, "MM_%p", this);
	ACE_LOCK lock(lockname);
	lock.acquire();
	if (i_blocks == NULL)
	{
	    i_blocksize = blocksize;
	    i_blocks = new MemoryBlock(i_blocksize);
	    i_blocks->init();
	}
	lock.release();
    }

    void *alloc(size_t size, size_t blocksize = DEF_MEMSIZE)
    {
	void *retval = NULL;
	MemoryBlock *iter = NULL;

	if (i_blocks == NULL)
	    init(blocksize);

	ACE_LOCK lock(lockname);
	lock.acquire();
	if (size > i_blocksize)
	{
	    for (iter=i_blocks; iter->next != NULL; iter = iter->next) ;
	    iter->next = new MemoryBlock(size);
	    iter->next->prev = iter;
	    iter = iter->next;
	    iter->init();
	}
	else
	{
	    for (iter=i_blocks; iter != NULL; iter = iter->next)
	    {
		if (iter->have(size))
		    break;
	    }
	    if (iter == NULL)
	    {
		for (iter=i_blocks; iter->next != NULL; iter = iter->next) ;
		iter->next = new MemoryBlock(i_blocksize);
		iter->next->prev = iter;
		iter = iter->next;
		iter->init();
	    }
	}
	retval = iter->alloc(size);
	lock.release();
	return retval;
    }
    void dealloc(void *loc)
    {
	MemoryBlock *iter = NULL;

	if (i_blocks == NULL)
	    return;

	ACE_LOCK lock(lockname);
	lock.acquire();
	for (iter=i_blocks; iter != NULL; iter = iter->next)
	{
	    if (iter->ismine(loc))
		break;
	}
	if (iter != NULL)
	{
	    iter->dealloc(loc);
	    if (iter->avail() >= iter->size())
	    {
		// NOT the only entry ...
		if (!(iter->prev == NULL && iter->next == NULL))
		{
		    if (iter->prev != NULL)
			iter->prev->next = iter->next;
		    else
			i_blocks = iter->next;
		    if (iter->next != NULL)
			iter->next->prev = iter->prev;
		    delete iter;
		}
	    }
	}

	lock.release();
    }

    size_t blocksize()
	{ return i_blocksize; }
    void blocksize(size_t blocksize)
	{ i_blocksize = blocksize; }
    size_t blocks()
	{
	    size_t count = 0;
	    for (iter=i_blocks; iter != NULL; iter = iter->next)
		count++;
	    return count;
	}
};

#endif
