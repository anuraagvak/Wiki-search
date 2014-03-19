#ifndef SAMPLE_CLASSES_H
#define SAMPLE_CLASSES_H

/*
This file is part of the sample code for the article,
"Processing XML with Xerces and SAX" by Ethan McCallum (2005/11/10)

Published on ONLamp.com (http://www.onlamp.com/)
http://www.onlamp.com/pub/a/onlamp/2005/11/10/xerces_sax.html
*/


#include<xercesc/sax2/Attributes.hpp>
#include<xercesc/sax2/ContentHandler.hpp>
#include<xercesc/sax2/DefaultHandler.hpp>
#include<xercesc/sax2/SAX2XMLReader.hpp>
#include<xercesc/sax2/XMLReaderFactory.hpp>

#include<xercesc/util/PlatformUtils.hpp>
#include<xercesc/util/XMLString.hpp>
#include<xercesc/util/XMLUni.hpp>

#include<helper-classes.h>

#include<iostream>
#include<map>
#include<string>
// - - - - - - - - - - - - - - - - - - - -

class SimpleContentHandler : public xercesc::ContentHandler {

	public:
	SimpleContentHandler() ;
	void startElement(
		const XMLCh* const uri ,
		const XMLCh* const localname ,
		const XMLCh* const qname ,
		const xercesc::Attributes& attrs
	) ;

	void endElement(
		const XMLCh* const uri ,
		const XMLCh* const localname ,
		const XMLCh* const qname
	) ;

	void characters (const XMLCh *const chars, const unsigned int length) ;

	void ignorableWhitespace (const XMLCh *const chars, const unsigned int length) ;

	void processingInstruction (const XMLCh *const target, const XMLCh *const data) ;

	void resetDocument () ;

	void setDocumentLocator (const xercesc::Locator *const locator) ;

	void startDocument () ;

	void endDocument () ;

	void startPrefixMapping( const XMLCh *const prefix, const XMLCh *const uri ) ;

	void endPrefixMapping( const XMLCh *const prefix ) ;

	void skippedEntity( const XMLCh *const name ) ;

	protected:
	// empty

	private:
	// empty

} ; // class SimpleContentHandler

// - - - - - - - - - - - - - - - - - - - -

class RPMInfo {

	public:
	RPMInfo() ;

	const std::string& getName() const throw() ;

	void setName( const std::string& in ) ;

	const std::string& getVersion() const throw() ;

	void setVersion( const std::string& in ) ;

	const std::string& getLicense() const throw() ;

	void setLicense( const std::string& in ) ;

	const std::string& getVendor() const throw() ;

	void setVendor( const std::string& in ) ;

	std::ostream& print( std::ostream& s ) const ;

	protected:
	// empty


	private:
	std::string name_ ;
	std::string version_ ;
	std::string license_ ;
	std::string vendor_ ;

} ; // class RPMInfo


std::ostream& operator<<( std::ostream& s , const RPMInfo& obj ) ;

// - - - - - - - - - - - - - - - - - - - -

// ... instead of a full-blown, custom class...

/*
This is one way to handle objects created during a
SAX run: collect them in a list, for future processing.

Another way would be to process the objects as they
are fully instantiated (e.g. in the end-tag function).
For example, you can do this if you just want to print
the data, or if you pass the objects you create to a 
queueing system in another thread.
*/

#include<list>
typedef std::list< const RPMInfo* > RPMList ;


// - - - - - - - - - - - - - - - - - - - -

#include<functional>
struct ShowRPMInfo : public std::unary_function< void , const RPMInfo* >
{

	public:
	void operator()( const RPMInfo* obj ){
		std::cout << *obj << std::endl ;
	}

} ; // struct ShowRPMInfo


// - - - - - - - - - - - - - - - - - - - -

class TagNames {

	public:

	const DualString TAG_PACKAGE ;
	const DualString TAG_NAME ;
	const DualString TAG_VERSION ;
	const DualString TAG_LICENSE ;
	const DualString TAG_VENDOR ;

	const DualString ATTR_VER ;
	const DualString ATTR_REL ;

	const DualString URI_RPM ;
	const DualString URI_METADATA ;

	TagNames() ;

	/*
	notice, there's no explicit dtor -- DualString's
	dtor disposes of any loose XMLCh* strings without
	manual intervention.
	*/

} ; // class TagNames

// - - - - - - - - - - - - - - - - - - - -

#include<stack>
#include<sstream>

/*

Note the use of DefaultHandler instead of ContentHandler --
this class has no-op implementations of the member functions
defined by the ContentHandler interface.  In turn, then, this
code only has to override those member functions relevant to
its purpose.

*/
class RepodataContentHandler : public xercesc::DefaultHandler {

	public:

	RepodataContentHandler( RPMList& rpms ) ;

	void startElement(
		const XMLCh* const uri ,
		const XMLCh* const localName ,
		const XMLCh* const qName ,
		const xercesc::Attributes& attrs
	) ;


	void endElement(
		const XMLCh* const uri ,
		const XMLCh* const localName ,
		const XMLCh* const qName
	) ;


	void characters (const XMLCh *const chars, const unsigned int length) ;

	protected:
	// empty


	private:

	enum {
		TAG_UNKNOWN = 0 ,
		TAG_METADATA_PACKAGE  ,
		TAG_METADATA_NAME     ,
		TAG_METADATA_VERSION  ,
		TAG_RPM_LICENSE  ,
		TAG_RPM_VENDOR
	} ;


	const TagNames tagNames_ ;
	std::stack<int> tagStack_ ;
	RPMList& rpms_ ;
	RPMInfo* current_ ;
	std::ostringstream charBuf_ ;


	void pushTag(
		const XMLCh* const uri ,
		const XMLCh* const localName
	) ;

	void popTag() ;

	int currentTag() ;

} ; // class RepodataContentHandler

// - - - - - - - - - - - - - - - - - - - -

#include<xercesc/sax/ErrorHandler.hpp>

class SimpleErrorHandler : public xercesc::ErrorHandler {

	public:
	SimpleErrorHandler() ;

	void warning( const xercesc::SAXParseException& e ) ;

	void error( const xercesc::SAXParseException& e ) ;

	void fatalError( const xercesc::SAXParseException& e ) ;

	void resetErrors() ;

	std::ostream& print( std::ostream& s ) const ;

	protected:
	// empty

	private:
	int warningCount_ ;
	int errorCount_ ;
	int fatalErrorCount_ ;

} ;// class SimpleErrorHandler


std::ostream& operator<<( std::ostream& s , const SimpleErrorHandler& obj ) ;

// - - - - - - - - - - - - - - - - - - - -

#include<xercesc/sax/ErrorHandler.hpp>

class CountingErrorHandler : public xercesc::ErrorHandler {

	public:
	CountingErrorHandler() ;

	void warning( const xercesc::SAXParseException& e ) ;

	void error( const xercesc::SAXParseException& e ) ;

	void fatalError( const xercesc::SAXParseException& e ) ;

	void resetErrors() ;

	std::ostream& print( std::ostream& s ) const ;


	protected:
	// empty

	private:
	int warningCount_ ;
	int errorCount_ ;
	int fatalErrorCount_ ;

} ;// class CountingErrorHandler

std::ostream& operator<<( std::ostream& s , const CountingErrorHandler& obj ) ;

// - - - - - - - - - - - - - - - - - - - -


#include<functional>

/**
\brief
Dereferences <TT>const XMLCh*</TT> pointers before comparing them.

blatantly sto^H^H^H based on Meyers/STL, #20
*/

struct CompareDualString
	: public std::binary_function< DualString* , DualString* , bool >
{

	bool operator()( DualString* str1 , DualString* str2 ) const throw() {

		return( 0 > xercesc::XMLString::compareString( str1->asXMLString() , str2->asXMLString() ) ) ;
	
	} // CompareDualString::operator()()


} ; // struct CompareDualString


// - - - - - - - - - - - - - - - - - - - -

#include<map>
#include<stdexcept>
#include<xercesc/sax/EntityResolver.hpp>
#include<xercesc/sax/InputSource.hpp>
#include<xercesc/framework/LocalFileInputSource.hpp>
#include<xercesc/framework/MemBufInputSource.hpp>
#include<xercesc/framework/StdInInputSource.hpp>
#include<xercesc/framework/URLInputSource.hpp>

/**
\brief custom EntityResolver that may be populated by client code

NOTE: there is an ownership concerning the XMLCh pointers used as
the internal map<> keys: should they be owned (and thus destroyed)
by the EntityResolver, or by client code?

*/

class SimpleEntityResolver : public xercesc::EntityResolver {

	public:
	SimpleEntityResolver() ;

	~SimpleEntityResolver() throw() ;

	void add( const char* const systemID , const char* const file ) throw( std::runtime_error ) ;

	xercesc::InputSource * resolveEntity (
		const XMLCh* const publicId ,
		const XMLCh* const systemId
	) ;


	protected:
	// empty


	private:
	typedef std::map< DualString* , DualString* , CompareDualString > MapType ;

	MapType _entities ;

} ; // class SimpleEntityResolver


// - - - - - - - - - - - - - - - - - - - -

#endif // #ifndef SAMPLE_CLASSES_H
