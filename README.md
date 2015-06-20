NodeJS C++ ArrayBuffer handler

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

## NodeJS version < 12

```
static ArrayBuffer* New(const char *str = 0)
static ArrayBuffer* New(const char *str, size_t length)
static ArrayBuffer* New(const std::string &data)
static ArrayBuffer* New(const v8::Local<v8::Object> &arrayBuffer)
static ArrayBuffer* New(const v8::Local<v8::Value> &arg)
  
v8::Local<v8::Object> ToArrayBuffer() const
v8::Local<v8::String> ToString() const

const char *ToUtf8() const
void *Data() const
size_t Length() const
size_t ByteLength() const
```

## NodeJS version >= 12

```
static ArrayBuffer* New(const char *str = 0)
static ArrayBuffer* New(const char *str, size_t length)
static ArrayBuffer* New(const std::string &data)
static ArrayBuffer* New(const v8::Local<v8::ArrayBuffer> &arrayBuffer)
static ArrayBuffer* New(const v8::Local<v8::Value> &arg)

static ArrayBuffer* New(v8::Isolate *isolate, const char *str = 0)
static ArrayBuffer* New(v8::Isolate *isolate, const char *str, size_t length)
static ArrayBuffer* New(v8::Isolate *isolate, const std::string &data)
static ArrayBuffer* New(v8::Isolate *isolate, const v8::Local<v8::ArrayBuffer> &arrayBuffer)
static ArrayBuffer* New(v8::Isolate *isolate, const v8::Local<v8::Value> &arg)

v8::Local<v8::ArrayBuffer> ToArrayBuffer(v8::Isolate *isolate = 0) const
v8::Local<v8::String> ToString(v8::Isolate *isolate = 0) const

const char *ToUtf8() const
void *Data() const
size_t Length() const
size_t ByteLength() const
```

## Example

[sample.cc](https://github.com/vmolsa/node-arraybuffer-example/blob/master/sample.cc)

[sample.js](https://github.com/vmolsa/node-arraybuffer-example/blob/master/sample.js)
