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
#include<sample-classes.h>

#include<iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NOTHING 0
#define idWiki 1
#define titleWiki 2

// - - - - - - - - - - - - - - - - - - - -


SimpleContentHandler::SimpleContentHandler()
{
} // SimpleContentHandler::ctor

int Flag = NOTHING;
bool check = false;
void SimpleContentHandler::startElement(
	const XMLCh* const uri ,
	const XMLCh* const localname ,
	const XMLCh* const qname ,
	const xercesc::Attributes& attrs
	){
	char *str = xercesc::XMLString::transcode(qname);
	if (strcmp(str, "title")==0) Flag = titleWiki;
	else if (strcmp(str, "id") == 0 && check) {
		Flag = idWiki;
		check = false;
	}
	return ;

} // SimpleContentHandler::startElement()

void SimpleContentHandler::endElement(
	const XMLCh* const uri ,
	const XMLCh* const localname ,
	const XMLCh* const qname
){
	//std::cout << "[endElement( \"" << DualString( qname ) << "\" )]" << std::endl ;
} // SimpleContentHandler::endElement

FILE *doc_index = fopen("/home/dilipvamsi/docindex.txt","wb");
FILE *title_index = fopen("/home/dilipvamsi/titleindex.txt","wb");

int idNum, pos = 0;
void SimpleContentHandler::characters (const XMLCh *const chars, const unsigned int length) {
	//std::cout << "[characters( XMLCh[" << length << "] ) ]" << std::endl ;
   char *str = xercesc::XMLString::transcode(chars);
	//printf("flag = %d\n", Flag);
	if (Flag == idWiki) {
		idNum = atoi(str);
		fwrite(&idNum, 4,1,doc_index);
		fwrite(&pos, 4,1,doc_index);
		Flag = NOTHING;
	} else if (Flag == titleWiki) {
		check = true;
		fputs(str, title_index);
		fputc('\n', title_index);
		pos += strlen(str) + 1;
		Flag = NOTHING;
	}
} // SimpleContentHandler::characters()


void SimpleContentHandler::ignorableWhitespace (const XMLCh *const chars, const unsigned int length){
	std::cout << "[ignorableWhitespace( XMLCh[" << length << "] ) ]" << std::endl ;
} // SimpleContentHandler::ignorableWhitespace()

void SimpleContentHandler::processingInstruction (const XMLCh *const target, const XMLCh *const data){
	std::cout << "[processingInstruction( "
		<< DualString( target )
		<< " )]"
		<< std::endl ;
} // SimpleContentHandler::processingInstruction()

void SimpleContentHandler::resetDocument (){
	std::cout << "[resetDocument()]" << std::endl ;
} // SimpleContentHandler::resetDocument()

void SimpleContentHandler::setDocumentLocator (const xercesc::Locator *const locator){
	std::cout << "[setDocumentLocator()]" << std::endl ;
} // SimpleContentHandler::setDocumentLocator()


void SimpleContentHandler::startDocument (){
	std::cout << "[startDocument()]" << std::endl ;
} // SimpleContentHandler::startDocument()

void SimpleContentHandler::endDocument (){
	fclose(doc_index);
	fclose(title_index);
	std::cout << "[endDocument()]" << std::endl ;
} // SimpleContentHandler::endDocument()


void SimpleContentHandler::startPrefixMapping( const XMLCh *const prefix, const XMLCh *const uri ){
	std::cout << "[startPrefixMapping( "
		<< DualString( prefix )
		<< " , "
		<< DualString( uri )
		<< " )]"
		<< std::endl ;
} // SimpleContentHandler::startPrefixMapping()

void SimpleContentHandler::endPrefixMapping( const XMLCh *const prefix ){
	std::cout << "[endPrefixMapping( "
		<< DualString( prefix )
		<< " )]"
		<< std::endl ;
} // SimpleContentHandler::endPrefixMapping()


void SimpleContentHandler::skippedEntity( const XMLCh *const name ){
	std::cout << "[skippedEntity( "
		<< DualString( name )
		<< " )]"
		<< std::endl ;
} // SimpleContentHandler::skippedEntity()


// - - - - - - - - - - - - - - - - - - - -

RPMInfo::RPMInfo()
	:
	name_() ,
	version_() ,
	license_() ,
	vendor_()
{
} // RPMInfo::ctor


// - - - -
const std::string& RPMInfo::getName() const throw() {
	return( name_ ) ;
} // RPMInfo::getName()

void RPMInfo::setName( const std::string& in ){
	name_ = in ;
} // RPMInfo::setName()


// - - - -
const std::string& RPMInfo::getVersion() const throw() {
	return( version_ ) ;
} // RPMInfo::getVersion()

void RPMInfo::setVersion( const std::string& in ){
	version_ = in ;
} // RPMInfo::setVersion()


// - - - -
const std::string& RPMInfo::getLicense() const throw() {
	return( license_ ) ;
} // RPMInfo::getLicense()

void RPMInfo::setLicense( const std::string& in ){
	license_ = in ;
} // RPMInfo::setLicense()


// - - - -
const std::string& RPMInfo::getVendor() const throw() {
	return( vendor_ ) ;
} // RPMInfo::getVendor()

void RPMInfo::setVendor( const std::string& in ){
	vendor_ = in ;
} // RPMInfo::setVendor()


std::ostream& RPMInfo::print( std::ostream& s ) const {
	s << "[RPMInfo: " 
		<< name_ << "-" << version_
		<< "/" << license_
		<< "/" << vendor_
		<< "]"
	;
		
	return( s ) ;
} // RPMInfo::print()


std::ostream& operator<<( std::ostream& s , const RPMInfo& obj ) {
	obj.print( s ) ;
	s << std::flush ;
	return( s ) ;
}


// - - - - - - - - - - - - - - - - - - - -

TagNames::TagNames()
	:
	TAG_PACKAGE( "package" ) ,

	TAG_NAME( "name" ) ,
	TAG_VERSION( "version" ) ,
	TAG_LICENSE( "license" ) ,
	TAG_VENDOR( "vendor" ) ,

	ATTR_VER( "ver" ) ,
	ATTR_REL( "rel" ) ,

	URI_RPM( "http://linux.duke.edu/metadata/rpm" ) ,
	URI_METADATA( "http://linux.duke.edu/metadata/common" )

	// we could have made these static members; but
	// DualString can't call XMLString::transcode() until
	// we've initialized Xerces...
{
	return ;
} // TagNames ctor

// - - - - - - - - - - - - - - - - - - - -

RepodataContentHandler::RepodataContentHandler( RPMList& rpms )
	:
	tagNames_() ,
	tagStack_() ,
	rpms_( rpms ) ,
	current_( NULL ) ,
	charBuf_()
{
} // RepodataContentHandler ctor


void RepodataContentHandler::startElement(
	const XMLCh* const uri ,
	const XMLCh* const localName ,
	const XMLCh* const qName ,
	const xercesc::Attributes& attrs
){


	// determine which element this is --
	// centralizes the if/then/compare-string logic
	pushTag( uri , localName ) ;

	switch( currentTag() ){

		case TAG_METADATA_PACKAGE:
		{

			current_ = new RPMInfo() ;
			break ;
		}

		case TAG_METADATA_NAME:
		{
			 // ... do nothing ...
			break ;
		}

		case TAG_METADATA_VERSION:
		{

			/*
			for( unsigned int ix = 0 ; ix < attrs.getLength() ; ++ix ){
	
				// The SAX2 Attributes class also permits you to fetch data
				// based on local/namespace-qualified names.
	
				std::cout
					<< "\"" << DualString( attrs.getURI( ix ) ) << "\" : "
					<< "\"" << DualString( attrs.getLocalName( ix ) ) << "\" => \""
					<< DualString( attrs.getValue( ix ) ) << "\" (type: "
					<< DualString( attrs.getType( ix ) ) << ")"
					<< std::endl
				;
	
			}
			*/

			/*
			std::cout << "attr: \"" <<  tagNames_.ATTR_VER << "\"" << std::endl ;
			std::cout << "attr: \"" <<  tagNames_.ATTR_REL << "\"" << std::endl ;
			*/

			std::ostringstream versionBuf ;

			versionBuf
				<< DualString( attrs.getValue( NULL , tagNames_.ATTR_VER.asXMLString() ) )
				<< "."
				<< DualString( attrs.getValue( NULL , tagNames_.ATTR_REL.asXMLString() ) )
				<< std::flush
			;

			current_->setVersion( versionBuf.str() ) ;
			break ;
		}

		case TAG_RPM_LICENSE:
		{
			 // ... do nothing ...
			break ;
		}

		case TAG_RPM_VENDOR:
		{
			 // ... do nothing ...
			break ;
		}

		// sic: no default case, because we only care about
		// these specific tags

	} // switch( currentTag() ){

	return ;

} // RepodataContentHandler::startElement()


void RepodataContentHandler::endElement(
	const XMLCh* const uri ,
	const XMLCh* const localName ,
	const XMLCh* const qName
){

	switch( currentTag() ){

		case TAG_METADATA_PACKAGE:
		{
			rpms_.push_back( current_ ) ;
			current_ = NULL ;
			break ;
		}

		case TAG_METADATA_NAME:
		{
			current_->setName( charBuf_.str() ) ;
			break ;
		}

		case TAG_RPM_LICENSE:
		{
			current_->setLicense( charBuf_.str() ) ;
			break ;
		}

		case TAG_RPM_VENDOR:
		{
			current_->setVendor( charBuf_.str() ) ;
			break ;
		}

		// sic: no default case, because we only care about
		// these specific tags

	} // switch( currentTag() )


	// zero the char buffer...
	charBuf_.str( "" ) ;

	// pop the element off the stack
	popTag() ;

} // RepodataContentHandler::endElement()


void RepodataContentHandler::characters (const XMLCh *const chars, const unsigned int length) {

	/*
	the structure of the expected document requires only a single
	character buffer; if we expected character data between tags,
	we'd need one such character buffer for each situation.
	*/

	switch( currentTag() ){

		case TAG_METADATA_NAME:
		case TAG_RPM_VENDOR:
		case TAG_RPM_LICENSE:
		{	
			XMLCh* converted = new XMLCh[ length + 1 ] ;
			converted[ length ] = '\0' ;
	
			xercesc::XMLString::copyNString(
				converted , // target
				chars , // source
				length // size
			) ;
	
			charBuf_.write(
				DualString( converted ).asCString() ,
				length
			) ;

			break ;
		}

		// sic - no default; we only care about those specific tags
	}

} // RepodataContentHandler::characters ()

void RepodataContentHandler::pushTag(
	const XMLCh* const uri ,
	const XMLCh* const localName
){

	if( 0 == xercesc::XMLString::compareString(
		uri ,
		tagNames_.URI_METADATA.asXMLString()
	) ){

		if( 0 == xercesc::XMLString::compareString(
			localName ,
			tagNames_.TAG_PACKAGE.asXMLString()
		) ){

			tagStack_.push( TAG_METADATA_PACKAGE ) ;

		}else if( 0 == xercesc::XMLString::compareString(
			localName ,
			tagNames_.TAG_NAME.asXMLString()
		) ){

			tagStack_.push( TAG_METADATA_NAME ) ;

		}else if( 0 == xercesc::XMLString::compareString(
			localName ,
			tagNames_.TAG_VERSION.asXMLString()
		) ){

			tagStack_.push( TAG_METADATA_VERSION ) ;

		}else{

			// ... we're not interested in this element ...
			tagStack_.push( TAG_UNKNOWN ) ;

		}

	}else if( 0 == xercesc::XMLString::compareString(
		uri ,
		tagNames_.URI_RPM.asXMLString()
	) ){

		if( 0 == xercesc::XMLString::compareString(
			localName ,
			tagNames_.TAG_LICENSE.asXMLString()
		) ){

			tagStack_.push( TAG_RPM_LICENSE ) ;

		}else if( 0 == xercesc::XMLString::compareString(
			localName ,
			tagNames_.TAG_VENDOR.asXMLString()
		) ){

			tagStack_.push( TAG_RPM_VENDOR ) ;

		}else{

			// ... we're not interested in this element ...
			tagStack_.push( TAG_UNKNOWN ) ;

		}

	}else{

		// ... we're not interested in this element ...
		tagStack_.push( TAG_UNKNOWN ) ;
	}


} // RepodataContentHandler::pushTag()


void RepodataContentHandler::popTag(){
	tagStack_.pop() ;
} // RepodataContentHandler::popTag()


int RepodataContentHandler::currentTag(){
	return( tagStack_.top() ) ;
} // RepodataContentHandler::currentTag()


// - - - - - - - - - - - - - - - - - - - -

#include<xercesc/sax/ErrorHandler.hpp>

SimpleErrorHandler::SimpleErrorHandler()
	:
	warningCount_( 0 ) ,
	errorCount_( 0 ) ,
	fatalErrorCount_( 0 )
{
	return ;
} // ctor


void SimpleErrorHandler::warning( const xercesc::SAXParseException& e ){
	++warningCount_ ;
	std::cerr << "[ErrorHandler: warning: " << DualString( e.getMessage() ) << "]" << std::endl ;
} // SimpleErrorHandler::warning( )

void SimpleErrorHandler::error( const xercesc::SAXParseException& e ){
	++errorCount_ ;
	std::cerr << "[ErrorHandler: error: " << DualString( e.getMessage() ) << "]" << std::endl ;
} // SimpleErrorHandler::error()

void SimpleErrorHandler::fatalError( const xercesc::SAXParseException& e ){
	++fatalErrorCount_ ;
	std::cerr << "[ErrorHandler: fatalError: " << DualString( e.getMessage() ) << "]" << std::endl ;
} // SimpleErrorHandler::fatalError()

void SimpleErrorHandler::resetErrors(){
	std::cerr << "[ErrorHandler: resetErrors]" << std::endl ;
} // SimpleErrorHandler::resetErrors()


std::ostream& SimpleErrorHandler::print( std::ostream& s ) const {

	s << "[SimpleErrorHandler: "
		<< warningCount_ << "w/"
		<< errorCount_ << "e/"
		<< fatalErrorCount_ << "f"
		<< "]"
	;

	return( s ) ;

} // SimpleErrorHandler::print() ;



std::ostream& operator<<( std::ostream& s , const SimpleErrorHandler& obj ){
	obj.print( s ) ;
	return( s ) ;
} // operator<< for SimpleErrorHandler



// - - - - - - - - - - - - - - - - - - - -

#include<xercesc/sax/ErrorHandler.hpp>

CountingErrorHandler::CountingErrorHandler()
	:
	warningCount_( 0 ) ,
	errorCount_( 0 ) ,
	fatalErrorCount_( 0 )
{
	return ;
} // ctor


void CountingErrorHandler::warning( const xercesc::SAXParseException& e ){
	++warningCount_ ;
} // CountingErrorHandler::warning()

void CountingErrorHandler::error( const xercesc::SAXParseException& e ){
	++errorCount_ ;
} // CountingErrorHandler::error()

void CountingErrorHandler::fatalError( const xercesc::SAXParseException& e ){
	++fatalErrorCount_ ;
} // CountingErrorHandler::fatalError()

void CountingErrorHandler::resetErrors(){
	std::cerr << "[ErrorHandler: resetErrors]" << std::endl ;
} // CountingErrorHandler::resetErrors()


std::ostream& CountingErrorHandler::print( std::ostream& s ) const {

	s << "[CountingErrorHandler: "
		<< warningCount_ << "w/"
		<< errorCount_ << "e/"
		<< fatalErrorCount_ << "f"
		<< "]"
	;

	return( s ) ;

} // CountingErrorHandler::print() ;



std::ostream& operator<<( std::ostream& s , const CountingErrorHandler& obj ){
	obj.print( s ) ;
	return( s ) ;
} // operator<< for CountingErrorHandler



// - - - - - - - - - - - - - - - - - - - -

#include<map>
#include<stdexcept>
#include<xercesc/sax/EntityResolver.hpp>
#include<xercesc/sax/InputSource.hpp>
#include<xercesc/framework/LocalFileInputSource.hpp>
#include<xercesc/framework/MemBufInputSource.hpp>
#include<xercesc/framework/StdInInputSource.hpp>
#include<xercesc/framework/URLInputSource.hpp>

SimpleEntityResolver::SimpleEntityResolver()
	:
	_entities()
{

	return ;

} // SimpleEntityResolver ctor


SimpleEntityResolver::~SimpleEntityResolver() throw()
{

	for(
		MapType::iterator ix = _entities.begin() , stop = _entities.end() ;
		ix != stop ;
		++ix
	){

		if( NULL != ix->first ){
			delete( ix->first ) ;
		}

		if( NULL != ix->second ){
			delete( ix->second ) ;
		}

	}

	return ;

} // SimpleEntityResolver ctor



void SimpleEntityResolver::add( const char* const systemID , const char* const file ) throw( std::runtime_error ) {

	std::cout << "SimpleEntityResolver::add(): "
		<< "called for "
		<< "systemID \"" << systemID << "\""
		<< " and  "
		<< "file \"" << file << "\""
		<< std::endl
	;


	bool insertSuccess = (
		_entities.insert(
			MapType::value_type(
				new DualString( systemID ) ,
				new DualString( file )
			)
		)
	).second ;

	if( ! insertSuccess ){
		throw( std::runtime_error( "SimpleEntityResolver::add(): Unable make map insertion." ) ) ;
	}

	return ;

} // SimpleEntityResolver::add( const XMLch* const , const char* const )



xercesc::InputSource * SimpleEntityResolver::resolveEntity (
	const XMLCh* const publicId ,
	const XMLCh* const systemId
){

	std::cout << "[SimpleEntityResolver::resolveEntity() - called]" ;

	xercesc::InputSource* result = NULL ;

	DualString key( systemId ) ;

	MapType::iterator item = _entities.find( &key ) ;

	// note that we can't keep returning the same object;
	// we create a new InputSource and let the caller (technically,
	// the parser) take ownership

	if( _entities.end() != item ){

		std::cout << "  Found requested entity" << std::endl ;
		DualString* file = item->second ;
		result = new xercesc::LocalFileInputSource( NULL , file->asXMLString() ) ;

	}

	return( result ) ;

} // SimpleEntityResolver::resolveEntity()

// - - - - - - - - - - - - - - - - - - - -


