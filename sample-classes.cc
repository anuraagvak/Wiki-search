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
#include "./include/Stopit.h"
#include "./include/porter.h"
#include "./include/compress_uncompress.h"
#include "./include/hashword.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <map>
//#include <queue>
#include <vector>

#define NOTHING	0
#define titleWiki	46875
#define infoWiki	1875
#define bodyWiki 	25
#define catWiki	5
#define extWiki	1
#define idWiki 	2
#define max_size	20971520
//#define max_size	10485760
//#define max_size	5242880
int counterit = 0;
using namespace std;

typedef unsigned char UINT_8;
typedef unsigned short int UINT_16;
typedef unsigned int UINT_32;
typedef unsigned long long int UINT_64;

UINT_32 Flag = NOTHING;

UINT_16 catCheck = 0, infoCheck = 0;
char * mytok(char * str, UINT_32 * pos) {
	UINT_32 i, cat = 0, info = 0, ext = 0;
	static bool checkinfo = false, checkext = false, changeit = false,doneit = false;
	if (doneit) { doneit = false; return NULL;}
	if (changeit) {
		Flag = bodyWiki;
		changeit = false;
	}
	for (i = *pos;(str[i] < 48 || (str[i] > 57 && str[i] < 65) || (str[i] > 90 && str[i] < 97) || str[i] > 122) && str[i] != '\0'; i++) {
		if (str[i] == '[') {
			cat++;
			if (Flag == catWiki) catCheck++;
		}
		else if (str[i] == '{') {
			info++;
			if(Flag == infoWiki) infoCheck++;
		}
		else if (str[i] == '=') ext++;
		if (str[i] == ']') {
			if (Flag == catWiki && str[i+1] == ']') {
				catCheck = 0; 
				Flag = bodyWiki;
				//changeit = true;
			}
			else if (Flag == extWiki && str[i] && str[i+2] != '*') Flag = bodyWiki;
		}
		else if (str[i] == '}' && Flag == infoWiki) {
			infoCheck--;
			if (!infoCheck) {
				Flag = bodyWiki;
				//changeit = true;
			}
		}
	}

	if (str[i] == '\0') return NULL;
	char *ret = &str[i];
	for (; (str[i] > 96 && str[i] < 123) || (str[i] > 64 && str[i] < 91) || (str[i] > 47 && str[i] < 58); i++) {}

	if (str[i] == '[') {
		cat++;
		if (Flag == catWiki) catCheck++;
	}
	else if (str[i] == '{') {
		info++;
		if(Flag == infoWiki) infoCheck++;
	}
	else if (str[i] == '=') ext++;
	if (str[i] == ']') {
		if (Flag == catWiki && str[i+1] == ']') {
			catCheck = 0; 
			changeit = true;
		}
		else if (Flag == extWiki && str[i] && str[i+2] != '*') changeit = true;
	}
	else if (str[i] == '}' && Flag == infoWiki) {
		infoCheck--;
		if (!infoCheck) changeit = true;
	}
	if (str[i] == '\0') doneit = true;
	else {
		str[i] = '\0';
		*pos = i+1;
	}
	if (cat == 2 &&  strcmp(ret, "Category") == 0) {Flag = catWiki; catCheck = 2; return mytok(str,pos);}
	else if (info == 2 && strcmp(ret, "Infobox") == 0)  
	{
		if (strncmp(&str[*pos], "settlement", 10) == 0) {
			Flag = infoWiki; infoCheck = 2;
			*pos += 10;
			return mytok(str,pos);
		}
	}
	else if (ext == 2 && strcmp(ret, "External") == 0) 
	{
		if (strncmp(&str[*pos], "links", 5) == 0) {
			Flag = extWiki;
			*pos += 5;
			return mytok(str,pos);
		}
	}
	return ret;
}

UINT_64 idNum;
typedef struct Ranking {
	UINT_32 doc;
	UINT_64 rank;
} Ranking;

struct CompareRank  {  
	bool operator()(const Ranking& l, const Ranking& r) {return l.rank < r.rank; }
};  

map <UINT_64, UINT_64> allwords;
//map <UINT_64, UINT_64> doc_freq;
map <UINT_64, vector<Ranking> > inv_index;

FILE *fp;

bool WordPresent(UINT_64 word) {
    map<UINT_64, UINT_64>::const_iterator it = allwords.find(word);
    return (it != allwords.end());
}

// - - - - - - - - - - - - - - - - - - - -


SimpleContentHandler::SimpleContentHandler()
{
} // SimpleContentHandler::ctor


void SimpleContentHandler::startElement(
	const XMLCh* const uri ,
	const XMLCh* const localname ,
	const XMLCh* const qname ,
	const xercesc::Attributes& attrs
){

    char *str = xercesc::XMLString::transcode(qname);
//    cout << str << " <-----> start"<< endl;
    if (strcmp(str, "title")==0) Flag = titleWiki;
    //else if (strcmp(str, "page")==0) Flag = pageWiki;
    else if (strcmp(str, "id") == 0 && Flag == titleWiki) Flag = idWiki;
	 else if (strcmp(str, "text") == 0) Flag = bodyWiki;
	
    //std::cout << "[startElement( \"" << DualString( qname ) << "\" )]" << std::endl ;

//	for( unsigned int ix = 0 ; ix < attrs.getLength() ; ++ix ){

		/*
		The SAX2 Attributes class also permits you to fetch data
		based on local/namespace-qualified names.
		*/

///		const DualString attrName( attrs.getQName( ix ) ) ;
//		const DualString attrValue( attrs.getValue( ix ) ) ;
//		const DualString attrType( attrs.getType( ix ) ) ;

//		std::cout << "    " << "\"" << attrName << "\" => \""
//			<< attrValue << "\" (type: "
//			<< attrType << ")"
//			<< std::endl
//		;
//
//	}

	return ;

} // SimpleContentHandler::startElement()

UINT_8 Binaryit[20971520];
//UINT_8 Binaryit[5242950];
UINT_64 prinpos = 0;

void SimpleContentHandler::endElement(
	const XMLCh* const uri ,
	const XMLCh* const localname ,
	const XMLCh* const qname
){
	char *str = xercesc::XMLString::transcode(qname);
	if (strcmp(str,"text") == 0) Flag = NOTHING;
	if (strcmp(str, "page") == 0) {
		//fprintf(stderr, "%llu ", idNum);
		for (map<UINT_64,UINT_64>::iterator iter = allwords.begin(), iterend = allwords.end(); iter != iterend; ++iter) {
			Ranking word;
			word.doc = idNum;
			word.rank = iter -> second;
			inv_index[iter -> first].push_back(word);
		}
		allwords.clear();
	}
//    char *str = xercesc::XMLString::transcode(qname);
//    cout << str << " <-----> end"<< endl;

	//std::cout << "[endElement( \"" << DualString( qname ) << "\" )]" << std::endl ;
} // SimpleContentHandler::endElement

void SimpleContentHandler::characters (const XMLCh *const chars, const unsigned int length) {
	//std::cout << "[characters( XMLCh[" << length << "] ) ]" << std::endl ;
    char *str = xercesc::XMLString::transcode(chars);

	if (Flag == NOTHING) return;
	else if (Flag == idWiki) {
		idNum = strtoull(str, NULL, 10);
		//printf("id = %llu\n", idNum);
		Flag = NOTHING;
		return;
	} else if (Flag == titleWiki) {
		UINT_64 word; UINT_32 pos = 0;
		char *pch = mytok(str, &pos);
		while (pch != NULL) {
		//	printf("%lu\t\t\t%s\n", Flag, pch);
			word = HashMapWord(stemword(pch));
			if(!WordPresent(word)) allwords[word] = Flag; 
			else allwords[word] += Flag;
			pch = mytok(str, &pos);
		}
	} else {
		UINT_64 word; UINT_32 pos = 0;
		char *pch = mytok(str, &pos);
		while (pch != NULL) {
			word = HashMapWord(stemword(pch));
			if (Allowit(word)) {
		//		printf("%lu\t\t\t%s\n", Flag, pch);
				if(!WordPresent(word)) allwords[word] = Flag; 
				else allwords[word] += Flag;
			}
			pch = mytok(str, &pos);
		}
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
	//std::cout << "[resetDocument()]" << std::endl ;
} // SimpleContentHandler::resetDocument()

void SimpleContentHandler::setDocumentLocator (const xercesc::Locator *const locator){
	//std::cout << "[setDocumentLocator()]" << std::endl ;
} // SimpleContentHandler::setDocumentLocator()


void SimpleContentHandler::startDocument (){
	std::cerr << "[startDocument()]" << std::endl ;
	char fi[50];
	time_t t = time(NULL);
	sprintf(fi,"./data/Index%d.txt",t);
   fp = fopen(fi,"w");
} // SimpleContentHandler::startDocument()
UINT_64 maxy = max_size;
UINT_64 uncompressit(UINT_8 * buff, UINT_64 *curr){
	UINT_64 ret = 0;
	UINT_8 iter = *curr,pos = 0;
	do {
		if (iter == maxy) {
			maxy = fread(buff,1,max_size,fp);
			*curr = 0; iter = 0;
			//if (maxy == 0) return 0;
			fputs("hello", stderr);
		}
		ret += (buff[iter] >> 1) << pos;
		//	printf("buff %d = %d , ret = %llu ", iter, buff[iter],ret);
		pos += 7;

	} while(buff[iter++]&1);
	*curr = iter;
	//	printf("\n%llu ", ret);
	return ret;
}

void SimpleContentHandler::endDocument (){
	std::cerr << "[endDocument()]" << std::endl ;
	
	for (map <UINT_64, vector<Ranking> >::iterator iind =  inv_index.begin(), eind = inv_index.end(); iind != eind; ++iind) {
		UINT_64 val;
		if (iind -> first == 2147504168 || iind -> first == 2147481661) {
			printf("word = %llu pos = %llu\n", iind -> first, prinpos);
			val = prinpos;
		}
		prinpos = compressit(iind -> first, Binaryit, prinpos);
		if (iind -> first == 2147504168|| iind -> first == 2147481661) {
			val = uncompressit(Binaryit, &val);
			printf("val = %llu\n",val);
		}
		vector<Ranking> ivr = iind -> second;
		for (int ivec = 0; ivec < ivr.size(); ivec++) {
			if (prinpos > max_size) {
	//			cout << "pos = " << prinpos << endl;
				fwrite(Binaryit, 1, prinpos, fp);
				prinpos = 0;
			}		
			Ranking word = ivr[ivec];
			prinpos = compressit(word.rank, Binaryit, prinpos);
			prinpos = compressit(word.doc, Binaryit, prinpos);
		}
		prinpos = compressit(0, Binaryit, prinpos);
	}
	if (prinpos) {
	//	cout << "pos = " << prinpos << endl;
		fwrite(Binaryit, 1, prinpos, fp);
		prinpos = 0;
	}

/*
	std::string Writeit;
	for (map <UINT_64, vector<Ranking> >::iterator iind =  inv_index.begin(), eind = inv_index.end(); iind != eind; ++iind) {
		Writeit.append(to_string(iind -> first)+"$");
		vector<Ranking> ivr = iind -> second;
		for (int ivec = 0; ivec < ivr.size(); ivec++) {
			Ranking word = ivr[ivec];
			Writeit.append(to_string(word.rank)+"-"+to_string(word.doc)+";");
		}
		Writeit.pop_back();
		Writeit.append("\n");
		if (Writeit.size() > max_size) {
			fputs(Writeit.c_str(), fp);
		Writeit.clear();
		}
	}
	inv_index.clear();
	if (!Writeit.empty()) {
		fputs(Writeit.c_str(), fp);
		Writeit.clear();
	}
*/	
	fclose(fp);
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


