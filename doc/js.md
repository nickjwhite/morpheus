# morpheus.js

morpheus.js is a morpheus compiled down to javascript, so that it
can be used directly anywhere that javascript can run.

## Building morpheus.js

The first step is to build a minimal version of the stemlib, which
ensures fast loading of morpheus.js. Do that with this command (once
morpheus has been built):

```
cd stemlib/Latin
export PATH=$PATH:../../bin
MORPHLIB=.. make outonly
```

morpheus.js uses emscripten to compile, so the first step is to
download and install that from https://github.com/kripken/emscripten
and put the tools on your path, following the emscripten
documentation. On Linux this will be something like the following:

```
cd emscripten && . emsdk_env.sh
```

You can then rebuild morpheus with emscripten, using the normal make
tools, with this command:

```
cd morpheus/src
make clean
emmake make CC=emcc all
```

And to package it all together into the final javascript:

```
cd anal
emmake make CC=emcc morpheus.js
```

This will produce three files, *morpheus.js*, *morpheus.js.mem* and
*morpheus.data*.

## Using morpheus.js

Once built, morpheus.js can be loaded into the browser in the normal
way for javascript.

Once loaded, the function MorpheusAnalysis() is available, which is
passed a lemma as a string, and returns a morphological analysis in
JSON format.

You can see the morpheus.js in action in js/demo.html for a very
basic example of how to use it in a web context.

## morpheus.js on the command line

As well as using it in a web browser, morpheus.js can also be used
on the command line. This requires building it without a separate
.data file, like this:

```
cd anal
emmake make CC=emcc morpheus-embed.js
```

If you have nodejs installed, you can now run morpheus with a
command like this:

```
js morpheus-embed.js lemma
```
