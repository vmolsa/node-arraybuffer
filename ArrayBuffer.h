/*
* The MIT License (MIT)
*
* Copyright (c) 2015 vmolsa <ville.molsa@gmail.com> (http://github.com/vmolsa)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* static ArrayBuffer* New(void *ptr = 0, size_t length = 0, bool release = false)
* static ArrayBuffer* New(const char *ptr, int length = -1, bool release = false)
* static ArrayBuffer* New(const char *ptr, size_t length = 0, bool release = false)
* static ArrayBuffer* New(char *ptr, size_t length = 0, bool release = false)
* static ArrayBuffer* New(const v8::Local<v8::ArrayBuffer> &arrayBuffer)
* static ArrayBuffer* New(const T &content)
* static ArrayBuffer* New(const T &content, const char *ptr, size_t length)
* static ArrayBuffer* New(const T &content, void *ptr, size_t length = 0)
* static ArrayBuffer* New(const T &content, const char *ptr, int length = -1)
* static ArrayBuffer* New(const v8::Local<v8::Value> &arg)
*
* static ArrayBuffer* New(v8::Isolate *isolate = 0, void *ptr = 0, size_t length = 0, bool release = false)
* static ArrayBuffer* New(v8::Isolate *isolate, const char *ptr, int length = -1, bool release = false)
* static ArrayBuffer* New(v8::Isolate *isolate, const char *ptr, size_t length, bool release = false)
* static ArrayBuffer* New(v8::Isolate *isolate, char *ptr, size_t length = 0, bool release = false)
* static ArrayBuffer* New(v8::Isolate *isolate, const v8::Local<v8::ArrayBuffer> &arrayBuffer)
* static ArrayBuffer* New(v8::Isolate *isolate, const T &content)
* static ArrayBuffer* New(v8::Isolate *isolate, const T &content, void *ptr, size_t length = 0)
* static ArrayBuffer* New(v8::Isolate *isolate, const T &content, const char *ptr, size_t length)
* static ArrayBuffer* New(v8::Isolate *isolate, const T &content, const char *ptr, int length = -1)
* static ArrayBuffer* New(v8::Isolate *isolate, const v8::Local<v8::Value> &arg)
*
* ArrayBufferWrapper(v8::Isolate *isolate, const T &content, void *ptr = 0, size_t length = 0)
* ArrayBufferWrapper(const T &content, void *ptr = 0, size_t length = 0)
*
* v8::Local<v8::ArrayBuffer> ToArrayBuffer() const
* v8::Local<v8::String> ToString() const
* void *Data() const
* size_t Length() const
* size_t ByteLength() const
* const T &Unwrap() const
*
*/

#ifndef NODE_ARRAYBUFFER_H
#define NODE_ARRAYBUFFER_H

#ifdef WIN32
#pragma warning( disable : 4267 )
#endif

#include <node.h>
#include <v8.h>
#include <string>

namespace node {
#if NODE_MINOR_VERSION >= 11

  class ArrayBuffer {
  public:
    inline static ArrayBuffer* New(const char *str = 0) {
      return ArrayBuffer::New(v8::Isolate::GetCurrent(), std::string(str));
    }

    inline static ArrayBuffer* New(const char *str, size_t length) {
      return ArrayBuffer::New(v8::Isolate::GetCurrent(), str, length);
    }

    inline static ArrayBuffer* New(const std::string &data) {
      return ArrayBuffer::New(v8::Isolate::GetCurrent(), data.data(), data.size());
    }

    inline static ArrayBuffer* New(const v8::Local<v8::ArrayBuffer> &arrayBuffer) {
      return ArrayBuffer::New(v8::Isolate::GetCurrent(), arrayBuffer);
    }

    inline static ArrayBuffer* New(const v8::Local<v8::Value> &arg) {
      return ArrayBuffer::New(v8::Isolate::GetCurrent(), arg);
    }

    inline static ArrayBuffer* New(v8::Isolate *isolate, const std::string &data) {
      return ArrayBuffer::New(isolate, data.data(), data.size());
    }

    inline static ArrayBuffer* New(v8::Isolate *isolate, const char *str = 0) {
      return ArrayBuffer::New(isolate, std::string(str));
    }

    inline static ArrayBuffer* New(v8::Isolate *isolate, const char *str, size_t length) {
      if (!isolate) {
        isolate = v8::Isolate::GetCurrent();
      }

      ArrayBuffer *buffer = new ArrayBuffer();
      v8::Local<v8::ArrayBuffer> arrayBuffer;

      buffer->_len = length;

      if (length) {
        buffer->_data = new char[length + 1];
        buffer->_data[length] = '\0';

        for (size_t index = 0; index < length; index++) {
          buffer->_data[index] = str[index];
        }

        arrayBuffer = v8::ArrayBuffer::New(isolate, buffer->_data, length);
      }
      else {
        arrayBuffer = v8::ArrayBuffer::New(isolate, length);
      }

      buffer->_arrayBuffer.Reset(isolate, arrayBuffer);
      buffer->_arrayBuffer.SetWeak(buffer, ArrayBuffer::onDispose);
      buffer->_arrayBuffer.MarkIndependent();

      arrayBuffer->SetHiddenValue(v8::String::NewFromUtf8(isolate, "node::ArrayBuffer"), v8::External::New(isolate, buffer));
      return buffer;
    }

    inline static ArrayBuffer* New(v8::Isolate *isolate, const v8::Local<v8::ArrayBuffer> &arrayBuffer) {
      if (!isolate) {
        isolate = v8::Isolate::GetCurrent();
      }

      if (arrayBuffer.IsEmpty()) {
        return ArrayBuffer::New(isolate);
      }

      if (arrayBuffer->IsExternal()) {
        v8::Local<v8::Value> ptr = arrayBuffer->GetHiddenValue(v8::String::NewFromUtf8(isolate, "node::ArrayBuffer"));

        if (ptr.IsEmpty()) {
          v8::Local<v8::Value> uintArray = v8::Uint8Array::New(arrayBuffer, 0, arrayBuffer->ByteLength());
          return ArrayBuffer::New(isolate, uintArray);
        }
        else {
          v8::Local<v8::External> ext = v8::Local<v8::External>::Cast(ptr);
          return static_cast<ArrayBuffer*>(ext->Value());
        }
      }
      else {
        ArrayBuffer *buffer = new ArrayBuffer();
        v8::ArrayBuffer::Contents content = arrayBuffer->Externalize();

        buffer->_data = static_cast<char*>(content.Data());
        buffer->_len = content.ByteLength();
        buffer->_arrayBuffer.Reset(isolate, arrayBuffer);
        buffer->_arrayBuffer.SetWeak(buffer, ArrayBuffer::onDispose);
        buffer->_arrayBuffer.MarkIndependent();

        arrayBuffer->SetHiddenValue(v8::String::NewFromUtf8(isolate, "node::ArrayBuffer"), v8::External::New(isolate, buffer));

        return buffer;
      }

      return 0;
    }

    inline static ArrayBuffer* New(v8::Isolate *isolate, const v8::Local<v8::Value> &arg) {
      if (!arg.IsEmpty()) {
        if (arg->IsArrayBuffer() || arg->IsTypedArray()) {
          v8::Local<v8::ArrayBuffer> arrayBuffer;

          if (arg->IsArrayBuffer()) {
            arrayBuffer = v8::Local<v8::ArrayBuffer>::Cast(arg);
          }
          else {
            v8::Local<v8::ArrayBufferView> view = v8::Local<v8::ArrayBufferView>::Cast(arg);
            arrayBuffer = view->Buffer();
          }

          return ArrayBuffer::New(isolate, arrayBuffer);
        }

        if (arg->IsString()) {
          v8::String::Utf8Value str(arg->ToString());
          return ArrayBuffer::New(isolate, *str, str.length());
        }
      }

      return ArrayBuffer::New(isolate);
    }

    inline v8::Local<v8::ArrayBuffer> ToArrayBuffer(v8::Isolate *isolate = 0) const {
      if (!isolate) {
        isolate = v8::Isolate::GetCurrent();
      }

      v8::EscapableHandleScope scope(isolate);
      return scope.Escape(v8::Local<v8::ArrayBuffer>::New(isolate, _arrayBuffer));
    }

    inline v8::Local<v8::String> ToString(v8::Isolate *isolate = 0) const {
      if (!isolate) {
        isolate = v8::Isolate::GetCurrent();
      }

      v8::EscapableHandleScope scope(isolate);
      v8::Local<v8::String> retval = v8::String::NewFromUtf8(isolate,
        ArrayBuffer::ToUtf8(),
        v8::String::kNormalString,
        ArrayBuffer::Length());
      return scope.Escape(retval);
    }

    inline const char *ToUtf8() const {
      return _data;
    }

    inline void *Data() const {
      return _data;
    }

    inline size_t Length() const {
      return _len;
    }

    inline size_t ByteLength() const {
      return _len;
    }

    static inline void onDispose(const v8::WeakCallbackData<v8::ArrayBuffer, ArrayBuffer> &info) {
      v8::Isolate *isolate = info.GetIsolate();
      v8::HandleScope scope(isolate);

      ArrayBuffer *wrap = info.GetParameter();

      if (wrap) {
        v8::Local<v8::ArrayBuffer> arrayBuffer = v8::Local<v8::ArrayBuffer>::New(isolate, wrap->_arrayBuffer);
        wrap->_arrayBuffer.Reset();

        if (!arrayBuffer.IsEmpty()) {
          arrayBuffer->DeleteHiddenValue(v8::String::NewFromUtf8(isolate, "node::ArrayBuffer"));
        }

        delete wrap;
      }
    }

  private:
    virtual ArrayBuffer::~ArrayBuffer() {
      if (_len) {
        delete[] _data;
      }
    }

  protected:
    char* _data;
    size_t _len;
    v8::Persistent<v8::ArrayBuffer> _arrayBuffer;
  };

#else


#endif

};

#endif
