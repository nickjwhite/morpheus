/* TODO:
 * - Don't hardcode language
 */

#include <stdio.h>
#include <prntflags.h>
#include <gkstring.h>
#include "../morphlib/gkstring.proto.h"
#include "../morphlib/morphkeys.proto.h"
#include "../morphlib/morphflags.proto.h"
#include "../morphlib/setlang.proto.h"
#include "checkstring.proto.h"
int quickflag = 0;

#define LENGTH(X) (sizeof X / sizeof X[0])
#define MAXJSON 131072

typedef struct {
	char * (*fn)(word_form);
	char *name;	
} Forms;

Forms forms[] = {
	{ NameOfTense, "tense" },
	{ NameOfMood, "mood" },
	{ NameOfVoice, "voice" },
	{ NameOfGender, "gender" },
	{ NameOfCase, "case" },
	{ NameOfDegree, "degree" },
	{ NameOfPerson, "person" },
	{ NameOfNumber, "number" },
};

typedef struct {
	int (*fn)(gk_analysis *, char *, char *);
	char *name;
} Names;

Names names[] = {
	{ DialectNamesFromAnal, "dialect" },
	{ DomainNamesFromAnal, "domain" },
	{ GeogRegionNamesFromAnal, "geographic-region" },
};

char * analysis_as_json(char *in)
{
	gk_word *w = NULL;
	char s[MAXJSON];
	char *s2;
	char s3[MAXJSON];
	int i;
	int j;
	gk_analysis *a;
	word_form wf;
	char *json;

	s[0] = 0;
	s2 = 0;

	json = malloc(sizeof(char) * MAXJSON);

	/* We expect this to be run from an emscripten environment, where the MORPHLIB
	 * variable won't be set, so set it to the right location for emscripten if so. */
	if(getenv("MORPHLIB") == NULL) {
		putenv("MORPHLIB=stemlib-outonly");
	}

	set_lang(LATIN);

	w = (gk_word *) CreatGkword(1);
	check_gkword(w, in);

	snprintf(json, MAXJSON-1, "{\n\t\"%s\": [\n", in);

	for(i = 0; i < totanal_of(w); i++) {
		a = analysis_of(w)+i;
		wf = forminfo_of(a);

		if(i > 0) {
			strncat(json, ",\n", MAXJSON - strlen(json) - 1);
		}
		strncat(json, "\t\t{\n", MAXJSON - strlen(json) - 1);

		/* Broadly copied from DumpPerseusAnalysis() */
		snprintf(s, MAXJSON - 1, "\t\t\t\"%s\": \"%s\",\n", "lemma", lemma_of(a));
		strncat(json, s, MAXJSON - strlen(json) - 1);

		snprintf(s, MAXJSON - 1, "\t\t\t\"%s\": \"%s\",\n", "work", workword_of(a));
		strncat(json, s, MAXJSON - strlen(json) - 1);

		snprintf(s, MAXJSON - 1, "\t\t\t\"%s\": \"%s\",\n", "raw", rawword_of(a));
		strncat(json, s, MAXJSON - strlen(json) - 1);

		snprintf(s, MAXJSON - 1, "\t\t\t\"type\": \"");
		if(Is_participle(a)) {
			strncat(s, "participle", MAXJSON - strlen(s) - 1);
		} else if( Is_nounform(a) || Is_adjform(a) ) {
			if(isupper(lemma_of(a)[0]) && cur_lang() == GREEK) {
				strncat(s, "english", MAXJSON - strlen(s) - 1);
			} else {
				strncat(s, "noun", MAXJSON - strlen(s) - 1);
			}
		} else if(Is_verbform(a)) {
			strncat(s, "verb", MAXJSON - strlen(s) - 1);
		} else {
			strncat(s, "indeclinable", MAXJSON - strlen(s) - 1);
		}
		strncat(s, "\"", MAXJSON - strlen(s) - 1);
		strncat(json, s, MAXJSON - strlen(json) - 1);

		for(j = 0; j < LENGTH(forms); j++) {
			s2 = forms[j].fn(wf);
			if(*s2) {
				snprintf(s, MAXJSON - 1, ",\n\t\t\t\"%s\": \"%s\"", forms[j].name, s2);
				strncat(json, s, MAXJSON - strlen(json) - 1);
			}
			
		}

		for(j = 0; j < LENGTH(names); j++) {
			s3[0] = 0;
			names[j].fn(a, s3, " ");
			if(*s3) {
				snprintf(s, MAXJSON - 1, ",\n\t\t\t\"%s\": \"%s\"", names[j].name, s3);
				strncat(json, s, MAXJSON - strlen(json) - 1);
			}
		}

		if(*(s2 = NameOfStemtype(stemtype_of(a)))) {
			snprintf(s, MAXJSON - 1, ",\n\t\t\t\"%s\": \"%s\"", "stemtype", s2);
			strncat(json, s, MAXJSON - strlen(json) - 1);
		}
		if(*(s2 = NameOfDerivtype(derivtype_of(a)))) {
			snprintf(s, MAXJSON - 1, ",\n\t\t\t\"%s\": \"%s\"", "derivtype", s2);
			strncat(json, s, MAXJSON - strlen(json) - 1);
		}

		s3[0] = 0;
		MorphNames(morphflags_of(a),s3," ", 0);
		if(*s3) {
			snprintf(s, MAXJSON - 1, ",\n\t\t\t\"%s\": \"%s\"", "morphology-names", s3);
			strncat(json, s, MAXJSON - strlen(json) - 1);
		}

		strncat(json, "\n\t\t}", MAXJSON - strlen(json) - 1);
	}

	strncat(json, "\n\t]\n}\n", MAXJSON - strlen(json) - 1);

	FreeGkword(w);

	return json;
}

int main()
{
	char line[BUFSIZ];
	char *s;

	while(fgets(line, BUFSIZ, stdin)) {
		line[strlen(line)-1] = 0; // cut newline

		s = analysis_as_json(line);
		printf("%s", s);
		free(s);
	}

	return 0;
}
