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
** Revision 1.5  2000/11/09 10:58:19  prez
** THINK I have it working again ... with the free list.
** Will check, still thinking of sorting free list by size.
**
** Revision 1.4  2000/10/26 07:59:52  prez
** The goddamn memory system and mstring WORK!  Well, so far ;)
**
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

#define DEF_MEMSIZE 4096

class MemoryNode
{
    friend class MemoryBlock;

    void *i_loc;
    bool i_avail;
    size_t i_size;
    MemoryNode *prev, *next;

public:
    MemoryNode()
    {
	prev = next = NULL;
	i_loc = NULL;
	i_size = 0;
	i_avail = false;
    }
    MemoryNode(void *loc, size_t size)
    {
	prev = next = NULL;
	init(loc, size);
    }
    void init(void *loc, size_t size)
    {
	i_loc = loc;
	i_size = size;
	i_avail = true;
    }

    bool avail() { return i_avail; }
    size_t size() { return i_size; }
};

class MemoryNodeFree
{
    friend class MemoryBlock;
    MemoryNode *i_node;
    MemoryNodeFree *prev, *next;
public:
    MemoryNodeFree(MemoryNode *node = NULL) { init(node); }
    void init(MemoryNode *node = NULL)
    {
	prev = next = NULL;
	set(node);
    }
    void set(MemoryNode *node)
    {
	i_node = node;
    }
    MemoryNode *node()
    {
	return i_node;
    }
    size_t size()
    {
	if (i_node != NULL)
	    return i_node->size();
	else
	    return 0;
    }
};

#define CLUSTER_BLOCK_SIZE ((sizeof(MemoryNode) < sizeof(MemoryNodeFree)) ? \
			sizeof(MemoryNodeFree) : sizeof(MemoryNode))

class MemoryBlock
{
    char *i_memory;
    size_t i_size;
    size_t i_avail;
    ACE_Expandable_Cached_Fixed_Allocator<ACE_Thread_Mutex> i_cluster;
    MemoryNode *i_nodes, *i_end;
    MemoryNodeFree *i_free;

    bool LinkNextNode(MemoryNode *node, MemoryNode *next)
    {
	if (node == NULL || next == NULL)
	    return false;
	next->prev = node;
	next->next = node->next;
	if (node->next != NULL)
	    node->next->prev = next;
	else
	{
	    i_end = next;
	    i_end->next = NULL;
	}
	node->next = next;
	return true;
    }
    bool LinkPrevNode(MemoryNode *node, MemoryNode *prev)
    {
	if (node == NULL || prev == NULL)
	    return false;
	prev->next = node;
	prev->prev = node->prev;
	if (node->prev != NULL)
	    node->prev->next = prev;
	else
	{
	    i_nodes = prev;
	    i_nodes->prev = NULL;
	}
	node->prev = prev;
	return true;
    }
    bool RemoveNode(MemoryNode *node)
    {
	if (node == NULL)
	    return false;
	if (node->prev != NULL)
	    node->prev->next = node->next;
	else
	{
	    i_nodes = node->next;
	    if (i_nodes != NULL)
		i_nodes->prev = NULL;
	}
	if (node->next != NULL)
	    node->next->prev = node->prev;
	else
	{
	    i_end = node->prev;
	    if (i_end != NULL)
		i_end->next = NULL;
	}
	return true;
    }

    bool LinkNextNodeFree(MemoryNodeFree *node, MemoryNodeFree *next)
    {
	if (node == NULL || next == NULL)
	    return false;
	next->prev = node;
	next->next = node->next;
	if (node->next != NULL)
	    node->next->prev = next;
	else
	    next->next = NULL;
	node->next = next;
	return true;
    }
    bool LinkPrevNodeFree(MemoryNodeFree *node, MemoryNodeFree *prev)
    {
	if (node == NULL || prev == NULL)
	    return false;
	prev->next = node;
	prev->prev = node->prev;
	if (node->prev != NULL)
	    node->prev->next = prev;
	else
	{
	    i_free = prev;
	    i_free->prev = NULL;
	}
	node->prev = prev;
	return true;
    }
    bool RemoveNodeFree(MemoryNodeFree *node)
    {
	if (node == NULL)
	    return false;
	if (node->prev != NULL)
	{
	    if (node->next != NULL)
		node->prev->next = node->next;
	    else
		node->prev->next = NULL;
	}
	else
	{
	    i_free = node->next;
	    if (i_free != NULL)
		i_free->prev = NULL;
	}
	if (node->next != NULL)
	    node->next->prev = node->prev;
	return true;
    }


public:
    MemoryBlock *prev, *next;
    MemoryBlock(size_t size)
	: i_cluster(CLUSTER_BLOCK_SIZE, 32)
    {
	i_avail = 0;
	i_size = size;
	i_memory = NULL;
	i_nodes = i_end = NULL;
	i_free = NULL;
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
	MemoryNodeFree *fiter = NULL, *fnext = NULL;
	for (fiter = i_free; fiter != NULL;)
	{
	    fnext = fiter->next;
	    i_cluster.free(fiter);
	    fiter = fnext;
	}
	if (i_memory != NULL)
	    delete [] i_memory;
    }

    void *alloc(size_t size)
    {
	void *retval = NULL;
	MemoryNodeFree *fiter = NULL, *fitertmp = NULL, *fuseiter = NULL;
	MemoryNode *iter = NULL, *useiter = NULL;
	size_t diff = 0;

	for (fiter=i_free; fiter != NULL; fiter = fiter->next)
	{
	    // Find smallest segment we have that fits ...
	    if ((fiter->size() >= size) &&
	    	(diff == 0 || fiter->size() - size < diff))
	    {
		diff = fiter->size() - size;
		fuseiter = fiter;
		if (diff == 0)
		    break;
	    }
	}
	if (fuseiter != NULL)
	{
	    iter = useiter = fuseiter->node();
	}
	if (diff != 0)
	{
	    // Add a new memory block if the next is 
	    iter = iter->next;
	    if (iter != NULL)
	    {
		MemoryNode *tmp = (MemoryNode *) i_cluster.malloc(sizeof(MemoryNode));
		if (tmp == NULL)
		    return NULL;
		tmp->init((void *) (((char *) iter->i_loc) - diff), diff);
		LinkPrevNode(iter, tmp);
		fuseiter->set(tmp);
		fuseiter = NULL;
	    }
	    useiter->i_size = size;
	}
	else if (useiter == NULL && size <= i_avail)
	{
	    MemoryNode *tmp = (MemoryNode *) i_cluster.malloc(sizeof(MemoryNode));
	    if (tmp == NULL)
		return NULL;
	    if (i_nodes != NULL)
	    {
		iter = i_end;
		tmp->init((void *) (((char *) iter->i_loc) + iter->i_size), size);
		LinkNextNode(iter, tmp);
	    }
	    else
	    {
		tmp->init((void *) i_memory, size);
		i_nodes = tmp;
		i_end = tmp;
		i_nodes->prev = NULL;
		i_end->next = NULL;
	    }
	    useiter = tmp;
	    i_avail -= size;
	}
	if (useiter != NULL)
	{
	    retval = useiter->i_loc;
	    useiter->i_avail = false;
	}
	if (fuseiter != NULL)
	{
	    RemoveNodeFree(fuseiter);
	    i_cluster.free(fuseiter);
	}
	return retval;
    }
    void dealloc(void *loc)
    {
	MemoryNode *iter = NULL, *iternext = NULL;
	MemoryNodeFree *fiter = NULL, *fiternext = NULL;

	if (!(loc >= i_memory && loc < i_memory + i_size))
	    return;

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
		    RemoveNode(iternext);
		    for (fiter = i_free; fiter != NULL; fiter = fiter->next)
		    {
			if (fiter->node() == iternext)
			{
			    RemoveNodeFree(fiter);
			    i_cluster.free(fiter);
			    break;
			}
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
		    iternext = iter;
		    iter = iter->prev;
		    RemoveNode(iternext);
		    for (fiter = i_free; fiter != NULL; fiter = fiter->next)
		    {
			if (fiter->node() == iternext)
			{
			    RemoveNodeFree(fiter);
			    i_cluster.free(fiter);
			    break;
			}
		    }
		    i_cluster.free(iternext);
		}
	    }

	    // If we are now the LAST node, we're free, so
	    // just free up this node and return it to this
	    // chunk's 'free' pool.
	    if (iter->next == NULL)
	    {
		i_avail += iter->i_size;
		RemoveNode(iter);
		for (fiter = i_free; fiter != NULL; fiter = fiter->next)
		{
		    if (fiter->node() == iter)
		    {
			RemoveNodeFree(fiter);
			i_cluster.free(fiter);
			break;
		    }
		}
		i_cluster.free(iter);
	    }
	    else
	    {
		iter->i_avail = true;
		MemoryNodeFree *tmp = (MemoryNodeFree *) i_cluster.malloc(sizeof(MemoryNodeFree));
		tmp->init(iter);
		if (i_free != NULL)
		{
		    LinkPrevNodeFree(i_free, tmp);
		}
		else
		    tmp->next = NULL;
	    }
	}
    }
    bool have(size_t size)
    {
	if (size <= i_avail)
	    return true;

	MemoryNodeFree *iter;
	for (iter=i_free; iter != NULL; iter = iter->next)
	{
	    if (size <= iter->size())
		return true;
	}
	return false;
    }
    bool ismine(void *loc)
    {
	return (loc >= i_memory && loc < i_memory + i_size);
    }
    size_t avail() { return i_avail; }
    size_t size() { return i_size; }
};

template <class ACE_LOCK> class MemoryManager
{
    MemoryBlock *i_blocks;
    size_t i_blocksize;
    char lockname[15];
    ACE_LOCK *lock;

public:
    MemoryManager()
    {
	i_blocks = NULL;
	i_blocksize = 0;
	memset(lockname, 0, 15);
	lock = NULL;
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
	delete lock;
    }

    void init(size_t blocksize = DEF_MEMSIZE)
    {
	if (lockname[0]==0)
	    sprintf(lockname, "MM_%p", this);
	if (lock == NULL)
	{
	    ACE_NEW(lock, ACE_LOCK(lockname));
	}
	ACE_GUARD(ACE_LOCK, lock_mon, *lock);
	if (i_blocks == NULL)
	{
	    i_blocksize = blocksize;
	    
	    ACE_NEW(i_blocks, MemoryBlock(i_blocksize));
	    i_blocks->init();
	}
    }

    void *alloc(size_t size, size_t blocksize = DEF_MEMSIZE)
    {
	void *retval = NULL;
	MemoryBlock *iter = NULL;

	if (i_blocks == NULL)
	    init(blocksize);
	if (i_blocks == NULL)
	    return NULL;

	ACE_GUARD_RETURN(ACE_LOCK, lock_mon, *lock, 0);
	if (size > i_blocksize)
	{
	    MemoryBlock *tmp;
	    ACE_NEW_RETURN(tmp, MemoryBlock(size), 0);
	    for (iter=i_blocks; iter->next != NULL; iter = iter->next) ;
	    iter->next = tmp;
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
		MemoryBlock *tmp;
		ACE_NEW_RETURN(tmp, MemoryBlock(i_blocksize), 0);
		for (iter=i_blocks; iter->next != NULL; iter = iter->next) ;
		iter->next = tmp;
		iter->next->prev = iter;
		iter = iter->next;
		iter->init();
	    }
	}
	retval = iter->alloc(size);
	return retval;
    }
    void dealloc(void *loc)
    {
	MemoryBlock *iter = NULL;

	if (i_blocks == NULL)
	    return;

	ACE_GUARD(ACE_LOCK, lock_mon, *lock);
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
    size_t size()
	{
	    size_t count = 0;
	    for (iter=i_blocks; iter != NULL; iter = iter->next)
		count += iter->size();
	    return count;
	}
};

#endif
