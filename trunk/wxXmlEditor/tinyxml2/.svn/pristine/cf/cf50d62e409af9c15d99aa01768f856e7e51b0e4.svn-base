/*
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.


2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.


Modified by Jeson Chen (jesonsara@gmail.com)
1. Rename class XML* to Ti*, avoid confilic with MSXML name.
2. Add function GetDocEncoding to get doc encoding type. (tinyxml2.h)
3. TiNode:
   add:
     TiElement* FirstChildElemWithAttr( const char* pNode, const char* pAttrName, const char* pAttrValue);
	 TiElement* ElementByPath(const char* pNodePath);
	 bool SetAttribute(const char* pNodePath, const char* pAttrName, const char* pAttrValue);

4. TiElement:
   remove private: TiAttribute* FindAttribute( const char* name );
   change public: const TiAttribute* FindAttribute( const char* name ) const
			  to: TiAttribute* FindAttribute( const char* name ) const

5. class and functions to simplify path or attrValue parse
   class StrPairLink;
   class StringLink;
   void ParseNodeName(StringLink* pLink);
   void ParsePath(const char* pNodePath, StrPairLink& linkStrPair, const char chSeparator = '/');

6. copy xml string / element and append to element end
   RaiiPtr
		class for RAII realization. 
   TiElement::CopyAndAppendXmlString
		append xml string node to element, return appended element.
   TiElement::CopyAndAppendNode
		append node peSrc to element, return appended element. peSrc remain unchanged.
   RaiiPtr TiDocument::_xmlStrings
		used to collect xml string buffer and free automatically.

all modification is marked by 'USER CODE BY Jeson Chen '
*/

#ifndef TINYXML2_INCLUDED
#define TINYXML2_INCLUDED

#if defined(ANDROID_NDK) || defined(__BORLANDC__)
#   include <ctype.h>
#   include <limits.h>
#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>
#   include <stdarg.h>
#else
#   include <cctype>
#   include <climits>
#   include <cstdio>
#   include <cstdlib>
#   include <cstring>
#   include <cstdarg>
#endif

/*
   TODO: intern strings instead of allocation.
*/
/*
	gcc:
        g++ -Wall -DDEBUG tinyxml2.cpp xmltest.cpp -o gccxmltest.exe

    Formatting, Artistic Style:
        AStyle.exe --style=1tbs --indent-switches --break-closing-brackets --indent-preprocessor tinyxml2.cpp tinyxml2.h
*/

#if defined( _DEBUG ) || defined( DEBUG ) || defined (__DEBUG__)
#   ifndef DEBUG
#       define DEBUG
#   endif
#endif

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable: 4251 4345)
#endif

#ifdef _WIN32
#   ifdef TINYXML2_EXPORT
#       define TINYXML2_LIB __declspec(dllexport)
#   elif defined(TINYXML2_IMPORT)
#       define TINYXML2_LIB __declspec(dllimport)
#   else
#       define TINYXML2_LIB
#   endif
#else
#   define TINYXML2_LIB
#endif


#if defined(DEBUG)
#   if defined(_MSC_VER)
#       define TIXMLASSERT( x )           if ( !(x)) { __debugbreak(); } //if ( !(x)) WinDebugBreak()
#   elif defined (ANDROID_NDK)
#       include <android/log.h>
#       define TIXMLASSERT( x )           if ( !(x)) { __android_log_assert( "assert", "grinliz", "ASSERT in '%s' at %d.", __FILE__, __LINE__ ); }
#   else
#       include <assert.h>
#       define TIXMLASSERT                assert
#   endif
#   else
#       define TIXMLASSERT( x )           {}
#endif


#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
// Microsoft visual studio, version 2005 and higher.
/*int _snprintf_s(
   char *buffer,
   size_t sizeOfBuffer,
   size_t count,
   const char *format [,
	  argument] ...
);*/
inline int TIXML_SNPRINTF( char* buffer, size_t size, const char* format, ... )
{
    va_list va;
    va_start( va, format );
    int result = vsnprintf_s( buffer, size, _TRUNCATE, format, va );
    va_end( va );
    return result;
}
#define TIXML_SSCANF   sscanf_s
#else
// GCC version 3 and higher
//#warning( "Using sn* functions." )
#define TIXML_SNPRINTF snprintf
#define TIXML_SSCANF   sscanf
#endif

static const int TIXML2_MAJOR_VERSION = 1;
static const int TIXML2_MINOR_VERSION = 0;
static const int TIXML2_PATCH_VERSION = 12;

namespace tinyxml2
{
class TiDocument;
class TiElement;
class TiAttribute;
class TiComment;
class TiText;
class TiDeclaration;
class TiUnknown;
class TiPrinter;


/************************************************************************/
/*                     USER CODE BY Jeson Chen BEGIN                    */
/************************************************************************/
enum TiEncoding
{
	TiEncUnknown = 0,
	TiEncUTF8,
	TiEncGB2312,
};

TiEncoding GetDocEncoding(const TiDocument& xmlDoc);


class StrPairLink;
class StringLink;
void ParseNodeName(StringLink* pLink);
void ParsePath(const char* pNodePath, StrPairLink& linkStrPair, const char chSeparator = '/');

// used for RAII, store pointer and free mem when class destructed.
// only class pointer & simple array supported. NO class array support. 
class RaiiPtr
{
public:
	RaiiPtr();
	~RaiiPtr();

	void Add(void* p);

private:
	void** pBuffer;
	int nCount;
	int nCapacity;
};

/************************************************************************/
/*                      USER CODE BY Jeson Chen END                     */
/************************************************************************/


/*
	A class that wraps strings. Normally stores the start and end
	pointers into the XML file itself, and will apply normalization
	and entity translation if actually read. Can also store (and memory
	manage) a traditional char[]
*/
class StrPair
{
public:
    enum {
        NEEDS_ENTITY_PROCESSING			= 0x01,
        NEEDS_NEWLINE_NORMALIZATION		= 0x02,
        COLLAPSE_WHITESPACE	                = 0x04,

        TEXT_ELEMENT		            	= NEEDS_ENTITY_PROCESSING | NEEDS_NEWLINE_NORMALIZATION,
        TEXT_ELEMENT_LEAVE_ENTITIES		= NEEDS_NEWLINE_NORMALIZATION,
        ATTRIBUTE_NAME		            	= 0,
        ATTRIBUTE_VALUE		            	= NEEDS_ENTITY_PROCESSING | NEEDS_NEWLINE_NORMALIZATION,
        ATTRIBUTE_VALUE_LEAVE_ENTITIES  	= NEEDS_NEWLINE_NORMALIZATION,
        COMMENT				        = NEEDS_NEWLINE_NORMALIZATION
    };

    StrPair() : _flags( 0 ), _start( 0 ), _end( 0 ) {}
    ~StrPair();

    void Set( char* start, char* end, int flags ) {
        Reset();
        _start  = start;
        _end    = end;
        _flags  = flags | NEEDS_FLUSH;
    }

    const char* GetStr();

    bool Empty() const {
        return _start == _end;
    }

    void SetInternedStr( const char* str ) {
        Reset();
        _start = const_cast<char*>(str);
    }

    void SetStr( const char* str, int flags=0 );

    char* ParseText( char* in, const char* endTag, int strFlags );
    char* ParseName( char* in );

private:
    void Reset();
    void CollapseWhitespace();

    enum {
        NEEDS_FLUSH = 0x100,
        NEEDS_DELETE = 0x200
    };

    // After parsing, if *_end != 0, it can be set to zero.
    int     _flags;
    char*   _start;
    char*   _end;
};


/*
	A dynamic array of Plain Old Data. Doesn't support constructors, etc.
	Has a small initial memory pool, so that low or no usage will not
	cause a call to new/delete
*/
template <class T, int INIT>
class DynArray
{
public:
    DynArray< T, INIT >() {
        _mem = _pool;
        _allocated = INIT;
        _size = 0;
    }

    ~DynArray() {
        if ( _mem != _pool ) {
            delete [] _mem;
        }
    }

    void Push( T t ) {
        EnsureCapacity( _size+1 );
        _mem[_size++] = t;
    }

    T* PushArr( int count ) {
        EnsureCapacity( _size+count );
        T* ret = &_mem[_size];
        _size += count;
        return ret;
    }

    T Pop() {
        return _mem[--_size];
    }

    void PopArr( int count ) {
        TIXMLASSERT( _size >= count );
        _size -= count;
    }

    bool Empty() const					{
        return _size == 0;
    }

    T& operator[](int i)				{
        TIXMLASSERT( i>= 0 && i < _size );
        return _mem[i];
    }

    const T& operator[](int i) const	{
        TIXMLASSERT( i>= 0 && i < _size );
        return _mem[i];
    }

    const T& PeekTop() const                            {
        TIXMLASSERT( _size > 0 );
        return _mem[ _size - 1];
    }

    int Size() const					{
        return _size;
    }

    int Capacity() const				{
        return _allocated;
    }

    const T* Mem() const				{
        return _mem;
    }

    T* Mem()							{
        return _mem;
    }

private:
    void EnsureCapacity( int cap ) {
        if ( cap > _allocated ) {
            int newAllocated = cap * 2;
            T* newMem = new T[newAllocated];
            memcpy( newMem, _mem, sizeof(T)*_size );	// warning: not using constructors, only works for PODs
            if ( _mem != _pool ) {
                delete [] _mem;
            }
            _mem = newMem;
            _allocated = newAllocated;
        }
    }

    T*  _mem;
    T   _pool[INIT];
    int _allocated;		// objects allocated
    int _size;			// number objects in use
};


/*
	Parent virtual class of a pool for fast allocation
	and deallocation of objects.
*/
class MemPool
{
public:
    MemPool() {}
    virtual ~MemPool() {}

    virtual int ItemSize() const = 0;
    virtual void* Alloc() = 0;
    virtual void Free( void* ) = 0;
    virtual void SetTracked() = 0;
};


/*
	Template child class to create pools of the correct type.
*/
template< int SIZE >
class MemPoolT : public MemPool
{
public:
    MemPoolT() : _root(0), _currentAllocs(0), _nAllocs(0), _maxAllocs(0), _nUntracked(0)	{}
    ~MemPoolT() {
        // Delete the blocks.
        for( int i=0; i<_blockPtrs.Size(); ++i ) {
            delete _blockPtrs[i];
        }
    }

    virtual int ItemSize() const	{
        return SIZE;
    }
    int CurrentAllocs() const		{
        return _currentAllocs;
    }

    virtual void* Alloc() {
        if ( !_root ) {
            // Need a new block.
            Block* block = new Block();
            _blockPtrs.Push( block );

            for( int i=0; i<COUNT-1; ++i ) {
                block->chunk[i].next = &block->chunk[i+1];
            }
            block->chunk[COUNT-1].next = 0;
            _root = block->chunk;
        }
        void* result = _root;
        _root = _root->next;

        ++_currentAllocs;
        if ( _currentAllocs > _maxAllocs ) {
            _maxAllocs = _currentAllocs;
        }
        _nAllocs++;
        _nUntracked++;
        return result;
    }
    virtual void Free( void* mem ) {
        if ( !mem ) {
            return;
        }
        --_currentAllocs;
        Chunk* chunk = (Chunk*)mem;
#ifdef DEBUG
        memset( chunk, 0xfe, sizeof(Chunk) );
#endif
        chunk->next = _root;
        _root = chunk;
    }
    void Trace( const char* name ) {
        printf( "Mempool %s watermark=%d [%dk] current=%d size=%d nAlloc=%d blocks=%d\n",
                name, _maxAllocs, _maxAllocs*SIZE/1024, _currentAllocs, SIZE, _nAllocs, _blockPtrs.Size() );
    }

    void SetTracked() {
        _nUntracked--;
    }

    int Untracked() const {
        return _nUntracked;
    }

	// This number is perf sensitive. 4k seems like a good tradeoff on my machine.
	// The test file is large, 170k.
	// Release:		VS2010 gcc(no opt)
	//		1k:		4000
	//		2k:		4000
	//		4k:		3900	21000
	//		16k:	5200
	//		32k:	4300
	//		64k:	4000	21000
    enum { COUNT = (4*1024)/SIZE }; // Some compilers do not accept to use COUNT in private part if COUNT is private

private:
    union Chunk {
        Chunk*  next;
        char    mem[SIZE];
    };
    struct Block {
        Chunk chunk[COUNT];
    };
    DynArray< Block*, 10 > _blockPtrs;
    Chunk* _root;

    int _currentAllocs;
    int _nAllocs;
    int _maxAllocs;
    int _nUntracked;
};



/**
	Implements the interface to the "Visitor pattern" (see the Accept() method.)
	If you call the Accept() method, it requires being passed a TiVisitor
	class to handle callbacks. For nodes that contain other nodes (Document, Element)
	you will get called with a VisitEnter/VisitExit pair. Nodes that are always leafs
	are simply called with Visit().

	If you return 'true' from a Visit method, recursive parsing will continue. If you return
	false, <b>no children of this node or its siblings</b> will be visited.

	All flavors of Visit methods have a default implementation that returns 'true' (continue
	visiting). You need to only override methods that are interesting to you.

	Generally Accept() is called on the TiDocument, although all nodes support visiting.

	You should never change the document from a callback.

	@sa TiNode::Accept()
*/
class TINYXML2_LIB TiVisitor
{
public:
    virtual ~TiVisitor() {}

    /// Visit a document.
    virtual bool VisitEnter( const TiDocument& /*doc*/ )			{
        return true;
    }
    /// Visit a document.
    virtual bool VisitExit( const TiDocument& /*doc*/ )			{
        return true;
    }

    /// Visit an element.
    virtual bool VisitEnter( const TiElement& /*element*/, const TiAttribute* /*firstAttribute*/ )	{
        return true;
    }
    /// Visit an element.
    virtual bool VisitExit( const TiElement& /*element*/ )			{
        return true;
    }

    /// Visit a declaration.
    virtual bool Visit( const TiDeclaration& /*declaration*/ )		{
        return true;
    }
    /// Visit a text node.
    virtual bool Visit( const TiText& /*text*/ )					{
        return true;
    }
    /// Visit a comment node.
    virtual bool Visit( const TiComment& /*comment*/ )				{
        return true;
    }
    /// Visit an unknown node.
    virtual bool Visit( const TiUnknown& /*unknown*/ )				{
        return true;
    }
};


/*
	Utility functionality.
*/
class TiUtil
{
public:
    // Anything in the high order range of UTF-8 is assumed to not be whitespace. This isn't
    // correct, but simple, and usually works.
    static const char* SkipWhiteSpace( const char* p )	{
        while( !IsUTF8Continuation(*p) && isspace( *reinterpret_cast<const unsigned char*>(p) ) ) {
            ++p;
        }
        return p;
    }
    static char* SkipWhiteSpace( char* p )				{
        while( !IsUTF8Continuation(*p) && isspace( *reinterpret_cast<unsigned char*>(p) ) )		{
            ++p;
        }
        return p;
    }
    static bool IsWhiteSpace( char p )					{
        return !IsUTF8Continuation(p) && isspace( static_cast<unsigned char>(p) );
    }
    
    inline static bool IsNameStartChar( unsigned char ch ) {
        return ( ( ch < 128 ) ? isalpha( ch ) : 1 )
               || ch == ':'
               || ch == '_';
    }
    
    inline static bool IsNameChar( unsigned char ch ) {
        return IsNameStartChar( ch )
               || isdigit( ch )
               || ch == '.'
               || ch == '-';
    }

    inline static bool StringEqual( const char* p, const char* q, int nChar=INT_MAX )  {
        int n = 0;
        if ( p == q ) {
            return true;
        }
        while( *p && *q && *p == *q && n<nChar ) {
            ++p;
            ++q;
            ++n;
        }
        if ( (n == nChar) || ( *p == 0 && *q == 0 ) ) {
            return true;
        }
        return false;
    }
    
    inline static int IsUTF8Continuation( const char p ) {
        return p & 0x80;
    }

    static const char* ReadBOM( const char* p, bool* hasBOM );
    // p is the starting location,
    // the UTF-8 value of the entity will be placed in value, and length filled in.
    static const char* GetCharacterRef( const char* p, char* value, int* length );
    static void ConvertUTF32ToUTF8( unsigned long input, char* output, int* length );

    // converts primitive types to strings
    static void ToStr( int v, char* buffer, int bufferSize );
    static void ToStr( unsigned v, char* buffer, int bufferSize );
    static void ToStr( bool v, char* buffer, int bufferSize );
    static void ToStr( float v, char* buffer, int bufferSize );
    static void ToStr( double v, char* buffer, int bufferSize );

    // converts strings to primitive types
    static bool	ToInt( const char* str, int* value );
    static bool ToUnsigned( const char* str, unsigned* value );
    static bool	ToBool( const char* str, bool* value );
    static bool	ToFloat( const char* str, float* value );
    static bool ToDouble( const char* str, double* value );
};


/** TiNode is a base class for every object that is in the
	XML Document Object Model (DOM), except TiAttributes.
	Nodes have siblings, a parent, and children which can
	be navigated. A node is always in a TiDocument.
	The type of a TiNode can be queried, and it can
	be cast to its more defined type.

	A TiDocument allocates memory for all its Nodes.
	When the TiDocument gets deleted, all its Nodes
	will also be deleted.

	@verbatim
	A Document can contain:	Element	(container or leaf)
							Comment (leaf)
							Unknown (leaf)
							Declaration( leaf )

	An Element can contain:	Element (container or leaf)
							Text	(leaf)
							Attributes (not on tree)
							Comment (leaf)
							Unknown (leaf)

	@endverbatim
*/
class TINYXML2_LIB TiNode
{
    friend class TiDocument;
    friend class TiElement;
public:

    /// Get the TiDocument that owns this TiNode.
    const TiDocument* GetDocument() const	{
        return _document;
    }
    /// Get the TiDocument that owns this TiNode.
    TiDocument* GetDocument()				{
        return _document;
    }

    /// Safely cast to an Element, or null.
    virtual TiElement*		ToElement()		{
        return 0;
    }
    /// Safely cast to Text, or null.
    virtual TiText*		ToText()		{
        return 0;
    }
    /// Safely cast to a Comment, or null.
    virtual TiComment*		ToComment()		{
        return 0;
    }
    /// Safely cast to a Document, or null.
    virtual TiDocument*	ToDocument()	{
        return 0;
    }
    /// Safely cast to a Declaration, or null.
    virtual TiDeclaration*	ToDeclaration()	{
        return 0;
    }
    /// Safely cast to an Unknown, or null.
    virtual TiUnknown*		ToUnknown()		{
        return 0;
    }

    virtual const TiElement*		ToElement() const		{
        return 0;
    }
    virtual const TiText*			ToText() const			{
        return 0;
    }
    virtual const TiComment*		ToComment() const		{
        return 0;
    }
    virtual const TiDocument*		ToDocument() const		{
        return 0;
    }
    virtual const TiDeclaration*	ToDeclaration() const	{
        return 0;
    }
    virtual const TiUnknown*		ToUnknown() const		{
        return 0;
    }

    /** The meaning of 'value' changes for the specific type.
    	@verbatim
    	Document:	empty
    	Element:	name of the element
    	Comment:	the comment text
    	Unknown:	the tag contents
    	Text:		the text string
    	@endverbatim
    */
    const char* Value() const;

    /** Set the Value of an XML node.
    	@sa Value()
    */
    void SetValue( const char* val, bool staticMem=false );

    /// Get the parent of this node on the DOM.
    const TiNode*	Parent() const			{
        return _parent;
    }

    TiNode* Parent()						{
        return _parent;
    }

    /// Returns true if this node has no children.
    bool NoChildren() const					{
        return !_firstChild;
    }

    /// Get the first child node, or null if none exists.
    const TiNode*  FirstChild() const		{
        return _firstChild;
    }

    TiNode*		FirstChild()			{
        return _firstChild;
    }

    /** Get the first child element, or optionally the first child
        element with the specified name.
    */
    const TiElement* FirstChildElement( const char* value=0 ) const;

    TiElement* FirstChildElement( const char* value=0 )	{
        return const_cast<TiElement*>(const_cast<const TiNode*>(this)->FirstChildElement( value ));
    }

    /// Get the last child node, or null if none exists.
    const TiNode*	LastChild() const						{
        return _lastChild;
    }

    TiNode*		LastChild()								{
        return const_cast<TiNode*>(const_cast<const TiNode*>(this)->LastChild() );
    }

    /** Get the last child element or optionally the last child
        element with the specified name.
    */
    const TiElement* LastChildElement( const char* value=0 ) const;

    TiElement* LastChildElement( const char* value=0 )	{
        return const_cast<TiElement*>(const_cast<const TiNode*>(this)->LastChildElement(value) );
    }

    /// Get the previous (left) sibling node of this node.
    const TiNode*	PreviousSibling() const					{
        return _prev;
    }

    TiNode*	PreviousSibling()							{
        return _prev;
    }

    /// Get the previous (left) sibling element of this node, with an optionally supplied name.
    const TiElement*	PreviousSiblingElement( const char* value=0 ) const ;

    TiElement*	PreviousSiblingElement( const char* value=0 ) {
        return const_cast<TiElement*>(const_cast<const TiNode*>(this)->PreviousSiblingElement( value ) );
    }

    /// Get the next (right) sibling node of this node.
    const TiNode*	NextSibling() const						{
        return _next;
    }

    TiNode*	NextSibling()								{
        return _next;
    }

    /// Get the next (right) sibling element of this node, with an optionally supplied name.
    const TiElement*	NextSiblingElement( const char* value=0 ) const;

    TiElement*	NextSiblingElement( const char* value=0 )	{
        return const_cast<TiElement*>(const_cast<const TiNode*>(this)->NextSiblingElement( value ) );
    }

    /**
    	Add a child node as the last (right) child.
		If the child node is already part of the document,
		it is moved from its old location to the new location.
		Returns the addThis argument or 0 if the node does not
		belong to the same document.
    */
    TiNode* InsertEndChild( TiNode* addThis );

    TiNode* LinkEndChild( TiNode* addThis )	{
        return InsertEndChild( addThis );
    }
    /**
    	Add a child node as the first (left) child.
		If the child node is already part of the document,
		it is moved from its old location to the new location.
		Returns the addThis argument or 0 if the node does not
		belong to the same document.
    */
    TiNode* InsertFirstChild( TiNode* addThis );
    /**
    	Add a node after the specified child node.
		If the child node is already part of the document,
		it is moved from its old location to the new location.
		Returns the addThis argument or 0 if the afterThis node
		is not a child of this node, or if the node does not
		belong to the same document.
    */
    TiNode* InsertAfterChild( TiNode* afterThis, TiNode* addThis );

    /**
    	Delete all the children of this node.
    */
    void DeleteChildren();

    /**
    	Delete a child of this node.
    */
    void DeleteChild( TiNode* node );

    /**
    	Make a copy of this node, but not its children.
    	You may pass in a Document pointer that will be
    	the owner of the new Node. If the 'document' is
    	null, then the node returned will be allocated
    	from the current Document. (this->GetDocument())

    	Note: if called on a TiDocument, this will return null.
    */
    virtual TiNode* ShallowClone( TiDocument* document ) const = 0;

    /**
    	Test if 2 nodes are the same, but don't test children.
    	The 2 nodes do not need to be in the same Document.

    	Note: if called on a TiDocument, this will return false.
    */
    virtual bool ShallowEqual( const TiNode* compare ) const = 0;

    /** Accept a hierarchical visit of the nodes in the TinyXML-2 DOM. Every node in the
    	XML tree will be conditionally visited and the host will be called back
    	via the TiVisitor interface.

    	This is essentially a SAX interface for TinyXML-2. (Note however it doesn't re-parse
    	the XML for the callbacks, so the performance of TinyXML-2 is unchanged by using this
    	interface versus any other.)

    	The interface has been based on ideas from:

    	- http://www.saxproject.org/
    	- http://c2.com/cgi/wiki?HierarchicalVisitorPattern

    	Which are both good references for "visiting".

    	An example of using Accept():
    	@verbatim
    	TiPrinter printer;
    	tinyxmlDoc.Accept( &printer );
    	const char* xmlcstr = printer.CStr();
    	@endverbatim
    */
    virtual bool Accept( TiVisitor* visitor ) const = 0;

    // internal
    virtual char* ParseDeep( char*, StrPair* );

	/************************************************************************/
	/*                     USER CODE BY Jeson Chen BEGIN                    */
	/************************************************************************/
	TiElement* FirstChildElemWithAttr( const char* pNode, const char* pAttrName, const char* pAttrValue);

	// path:   /node1[attr="value"]/node2/node3
	TiElement* ElementByPath(const char* pNodePath);

	// path:   /node1[attr="value"]/node2/node3
	bool SetAttribute(const char* pNodePath, const char* pAttrName, const char* pAttrValue);
	/************************************************************************/
	/*                      USER CODE BY Jeson Chen END                     */
	/************************************************************************/


protected:
    TiNode( TiDocument* );
    virtual ~TiNode();
    TiNode( const TiNode& );	// not supported
    TiNode& operator=( const TiNode& );	// not supported

    TiDocument*	_document;
    TiNode*		_parent;
    mutable StrPair	_value;

    TiNode*		_firstChild;
    TiNode*		_lastChild;

    TiNode*		_prev;
    TiNode*		_next;

private:
    MemPool*		_memPool;
    void Unlink( TiNode* child );
};


/** XML text.

	Note that a text node can have child element nodes, for example:
	@verbatim
	<root>This is <b>bold</b></root>
	@endverbatim

	A text node can have 2 ways to output the next. "normal" output
	and CDATA. It will default to the mode it was parsed from the XML file and
	you generally want to leave it alone, but you can change the output mode with
	SetCData() and query it with CData().
*/
class TINYXML2_LIB TiText : public TiNode
{
    friend class TiDocument;
public:
    virtual bool Accept( TiVisitor* visitor ) const;

    virtual TiText* ToText()			{
        return this;
    }
    virtual const TiText* ToText() const	{
        return this;
    }

    /// Declare whether this should be CDATA or standard text.
    void SetCData( bool isCData )			{
        _isCData = isCData;
    }
    /// Returns true if this is a CDATA text element.
    bool CData() const						{
        return _isCData;
    }

    char* ParseDeep( char*, StrPair* endTag );
    virtual TiNode* ShallowClone( TiDocument* document ) const;
    virtual bool ShallowEqual( const TiNode* compare ) const;

protected:
    TiText( TiDocument* doc )	: TiNode( doc ), _isCData( false )	{}
    virtual ~TiText()												{}
    TiText( const TiText& );	// not supported
    TiText& operator=( const TiText& );	// not supported

private:
    bool _isCData;
};


/** An XML Comment. */
class TINYXML2_LIB TiComment : public TiNode
{
    friend class TiDocument;
public:
    virtual TiComment*	ToComment()					{
        return this;
    }
    virtual const TiComment* ToComment() const		{
        return this;
    }

    virtual bool Accept( TiVisitor* visitor ) const;

    char* ParseDeep( char*, StrPair* endTag );
    virtual TiNode* ShallowClone( TiDocument* document ) const;
    virtual bool ShallowEqual( const TiNode* compare ) const;

protected:
    TiComment( TiDocument* doc );
    virtual ~TiComment();
    TiComment( const TiComment& );	// not supported
    TiComment& operator=( const TiComment& );	// not supported

private:
};


/** In correct XML the declaration is the first entry in the file.
	@verbatim
		<?xml version="1.0" standalone="yes"?>
	@endverbatim

	TinyXML-2 will happily read or write files without a declaration,
	however.

	The text of the declaration isn't interpreted. It is parsed
	and written as a string.
*/
class TINYXML2_LIB TiDeclaration : public TiNode
{
    friend class TiDocument;
public:
    virtual TiDeclaration*	ToDeclaration()					{
        return this;
    }
    virtual const TiDeclaration* ToDeclaration() const		{
        return this;
    }

    virtual bool Accept( TiVisitor* visitor ) const;

    char* ParseDeep( char*, StrPair* endTag );
    virtual TiNode* ShallowClone( TiDocument* document ) const;
    virtual bool ShallowEqual( const TiNode* compare ) const;

protected:
    TiDeclaration( TiDocument* doc );
    virtual ~TiDeclaration();
    TiDeclaration( const TiDeclaration& );	// not supported
    TiDeclaration& operator=( const TiDeclaration& );	// not supported
};


/** Any tag that TinyXML-2 doesn't recognize is saved as an
	unknown. It is a tag of text, but should not be modified.
	It will be written back to the XML, unchanged, when the file
	is saved.

	DTD tags get thrown into TiUnknowns.
*/
class TINYXML2_LIB TiUnknown : public TiNode
{
    friend class TiDocument;
public:
    virtual TiUnknown*	ToUnknown()					{
        return this;
    }
    virtual const TiUnknown* ToUnknown() const		{
        return this;
    }

    virtual bool Accept( TiVisitor* visitor ) const;

    char* ParseDeep( char*, StrPair* endTag );
    virtual TiNode* ShallowClone( TiDocument* document ) const;
    virtual bool ShallowEqual( const TiNode* compare ) const;

protected:
    TiUnknown( TiDocument* doc );
    virtual ~TiUnknown();
    TiUnknown( const TiUnknown& );	// not supported
    TiUnknown& operator=( const TiUnknown& );	// not supported
};


enum TiError {
    XML_NO_ERROR = 0,
    XML_SUCCESS = 0,

    XML_NO_ATTRIBUTE,
    XML_WRONG_ATTRIBUTE_TYPE,

    XML_ERROR_FILE_NOT_FOUND,
    XML_ERROR_FILE_COULD_NOT_BE_OPENED,
    XML_ERROR_FILE_READ_ERROR,
    XML_ERROR_ELEMENT_MISMATCH,
    XML_ERROR_PARSING_ELEMENT,
    XML_ERROR_PARSING_ATTRIBUTE,
    XML_ERROR_IDENTIFYING_TAG,
    XML_ERROR_PARSING_TEXT,
    XML_ERROR_PARSING_CDATA,
    XML_ERROR_PARSING_COMMENT,
    XML_ERROR_PARSING_DECLARATION,
    XML_ERROR_PARSING_UNKNOWN,
    XML_ERROR_EMPTY_DOCUMENT,
    XML_ERROR_MISMATCHED_ELEMENT,
    XML_ERROR_PARSING,

    XML_CAN_NOT_CONVERT_TEXT,
    XML_NO_TEXT_NODE
};


/** An attribute is a name-value pair. Elements have an arbitrary
	number of attributes, each with a unique name.

	@note The attributes are not XMLNodes. You may only query the
	Next() attribute in a list.
*/
class TINYXML2_LIB TiAttribute
{
    friend class TiElement;
public:
    /// The name of the attribute.
    const char* Name() const;

    /// The value of the attribute.
    const char* Value() const;

    /// The next attribute in the list.
    const TiAttribute* Next() const {
        return _next;
    }

    /** IntValue interprets the attribute as an integer, and returns the value.
        If the value isn't an integer, 0 will be returned. There is no error checking;
    	use QueryIntValue() if you need error checking.
    */
    int		 IntValue() const				{
        int i=0;
        QueryIntValue( &i );
        return i;
    }
    /// Query as an unsigned integer. See IntValue()
    unsigned UnsignedValue() const			{
        unsigned i=0;
        QueryUnsignedValue( &i );
        return i;
    }
    /// Query as a boolean. See IntValue()
    bool	 BoolValue() const				{
        bool b=false;
        QueryBoolValue( &b );
        return b;
    }
    /// Query as a double. See IntValue()
    double 	 DoubleValue() const			{
        double d=0;
        QueryDoubleValue( &d );
        return d;
    }
    /// Query as a float. See IntValue()
    float	 FloatValue() const				{
        float f=0;
        QueryFloatValue( &f );
        return f;
    }

    /** QueryIntValue interprets the attribute as an integer, and returns the value
    	in the provided parameter. The function will return XML_NO_ERROR on success,
    	and XML_WRONG_ATTRIBUTE_TYPE if the conversion is not successful.
    */
    TiError QueryIntValue( int* value ) const;
    /// See QueryIntValue
    TiError QueryUnsignedValue( unsigned int* value ) const;
    /// See QueryIntValue
    TiError QueryBoolValue( bool* value ) const;
    /// See QueryIntValue
    TiError QueryDoubleValue( double* value ) const;
    /// See QueryIntValue
    TiError QueryFloatValue( float* value ) const;

    /// Set the attribute to a string value.
    void SetAttribute( const char* value );
    /// Set the attribute to value.
    void SetAttribute( int value );
    /// Set the attribute to value.
    void SetAttribute( unsigned value );
    /// Set the attribute to value.
    void SetAttribute( bool value );
    /// Set the attribute to value.
    void SetAttribute( double value );
    /// Set the attribute to value.
    void SetAttribute( float value );

private:
    enum { BUF_SIZE = 200 };

    TiAttribute() : _next( 0 ), _memPool( 0 ) {}
    virtual ~TiAttribute()	{}

    TiAttribute( const TiAttribute& );	// not supported
    void operator=( const TiAttribute& );	// not supported
    void SetName( const char* name );

    char* ParseDeep( char* p, bool processEntities );

    mutable StrPair _name;
    mutable StrPair _value;
    TiAttribute*   _next;
    MemPool*        _memPool;
};


/** The element is a container class. It has a value, the element name,
	and can contain other elements, text, comments, and unknowns.
	Elements also contain an arbitrary number of attributes.
*/
class TINYXML2_LIB TiElement : public TiNode
{
    friend class TiDocument;
public:
    /// Get the name of an element (which is the Value() of the node.)
    const char* Name() const		{
        return Value();
    }
    /// Set the name of the element.
    void SetName( const char* str, bool staticMem=false )	{
        SetValue( str, staticMem );
    }

    virtual TiElement* ToElement()				{
        return this;
    }
    virtual const TiElement* ToElement() const {
        return this;
    }
    virtual bool Accept( TiVisitor* visitor ) const;

    /** Given an attribute name, Attribute() returns the value
    	for the attribute of that name, or null if none
    	exists. For example:

    	@verbatim
    	const char* value = ele->Attribute( "foo" );
    	@endverbatim

    	The 'value' parameter is normally null. However, if specified,
    	the attribute will only be returned if the 'name' and 'value'
    	match. This allow you to write code:

    	@verbatim
    	if ( ele->Attribute( "foo", "bar" ) ) callFooIsBar();
    	@endverbatim

    	rather than:
    	@verbatim
    	if ( ele->Attribute( "foo" ) ) {
    		if ( strcmp( ele->Attribute( "foo" ), "bar" ) == 0 ) callFooIsBar();
    	}
    	@endverbatim
    */
    const char* Attribute( const char* name, const char* value=0 ) const;

    /** Given an attribute name, IntAttribute() returns the value
    	of the attribute interpreted as an integer. 0 will be
    	returned if there is an error. For a method with error
    	checking, see QueryIntAttribute()
    */
    int		 IntAttribute( const char* name ) const		{
        int i=0;
        QueryIntAttribute( name, &i );
        return i;
    }
    /// See IntAttribute()
    unsigned UnsignedAttribute( const char* name ) const {
        unsigned i=0;
        QueryUnsignedAttribute( name, &i );
        return i;
    }
    /// See IntAttribute()
    bool	 BoolAttribute( const char* name ) const	{
        bool b=false;
        QueryBoolAttribute( name, &b );
        return b;
    }
    /// See IntAttribute()
    double 	 DoubleAttribute( const char* name ) const	{
        double d=0;
        QueryDoubleAttribute( name, &d );
        return d;
    }
    /// See IntAttribute()
    float	 FloatAttribute( const char* name ) const	{
        float f=0;
        QueryFloatAttribute( name, &f );
        return f;
    }

    /** Given an attribute name, QueryIntAttribute() returns
    	XML_NO_ERROR, XML_WRONG_ATTRIBUTE_TYPE if the conversion
    	can't be performed, or XML_NO_ATTRIBUTE if the attribute
    	doesn't exist. If successful, the result of the conversion
    	will be written to 'value'. If not successful, nothing will
    	be written to 'value'. This allows you to provide default
    	value:

    	@verbatim
    	int value = 10;
    	QueryIntAttribute( "foo", &value );		// if "foo" isn't found, value will still be 10
    	@endverbatim
    */
    TiError QueryIntAttribute( const char* name, int* value ) const				{
        const TiAttribute* a = FindAttribute( name );
        if ( !a ) {
            return XML_NO_ATTRIBUTE;
        }
        return a->QueryIntValue( value );
    }
    /// See QueryIntAttribute()
    TiError QueryUnsignedAttribute( const char* name, unsigned int* value ) const	{
        const TiAttribute* a = FindAttribute( name );
        if ( !a ) {
            return XML_NO_ATTRIBUTE;
        }
        return a->QueryUnsignedValue( value );
    }
    /// See QueryIntAttribute()
    TiError QueryBoolAttribute( const char* name, bool* value ) const				{
        const TiAttribute* a = FindAttribute( name );
        if ( !a ) {
            return XML_NO_ATTRIBUTE;
        }
        return a->QueryBoolValue( value );
    }
    /// See QueryIntAttribute()
    TiError QueryDoubleAttribute( const char* name, double* value ) const			{
        const TiAttribute* a = FindAttribute( name );
        if ( !a ) {
            return XML_NO_ATTRIBUTE;
        }
        return a->QueryDoubleValue( value );
    }
    /// See QueryIntAttribute()
    TiError QueryFloatAttribute( const char* name, float* value ) const			{
        const TiAttribute* a = FindAttribute( name );
        if ( !a ) {
            return XML_NO_ATTRIBUTE;
        }
        return a->QueryFloatValue( value );
    }

	
    /** Given an attribute name, QueryAttribute() returns
    	XML_NO_ERROR, XML_WRONG_ATTRIBUTE_TYPE if the conversion
    	can't be performed, or XML_NO_ATTRIBUTE if the attribute
    	doesn't exist. It is overloaded for the primitive types,
		and is a generally more convenient replacement of
		QueryIntAttribute() and related functions.
		
		If successful, the result of the conversion
    	will be written to 'value'. If not successful, nothing will
    	be written to 'value'. This allows you to provide default
    	value:

    	@verbatim
    	int value = 10;
    	QueryAttribute( "foo", &value );		// if "foo" isn't found, value will still be 10
    	@endverbatim
    */
	int QueryAttribute( const char* name, int* value ) const {
		return QueryIntAttribute( name, value );
	}

	int QueryAttribute( const char* name, unsigned int* value ) const {
		return QueryUnsignedAttribute( name, value );
	}

	int QueryAttribute( const char* name, bool* value ) const {
		return QueryBoolAttribute( name, value );
	}

	int QueryAttribute( const char* name, double* value ) const {
		return QueryDoubleAttribute( name, value );
	}

	int QueryAttribute( const char* name, float* value ) const {
		return QueryFloatAttribute( name, value );
	}

	/// Sets the named attribute to value.
    void SetAttribute( const char* name, const char* value )	{
        TiAttribute* a = FindOrCreateAttribute( name );
        a->SetAttribute( value );
    }
    /// Sets the named attribute to value.
    void SetAttribute( const char* name, int value )			{
        TiAttribute* a = FindOrCreateAttribute( name );
        a->SetAttribute( value );
    }
    /// Sets the named attribute to value.
    void SetAttribute( const char* name, unsigned value )		{
        TiAttribute* a = FindOrCreateAttribute( name );
        a->SetAttribute( value );
    }
    /// Sets the named attribute to value.
    void SetAttribute( const char* name, bool value )			{
        TiAttribute* a = FindOrCreateAttribute( name );
        a->SetAttribute( value );
    }
    /// Sets the named attribute to value.
    void SetAttribute( const char* name, double value )		{
        TiAttribute* a = FindOrCreateAttribute( name );
        a->SetAttribute( value );
    }

    /**
    	Delete an attribute.
    */
    void DeleteAttribute( const char* name );

    /// Return the first attribute in the list.
    const TiAttribute* FirstAttribute() const {
        return _rootAttribute;
    }
    /// Query a specific attribute in the list.

	/************************************************************************/
	/*                     USER CODE BY Jeson Chen BEGIN                    */
	/************************************************************************/

    // const TiAttribute* FindAttribute( const char* name ) const;
	TiAttribute* FindAttribute( const char* name ) const;

	// append xml string node to element, return appended element.
	TiElement* CopyAndAppendXmlString(const char* pXml);

	// append node peSrc to element, return appended element.
	// peSrc remain unchanged.
	TiElement* CopyAndAppendNode(const TiElement* peSrc);

	/************************************************************************/
	/*                      USER CODE BY Jeson Chen END                     */
	/************************************************************************/

    /** Convenience function for easy access to the text inside an element. Although easy
    	and concise, GetText() is limited compared to getting the TiText child
    	and accessing it directly.

    	If the first child of 'this' is a TiText, the GetText()
    	returns the character string of the Text node, else null is returned.

    	This is a convenient method for getting the text of simple contained text:
    	@verbatim
    	<foo>This is text</foo>
    		const char* str = fooElement->GetText();
    	@endverbatim

    	'str' will be a pointer to "This is text".

    	Note that this function can be misleading. If the element foo was created from
    	this XML:
    	@verbatim
    		<foo><b>This is text</b></foo>
    	@endverbatim

    	then the value of str would be null. The first child node isn't a text node, it is
    	another element. From this XML:
    	@verbatim
    		<foo>This is <b>text</b></foo>
    	@endverbatim
    	GetText() will return "This is ".
    */
    const char* GetText() const;

    /**
    	Convenience method to query the value of a child text node. This is probably best
    	shown by example. Given you have a document is this form:
    	@verbatim
    		<point>
    			<x>1</x>
    			<y>1.4</y>
    		</point>
    	@endverbatim

    	The QueryIntText() and similar functions provide a safe and easier way to get to the
    	"value" of x and y.

    	@verbatim
    		int x = 0;
    		float y = 0;	// types of x and y are contrived for example
    		const TiElement* xElement = pointElement->FirstChildElement( "x" );
    		const TiElement* yElement = pointElement->FirstChildElement( "y" );
    		xElement->QueryIntText( &x );
    		yElement->QueryFloatText( &y );
    	@endverbatim

    	@returns XML_SUCCESS (0) on success, XML_CAN_NOT_CONVERT_TEXT if the text cannot be converted
    			 to the requested type, and XML_NO_TEXT_NODE if there is no child text to query.

    */
    TiError QueryIntText( int* ival ) const;
    /// See QueryIntText()
    TiError QueryUnsignedText( unsigned* uval ) const;
    /// See QueryIntText()
    TiError QueryBoolText( bool* bval ) const;
    /// See QueryIntText()
    TiError QueryDoubleText( double* dval ) const;
    /// See QueryIntText()
    TiError QueryFloatText( float* fval ) const;

    // internal:
    enum {
        OPEN,		// <foo>
        CLOSED,		// <foo/>
        CLOSING		// </foo>
    };
    int ClosingType() const {
        return _closingType;
    }
    char* ParseDeep( char* p, StrPair* endTag );
    virtual TiNode* ShallowClone( TiDocument* document ) const;
    virtual bool ShallowEqual( const TiNode* compare ) const;

private:
    TiElement( TiDocument* doc );
    virtual ~TiElement();
    TiElement( const TiElement& );	// not supported
    void operator=( const TiElement& );	// not supported

	/************************************************************************/
	/*                     USER CODE BY Jeson Chen BEGIN                    */
	/************************************************************************/
 //   TiAttribute* FindAttribute( const char* name );
	/************************************************************************/
	/*                      USER CODE BY Jeson Chen END                     */
	/************************************************************************/

    TiAttribute* FindOrCreateAttribute( const char* name );
    //void LinkAttribute( TiAttribute* attrib );
    char* ParseAttributes( char* p );

    int _closingType;
    // The attribute list is ordered; there is no 'lastAttribute'
    // because the list needs to be scanned for dupes before adding
    // a new attribute.
    TiAttribute* _rootAttribute;
};


enum Whitespace {
    PRESERVE_WHITESPACE,
    COLLAPSE_WHITESPACE
};


/** A Document binds together all the functionality.
	It can be saved, loaded, and printed to the screen.
	All Nodes are connected and allocated to a Document.
	If the Document is deleted, all its Nodes are also deleted.
*/
class TINYXML2_LIB TiDocument : public TiNode
{
    friend class TiElement;
public:
    /// constructor
    TiDocument( bool processEntities = true, Whitespace = PRESERVE_WHITESPACE );
    ~TiDocument();

    virtual TiDocument* ToDocument()				{
        return this;
    }
    virtual const TiDocument* ToDocument() const	{
        return this;
    }

    /**
    	Parse an XML file from a character string.
    	Returns XML_NO_ERROR (0) on success, or
    	an errorID.

    	You may optionally pass in the 'nBytes', which is
    	the number of bytes which will be parsed. If not
    	specified, TinyXML-2 will assume 'xml' points to a
    	null terminated string.
    */
    TiError Parse( const char* xml, size_t nBytes=(size_t)(-1) );

    /**
    	Load an XML file from disk.
    	Returns XML_NO_ERROR (0) on success, or
    	an errorID.
    */
	TiError LoadFile( const char* filename );
	TiError LoadFile( const wchar_t* filenameW );

    /**
    	Load an XML file from disk. You are responsible
    	for providing and closing the FILE*.

    	Returns XML_NO_ERROR (0) on success, or
    	an errorID.
    */
    TiError LoadFile( FILE* );

    /**
    	Save the XML file to disk.
    	Returns XML_NO_ERROR (0) on success, or
    	an errorID.
    */
    TiError SaveFile( const char* filename, bool compact = false );

    /**
    	Save the XML file to disk. You are responsible
    	for providing and closing the FILE*.

    	Returns XML_NO_ERROR (0) on success, or
    	an errorID.
    */
    TiError SaveFile( FILE* fp, bool compact = false );

    bool ProcessEntities() const		{
        return _processEntities;
    }
    Whitespace WhitespaceMode() const	{
        return _whitespace;
    }

    /**
    	Returns true if this document has a leading Byte Order Mark of UTF8.
    */
    bool HasBOM() const {
        return _writeBOM;
    }
    /** Sets whether to write the BOM when writing the file.
    */
    void SetBOM( bool useBOM ) {
        _writeBOM = useBOM;
    }

    /** Return the root element of DOM. Equivalent to FirstChildElement().
        To get the first node, use FirstChild().
    */
    TiElement* RootElement()				{
        return FirstChildElement();
    }
    const TiElement* RootElement() const	{
        return FirstChildElement();
    }

    /** Print the Document. If the Printer is not provided, it will
        print to stdout. If you provide Printer, this can print to a file:
    	@verbatim
    	TiPrinter printer( fp );
    	doc.Print( &printer );
    	@endverbatim

    	Or you can use a printer to print to memory:
    	@verbatim
    	TiPrinter printer;
    	doc.Print( &printer );
    	// printer.CStr() has a const char* to the XML
    	@endverbatim
    */
    void Print( TiPrinter* streamer=0 ) const;
    virtual bool Accept( TiVisitor* visitor ) const;

    /**
    	Create a new Element associated with
    	this Document. The memory for the Element
    	is managed by the Document.
    */
    TiElement* NewElement( const char* name );
    /**
    	Create a new Comment associated with
    	this Document. The memory for the Comment
    	is managed by the Document.
    */
    TiComment* NewComment( const char* comment );
    /**
    	Create a new Text associated with
    	this Document. The memory for the Text
    	is managed by the Document.
    */
    TiText* NewText( const char* text );
    /**
    	Create a new Declaration associated with
    	this Document. The memory for the object
    	is managed by the Document.

    	If the 'text' param is null, the standard
    	declaration is used.:
    	@verbatim
    		<?xml version="1.0" encoding="UTF-8"?>
    	@endverbatim
    */
    TiDeclaration* NewDeclaration( const char* text=0 );
    /**
    	Create a new Unknown associated with
    	this Document. The memory for the object
    	is managed by the Document.
    */
    TiUnknown* NewUnknown( const char* text );

    /**
    	Delete a node associated with this document.
    	It will be unlinked from the DOM.
    */
    void DeleteNode( TiNode* node )	{
        node->_parent->DeleteChild( node );
    }

    void SetError( TiError error, const char* str1, const char* str2 );

    /// Return true if there was an error parsing the document.
    bool Error() const {
        return _errorID != XML_NO_ERROR;
    }
    /// Return the errorID.
    TiError  ErrorID() const {
        return _errorID;
    }
    /// Return a possibly helpful diagnostic location or string.
    const char* GetErrorStr1() const {
        return _errorStr1;
    }
    /// Return a possibly helpful secondary diagnostic location or string.
    const char* GetErrorStr2() const {
        return _errorStr2;
    }
    /// If there is an error, print it to stdout.
    void PrintError() const;
    
    /// Clear the document, resetting it to the initial state.
    void Clear();

    // internal
    char* Identify( char* p, TiNode** node );

    virtual TiNode* ShallowClone( TiDocument* /*document*/ ) const	{
        return 0;
    }
    virtual bool ShallowEqual( const TiNode* /*compare*/ ) const	{
        return false;
    }

private:
    TiDocument( const TiDocument& );	// not supported
    void operator=( const TiDocument& );	// not supported

    bool        _writeBOM;
    bool        _processEntities;
    TiError    _errorID;
    Whitespace  _whitespace;
    const char* _errorStr1;
    const char* _errorStr2;
    char*       _charBuffer;

    MemPoolT< sizeof(TiElement) >	 _elementPool;
    MemPoolT< sizeof(TiAttribute) > _attributePool;
    MemPoolT< sizeof(TiText) >		 _textPool;
    MemPoolT< sizeof(TiComment) >	 _commentPool;

	/************************************************************************/
	/*                     USER CODE BY Jeson Chen BEGIN                    */
	/************************************************************************/
	// store xml string used for DeepParse.
	RaiiPtr _xmlStrings;
	/************************************************************************/
	/*                      USER CODE BY Jeson Chen END                     */
	/************************************************************************/
};


/**
	A TiHandle is a class that wraps a node pointer with null checks; this is
	an incredibly useful thing. Note that TiHandle is not part of the TinyXML-2
	DOM structure. It is a separate utility class.

	Take an example:
	@verbatim
	<Document>
		<Element attributeA = "valueA">
			<Child attributeB = "value1" />
			<Child attributeB = "value2" />
		</Element>
	</Document>
	@endverbatim

	Assuming you want the value of "attributeB" in the 2nd "Child" element, it's very
	easy to write a *lot* of code that looks like:

	@verbatim
	TiElement* root = document.FirstChildElement( "Document" );
	if ( root )
	{
		TiElement* element = root->FirstChildElement( "Element" );
		if ( element )
		{
			TiElement* child = element->FirstChildElement( "Child" );
			if ( child )
			{
				TiElement* child2 = child->NextSiblingElement( "Child" );
				if ( child2 )
				{
					// Finally do something useful.
	@endverbatim

	And that doesn't even cover "else" cases. TiHandle addresses the verbosity
	of such code. A TiHandle checks for null pointers so it is perfectly safe
	and correct to use:

	@verbatim
	TiHandle docHandle( &document );
	TiElement* child2 = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).FirstChild().NextSibling().ToElement();
	if ( child2 )
	{
		// do something useful
	@endverbatim

	Which is MUCH more concise and useful.

	It is also safe to copy handles - internally they are nothing more than node pointers.
	@verbatim
	TiHandle handleCopy = handle;
	@endverbatim

	See also TiConstHandle, which is the same as TiHandle, but operates on const objects.
*/
class TINYXML2_LIB TiHandle
{
public:
    /// Create a handle from any node (at any depth of the tree.) This can be a null pointer.
    TiHandle( TiNode* node )												{
        _node = node;
    }
    /// Create a handle from a node.
    TiHandle( TiNode& node )												{
        _node = &node;
    }
    /// Copy constructor
    TiHandle( const TiHandle& ref )										{
        _node = ref._node;
    }
    /// Assignment
    TiHandle& operator=( const TiHandle& ref )							{
        _node = ref._node;
        return *this;
    }

    /// Get the first child of this handle.
    TiHandle FirstChild() 													{
        return TiHandle( _node ? _node->FirstChild() : 0 );
    }
    /// Get the first child element of this handle.
    TiHandle FirstChildElement( const char* value=0 )						{
        return TiHandle( _node ? _node->FirstChildElement( value ) : 0 );
    }
    /// Get the last child of this handle.
    TiHandle LastChild()													{
        return TiHandle( _node ? _node->LastChild() : 0 );
    }
    /// Get the last child element of this handle.
    TiHandle LastChildElement( const char* _value=0 )						{
        return TiHandle( _node ? _node->LastChildElement( _value ) : 0 );
    }
    /// Get the previous sibling of this handle.
    TiHandle PreviousSibling()												{
        return TiHandle( _node ? _node->PreviousSibling() : 0 );
    }
    /// Get the previous sibling element of this handle.
    TiHandle PreviousSiblingElement( const char* _value=0 )				{
        return TiHandle( _node ? _node->PreviousSiblingElement( _value ) : 0 );
    }
    /// Get the next sibling of this handle.
    TiHandle NextSibling()													{
        return TiHandle( _node ? _node->NextSibling() : 0 );
    }
    /// Get the next sibling element of this handle.
    TiHandle NextSiblingElement( const char* _value=0 )					{
        return TiHandle( _node ? _node->NextSiblingElement( _value ) : 0 );
    }

    /// Safe cast to TiNode. This can return null.
    TiNode* ToNode()							{
        return _node;
    }
    /// Safe cast to TiElement. This can return null.
    TiElement* ToElement() 					{
        return ( ( _node && _node->ToElement() ) ? _node->ToElement() : 0 );
    }
    /// Safe cast to TiText. This can return null.
    TiText* ToText() 							{
        return ( ( _node && _node->ToText() ) ? _node->ToText() : 0 );
    }
    /// Safe cast to TiUnknown. This can return null.
    TiUnknown* ToUnknown() 					{
        return ( ( _node && _node->ToUnknown() ) ? _node->ToUnknown() : 0 );
    }
    /// Safe cast to TiDeclaration. This can return null.
    TiDeclaration* ToDeclaration() 			{
        return ( ( _node && _node->ToDeclaration() ) ? _node->ToDeclaration() : 0 );
    }

private:
    TiNode* _node;
};


/**
	A variant of the TiHandle class for working with const XMLNodes and Documents. It is the
	same in all regards, except for the 'const' qualifiers. See TiHandle for API.
*/
class TINYXML2_LIB TiConstHandle
{
public:
    TiConstHandle( const TiNode* node )											{
        _node = node;
    }
    TiConstHandle( const TiNode& node )											{
        _node = &node;
    }
    TiConstHandle( const TiConstHandle& ref )										{
        _node = ref._node;
    }

    TiConstHandle& operator=( const TiConstHandle& ref )							{
        _node = ref._node;
        return *this;
    }

    const TiConstHandle FirstChild() const											{
        return TiConstHandle( _node ? _node->FirstChild() : 0 );
    }
    const TiConstHandle FirstChildElement( const char* value=0 ) const				{
        return TiConstHandle( _node ? _node->FirstChildElement( value ) : 0 );
    }
    const TiConstHandle LastChild()	const										{
        return TiConstHandle( _node ? _node->LastChild() : 0 );
    }
    const TiConstHandle LastChildElement( const char* _value=0 ) const				{
        return TiConstHandle( _node ? _node->LastChildElement( _value ) : 0 );
    }
    const TiConstHandle PreviousSibling() const									{
        return TiConstHandle( _node ? _node->PreviousSibling() : 0 );
    }
    const TiConstHandle PreviousSiblingElement( const char* _value=0 ) const		{
        return TiConstHandle( _node ? _node->PreviousSiblingElement( _value ) : 0 );
    }
    const TiConstHandle NextSibling() const										{
        return TiConstHandle( _node ? _node->NextSibling() : 0 );
    }
    const TiConstHandle NextSiblingElement( const char* _value=0 ) const			{
        return TiConstHandle( _node ? _node->NextSiblingElement( _value ) : 0 );
    }


    const TiNode* ToNode() const				{
        return _node;
    }
    const TiElement* ToElement() const			{
        return ( ( _node && _node->ToElement() ) ? _node->ToElement() : 0 );
    }
    const TiText* ToText() const				{
        return ( ( _node && _node->ToText() ) ? _node->ToText() : 0 );
    }
    const TiUnknown* ToUnknown() const			{
        return ( ( _node && _node->ToUnknown() ) ? _node->ToUnknown() : 0 );
    }
    const TiDeclaration* ToDeclaration() const	{
        return ( ( _node && _node->ToDeclaration() ) ? _node->ToDeclaration() : 0 );
    }

private:
    const TiNode* _node;
};


/**
	Printing functionality. The TiPrinter gives you more
	options than the TiDocument::Print() method.

	It can:
	-# Print to memory.
	-# Print to a file you provide.
	-# Print XML without a TiDocument.

	Print to Memory

	@verbatim
	TiPrinter printer;
	doc.Print( &printer );
	SomeFunction( printer.CStr() );
	@endverbatim

	Print to a File

	You provide the file pointer.
	@verbatim
	TiPrinter printer( fp );
	doc.Print( &printer );
	@endverbatim

	Print without a TiDocument

	When loading, an XML parser is very useful. However, sometimes
	when saving, it just gets in the way. The code is often set up
	for streaming, and constructing the DOM is just overhead.

	The Printer supports the streaming case. The following code
	prints out a trivially simple XML file without ever creating
	an XML document.

	@verbatim
	TiPrinter printer( fp );
	printer.OpenElement( "foo" );
	printer.PushAttribute( "foo", "bar" );
	printer.CloseElement();
	@endverbatim
*/
class TINYXML2_LIB TiPrinter : public TiVisitor
{
public:
    /** Construct the printer. If the FILE* is specified,
    	this will print to the FILE. Else it will print
    	to memory, and the result is available in CStr().
    	If 'compact' is set to true, then output is created
    	with only required whitespace and newlines.
    */
    TiPrinter( FILE* file=0, bool compact = false, int depth = 0 );
    virtual ~TiPrinter()	{}

    /** If streaming, write the BOM and declaration. */
    void PushHeader( bool writeBOM, bool writeDeclaration );
    /** If streaming, start writing an element.
        The element must be closed with CloseElement()
    */
    void OpenElement( const char* name );
    /// If streaming, add an attribute to an open element.
    void PushAttribute( const char* name, const char* value );
    void PushAttribute( const char* name, int value );
    void PushAttribute( const char* name, unsigned value );
    void PushAttribute( const char* name, bool value );
    void PushAttribute( const char* name, double value );
    /// If streaming, close the Element.
    virtual void CloseElement();

    /// Add a text node.
    void PushText( const char* text, bool cdata=false );
    /// Add a text node from an integer.
    void PushText( int value );
    /// Add a text node from an unsigned.
    void PushText( unsigned value );
    /// Add a text node from a bool.
    void PushText( bool value );
    /// Add a text node from a float.
    void PushText( float value );
    /// Add a text node from a double.
    void PushText( double value );

    /// Add a comment
    void PushComment( const char* comment );

    void PushDeclaration( const char* value );
    void PushUnknown( const char* value );

    virtual bool VisitEnter( const TiDocument& /*doc*/ );
    virtual bool VisitExit( const TiDocument& /*doc*/ )			{
        return true;
    }

    virtual bool VisitEnter( const TiElement& element, const TiAttribute* attribute );
    virtual bool VisitExit( const TiElement& element );

    virtual bool Visit( const TiText& text );
    virtual bool Visit( const TiComment& comment );
    virtual bool Visit( const TiDeclaration& declaration );
    virtual bool Visit( const TiUnknown& unknown );

    /**
    	If in print to memory mode, return a pointer to
    	the XML file in memory.
    */
    const char* CStr() const {
        return _buffer.Mem();
    }
    /**
    	If in print to memory mode, return the size
    	of the XML file in memory. (Note the size returned
    	includes the terminating null.)
    */
    int CStrSize() const {
        return _buffer.Size();
    }

protected:
    void SealElement();
    bool _elementJustOpened;
    DynArray< const char*, 10 > _stack;

private:
    void PrintSpace( int depth );
    void PrintString( const char*, bool restrictedEntitySet );	// prints out, after detecting entities.
    void Print( const char* format, ... );

    bool _firstElement;
    FILE* _fp;
    int _depth;
    int _textDepth;
    bool _processEntities;
    bool _compactMode;

    enum {
        ENTITY_RANGE = 64,
        BUF_SIZE = 200
    };
    bool _entityFlag[ENTITY_RANGE];
    bool _restrictedEntityFlag[ENTITY_RANGE];

    DynArray< char, 20 > _buffer;
#ifdef _MSC_VER
    DynArray< char, 20 > _accumulator;
#endif
};


}	// tinyxml2

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif // TINYXML2_INCLUDED
