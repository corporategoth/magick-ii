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
** Revision 1.6  2000/11/28 10:15:05  prez
** Trying to figure out why the free list doesnt work in mmemory.
** ALOT OF PRINTF'S - YOU'VE BEEN WARNED, OUTPUT STDOUT TO A FILE!
**
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

#define DEF_MEMSIZE 8192

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

    bool LinkNextNode(MemoryNode *node, MemoryNode *nextnode)
    {
printf("LinkNextNode(%p, %p)\n", node, nextnode); fflush(stdout);
	if (node == NULL || nextnode == NULL)
	    return false;
printf("%p <- %p %p -> %p\n",
		node->prev, ((node->prev != NULL) ? node->prev->next : NULL),
		((node->next != NULL) ? node->next->prev : NULL), node->next); fflush(stdout);
	nextnode->prev = node;
	nextnode->next = node->next;
	if (node->next != NULL)
	    node->next->prev = nextnode;
	else
	{
	    i_end = nextnode;
	    i_end->next = NULL;
	}
	node->next = nextnode;
printf("%p <- %p %p -> %p %p -> %p\n",
		node->prev, ((node->prev != NULL) ? node->prev->next : NULL),
		((node->next != NULL) ? node->next->prev : NULL), node->next,
		((node->next->next != NULL) ? node->next->next->prev : NULL),
		node->next->next); fflush(stdout);
printf("i_nodes = %p, i_end = %p\n", i_nodes, i_end); fflush(stdout);
	return true;
    }
    bool LinkPrevNode(MemoryNode *node, MemoryNode *prevnode)
    {
printf("LinkPrevNode(%p, %p)\n", node, prevnode); fflush(stdout);
	if (node == NULL || prevnode == NULL)
	    return false;
printf("%p <- %p %p -> %p\n",
		node->prev, ((node->prev != NULL) ? node->prev->next : NULL),
		((node->next != NULL) ? node->next->prev : NULL), node->next); fflush(stdout);
	prevnode->next = node;
	prevnode->prev = node->prev;
	if (node->prev != NULL)
	    node->prev->next = prevnode;
	else
	{
	    i_nodes = prevnode;
	    i_nodes->prev = NULL;
	}
	node->prev = prevnode;
printf("%p <- %p %p <- %p %p -> %p\n",
		node->prev->prev, ((node->prev->prev != NULL) ?
			node->prev->prev->next : NULL),
		node->prev, ((node->prev != NULL) ? node->prev->next : NULL),
		((node->next != NULL) ? node->next->prev : NULL), node->next); fflush(stdout);
printf("i_nodes = %p, i_end = %p\n", i_nodes, i_end); fflush(stdout);
	return true;
    }
    bool RemoveNode(MemoryNode *node)
    {
printf("RemoveNode(%p)\n", node); fflush(stdout);
	if (node == NULL)
	    return false;
printf("%p <- %p %p -> %p\n",
		node->prev, ((node->prev != NULL) ? node->prev->next : NULL),
		((node->next != NULL) ? node->next->prev : NULL), node->next); fflush(stdout);
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
printf("%p <- %p %p -> %p\n",
		node->prev, ((node->prev != NULL) ? node->prev->next : NULL),
		((node->next != NULL) ? node->next->prev : NULL), node->next); fflush(stdout);
printf("i_nodes = %p, i_end = %p\n", i_nodes, i_end); fflush(stdout);
	return true;
    }

    bool LinkNextNodeFree(MemoryNodeFree *node, MemoryNodeFree *nextnode)
    {
printf("LinkNextNodeFree(%p, %p)\n", node, nextnode); fflush(stdout);
	if (node == NULL || nextnode == NULL)
	    return false;
printf("%p <- %p %p -> %p\n",
		node->prev, ((node->prev != NULL) ? node->prev->next : NULL),
		((node->next != NULL) ? node->next->prev : NULL), node->next); fflush(stdout);
	nextnode->prev = node;
	nextnode->next = node->next;
	if (node->next != NULL)
	    node->next->prev = nextnode;
	else
	    nextnode->next = NULL;
	node->next = nextnode;
printf("%p <- %p %p -> %p %p -> %p\n",
		node->prev, ((node->prev != NULL) ? node->prev->next : NULL),
		((node->next != NULL) ? node->next->prev : NULL), node->next,
		((node->next->next != NULL) ? node->next->next->prev : NULL),
		node->next->next); fflush(stdout);
printf("i_free = %p\n", i_free); fflush(stdout);
	return true;
    }
    bool LinkPrevNodeFree(MemoryNodeFree *node, MemoryNodeFree *prevnode)
    {
printf("LinkPrevNodeFree(%p, %p)\n", node, prevnode); fflush(stdout);
	if (node == NULL || prevnode == NULL)
	    return false;
printf("%p <- %p %p -> %p\n",
		node->prev, ((node->prev != NULL) ? node->prev->next : NULL),
		((node->next != NULL) ? node->next->prev : NULL), node->next); fflush(stdout);
	prevnode->next = node;
	prevnode->prev = node->prev;
	if (node->prev != NULL)
	    node->prev->next = prevnode;
	else
	{
	    i_free = prevnode;
	    i_free->prev = NULL;
	}
	node->prev = prevnode;
printf("%p <- %p %p <- %p %p -> %p\n",
		node->prev->prev, ((node->prev->prev != NULL) ?
			node->prev->prev->next : NULL),
		node->prev, ((node->prev != NULL) ? node->prev->next : NULL),
		((node->next != NULL) ? node->next->prev : NULL), node->next); fflush(stdout);
printf("i_free = %p\n", i_free); fflush(stdout);
	return true;
    }
    bool RemoveNodeFree(MemoryNodeFree *node)
    {
printf("RemoveNodeFree(%p)\n", node); fflush(stdout);
	if (node == NULL)
	    return false;
printf("Test: %d <- %d -> %d\n", ((node->prev != NULL) ? (int) node->prev->size() : -1),
		node->size(), ((node->next != NULL) ? (int) node->next->size() : -1));
printf("%p <- %p %p -> %p\n",
		node->prev, ((node->prev != NULL) ? node->prev->next : NULL),
		((node->next != NULL) ? node->next->prev : NULL), node->next); fflush(stdout);
	if (node->prev != NULL)
	{
	    node->prev->next = node->next;
	}
	else
	{
	    i_free = node->next;
	    if (i_free != NULL)
		i_free->prev = NULL;
	}
	if (node->next != NULL)
	    node->next->prev = node->prev;
printf("%p <- %p %p -> %p\n",
		node->prev, ((node->prev != NULL) ? node->prev->next : NULL),
		((node->next != NULL) ? node->next->prev : NULL), node->next); fflush(stdout);
printf("Test: %d <- %d -> %d\n", ((node->prev != NULL) ? (int) node->prev->size() : -1),
		node->size(), ((node->next != NULL) ? (int) node->next->size() : -1));
printf("i_free = %p\n", i_free); fflush(stdout);
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
	MemoryNodeFree *fiter = NULL;
	MemoryNode *iter = NULL, *useiter = NULL;
	size_t diff = 0;
printf("alloc(%d) : i_memory = %p, i_size = %d\n", size,
	i_memory, i_size); fflush(stdout);

	if (size == 0)
	    return NULL;

	for (fiter=i_free; fiter != NULL; fiter = fiter->next)
	{
printf("fiter = %p, fiter->node() = %p\n", fiter, fiter->node()); fflush(stdout);
printf("fiter->node()->i_loc = %p, fiter->size() = %d, fiter->next = %p\n",
	fiter->node()->i_loc, fiter->size(), fiter->next); fflush(stdout);
	    // Find smallest segment we have that fits ...
	    if (fiter->size() >= size)
	    {
		diff = fiter->size() - size;
		break;
	    }
	}
	if (fiter != NULL)
	{
	    iter = useiter = fiter->node();
	}
	if (diff != 0)
	{
printf("Size Difference - %d\n", diff); fflush(stdout);
	    // Add a new memory block if the next is 
	    iter = iter->next;
	    if (iter != NULL)
	    {
printf("Creating intermitant record\n"); fflush(stdout);
		MemoryNodeFree *ftmp = NULL, *fiternext = NULL;
		MemoryNode *tmp = (MemoryNode *) i_cluster.malloc(sizeof(MemoryNode));

		if (tmp == NULL)
		    return NULL;
		tmp->init((void *) (((char *) iter->i_loc) - diff), diff);
		LinkPrevNode(iter, tmp);
		RemoveNodeFree(fiter);
		ftmp = fiter;
		ftmp->set(tmp);
		for (fiter=i_free; fiter != NULL; fiter = fiter->next)
		{
		    if (fiter->size() > tmp->size())
		    {
			LinkPrevNodeFree(fiter, ftmp);
			break;
		    }
		    fiternext = fiter;
		}
		if (fiter == NULL && fiternext != NULL)
		{
		    LinkNextNodeFree(fiternext, ftmp);
		}
		else /* should never be the case ... */
		{
		    i_free = ftmp;
		    ftmp->next = NULL;
		    ftmp->prev = NULL;
		}
		fiter = NULL;
	    }
	    useiter->i_size = size;
	}
	else if (useiter == NULL && size <= i_avail)
	{
printf("Not found - appending\n"); fflush(stdout);
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
printf("We have record (%p)!\n", useiter); fflush(stdout);
	    retval = useiter->i_loc;
	    useiter->i_avail = false;
	}
	if (fiter != NULL)
	{
printf("We HAD a free node ...\n"); fflush(stdout);
	    RemoveNodeFree(fiter);
	    i_cluster.free(fiter);
	}
printf("~alloc(%p)\n", retval); fflush(stdout);
	return retval;
    }
    void dealloc(void *loc)
    {
	MemoryNode *iter = NULL, *iternext = NULL;
	MemoryNodeFree *fiter = NULL;
printf("dealloc(%p) : i_memory = %p, i_size = %d\n", loc,
	i_memory, i_size); fflush(stdout);

	if (!(loc >= i_memory && loc < i_memory + i_size))
	    return;

	for (iter=i_nodes; iter!=NULL; iter = iter->next)
	{
printf("CHK: iter = %p\n", iter); fflush(stdout);
printf("CHK: iter->i_loc = %p\n", iter->i_loc); fflush(stdout);
	    if (iter->i_loc == loc)
		break;
	}

	// We found it in the for ...
	if (iter != NULL)
	{
printf("Found specific element\n"); fflush(stdout);
printf("iter = %p, iter->size = %d\n", iter, iter->i_size); fflush(stdout);
	    // make it one bigger chunk, if we can!
	    // We do this by seeing if we can combine
	    // with the nodes before/after us.
	    if (iter->next != NULL && iter->next->avail())
	    {
printf("Joining to next\n"); fflush(stdout);
		iternext = iter->next;
printf("iter = %p, iternext = %p, iter->size = %d, iternext->size = %d\n",
	iter, iternext, iter->i_size, iternext->i_size); fflush(stdout);
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
printf("iter = %p, iternext = %p, iter->size = %d, iternext->size = %d\n",
	iter, iternext, iter->i_size, iternext->i_size); fflush(stdout);
		i_cluster.free(iternext);
	    }
	    if (iter->prev != NULL && iter->prev->avail())
	    {
printf("Joining to prev\n"); fflush(stdout);
		iternext = iter;
		iter = iter->prev;
printf("iter = %p, iternext = %p, iter->size = %d, iternext->size = %d\n",
	iter, iternext, iter->i_size, iternext->i_size); fflush(stdout);
		iter->i_size += iternext->i_size;
		RemoveNode(iternext);
		/*
		 * Useless processing, we KNOW we're avail ...
		for (fiter = i_free; fiter != NULL; fiter = fiter->next)
		{
		    if (fiter->node() == iternext)
		    {
			RemoveNodeFree(fiter);
			i_cluster.free(fiter);
			break;
		    }
		} */
printf("iter = %p, iternext = %p, iter->size = %d, iternext->size = %d\n",
	iter, iternext, iter->i_size, iternext->i_size); fflush(stdout);
		i_cluster.free(iternext);
	    }

	    // If we are now the LAST node, we're free, so
	    // just free up this node and return it to this
	    // chunk's 'free' pool.
printf("iter = %p, iter->size = %d\n", iter, iter->i_size); fflush(stdout);
	    if (iter->next == NULL)
	    {
printf("Last node, freeing up space\n"); fflush(stdout);
printf("iter = %p, iter->size = %d\n, i_avail = %d", iter,
	iter->i_size, i_avail); fflush(stdout);
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
printf("iter = %p, iter->size = %d\n, i_avail = %d", iter,
	iter->i_size, i_avail); fflush(stdout);
		i_cluster.free(iter);
	    }
	    else
	    {
printf("Creating free node\n"); fflush(stdout);
		iter->i_avail = true;
		MemoryNodeFree *ftmp = NULL, *fiternext = NULL;
		ftmp = (MemoryNodeFree *) i_cluster.malloc(sizeof(MemoryNodeFree));
printf("Created ftmp = %p\n", ftmp); fflush(stdout);
		ftmp->init(iter);
printf("ftmp->node() = %p, ftmp->size() = %d\n", ftmp->node(),
	ftmp->size()); fflush(stdout);
		for (fiter=i_free; fiter != NULL; fiter = fiter->next)
		{
printf("Checking if %p (%d) > %d\n", fiter, fiter->size(), iter->size());
	fflush(stdout);
		    if (fiter->size() > iter->size())
		    {
printf("Linked next node\n"); fflush(stdout);
			LinkPrevNodeFree(fiter, ftmp);
			break;
		    }
		    fiternext = fiter;
		}
		if (fiter == NULL && fiternext != NULL)
		{
printf("fiter == NULL && fiternext = %p\n", fiternext); fflush(stdout);
		    LinkNextNodeFree(fiternext, ftmp);
		}
		else
		{
printf("New node is free list\n"); fflush(stdout);
		    i_free = ftmp;
		    ftmp->next = NULL;
		    ftmp->prev = NULL;
		}
printf("iter = %p, iter->size = %d\n", iter, iter->i_size); fflush(stdout);
printf("ftmp = %p, ftmp->size = %d\n", ftmp, ftmp->size()); fflush(stdout);
	    }
	}
printf("~dealloc()\n"); fflush(stdout);
    }
    bool have(size_t size)
    {
printf("have(%d) : i_memory = %p, i_size = %d\n", size,
	i_memory, i_size); fflush(stdout);
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
printf("ismine(%p) : i_memory = %p, i_size = %d\n", loc,
	i_memory, i_size); fflush(stdout);
	return (loc >= i_memory && loc < i_memory + i_size);
    }
    size_t avail() { return i_avail; }
    size_t size() { return i_size; }
};

template <class ACE_LOCK> class MemoryManager
{
    MemoryBlock *i_blocks, *i_end;
    size_t i_blocksize;
    char lockname[15];
    ACE_LOCK *lock;

    bool LinkNextBlock(MemoryBlock *block, MemoryBlock *nextblock)
    {
printf("LinkNextBlock(%p, %p)\n", block, nextblock); fflush(stdout);
	if (block == NULL || nextblock == NULL)
	    return false;
printf("%p <- %p %p -> %p\n",
		block->prev, ((block->prev != NULL) ? block->prev->next : NULL),
		((block->next != NULL) ? block->next->prev : NULL), block->next); fflush(stdout);
	nextblock->prev = block;
	nextblock->next = block->next;
	if (block->next != NULL)
	    block->next->prev = nextblock;
	else
	{
	    i_end = nextblock;
	    i_end->next = NULL;
	}
	block->next = nextblock;
printf("%p <- %p %p -> %p %p -> %p\n",
		block->prev, ((block->prev != NULL) ? block->prev->next : NULL),
		((block->next != NULL) ? block->next->prev : NULL), block->next,
		((block->next->next != NULL) ? block->next->next->prev : NULL),
		block->next->next); fflush(stdout);
printf("i_blocks = %p, i_end = %p\n", i_blocks, i_end); fflush(stdout);
	return true;
    }
    bool LinkPrevBlock(MemoryBlock *block, MemoryBlock *prevblock)
    {
printf("LinkPrevBlock(%p, %p)\n", block, prevblock); fflush(stdout);
	if (block == NULL || prevblock == NULL)
	    return false;
printf("%p <- %p %p -> %p\n",
		block->prev, ((block->prev != NULL) ? block->prev->next : NULL),
		((block->next != NULL) ? block->next->prev : NULL), block->next); fflush(stdout);
	prevblock->next = block;
	prevblock->prev = block->prev;
	if (block->prev != NULL)
	    block->prev->next = prevblock;
	else
	{
	    i_blocks = prevblock;
	    i_blocks->prev = NULL;
	}
	block->prev = prevblock;
printf("%p <- %p %p <- %p %p -> %p\n",
		block->prev->prev, ((block->prev->prev != NULL) ?
			block->prev->prev->next : NULL),
		block->prev, ((block->prev != NULL) ? block->prev->next : NULL),
		((block->next != NULL) ? block->next->prev : NULL), block->next); fflush(stdout);
printf("i_blocks = %p, i_end = %p\n", i_blocks, i_end); fflush(stdout);
	return true;
    }
    bool RemoveBlock(MemoryBlock *block)
    {
printf("RemoveBlock(%p)\n", block); fflush(stdout);
	if (block == NULL)
	    return false;
printf("%p <- %p %p -> %p\n",
		block->prev, ((block->prev != NULL) ? block->prev->next : NULL),
		((block->next != NULL) ? block->next->prev : NULL), block->next); fflush(stdout);
	if (block->prev != NULL)
	    block->prev->next = block->next;
	else
	{
	    i_blocks = block->next;
	    if (i_blocks != NULL)
		i_blocks->prev = NULL;
	}
	if (block->next != NULL)
	    block->next->prev = block->prev;
	else
	{
	    i_end = block->prev;
	    if (i_end != NULL)
		i_end->next = NULL;
	}
printf("%p <- %p %p -> %p\n",
		block->prev, ((block->prev != NULL) ? block->prev->next : NULL),
		((block->next != NULL) ? block->next->prev : NULL), block->next); fflush(stdout);
printf("i_blocks = %p, i_end = %p\n", i_blocks, i_end); fflush(stdout);
	return true;
    }

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
	    i_end = i_blocks;
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
	    ACE_NEW_RETURN(iter, MemoryBlock(size), 0);
	    LinkNextBlock(i_end, iter);
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
		ACE_NEW_RETURN(iter, MemoryBlock(i_blocksize), 0);
		LinkNextBlock(i_end, iter);
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
		if (i_blocks != i_end)
		{
		    RemoveBlock(iter);
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
