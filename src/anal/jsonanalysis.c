/* TODO:
 * - Don't hardcode language
 * - Get the struct based method working, and expand it to the
 *   other generalisable things
 * - Use EMSCRIPTEN_KEEPALIVE or EXPORTED_FUNCTIONS for analysis_as_json()
 */

#include <stdio.h>
#include <prntflags.h> // for LATIN
#include <gkstring.h>
int quickflag = 0;

#define LENGTH(X) (sizeof X / sizeof X[0])

typedef struct {
	char (*fn)(word_form);
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
			strncat(s, "I", BUFSIZ - strlen(s) - 1);
		}
		strncat(s, "\",\n", BUFSIZ - strlen(s) - 1);
		strncat(json, s, BUFSIZ - strlen(json) - 1);


		/* For some reason this segfaults, so just do this the messy way for now */
	/*	for(j = 0; j < LENGTH(forms); j++) {
			s2 = forms[j].fn(wf);
			if(*s2) {
				snprintf(s, BUFSIZ - 1, "\"%s\": \"%s\",", forms[j].name, s2);
			}
			
		}
	*/

		if(*(s2 = NameOfTense(wf))) {
			snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "tense", s2);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}
		if(*(s2 = NameOfMood(wf))) {
			snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "mood", s2);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}
		if(*(s2 = NameOfVoice(wf))) {
			snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "voice", s2);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}
		if(*(s2 = NameOfGender(wf))) {
			snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "gender", s2);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}
		if(*(s2 = NameOfCase(wf))) {
			snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "case", s2);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}
		if(*(s2 = NameOfDegree(wf))) {
			snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "degree", s2);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}
		if(*(s2 = NameOfPerson(wf))) {
			snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "person", s2);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}
		if(*(s2 = NameOfNumber(wf))) {
			snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "number", s2);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}

		s3[0] = 0;
		DialectNames(dialect_of(a),s3," ");
		if(*s3) {
			snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "dialects", s3);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}

		s3[0] = 0;
		GeogRegionNames(geogregion_of(a),s3," ");
		if(*s3) {
			snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "geographic-region", s3);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}

		s3[0] = 0;
		DomainNames(domains_of(a),s3," ");
		if(*s3) {
			snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "domains", s3);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}

		s3[0] = 0;
		MorphNames(morphflags_of(a),s3," ", 0);
		if(*s3) {
			snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "morphology-names", s3);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}

		if(*(s2 = NameOfStemtype(stemtype_of(a)))) {
			snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "stemtype", s2);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}
		if(*(s2 = NameOfDerivtype(derivtype_of(a)))) {
			snprintf(s, BUFSIZ - 1, "\t\t\t\"%s\": \"%s\",\n", "derivtype", s2);
			strncat(json, s, BUFSIZ - strlen(json) - 1);
		}

		strncat(json, "\t\t}", BUFSIZ - strlen(json) - 1);
	}

	strncat(json, "\n\t]\n}\n", BUFSIZ - strlen(json) - 1);

	FreeGkword(w);

	return 0;
}

main()
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
}

