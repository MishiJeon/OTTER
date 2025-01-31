#pragma once
#include <glad/glad.h>

/// <summary>
/// The possible options for our buffer types
/// </summary>
/// <see>https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml</see>
enum class BufferType {
	Vertex = GL_ARRAY_BUFFER,
	Index = GL_ELEMENT_ARRAY_BUFFER
};

/// <summary>
/// The possible options for our buffer usage hints
/// Stream: Contents will be modified once and used rarely
/// Static: Contents will be modified once and used regularly
/// Dynamic: Contents will be modified and used regularly
/// 
/// Draw: Content will be modified by our application and used by OpenGL
/// Read: Content will be filled with content by OpenGL to be read by our application
/// Copy: Content will be filled by OpenGL and used by other OpenGL commands (not optimized for application access)
/// </summary>
/// <see>https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml</see>
enum class BufferUsage {
	StreamDraw = GL_STREAM_DRAW,
	StreamRead = GL_STREAM_READ,
	StreamCopy = GL_STREAM_COPY,

	StaticDraw = GL_STATIC_DRAW,
	StaticRead = GL_STATIC_READ,
	StaticCopy = GL_STATIC_COPY,

	DynamicDraw = GL_DYNAMIC_DRAW,
	DynamicRead = GL_DYNAMIC_READ,
	DynamicCopy = GL_DYNAMIC_COPY,
};

/// <summary>
/// This is our abstract base class for all our OpenGL buffer types
/// </summary>
class IBuffer
{
public:
	// We'll disallow moving and copying, since we want to manually control when the destructor is called
	// We'll use these classes via pointers
	IBuffer(const IBuffer& other) = delete;
	IBuffer(IBuffer&& other) = delete;
	IBuffer& operator=(const IBuffer& other) = delete;
	IBuffer& operator=(IBuffer&& other) = delete;

public:
	/// <summary>
	/// Virtual destructor to allow child buffers to overload it when needed
	/// </summary>
	virtual ~IBuffer();

	/// <summary>
	/// Loads data into this buffer, using the bindless method glNamedBufferData
	/// </summary>
	/// <param name="data">The data that you want to load into the buffer</param>
	/// <param name="elementSize">The size of a single element, in bytes</param>
	/// <param name="elementCount">The number of elements to upload</param>
	virtual void LoadData(const void* data, size_t elementSize, size_t elementCount);

	/// <summary>
	/// Loads an array of data into this buffer, using the bindless method glNamedBufferData
	/// </summary>
	/// <typeparam name="T">The type of data you are uploading</typeparam>
	/// <param name="data">A pointer to the firest element in the array</param>
	/// <param name="count">The number of elements in the array to upload</param>
	template <typename T>
	void LoadData(const T* data, size_t count) {
		IBuffer::LoadData((const void*)(data), sizeof(T), count);
	}

	/// <summary>
	/// Returns the number of elements that are loaded into this buffer
	/// </summary>
	size_t GetElementCount() const { return _elementCount; }
	/// <summary>
	/// Returns the size in bytes of a single element in this buffer
	/// </summary>
	size_t GetElementSize() const { return _elementSize; }
	/// <summary>
	/// Returns the total size in bytes that this buffer occupies
	/// </summary>
	size_t GetTotalSize() const { return _elementCount * _elementSize; }
	/// <summary>
	/// Returns the type of buffer (ex GL_ARRAY_BUFFER, GL_ARRAY_ELEMENT_BUFFER, etc...)
	/// </summary>
	BufferType GetType() const { return _type; }
	/// <summary>
	/// Returns the usage hint for this buffer (ex GL_STATIC_DRAW, GL_DYNAMIC_DRAW)
	/// </summary>
	BufferUsage GetUsage() const { return _usage; }
	/// <summary>
	/// Returns the underlying OpenGL handle that this class is wrapping around
	/// </summary>
	GLuint GetHandle() const { return _handle; }

	/// <summary>
	/// Binds this buffer for use to the slot returned by GetType()
	/// </summary>
	virtual void Bind();
	/// <summary>
	/// Unbinds the buffer bound to the slot given by type
	/// </summary>
	/// <param name="type">The type or slot of buffer to unbind (ex: GL_ARRAY_BUFFER, GL_ARRAY_ELEMENT_BUFFER)</param>
	static void UnBind(BufferType type);

protected:
	/// <summary>
	/// Creates a new buffer with the given type and usage. Note that this is protected so only derived classes can call this
	/// </summary>
	/// <param name="type">The type of buffer (EX: GL_ARRAY_BUFFER, GL_ARRAY_ELEMENT_BUFFER)</param>
	/// <param name="usage">The usage hint for the buffer (EX: GL_STATIC_DRAW, GL_DYNAMIC_DRAW)</param>
	IBuffer(BufferType type, BufferUsage usage);

	size_t _elementSize; // The size or stride of our elements
	size_t _elementCount; // The number of elements in the buffer
	GLuint _handle; // The OpenGL handle for the underlying buffer
	BufferUsage _usage; // The buffer usage mode (GL_STATIC_DRAW, GL_DYNAMIC_DRAW)
	BufferType _type; // The buffer type (ex GL_ARRAY_BUFFER, GL_ARRAY_ELEMENT_BUFFER)
};