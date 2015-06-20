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

namespace node {
  template<class T> class ArrayBufferWrapper;

#if NODE_MINOR_VERSION >= 11

  class ArrayBuffer {
    template<class T> friend class ArrayBufferWrapper;

  public:
    inline static ArrayBuffer* New(void *ptr = 0, size_t length = 0, bool release = false) {
      return ArrayBuffer::New(v8::Isolate::GetCurrent(), ptr, length, release);
    }

    inline static ArrayBuffer* New(const char *ptr, int length = -1, bool release = false) {
      return ArrayBuffer::New(v8::Isolate::GetCurrent(), ptr, length, release);
    }

    inline static ArrayBuffer* New(const char *ptr, size_t length, bool release = false) {
      return ArrayBuffer::New(v8::Isolate::GetCurrent(), ptr, length, release);
    }

    inline static ArrayBuffer* New(char *ptr, size_t length = 0, bool release = false) {
      return ArrayBuffer::New(v8::Isolate::GetCurrent(), ptr, length, release);
    }

    inline static ArrayBuffer* New(const v8::Local<v8::ArrayBuffer> &arrayBuffer) {
      return ArrayBuffer::New(v8::Isolate::GetCurrent(), arrayBuffer);
    }

    template<class T>
    inline static ArrayBuffer* New(const T &content) {
      return ArrayBuffer::New(v8::Isolate::GetCurrent(), content);
    }

    template<class T>
    inline static ArrayBuffer* New(const T &content, void *ptr, size_t length) {
      return ArrayBuffer::New(v8::Isolate::GetCurrent(), content, ptr, length);
    }

    template<class T>
    inline static ArrayBuffer* New(const T &content, const char *ptr, size_t length) {
      return ArrayBuffer::New(v8::Isolate::GetCurrent(), content, ptr, length);
    }

    template<class T>
    inline static ArrayBuffer* New(const T &content, const char *ptr, int length = -1) {
      return ArrayBuffer::New(v8::Isolate::GetCurrent(), content, ptr, length);
    }

    inline static ArrayBuffer* New(const v8::Local<v8::Value> &arg) {
      return ArrayBuffer::New(v8::Isolate::GetCurrent(), arg);
    }

    inline static ArrayBuffer* New(v8::Isolate *isolate = 0, void *ptr = 0, size_t length = 0, bool release = false) {
      return ArrayBuffer::New(isolate, ptr, length, release);
    }

    inline static ArrayBuffer* New(v8::Isolate *isolate, const char *ptr, int length = -1, bool release = false) {
      if (length < 0) {
        for (length = 0; ptr && (ptr[length] || ptr[length] != '\0'); length++) {}
      }

      return ArrayBuffer::New(isolate, const_cast<char*>(ptr), static_cast<size_t>(length), release);
    }

    inline static ArrayBuffer* New(v8::Isolate *isolate, char *ptr, size_t length = 0, bool release = false) {
      ArrayBuffer *data = new ArrayBuffer(isolate);
      v8::Local<v8::ArrayBuffer> arrayBuffer = v8::ArrayBuffer::New(data->_isolate, ptr, length);

      data->_rel = release;
      data->_len = length;
      data->_data = ptr;
      data->_arrayBuffer.Reset(isolate, arrayBuffer);
      data->_arrayBuffer.SetWeak(data, ArrayBuffer::onDispose);
      data->_arrayBuffer.MarkIndependent();

      arrayBuffer->SetHiddenValue(v8::String::NewFromUtf8(data->_isolate, "node::ArrayBuffer"), v8::External::New(data->_isolate, data));
      return data;
    }

    inline static ArrayBuffer* New(v8::Isolate *isolate, const char *ptr, size_t length, bool release = false) {
      return ArrayBuffer::New(isolate, const_cast<char*>(ptr), length, release);
    }

    inline static ArrayBuffer* New(v8::Isolate *isolate, const v8::Local<v8::ArrayBuffer> &arrayBuffer) {
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
        ArrayBuffer *data = new ArrayBuffer(isolate);
        v8::ArrayBuffer::Contents content = arrayBuffer->Externalize();

        data->_rel = true;
        data->_len = content.ByteLength();
        data->_data = static_cast<char*>(content.Data());
        data->_arrayBuffer.Reset(data->_isolate, arrayBuffer);
        data->_arrayBuffer.SetWeak(data, ArrayBuffer::onDispose);
        data->_arrayBuffer.MarkIndependent();

        arrayBuffer->SetHiddenValue(v8::String::NewFromUtf8(data->_isolate, "node::ArrayBuffer"),
          v8::External::New(data->_isolate, data));

        return data;
      }

      return 0;
    }

    template<class T>
    inline static ArrayBuffer* New(v8::Isolate *isolate, const T &content) {
      return ArrayBuffer::New(isolate, content, content.data(), content.size());
    }

    template<class T>
    inline static ArrayBuffer* New(v8::Isolate *isolate, const T &content, void *ptr, size_t length = 0) {
      ArrayBufferWrapper<T> *ret = new ArrayBufferWrapper<T>(isolate, content, ptr, length);
      return static_cast<ArrayBuffer*>(ret);
    }

    template<class T>
    inline static ArrayBuffer* New(v8::Isolate *isolate, const T &content, const char *ptr, size_t length) {
      ArrayBufferWrapper<T> *ret = new ArrayBufferWrapper<T>(isolate, content, const_cast<char*>(ptr), length);
      return static_cast<ArrayBuffer*>(ret);
    }

    template<class T>
    inline static ArrayBuffer* New(v8::Isolate *isolate, const T &content, const char *ptr, int length = -1) {
      if (length < 0) {
        for (length = 0; ptr && (ptr[length] || ptr[length] != '\0'); length++) {}
      }

      ArrayBufferWrapper<T> *ret = new ArrayBufferWrapper<T>(isolate, content, const_cast<char*>(ptr), static_cast<size_t>(length));
      return static_cast<ArrayBuffer*>(ret);
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
          int length = str.length();
          const char *ptr = *str;
          char *data = 0;

          if (length > 0) {
            data = new char[length];
            data[length] = '\0';

            for (int index = 0; index < length; index++) {
              data[index] = ptr[index];
            }
          }

          return ArrayBuffer::New(isolate, ptr, length, (length > 0));
        }
      }

      return ArrayBuffer::New(isolate);
    }

    inline v8::Local<v8::ArrayBuffer> ToArrayBuffer() const {
      v8::EscapableHandleScope scope(_isolate);
      return scope.Escape(v8::Local<v8::ArrayBuffer>::New(_isolate, _arrayBuffer));
    }

    inline v8::Local<v8::String> ToString() const {
      v8::EscapableHandleScope scope(_isolate);
      v8::Local<v8::String> retval = v8::String::NewFromUtf8(_isolate,
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

    template<class T> inline const T &Unwrap() const {
      static T nowrap;

      if (_wrap) {
        const ArrayBufferWrapper<T> *ptr = static_cast<const ArrayBufferWrapper<T>*>(this);
        return ptr->_content;
      }

      nowrap = T();
      return nowrap;
    }

  private:
    explicit ArrayBuffer(v8::Isolate *isolate = 0) :
      _rel(false),
      _wrap(false),
      _len(0),
      _data(0),
      _isolate(isolate)
    {
      if (!_isolate) {
        _isolate = v8::Isolate::GetCurrent();
      }
    }

    virtual ~ArrayBuffer() {
      if (_rel && _data) {
        delete[] _data;
      }
    }

    static inline void onDispose(const v8::WeakCallbackData<v8::ArrayBuffer, ArrayBuffer> &info) {
      v8::Isolate *isolate = info.GetIsolate();
      v8::HandleScope scope(isolate);

      ArrayBuffer *wrap = info.GetParameter();

      v8::Local<v8::ArrayBuffer> arrayBuffer = v8::Local<v8::ArrayBuffer>::New(isolate, wrap->_arrayBuffer);
      wrap->_arrayBuffer.Reset();

      if (!arrayBuffer.IsEmpty()) {
        arrayBuffer->DeleteHiddenValue(v8::String::NewFromUtf8(isolate, "node::ArrayBuffer"));
      }

      delete wrap;
    }

  protected:
    bool _rel;
    bool _wrap;
    size_t _len;
    char* _data;
    v8::Isolate* _isolate;
    v8::Persistent<v8::ArrayBuffer> _arrayBuffer;
  };

#else

  class ArrayBuffer {
    template<class T> friend class ArrayBufferWrapper;
  public:
    inline static ArrayBuffer* New(void *ptr = 0, size_t length = 0, bool release = false) {
      return ArrayBuffer::New(isolate, ptr, length, release);
    }

    inline static ArrayBuffer* New(const char *ptr, int length = -1, bool release = false) {
      if (length < 0) {
        for (length = 0; ptr && (ptr[length] || ptr[length] != '\0'); length++) {}
      }

      return ArrayBuffer::New(const_cast<char*>(ptr), static_cast<size_t>(length), release);
    }

    inline static ArrayBuffer* New(const char *ptr, size_t length = 0, bool release = false) {
      return ArrayBuffer::New(const_cast<char*>(ptr), length, release);
    }

    inline static ArrayBuffer* New(char *ptr, size_t length = 0, bool release = false) {
      ArrayBuffer *data = new ArrayBuffer();

      v8::Local<v8::Object> global = v8::Context::GetCurrent()->Global();
      v8::Local<v8::Object> constructor = v8::Object::Cast(global->Get(v8::String::New("ArrayBuffer")));
      v8::Local<v8::Object> arrayBuffer = constructor->CallAsConstructor();

      arrayBuffer->SetIndexedPropertiesToExternalArrayData(ptr, v8::kExternalByteArray, length);

      data->_rel = release;
      data->_len = length;
      data->_data = ptr;
      data->_arrayBuffer.Reset(arrayBuffer);
      data->_arrayBuffer.SetWeak(data, ArrayBuffer::onDispose);
      data->_arrayBuffer.MarkIndependent();

      arrayBuffer->SetHiddenValue(v8::String::New("node::ArrayBuffer"), v8::External::New(data));

      return data;
    }

    inline static ArrayBuffer* New(const v8::Local<v8::Object> &arrayBuffer) {
      return 0;
    }

    template<class T>
    inline static ArrayBuffer* New(const T &content) {
      return ArrayBuffer::New(content, content.data(), content.size());
    }

    template<class T>
    inline static ArrayBuffer* New(const T &content, void *ptr, size_t length = 0) {
      ArrayBufferWrapper<T> *ret = new ArrayBufferWrapper<T>(isolate, content, ptr, length);
      return static_cast<ArrayBuffer*>(ret);
    }

    template<class T>
    inline static ArrayBuffer* New(const T &content, const char *ptr, size_t length) {
      ArrayBufferWrapper<T> *ret = new ArrayBufferWrapper<T>(isolate, content, const_cast<char*>(ptr), length);
      return static_cast<ArrayBuffer*>(ret);
    }

    template<class T>
    inline static ArrayBuffer* New(const T &content, const char *ptr, int length = -1) {
      if (length < 0) {
        for (length = 0; ptr && (ptr[length] || ptr[length] != '\0'); length++) {}
      }

      ArrayBufferWrapper<T> *ret = new ArrayBufferWrapper<T>(isolate, content, const_cast<char*>(ptr), static_cast<size_t>(length));
      return static_cast<ArrayBuffer*>(ret);
    }

    inline static ArrayBuffer* New(const v8::Local<v8::Value> &arg) {
      return 0;
    }

    inline v8::Local<v8::ArrayBuffer> ToArrayBuffer() const {
      v8::HandleScope scope;
      return scope.Close(v8::Local<v8::Object>::New(_arrayBuffer));
    }

    inline v8::Local<v8::String> ToString() const {
      v8::HandleScope scope;
      return scope.Close(v8::String::New(ArrayBuffer::ToUtf8(), ArrayBuffer::Length()));
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

    inline const T &Unwrap() const {
      static T nowrap;

      if (_wrap) {
        const ArrayBufferWrapper<T> *ptr = static_cast<const ArrayBufferWrapper<T>*>(this);
        return ptr->_content;
      }

      nowrap = T();
      return nowrap;
    }

  private:
    explicit ArrayBuffer() :
      _rel(false),
      _wrap(false),
      _len(0),
      _data(0),
      {}

      virtual ~ArrayBuffer() {
      if (_rel && _data) {
        delete[] _data;
      }
    }

    static inline void onDispose(const v8::WeakCallbackData<v8::Object, ArrayBuffer> &info) {
      v8::HandleScope scope;
      ArrayBuffer *wrap = info.GetParameter();

      v8::Local<v8::Object> arrayBuffer = v8::Local<v8::Object>::New(wrap->_arrayBuffer);
      wrap->_arrayBuffer.Reset();

      if (!arrayBuffer.IsEmpty()) {
        arrayBuffer->DeleteHiddenValue(v8::String::New("node::ArrayBuffer"));
      }

      delete wrap;
    }

  protected:
    bool _rel;
    bool _wrap;
    size_t _len;
    char* _data;
    v8::Persistent<v8::Object> _arrayBuffer;
  };

#endif

  template<class T> class ArrayBufferWrapper : public ArrayBuffer {
    friend class ArrayBuffer;

  private:
#if NODE_MINOR_VERSION >= 11
    explicit ArrayBufferWrapper(v8::Isolate *isolate, const T &content, char *ptr = 0, size_t length = 0) :
      ArrayBuffer(isolate),
      _content(content)
    {
      v8::Local<v8::ArrayBuffer> arrayBuffer = v8::ArrayBuffer::New(_isolate, ptr, length);

      _wrap = true;
      _len = length;
      _data = ptr;
      _arrayBuffer.Reset(_isolate, arrayBuffer);
      _arrayBuffer.SetWeak(static_cast<ArrayBuffer*>(this), ArrayBuffer::onDispose);
      _arrayBuffer.MarkIndependent();

      arrayBuffer->SetHiddenValue(v8::String::NewFromUtf8(_isolate, "node::ArrayBuffer"), v8::External::New(_isolate, this));
    }
#else
    explicit ArrayBufferWrapper(const T &content, char *ptr = 0, size_t length = 0) :
      _content(content)
    {
      v8::Local<v8::Object> global = v8::Context::GetCurrent()->Global();
      v8::Local<v8::Object> constructor = v8::Object::Cast(global->Get(v8::String::New("ArrayBuffer")));
      v8::Local<v8::Object> arrayBuffer = constructor->CallAsConstructor();

      arrayBuffer->SetIndexedPropertiesToExternalArrayData(ptr, v8::kExternalByteArray, length);

      _wrap = true;
      _len = length;
      _data = ptr;
      _arrayBuffer.Reset(arrayBuffer);
      _arrayBuffer.SetWeak(static_cast<ArrayBuffer*>(this), ArrayBuffer::onDispose);
      _arrayBuffer.MarkIndependent();

      arrayBuffer->SetHiddenValue(v8::String::New("node::ArrayBuffer"), v8::External::New(this));
    }
#endif

    virtual ~ArrayBufferWrapper() { }

  protected:
    T _content;
  };
};

#endif
