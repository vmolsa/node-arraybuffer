# node-arraybuffer
node-v0.12.0 > v8::ArrayBuffer handler

## Install

```
npm install node-arraybuffer --save
```

## Usage

Add node-arraybuffer to your project binding.gyp 

```
"include_dirs" : [
    "<!(node -e \"require('node-arraybuffer')\")"
]
```

And add #include header in source code

```
#include <ArrayBuffer.h>
```

## Example

http://github.com/vmolsa/node-arraybuffer-example
