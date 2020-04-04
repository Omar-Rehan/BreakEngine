#include "pch.h"
#include "Texture.h"


Texture::Texture(const char* imagePath, ImageMapType imageMapType, FileManager* pFileManager) : m_imageMapType(imageMapType), m_path(imagePath) {
    m_target = GL_TEXTURE_2D;
    CreateTextureObject(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

    unsigned long long readBytes = NULL;
    unsigned char* data = LoadDataFromFile(pFileManager, readBytes);
    if (data == nullptr) {
        std::cout << "Failed to load texture: " << m_path << std::endl;
        return;
    }
    
    ParseImageData(data, readBytes, true, false);
    
    delete[] data;
}
Texture::Texture(const TextureInfo& parameters, FileManager* pFileManager) : m_imageMapType(parameters.imageMapType), m_target(parameters.target) {
    CreateTextureObject(parameters.wrapOptionR, parameters.wrapOptionR, parameters.filterOptionMag, parameters.filterOptionMin);
    
    unsigned long long readBytes = NULL;
    unsigned char* data = LoadDataFromFile(pFileManager, readBytes);
    if (data == nullptr) {
        std::cout << "Failed to load texture: " << m_path << std::endl;
        return;
    }

    ParseImageData(data, readBytes, parameters.bVerticalFlip, parameters.bGenMipmap);

    delete[] data;
}

void Texture::CreateTextureObject(const GLint& wrapS, const GLint& wrapR, const GLint& magFilter, const GLint& minFilter) {
    /// Generate and bind texture
    glGenTextures(1, &m_id);
    glBindTexture(m_target, m_id);

    /// Set texture wrapping options
    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, wrapR);

    /// Set texture filtering options
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, minFilter);
}
unsigned char* Texture::LoadDataFromFile(FileManager* pFileManager, unsigned long long& readBytes) {
    File* pFile = pFileManager->CreateNewFile();
    pFile->OpenRead(m_path.c_str());
    
    unsigned char* buffer = new unsigned char[pFile->GetFileLength()];
    pFile->Read(buffer, 1, NULL, &readBytes);
    
    pFile->Close();
    pFileManager->DestroyFile(pFile);

    return buffer;
}
void Texture::ParseImageData(unsigned char* data, unsigned long long size, const bool& bFlipVertically, const bool& bGenMipMap) {
    if (bFlipVertically) stbi_set_flip_vertically_on_load(true);
    
    int width = NULL, height = NULL, numOfChannels = NULL;
    unsigned char* buffer = stbi_load_from_memory(data, size, &width, &height, &numOfChannels, 4);
    if (buffer == nullptr) {
        std::cout << "Error parsing the loaded image file" << std::endl;
        return;
    }
    
    GLenum format = NULL;
    {
        if (numOfChannels == 1)      format = GL_RED;
        else if (numOfChannels == 3) format = GL_RGB;
        else if (numOfChannels == 4) format = GL_RGBA;
    }
    glTexImage2D(m_target, NULL, format, width, height, NULL, format, GL_UNSIGNED_BYTE, buffer);
    
    if (bGenMipMap) glGenerateMipmap(m_target);
    
    delete[] buffer;
}

void Texture::Activate(ShaderProgram& shaderProgram, unsigned int textureUnit, const std::string& uniformName) const {
    /// Set uniform to texture unit
    shaderProgram.SetIntUniform(uniformName, textureUnit);

    /// Bind texture object to texture unit
    shaderProgram.Activate();
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(m_target, m_id);
}

const ImageMapType Texture::GetImageMapType() const {
    return m_imageMapType;
}
const std::string& Texture::GetPath() const {
    return m_path;
}