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
** Revision 1.7  2000/12/10 05:14:20  prez
** STL based memory clustering -- however it does NOT work
** currently -- so it is disabled (it doesnt work due to
** STL being broken!)
**
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
#define CLUSTER_SIZE 8

class MemoryNode
{
    friend class MemoryBlock;

    void *i_loc;
    bool i_avail;
    size_t i_size;

public:
    MemoryNode()
    {
	i_loc = NULL;
	i_size = 0;
	i_avail = false;
    }
    MemoryNode(void *loc, size_t size)
    {
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

public:
    MemoryNodeFree(MemoryNode *node = NULL) { init(node); }
    void init(MemoryNode *node = NULL)
    {
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

class MemoryBlock
{
    char *i_memory;
    size_t i_size;
    size_t i_avail;
    ACE_Expandable_Cached_Allocator<MemoryNode, ACE_Thread_Mutex> i_cluster;
    list<MemoryNode *> i_nodes;
    list<MemoryNodeFree> i_free;

    typedef list<MemoryNode *>::iterator N_iter;
    typedef list<MemoryNodeFree>::iterator F_iter;

public:
    MemoryBlock(size_t size)
	: i_cluster(CLUSTER_SIZE)
    {
	i_avail = 0;
	i_size = size;
	i_memory = NULL;
    }
    void init()
    {
	i_avail = i_size;
	ACE_NEW(i_memory, char[i_size]);
    }
    ~MemoryBlock()
    {
	N_iter iter;
	for (iter = i_nodes.begin(); iter != i_nodes.end(); iter++)
	    i_cluster.free(*iter);

	if (i_memory != NULL)
	    delete [] i_memory;
    }

    void *alloc(size_t size)
    {
	void *retval = NULL;
	F_iter fiter;
	N_iter iter;
	MemoryNode *useiter = NULL;
	size_t diff = 0;

	if (size == 0)
	    return NULL;

	for (fiter=i_free.begin(); fiter != i_free.end(); fiter++)
	{
	    // Find smallest segment we have that fits ...
	    if (fiter->size() >= size)
	    {
		diff = fiter->size() - size;
		break;
	    }
	}

	if (fiter != i_free.end())
	{
	    useiter = fiter->node();
	    for (iter=i_nodes.begin(); iter != i_nodes.end(); iter++)
	    {
		if (useiter == (*iter))
		    break;
	    }
	    if (iter == i_nodes.end())
	    {
		// Oh REAL BIG darn ... we couldnt find it!
		// Now we have some locked memory!

		i_free.erase(fiter);
		fiter = i_free.end();
		fprintf(stderr, "WARNING!  Locked memory at %p\n", useiter->i_loc); fflush(stderr);
		i_cluster.free(useiter);
		useiter = NULL;
		return NULL;
	    }
	}
	if (diff != 0)
	{
	    // Add a new memory block if the next is
	    iter++;
	    if (iter != i_nodes.end())
	    {
		MemoryNode *tmp = (MemoryNode *) i_cluster.malloc(sizeof(MemoryNode));

		if (tmp == NULL)
		{
		    fprintf(stderr, "Out of memory on line %d of file %s\n", __LINE__, __FILE__); fflush(stderr);
		    return NULL;
		}
		tmp->init((void *) (((char *) (*iter)->i_loc) - diff), diff);
		i_nodes.insert(iter, tmp);
		i_free.erase(fiter);

		for (fiter=i_free.begin(); fiter != i_free.end(); fiter++)
		{
		    if (fiter->size() > diff)
		    {
			i_free.insert(fiter, MemoryNodeFree(tmp));
			break;
		    }
		}
		if (fiter == i_free.end())
		    i_free.push_back(MemoryNodeFree(tmp));
		fiter = i_free.end();
	    }
	    useiter->i_size = size;
	}
	else if (useiter == NULL && size <= i_avail)
	{
	    MemoryNode *tmp = (MemoryNode *) i_cluster.malloc(sizeof(MemoryNode));
	    if (tmp == NULL)
	    {
		fprintf(stderr, "Out of memory on line %d of file %s\n", __LINE__, __FILE__); fflush(stderr);
		return NULL;
	    }
	    if (i_nodes.size())
	    {
		iter = i_nodes.end();
		iter--;
		tmp->init((void *) (((char *) (*iter)->i_loc) + (*iter)->i_size), size);
	    }
	    else
	    {
		tmp->init((void *) i_memory, size);
	    }
	    i_nodes.push_back(tmp);
	    useiter = tmp;
	    i_avail -= size;
	}
	if (useiter != NULL)
	{
	    retval = useiter->i_loc;
	    useiter->i_avail = false;
	}
	if (fiter != i_free.end())
	{
	    i_free.erase(fiter);
	}
	return retval;
    }
    void dealloc(void *loc)
    {
	if (!(loc >= i_memory && loc < i_memory + i_size))
	    return;

	N_iter iter, iterwork;
	for (iter=i_nodes.begin(); iter != i_nodes.end(); iter++)
	{
	    if ((*iter)->i_loc == loc)
		break;
	}

	// We found it in the for ...
	if (iter != i_nodes.end())
	{
	    // make it one bigger chunk, if we can!
	    // We do this by seeing if we can combine
	    // with the nodes before/after us.
	    iterwork = iter;
	    iterwork++;
	    if (iterwork != i_nodes.end() && (*iterwork)->avail())
	    {
		(*iter)->i_size += (*iterwork)->i_size;
		F_iter fiter;
		for (fiter = i_free.begin(); fiter != i_free.end(); fiter++)
		{
		    if (fiter->node() == (*iterwork))
		    {
			i_free.erase(fiter);
			break;
		    }
		}
		i_cluster.free(*iterwork);
		i_nodes.erase(iterwork);
	    }
	    if (iter != i_nodes.begin())
	    {
		iterwork = iter;
		iterwork--;
		if ((*iterwork)->avail())
		{
		    (*iterwork)->i_size += (*iter)->i_size;
		    i_cluster.free(*iter);
		    i_nodes.erase(iter);
		    iter = iterwork;
		}
	    }

	    // If we are now the LAST node, we're free, so
	    // just free up this node and return it to this
	    // chunk's 'free' pool.
	    iterwork = iter;
	    iterwork++;
	    if (iterwork == i_nodes.end())
	    {
		i_avail += (*iter)->i_size;
		F_iter fiter;
		for (fiter = i_free.begin(); fiter != i_free.end(); fiter++)
		{
		    if (fiter->node() == (*iter))
		    {
			i_free.erase(fiter);
			break;
		    }
		}
		i_cluster.free(*iter);
		i_nodes.erase(iter);
	    }
	    else
	    {
		(*iter)->i_avail = true;
		F_iter fiter;
		for (fiter=i_free.begin(); fiter != i_free.end(); fiter++)
		{
		    if (fiter->size() > (*iter)->size())
		    {
			i_free.insert(fiter, MemoryNodeFree(*iter));
			break;
		    }
		}
		if (fiter == i_free.end())
		{
		    i_free.push_back(MemoryNodeFree(*iter));
		}
	    }
	}
    }
    bool have(size_t size)
    {
	if (size <= i_avail)
	    return true;

	F_iter fiter;
	for (fiter=i_free.begin(); fiter != i_free.end(); fiter++)
	{
	    if (size <= fiter->size())
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
    list<MemoryBlock *> i_blocks;
    typedef list<MemoryBlock *>::iterator B_iter;

    size_t i_blocksize;
    char lockname[15];
    ACE_LOCK *lock;

public:
    MemoryManager()
    {
	i_blocksize = 0;
	memset(lockname, 0, 15);
	lock = NULL;
    }
    ~MemoryManager()
    {
	B_iter iter;
	for (iter=i_blocks.begin(); iter != i_blocks.end(); iter++)
	    delete *iter;
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
	if (!i_blocks.size())
	{
	    i_blocksize = blocksize;

	    MemoryBlock *tmp;
	    ACE_NEW(tmp, MemoryBlock(i_blocksize));
	    i_blocks.push_back(tmp);
	    tmp->init();
	}
    }

    void *alloc(size_t size, size_t blocksize = DEF_MEMSIZE)
    {
	void *retval = NULL;
	B_iter iter;
	if (!i_blocks.size())
	    init(blocksize);
	if (!i_blocks.size())
	{
	    fprintf(stderr, "Could not create memory blocks!\n"); fflush(stderr);
	    return NULL;
	}

	ACE_GUARD_RETURN(ACE_LOCK, lock_mon, *lock, 0);
	if (size > i_blocksize)
	{
	    MemoryBlock *tmp;
	    ACE_NEW_RETURN(tmp, MemoryBlock(size), 0);
	    i_blocks.push_back(tmp);
	    tmp->init();
	    iter = i_blocks.end();
	    iter--;
	}
	else
	{
	    B_iter iter;
	    for (iter=i_blocks.begin(); iter != i_blocks.end(); iter++)
	    {
		if ((*iter)->have(size))
		    break;
	    }
	    if (iter == i_blocks.end())
	    {
		MemoryBlock *tmp;
		ACE_NEW_RETURN(tmp, MemoryBlock(i_blocksize), 0);
		i_blocks.push_back(tmp);
		tmp->init();
		iter = i_blocks.end();
		iter--;
	    }
	}
	retval = (*iter)->alloc(size);
	return retval;
    }
    void dealloc(void *loc)
    {
	if (!i_blocks.size())
	    return;

	ACE_GUARD(ACE_LOCK, lock_mon, *lock);
	B_iter iter;
	for (iter=i_blocks.begin(); iter != i_blocks.end(); iter++)
	{
	    if ((*iter)->ismine(loc))
		break;
	}
	if (iter != i_blocks.end())
	{
	    (*iter)->dealloc(loc);
	    if ((*iter)->avail() >= (*iter)->size())
	    {
		// NOT the only entry ...
		if (i_blocks.size() > 1)
		{
		    delete *iter;
		    i_blocks.erase(iter);
		}
	    }
	}
    }

    size_t blocksize()
	{ return i_blocksize; }
    void blocksize(size_t blocksize)
	{ i_blocksize = blocksize; }
    size_t blocks()
	{ return i_blocks.size(); }
    size_t size()
	{
	    size_t count = 0;
	    B_iter iter;
	    for (iter=i_blocks.begin(); iter != i_blocks.end(); iter++)
		count += (*iter)->size();
	    return count;
	}
};

#endif
