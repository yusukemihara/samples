#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libxml/xmlreader.h>

static xmlChar *
GetAttributeValue(
	xmlTextReaderPtr reader,
	const xmlChar *name)
{
	if (!xmlTextReaderMoveToAttribute(reader,name)) {
		return NULL;
	}	
	return xmlTextReaderValue(reader);
}

void
ProcessNode(xmlTextReaderPtr reader)
{
	char *name;
	char *value;
	int isString;
	
	isString = 0;
	if ((name = xmlTextReaderName(reader)) != NULL) {
//fprintf(stderr, "name[%s]\n",name);
		if (!xmlStrcmp(name, "embed")) {
			if ((value = GetAttributeValue(reader, "object")) != NULL) {
				if (!xmlStrcmp(value, "string")) {
					isString = 1;
				}
				xmlFree(value);
			}
			if (isString) {
				if ((value = GetAttributeValue(reader, "length")) != NULL) {
fprintf(stderr, "length=\"%s\"/>\n",value);
					xmlFree(value);
				}
			}
		}
		xmlFree(name);
	}
}

int
main(int argc, char *argv[])
{
	int rc;
	xmlTextReaderPtr reader;

	if (argc < 2) {
		printf("./%s hoge.xml\n", argv[0]);
		exit(1);
	}

	reader = xmlNewTextReaderFilename(argv[1]);
	if (reader != NULL) {
		rc = xmlTextReaderRead(reader);
		while(rc == 1) {
			ProcessNode(reader);
			rc = xmlTextReaderRead(reader);
		}
		xmlFreeTextReader(reader);
	} else {
		printf("Unable to open %s\n",argv[1]);
	}
	return 0;
}
