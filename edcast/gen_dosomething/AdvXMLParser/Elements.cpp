// ===================================================================
// AdvXMLParser
//
// See AdvXMLParser.h
// ===================================================================

#include "AdvXMLParser.h"

#if defined(_DEBUG) & defined(DEBUG_NEW)
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// ===================================================================
// Our namespace
// ===================================================================
namespace AdvXMLParser
{

using advstd::auto_ptr;

// ===================================================================
// Null nodes
// ===================================================================

NodeContainer&  Node::null = Element::null;
Attribute       Attribute::null(Element::null, TEXT(""));
Element         Element::null(Element::null, TEXT(""));
Comment         Comment::null(TEXT(""));
Pi              Pi::null(TEXT(""), TEXT(""));
CData           CData::null(TEXT(""));


// ===================================================================
// Helpers
// ===================================================================

// White space: space, tabulation, carriage return, line feed.
static const Char s_spaces[] = TEXT(" \t\r\n");

void NormalizeWhiteSpaces(String& strText)
// Replace white spaces by only one space
{
    String::size_type nPosition = 0;
    while(nPosition < strText.size())
    {
        String::size_type nFirst = strText.find_first_of(s_spaces, nPosition);
        if(nFirst == String::npos)
            break; // No space, so nothing to normalize

        String::size_type nEnd = strText.find_first_not_of(s_spaces, nFirst);
        if(nEnd == String::npos)
        {
            // Only spaces. Replace by only one space
            strText.replace(nFirst, strText.size() - nFirst, 1, TEXT(' '));
            break;
        }

        // Replace by only one space
        strText.replace(nFirst, nEnd - nFirst, 1, TEXT(' '));
        // New position: just after the space
        nPosition = nFirst + 1;
    }
}

void TrimSpaces(String& strText)
// Remove leading and trailing spaces 
{
    // Seach spaces at the beginning
    String::size_type nFirst = strText.find_first_not_of(s_spaces);
    if(nFirst == String::npos)
    {
        // Only spaces. Empty the string
        strText.erase();
        return;
    }

    if(nFirst != 0) // Some spaces at the beginning. Remove them
        strText.erase(0, nFirst);

    // Search spaces at the end
    String::size_type nLast = strText.find_last_not_of(s_spaces);
   if(nLast < strText.size() - 1) // Some spaces at the end. Remove them
        strText.erase(nLast+1);
}

// ===================================================================
// Single node in the document tree - Abstract class
// ===================================================================

Node::Node(NodeContainer& parent, const String& strName)
:   m_strName(strName),
    m_parent(parent)
{
}

Node::~Node()
{
}

bool Node::IsKindOf(NODE_TYPE nType) const // virtual
{
    return(false);
}

void Node::Delete()
{
    m_parent.DeleteChild(self);
}

// ===================================================================
// Text - Textual content
// ===================================================================

Text::Text(const String& strText)
:   Node(Node::null, TEXT("#text")),
    m_strText(strText)
{
}

Text::Text(NodeContainer& parent, const String& strText)
:   Node(parent, TEXT("#text")),
    m_strText(strText)
{
}

String Text::GetData() const // virtual
{
    // TODO: Normalize Text
    return(m_strText);
}

String Text::GetValue() const // virtual
{
    // TODO: Normalize Text
    return(m_strText);
}

bool Text::IsKindOf(NODE_TYPE nType) const // virtual
{
    return(nType == TYPE ? true : Super::IsKindOf(nType));
}

Text* Text::Clone(NodeContainer& parent) const
{
    return(new Text(parent, m_strText));
}

Node* Text::CloneNode(NodeContainer& parent) const
{
    return(Clone(parent));
}

// ===================================================================
// NodeContainer - Contains other nodes (Element and Attribute)
// ===================================================================

NodeContainer::NodeContainer(NodeContainer& parent, const String& strName)
:   Node(parent, strName),
    m_pLastText(NULL)
{
}

NodeContainer::~NodeContainer()
{
    for(Nodes::iterator itChild = m_children.begin(); itChild < m_children.end(); ++itChild)
        delete *itChild;
}

bool NodeContainer::IsEmpty() const
{
    return(m_children.empty());
}

void NodeContainer::DeleteChildren()
{
    for(Nodes::iterator it = m_children.begin(); it < m_children.end(); ++it)
        delete (*it);

    m_children.clear();
}

void NodeContainer::CloneChildren(NodeContainer& node) const
{
    // Clone the children and associate them with the new node
    for(Nodes::const_iterator itChild = m_children.begin(); itChild < m_children.end(); ++itChild)
        node.Add((*itChild)->CloneNode(node));
}

void NodeContainer::Add(Node* pNode)
// Don't call this directly if you want to merge Texts.
// Instead call AddText
{
    ASSERT(this == &pNode->GetParent());

    m_children.push_back(pNode);
    if(!pNode->IsKindOf(TEXT_NODE))
        m_pLastText = NULL;
}

void NodeContainer::AddText(const String& strText)
// Add some text to the Node.
// Check if the previous Node was also a Text. If so, concatenate the text.
// Then, if neccessary, normalize the text (remove duplicated white spaces).
{
    if(m_pLastText != NULL)
        m_pLastText->Concatenate(strText);
    else
        Add(new Text(self, strText));
}

void NodeContainer::AddText(Bookmark& bookmark, int nNbCharToSkip)
// Add some text to the Node.
// Check if the previous Node was also a Text. If so, concatenate the text.
// Then normalize the text (remove duplicated white spaces).
{
    String strText;
    bookmark.GetSubString(strText, nNbCharToSkip);
    bookmark.Reset();

    AddText(strText);
}

void NodeContainer::InsertBefore(const Node& node, Node* pNode)
{
    Nodes::iterator it = m_children.begin();
    if(!FindChild(node, it))
    {
        ASSERT(false);
        return;
    }

    m_children.insert(it, pNode);
}

void NodeContainer::InsertAfter(const Node& node, Node* pNode)
{
    Nodes::iterator it = m_children.begin();
    if(!FindChild(node, it))
    {
        ASSERT(false);
        return;
    }

    ASSERT(it != m_children.end());
    m_children.insert(++it, pNode);
}

void NodeContainer::DeleteChild(Nodes::iterator it)
{
    ASSERT(it != m_children.end());
    delete *it;
    m_children.erase(it);
}

void NodeContainer::DeleteChild(const Node& node)
{
    Nodes::iterator it = m_children.begin();
    if(!FindChild(node, it))
    {
        ASSERT(false);
        return;
    }

    DeleteChild(it);
}

Node& NodeContainer::GetChild(int nIndex) const
// Get the nth child
{
    if(nIndex < 0 || nIndex >= m_children.size())
        return(Node::null);
    return(*m_children[nIndex]);
}

Node& NodeContainer::GetChild(int nIndex, NODE_TYPE nType) const
// Get the nth child
{
    // Start with the first child
    Nodes::const_iterator it = m_children.begin();
    // Count the number of match to skip (less 1)
    // Bug#0001: was ++nIndex instead of ++nCount
    for(int nCount = 0; nCount < nIndex; ++nCount)
    {
        // Search a child named szName
        if(!FindChild(nType, it))
            return(null);
        // Next child
        ++it;
    }

    // Last search (we skip nIndex - 1 children)
    if(!FindChild(nType, it))
        return(null);
    // Return the child
    return(**it);
}

Node& NodeContainer::GetChild(const Char* szName, int nIndex, NODE_TYPE nType) const
// Get the nth child called szName and with the given type
{
    // Start with the first child
    Nodes::const_iterator it = m_children.begin();
    // Count the number of match to skip (less 1)
    // Bug#0001: was ++nIndex instead of ++nCount
    for(int nCount = 0; nCount < nIndex; ++nCount)
    {
        // Search a child named szName
        if(!FindChild(szName, nType, it))
            return(null);
        // Next child
        ++it;
    }

    // Last search (we skip nIndex - 1 children)
    if(!FindChild(szName, nType, it))
        return(null);
    // Return the child
    return(**it);
}

bool NodeContainer::FindChild(NODE_TYPE nType, Nodes::iterator& it) const
// Find a child of the given type starting at it
{
    // Simply look at each child
    for(; it < m_children.end(); ++it)
    {
        if((*it)->IsKindOf(nType)) // Same type ?
            return(true);
    }

    return(false);
}

bool NodeContainer::FindChild(const Char* szName, Nodes::iterator& it) const
// Find a child called szName starting at it
{
    // Simply look at each child
    for(; it < m_children.end(); ++it)
    {
        if((*it)->GetName() == szName) // Same name ?
            return(true);
    }

    return(false);
}

bool NodeContainer::FindChild(const Char* szName, NODE_TYPE nType, Nodes::iterator& it) const
// Find a child called szName starting at it
{
    // Simply look at each child
    for(; it < m_children.end(); ++it)
    {
        if((*it)->GetName() == szName && (*it)->IsKindOf(nType)) // Same name, same type ?
            return(true);
    }

    return(false);
}

bool NodeContainer::FindChild(const Node& node, Nodes::iterator& it) const
// Find a given child starting at it
{
    // Simply look at each child
    for(; it < m_children.end(); ++it)
    {
        if(*it == &node) // Same type ?
            return(true);
    }

    return(false);
}

bool NodeContainer::FindChild(NODE_TYPE nType, Nodes::const_iterator& it) const
// Find a child of the given type starting at it
{
    // Simply look at each child
    for(; it < m_children.end(); ++it)
    {
        if((*it)->IsKindOf(nType)) // Same type ?
            return(true);
    }

    return(false);
}

bool NodeContainer::FindChild(const Char* szName, Nodes::const_iterator& it) const
// Find a child called szName starting at it
{
    // Simply look at each child
    for(; it < m_children.end(); ++it)
    {
        if((*it)->GetName() == szName) // Same name ?
            return(true);
    }

    return(false);
}

bool NodeContainer::FindChild(const Char* szName, NODE_TYPE nType, Nodes::const_iterator& it) const
// Find a child called szName starting at it
{
    // Simply look at each child
    for(; it < m_children.end(); ++it)
    {
        if((*it)->GetName() == szName && (*it)->IsKindOf(nType)) // Same name, same type ?
            return(true);
    }

    return(false);
}

bool NodeContainer::FindChild(const Node& node, Nodes::const_iterator& it) const
// Find a given child starting at it
{
    // Simply look at each child
    for(; it < m_children.end(); ++it)
    {
        if(*it == &node) // Same type ?
            return(true);
    }

    return(false);
}

// ===================================================================
// Attribute of Elements
// ===================================================================

Attribute::Attribute(NodeContainer& parent, const String& strName)
:   NodeContainer(parent, strName)
{
}

String Attribute::GetData() const // virtual
{
    return(GetValue());
}

String Attribute::GetValue() const // virtual
{
    String strValue;
    for(Nodes::const_iterator itChild = m_children.begin(); itChild < m_children.end(); ++itChild)
        strValue += (*itChild)->GetValue();

    // Remove spaces at the beginning and at the end
    TrimSpaces(strValue);
    return(strValue);
}

bool Attribute::IsKindOf(NODE_TYPE nType) const // virtual
{
    return(nType == TYPE ? true : Super::IsKindOf(nType));
}

Attribute* Attribute::Clone(NodeContainer& parent) const
{
    // Clone the attribute itself
    auto_ptr<Attribute> pAttribute(new Attribute(parent, GetName()));
    // Then clone its children and associate them with the new attribute
    CloneChildren(*pAttribute);

    return(pAttribute.release());
}

Node* Attribute::CloneNode(NodeContainer& parent) const
{
    return(Clone(parent));
}

// ===================================================================
// References
// ===================================================================

Reference::Reference(NodeContainer& parent, const String& strName)
:   Node(parent, strName)
{
}

bool Reference::IsKindOf(NODE_TYPE nType) const // virtual
{
    return(nType == TYPE ? true : Super::IsKindOf(nType));
}

// ===================================================================
// Character Reference
// ===================================================================

CharRef::CharRef(int nNum)
:   Reference(Node::null, TEXT("")),
    m_c(nNum)
{
}

CharRef::CharRef(NodeContainer& parent, int nNum)
:   Reference(parent, TEXT("")),
    m_c(nNum)
{
}

String CharRef::GetData() const // virtual
{
    return(String(1, m_c));
}

String CharRef::GetValue() const // virtual
{
    return(String(1, m_c));
}

CharRef* CharRef::Clone(NodeContainer& parent) const
{
    return(new CharRef(parent, m_c));
}

Node* CharRef::CloneNode(NodeContainer& parent) const
{
    return(Clone(parent));
}

// ===================================================================
// Entity Reference
// ===================================================================

EntityRef::EntityRef(const String& strName)
:   Reference(Node::null, strName),
    m_c(0)
{
    if(!MapReferenceName())
        throw InvalidRefException(strName);
}

EntityRef::EntityRef(NodeContainer& parent, const String& strName)
:   Reference(parent, strName),
    m_c(0)
{
    MapReferenceName();
    // TODO: Make something if the ref. is unknown
}

EntityRef* EntityRef::Clone(NodeContainer& parent) const
{
    auto_ptr<EntityRef> pEntity(new EntityRef(GetName()));
    pEntity->m_c = m_c;
    return(pEntity.release());
}

Node* EntityRef::CloneNode(NodeContainer& parent) const
{
    return(Clone(parent));
}

String EntityRef::GetData() const // virtual
{
    return(String(1, m_c));
}

String EntityRef::GetValue() const // virtual
{
    return(String(1, m_c));
}

MapReferences s_mapReferences;

void FillMap()
// Must be called somewhere before using MapReferenceName
{
    s_mapReferences[TEXT("lt")]     = TEXT('<');
    s_mapReferences[TEXT("gt")]     = TEXT('>');
    s_mapReferences[TEXT("amp")]    = TEXT('&');
    s_mapReferences[TEXT("apos")]   = TEXT('\'');
    s_mapReferences[TEXT("quot")]   = TEXT('\"');
};

bool EntityRef::MapReferenceName()
// Find the reference strName and return its equivalent
{
    if(s_mapReferences.empty())
        FillMap();

    MapReferences::const_iterator it = s_mapReferences.find(m_strName);
    if(it == s_mapReferences.end())
        return(false);

    m_c = (*it).second; // I'm not sure that it->second works with every compiler
    return(true);
}


// ===================================================================
// Elements (Markups) - Abstract class
// ===================================================================

Element::Element(const String& strName)
:   NodeContainer(Node::null, strName),
    m_bPreserveWS(false)
{
}

Element::Element(NodeContainer& parent, const String& strName)
:   NodeContainer(parent, strName),
    m_bPreserveWS(false)
{
}

Element::~Element()
{
    // Destroy attributes
    for(Attributes::iterator itAttrib = m_attributes.begin(); itAttrib < m_attributes.end(); ++itAttrib)
        delete *itAttrib;
}

String Element::GetData() const // virtual
{
    return(TEXT(""));
}

String Element::GetValue() const // virtual
{
    String strValue;
    for(Nodes::const_iterator itChild = m_children.begin(); itChild < m_children.end(); ++itChild)
        strValue += (*itChild)->GetValue();

    if(!m_bPreserveWS)
        Normalize(strValue);

    return(strValue);
}

void Element::Normalize(String& strText) // static
{
    NormalizeWhiteSpaces(strText);
    TrimSpaces(strText);
}

bool Element::IsKindOf(NODE_TYPE nType) const // virtual
{
    return(nType == TYPE ? true : Super::IsKindOf(nType));
}

Element* Element::Clone(NodeContainer& parent) const
{
    // Clone the element itself
    auto_ptr<Element> pElement(new Element(parent, GetName()));
    // Then clone its children and associate them with the new element
    CloneChildren(*pElement);

    return(pElement.release());
}

Node* Element::CloneNode(NodeContainer& parent) const
{
    return(Clone(parent));
}

const Attribute& Element::GetAttribute(const Char* szName) const
{
    for(Attributes::const_iterator it = m_attributes.begin(); it < m_attributes.end(); ++it)
    {
        if((*it)->GetName() == szName)
            return(**it);
    }

    return(Attribute::null);
}

Attribute& Element::GetAttribute(const Char* szName)
{
    for(Attributes::const_iterator it = m_attributes.begin(); it < m_attributes.end(); ++it)
    {
        if((*it)->GetName() == szName)
            return(**it);
    }

    return(Attribute::null);
}

const Attribute& Element::GetAttribute(int nIndex) const
{
    if(nIndex < 0 || nIndex >= m_attributes.size())
        return(Attribute::null);
    return(*m_attributes[nIndex]);
}

Attribute& Element::GetAttribute(int nIndex)
{
    if(nIndex < 0 || nIndex >= m_attributes.size())
        return(Attribute::null);
    return(*m_attributes[nIndex]);
}

// ===================================================================

Attribute& Element::CreateAttribute(const Char* szName)
{
    if(IsNull())
        return(Attribute::null);

    Attribute& attr = GetAttribute(szName);
    if(!attr.IsNull())
        return(attr);

    auto_ptr<Attribute> pNewAttr(new Attribute(self, szName));
    Attribute* pAttr = pNewAttr.get();
    m_attributes.push_back(pNewAttr.release());
    return(*pAttr);
}

// ===================================================================

Element& Element::CreateElement(const Char* szName, int nIndex)
{
    if(IsNull())
        return(null);

    for(;;)
    {
        Element& elem = GetElement(szName, nIndex);
        if(!elem.IsNull())
            return(elem);

        Add(new Element(self, szName));
    }

    // Never reached
    return(null);
}

Element& Element::AddElementInto(const Char* szName)
// Add an Element as a sub-node
{
    if(IsNull())
        return(null);

    Element* pElem = new Element(self, szName);
    Add(pElem);
    return(*pElem);
}

Element& Element::AppendElement(const Char* szName)
// Add an Element after this element and all other nodes (sibling)
{
    if(IsNull())
        return(null);

    NodeContainer& parent = GetParent();
    ASSERT(!parent.IsNull());

    Element* pElem = new Element(parent, szName);
    parent.Add(pElem);
    return(*pElem);
}

Element& Element::InsertElementBefore(const Char* szName)
// Add an Element before this element (sibling)
{
    if(IsNull())
        return(null);

    NodeContainer& parent = GetParent();
    ASSERT(!parent.IsNull()); // Impossible

    Element* pElem = new Element(parent, szName);
    parent.InsertBefore(self, pElem);
    return(*pElem);
}

Element& Element::InsertElementAfter(const Char* szName)
// Add an Element after this element (sibling)
{
    if(IsNull())
        return(null);

    NodeContainer& parent = GetParent();
    ASSERT(!parent.IsNull()); // Impossible

    Element* pElem = new Element(parent, szName);
    parent.InsertAfter(self, pElem);
    return(*pElem);
}

// ===================================================================

bool Element::DeleteChildElement(const Char* szName, int nIndex)
{
    Nodes::iterator it = m_children.begin();
    for(int nCount = 0; nCount <= nIndex; ++nCount)
    {
        if(!FindChild(szName, ELEMENT_NODE, it))
            return(false);
    }

    Super::DeleteChild(it);
    return(true);
}

// ===================================================================

const Comment& Element::GetComment(int nIndex) const
{
    Nodes::const_iterator it = m_children.begin();
    for(int nCount = 0; nCount <= nIndex; ++nCount)
    {
        if(!FindChild(COMMENT_NODE, it))
            return(Comment::null);
    }

    return(static_cast<const Comment&>(**it));
}

Comment& Element::GetComment(int nIndex)
{
    Nodes::iterator it = m_children.begin();
    for(int nCount = 0; nCount <= nIndex; ++nCount)
    {
        if(!FindChild(COMMENT_NODE, it))
            return(Comment::null);
    }

    return(static_cast<Comment&>(**it));
}

// ===================================================================

const CData& Element::GetCData(int nIndex) const
{
    Nodes::const_iterator it = m_children.begin();
    for(int nCount = 0; nCount <= nIndex; ++nCount)
    {
        if(!FindChild(CDATA_SECTION_NODE, it))
            return(CData::null);
    }

    return(static_cast<const CData&>(**it));
}

CData& Element::GetCData(int nIndex)
{
    Nodes::iterator it = m_children.begin();
    for(int nCount = 0; nCount <= nIndex; ++nCount)
    {
        if(!FindChild(CDATA_SECTION_NODE, it))
            return(CData::null);
    }

    return(static_cast<CData&>(**it));
}

// ===================================================================

const Pi& Element::GetPi(const Char* szTarget, int nIndex) const
{
    Nodes::const_iterator it = m_children.begin();
    for(int nCount = 0; nCount <= nIndex; ++nCount)
    {
        if(!FindChild(szTarget, PROCESSING_INSTRUCTION_NODE, it))
            return(Pi::null);
    }

    return(static_cast<const Pi&>(**it));
}

const Pi& Element::GetPi(int nIndex) const
{
    Nodes::const_iterator it = m_children.begin();
    for(int nCount = 0; nCount <= nIndex; ++nCount)
    {
        if(!FindChild(PROCESSING_INSTRUCTION_NODE, it))
            return(Pi::null);
    }

    return(static_cast<const Pi&>(**it));
}

Pi& Element::GetPi(const Char* szTarget, int nIndex)
{
    Nodes::iterator it = m_children.begin();
    for(int nCount = 0; nCount <= nIndex; ++nCount)
    {
        if(!FindChild(szTarget, PROCESSING_INSTRUCTION_NODE, it))
            return(Pi::null);
    }

    return(static_cast<Pi&>(**it));
}

Pi& Element::GetPi(int nIndex)
{
    Nodes::iterator it = m_children.begin();
    for(int nCount = 0; nCount <= nIndex; ++nCount)
    {
        if(!FindChild(PROCESSING_INSTRUCTION_NODE, it))
            return(Pi::null);
    }

    return(static_cast<Pi&>(**it));
}

// ===================================================================
// Comment
// ===================================================================

Comment::Comment(const String& strComment)
:   Node(Node::null, TEXT("#comment")),
    m_strComment(strComment)
{
}

Comment::Comment(NodeContainer& parent, const String& strComment)
:   Node(parent, TEXT("#comment")),
    m_strComment(strComment)
{
}

String Comment::GetData() const // virtual
{
    return(m_strComment);
}

String Comment::GetValue() const // virtual
{
    return(m_strComment);
}

bool Comment::IsKindOf(NODE_TYPE nType) const // virtual
{
    return(nType == TYPE ? true : Super::IsKindOf(nType));
}

Comment* Comment::Clone(NodeContainer& parent) const
{
    return(new Comment(parent, GetData()));
}

Node* Comment::CloneNode(NodeContainer& parent) const
{
    return(Clone(parent));
}

// ===================================================================
// Processing Instruction (PI)
// ===================================================================

Pi::Pi(const String& strTarget, const String& strInstruction)
:   Node(Node::null, strTarget),
    m_strInstruction(strInstruction)
{
}

Pi::Pi(NodeContainer& parent, const String& strTarget, const String& strInstruction)
:   Node(parent, strTarget),
    m_strInstruction(strInstruction)
{
}

String Pi::GetData() const // virtual
{
    return(m_strInstruction);
}

String Pi::GetValue() const // virtual
{
    return(m_strInstruction);
}

bool Pi::IsKindOf(NODE_TYPE nType) const // virtual
{
    return(nType == TYPE ? true : Super::IsKindOf(nType));
}

Pi* Pi::Clone(NodeContainer& parent) const
{
    return(new Pi(parent, GetName(), GetData()));
}

Node* Pi::CloneNode(NodeContainer& parent) const
{
    return(Clone(parent));
}


// ===================================================================
// CDATA Section
// ===================================================================

CData::CData(const String& strSection)
:   Node(Node::null, TEXT("#cdata-section")),
    m_strSection(strSection)
{
}

CData::CData(NodeContainer& parent, const String& strSection)
:   Node(parent, TEXT("#cdata-section")),
    m_strSection(strSection)
{
}

String CData::GetData() const // virtual
{
    return(m_strSection);
}

String CData::GetValue() const // virtual
{
    return(m_strSection);
}

bool CData::IsKindOf(NODE_TYPE nType) const // virtual
{
    return(nType == TYPE ? true : Super::IsKindOf(nType));
}

CData* CData::Clone(NodeContainer& parent) const
{
    return(new CData(parent, GetData()));
}

Node* CData::CloneNode(NodeContainer& parent) const
{
    return(Clone(parent));
}

// ===================================================================
// Document
// ===================================================================

// warning C4355: 'this' : used in base member initializer list. 
//
// Normally, it would be dangerous for the contained-object 
// constructor to access the container class because the contained 
// objects are initialized before the container is initialized. 
// We don't do that here, and it's convient to pass a pointer 
// to the container at construction time, so turn off the warning. 
#ifdef _MSC_VER // Microsoft specific
#pragma warning(disable : 4355)
#endif

// Note: a Document has no parent (i.e. Node::null)

Document::Document()
:   NodeContainer(Node::null, TEXT("#document")),
    m_strXmlVersion(TEXT("1.0")),   // By default
    m_strEncoding(TEXT("UTF-8")),   // By default
    m_bStandalone(true),            // By default
#if ADVXMLPARSER_DTD != 0
    m_dtd(self),
#endif // ADVXMLPARSER_DTD
    m_pRoot(NULL)
{
}

Document::Document(const Char* szRootName)
:   NodeContainer(Node::null, TEXT("#document")),
    m_strXmlVersion(TEXT("1.0")),   // By default
    m_strEncoding(TEXT("UTF-8")),   // By default
    m_bStandalone(true),            // By default
#if ADVXMLPARSER_DTD != 0
    m_dtd(self),
#endif // ADVXMLPARSER_DTD
    m_pRoot(new Element(self, szRootName))
{
    Add(m_pRoot);
}

#ifdef _MSC_VER
#pragma warning(default : 4355)
#endif

String Document::GetData() const // virtual
{
    return(TEXT(""));
}

String Document::GetValue() const // virtual
{
    String strValue;
    for(Nodes::const_iterator itChild = m_children.begin(); itChild < m_children.end(); ++itChild)
        strValue += (*itChild)->GetValue();

    NormalizeWhiteSpaces(strValue);
    TrimSpaces(strValue);

    return(strValue);
}

bool Document::IsKindOf(NODE_TYPE nType) const // virtual
{
    return(nType == TYPE ? true : Super::IsKindOf(nType));
}

Document* Document::Clone() const
{
    // Clone the element itself
    auto_ptr<Document> pDocument(new Document);
    // Then clone its children and associate them with the new element
    CloneChildren(*pDocument);

    return(pDocument.release());
}

Node* Document::CloneNode(NodeContainer& parent) const
{
    ASSERT(parent.IsNull());
    return(Clone());
}

// ===================================================================
// Exception
// ===================================================================

Exception::Exception()
{
}

Exception::~Exception()
{
}

// ===================================================================
// Invalid reference
// ===================================================================

InvalidRefException::InvalidRefException(const String& strName)
:   m_strName(strName)
{
}

// ===================================================================

} // End of namespace AdvXMLParser

// ===================================================================
// Some debugging stuff
// ===================================================================

#if defined(DEBUG) || defined(_DEBUG)

#ifdef _MSC_VER

void BreakInDebugger()
{
    // If you reach this point, this is because an ASSERTion as failed.
    // Look in the stack trace for the previous function or simply step
    // out of this function.

    __asm
    {
        int 3
    }
}

#endif // _MSC_VER

#endif // DEBUG || _DEBUG

