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

template <class ACE_LOCK> class MemCluster
{
    ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<void *>, ACE_LOCK> free_list;
    vector<char *> pool;
    size_t e_size;
    size_t e_max;

    // Disallow blank construction and copying
    MemCluster();
    MemCluster(const MemCluster<ACE_LOCK> &);
    void operator=(const MemCluster<ACE_LOCK> &);
public:
    MemCluster(size_t size, size_t count) : free_list(ACE_PURE_FREE_LIST)
    {
	e_size = size;
	e_max = count;
	char *tmp = new char[e_size * e_max];
	memset(tmp, 0, sizeof(char) * e_size * e_max);
	pool.push_back(tmp);

	for (unsigned int i=0; i < e_max; i++)
	{
	    void *ptr = (void *) &tmp[i * e_size];
	    free_list.add(new (ptr) ACE_Cached_Mem_Pool_Node<void *>);
	}
    }

    ~MemCluster()
    {
	vector<char *>::iterator iter;
	for (iter=pool.begin(); iter!=pool.end(); iter++)
	    delete [] *iter;
    }
    
    void * alloc()
    {
	// If we dont have any left, add a segment
	if (!free_list.size())
	{
	    char *tmp = new char[e_size * e_max];
	    memset(tmp, 0, sizeof(char) * e_size * e_max);
	    pool.push_back(tmp);
	    for (unsigned int i=0; i < e_max; i++)
	    {
		void *ptr = (void *) &tmp[i * e_size];
		free_list.add(new (ptr) ACE_Cached_Mem_Pool_Node<void *>);
	    }
	}
	return free_list.remove ()->addr ();
    }

    void dealloc(void *ptr)
    {
	memset(ptr, 0, sizeof(char) * e_size);
	free_list.add (new (ptr) ACE_Cached_Mem_Pool_Node<void *>);
    }

    size_t size()	{ return e_max * pool.size(); }
    size_t segsize()	{ return e_max; }
    size_t segments()	{ return pool.size(); }
    size_t elesize()	{ return e_size; }
    size_t count()	{ return (size() - free_list.size()); }
};



class MemoryNode
{
    friend class MemoryBlock;

    void *i_loc;
    bool i_avail;
    size_t i_size;

public:
    MemoryNode(void *loc, size_t size)
    {
	i_loc = loc;
	i_size = size;
	i_avail = true;
    }

    bool avail() { return i_avail; }
    size_t size() { return i_size; }
};

class MemoryBlock
{
    char *i_memory;
    size_t i_size;
    size_t i_avail;
    list<MemoryNode> i_nodes;

public:
    MemoryBlock(size_t size)
    {
printf("NEW MEM BLOCK (%d)\n", size); fflush(stdout);
	i_avail = 0;
	i_size = size;
	i_memory = NULL;
    }
    void init()
    {
printf("MEM BLOCK INIT (%d)\n", i_size); fflush(stdout);
	i_avail = i_size;
	i_memory = new char[i_size];
    }
    ~MemoryBlock()
    {
printf("DELETE MEM BLOCK\n"); fflush(stdout);
	if (i_memory != NULL)
	    delete [] i_memory;
    }

    void *alloc(size_t size)
    {
	void *retval = NULL;
	list<MemoryNode>::iterator iter, useiter = i_nodes.end();
	size_t diff = 0;
	for (iter=i_nodes.begin(); iter != i_nodes.end(); iter++)
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
	    iter++;
	    if (iter != i_nodes.end())
	    {
		MemoryNode tmp((void *) (((char *) iter->i_loc) - diff),
	    				iter->i_avail + diff);
		i_nodes.insert(iter, tmp);
	    }
	}
	else if (useiter == i_nodes.end() && size <= i_avail)
	{
	    iter--;
	    // NEW memory ...
	    if (i_nodes.size())
	    {
		MemoryNode tmp((void *) (((char *) iter->i_loc) + iter->i_size),
	    				size);
		i_nodes.push_back(tmp);
	    }
	    else
	    {
		i_nodes.push_back(MemoryNode(i_memory, size));
	    }
	    useiter = i_nodes.end();
	    useiter--;
	    i_avail -= size;
	}
	if (useiter != i_nodes.end())
	{
	    retval = useiter->i_loc;
	    useiter->i_avail = false;
	}
	return retval;
    }
    void dealloc(void *loc)
    {
	list<MemoryNode>::iterator iter, iternext, iterprev;
	for (iter=i_nodes.begin(); iter!=i_nodes.end(); iter++)
	{
	    if (iter->i_loc == loc)
		break;
	}

	// We found it in the for ...
	if (iter != i_nodes.end())
	{
	    // make it one bigger chunk, if we can!
	    // We do this by seeing if we can combine
	    // with the nodes before/after us.
	    iternext = iter;
	    iternext++;
	    if (iternext != i_nodes.end())
	    {
		// We got a chunk next, is it free ...
		if (iternext->avail())
		{
		    iter->i_size += iternext->i_size;
		    i_nodes.erase(iternext);
		}
	    }
	    if (iter != i_nodes.begin())
	    {
		// We're not the first, see if we can do it again ...
		iterprev = iter;
		iterprev--;
		if (iterprev->avail())
		{
		    iterprev->i_size += iter->i_size;
		    i_nodes.erase(iter);
		    iter = iterprev;
		}
	    }

	    // If we are now the LAST node, we're free, so
	    // just free up this node and return it to this
	    // chunk's 'free' pool.
	    iternext = iter;
	    iternext++;
	    if (iternext == i_nodes.end())
	    {
		i_avail += iter->i_size;
		i_nodes.erase(iter);
	    }
	    else
	    {
		iter->i_avail = true;
	    
	    }
	}
    }
    bool have(size_t size)
    {
printf("CHECKING IF I HAVE %d (%d avail)\n", size, i_avail); fflush(stdout);
	if (size <= i_avail)
	    return true;

	list<MemoryNode>::iterator iter;
	for (iter=i_nodes.begin(); iter != i_nodes.end(); iter++)
	{
	    if (iter->avail() && size <= iter->size())
		return true;
	}
printf("NO AVAIL MEMORY CHUNKS\n"); fflush(stdout);
	return false;
    }
    bool ismine(void *loc)
    {
	list<MemoryNode>::iterator iter;
	for (iter=i_nodes.begin(); iter != i_nodes.end(); iter++)
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
    list<MemoryBlock> i_blocks;
    size_t i_blocksize;
    char lockname[15];

public:
    MemoryManager()
    {
	i_blocksize = 0;
	memset(lockname, 0, 15);
    }

    void init(size_t blocksize = DEF_MEMSIZE)
    {
	sprintf(lockname, "MM_%p", this);
	ACE_LOCK lock(lockname);
	lock.acquire();
	if (!i_blocksize)
	{
	    i_blocksize = blocksize;
printf("CREATING INITIAL BLOCK (%d)\n", i_blocksize); fflush(stdout);
	    i_blocks.push_back(MemoryBlock(i_blocksize));
printf("DEBUG 1\n"); fflush(stdout);
	    i_blocks.begin()->init();
printf("DEBUG 2\n"); fflush(stdout);
	}
	lock.release();
    }

    void *alloc(size_t size, size_t blocksize = DEF_MEMSIZE)
    {
	void *retval = NULL;
	list<MemoryBlock>::iterator iter;

printf("BLOCKSIZE: %d\n", i_blocksize); fflush(stdout);
	if (!i_blocksize)
	    init(blocksize);

	ACE_LOCK lock(lockname);
	lock.acquire();
printf("REQUEST TO ALLOCATE %d (%d)\n", size, i_blocksize); fflush(stdout);
	if (size > i_blocksize)
	{
printf("EXPANDING MEMORY (SPECIAL SIZE)\n"); fflush(stdout);
	    i_blocks.push_back(MemoryBlock(size));
	    iter = i_blocks.end();
	    iter--;
	    iter->init();
	}
	else
	{
	    for (iter=i_blocks.begin(); iter!=i_blocks.end(); iter++)
		if (iter->have(size))
		    break;
	    if (iter == i_blocks.end())
	    {
printf("EXPANDING MEMORY (NO SUITABLE MEMORY FOUND)\n"); fflush(stdout);
		i_blocks.push_back(MemoryBlock(i_blocksize));
		iter = i_blocks.end();
		iter--;
		iter->init();
	    }
	}
printf("DOING ALLOC\n"); fflush(stdout);
	retval = iter->alloc(size);
	lock.release();
	return retval;
    }
    void dealloc(void *loc)
    {
	list<MemoryBlock>::iterator iter;

	if (!i_blocksize)
	    return;

	ACE_LOCK lock(lockname);
	lock.acquire();
	for (iter=i_blocks.begin(); iter!=i_blocks.end(); iter++)
	    if (iter->ismine(loc))
		break;

	if (iter != i_blocks.end())
	{
	    iter->dealloc(loc);
	    if (i_blocks.size() > 1 && iter->avail() >= iter->size())
		i_blocks.erase(iter);
	}
	lock.release();
    }

    size_t blocksize()
	{ return i_blocksize; }
    void blocksize(size_t blocksize)
	{ i_blocksize = blocksize; }
    size_t blocks()
	{ return i_blocks.size(); }
};

#endif
