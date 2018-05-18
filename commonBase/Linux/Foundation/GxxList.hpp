/*
******************************************
Copyright (C): GOSUN
Filename : GSLIST.HPP
Author :  zouyx
Description: 
********************************************
*/

#ifndef _GS_H_GXXLIST_H_
#define _GS_H_GXXLIST_H_
#include "GSMutex.h"



template <class T>
struct StruGxxListNode
{	

	StruGxxListNode<T> *prev;
	StruGxxListNode<T> *next;	
	T data;

	StruGxxListNode(void) 
	{
		prev = this;
		next = this;
	}

	~StruGxxListNode(void)
	{
		unlink();
	}

	T &operator *()
	{
		return data;
	}
	
	bool not_link(void) const
	{
		return  (next==this && prev==this);
	}

	
	void unlink(void)
	{
		if( !not_link() )
		{
			prev->next = next;
			next->prev = prev;
			prev = this;
			next = this;
		}
	}

	void reset(void)
	{
		prev = this;
		next = this;
	}
};




template <class T>
class GxxList
{

public :
	typedef  StruGxxListNode<T> MyListNode;	
	typedef  MyListNode *(*FunPtrCallocNode)(void *pUserData);
	typedef  void (*FunPtrFreeNode)(MyListNode *pNode, void *pUserData);

	class iterator
	{
	private :
		friend class GxxList;
		MyListNode *_pCurPointer;
		MyListNode *_listHeader;
	public :
		iterator(void) : _pCurPointer(NULL), _listHeader(NULL)
		{

		}

		iterator(const iterator &dest)
		{
			*this = dest;
		}

		~iterator(void)
		{

		}

		iterator &operator=(const iterator &dest)
		{
			if( this!=&dest )
			{
				_listHeader = dest._listHeader;
				_pCurPointer = dest._pCurPointer;
			}
			return *this;
		}

		bool operator==(const iterator &dest) const
		{
			return _pCurPointer==dest._pCurPointer && _listHeader==dest._listHeader;
			
		}
		bool operator!=(const iterator &dest) const
		{
			return !(*this==dest);
		}

		MyListNode *node() const
		{
			return _pCurPointer;
		}

		 T & operator*() const
		{	
			GSAbort(_pCurPointer);
			return (_pCurPointer->data);
		}

		T * operator->() const
		{	
			GSAbort(_pCurPointer);
			return &(_pCurPointer->data);
		}

		iterator& operator++()
		{	
			GSAbort(_pCurPointer);
			_pCurPointer = _pCurPointer->next;		
			return (*this);
		}

		iterator operator++(int)
		{
			GSAbort(_pCurPointer);
			iterator _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		iterator& operator--()
		{	
			GSAbort(_pCurPointer);
			_pCurPointer = _pCurPointer->prev;		
			return (*this);
		}

		iterator operator--(int)
		{	
			GSAbort(_pCurPointer);
			iterator _Tmp = *this;
			--*this;
			return (_Tmp);
		}
	};


	class const_iterator
	{
	private :
		friend class GxxList;
		const MyListNode *_pCurPointer;
		const MyListNode *_listHeader;
	public :
		const_iterator(void) : _pCurPointer(NULL), _listHeader(NULL)
		{

		}

		const_iterator(const const_iterator &dest)
		{
			*this = dest;
		}

		~const_iterator(void)
		{

		}

		const_iterator &operator=(const const_iterator &dest)
		{
			if( this!=&dest )
			{
				_listHeader = dest._listHeader;
				_pCurPointer = dest._pCurPointer;
			}
			return *this;
		}

		bool operator==(const const_iterator &dest) const
		{
			return _pCurPointer==dest._pCurPointer && _listHeader==dest._listHeader;

		}
		bool operator!=(const const_iterator &dest) const
		{
			return !(*this==dest);
		}

		const MyListNode *node() const
		{
			return _pCurPointer;
		}

		const T & operator*() const
		{	
			//GSAbort(_pCurPointer && _pCurPointer != (const )&m_listHeader);
			return (_pCurPointer->data);
		}

		const T * operator->() const
		{	
			//GSAbort(_pCurPointer && _pCurPointer!=&m_listHeader);
			return &(_pCurPointer->data);
		}

		const_iterator& operator++()
		{	
//			GSAbort(_pCurPointer && _pCurPointer!=&m_listHeader);
			_pCurPointer = _pCurPointer->next;		
			return (*this);
		}

		const_iterator operator++(int)
		{
			//GSAbort(_pCurPointer && _pCurPointer!=&m_listHeader);
			iterator _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		const_iterator& operator--()
		{	
			//GSAbort(_pCurPointer && _pCurPointer!=&m_listHeader);
			_pCurPointer = _pCurPointer->prev;		
			return (*this);
		}

		const_iterator operator--(int)
		{	
			//GSAbort(_pCurPointer && _pCurPointer!=&m_listHeader);
			iterator _Tmp = *this;
			--*this;
			return (_Tmp);
		}
	};



private :
	MyListNode m_listHeader;
	long m_iCounts;	
	FunPtrCallocNode m_fnCalloc;
	FunPtrFreeNode m_fnFree;	
	void *m_pCallocUserData;
	typename GxxList::iterator  m_csItEnd;
	typename GxxList::const_iterator m_csConstItEnd;
public :
	GxxList(void) :
	  m_listHeader(),m_iCounts(0)
	   ,m_fnCalloc(calloc_node),m_fnFree(free_node)
	   ,m_pCallocUserData(NULL)
	{
		m_csItEnd._pCurPointer = &m_listHeader;
		m_csItEnd._listHeader = &m_listHeader;
		m_csConstItEnd._pCurPointer = &m_listHeader;
		m_csConstItEnd._listHeader = &m_listHeader;
	}	 
	GxxList( FunPtrCallocNode fnCalloc, FunPtrFreeNode fnFree, void *pUserData) :
			m_listHeader(),m_iCounts(0)
			,m_fnCalloc(fnCalloc),m_fnFree(fnFree)
			,m_pCallocUserData(pUserData)
	{
		m_csItEnd._pCurPointer = &m_listHeader;
		m_csItEnd._listHeader = &m_listHeader;
		m_csConstItEnd._pCurPointer = &m_listHeader;
		m_csConstItEnd._listHeader = &m_listHeader;
		GSAbort(fnCalloc && fnFree);
	}

	GxxList(const GxxList &dest) :
		 m_listHeader(),m_iCounts(0)
		,m_fnCalloc(calloc_node),m_fnFree(free_node)
		,m_pCallocUserData(NULL)
	{
		m_csItEnd._pCurPointer = &m_listHeader;
		m_csItEnd._listHeader = &m_listHeader;
		m_csConstItEnd._pCurPointer = &m_listHeader;
		m_csConstItEnd._listHeader = &m_listHeader;
		*this = dest;

	}

	void reset(FunPtrCallocNode fnCalloc, FunPtrFreeNode fnFree, void *pUserData)
	{
		clear();
		m_fnCalloc = fnCalloc;
		m_fnFree = fnFree;
		m_pCallocUserData = pUserData;
	}

	~GxxList(void)
	{
		clear();
	}

	long size(void) const
	{
		return m_iCounts;
	}

	bool empty(void) const
	{
		return m_listHeader.not_link();
	}

	void clear(void)
	{
		MyListNode *p, *tmp;
		p = m_listHeader.next;
		while( (p != &m_listHeader) )
		{
			tmp = p->next;
			 p->unlink();
			m_fnFree(p, m_pCallocUserData);
			p = tmp;
			m_iCounts--;
			
		}
	}
	
	//把 dest 接到结尾 
	void JoinBack(GxxList &dest)
	{
		if(dest.empty() )
		{
			return;
		}
		MyListNode *pFirst, *pLast;
		pFirst = dest.m_listHeader.next;
		pLast = dest.m_listHeader.prev;
		m_listHeader.prev->next = pFirst;
		pFirst->prev = m_listHeader.prev;
		m_listHeader.prev = pLast;
		pLast->next = &m_listHeader;		
		m_iCounts += dest.m_iCounts;
		dest.m_listHeader.reset();
		dest.m_iCounts = 0;
	}

	//把 dest 接到开头
	void JoinFront(GxxList &dest)
	{
		if(dest.empty() )
		{
			return;
		}
		MyListNode *pFirst, *pLast;
		pFirst = dest.m_listHeader.next;
		pLast = dest.m_listHeader.prev;
		m_listHeader.next->prev = pLast;
		pLast->next = m_listHeader.next;	
		m_listHeader.next = pFirst;
		pFirst->prev = &m_listHeader;
		m_iCounts += dest.m_iCounts;
		dest.m_listHeader.reset();
		dest.m_iCounts = 0;
	}

	void swap( GxxList &dest)
	{
		GSAbort(m_fnFree==dest.m_fnFree && m_fnCalloc==dest.m_fnCalloc);
		MyListNode *tmpNext=NULL, *tmpPrev=NULL;

		MyListNode &h1 = m_listHeader;
		MyListNode &h2 = dest.m_listHeader;

		
		long iTemp = m_iCounts;
		m_iCounts = dest.m_iCounts;
		dest.m_iCounts = iTemp;


		if( !h1.not_link() )
		{
			tmpNext = h1.next;
			tmpPrev = h1.prev;
		}

		if( h2.not_link() )
		{
			h2.reset();
		}
		else
		{
			h1.next = h2.next;
			h1.prev = h2.prev;
			h2.next->prev = &h1;
			h2.prev->next = &h1;
		}

		if( !tmpNext )
		{
			h2.reset();
		}
		else
		{
			h2.next = tmpNext;
			h2.prev = tmpPrev;
			tmpNext->prev = &h2;
			tmpPrev->next = &h2;
		}
	}


	void insert(MyListNode *pPrev,MyListNode *pNext, MyListNode *pNew)
	{
		pNext->prev = pNew;
		pNew->next = pNext;
		pNew->prev = pPrev;
		pPrev->next = pNew;
		m_iCounts++;
	}

	MyListNode * push_back( MyListNode *pNode)
	{
		insert(m_listHeader.prev, &m_listHeader, pNode);
		return pNode;
	}

	MyListNode * push_back( const T &data)
	{
		MyListNode *pNode = m_fnCalloc(m_pCallocUserData);
		GSAbort(pNode);
		pNode->data = data;

		insert(m_listHeader.prev, &m_listHeader, pNode);
		return pNode;
	}

	MyListNode * push_front( MyListNode *pNode)
	{
		insert( &m_listHeader, m_listHeader.next, pNode);
		return pNode;
	}

	MyListNode *push_front( const T &data)
	{
		MyListNode *pNode = m_fnCalloc(m_pCallocUserData);
		GSAbort(pNode);
		pNode->data = data;

		insert(&m_listHeader, m_listHeader.next, pNode);
		return pNode;
	}

	MyListNode *front_node(void)
	{
		if( m_listHeader.not_link() )
		{
			return NULL;
		}
		return m_listHeader.next;
	}

	MyListNode *back_node(void)
	{
		if( m_listHeader.not_link() )
		{
			return NULL;
		}
		return m_listHeader.prev;
	}

	MyListNode *pop_front_node(void)
	{
		if( m_listHeader.not_link() )
		{
			return NULL;
		}
		MyListNode *p = m_listHeader.next;
		p->unlink();
		m_iCounts--;
		return p;
	}

	MyListNode *pop_back_node(void)
	{
		if( m_listHeader.not_link() )
		{
			return NULL;
		}
		MyListNode *p = m_listHeader.prev;
		p->unlink();
		m_iCounts--;
		return p;
	}

	T &front(void)
	{
		GSAbort(!m_listHeader.not_link());
		return m_listHeader.next->data;

	}

	T &back(void){
		GSAbort(!m_listHeader.not_link());
		return m_listHeader.prev->data;

	}
	void pop_front(void)
	{
		if( !m_listHeader.not_link() )
		{
			MyListNode *p = m_listHeader.next;
			p->unlink();
			m_fnFree(p,m_pCallocUserData);
			m_iCounts--;
		}
	}

	void pop_back(void)
	{
		if( !m_listHeader.not_link() )
		{
			MyListNode *p = m_listHeader.prev;
			p->unlink();
			m_fnFree(p,m_pCallocUserData);
			m_iCounts--;
		}
	}

	void erase(MyListNode *pNode)
	{
		if( pNode && !pNode->not_link() && m_iCounts )
		{
			pNode->unlink();
			m_fnFree(pNode,m_pCallocUserData);
			m_iCounts--;
		}
	}

	void remove(MyListNode *pNode)
	{
		if( pNode && !pNode->not_link() && m_iCounts )
		{
			pNode->unlink();			
			m_iCounts--;
		}
	}

	iterator begin(void)
	{
		iterator it;
		it._listHeader = &m_listHeader;
		it._pCurPointer = m_listHeader.next;
		return it;
	}

	const_iterator begin(void) const
	{
		const_iterator it;
		it._listHeader = &m_listHeader;
		it._pCurPointer = m_listHeader.next;
		return it;
	}


	const  iterator &end(void)
	{
		return m_csItEnd;
	}

	const const_iterator &end(void) const
	{
		return m_csConstItEnd;
	}

	iterator rbegin(void)
	{
		iterator it;
		it._listHeader = &m_listHeader;
		it._pCurPointer = m_listHeader.prev;
		return it;
	}

	const_iterator rbegin(void) const
	{
		const_iterator it;
		it._listHeader = &m_listHeader;
		it._pCurPointer = &m_listHeader;
		return it;
	}

	iterator rend(void)
	{
		iterator it;
		it._listHeader = &m_listHeader;
		it._pCurPointer = &m_listHeader;
		return it;
	}

	const_iterator rend(void) const
	{
		const_iterator it;
		it._listHeader = &m_listHeader;
		it._pCurPointer = &m_listHeader;
		return it;
	}

	//返回 it 的后一个节点
	iterator erase(iterator &it)
	{
		if( it != end() )
		{
			iterator ret = it;
			++ret;
			erase(it._pCurPointer);
			it = ret;
			return ret;
		}
		GSAssert(0);
		return end();		
	}

	//返回 it 的后一个节点
	iterator remove(iterator &it)
	{
		if( it != end() )
		{
			iterator ret = it;
			++ret;
			remove(it._pCurPointer);
			it = ret;
			return ret;
		}
		GSAssert(0);
		return end();

	}


	GxxList &operator=( const GxxList &dest )
	{
		clear();
		m_fnFree = dest.m_fnFree;
		m_fnCalloc = dest.m_fnCalloc;
		m_pCallocUserData = dest.m_pCallocUserData;
		for( GxxList::const_iterator csIt = dest.begin(); 
			 csIt != dest.end(); ++csIt )
		{
			push_back( *csIt );
		}
	}
	

	static MyListNode *calloc_node(void *pUserData)
	{
		return new MyListNode();
	}
	static void free_node( MyListNode *pNode,void *pUserData )
	{
		delete pNode;
	}


	
};




template <class T>
struct GSFDT_CLASS StruGxxListInnerNode
{	

	StruGxxListInnerNode<T> *prev;
	StruGxxListInnerNode<T> *next;	
	T data;

	StruGxxListInnerNode(void) 
	{
		prev = this;
		next = this;
	}

	~StruGxxListInnerNode(void)
	{
		unlink();
	}

	T &operator *()
	{
		return data;
	}

	bool not_link(void) const
	{
		return  (next==this && prev==this);
	}


	void unlink(void)
	{
		if( !not_link() )
		{
			prev->next = next;
			next->prev = prev;
			prev = this;
			next = this;
		}
	}

	void reset(void)
	{
		prev = this;
		next = this;
	}
};




template <class T>
class GSFDT_CLASS GxxListInner
{

public :
	typedef  StruGxxListInnerNode<T> MyListNode;	
	typedef  MyListNode *(*FunPtrCallocNode)(void *pUserData);
	typedef  void (*FunPtrFreeNode)(MyListNode *pNode, void *pUserData);

	class GSFDT_CLASS iterator
	{
	private :
		friend class GxxListInner;
		MyListNode *_pCurPointer;
		MyListNode *_listHeader;
	public :
		iterator(void) : _pCurPointer(NULL), _listHeader(NULL)
		{

		}

		iterator(const iterator &dest)
		{
			*this = dest;
		}

		~iterator(void)
		{

		}

		iterator &operator=(const iterator &dest)
		{
			if( this!=&dest )
			{
				_listHeader = dest._listHeader;
				_pCurPointer = dest._pCurPointer;
			}
			return *this;
		}

		bool operator==(const iterator &dest) const
		{
			return _pCurPointer==dest._pCurPointer && _listHeader==dest._listHeader;

		}
		bool operator!=(const iterator &dest) const
		{
			return !(*this==dest);
		}

		MyListNode *node() const
		{
			return _pCurPointer;
		}

		T & operator*() const
		{	
			GSAbort(_pCurPointer);
			return (_pCurPointer->data);
		}

		T * operator->() const
		{	
			GSAbort(_pCurPointer);
			return &(_pCurPointer->data);
		}

		iterator& operator++()
		{	
			GSAbort(_pCurPointer);
			_pCurPointer = _pCurPointer->next;		
			return (*this);
		}

		iterator operator++(int)
		{
			GSAbort(_pCurPointer);
			iterator _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		iterator& operator--()
		{	
			GSAbort(_pCurPointer);
			_pCurPointer = _pCurPointer->prev;		
			return (*this);
		}

		iterator operator--(int)
		{	
			GSAbort(_pCurPointer);
			iterator _Tmp = *this;
			--*this;
			return (_Tmp);
		}
	};


	class GSFDT_CLASS const_iterator
	{
	private :
		friend class GxxListInner;
		const MyListNode *_pCurPointer;
		const MyListNode *_listHeader;
	public :
		const_iterator(void) : _pCurPointer(NULL), _listHeader(NULL)
		{

		}

		const_iterator(const const_iterator &dest)
		{
			*this = dest;
		}

		~const_iterator(void)
		{

		}

		const_iterator &operator=(const const_iterator &dest)
		{
			if( this!=&dest )
			{
				_listHeader = dest._listHeader;
				_pCurPointer = dest._pCurPointer;
			}
			return *this;
		}

		bool operator==(const const_iterator &dest) const
		{
			return _pCurPointer==dest._pCurPointer && _listHeader==dest._listHeader;

		}
		bool operator!=(const const_iterator &dest) const
		{
			return !(*this==dest);
		}

		const MyListNode *node() const
		{
			return _pCurPointer;
		}

		const T & operator*() const
		{	
//			GSAbort(_pCurPointer && _pCurPointer!=&m_listHeader);
			return (_pCurPointer->data);
		}

		const T * operator->() const
		{	
//			GSAbort(_pCurPointer && _pCurPointer!=&m_listHeader);
			return &(_pCurPointer->data);
		}

		const_iterator& operator++()
		{	
//			GSAbort(_pCurPointer && _pCurPointer!=&m_listHeader);
			_pCurPointer = _pCurPointer->next;		
			return (*this);
		}

		const_iterator operator++(int)
		{
//			GSAbort(_pCurPointer && _pCurPointer!=&m_listHeader);
			iterator _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		const_iterator& operator--()
		{	
//			GSAbort(_pCurPointer && _pCurPointer!=&m_listHeader);
			_pCurPointer = _pCurPointer->prev;		
			return (*this);
		}

		const_iterator operator--(int)
		{	
//			GSAbort(_pCurPointer && _pCurPointer!=&m_listHeader);
			iterator _Tmp = *this;
			--*this;
			return (_Tmp);
		}
	};



private :
	MyListNode m_listHeader;
	long m_iCounts;	
	FunPtrCallocNode m_fnCalloc;
	FunPtrFreeNode m_fnFree;	
	void *m_pCallocUserData;
	typename GxxListInner::iterator  m_csItEnd;
	typename GxxListInner::const_iterator m_csConstItEnd;
public :
	GxxListInner(void) :
	   m_listHeader(),m_iCounts(0)
		   ,m_fnCalloc(calloc_node),m_fnFree(free_node)
		   ,m_pCallocUserData(NULL)
	   {
		   m_csItEnd._pCurPointer = &m_listHeader;
		   m_csItEnd._listHeader = &m_listHeader;
		   m_csConstItEnd._pCurPointer = &m_listHeader;
		   m_csConstItEnd._listHeader = &m_listHeader;
	   }	 
	   GxxListInner( FunPtrCallocNode fnCalloc, FunPtrFreeNode fnFree, void *pUserData) :
	   m_listHeader(),m_iCounts(0)
		   ,m_fnCalloc(fnCalloc),m_fnFree(fnFree)
		   ,m_pCallocUserData(pUserData)
	   {
		   m_csItEnd._pCurPointer = &m_listHeader;
		   m_csItEnd._listHeader = &m_listHeader;
		   m_csConstItEnd._pCurPointer = &m_listHeader;
		   m_csConstItEnd._listHeader = &m_listHeader;
		   GSAbort(fnCalloc && fnFree);
	   }

	   GxxListInner(const GxxListInner &dest) :
	   m_listHeader(),m_iCounts(0)
		   ,m_fnCalloc(calloc_node),m_fnFree(free_node)
		   ,m_pCallocUserData(NULL)
	   {
		   m_csItEnd._pCurPointer = &m_listHeader;
		   m_csItEnd._listHeader = &m_listHeader;
		   m_csConstItEnd._pCurPointer = &m_listHeader;
		   m_csConstItEnd._listHeader = &m_listHeader;
		   *this = dest;

	   }

	   void reset(FunPtrCallocNode fnCalloc, FunPtrFreeNode fnFree, void *pUserData)
	   {
		   clear();
		   m_fnCalloc = fnCalloc;
		   m_fnFree = fnFree;
		   m_pCallocUserData = pUserData;
	   }

	   ~GxxListInner(void)
	   {
		   clear();
	   }

	   long size(void) const
	   {
		   return m_iCounts;
	   }

	   bool empty(void) const
	   {
		   return m_listHeader.not_link();
	   }

	   void clear(void)
	   {
		   MyListNode *p, *tmp;
		   p = m_listHeader.next;
		   while( (p != &m_listHeader) )
		   {
			   tmp = p->next;
			   p->unlink();
			   m_fnFree(p, m_pCallocUserData);
			   p = tmp;
			   m_iCounts--;

		   }
	   }

	   void swap( GxxListInner &dest)
	   {
		   GSAbort(m_fnFree==dest.m_fnFree && m_fnCalloc==dest.m_fnCalloc);
		   MyListNode *tmpNext=NULL, *tmpPrev=NULL;

		   MyListNode &h1 = m_listHeader;
		   MyListNode &h2 = dest.m_listHeader;


		   long iTemp = m_iCounts;
		   m_iCounts = dest.m_iCounts;
		   dest.m_iCounts = iTemp;


		   if( !h1.not_link() )
		   {
			   tmpNext = h1.next;
			   tmpPrev = h1.prev;
		   }

		   if( h2.not_link() )
		   {
			   h2.reset();
		   }
		   else
		   {
			   h1.next = h2.next;
			   h1.prev = h2.prev;
			   h2.next->prev = &h1;
			   h2.prev->next = &h1;
		   }

		   if( !tmpNext )
		   {
			   h2.reset();
		   }
		   else
		   {
			   h2.next = tmpNext;
			   h2.prev = tmpPrev;
			   tmpNext->prev = &h2;
			   tmpPrev->next = &h2;
		   }
	   }


	   void insert(MyListNode *pPrev,MyListNode *pNext, MyListNode *pNew)
	   {
		   pNext->prev = pNew;
		   pNew->next = pNext;
		   pNew->prev = pPrev;
		   pPrev->next = pNew;
		   m_iCounts++;
	   }

	   MyListNode * push_back( MyListNode *pNode)
	   {
		   insert(m_listHeader.prev, &m_listHeader, pNode);
		   return pNode;
	   }

	   MyListNode * push_back( const T &data)
	   {
		   MyListNode *pNode = m_fnCalloc(m_pCallocUserData);
		   GSAbort(pNode);
		   pNode->data = data;

		   insert(m_listHeader.prev, &m_listHeader, pNode);
		   return pNode;
	   }

	   MyListNode * push_front( MyListNode *pNode)
	   {
		   insert( &m_listHeader, m_listHeader.next, pNode);
		   return pNode;
	   }

	   MyListNode *push_front( const T &data)
	   {
		   MyListNode *pNode = m_fnCalloc(m_pCallocUserData);
		   GSAbort(pNode);
		   pNode->data = data;

		   insert(&m_listHeader, m_listHeader.next, pNode);
		   return pNode;
	   }

	   MyListNode *front_node(void)
	   {
		   if( m_listHeader.not_link() )
		   {
			   return NULL;
		   }
		   return m_listHeader.next;
	   }

	   MyListNode *back_node(void)
	   {
		   if( m_listHeader.not_link() )
		   {
			   return NULL;
		   }
		   return m_listHeader.prev;
	   }

	   MyListNode *pop_front_node(void)
	   {
		   if( m_listHeader.not_link() )
		   {
			   return NULL;
		   }
		   MyListNode *p = m_listHeader.next;
		   p->unlink();
		   m_iCounts--;
		   return p;
	   }

	   MyListNode *pop_back_node(void)
	   {
		   if( m_listHeader.not_link() )
		   {
			   return NULL;
		   }
		   MyListNode *p = m_listHeader.prev;
		   p->unlink();
		   m_iCounts--;
		   return p;
	   }

	   T &front(void)
	   {
		   GSAbort(!m_listHeader.not_link());
		   return m_listHeader.next->data;

	   }

	   T &back(void){
		   GSAbort(!m_listHeader.not_link());
		   return m_listHeader.prev->data;

	   }
	   void pop_front(void)
	   {
		   if( !m_listHeader.not_link() )
		   {
			   MyListNode *p = m_listHeader.next;
			   p->unlink();
			   m_fnFree(p,m_pCallocUserData);
			   m_iCounts--;
		   }
	   }

	   void pop_back(void)
	   {
		   if( !m_listHeader.not_link() )
		   {
			   MyListNode *p = m_listHeader.prev;
			   p->unlink();
			   m_fnFree(p,m_pCallocUserData);
			   m_iCounts--;
		   }
	   }

	   void erase(MyListNode *pNode)
	   {
		   if( pNode && !pNode->not_link() && m_iCounts )
		   {
			   pNode->unlink();
			   m_fnFree(pNode,m_pCallocUserData);
			   m_iCounts--;
		   }
	   }

	   void remove(MyListNode *pNode)
	   {
		   if( pNode && !pNode->not_link() && m_iCounts )
		   {
			   pNode->unlink();			
			   m_iCounts--;
		   }
	   }

	   iterator begin(void)
	   {
		   iterator it;
		   it._listHeader = &m_listHeader;
		   it._pCurPointer = m_listHeader.next;
		   return it;
	   }

	   const_iterator begin(void) const
	   {
		   const_iterator it;
		   it._listHeader = &m_listHeader;
		   it._pCurPointer = m_listHeader.next;
		   return it;
	   }


	   const  iterator &end(void)
	   {
		   return m_csItEnd;
	   }

	   const const_iterator &end(void) const
	   {
		   return m_csConstItEnd;
	   }

	   iterator rbegin(void)
	   {
		   iterator it;
		   it._listHeader = &m_listHeader;
		   it._pCurPointer = m_listHeader.prev;
		   return it;
	   }

	   const_iterator rbegin(void) const
	   {
		   const_iterator it;
		   it._listHeader = &m_listHeader;
		   it._pCurPointer = &m_listHeader;
		   return it;
	   }

	   iterator rend(void)
	   {
		   iterator it;
		   it._listHeader = &m_listHeader;
		   it._pCurPointer = &m_listHeader;
		   return it;
	   }

	   const_iterator rend(void) const
	   {
		   const_iterator it;
		   it._listHeader = &m_listHeader;
		   it._pCurPointer = &m_listHeader;
		   return it;
	   }

	   //返回 it 的后一个节点
	   iterator erase(iterator &it)
	   {
		   if( it != end() )
		   {
			   iterator ret = it;
			   ++ret;
			   erase(it._pCurPointer);
			   it = ret;
			   return ret;
		   }
		   GSAssert(0);
		   return end();		
	   }

	   //返回 it 的后一个节点
	   iterator remove(iterator &it)
	   {
		   if( it != end() )
		   {
			   iterator ret = it;
			   ++ret;
			   remove(it._pCurPointer);
			   it = ret;
			   return ret;
		   }
		   GSAssert(0);
		   return end();

	   }


	   GxxListInner &operator=( const GxxListInner &dest )
	   {
		   clear();
		   m_fnFree = dest.m_fnFree;
		   m_fnCalloc = dest.m_fnCalloc;
		   m_pCallocUserData = dest.m_pCallocUserData;
		   for( GxxListInner::const_iterator csIt = dest.begin(); 
			   csIt != dest.end(); ++csIt )
		   {
			   push_back( *csIt );
		   }
	   }


	   static MyListNode *calloc_node(void *pUserData)
	   {
		   return new MyListNode();
	   }
	   static void free_node( MyListNode *pNode,void *pUserData )
	   {
		   delete pNode;
	   }



};



#endif //end _GS_H_GSLIST_H_
