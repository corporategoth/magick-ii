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
#ifndef _ACE_MEMORY_H
#define _ACE_MEMORY_H
static const char *ident_ace_memory_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.3  2000/11/09 10:58:18  prez
** THINK I have it working again ... with the free list.
** Will check, still thinking of sorting free list by size.
**
** Revision 1.2  2000/10/26 07:59:52  prez
** The goddamn memory system and mstring WORK!  Well, so far ;)
**
** Revision 1.1  2000/10/18 18:46:33  prez
** Well, mstring still coredumps, but it gets past the initial loading of
** all the STATIC (or const) strings, etc -- now its coring on loading a
** file (or possibly language.h or something).  Still investigating.
**
**
*/

/* This is written by Preston Elder, and is included
 * only until it is intergrated into ACE propper
 */

template <class T, class ACE_LOCK>
class ACE_Expandable_Cached_Allocator : public ACE_New_Allocator
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
  ACE_Expandable_Cached_Allocator (size_t n_chunks);
  // Create a cached memory poll with <n_chunks> chunks
  // each with sizeof (TYPE) size.

  ~ACE_Expandable_Cached_Allocator (void);
  // clear things up.

  void* malloc (size_t);
  // get a chunk of memory from free store.

  void free (void *);
  // return a chunk of memory back to free store.

  size_t chunks() { return pool_.size(); }
  size_t used() { return (pool_.size() * n_chunks_) - free_list_.size(); }

private:
  ACE_Array_Base<char *> pool_;
  // remember how we allocate the memory in the first place so
  // we can clear things up later.
  
  size_t n_chunks_;
  // how many chunks per segment.
  
  ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<T>, ACE_LOCK> free_list_;
  // Maintain a cached memory free list.
};

template <class T, class ACE_LOCK> ACE_INLINE void
ACE_Expandable_Cached_Allocator<T, ACE_LOCK>::free (void * ptr)
{
  ACE_OS::memset(ptr, 0, sizeof(T));
  this->free_list_.add ((ACE_Cached_Mem_Pool_Node<T> *) ptr) ;
}

template <class T, class ACE_LOCK>
ACE_Expandable_Cached_Allocator<T, ACE_LOCK>::ACE_Expandable_Cached_Allocator (size_t n_chunks)
  : n_chunks_ (n_chunks),
    free_list_ (ACE_PURE_FREE_LIST),
    pool_ (1)
{
  char *temp;
  ACE_NEW (temp,
           char[n_chunks_ * sizeof (T)]);

  if (pool_.set(temp, 0) < 0)
    {
      delete [] temp;
      return;
    }

  ACE_OS::memset(temp, 0, n_chunks_ * sizeof(T));
  for (size_t c = 0;
       c < n_chunks_;
       c++)
    {
      void* placement = temp + c * sizeof(T);
      this->free_list_.add ((ACE_Cached_Mem_Pool_Node<T> *) placement);
    }
  // Put into free list using placement contructor, no real memory
  // allocation in the above <new>.
}

template <class T, class ACE_LOCK>
ACE_Expandable_Cached_Allocator<T, ACE_LOCK>::~ACE_Expandable_Cached_Allocator (void)
{
  for (size_t c = 0;
       c < pool_.size();
       c++)
    delete [] this->pool_[c];
}

template <class T, class ACE_LOCK> void *
ACE_Expandable_Cached_Allocator<T, ACE_LOCK>::malloc (size_t nbytes)
{
  // Check if size requested fits within pre-determined size.
  if (nbytes > sizeof (T))
    return NULL;

  // Heres where we expand the buffer if needed ...
  if (!this->free_list_.size())
  {
    char *temp = NULL;
    ACE_NEW_RETURN (temp,
             char[n_chunks_ * sizeof (T)],
             0);

    pool_.max_size(pool_.max_size() + 1);
    if (pool_.set(temp, pool_.size()-1) < 0)
      {
        delete [] temp;
        return NULL;
      }
    // We could not add to the map ...
    ACE_OS::memset(temp, 0, n_chunks_ * sizeof(T));

    for (size_t c = 0;
         c < n_chunks_;
         c++)
      {
        void* placement = temp + c * sizeof(T);
        this->free_list_.add ((ACE_Cached_Mem_Pool_Node<T> *) placement);
      }    
    // Put into free list using placement contructor, no real memory
    // allocation in the above <new>.
  }

  // addr() call is really not absolutely necessary because of the way
  // ACE_Cached_Mem_Pool_Node's internal structure arranged.
  return this->free_list_.remove ()->addr ();
}


template <class ACE_LOCK>
class ACE_Cached_Fixed_Allocator : public ACE_New_Allocator
{
  // = TITLE
  //   Create a cached memory poll with <n_chunks> chunks each with
  //   <n_size> size.
  //
  // = DESCRIPTION
  //   This class enables caching of dynamically allocated,
  //   fixed-sized classes.
public:
  ACE_Cached_Fixed_Allocator (size_t n_size, size_t n_chunks);

  ~ACE_Cached_Fixed_Allocator (void);
  // clear things up.

  void* malloc (size_t);
  // get a chunk of memory from free store.

  void free (void *);
  // return a chunk of memory back to free store.

  size_t size();
  // Total size (amount of nodes available)

  size_t nodesize();
  // Size of each node (same as specified)

  size_t chunksize();
  // Amount of nodes per allocated chunk

  size_t count();
  // Amount of used chunks so far

private:
  char *pool_;
  // remember how we allocate the memory in the first place so
  // we can clear things up later.

  size_t n_size_;
  // Size of each element
  
  size_t n_chunks_;
  // How many chunks per segment

  ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<void *>, ACE_LOCK> free_list_;
  // Maintain a cached memory free list.
};


template <class ACE_LOCK> ACE_INLINE void *
ACE_Cached_Fixed_Allocator<ACE_LOCK>::malloc (size_t nbytes)
{
  // Check if size requested fits within pre-determined size.
  if (nbytes > n_size_)
    return NULL;

  // addr() call is really not absolutely necessary because of the way
  // ACE_Cached_Mem_Pool_Node's internal structure arranged.
  return this->free_list_.remove ()->addr ();
}

template <class ACE_LOCK> ACE_INLINE void
ACE_Cached_Fixed_Allocator<ACE_LOCK>::free (void * ptr)
{
  ACE_OS::memset(ptr, 0, n_size_);
  this->free_list_.add ((ACE_Cached_Mem_Pool_Node<void *> *) ptr) ;
}

template <class ACE_LOCK> ACE_INLINE size_t
ACE_Cached_Fixed_Allocator<ACE_LOCK>::size ()
{
  return n_chunks_;
}

template <class ACE_LOCK> ACE_INLINE size_t
ACE_Cached_Fixed_Allocator<ACE_LOCK>::nodesize ()
{
  return n_size_;
}

template <class ACE_LOCK> ACE_INLINE size_t
ACE_Cached_Fixed_Allocator<ACE_LOCK>::chunksize ()
{
  return n_chunks_;
}

template <class ACE_LOCK> ACE_INLINE size_t
ACE_Cached_Fixed_Allocator<ACE_LOCK>::count ()
{
  return size() - free_list_.size();
}


template <class ACE_LOCK>
ACE_Cached_Fixed_Allocator<ACE_LOCK>::ACE_Cached_Fixed_Allocator (size_t n_size, size_t n_chunks)
  : n_size_ (n_size),
    n_chunks_ (n_chunks),
    free_list_ (ACE_PURE_FREE_LIST),
    pool_ (0)
{
  ACE_NEW (pool_,
           char[n_chunks_ * n_size_]);

  ACE_OS::memset(n_chunks_ * n_size_);
  for (size_t c = 0;
       c < n_chunks_;
       c++)
    {
      void* placement = pool_ + c * n_size_;
      this->free_list_.add ((ACE_Cached_Mem_Pool_Node<void *> *) placement);
    }
  // Put into free list using placement contructor, no real memory
  // allocation in the above <new>.
}

template <class ACE_LOCK>
ACE_Cached_Fixed_Allocator<ACE_LOCK>::~ACE_Cached_Fixed_Allocator (void)
{
  delete [] pool_;
}

template <class ACE_LOCK>
class ACE_Expandable_Cached_Fixed_Allocator : public ACE_New_Allocator
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
  ACE_Expandable_Cached_Fixed_Allocator (size_t n_size, size_t n_chunks);

  ~ACE_Expandable_Cached_Fixed_Allocator (void);
  // clear things up.

  void* malloc (size_t);
  // get a chunk of memory from free store.

  void free (void *);
  // return a chunk of memory back to free store.

  size_t size();
  // Total size (amount of nodes available)

  size_t nodesize();
  // Size of each node (same as specified)

  size_t chunksize();
  // Amount of nodes per allocated chunk

  size_t count();
  // Amount of used chunks so far

private:
  ACE_Array_Base<char *> pool_;
  // remember how we allocate the memory in the first place so
  // we can clear things up later.

  size_t n_size_;
  // Size of each element
  
  size_t n_chunks_;
  // How many chunks per segment

  ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<void *>, ACE_LOCK> free_list_;
  // Maintain a cached memory free list.
};


template <class ACE_LOCK> ACE_INLINE void
ACE_Expandable_Cached_Fixed_Allocator<ACE_LOCK>::free (void * ptr)
{
  ACE_OS::memset(ptr, 0, n_size_);
  this->free_list_.add ((ACE_Cached_Mem_Pool_Node<void *> *) ptr) ;
}

template <class ACE_LOCK> ACE_INLINE size_t
ACE_Expandable_Cached_Fixed_Allocator<ACE_LOCK>::size ()
{
  return pool_.size() * n_chunks_;
}

template <class ACE_LOCK> ACE_INLINE size_t
ACE_Expandable_Cached_Fixed_Allocator<ACE_LOCK>::nodesize ()
{
  return n_size_;
}

template <class ACE_LOCK> ACE_INLINE size_t
ACE_Expandable_Cached_Fixed_Allocator<ACE_LOCK>::chunksize ()
{
  return n_chunks_;
}

template <class ACE_LOCK> ACE_INLINE size_t
ACE_Expandable_Cached_Fixed_Allocator<ACE_LOCK>::count ()
{
  return size() - free_list_.size();
}


template <class ACE_LOCK>
ACE_Expandable_Cached_Fixed_Allocator<ACE_LOCK>::ACE_Expandable_Cached_Fixed_Allocator (size_t n_size, size_t n_chunks)
  : n_size_ (n_size),
    n_chunks_ (n_chunks),
    free_list_ (ACE_PURE_FREE_LIST),
    pool_ (1)
{
  char *temp;
  ACE_NEW (temp,
           char[n_chunks_ * n_size_]);

  if (pool_.set(temp, 0) < 0)
    {
      delete [] temp;
      return;
    }

  ACE_OS::memset(temp, 0, n_chunks_ * n_size_);
  for (size_t c = 0;
       c < n_chunks_;
       c++)
    {
      void* placement = temp + c * n_size_;
      this->free_list_.add ((ACE_Cached_Mem_Pool_Node<void *> *) placement);
    }
  // Put into free list using placement contructor, no real memory
  // allocation in the above <new>.
}

template <class ACE_LOCK>
ACE_Expandable_Cached_Fixed_Allocator<ACE_LOCK>::~ACE_Expandable_Cached_Fixed_Allocator (void)
{
  for (size_t c = 0;
       c < pool_.size();
       c++)
    delete [] this->pool_[c];
}

template <class ACE_LOCK> void *
ACE_Expandable_Cached_Fixed_Allocator<ACE_LOCK>::malloc (size_t nbytes)
{
  // Check if size requested fits within pre-determined size.
  if (nbytes > n_size_)
    return NULL;

  // Heres where we expand the buffer if needed ...
  if (!this->free_list_.size())
  {
    char *temp = NULL;
    ACE_NEW_RETURN (temp,
             char[n_chunks_ * n_size_],
             0);

    pool_.max_size(pool_.max_size() + 1);
    if (pool_.set(temp, pool_.size()-1) < 0)
      {
        delete [] temp;
        return NULL;
      }
    // We could not add to the map ...

    ACE_OS::memset(temp, 0, n_chunks_ * n_size_);
    for (size_t c = 0;
         c < n_chunks_;
         c++)
      {
        void* placement = temp + c * n_size_;
        this->free_list_.add ((ACE_Cached_Mem_Pool_Node<void *> *) placement);
      }    
    // Put into free list using placement contructor, no real memory
    // allocation in the above <new>.
  }

  // addr() call is really not absolutely necessary because of the way
  // ACE_Cached_Mem_Pool_Node's internal structure arranged.
  return this->free_list_.remove ()->addr ();
}

#endif
