

#pragma once
#ifndef CITERATOR_H_
#define CITERATOR_H_

/*! Base class for iterators.
 */
template<class T> class CIterator
{
public:
    virtual ~CIterator() {}

    //! Set to the first location in the collection
    virtual void First() = 0;

    //! Advance to the next location in the collection
    virtual void Next() = 0;

    //! Determine if we reached the end of the collection
    //! \returns true if at end of collection
    virtual bool IsDone() = 0;

    //! Return the current item in the collection
    virtual T Current() = 0;
};

/*! Iterator smart pointer
 */
template <class T> class CIteratorPtr
{
public:
    //! Constructor
    CIteratorPtr(CIterator<T> * i) : mIter(i) {}

    //! Destructor
    ~CIteratorPtr() {delete mIter;}

    //! Overloaded -> operator, allows use to use as a pointer
    CIterator<T> *operator->() {return mIter;}

    //! Overloaded * operator, allows us to use dereferenced
    CIterator<T> &operator*() {return *mIter;}

private:
    CIteratorPtr();
    CIteratorPtr(const CIteratorPtr *);
    CIteratorPtr &operator=(CIteratorPtr &);

    //! The pointer to the actual iterator
    CIterator<T> *mIter;
};

//! General purpose null iterator
template <class T> class CNullIterator : public CIterator<T>
{
public:
    //! Set to the first location in the collection
    virtual void First() {}

    //! Advance to the next location in the collection
    virtual void Next() {}

    //! Determine if we reached the end of the collection
    //! \returns true if at end of collection
    virtual bool IsDone() {return true;}

    //! Return the current item in the collection
    virtual T Current() {return NULL;}
};


#endif /* CITERATOR_H_ */
