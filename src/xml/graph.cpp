// $Id$
// This sample demonstrates the use of SXP for persisting more complex
// data structures, consisting of a lot of objects referencing each other.
// Object references are handled via storing unique object ID's in the
// XML file; on loading, the objects reference each other via these ID's;
// after everything is loaded, they are replaced with "real" references -
// the root object which is loaded from the file keeps track of the
// ID-object correspondence

// The data structure we use for an example is a graph.
// Each node in the graph contains a list of pointers to the nodes
// it is linked to; the graph itself consists of a list of nodes.

#include "xml/sxp.h"

#include <list>
#include <vector>
#include <map>

// sprintf
#include <stdio.h>
class CNode;

// this is used to resolve object pointers from integer IDs stored in the
// XML file - see below for clarification
template <typename T>
struct IResolver {
	virtual T *operator() (int ID) = 0;
};

typedef std::list<CNode *> NodePtrList;
typedef std::vector<CNode *> NodePtrVect;

// The graph node

class CNode :
public SXP::IPersistObj {
public:
	std::string m_strName;
	NodePtrList m_Neighbors;

	// references will be handled the following way:
	// on reading, instead of the normal list of CNode *'s m_Neighbors
	// another list, of ints, will be created, with the ID's of the
	// neighbours. After the entire graph is read, this list will
	// be translated into the normal CNode * list using the translation
	// function CGraph::GetNodeByID; after the translation, the temporary 
	// list of will be discarded - that's why its a list*, not a list
	std::list<int> *m_pNeighborIDs;

	CNode(const char *pchName = 0) {
		if( pchName )
			m_strName = pchName;
		m_pNeighborIDs = 0;
	}

	void PrintOut() {
		printf("Node %s is connected to nodes ", m_strName.c_str());
		for(NodePtrList::iterator i=m_Neighbors.begin(); i!=m_Neighbors.end(); i++) {
			printf("%s ", (*i)->m_strName.c_str());
		}
		printf("\n");
	}

	void ConnectTo(CNode *pOther) {
		m_Neighbors.push_back(pOther);
	}

	// The members needed for the SXP mechanism
	static SXP::Tag tag_name, tag_link;
	static SXP::Tag tag_own;
	SXP::Tag& GetClassTag() const    { return tag_own; }
	static SXP::Tag& s_GetClassTag() { return tag_own; }
	int GetID()                      { return (int)this; }

	void BeginElement(SXP::IParser *pIn, SXP::IElement *pElement) {}

	void EndElement(SXP::IParser *pIn, SXP::IElement *pElement) {
		if( pElement->IsA(tag_name) ) pElement->Retrieve(m_strName);
		if( pElement->IsA(tag_link) ) {
			// on reading, ID's of neighbors are stored in temp list
			int ID;
			pElement->Retrieve(ID);
			if( !m_pNeighborIDs ) {
				m_pNeighborIDs = new std::list<int>;
			}
			m_pNeighborIDs->push_back(ID);
		}
	}

	// nothing special here: store a <name ID="*"> tag for the name,
	// and one <link> tag each for neighbors' IDs
	void WriteElement(SXP::IOutStream *pOut, SXP::dict& attribs) {
		char chID[10];
		// Not all systems have ltoa...
		// ltoa( GetID(), chID, 10 );
#if HAVE_SNPRINTF
		snprintf(chID, 10, "%d", GetID());
#else /* HAVE_SNPRINTF */
		sprintf(chID, "%d", GetID());
#endif /* !HAVE_SNPRINTF */
		attribs["id"] = chID;
		pOut->BeginObject(tag_own, attribs);

		pOut->WriteElement(tag_name, m_strName);
		for(NodePtrList::iterator i=m_Neighbors.begin(); i!=m_Neighbors.end(); i++) {
			pOut->WriteElement(tag_link, (*i)->GetID() );
		}

		pOut->EndObject(tag_own);
	}

	// translate integer IDs from temporary list m_pNeighborIDs to
	// CNode pointers, which are stored in the "real" neighbors list
	// m_Neighbors
	void ResolveRefs(IResolver<CNode>& pgr) {
		for(std::list<int>::iterator i=m_pNeighborIDs->begin(); i!=m_pNeighborIDs->end(); i++) {
			m_Neighbors.push_back( pgr(*i) );
		}
		delete m_pNeighborIDs;
	}
};

SXP::Tag CNode::tag_name("name");
SXP::Tag CNode::tag_link("link");
SXP::Tag CNode::tag_own ("node");

class CGraph : 
	public IResolver<CNode>,
		public SXP::IPersistObj
{
public:
	NodePtrVect m_Nodes;

	// the ID-to-CNode* correspondence is stored in this STL map;
	// after all references are resolved, it's not needed anymore
	// and is discarded
	std::map<int, CNode *> *m_pIDMap;

	CGraph() { m_pIDMap = 0; }

	~CGraph() {
		for(NodePtrVect::iterator i=m_Nodes.begin(); i!=m_Nodes.end(); i++) {
			delete (*i);
		}
	}

	void PrintOut() {
		for(NodePtrVect::iterator i=m_Nodes.begin(); i!=m_Nodes.end(); i++) {
			(*i)->PrintOut();
		}
	}

	void AddNode(CNode *p) {
		m_Nodes.push_back(p);
	}

	void AddNode(const char *pchName) {
		CNode *pNode = new CNode(pchName);
		AddNode(pNode);
	}

	// this code allows several connections between two nodes
	// after all, the point here is not to demonstrate how to write
	// a good graph data structure :-)
	void ConnectNodes(int n1, int n2) {
		if( n1 >= m_Nodes.size() )	return;
		if( n2 >= m_Nodes.size() )	return;
		if( n1 == n2 ) return;
		m_Nodes[n1]->ConnectTo( m_Nodes[n2] );
		m_Nodes[n2]->ConnectTo( m_Nodes[n1] );
	}

	// The members needed for the SXP mechanism
	static SXP::Tag tag_own;
	SXP::Tag& GetClassTag() const { return tag_own; }

	void BeginElement(SXP::IParser *pIn, SXP::IElement *pElement) {
		if( pElement->IsA( CNode::s_GetClassTag() ) ) {
			// a node! create the ID table, if it's not already created
			if( !m_pIDMap ) {
				m_pIDMap = new std::map<int, CNode *>;
			}

			// read the node ID from the element attributes
			int ID;
			ID = atol( pElement->Attrib("id") );

			// create a new node
			CNode *pNode = new CNode;
			AddNode(pNode);

			// store ID-to-node correspondence
			(*m_pIDMap) [ ID ] = pNode;

			// make node read itself off XML stream
			pIn->ReadTo(pNode);
		}
	}

	void EndElement(SXP::IParser *pIn, SXP::IElement *pElement) {}

	// nothing special here 
	void WriteElement(SXP::IOutStream *pOut, SXP::dict& attribs) {
		pOut->BeginObject(tag_own, attribs);

		for(NodePtrVect::iterator i=m_Nodes.begin(); i!=m_Nodes.end(); i++) {
			SXP::dict attr;
			pOut->WriteSubElement((*i), attr);
		}

		pOut->EndObject(tag_own);
	}

	// this is the ID-to-node ptr resolving operator used by the
	// CNode; this is a method of the IResolver interface
	CNode *operator() (int ID) {
		return (*m_pIDMap)[ID];
	}

	// make all nodes resolve their references; they will need
	// a IResolver<CNode> instance to convert the IDs to CNode *'s;
	// the CGraph itself will serve this purpose
	void ResolveRefs() {
		for(NodePtrVect::iterator i=m_Nodes.begin(); i!=m_Nodes.end(); i++) {
			(*i)->ResolveRefs(*this);
		}
		delete m_pIDMap;
	}
};

SXP::Tag CGraph::tag_own("graph");

void main()
{
	CGraph gr;

/*
	// generate graph and connect random nodes

	char tmp[10];
	// int nNodes = 1000, nConn = 100000; // stress test
	int nNodes = 10, nConn = 30;          // gentle demo
	for(int i=0; i<nNodes; i++) {
		sprintf(tmp, "{%d}", i);
		gr.AddNode(tmp);
	}

	for(i=0; i<nConn; i++) {
		gr.ConnectNodes( rand() % nNodes, rand() % nNodes );
	}
	gr.PrintOut();
*/

	// read graph from file
	SXP::CParser p( &gr );
	p.FeedFile( "graph.xml" );
	gr.ResolveRefs();

	gr.PrintOut();


	// store graph in file

	SXP::CFileOutStream *o = new SXP::CFileOutStream("graph.xml");
	o->BeginXML();

	SXP::dict attribs;
	gr.WriteElement(o, attribs);
	delete o; // the destructor of o closes the file

}
