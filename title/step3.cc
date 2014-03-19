/*
This file is part of the sample code for the article,
"Processing XML with Xerces and SAX" by Ethan McCallum (2005/11/10)

Published on ONLamp.com (http://www.onlamp.com/)
http://www.onlamp.com/pub/a/onlamp/2005/11/10/xerces_sax.html
*/

/*
Step 3, sax2

error handler

(expected input: an RPM/yum repodata file)
*/

#include<iostream>
#include<list>
#include<stack>
#include<sstream>

#include<algorithm>
#include<functional>

#include<xercesc/sax/EntityResolver.hpp>
#include<xercesc/sax/ErrorHandler.hpp>

#include<xercesc/sax2/Attributes.hpp>
#include<xercesc/sax2/ContentHandler.hpp>
#include<xercesc/sax2/DefaultHandler.hpp>
#include<xercesc/sax2/SAX2XMLReader.hpp>
#include<xercesc/sax2/XMLReaderFactory.hpp>

#include<xercesc/util/PlatformUtils.hpp>
#include<xercesc/util/SecurityManager.hpp>
#include<xercesc/util/XMLString.hpp>
#include<xercesc/util/XMLUni.hpp>

#include<helper-classes.h>
#include<sample-classes.h>

// - - - - - - - - - - - - - - - - - - - -

enum {
	ERROR_ARGS = 1 ,
	ERROR_ALLOCATE_PARSER ,
	ERROR_CONFIGURE_PARSER ,
	ERROR_PARSE
} ;


int main( int argc , char** argv ){

	if( 2 != argc ){
		std::cerr << "Usage: " << argv[0] << " {XML file}" << std::endl ;
		return( ERROR_ARGS ) ;
	}

	try{
		xercesc::XMLPlatformUtils::Initialize();
	}catch( xercesc::XMLException& e ){
		std::cerr << "XML toolkit initialization error: "
			<< DualString( e.getMessage() )
			<< std::endl
		;
	}

	const char* xmlFile = argv[1] ;


	xercesc::SAX2XMLReader* p = xercesc::XMLReaderFactory::createXMLReader();

	RPMList list ;
	xercesc::ContentHandler* h = new RepodataContentHandler( list ) ;
	p->setContentHandler( h ) ;

	SimpleErrorHandler* e = new SimpleErrorHandler() ;
	p->setErrorHandler( e ) ;

	try{

		p->setFeature( xercesc::XMLUni::fgXercesSchema , false ) ;

		p->setFeature( xercesc::XMLUni::fgXercesValidationErrorAsFatal , false ) ;

		p->setFeature( xercesc::XMLUni::fgXercesLoadExternalDTD , false ) ;

	}catch( const xercesc::SAXNotRecognizedException& e ){

		return( ERROR_CONFIGURE_PARSER ) ;
	}


	try{

		std::cout << "[begin: parse]" << std::endl ;
		p->parse( xmlFile ) ;
		std::cout << "[end: parse]" << std::endl ;

	}catch( xercesc::SAXException& e ){

		std::cerr << "SAX Error: " << DualString( e.getMessage() ) << std::endl ;
	
		return( ERROR_PARSE ) ;

	}catch( xercesc::XMLException& e ){

		std::cerr << "Error:"
			<< " type: " << DualString( e.getType() )
			<< " code: " << e.getCode()
			<< " message: " << DualString( e.getMessage() )
			<< " (" << e.getSrcFile() << ":" << e.getSrcLine() << ")"
			<< std::endl
		;
	
		return( ERROR_PARSE ) ;

	}

	std::cout << "[done]" << std::endl ;

	std::cout << "Error stats: " << *e << std::endl ;
	
	if( 0 != p->getErrorCount() ){
		std::cerr << "Total parse errors: " << p->getErrorCount() << std::endl ;
	}

	std::cout << "Total RPMs processed: " << list.size() << std::endl ;

	std::for_each(
		list.begin() ,
		list.end() ,
		ShowRPMInfo()
	) ;
		


	delete( e ) ;
	delete( h ) ;
	delete( p ) ;

	try{
		xercesc::XMLPlatformUtils::Terminate();
	}catch( xercesc::XMLException& e ){
		std::cerr << "XML toolkit teardown error: "
			<< DualString( e.getMessage() )
			<< std::endl
		;
	}

	return( 0 ) ;

} // main()

