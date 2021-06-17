# mwa::WebApiClient

## What is it?

It's a primitive, opinionated, and very unrobust quick
way to make use of a web api from within a C++ application.

If you were expecting something more robust, with error
handling and bunch of other features, check out libcurl.

## Why'd you make it?

I wanted a quick and easy way to make use of a custom web
api from within my applications. So here we are.

## Usage

see ``mwaExample.cpp``

## Build - Linux

### Install Toolchain & Dependencies
```bash
sudo apt-get update
sudo apt-get install build-essential libssl-dev
```

### Example compile command
```bash
g++ mwaExample.cpp -o mwaExample -lssl -lcrypto 
```

## Build - Windows (Microsoft Visual Studio 2019)

### Acquire/Build/Install Dependencies
I haven't got the patience to write detailed
instructions to acquire and/or build the
dependencies. But you need **libssl**

## Build - Emscripten

```bash
em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s -s ASYNCIFY -s 'ASYNCIFY_IMPORTS=["mwa_do_fetch"]' mwaExample.cpp -o mwaExample.html
```
