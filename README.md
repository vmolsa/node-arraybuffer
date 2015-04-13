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

## Static Public Member Functions

```
ArrayBuffer* New(v8::Isolate *isolate = 0, void *ptr = 0, size_t length = 0, bool release = false)
ArrayBuffer* New(v8::Isolate *isolate, const char *ptr, int length = -1, bool release = false)
ArrayBuffer* New(v8::Isolate *isolate, const v8::Local<v8::ArrayBuffer> &arrayBuffer)
ArrayBuffer* New(v8::Isolate *isolate, const v8::Local<v8::Value> &arg)
```

## Static Template Public Member Functions

```
ArrayBuffer* New(v8::Isolate *isolate, const T &content)
ArrayBuffer* New(v8::Isolate *isolate, const T &content, void *ptr, size_t length = 0)
ArrayBuffer* New(v8::Isolate *isolate, const T &content, const char *ptr, int length = -1)
```

## Public Member Functions

```
v8::Local<v8::ArrayBuffer> ToArrayBuffer() const
v8::Local<v8::String> ToString() const
const char *ToUtf8() const
void *Data() const
size_t Length() const
size_t ByteLength() const
```

## Public Template Member Functions

```
const T &Unwrap() const
```

## Example

[sample.cc](https://github.com/vmolsa/node-arraybuffer-example/blob/master/sample.cc)
[sample.js](https://github.com/vmolsa/node-arraybuffer-example/blob/master/sample.js)
