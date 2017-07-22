#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "search-engine.h"
#include "webcrawl.h"

DictionaryType dictType;

SearchEngine::SearchEngine( int port, DictionaryType dictionaryType):
  MiniHTTPD(port)
{
  // Create dictionary of the indicated type
  if(dictionaryType == ArrayDictionaryType)
	  _wordToURLList = new ArrayDictionary();
  else if(dictionaryType == HashDictionaryType)
	  _wordToURLList = new HashDictionary();
  else if(dictionaryType == BinarySearchDictionaryType)
	  _wordToURLList = new BinarySearchDictionary();
  else if(dictionaryType == AVLDictionaryType)
	  _wordToURLList = new AVLDictionary();
  else
	  _wordToURLList = NULL;
	
  dictType = dictionaryType;

  // Populate dictionary and sort it if necessary

  // URLRecord 
  URLRecordList ** list = new URLRecordList * [1024];
  for(int i = 0; i < 1024; i++){
	  list[i] = new URLRecordList();
  }

}

void
SearchEngine::dispatch( FILE * fout, const char * documentRequested)
{
  if (strcmp(documentRequested, "/")==0) {
    // Send initial form
    fprintf(fout, "<TITLE>CS251 Search</TITLE>\r\n");
    fprintf(fout, "<CENTER><H1><em>Boiler Search</em></H1>\n");
    fprintf(fout, "<H2>\n");
    fprintf(fout, "<FORM ACTION=\"search\">\n");
    fprintf(fout, "Search:\n");
    fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
    fprintf(fout, "</H2>\n");
    fprintf(fout, "</FORM></CENTER>\n");
    return;
  }

  // TODO: The words to search in "documentRequested" are in the form
  // /search?word=a+b+c
  //
  // You need to separate the words before search
  // Search the words in the dictionary and find the URLs that
  // are common for al the words. Then print the URLs and descriptions
  // in HTML. Make the output look nicer.

  // Here the URLs printed are hardwired
  
  const int nurls=2;

  const char * words = "data structures";

  const char * urls[] = {
    "http://www.cs.purdue.edu",
    "http://www.cs.purdue.edu/homes/cs251"
  };
  
  const char * description[] = {
    "Computer Science Department. Purdue University.",
    "CS251 Data Structures"
  };

  fprintf( stderr, "Search for words: \"%s\"\n", words);

  fprintf( fout, "<TITLE>Search Results</TITLE>\r\n");
  fprintf( fout, "<H1> <Center><em>Boiler Search</em></H1>\n");
  fprintf( fout, "<H2> Search Results for \"%s\"</center></H2>\n", words );

  for ( int i = 0; i < nurls; i++ ) {
    fprintf( fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", i+1, urls[i], urls[i] );
    fprintf( fout, "<blockquote>%s<p></blockquote>\n", description[i] );
  }

  // Add search form at the end
  fprintf(fout, "<HR><H2>\n");
  fprintf(fout, "<FORM ACTION=\"search\">\n");
  fprintf(fout, "Search:\n");
  fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
  fprintf(fout, "</H2>\n");
  fprintf(fout, "</FORM>\n");
}

void
printUsage()
{
  const char * usage =
    "Usage: search-engine port (array | hash | avl | bsearch)\n"
    "  It starts a search engine at this port using the\n"
    "  data structure indicated. Port has to be larger than 1024.\n";

  fprintf(stderr, "%s", usage);
}

int main(int argc, char ** argv)
{
  if (argc < 3) {
    printUsage();
    return 1;
  }

  // Get port
  int port;
  sscanf( argv[1], "%d", &port);

  // Get DictionaryType
  const char * dictType = argv[2];
  DictionaryType dictionaryType;
  if (!strcmp(dictType, "array")) {
    dictionaryType = ArrayDictionaryType;
  }
  else if (!strcmp(dictType, "hash")) {
    dictionaryType = HashDictionaryType;
  }
  else if (!strcmp(dictType, "avl")) {
    dictionaryType = AVLDictionaryType;
  }
  else if (!strcmp(dictType, "bsearch")) {
    dictionaryType = BinarySearchDictionaryType;
  }
  else {
    printUsage();
    return 0;
  }
  
  SearchEngine httpd(port, dictionaryType);
  
  httpd.run();

  return 0;
}
