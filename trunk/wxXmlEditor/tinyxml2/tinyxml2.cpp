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

all modification is marked by 'USER CODE BY Jeson Chen '
*/

#include "tinyxml2.h"

#include <new>		// yes, this one new style header, is in the Android SDK.
#   ifdef ANDROID_NDK
#   include <stddef.h>
#else
#   include <cstddef>
#endif

/************************************************************************/
/*                     USER CODE BY Jeson Chen BEGIN                    */
/************************************************************************/
namespace tinyxml2
{

//////////////////////////////////////////////////////////////////////////
// Sub string search
// return index when sub string first occurred; or -1 if not found.
// Very low efficient, but will not call too frequency, so still works well.
int SubIndex(const char* pSrc, const char* pSub, int nSrcLength, int nSubLength, int nStartPos)
{
	int i = nStartPos;
	int j = 0;
	while (i < nSrcLength && j < nSubLength)
	{
		if (pSrc[i] == pSub[j])
		{
			++i;
			++j;
		}
		else
		{
			i = i - j + 1;
			j = 0;
		}
	}
	return j == nSubLength ? 
				i - nSubLength :
				-1;
}

//////////////////////////////////////////////////////////////////////////
// XML declaration encoding parse function.

bool IsEncoding(const char* pEncString, const char* pEncStd)
{
	if (0 == SubIndex(pEncString, pEncStd, 
						strlen(pEncString), strlen(pEncStd), 0))
	{
		return true;
	}

	return false;
}

TiEncoding GetDocEncoding(const TiDocument& xmlDoc)
{
	TiEncoding code = TiEncUnknown;
	if (!xmlDoc.FirstChild())
	{
		return code;
	}

	const char* pDecl = xmlDoc.FirstChild()->Value();
	if (!pDecl)
	{
		return code;
	}

	int nDeclLen = strlen(pDecl);
	if (nDeclLen <= 0)
	{
		return code;
	}
	// xml version="1.0" encoding="UTF-8"
	// check header
	const char* const pszDeclHeader = "xml ";
	int nDeclHeader = strlen(pszDeclHeader);
	int nPos = 0;
	nPos = SubIndex(pDecl, pszDeclHeader, nDeclLen, nDeclHeader, 0);
	if (nPos)
	{
		return code;
	}

	// find encoding string pos (eg. utf-8, gb2312 etc.)
	char* pszEncDecl = "encoding=\"";
	int nEncDecl = strlen(pszEncDecl);
	nPos = SubIndex(pDecl, pszEncDecl, nDeclLen, nEncDecl, 0);
	if (nPos <= nDeclHeader)
	{
		return code;
	}

	const char* pEncString = pDecl + nPos + nEncDecl;
	if (IsEncoding(pEncString, "utf-8") ||
		IsEncoding(pEncString, "UTF-8"))
	{
		code = TiEncUTF8;
	}
	else if (IsEncoding(pEncString, "gb2312") ||
		IsEncoding(pEncString, "GB2312"))
	{
		code = TiEncGB2312;
	}
	return code;
}


//////////////////////////////////////////////////////////////////////////
// to simplify path or attrValue parse

class StrPairLink
{
public:
	StrPair m_Node;
	StrPairLink* m_pNext;

	// optional
	StrPair m_AttrName;
	StrPair m_AttrValue;

	StrPairLink(): m_pNext(0){};

	~StrPairLink()
	{
		delete m_pNext;
	} 

private:
	// forbid copy construct and operator=
	StrPairLink(const StrPairLink& link){};
	StrPairLink& operator=(const StrPairLink& link){return *this;};
};

// memory not managed except *next.
class StringLink
{
public:
	char* data;
	StringLink* next;

	// optional
	char* attrName;
	char* attrValue;

	StringLink(): data(0), next(0), attrName(0), attrValue(0) {};

	~StringLink()
	{
		delete next;
	}

private:
	// forbid copy construct and operator=
	StringLink(const StringLink& link){};
	StringLink& operator=(const StringLink& link){return *this;};
};

// nodeName [ attrName = "attrValue"]
// ' ', '='and '"' are optional, pLink->data will modified.
void ParseNodeName(StringLink* pLink)
{
	const char chSpace = ' ';
	const char chStart = '[';
	const char chEqual = '=';
	const char chQuotation = '"';
	const char chEnd = ']';

	char* p = pLink->data;

	// search ' ' or [
	while (*p != chSpace && 
		*p != chStart && 
		*p != NULL)
	{
		++p;
	}

	// erase ' '
	while (*p == chSpace)
	{
		*p = NULL;
		++p;
	}
	if (*p == NULL)
	{
		return;
	}

	// found [
	if (*p == chStart)
	{
		*p = NULL;
		++p;

		// bypass ' '
		while (*p == chSpace)
		{
			++p;
		}
		pLink->attrName = p;
	}
	else
	{
		return;
	}

	// search ' ' or = 
	while (*p != chSpace &&
		*p != chEqual && 
		*p != NULL)
	{
		++p;
	}

	// erase ' ' and =
	while (*p == chSpace ||
		*p == chEqual)
	{
		*p = NULL;
		++p;
	}

	// found "
	if (*p == chQuotation)
	{
		pLink->attrValue = ++p;
	}
	else
	{
		pLink->attrValue = p;
	}

	// search " or ]
	while (*p != chQuotation && 
		*p != chEnd && 
		*p != NULL)
	{
		++p;
	}

	if (*p != NULL)
	{
		*p = NULL;
	}

	if (*pLink->attrValue == NULL)
	{
		// node attr not correct
		pLink->attrName = NULL;
		pLink->attrValue = NULL;
	}
}

void ParsePath(const char* pNodePath, StrPairLink& linkStrPair, const char chSeparator/* = '/'*/)
{
	const int nPathLength = strlen(pNodePath) + 1;
	char* const pTempPath = new char[nPathLength];
	memcpy(pTempPath, pNodePath, nPathLength);
	char* p = pTempPath;
	char* pEnd = pTempPath + nPathLength;

	StringLink* pOrg = NULL;
	StringLink* pLink = NULL;

	do 
	{
		while (*p == chSeparator)
		{
			*p = NULL;
			++p;
		}
		if (*p == NULL)
		{
			break;
		}
		if (pLink)
		{
			pLink->next = new StringLink();
			pLink = pLink->next;
		}
		else
		{
			pLink = new StringLink();
			pOrg = pLink;
		}

		pLink->data = p;

		while (*p != chSeparator && *p != NULL)
		{
			++p;
		}
	} while (*p != NULL);


	pLink = pOrg;
	while (pLink)
	{
		ParseNodeName(pLink);
		pLink = pLink->next;
	}

	pLink = pOrg;
	StrPairLink* pPair = &linkStrPair;
	StrPairLink* pPairPrev = pPair;
	while (pLink)
	{
		if (strlen(pLink->data))
		{
			if (!pPair)
			{
				pPair = pPairPrev->m_pNext = new StrPairLink();
			}
			pPair->m_Node.SetStr(pLink->data);
			if (pLink->attrName && strlen(pLink->attrName) &&
				pLink->attrValue && strlen(pLink->attrValue))
			{
				pPair->m_AttrName.SetStr(pLink->attrName);
				pPair->m_AttrValue.SetStr(pLink->attrValue);
			}
			pPairPrev = pPair;
			pPair = pPairPrev->m_pNext;
		}
		pLink = pLink->next;
	}

	delete pTempPath;
	delete pOrg;
}

//////////////////////////////////////////////////////////////////////////
// RaiiPtr
// used for RAII, store pointer and free mem when class destructed.

RaiiPtr::RaiiPtr()
: pBuffer(0)
, nCount(0)
, nCapacity(0)
{
}

RaiiPtr::~RaiiPtr()
{
	for (int ii = 0; ii < nCount; ++ii)
	{
		delete []pBuffer[ii];
	}
	delete []pBuffer;
}

void RaiiPtr::Add(void* p)
{
	TIXMLASSERT(nCount <= nCapacity)
		if (nCapacity == nCount)
		{
			// realloc
			if (!nCapacity)
			{
				nCapacity = 16;
				pBuffer = new void*[nCapacity];
				memset(pBuffer, 0, sizeof(void*) * nCapacity);
			}
			else
			{
				int nNewCapacity = nCapacity * 2;
				void** pNewBuffer = new void*[nNewCapacity];
				memset(pNewBuffer, 0, sizeof(void*) * nNewCapacity);
				memcpy(pNewBuffer, pBuffer, sizeof(void*) * nCapacity);
				delete pBuffer;
				pBuffer = pNewBuffer;
				nCapacity = nNewCapacity;
			}
		}
		pBuffer[nCount] = p;
		nCount++;
}

} // namespace tinyxml2
/************************************************************************/
/*                     USER CODE BY Jeson Chen END                      */
/************************************************************************/

static const char LINE_FEED				= (char)0x0a;			// all line endings are normalized to LF
static const char LF = LINE_FEED;
static const char CARRIAGE_RETURN		= (char)0x0d;			// CR gets filtered out
static const char CR = CARRIAGE_RETURN;
static const char SINGLE_QUOTE			= '\'';
static const char DOUBLE_QUOTE			= '\"';

// Bunch of unicode info at:
//		http://www.unicode.org/faq/utf_bom.html
//	ef bb bf (Microsoft "lead bytes") - designates UTF-8

static const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
static const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
static const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;


#define DELETE_NODE( node )	{			\
        if ( node ) {						\
            MemPool* pool = node->_memPool;	\
            node->~TiNode();				\
            pool->Free( node );				\
        }									\
    }
#define DELETE_ATTRIBUTE( attrib ) {		\
        if ( attrib ) {							\
            MemPool* pool = attrib->_memPool;	\
            attrib->~TiAttribute();			\
            pool->Free( attrib );				\
        }										\
    }

namespace tinyxml2
{

struct Entity {
    const char* pattern;
    int length;
    char value;
};

static const int NUM_ENTITIES = 5;
static const Entity entities[NUM_ENTITIES] = {
    { "quot", 4,	DOUBLE_QUOTE },
    { "amp", 3,		'&'  },
    { "apos", 4,	SINGLE_QUOTE },
    { "lt",	2, 		'<'	 },
    { "gt",	2,		'>'	 }
};


StrPair::~StrPair()
{
    Reset();
}


void StrPair::Reset()
{
    if ( _flags & NEEDS_DELETE ) {
        delete [] _start;
    }
    _flags = 0;
    _start = 0;
    _end = 0;
}


void StrPair::SetStr( const char* str, int flags )
{
    Reset();
    size_t len = strlen( str );
    _start = new char[ len+1 ];
    memcpy( _start, str, len+1 );
    _end = _start + len;
    _flags = flags | NEEDS_DELETE;
}


char* StrPair::ParseText( char* p, const char* endTag, int strFlags )
{
    TIXMLASSERT( endTag && *endTag );

    char* start = p;	// fixme: hides a member
    char  endChar = *endTag;
    size_t length = strlen( endTag );

    // Inner loop of text parsing.
    while ( *p ) {
        if ( *p == endChar && strncmp( p, endTag, length ) == 0 ) {
            Set( start, p, strFlags );
            return p + length;
        }
        ++p;
    }
    return 0;
}


char* StrPair::ParseName( char* p )
{
    char* start = p;

    if ( !start || !(*start) ) {
        return 0;
    }

    while( *p && ( p == start ? TiUtil::IsNameStartChar( *p ) : TiUtil::IsNameChar( *p ) )) {
        ++p;
    }

    if ( p > start ) {
        Set( start, p, 0 );
        return p;
    }
    return 0;
}


void StrPair::CollapseWhitespace()
{
    // Trim leading space.
    _start = TiUtil::SkipWhiteSpace( _start );

    if ( _start && *_start ) {
        char* p = _start;	// the read pointer
        char* q = _start;	// the write pointer

        while( *p ) {
            if ( TiUtil::IsWhiteSpace( *p )) {
                p = TiUtil::SkipWhiteSpace( p );
                if ( *p == 0 ) {
                    break;    // don't write to q; this trims the trailing space.
                }
                *q = ' ';
                ++q;
            }
            *q = *p;
            ++q;
            ++p;
        }
        *q = 0;
    }
}


const char* StrPair::GetStr()
{
    if ( _flags & NEEDS_FLUSH ) {
        *_end = 0;
        _flags ^= NEEDS_FLUSH;

        if ( _flags ) {
            char* p = _start;	// the read pointer
            char* q = _start;	// the write pointer

            while( p < _end ) {
                if ( (_flags & NEEDS_NEWLINE_NORMALIZATION) && *p == CR ) {
                    // CR-LF pair becomes LF
                    // CR alone becomes LF
                    // LF-CR becomes LF
                    if ( *(p+1) == LF ) {
                        p += 2;
                    }
                    else {
                        ++p;
                    }
                    *q++ = LF;
                }
                else if ( (_flags & NEEDS_NEWLINE_NORMALIZATION) && *p == LF ) {
                    if ( *(p+1) == CR ) {
                        p += 2;
                    }
                    else {
                        ++p;
                    }
                    *q++ = LF;
                }
                else if ( (_flags & NEEDS_ENTITY_PROCESSING) && *p == '&' ) {
                    // Entities handled by tinyXML2:
                    // - special entities in the entity table [in/out]
                    // - numeric character reference [in]
                    //   &#20013; or &#x4e2d;

                    if ( *(p+1) == '#' ) {
                        char buf[10] = { 0 };
                        int len;
                        p = const_cast<char*>( TiUtil::GetCharacterRef( p, buf, &len ) );
                        for( int i=0; i<len; ++i ) {
                            *q++ = buf[i];
                        }
                        TIXMLASSERT( q <= p );
                    }
                    else {
                        int i=0;
                        for(; i<NUM_ENTITIES; ++i ) {
                            if (    strncmp( p+1, entities[i].pattern, entities[i].length ) == 0
                                    && *(p+entities[i].length+1) == ';' ) {
                                // Found an entity convert;
                                *q = entities[i].value;
                                ++q;
                                p += entities[i].length + 2;
                                break;
                            }
                        }
                        if ( i == NUM_ENTITIES ) {
                            // fixme: treat as error?
                            ++p;
                            ++q;
                        }
                    }
                }
                else {
                    *q = *p;
                    ++p;
                    ++q;
                }
            }
            *q = 0;
        }
        // The loop below has plenty going on, and this
        // is a less useful mode. Break it out.
        if ( _flags & COLLAPSE_WHITESPACE ) {
            CollapseWhitespace();
        }
        _flags = (_flags & NEEDS_DELETE);
    }
    return _start;
}




// --------- TiUtil ----------- //

const char* TiUtil::ReadBOM( const char* p, bool* bom )
{
    *bom = false;
    const unsigned char* pu = reinterpret_cast<const unsigned char*>(p);
    // Check for BOM:
    if (    *(pu+0) == TIXML_UTF_LEAD_0
            && *(pu+1) == TIXML_UTF_LEAD_1
            && *(pu+2) == TIXML_UTF_LEAD_2 ) {
        *bom = true;
        p += 3;
    }
    return p;
}


void TiUtil::ConvertUTF32ToUTF8( unsigned long input, char* output, int* length )
{
    const unsigned long BYTE_MASK = 0xBF;
    const unsigned long BYTE_MARK = 0x80;
    const unsigned long FIRST_BYTE_MARK[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

    if (input < 0x80) {
        *length = 1;
    }
    else if ( input < 0x800 ) {
        *length = 2;
    }
    else if ( input < 0x10000 ) {
        *length = 3;
    }
    else if ( input < 0x200000 ) {
        *length = 4;
    }
    else {
        *length = 0;    // This code won't covert this correctly anyway.
        return;
    }

    output += *length;

    // Scary scary fall throughs.
    switch (*length) {
        case 4:
            --output;
            *output = (char)((input | BYTE_MARK) & BYTE_MASK);
            input >>= 6;
        case 3:
            --output;
            *output = (char)((input | BYTE_MARK) & BYTE_MASK);
            input >>= 6;
        case 2:
            --output;
            *output = (char)((input | BYTE_MARK) & BYTE_MASK);
            input >>= 6;
        case 1:
            --output;
            *output = (char)(input | FIRST_BYTE_MARK[*length]);
        default:
            break;
    }
}


const char* TiUtil::GetCharacterRef( const char* p, char* value, int* length )
{
    // Presume an entity, and pull it out.
    *length = 0;

    if ( *(p+1) == '#' && *(p+2) ) {
        unsigned long ucs = 0;
        ptrdiff_t delta = 0;
        unsigned mult = 1;

        if ( *(p+2) == 'x' ) {
            // Hexadecimal.
            if ( !*(p+3) ) {
                return 0;
            }

            const char* q = p+3;
            q = strchr( q, ';' );

            if ( !q || !*q ) {
                return 0;
            }

            delta = q-p;
            --q;

            while ( *q != 'x' ) {
                if ( *q >= '0' && *q <= '9' ) {
                    ucs += mult * (*q - '0');
                }
                else if ( *q >= 'a' && *q <= 'f' ) {
                    ucs += mult * (*q - 'a' + 10);
                }
                else if ( *q >= 'A' && *q <= 'F' ) {
                    ucs += mult * (*q - 'A' + 10 );
                }
                else {
                    return 0;
                }
                mult *= 16;
                --q;
            }
        }
        else {
            // Decimal.
            if ( !*(p+2) ) {
                return 0;
            }

            const char* q = p+2;
            q = strchr( q, ';' );

            if ( !q || !*q ) {
                return 0;
            }

            delta = q-p;
            --q;

            while ( *q != '#' ) {
                if ( *q >= '0' && *q <= '9' ) {
                    ucs += mult * (*q - '0');
                }
                else {
                    return 0;
                }
                mult *= 10;
                --q;
            }
        }
        // convert the UCS to UTF-8
        ConvertUTF32ToUTF8( ucs, value, length );
        return p + delta + 1;
    }
    return p+1;
}


void TiUtil::ToStr( int v, char* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, "%d", v );
}


void TiUtil::ToStr( unsigned v, char* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, "%u", v );
}


void TiUtil::ToStr( bool v, char* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, "%d", v ? 1 : 0 );
}


void TiUtil::ToStr( float v, char* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, "%f", v );
}


void TiUtil::ToStr( double v, char* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, "%f", v );
}


bool TiUtil::ToInt( const char* str, int* value )
{
    if ( TIXML_SSCANF( str, "%d", value ) == 1 ) {
        return true;
    }
    return false;
}

bool TiUtil::ToUnsigned( const char* str, unsigned *value )
{
    if ( TIXML_SSCANF( str, "%u", value ) == 1 ) {
        return true;
    }
    return false;
}

bool TiUtil::ToBool( const char* str, bool* value )
{
    int ival = 0;
    if ( ToInt( str, &ival )) {
        *value = (ival==0) ? false : true;
        return true;
    }
    if ( StringEqual( str, "true" ) ) {
        *value = true;
        return true;
    }
    else if ( StringEqual( str, "false" ) ) {
        *value = false;
        return true;
    }
    return false;
}


bool TiUtil::ToFloat( const char* str, float* value )
{
    if ( TIXML_SSCANF( str, "%f", value ) == 1 ) {
        return true;
    }
    return false;
}

bool TiUtil::ToDouble( const char* str, double* value )
{
    if ( TIXML_SSCANF( str, "%lf", value ) == 1 ) {
        return true;
    }
    return false;
}


char* TiDocument::Identify( char* p, TiNode** node )
{
    TiNode* returnNode = 0;
    char* start = p;
    p = TiUtil::SkipWhiteSpace( p );
    if( !p || !*p ) {
        return p;
    }

    // What is this thing?
    // - Elements start with a letter or underscore, but xml is reserved.
    // - Comments: <!--
    // - Declaration: <?
    // - Everything else is unknown to tinyxml.
    //

    static const char* xmlHeader		= { "<?" };
    static const char* commentHeader	= { "<!--" };
    static const char* dtdHeader		= { "<!" };
    static const char* cdataHeader		= { "<![CDATA[" };
    static const char* elementHeader	= { "<" };	// and a header for everything else; check last.

    static const int xmlHeaderLen		= 2;
    static const int commentHeaderLen	= 4;
    static const int dtdHeaderLen		= 2;
    static const int cdataHeaderLen		= 9;
    static const int elementHeaderLen	= 1;

#if defined(_MSC_VER)
#pragma warning ( push )
#pragma warning ( disable : 4127 )
#endif
    TIXMLASSERT( sizeof( TiComment ) == sizeof( TiUnknown ) );		// use same memory pool
    TIXMLASSERT( sizeof( TiComment ) == sizeof( TiDeclaration ) );	// use same memory pool
#if defined(_MSC_VER)
#pragma warning (pop)
#endif
    if ( TiUtil::StringEqual( p, xmlHeader, xmlHeaderLen ) ) {
        returnNode = new (_commentPool.Alloc()) TiDeclaration( this );
        returnNode->_memPool = &_commentPool;
        p += xmlHeaderLen;
    }
    else if ( TiUtil::StringEqual( p, commentHeader, commentHeaderLen ) ) {
        returnNode = new (_commentPool.Alloc()) TiComment( this );
        returnNode->_memPool = &_commentPool;
        p += commentHeaderLen;
    }
    else if ( TiUtil::StringEqual( p, cdataHeader, cdataHeaderLen ) ) {
        TiText* text = new (_textPool.Alloc()) TiText( this );
        returnNode = text;
        returnNode->_memPool = &_textPool;
        p += cdataHeaderLen;
        text->SetCData( true );
    }
    else if ( TiUtil::StringEqual( p, dtdHeader, dtdHeaderLen ) ) {
        returnNode = new (_commentPool.Alloc()) TiUnknown( this );
        returnNode->_memPool = &_commentPool;
        p += dtdHeaderLen;
    }
    else if ( TiUtil::StringEqual( p, elementHeader, elementHeaderLen ) ) {
        returnNode = new (_elementPool.Alloc()) TiElement( this );
        returnNode->_memPool = &_elementPool;
        p += elementHeaderLen;
    }
    else {
        returnNode = new (_textPool.Alloc()) TiText( this );
        returnNode->_memPool = &_textPool;
        p = start;	// Back it up, all the text counts.
    }

    *node = returnNode;
    return p;
}


bool TiDocument::Accept( TiVisitor* visitor ) const
{
    if ( visitor->VisitEnter( *this ) ) {
        for ( const TiNode* node=FirstChild(); node; node=node->NextSibling() ) {
            if ( !node->Accept( visitor ) ) {
                break;
            }
        }
    }
    return visitor->VisitExit( *this );
}


// --------- TiNode ----------- //

TiNode::TiNode( TiDocument* doc ) :
    _document( doc ),
    _parent( 0 ),
    _firstChild( 0 ), _lastChild( 0 ),
    _prev( 0 ), _next( 0 ),
    _memPool( 0 )
{
}


TiNode::~TiNode()
{
    DeleteChildren();
    if ( _parent ) {
        _parent->Unlink( this );
    }
}

const char* TiNode::Value() const 
{
    return _value.GetStr();
}

void TiNode::SetValue( const char* str, bool staticMem )
{
    if ( staticMem ) {
        _value.SetInternedStr( str );
    }
    else {
        _value.SetStr( str );
    }
}


void TiNode::DeleteChildren()
{
    while( _firstChild ) {
        TiNode* node = _firstChild;
        Unlink( node );

        DELETE_NODE( node );
    }
    _firstChild = _lastChild = 0;
}


void TiNode::Unlink( TiNode* child )
{
    if ( child == _firstChild ) {
        _firstChild = _firstChild->_next;
    }
    if ( child == _lastChild ) {
        _lastChild = _lastChild->_prev;
    }

    if ( child->_prev ) {
        child->_prev->_next = child->_next;
    }
    if ( child->_next ) {
        child->_next->_prev = child->_prev;
    }
	child->_parent = 0;
}


void TiNode::DeleteChild( TiNode* node )
{
    TIXMLASSERT( node->_parent == this );
    DELETE_NODE( node );
}


TiNode* TiNode::InsertEndChild( TiNode* addThis )
{
	if (addThis->_document != _document)
		return 0;

	if (addThis->_parent)
		addThis->_parent->Unlink( addThis );
	else
	   addThis->_memPool->SetTracked();

    if ( _lastChild ) {
        TIXMLASSERT( _firstChild );
        TIXMLASSERT( _lastChild->_next == 0 );
        _lastChild->_next = addThis;
        addThis->_prev = _lastChild;
        _lastChild = addThis;

        addThis->_next = 0;
    }
    else {
        TIXMLASSERT( _firstChild == 0 );
        _firstChild = _lastChild = addThis;

        addThis->_prev = 0;
        addThis->_next = 0;
    }
    addThis->_parent = this;
    return addThis;
}


TiNode* TiNode::InsertFirstChild( TiNode* addThis )
{
	if (addThis->_document != _document)
		return 0;

	if (addThis->_parent)
		addThis->_parent->Unlink( addThis );
	else
	   addThis->_memPool->SetTracked();

    if ( _firstChild ) {
        TIXMLASSERT( _lastChild );
        TIXMLASSERT( _firstChild->_prev == 0 );

        _firstChild->_prev = addThis;
        addThis->_next = _firstChild;
        _firstChild = addThis;

        addThis->_prev = 0;
    }
    else {
        TIXMLASSERT( _lastChild == 0 );
        _firstChild = _lastChild = addThis;

        addThis->_prev = 0;
        addThis->_next = 0;
    }
    addThis->_parent = this;
     return addThis;
}


TiNode* TiNode::InsertAfterChild( TiNode* afterThis, TiNode* addThis )
{
	if (addThis->_document != _document)
		return 0;

    TIXMLASSERT( afterThis->_parent == this );

    if ( afterThis->_parent != this ) {
        return 0;
    }

    if ( afterThis->_next == 0 ) {
        // The last node or the only node.
        return InsertEndChild( addThis );
    }
	if (addThis->_parent)
		addThis->_parent->Unlink( addThis );
	else
	   addThis->_memPool->SetTracked();
    addThis->_prev = afterThis;
    addThis->_next = afterThis->_next;
    afterThis->_next->_prev = addThis;
    afterThis->_next = addThis;
    addThis->_parent = this;
    return addThis;
}




const TiElement* TiNode::FirstChildElement( const char* value ) const
{
    for( TiNode* node=_firstChild; node; node=node->_next ) {
        TiElement* element = node->ToElement();
        if ( element ) {
            if ( !value || TiUtil::StringEqual( element->Name(), value ) ) {
                return element;
            }
        }
    }
    return 0;
}


const TiElement* TiNode::LastChildElement( const char* value ) const
{
    for( TiNode* node=_lastChild; node; node=node->_prev ) {
        TiElement* element = node->ToElement();
        if ( element ) {
            if ( !value || TiUtil::StringEqual( element->Name(), value ) ) {
                return element;
            }
        }
    }
    return 0;
}


const TiElement* TiNode::NextSiblingElement( const char* value ) const
{
    for( TiNode* element=this->_next; element; element = element->_next ) {
        if (    element->ToElement()
                && (!value || TiUtil::StringEqual( value, element->Value() ))) {
            return element->ToElement();
        }
    }
    return 0;
}


const TiElement* TiNode::PreviousSiblingElement( const char* value ) const
{
    for( TiNode* element=_prev; element; element = element->_prev ) {
        if (    element->ToElement()
                && (!value || TiUtil::StringEqual( value, element->Value() ))) {
            return element->ToElement();
        }
    }
    return 0;
}


char* TiNode::ParseDeep( char* p, StrPair* parentEnd )
{
    // This is a recursive method, but thinking about it "at the current level"
    // it is a pretty simple flat list:
    //		<foo/>
    //		<!-- comment -->
    //
    // With a special case:
    //		<foo>
    //		</foo>
    //		<!-- comment -->
    //
    // Where the closing element (/foo) *must* be the next thing after the opening
    // element, and the names must match. BUT the tricky bit is that the closing
    // element will be read by the child.
    //
    // 'endTag' is the end tag for this node, it is returned by a call to a child.
    // 'parentEnd' is the end tag for the parent, which is filled in and returned.

    while( p && *p ) {
        TiNode* node = 0;

        p = _document->Identify( p, &node );
        if ( p == 0 || node == 0 ) {
            break;
        }

        StrPair endTag;
        p = node->ParseDeep( p, &endTag );
        if ( !p ) {
            DELETE_NODE( node );
            node = 0;
            if ( !_document->Error() ) {
                _document->SetError( XML_ERROR_PARSING, 0, 0 );
            }
            break;
        }

        // We read the end tag. Return it to the parent.
        if ( node->ToElement() && node->ToElement()->ClosingType() == TiElement::CLOSING ) {
            if ( parentEnd ) {
                *parentEnd = static_cast<TiElement*>(node)->_value;
            }
			node->_memPool->SetTracked();	// created and then immediately deleted.
            DELETE_NODE( node );
            return p;
        }

        // Handle an end tag returned to this level.
        // And handle a bunch of annoying errors.
        TiElement* ele = node->ToElement();
        if ( ele ) {
            if ( endTag.Empty() && ele->ClosingType() == TiElement::OPEN ) {
                _document->SetError( XML_ERROR_MISMATCHED_ELEMENT, node->Value(), 0 );
                p = 0;
            }
            else if ( !endTag.Empty() && ele->ClosingType() != TiElement::OPEN ) {
                _document->SetError( XML_ERROR_MISMATCHED_ELEMENT, node->Value(), 0 );
                p = 0;
            }
            else if ( !endTag.Empty() ) {
                if ( !TiUtil::StringEqual( endTag.GetStr(), node->Value() )) {
                    _document->SetError( XML_ERROR_MISMATCHED_ELEMENT, node->Value(), 0 );
                    p = 0;
                }
            }
        }
        if ( p == 0 ) {
            DELETE_NODE( node );
            node = 0;
        }
        if ( node ) {
            this->InsertEndChild( node );
        }
    }
    return 0;
}


/************************************************************************/
/*                     USER CODE BY Jeson Chen BEGIN                    */
/************************************************************************/

TiElement* TiNode::FirstChildElemWithAttr( const char* pNode, const char* pAttrName, const char* pAttrValue)
{
	TiElement* pe = FirstChildElement(pNode);
	if (!(pAttrName && pAttrValue))
	{
		// no attr check needed.
		return pe;
	}
	while (pe)
	{
		if (pe->Attribute(pAttrName, pAttrValue))
		{
			return pe;
		}
		pe = pe->NextSiblingElement(pNode);
	}
	return NULL;
}


TiElement* TiNode::ElementByPath(const char* pNodePath)
{
	StrPairLink paths;
	ParsePath(pNodePath, paths);
	TiElement* pe = NULL;
	if (!paths.m_Node.GetStr())
	{
		return NULL;
	}
	pe = FirstChildElemWithAttr(paths.m_Node.GetStr(), 
									paths.m_AttrName.GetStr(), 
									paths.m_AttrValue.GetStr());
	if (!pe)
	{
		return NULL;
	}

	StrPairLink* pLink = paths.m_pNext;
	while (pLink)
	{
		if (!pLink->m_Node.GetStr())
		{
			return NULL;
		}
		pe = pe->FirstChildElemWithAttr(pLink->m_Node.GetStr(), 
										pLink->m_AttrName.GetStr(), 
										pLink->m_AttrValue.GetStr());
		if (!pe)
		{
			return NULL;
		}
		pLink = pLink->m_pNext;
	}
	return pe;
}


bool TiNode::SetAttribute(const char* pNodePath, const char* pAttrName, const char* pAttrValue)
{
	TiElement* pe = ElementByPath(pNodePath);
	if (!pe)
	{
		return false;
	}

	pe->SetAttribute(pAttrName, pAttrValue);
	return true;
}

/************************************************************************/
/*                      USER CODE BY Jeson Chen END                     */
/************************************************************************/

// --------- TiText ---------- //
char* TiText::ParseDeep( char* p, StrPair* )
{
    const char* start = p;
    if ( this->CData() ) {
        p = _value.ParseText( p, "]]>", StrPair::NEEDS_NEWLINE_NORMALIZATION );
        if ( !p ) {
            _document->SetError( XML_ERROR_PARSING_CDATA, start, 0 );
        }
        return p;
    }
    else {
        int flags = _document->ProcessEntities() ? StrPair::TEXT_ELEMENT : StrPair::TEXT_ELEMENT_LEAVE_ENTITIES;
        if ( _document->WhitespaceMode() == COLLAPSE_WHITESPACE ) {
            flags |= StrPair::COLLAPSE_WHITESPACE;
        }

        p = _value.ParseText( p, "<", flags );
        if ( !p ) {
            _document->SetError( XML_ERROR_PARSING_TEXT, start, 0 );
        }
        if ( p && *p ) {
            return p-1;
        }
    }
    return 0;
}


TiNode* TiText::ShallowClone( TiDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    TiText* text = doc->NewText( Value() );	// fixme: this will always allocate memory. Intern?
    text->SetCData( this->CData() );
    return text;
}


bool TiText::ShallowEqual( const TiNode* compare ) const
{
    return ( compare->ToText() && TiUtil::StringEqual( compare->ToText()->Value(), Value() ));
}


bool TiText::Accept( TiVisitor* visitor ) const
{
    return visitor->Visit( *this );
}


// --------- TiComment ---------- //

TiComment::TiComment( TiDocument* doc ) : TiNode( doc )
{
}


TiComment::~TiComment()
{
}


char* TiComment::ParseDeep( char* p, StrPair* )
{
    // Comment parses as text.
    const char* start = p;
    p = _value.ParseText( p, "-->", StrPair::COMMENT );
    if ( p == 0 ) {
        _document->SetError( XML_ERROR_PARSING_COMMENT, start, 0 );
    }
    return p;
}


TiNode* TiComment::ShallowClone( TiDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    TiComment* comment = doc->NewComment( Value() );	// fixme: this will always allocate memory. Intern?
    return comment;
}


bool TiComment::ShallowEqual( const TiNode* compare ) const
{
    return ( compare->ToComment() && TiUtil::StringEqual( compare->ToComment()->Value(), Value() ));
}


bool TiComment::Accept( TiVisitor* visitor ) const
{
    return visitor->Visit( *this );
}


// --------- TiDeclaration ---------- //

TiDeclaration::TiDeclaration( TiDocument* doc ) : TiNode( doc )
{
}


TiDeclaration::~TiDeclaration()
{
    //printf( "~TiDeclaration\n" );
}


char* TiDeclaration::ParseDeep( char* p, StrPair* )
{
    // Declaration parses as text.
    const char* start = p;
    p = _value.ParseText( p, "?>", StrPair::NEEDS_NEWLINE_NORMALIZATION );
    if ( p == 0 ) {
        _document->SetError( XML_ERROR_PARSING_DECLARATION, start, 0 );
    }
    return p;
}


TiNode* TiDeclaration::ShallowClone( TiDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    TiDeclaration* dec = doc->NewDeclaration( Value() );	// fixme: this will always allocate memory. Intern?
    return dec;
}


bool TiDeclaration::ShallowEqual( const TiNode* compare ) const
{
    return ( compare->ToDeclaration() && TiUtil::StringEqual( compare->ToDeclaration()->Value(), Value() ));
}



bool TiDeclaration::Accept( TiVisitor* visitor ) const
{
    return visitor->Visit( *this );
}

// --------- TiUnknown ---------- //

TiUnknown::TiUnknown( TiDocument* doc ) : TiNode( doc )
{
}


TiUnknown::~TiUnknown()
{
}


char* TiUnknown::ParseDeep( char* p, StrPair* )
{
    // Unknown parses as text.
    const char* start = p;

    p = _value.ParseText( p, ">", StrPair::NEEDS_NEWLINE_NORMALIZATION );
    if ( !p ) {
        _document->SetError( XML_ERROR_PARSING_UNKNOWN, start, 0 );
    }
    return p;
}


TiNode* TiUnknown::ShallowClone( TiDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    TiUnknown* text = doc->NewUnknown( Value() );	// fixme: this will always allocate memory. Intern?
    return text;
}


bool TiUnknown::ShallowEqual( const TiNode* compare ) const
{
    return ( compare->ToUnknown() && TiUtil::StringEqual( compare->ToUnknown()->Value(), Value() ));
}


bool TiUnknown::Accept( TiVisitor* visitor ) const
{
    return visitor->Visit( *this );
}

// --------- TiAttribute ---------- //

const char* TiAttribute::Name() const 
{
    return _name.GetStr();
}

const char* TiAttribute::Value() const 
{
    return _value.GetStr();
}

char* TiAttribute::ParseDeep( char* p, bool processEntities )
{
    // Parse using the name rules: bug fix, was using ParseText before
    p = _name.ParseName( p );
    if ( !p || !*p ) {
        return 0;
    }

    // Skip white space before =
    p = TiUtil::SkipWhiteSpace( p );
    if ( !p || *p != '=' ) {
        return 0;
    }

    ++p;	// move up to opening quote
    p = TiUtil::SkipWhiteSpace( p );
    if ( *p != '\"' && *p != '\'' ) {
        return 0;
    }

    char endTag[2] = { *p, 0 };
    ++p;	// move past opening quote

    p = _value.ParseText( p, endTag, processEntities ? StrPair::ATTRIBUTE_VALUE : StrPair::ATTRIBUTE_VALUE_LEAVE_ENTITIES );
    return p;
}


void TiAttribute::SetName( const char* n )
{
    _name.SetStr( n );
}


TiError TiAttribute::QueryIntValue( int* value ) const
{
    if ( TiUtil::ToInt( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


TiError TiAttribute::QueryUnsignedValue( unsigned int* value ) const
{
    if ( TiUtil::ToUnsigned( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


TiError TiAttribute::QueryBoolValue( bool* value ) const
{
    if ( TiUtil::ToBool( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


TiError TiAttribute::QueryFloatValue( float* value ) const
{
    if ( TiUtil::ToFloat( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


TiError TiAttribute::QueryDoubleValue( double* value ) const
{
    if ( TiUtil::ToDouble( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


void TiAttribute::SetAttribute( const char* v )
{
    _value.SetStr( v );
}


void TiAttribute::SetAttribute( int v )
{
    char buf[BUF_SIZE];
    TiUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}


void TiAttribute::SetAttribute( unsigned v )
{
    char buf[BUF_SIZE];
    TiUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}


void TiAttribute::SetAttribute( bool v )
{
    char buf[BUF_SIZE];
    TiUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}

void TiAttribute::SetAttribute( double v )
{
    char buf[BUF_SIZE];
    TiUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}

void TiAttribute::SetAttribute( float v )
{
    char buf[BUF_SIZE];
    TiUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}


// --------- TiElement ---------- //
TiElement::TiElement( TiDocument* doc ) : TiNode( doc ),
    _closingType( 0 ),
    _rootAttribute( 0 )
{
}


TiElement::~TiElement()
{
    while( _rootAttribute ) {
        TiAttribute* next = _rootAttribute->_next;
        DELETE_ATTRIBUTE( _rootAttribute );
        _rootAttribute = next;
    }
}

/************************************************************************/
/*                     USER CODE BY Jeson Chen BEGIN                    */
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
// ???? difference between 
// protected:    TiAttribute* FindAttribute( const char* name )
// and
// public: const TiAttribute* FindAttribute( const char* name ) const;

//TiAttribute* TiElement::FindAttribute( const char* name )
//{
//    TiAttribute* a = 0;
//    for( a=_rootAttribute; a; a = a->_next ) {
//        if ( TiUtil::StringEqual( a->Name(), name ) ) {
//            return a;
//        }
//    }
//    return 0;
//}


/*const*/ TiAttribute* TiElement::FindAttribute( const char* name ) const
{
    TiAttribute* a = 0;
    for( a=_rootAttribute; a; a = a->_next ) {
        if ( TiUtil::StringEqual( a->Name(), name ) ) {
            return a;
        }
    }
    return 0;
}

// append xml string node to element, return appended element.
TiElement* TiElement::CopyAndAppendXmlString(const char* pXml)
{
	TiNode* pOldLast = _lastChild;
	char* psz = new char[strlen(pXml) + 1];
	memcpy(psz, pXml, strlen(pXml) + 1);
	// psz managed by _document
	_document->_xmlStrings.Add(psz);
	TiNode::ParseDeep(psz, 0);

	if (pOldLast == _lastChild)
	{
		return NULL;
	}
	return (TiElement*)_lastChild;
}

// append node peSrc to element, return appended element.
// peSrc remain unchanged.
TiElement* TiElement::CopyAndAppendNode(const TiElement* peSrc)
{
	TiPrinter printer;
	peSrc->Accept(&printer);
	const char* szText = printer.CStr();

	return CopyAndAppendXmlString(szText);
}

/************************************************************************/
/*                      USER CODE BY Jeson Chen END                     */
/************************************************************************/

const char* TiElement::Attribute( const char* name, const char* value ) const
{
    const TiAttribute* a = FindAttribute( name );
    if ( !a ) {
        return 0;
    }
    if ( !value || TiUtil::StringEqual( a->Value(), value )) {
        return a->Value();
    }
    return 0;
}


const char* TiElement::GetText() const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        return FirstChild()->ToText()->Value();
    }
    return 0;
}


TiError TiElement::QueryIntText( int* ival ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const char* t = FirstChild()->ToText()->Value();
        if ( TiUtil::ToInt( t, ival ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


TiError TiElement::QueryUnsignedText( unsigned* uval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const char* t = FirstChild()->ToText()->Value();
        if ( TiUtil::ToUnsigned( t, uval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


TiError TiElement::QueryBoolText( bool* bval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const char* t = FirstChild()->ToText()->Value();
        if ( TiUtil::ToBool( t, bval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


TiError TiElement::QueryDoubleText( double* dval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const char* t = FirstChild()->ToText()->Value();
        if ( TiUtil::ToDouble( t, dval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


TiError TiElement::QueryFloatText( float* fval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const char* t = FirstChild()->ToText()->Value();
        if ( TiUtil::ToFloat( t, fval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}



TiAttribute* TiElement::FindOrCreateAttribute( const char* name )
{
    TiAttribute* last = 0;
    TiAttribute* attrib = 0;
    for( attrib = _rootAttribute;
            attrib;
            last = attrib, attrib = attrib->_next ) {
        if ( TiUtil::StringEqual( attrib->Name(), name ) ) {
            break;
        }
    }
    if ( !attrib ) {
        attrib = new (_document->_attributePool.Alloc() ) TiAttribute();
        attrib->_memPool = &_document->_attributePool;
        if ( last ) {
            last->_next = attrib;
        }
        else {
            _rootAttribute = attrib;
        }
        attrib->SetName( name );
        attrib->_memPool->SetTracked(); // always created and linked.
    }
    return attrib;
}


void TiElement::DeleteAttribute( const char* name )
{
    TiAttribute* prev = 0;
    for( TiAttribute* a=_rootAttribute; a; a=a->_next ) {
        if ( TiUtil::StringEqual( name, a->Name() ) ) {
            if ( prev ) {
                prev->_next = a->_next;
            }
            else {
                _rootAttribute = a->_next;
            }
            DELETE_ATTRIBUTE( a );
            break;
        }
        prev = a;
    }
}


char* TiElement::ParseAttributes( char* p )
{
    const char* start = p;
    TiAttribute* prevAttribute = 0;

    // Read the attributes.
    while( p ) {
        p = TiUtil::SkipWhiteSpace( p );
        if ( !p || !(*p) ) {
            _document->SetError( XML_ERROR_PARSING_ELEMENT, start, Name() );
            return 0;
        }

        // attribute.
        if (TiUtil::IsNameStartChar( *p ) ) {
            TiAttribute* attrib = new (_document->_attributePool.Alloc() ) TiAttribute();
            attrib->_memPool = &_document->_attributePool;
			attrib->_memPool->SetTracked();

            p = attrib->ParseDeep( p, _document->ProcessEntities() );
            if ( !p || Attribute( attrib->Name() ) ) {
                DELETE_ATTRIBUTE( attrib );
                _document->SetError( XML_ERROR_PARSING_ATTRIBUTE, start, p );
                return 0;
            }
            // There is a minor bug here: if the attribute in the source xml
            // document is duplicated, it will not be detected and the
            // attribute will be doubly added. However, tracking the 'prevAttribute'
            // avoids re-scanning the attribute list. Preferring performance for
            // now, may reconsider in the future.
            if ( prevAttribute ) {
                prevAttribute->_next = attrib;
            }
            else {
                _rootAttribute = attrib;
            }
            prevAttribute = attrib;
        }
        // end of the tag
        else if ( *p == '/' && *(p+1) == '>' ) {
            _closingType = CLOSED;
            return p+2;	// done; sealed element.
        }
        // end of the tag
        else if ( *p == '>' ) {
            ++p;
            break;
        }
        else {
            _document->SetError( XML_ERROR_PARSING_ELEMENT, start, p );
            return 0;
        }
    }
    return p;
}


//
//	<ele></ele>
//	<ele>foo<b>bar</b></ele>
//
char* TiElement::ParseDeep( char* p, StrPair* strPair )
{
    // Read the element name.
    p = TiUtil::SkipWhiteSpace( p );
    if ( !p ) {
        return 0;
    }

    // The closing element is the </element> form. It is
    // parsed just like a regular element then deleted from
    // the DOM.
    if ( *p == '/' ) {
        _closingType = CLOSING;
        ++p;
    }

    p = _value.ParseName( p );
    if ( _value.Empty() ) {
        return 0;
    }

    p = ParseAttributes( p );
    if ( !p || !*p || _closingType ) {
        return p;
    }

    p = TiNode::ParseDeep( p, strPair );
    return p;
}



TiNode* TiElement::ShallowClone( TiDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    TiElement* element = doc->NewElement( Value() );					// fixme: this will always allocate memory. Intern?
    for( const TiAttribute* a=FirstAttribute(); a; a=a->Next() ) {
        element->SetAttribute( a->Name(), a->Value() );					// fixme: this will always allocate memory. Intern?
    }
    return element;
}


bool TiElement::ShallowEqual( const TiNode* compare ) const
{
    const TiElement* other = compare->ToElement();
    if ( other && TiUtil::StringEqual( other->Value(), Value() )) {

        const TiAttribute* a=FirstAttribute();
        const TiAttribute* b=other->FirstAttribute();

        while ( a && b ) {
            if ( !TiUtil::StringEqual( a->Value(), b->Value() ) ) {
                return false;
            }
            a = a->Next();
            b = b->Next();
        }
        if ( a || b ) {
            // different count
            return false;
        }
        return true;
    }
    return false;
}


bool TiElement::Accept( TiVisitor* visitor ) const
{
    if ( visitor->VisitEnter( *this, _rootAttribute ) ) {
        for ( const TiNode* node=FirstChild(); node; node=node->NextSibling() ) {
            if ( !node->Accept( visitor ) ) {
                break;
            }
        }
    }
    return visitor->VisitExit( *this );
}


// --------- TiDocument ----------- //
TiDocument::TiDocument( bool processEntities, Whitespace whitespace ) :
    TiNode( 0 ),
    _writeBOM( false ),
    _processEntities( processEntities ),
    _errorID( XML_NO_ERROR ),
    _whitespace( whitespace ),
    _errorStr1( 0 ),
    _errorStr2( 0 ),
    _charBuffer( 0 )
{
    _document = this;	// avoid warning about 'this' in initializer list
}


TiDocument::~TiDocument()
{
    DeleteChildren();
    delete [] _charBuffer;

#if 0
    _textPool.Trace( "text" );
    _elementPool.Trace( "element" );
    _commentPool.Trace( "comment" );
    _attributePool.Trace( "attribute" );
#endif

#ifdef DEBUG
	if ( Error() == false ) {
		TIXMLASSERT( _elementPool.CurrentAllocs()   == _elementPool.Untracked() );
		TIXMLASSERT( _attributePool.CurrentAllocs() == _attributePool.Untracked() );
		TIXMLASSERT( _textPool.CurrentAllocs()      == _textPool.Untracked() );
		TIXMLASSERT( _commentPool.CurrentAllocs()   == _commentPool.Untracked() );
	}
#endif
}


void TiDocument::Clear()
{
    DeleteChildren();

    _errorID = XML_NO_ERROR;
    _errorStr1 = 0;
    _errorStr2 = 0;

    delete [] _charBuffer;
    _charBuffer = 0;
}


TiElement* TiDocument::NewElement( const char* name )
{
    TiElement* ele = new (_elementPool.Alloc()) TiElement( this );
    ele->_memPool = &_elementPool;
    ele->SetName( name );
    return ele;
}


TiComment* TiDocument::NewComment( const char* str )
{
    TiComment* comment = new (_commentPool.Alloc()) TiComment( this );
    comment->_memPool = &_commentPool;
    comment->SetValue( str );
    return comment;
}


TiText* TiDocument::NewText( const char* str )
{
    TiText* text = new (_textPool.Alloc()) TiText( this );
    text->_memPool = &_textPool;
    text->SetValue( str );
    return text;
}


TiDeclaration* TiDocument::NewDeclaration( const char* str )
{
    TiDeclaration* dec = new (_commentPool.Alloc()) TiDeclaration( this );
    dec->_memPool = &_commentPool;
    dec->SetValue( str ? str : "xml version=\"1.0\" encoding=\"UTF-8\"" );
    return dec;
}


TiUnknown* TiDocument::NewUnknown( const char* str )
{
    TiUnknown* unk = new (_commentPool.Alloc()) TiUnknown( this );
    unk->_memPool = &_commentPool;
    unk->SetValue( str );
    return unk;
}


TiError TiDocument::LoadFile( const char* filename )
{
    Clear();
    FILE* fp = 0;

#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
    errno_t err = fopen_s(&fp, filename, "rb" );
    if ( !fp || err) {
#else
    fp = fopen( filename, "rb" );
    if ( !fp) {
#endif
        SetError( XML_ERROR_FILE_NOT_FOUND, filename, 0 );
        return _errorID;
    }
    LoadFile( fp );
    fclose( fp );
    return _errorID;
}

#pragma warning ( disable : 4996 )
TiError TiDocument::LoadFile( const wchar_t* filenameW )
{
	char* pFileNameA = NULL;
	int nByteNeed = wcstombs(NULL, filenameW, 0);
	if (!nByteNeed)
	{
		return XML_ERROR_FILE_NOT_FOUND;
	}

	pFileNameA = new char[nByteNeed + 1];
	int nByteA = wcstombs(pFileNameA, filenameW, nByteNeed);
	pFileNameA[nByteNeed] = NULL;

	TiError tinyRet = LoadFile(pFileNameA);
	delete pFileNameA;
	return tinyRet;
}
#pragma warning ( default : 4996 )


TiError TiDocument::LoadFile( FILE* fp )
{
    Clear();

    fseek( fp, 0, SEEK_END );
    size_t size = ftell( fp );
    fseek( fp, 0, SEEK_SET );

    if ( size == 0 ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return _errorID;
    }

    _charBuffer = new char[size+1];
    size_t read = fread( _charBuffer, 1, size, fp );
    if ( read != size ) {
        SetError( XML_ERROR_FILE_READ_ERROR, 0, 0 );
        return _errorID;
    }

    _charBuffer[size] = 0;

    const char* p = _charBuffer;
    p = TiUtil::SkipWhiteSpace( p );
    p = TiUtil::ReadBOM( p, &_writeBOM );
    if ( !p || !*p ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return _errorID;
    }

    ParseDeep( _charBuffer + (p-_charBuffer), 0 );
    return _errorID;
}


TiError TiDocument::SaveFile( const char* filename, bool compact )
{
    FILE* fp = 0;
#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
    errno_t err = fopen_s(&fp, filename, "w" );
    if ( !fp || err) {
#else
    fp = fopen( filename, "w" );
    if ( !fp) {
#endif
        SetError( XML_ERROR_FILE_COULD_NOT_BE_OPENED, filename, 0 );
        return _errorID;
    }
    SaveFile(fp, compact);
    fclose( fp );
    return _errorID;
}


TiError TiDocument::SaveFile( FILE* fp, bool compact )
{
    TiPrinter stream( fp, compact );
    Print( &stream );
    return _errorID;
}


TiError TiDocument::Parse( const char* p, size_t len )
{
	const char* start = p;
    Clear();

    if ( len == 0 ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return _errorID;
    }

    if ( !p || !*p ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return _errorID;
    }
    if ( len == (size_t)(-1) ) {
        len = strlen( p );
    }
    _charBuffer = new char[ len+1 ];
    memcpy( _charBuffer, p, len );
    _charBuffer[len] = 0;

    p = TiUtil::SkipWhiteSpace( p );
    p = TiUtil::ReadBOM( p, &_writeBOM );
    if ( !p || !*p ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return _errorID;
    }

    ptrdiff_t delta = p - start;	// skip initial whitespace, BOM, etc.
    ParseDeep( _charBuffer+delta, 0 );
    return _errorID;
}


void TiDocument::Print( TiPrinter* streamer ) const
{
    TiPrinter stdStreamer( stdout );
    if ( !streamer ) {
        streamer = &stdStreamer;
    }
    Accept( streamer );
}


void TiDocument::SetError( TiError error, const char* str1, const char* str2 )
{
    _errorID = error;
    _errorStr1 = str1;
    _errorStr2 = str2;
}


void TiDocument::PrintError() const
{
    if ( _errorID ) {
        static const int LEN = 20;
        char buf1[LEN] = { 0 };
        char buf2[LEN] = { 0 };

        if ( _errorStr1 ) {
            TIXML_SNPRINTF( buf1, LEN, "%s", _errorStr1 );
        }
        if ( _errorStr2 ) {
            TIXML_SNPRINTF( buf2, LEN, "%s", _errorStr2 );
        }

        printf( "TiDocument error id=%d str1=%s str2=%s\n",
                _errorID, buf1, buf2 );
    }
}


TiPrinter::TiPrinter( FILE* file, bool compact, int depth ) :
    _elementJustOpened( false ),
    _firstElement( true ),
    _fp( file ),
    _depth( depth ),
    _textDepth( -1 ),
    _processEntities( true ),
    _compactMode( compact )
{
    for( int i=0; i<ENTITY_RANGE; ++i ) {
        _entityFlag[i] = false;
        _restrictedEntityFlag[i] = false;
    }
    for( int i=0; i<NUM_ENTITIES; ++i ) {
        TIXMLASSERT( entities[i].value < ENTITY_RANGE );
        if ( entities[i].value < ENTITY_RANGE ) {
            _entityFlag[ (int)entities[i].value ] = true;
        }
    }
    _restrictedEntityFlag[(int)'&'] = true;
    _restrictedEntityFlag[(int)'<'] = true;
    _restrictedEntityFlag[(int)'>'] = true;	// not required, but consistency is nice
    _buffer.Push( 0 );
}


void TiPrinter::Print( const char* format, ... )
{
    va_list     va;
    va_start( va, format );

    if ( _fp ) {
        vfprintf( _fp, format, va );
    }
    else {
        // This seems brutally complex. Haven't figured out a better
        // way on windows.
#ifdef _MSC_VER
        int len = -1;
        int expand = 1000;
        while ( len < 0 ) {
            len = vsnprintf_s( _accumulator.Mem(), _accumulator.Capacity(), _TRUNCATE, format, va );
            if ( len < 0 ) {
                expand *= 3/2;
                _accumulator.PushArr( expand );
            }
        }
        char* p = _buffer.PushArr( len ) - 1;
        memcpy( p, _accumulator.Mem(), len+1 );
#else
        int len = vsnprintf( 0, 0, format, va );
        // Close out and re-start the va-args
        va_end( va );
        va_start( va, format );
        char* p = _buffer.PushArr( len ) - 1;
        vsnprintf( p, len+1, format, va );
#endif
    }
    va_end( va );
}


void TiPrinter::PrintSpace( int depth )
{
    for( int i=0; i<depth; ++i ) {
        Print( "    " );
    }
}


void TiPrinter::PrintString( const char* p, bool restricted )
{
    // Look for runs of bytes between entities to print.
    const char* q = p;
    const bool* flag = restricted ? _restrictedEntityFlag : _entityFlag;

    if ( _processEntities ) {
        while ( *q ) {
            // Remember, char is sometimes signed. (How many times has that bitten me?)
            if ( *q > 0 && *q < ENTITY_RANGE ) {
                // Check for entities. If one is found, flush
                // the stream up until the entity, write the
                // entity, and keep looking.
                if ( flag[(unsigned)(*q)] ) {
                    while ( p < q ) {
                        Print( "%c", *p );
                        ++p;
                    }
                    for( int i=0; i<NUM_ENTITIES; ++i ) {
                        if ( entities[i].value == *q ) {
                            Print( "&%s;", entities[i].pattern );
                            break;
                        }
                    }
                    ++p;
                }
            }
            ++q;
        }
    }
    // Flush the remaining string. This will be the entire
    // string if an entity wasn't found.
    if ( !_processEntities || (q-p > 0) ) {
        Print( "%s", p );
    }
}


void TiPrinter::PushHeader( bool writeBOM, bool writeDec )
{
    if ( writeBOM ) {
        static const unsigned char bom[] = { TIXML_UTF_LEAD_0, TIXML_UTF_LEAD_1, TIXML_UTF_LEAD_2, 0 };
        Print( "%s", bom );
    }
    if ( writeDec ) {
        PushDeclaration( "xml version=\"1.0\"" );
    }
}


void TiPrinter::OpenElement( const char* name )
{
    if ( _elementJustOpened ) {
        SealElement();
    }
    _stack.Push( name );

    if ( _textDepth < 0 && !_firstElement && !_compactMode ) {
        Print( "\n" );
    }
    if ( !_compactMode ) {
        PrintSpace( _depth );
    }

    Print( "<%s", name );
    _elementJustOpened = true;
    _firstElement = false;
    ++_depth;
}


void TiPrinter::PushAttribute( const char* name, const char* value )
{
    TIXMLASSERT( _elementJustOpened );
    Print( " %s=\"", name );
    PrintString( value, false );
    Print( "\"" );
}


void TiPrinter::PushAttribute( const char* name, int v )
{
    char buf[BUF_SIZE];
    TiUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void TiPrinter::PushAttribute( const char* name, unsigned v )
{
    char buf[BUF_SIZE];
    TiUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void TiPrinter::PushAttribute( const char* name, bool v )
{
    char buf[BUF_SIZE];
    TiUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void TiPrinter::PushAttribute( const char* name, double v )
{
    char buf[BUF_SIZE];
    TiUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void TiPrinter::CloseElement()
{
    --_depth;
    const char* name = _stack.Pop();

    if ( _elementJustOpened ) {
        Print( "/>" );
    }
    else {
        if ( _textDepth < 0 && !_compactMode) {
            Print( "\n" );
            PrintSpace( _depth );
        }
        Print( "</%s>", name );
    }

    if ( _textDepth == _depth ) {
        _textDepth = -1;
    }
    if ( _depth == 0 && !_compactMode) {
        Print( "\n" );
    }
    _elementJustOpened = false;
}


void TiPrinter::SealElement()
{
    _elementJustOpened = false;
    Print( ">" );
}


void TiPrinter::PushText( const char* text, bool cdata )
{
    _textDepth = _depth-1;

    if ( _elementJustOpened ) {
        SealElement();
    }
    if ( cdata ) {
        Print( "<![CDATA[" );
        Print( "%s", text );
        Print( "]]>" );
    }
    else {
        PrintString( text, true );
    }
}

void TiPrinter::PushText( int value )
{
    char buf[BUF_SIZE];
    TiUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void TiPrinter::PushText( unsigned value )
{
    char buf[BUF_SIZE];
    TiUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void TiPrinter::PushText( bool value )
{
    char buf[BUF_SIZE];
    TiUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void TiPrinter::PushText( float value )
{
    char buf[BUF_SIZE];
    TiUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void TiPrinter::PushText( double value )
{
    char buf[BUF_SIZE];
    TiUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void TiPrinter::PushComment( const char* comment )
{
    if ( _elementJustOpened ) {
        SealElement();
    }
    if ( _textDepth < 0 && !_firstElement && !_compactMode) {
        Print( "\n" );
        PrintSpace( _depth );
    }
    _firstElement = false;
    Print( "<!--%s-->", comment );
}


void TiPrinter::PushDeclaration( const char* value )
{
    if ( _elementJustOpened ) {
        SealElement();
    }
    if ( _textDepth < 0 && !_firstElement && !_compactMode) {
        Print( "\n" );
        PrintSpace( _depth );
    }
    _firstElement = false;
    Print( "<?%s?>", value );
}


void TiPrinter::PushUnknown( const char* value )
{
    if ( _elementJustOpened ) {
        SealElement();
    }
    if ( _textDepth < 0 && !_firstElement && !_compactMode) {
        Print( "\n" );
        PrintSpace( _depth );
    }
    _firstElement = false;
    Print( "<!%s>", value );
}


bool TiPrinter::VisitEnter( const TiDocument& doc )
{
    _processEntities = doc.ProcessEntities();
    if ( doc.HasBOM() ) {
        PushHeader( true, false );
    }
    return true;
}


bool TiPrinter::VisitEnter( const TiElement& element, const TiAttribute* attribute )
{
    OpenElement( element.Name() );
    while ( attribute ) {
        PushAttribute( attribute->Name(), attribute->Value() );
        attribute = attribute->Next();
    }
    return true;
}


bool TiPrinter::VisitExit( const TiElement& )
{
    CloseElement();
    return true;
}


bool TiPrinter::Visit( const TiText& text )
{
    PushText( text.Value(), text.CData() );
    return true;
}


bool TiPrinter::Visit( const TiComment& comment )
{
    PushComment( comment.Value() );
    return true;
}

bool TiPrinter::Visit( const TiDeclaration& declaration )
{
    PushDeclaration( declaration.Value() );
    return true;
}


bool TiPrinter::Visit( const TiUnknown& unknown )
{
    PushUnknown( unknown.Value() );
    return true;
}

}   // namespace tinyxml2

