#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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

	//read from url.txt
  	FILE * fp;
  	fp = fopen("url.txt", "r");
	assert(fp != NULL);
	
	char * buffer;
	buffer = new char [1000];

  	URLRecord ** records = new URLRecord * [1024];
  	for(int i = 0; i < 1024; i++){
	  	records[i] = new URLRecord();
  	}

	while(fgets(buffer, 1000, fp)){
		if(strcmp(buffer, "\n") != 0){
			//get the index
			char * token = new char [1000];
			token = strtok(buffer, " ");
			int index = atoi(token);

			//get the url
			token = strtok(NULL, " \n");
			char * url = new char [1000];
			strcpy(url, token);
 
			//get the description
			fgets(buffer, 1000, fp);
			token = strtok(buffer, "\n");
			char * description = new char [1000];
			strcpy(description, token);

			//store this entry
			records[index]->_url = url;
			records[index]->_description = description;
		}
	} //while

	delete buffer;
	fclose(fp);

	//read form word.txt
	fp = fopen("word.txt", "r");
	assert(fp != NULL);

	buffer = new char [1000];

	while(fgets(buffer, 1000, fp)){
		if(strcmp(buffer, "\n") != 0){
			char * token;
			token = strtok(buffer, "\n");

			char * word;
			word = new char [1000];
			strcpy(word, token);

			URLRecordList * head;
			head = NULL;
			URLRecordList * prev;
			prev = NULL;
			
			token = strtok(NULL, " \n");
			
			while(token != NULL){
				int position = atoi(token);

				if(records[position]->_url == NULL)
					continue;

				URLRecordList * entry = new URLRecordList();

				if(head == NULL)
					head = entry;

				entry->_urlRecord = records[position];
				entry->_next = NULL;

				if(prev != NULL)
					prev->_next = entry;

				prev = entry;
				token = strtok(NULL, " \n");
			}
			_wordToURLList->addRecord(word, (URLRecordList *)head);
			delete token;
		}
	}//while

	delete buffer;
	fclose(fp);
}

//break down to words
char *
SearchEngine::nextWord(char * &buffer){
	char * word = (char *)malloc(100 * sizeof(char));

	int i = 0;	//# of valid chars in word
	while(*buffer = '\0'){
		if(*buffer == ' ' || *buffer == '\n' || *buffer == '+'){	//invalid char
			if(i == 0){	//first char in word is invalid
				buffer++;	//skip invalid chars
				continue;
			} else {	//end of the word
				word[i] = '\0';
				return word;
			}
		} else {	//*buffer is valid char
			word[i++] = *buffer;	//add to word
			buffer++;
		}
	}	//while
	if(i > 0){	//check again
		word[i] = '\0';
		return word;
	}
	return NULL;
}

bool
SearchEngine::checkFormat(char * &docRequested){
	if(!(strncmp(docRequested, "/search?word=", 13) == 0)){	//format is valid
		docRequested += 13;	//skip
		return true;
	}
	return false;
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

	if(strlen(documentRequested) < 13){	//invalid 
		return;
	}

	//prepare documentRequested
	char * search = (char *)malloc((strlen(documentRequested)+1) * sizeof(char));
	strcpy(search, documentRequested);

	//words to search
	char ** wordList = new char * [100];
	for(int i = 0; i < 100; i++){
		wordList[i] = NULL;
	}

	int nWords = 0;	//# of words in documentRequested
	char * buffer = (char *)malloc(100 * sizeof(char));

	//add words to wordList[]
	if(checkFormat(search)){
		while((buffer = nextWord(search)) != NULL){
			wordList[nWords++] = strdup(buffer); 
		}
	}

	//print out a string with words to search
	char * words = (char *)malloc(100 * sizeof(char));
	strcpy(words, "");

	for(int i = 0; i < nWords; i++){
		strcat(words, wordList[i]);
		strcat(words, " ");
	}
	printf("Words to search for: %s\n", words);


  fprintf( stderr, "Search for words: \"%s\"\n", words);

  fprintf( fout, "<TITLE>Search Results</TITLE>\r\n");
  fprintf( fout, "<H1> <Center><em>Boiler Search</em></H1>\n");
  fprintf( fout, "<H2> Search Results for \"%s\"</center></H2>\n", words );

  	//number of urlRecords in urlList
  	int nRecords;
	nRecords = 0;

	URLRecord ** urlList = new URLRecord * [500];

	//add to urlList
	for(int i = 0; i < nWords; i++){
		URLRecordList * p;	//map elements in wordList to urlList
		p = (URLRecordList *)_wordToURLList->findRecord(wordList[i]);

		while(p != NULL){
			int duplicate = 0;	//check duplicate

			for(int j = 0; j < nRecords; j++){
				if(urlList[j] == p->_urlRecord){
					duplicate = 1;
					break;
				}
			}

			if(duplicate == 0){
				urlList[nRecords] = p->_urlRecord;	//add to the list
				nRecords++;
			}
			p = p->_next;
		}	//while
	}	//for

	//check if wordList maps urlList
	for(int i = 0; i < nRecords; i++){
		for(int j = 0; j < nWords; j++){
			URLRecordList * p;
			p = (URLRecordList *)_wordToURLList->findRecord(wordList[j]);

			int exists = 0;

			while(p != NULL){
				if(p->_urlRecord == urlList[i]){
					exists = 1;
				}
				p = p->_next;
			}

			if(exists == 0)
				urlList[i] = NULL;
		}
	}

	int count = 0;

	for(int i = 0; i < nRecords; i++){
		if(urlList[i] == NULL)	//skip
			continue;
		
		fprintf(fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", count+1, urlList[i]->_url, urlList[i]->_url);
		fprintf(fout, "<blockquote>%s<p></blockquote>\n", urlList[i]->_description);

		count++;
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
