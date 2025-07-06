#include <GLES3/gl3.h>

template<typename T>
struct Buffer
{
    GLuint m_VBO{0}; // Vertex Buffer Object for storing point cloud data
    std::vector<T> m_Data{}; // Data to be stored in the buffer

    Buffer() = default;

    Buffer(Buffer&& other) noexcept : m_VBO(other.m_VBO), m_Data(std::move(other.m_Data))
    {
        other.m_VBO = 0; // Reset the moved-from object's VBO to avoid double deletion
        other.m_Data.clear(); // Clear the data in the moved-from object
    }

    Buffer(const Buffer& other) = delete; // Disable copy constructor

    Buffer(const std::vector<T>& data) : m_Data(data)
    {
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_Data.size() * sizeof(T), m_Data.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    }
};