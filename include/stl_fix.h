
/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
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
#ifndef _STL_FIX_H
#define _STL_FIX_H
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
 * to work around some problems known in some STL's.
 */

#ifndef HAVE_MEM_FUN1
#define mem_fun1_ref mem_fun_ref
#define mem_fun1 mem_fun
#endif /* HAVE_MEM_FUN1 */

#ifndef HAVE_RETURN_VOID

// VOID version of mem_fun and mem_fun (const)

template<class T>
class BrokenFunc
{
    void (T::*pm)();
public:
	typedef T first_argument_type;
	typedef int result_type;

    BrokenFunc(void (T::*p)()) : pm(p) {}
    int operator()(const T *in) const
    {
	(in->*pm)();
	return 0;
    }
};

template<class T>
BrokenFunc<T> mem_fun_void(void (T::*pm)() const)
{
    return mem_fun_void((void (T::*)())pm);
}

template<class T>
BrokenFunc<T> mem_fun_void(void (T::*pm)())
{
    return BrokenFunc<T>(pm);
}

// VOID version of mem_fun_ref and mem_fun_ref (const)

template<class T>
class BrokenFunc_ref
{
    void (T::*pm)();
public:
	typedef T first_argument_type;
	typedef int result_type;

    BrokenFunc_ref(void (T::*p)()) : pm(p) {}
    int operator()(const T &in) const
    {
	(in.*pm)();
	return 0;
    }
};

template<class T>
BrokenFunc_ref<T> mem_fun_ref_void(void (T::*pm)() const)
{
    return mem_fun_ref_void((void (T::*)())pm);
}

template<class T>
BrokenFunc_ref<T> mem_fun_ref_void(void (T::*pm)())
{
    return BrokenFunc_ref<T>(pm);
}

#ifdef HAVE_MEM_FUN1

// VOID version of mem_fun1 and mem_fun1 (const)

template<class T, class P>
class BrokenFunc1
{
    void (T::*pm)(P);
public:
	typedef T first_argument_type;
	typedef P second_argument_type;
	typedef int result_type;

    BrokenFunc1(void (T::*p)(P)) : pm(p) {}
    int operator()(const T *in, const P &arg) const
    {
	(in->*pm)(arg);
	return 0;
    }
};

template<class R, class T, class P>
BrokenFunc1<T,P> mem_fun1_void(R (T::*pm)(P) const)
{
    return mem_fun1_void((R (T::*)(P))pm);
}

template<class R, class T, class P>
BrokenFunc1<T,P> mem_fun1_void(R (T::*pm)(P))
{
    return BrokenFunc1<T,P>(pm);
}

template<class R, class T, class P>
BrokenFunc1<T,const P> mem_fun1_void(R (T::*pm)(const P) const)
{
    return mem_fun1_void((R (T::*)(const P))pm);
}

template<class R, class T, class P>
BrokenFunc1<T,const P> mem_fun1_void(R (T::*pm)(const P))
{
    return BrokenFunc1<T,const P>(pm);
}

// VOID version of mem_fun1_ref and mem_fun1_ref (const)

template<class T, class P>
class BrokenFunc1_ref
{
    void (T::*pm)(P);
public:
	typedef T first_argument_type;
	typedef P second_argument_type;
	typedef int result_type;
	
    BrokenFunc1_ref(void (T::*p)(P)) : pm(p) {}
    int operator()(const T &in, const P &arg) const
    {
	(in.*pm)(arg);
	return 0;
    }
};

template<class R, class T, class P>
BrokenFunc1<T,P> mem_fun1_ref_void(R (T::*pm)(P) const)
{
    return mem_fun1_ref_void((R (T::*)(P))pm);
}

template<class R, class T, class P>
BrokenFunc1_ref<T,P> mem_fun1_ref_void(R (T::*pm)(P))
{
    return BrokenFunc1_ref<T,P>(pm);
}

template<class R, class T, class P>
BrokenFunc1<T,const P> mem_fun1_ref_void(R (T::*pm)(const P) const)
{
    return mem_fun1_ref_void((R (T::*)(const P))pm);
}

template<class R, class T, class P>
BrokenFunc1_ref<T,const P> mem_fun1_ref_void(R (T::*pm)(const P))
{
    return BrokenFunc1_ref<T,const P>(pm);
}

#endif /* HAVE_MEM_FUN1 */

#else /* HAVE_RETURN_VOID */

#define mem_fun_void mem_fun
#define mem_fun_ref_void mem_fun_ref
#ifdef HAVE_MEM_FUN1
#define mem_fun1_void mem_fun1
#define mem_fun1_ref_void mem_fun1_ref
#endif /* HAVE_MEM_FUN1 */

#endif /* HAVE_RETURN_VOID */

#ifndef HAVE_MEM_FUN_CONST

// Const version of mem_fun and mem_fun_ref

template<class R, class T>
mem_fun_t<R, T> mem_fun(R (T::*pm)() const)
{
    return mem_fun((R (T::*)())pm);
}

template<class R, class T>
mem_fun_ref_t<R, T> mem_fun_ref(R (T::*pm)() const)
{
    return mem_fun_ref((R (T::*)())pm);
}

#ifdef HAVE_MEM_FUN1

// Const version of mem_fun1 and mem_fun1_ref

template<class R, class T, class P>
mem_fun1_t<R, T, P> mem_fun1(R (T::*pm)(P) const)
{
    return mem_fun1((R (T::*)(P))pm);
}

template<class R, class T, class P>
mem_fun1_ref_t<R, T, P> mem_fun1_ref(R (T::*pm)(P) const)
{
    return mem_fun1_ref((R (T::*)(P))pm);
}

#endif /* HAVE_MEM_FUN1 */

#endif /* HAVE_MEM_FUN_CONST */


#endif
