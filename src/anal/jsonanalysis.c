/* TODO:
 * - Don't hardcode language
 * - Use EMSCRIPTEN_KEEPALIVE or EXPORTED_FUNCTIONS for analysis_as_json()
 */

#include <stdio.h>
#include <prntflags.h> // for LATIN
#include <gkstring.h>
#include "../morphlib/gkstring.proto.h"
#include "../morphlib/morphkeys.proto.h"
#include "../morphlib/morphflags.proto.h"
#include "../morphlib/setlang.proto.h"
#include "checkstring.proto.h"
int quickflag = 0;

#define LENGTH(X) (sizeof X / sizeof X[0])

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

int analysis_as_json(char *in, char *json)
{
	gk_word *w = NULL;
	char s[BUFSIZ];
	char *s2;
	char s3[BUFSIZ];
	int i;
	int j;
	gk_analysis *a;
	word_form wf;

	s[0] = 0;
	s2 = 0;
	json[0] = 0;

	w = (gk_word *) CreatGkword(1);
	check_gkword(w, in);

	snprintf(json, BUFSIZ-1, "{\n\t\"%s\": [\n", in);

	for(i = 0; i < totanal_of(w); i++) {
		a = analysis_of(w)+i;
		wf = forminfo_of(a);

		if(i > 0) {
			strncat(json, ",\n", BUFSIZ - strlen(json) - 1);
		}
		strncat(json, "\t\t{\n", BUFSIZ - strlen(json) - 1);

		/* Broadly copied from DumpPerseusAnalysis() */
		snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "lemma", lemma_of(a));
		strncat(json, s, BUFSIZ - strlen(json) - 1);

		snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "work", workword_of(a));
		strncat(json, s, BUFSIZ - strlen(json) - 1);

		snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "raw", rawword_of(a));
		strncat(json, s, BUFSIZ - strlen(json) - 1);

		snprintf(s, BUFSIZ - 1, "\t\t\t\"type\": \"");
		if(Is_participle(a)) {
			strncat(s, "participle", BUFSIZ - strlen(s) - 1);
		} else if( Is_nounform(a) || Is_adjform(a) ) {
			if(isupper(lemma_of(a)[0]) && cur_lang() == GREEK) {
				strncat(s, "english", BUFSIZ - strlen(s) - 1);
			} else {
				strncat(s, "noun", BUFSIZ - strlen(s) - 1);
			}
		} else if(Is_verbform(a)) {
			strncat(s, "verb", BUFSIZ - strlen(s) - 1);
		} else {
			strncat(s, "indeclinable", BUFSIZ - strlen(s) - 1);
		}
		strncat(s, "\"", BUFSIZ - strlen(s) - 1);
		strncat(json, s, BUFSIZ - strlen(json) - 1);

		for(j = 0; j < LENGTH(forms); j++) {
			s2 = forms[j].fn(wf);
			if(*s2) {
				snprintf(s, BUFSIZ - 1, ",\n\t\t\t\"%s\": \"%s\"", forms[j].name, s2);
				strncat(json, s, BUFSIZ - strlen(json) - 1);
			}
			
		}

		for(j = 0; j < LENGTH(names); j++) {
			s3[0] = 0;
			names[j].fn(a, s3, " ");
			if(*s3) {
				snprintf(s, BUFSIZ - 1, ",\n\t\t\t\"%s\": \"%s\"", names[j].name, s3);
				strncat(json, s, BUFSIZ - strlen(json) - 1);
			}
		}

		if(*(s2 = NameOfStemtype(stemtype_of(a)))) {
			snprintf(s, BUFSIZ - 1, ",\n\t\t\t\"%s\": \"%s\"", "stemtype", s2);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}
		if(*(s2 = NameOfDerivtype(derivtype_of(a)))) {
			snprintf(s, BUFSIZ - 1, ",\n\t\t\t\"%s\": \"%s\"", "derivtype", s2);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}

		s3[0] = 0;
		MorphNames(morphflags_of(a),s3," ", 0);
		if(*s3) {
			snprintf(s, BUFSIZ - 1, ",\n\t\t\t\"%s\": \"%s\"", "morphology-names", s3);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}

		strncat(json, "\n\t\t}", BUFSIZ - strlen(json) - 1);
	}

	strncat(json, "\n\t]\n}\n", BUFSIZ - strlen(json) - 1);

	FreeGkword(w);

	return 0;
}

int main()
{
	char line[BUFSIZ];
	char *s;

	set_lang(LATIN);
	while(fgets(line, BUFSIZ, stdin)) {
		line[strlen(line)-1] = 0; // cut newline

		s = malloc(sizeof(char) * BUFSIZ);
		analysis_as_json(line, s);
		printf("%s", s);
		free(s);
	}

	return 0;
}
