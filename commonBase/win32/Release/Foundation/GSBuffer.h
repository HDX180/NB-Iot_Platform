#ifndef GSBUFFER_DEF_H
#define GSBUFFER_DEF_H

#include "GSDefs.h"


template <class T>
class GSBuffer
	/// A very simple buffer class that allocates a buffer of
	/// a given type and size in the constructor and
	/// deallocates the buffer in the destructor.
	///
	/// This class is useful everywhere where a temporary buffer
	/// is needed.
{
public:
	GSBuffer(std::size_t size):
	  _size(size),
		  _ptr(new T[size])
		  /// Creates and allocates the Buffer.
	  {
	  }

	  ~GSBuffer()
		  /// Destroys the Buffer.
	  {
		  delete [] _ptr;
	  }

	  void resize(std::size_t newSize, bool preserveContent = true)
		  /// Resizes the buffer. If preserveContent is true,
		  /// the content of the old buffer is copied over to the
		  /// new buffer. NewSize can be larger or smaller than
		  /// the current size, but it must not be 0.
	  {
		  T* ptr = new T[newSize];
		  if (preserveContent)
		  {
			  std::size_t n = newSize > _size ? _size : newSize;
			  std::memcpy(ptr, _ptr, n);
		  }
		  delete [] _ptr;
		  _ptr  = ptr;
		  _size = newSize;
	  }

	  std::size_t size() const
		  /// Returns the size of the buffer.
	  {
		  return _size;
	  }

	  T* begin()
		  /// Returns a pointer to the beginning of the buffer.
	  {
		  return _ptr;
	  }

	  const T* begin() const
		  /// Returns a pointer to the beginning of the buffer.
	  {
		  return _ptr;
	  }

	  T* end()
		  /// Returns a pointer to end of the buffer.
	  {
		  return _ptr + _size;
	  }

	  const T* end() const
		  /// Returns a pointer to the end of the buffer.
	  {
		  return _ptr + _size;
	  }

	  T& operator [] (std::size_t index)
	  {
		  GSAssert(index < _size);

		  return _ptr[index];
	  }

	  const T& operator [] (std::size_t index) const
	  {
		  GSAssert(index < _size);

		  return _ptr[index];
	  }

private:
	GSBuffer();
	GSBuffer(const GSBuffer&);
	GSBuffer& operator = (const GSBuffer&);

	std::size_t _size;
	T* _ptr;
};

#endif // GSBUFFER_DEF_H