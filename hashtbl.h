/*
    hashtbl.h
    Kevin Perez
    7/18/15

    The HashTable class is a hashtable that takes data stored at a key value
    calculated by a hash function (in this case a class in itself whose object
    is declared in the HashTable member data as 'typedef H HashType
    hashObject_') where H is specified in the class template parameter. The data
    and key are stored in a vector(bucketVector_) of lists(BucketType). The
    vector elements can be depicted as buckets with lists of (key, data)
    entries. The vector index is the hashvalue which must obviously must be an
    unsigned int when returned by the hash function in use. (Note: the hash
    functions can be found in the file hashclasses.h). 

HashTable member data

   typedef K KeyType: the key value of the table (fsu::String, etc).
   
   typedef D DataType: Is the data stored in table with corresponding key(int,
     etc).
   
   typedef fsu::Entry<KeyType,DataType> EntryType: basic entry type in
     tables, maps, dictionaries, and associative arrays.

Functions requiring implementation:

1. bool Insert( const KeyType& k, DataType& d)
  An Entry<K,D> object is inserted into the list if the key is not in the
   table, otherwise the data is overwritten.
  The algorithm structure:
   a) The appropriate bucket to place the pair (k,d) is found by "hashing the key
      and moding by the number of buckets." 
   b) The appropriate bucket is then to be searched sequentially for the entry
      with that key.
   c) If the key exists the data, d, is overwritten. Otherwise insert into
      Bucket container.
  Note: Insert(k,d) must satisfy unimodal semantics. Non-const iterators may be used. 
    It returns true/ false if key was in the table. The boolean advantage is not
    seen in proj8(COP4530).



HashTableIterator <K,D,H>
---------------------------
tablePtr_  "identifies which table you are iterating in."
bucketNum_ "identifies which bucket you are iterating in."
bucketItr_ "points into the bucket spedified by the data above[const iterator]."  

  * Quotations and paraphrases in this documentations have been provided by FLorida State University
    professor Dr. Lacher, 2015.
*/

#ifndef _HASHTBL_H
#define _HASHTBL_H

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>    // used by Analysis in hashtbl.cpp

#include <entry.h>  //includes <pair.h>
#include <vector.h>
#include <list.h>
#include <primes.h> //includes <bitvect.h>
#include <genalg.h> // Swap()

namespace fsu
{

  template <typename K, typename D, class H>
  class HashTable;

  template <typename K, typename D, class H>
  class HashTableIterator;

  //--------------------------------------------
  //     HashTable <K,D,H>
  //--------------------------------------------

  template <typename K, typename D, class H>
  class HashTable
  {
    friend class HashTableIterator <K,D,H>;
  public:
    typedef K                                KeyType;
    typedef D                                DataType;
    typedef fsu::Entry<K,D>                  EntryType;
    typedef fsu::List<EntryType>             BucketType;
    typedef H                                HashType;
    typedef typename BucketType::ValueType   ValueType;
    typedef HashTableIterator<K,D,H>         Iterator;
    typedef HashTableIterator<K,D,H>         ConstIterator;

    // ADT Table
    Iterator       Insert        (const K& k, const D& d);  
    bool           Remove        (const K& k);                  
    bool           Retrieve      (const K& k, D& d) const;     
    Iterator       Includes      (const K& k) const;            

    // ADT Associative Array
    D&             Get           (const K& key);
    void           Put           (const K& key, const D& data);
    D&             operator[]    (const K& key);

    // const versions of Get & []
    const D&       Get           (const K& key) const;
    const D&       operator[]    (const K& key) const;

    void           Clear         ();
    void           Rehash        (size_t numBuckets = 0);
    size_t         Size          () const;
    bool           Empty         () const;

    // Iterator       Begin         ();
    // Iterator       End           ();

    ConstIterator  Begin         () const;
    ConstIterator  End           () const;

    // first ctor uses default hash object, second uses supplied hash object
    explicit       HashTable     (size_t numBuckets = 100, bool prime = 1);
    HashTable                    (size_t numBuckets, HashType hashObject, bool prime = 1);
                   ~HashTable    ();
    HashTable                    (const HashTable<K,D,H>&);
    HashTable& operator =        (const HashTable&);

    // these are for Debugging and analysis
    void           Dump          (std::ostream& os, int c1 = 0, int c2 = 0) const;
    size_t         MaxBucketSize () const;
    void           Analysis      (std::ostream& os) const;

  private:
    // data
    size_t                 numBuckets_;
    Vector < BucketType >  bucketVector_;
    HashType               hashObject_;
    bool                   prime_;     // flag for prime number of buckets

    // private method calculates bucket index
    size_t  Index          (const KeyType& k) const;
  } ;

  //--------------------------------------------
  //     HashTableIterator <K,D,H>
  //--------------------------------------------

  // Note: This is a ConstIterator - cannot be used to modify table 

  template <typename K, typename D, class H>
  class HashTableIterator
  {
    friend class HashTable <K,D,H>;
  public:
    typedef K                                KeyType;
    typedef D                                DataType;
    typedef fsu::Entry<K,D>                  EntryType;
    typedef fsu::List<EntryType>             BucketType;
    typedef H                                HashType;
    typedef typename BucketType::ValueType   ValueType;
    typedef HashTableIterator<K,D,H>         Iterator;
    typedef HashTableIterator<K,D,H>         ConstIterator;

    HashTableIterator   ();
    HashTableIterator   (const Iterator& i);
    bool Valid          () const; 
    HashTableIterator <K,D,H>& operator =  (const Iterator& i);
    HashTableIterator <K,D,H>& operator ++ ();                    
    HashTableIterator <K,D,H>  operator ++ (int);
    // Entry <K,D>&               operator * ();
    const Entry <K,D>&         operator *  () const;
    bool                       operator == (const Iterator& i2) const;
    bool                       operator != (const Iterator& i2) const;

  protected:
    const HashTable <K,D,H> *           tablePtr_;
    size_t                              bucketNum_;
    typename BucketType::ConstIterator  bucketItr_;
    // typename BucketType::Iterator  bucketItr_;
  } ;

  //--------------------------------------------
  //     HashTable <K,D,H>
  //--------------------------------------------

  // ADT Table

  template <typename K, typename D, class H>
  HashTableIterator<K,D,H> HashTable<K,D,H>::Insert (const K& k, const D& d)
  {
    //creating pair for key and data
    EntryType entry_(k,d);
    Iterator i;
    i.tablePtr_ = this;
    //obtain hash value for appropriate bucket (vector index)
    i.bucketNum_ = this->Index(k);
    //for sake of clarity (deep copy)
    BucketType& bucket = this->bucketVector_[i.bucketNum_];
    i.bucketItr_ = bucket.Includes(entry_);
    if(i.bucketItr_ == bucket.End())
    {
     bucket.PushBack(entry_);
    }
    else
    {
      i.bucketItr_ = bucket.Insert(i.bucketItr_, entry_);
      bucket.Remove(++i.bucketItr_);
    }
    i.tablePtr_ = this;
    return i;
  }

  template <typename K, typename D, class H>
  bool HashTable<K,D,H>::Remove (const K& k)
  {
    Iterator i;
    EntryType entry(k);
    i.bucketNum_ = this->Index(k);
    BucketType& bucket = this->bucketVector_[i.bucketNum_];
    if(bucket.Remove(entry))
    {
      return 1;
    }
    return 0;
  }

  template <typename K, typename D, class H>
  bool HashTable<K,D,H>::Retrieve (const K& k, D& d) const
  {
    Iterator i;
    EntryType entry(k,d);
    i.bucketNum_ = this->Index(k);
    i.bucketItr_ = this->bucketVector_[i.bucketNum_].Begin();
    // searching through bucket for existence of entry
    i.bucketItr_ = g_find(i.bucketItr_, this->bucketVector_[i.bucketNum_].End(),entry);
    if(i.bucketItr_ != this->bucketVector_[i.bucketNum_].End())
    {
      return 1;
    }
    return 0;
  }

  template <typename K, typename D, class H>
  HashTableIterator<K,D,H> HashTable<K,D,H>::Includes (const K& k) const
  {
    Iterator i;
    EntryType entry(k);
    i.bucketNum_ = this->Index(k);
    i.bucketItr_ = this->bucketVector_[i.bucketNum_].Begin();
    i.bucketItr_ = g_find(i.bucketItr_, this->bucketVector_[i.bucketNum_].End(), entry);
    i.tablePtr_ = this;
    if(i.bucketItr_.Valid())
    {
      return i;
    }
    return this->End();
  }

  // ADT Associative Array

  template <typename K, typename D, class H>
  D& HashTable<K,D,H>::Get (const K& key)
  {
    EntryType e(key);
    size_t bn = Index(key);
    typename BucketType::Iterator i = bucketVector_[bn].Includes(e); 
    if (i == bucketVector_[bn].End())
      i = bucketVector_[bn].Insert(e);
    return (*i).data_;
  }

  template <typename K, typename D, class H>
  const D& HashTable<K,D,H>::Get (const K& key) const
  {
    Iterator i = Includes(key);
    if (i == End())
    {
      std::cerr << "** Error: const bracket operator called on non-existence key\n";
      exit (EXIT_FAILURE);
    }
    return (*i).data_;
  }

  template <typename K, typename D, class H>
  void HashTable<K,D,H>::Put (const K& key, const D& data)
  {
    // any of these works:
    Insert(key,data);
    // Get(key) = data;
    // (*this)[key] = data;
  }

  template <typename K, typename D, class H>
  D& HashTable<K,D,H>::operator[] (const K& key)
  {
    return Get(key);
  }

  template <typename K, typename D, class H>
  const D& HashTable<K,D,H>::operator[] (const K& key) const
  {
    return Get(key);
  }

  // constructors

  template <typename K, typename D, class H>
  HashTable <K,D,H>::HashTable (size_t n, bool prime)
    :  numBuckets_(n), bucketVector_(0), hashObject_(), prime_(prime)
  {
    // ensure at least 2 buckets
    if (numBuckets_ < 3)
      numBuckets_ = 2;
    // optionally convert to prime number of buckets
    if (prime_)
      numBuckets_ = fsu::PrimeBelow(numBuckets_);
    // create buckets
    bucketVector_.SetSize(numBuckets_);
  }

  template <typename K, typename D, class H>
  HashTable <K,D,H>::HashTable (size_t n, H hashObject, bool prime)
    :  numBuckets_(n), bucketVector_(0), hashObject_(hashObject), prime_(prime)
  {
    // ensure at least 2 buckets
    if (numBuckets_ < 3)
      numBuckets_ = 2;
    // optionally convert to prime number of buckets
    if (prime_)
      numBuckets_ = fsu::PrimeBelow(numBuckets_);
    // create buckets
    bucketVector_.SetSize(numBuckets_);
  }

  // copies

  template <typename K, typename D, class H>
  HashTable <K,D,H>::HashTable (const HashTable& ht)
    :  numBuckets_(ht.numBuckets_), bucketVector_(ht.bucketVector_), hashObject_(ht.hashObject_)
  {}

  template <typename K, typename D, class H>
  HashTable<K,D,H>& HashTable <K,D,H>::operator =  (const HashTable& ht)
  {
    if (this != &ht)
    {
      numBuckets_ = ht.numBuckets_;
      bucketVector_ = ht.bucketVector_;
      hashObject_ = ht.hashObject_;
    }
    return *this;
  }

  // other public methods

  template <typename K, typename D, class H>
  HashTable <K,D,H>::~HashTable ()
  {
    Clear();
  }

  template <typename K, typename D, class H>
  void HashTable<K,D,H>::Rehash (size_t nb)
  {
    if (nb == 0) nb = Size();
    HashTable<K,D,H> newTable(nb,hashObject_,prime_);
    for (size_t i = 0; i < numBuckets_; ++i)
    {
      while (!bucketVector_[i].Empty()) // pop as we go saves local space bloat
      {
        newTable.Insert(bucketVector_[i].Back().key_,bucketVector_[i].Back().data_);
        bucketVector_[i].PopBack();
      }
    }
    fsu::Swap(numBuckets_,newTable.numBuckets_);
    bucketVector_.Swap(newTable.bucketVector_);
  }

  template <typename K, typename D, class H>
  void HashTable<K,D,H>::Clear ()
  {
    for (size_t i = 0; i < numBuckets_; ++i)
      bucketVector_[i].Clear();
  }

  template <typename K, typename D, class H>
  HashTableIterator<K,D,H> HashTable<K,D,H>::Begin () const
  {
    HashTableIterator<K,D,H> i;
    i.tablePtr_ = this;
    i.bucketNum_ = 0;
    while (i.bucketNum_ < numBuckets_ && bucketVector_[i.bucketNum_].Empty())
      ++i.bucketNum_;
    // now we either have the first non-empty bucket or we've exhausted the bucket numbers
    if (i.bucketNum_ < numBuckets_)
      i.bucketItr_ = bucketVector_[i.bucketNum_].Begin();
    else
    {
      i.bucketNum_ = 0;
      i.bucketItr_ = bucketVector_[i.bucketNum_].End();
    }
    return i;
  }

  template <typename K, typename D, class H>
  HashTableIterator<K,D,H> HashTable<K,D,H>::End () const
  {
    HashTableIterator<K,D,H> i;
    i.tablePtr_ = this;
    i.bucketNum_ = numBuckets_ - 1;
    i.bucketItr_ = bucketVector_[i.bucketNum_].End();
    return i;
  }

  template <typename K, typename D, class H>
  size_t HashTable<K,D,H>::Size () const
  {
    size_t size(0);
    for (size_t i = 0; i < numBuckets_; ++i)
      size += bucketVector_[i].Size();
    return size;
  }

  template <typename K, typename D, class H>
  bool HashTable<K,D,H>::Empty () const
  {
    for (size_t i = 0; i < numBuckets_; ++i)
      if (!bucketVector_[i].Empty())
        return 0;
    return 1;
  }

  template <typename K, typename D, class H>
  void HashTable<K,D,H>::Dump (std::ostream& os, int c1, int c2) const
  {
    typename BucketType::ConstIterator i;
    for (size_t b = 0; b < numBuckets_; ++b)
    {
      os << "b[" << b << "]:";
      for (i = bucketVector_[b].Begin(); i != bucketVector_[b].End(); ++i)
        os << '\t' << std::setw(c1) << (*i).key_ << ':' << std::setw(c2) << (*i).data_;
      os << '\n';
    }
  }

  // private helper

  template <typename K, typename D, class H>
  size_t HashTable <K,D,H>::Index (const K& k) const
  {
    return hashObject_ (k) % numBuckets_;
  }

  //--------------------------------------------
  //     HashTableIterator <K,D,H>
  //--------------------------------------------

  //Constructors
  
  template <typename K, typename D, class H>
  HashTableIterator<K,D,H>::HashTableIterator () 
    :  tablePtr_(0), bucketNum_(0), bucketItr_()
  {}

  template <typename K, typename D, class H>
  HashTableIterator<K,D,H>::HashTableIterator (const Iterator& i)
    :  tablePtr_(i.tablePtr_), bucketNum_(i.bucketNum_), bucketItr_(i.bucketItr_)
  {}

  //overloaded operators
  
  template <typename K, typename D, class H>
  HashTableIterator <K,D,H>& HashTableIterator<K,D,H>::operator = (const Iterator& i)
  {
    if (this != &i)
    {
      tablePtr_  = i.tablePtr_;
      bucketNum_ = i.bucketNum_;
      bucketItr_ = i.bucketItr_;
    }
    return *this;
  }

  template <typename K, typename D, class H>
  HashTableIterator <K,D,H>& HashTableIterator<K,D,H>::operator ++ ()
  {
    Iterator& i = *this;
    typename BucketType::ConstIterator& b_itr = i.bucketItr_;
    //note: numBuckets is always one greater than the last valid bucketNum_
    size_t& currentBucket = i.bucketNum_;
    const size_t& totalBuckets = i.tablePtr_->numBuckets_;
    EntryType null_entry;  //(blank):0
    ++b_itr; // go to next bucket element
    if(*b_itr == null_entry) // if reached end of bucket
    {
      do // Search until a non-empty bucket is found/ end of table 
      {
        ++currentBucket;
        if(currentBucket < totalBuckets)
        {
          b_itr = i.tablePtr_->bucketVector_[currentBucket].Begin();
        }   
      } while(*b_itr == null_entry && currentBucket != totalBuckets);
    }           
    return i;
  }

  template <typename K, typename D, class H>
  HashTableIterator <K,D,H> HashTableIterator<K,D,H>::operator ++ (int)
  {
    HashTableIterator <K,D,H> i = *this;
    operator ++();
    return i;
  }

  /* Note: the following would be in Iterator, which would differ from ConstIterator by
     (1) the underlying bucketItr_ would not be const
     (2) adding Begin() and End() support
     (3) adding this non-const dereference

  template <typename K, typename D, class H>
  Entry<K,D>& HashTableIterator<K,D,H>::operator * () 
  {
    if (!Valid())
    {
      std::cerr << "** HashTableIterator error: invalid dereference\n";
      exit (EXIT_FAILURE);
    }
    return *bucketItr_;
  }
  */

  template <typename K, typename D, class H>
  const Entry<K,D>& HashTableIterator<K,D,H>::operator * () const
  {
    if (!Valid())
    {
      std::cerr << "** HashTableIterator error: invalid dereference\n";
      exit (EXIT_FAILURE);
    }
    return *bucketItr_;
  }

  template <typename K, typename D, class H>
  bool HashTableIterator<K,D,H>::operator == (const Iterator& i2) const
  {
    if (!Valid() && !i2.Valid())
      return 1;
    if (Valid() && !i2.Valid())
      return 0;
    if (!Valid() && i2.Valid())
      return 0;

    // now both are valid
    if (tablePtr_ != i2.tablePtr_)
      return 0;
    if (bucketNum_ != i2.bucketNum_)
      return 0;
    if (bucketItr_ != i2.bucketItr_)
      return 0;
    return 1;
  }

  template <typename K, typename D, class H>
  bool HashTableIterator<K,D,H>::operator != (const Iterator& i2) const
  {
    return !(*this == i2);
  }

  template <typename K, typename D, class H>
  bool HashTableIterator<K,D,H>::Valid () const
  {
    if (tablePtr_ == 0)
      return 0;
    if (bucketNum_ >= tablePtr_->numBuckets_)
      return 0;
    return bucketItr_ != tablePtr_->bucketVector_[bucketNum_].End();
  }

  // The stub implementations below should remain as-is.
  // They will be implemented as a project in COP4531.

  template <typename K, typename D, class H>
  size_t HashTable<K,D,H>::MaxBucketSize () const
  {   
    std::cout << " ** MxBucketSize not implemented\n";
    return 0;
  }

  template <typename K, typename D, class H>
  void HashTable<K,D,H>::Analysis (std::ostream& os) const
  {
    os << " ** Analysis not implemented\n";
  } // Analysis

} // namespace fsu

#endif
