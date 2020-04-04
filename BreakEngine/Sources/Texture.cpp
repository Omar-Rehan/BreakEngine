#include "pch.h"
#include "Texture.h"


Texture::Texture(const char* imagePath, ImageMapType imageMapType) : m_imageMapType(imageMapType), m_path(imagePath) {
    m_target = GL_TEXTURE_2D;
   
    SetTextureObjectProperties(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    LoadDataFromFileIntoTextureObject(true, false);
}
Texture::Texture(const TextureInfo& parameters) : m_imageMapType(parameters.imageMapType), m_target(parameters.target) {    
    SetTextureObjectProperties(parameters.wrapOptionR, parameters.wrapOptionR, parameters.filterOptionMag, parameters.filterOptionMin);
    LoadDataFromFileIntoTextureObject(parameters.bVerticalFlip, parameters.bGenMipmap);
}

void Texture::LoadDataFromFileIntoTextureObject(const bool& bFlipVertically, const bool& bGenMipMap) {
    /*al_init();
    al_init_image_addon();
    al_set_physfs_file_interface();

    ALLEGRO_BITMAP* bitmap = al_load_bitmap(m_path.c_str());
    if (bitmap == nullptr) {
        std::cout << "Failed to load texture: " << m_path << std::endl;
        return;
    }
    
    GLuint texID = al_get_opengl_texture(bitmap);
    if (texID == NULL) {
        std::cout << "Error creating OpenGL texture" << std::endl;
        return;
    }
    m_id = texID;

    if (bGenMipMap) glGenerateMipmap(m_target);
    
    al_destroy_bitmap(bitmap);
    al_shutdown_image_addon();*/


    int width, height, numOfChannels;
    if (bFlipVertically) stbi_set_flip_vertically_on_load(true);

    std::string fullPath = "Assets/" + m_path;
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &numOfChannels, NULL);
    if (data == nullptr) {
        std::cout << "Failed to load texture: " << m_path << std::endl;
        return;
    }
    GLenum format = NULL;
    {
        if (numOfChannels == 1)      format = GL_RED;
        else if (numOfChannels == 3) format = GL_RGB;
        else if (numOfChannels == 4) format = GL_RGBA;
    }
    glTexImage2D(m_target, NULL, format, width, height, NULL, format, GL_UNSIGNED_BYTE, data);
    if (bGenMipMap) glGenerateMipmap(m_target);
    delete data;
}
void Texture::SetTextureObjectProperties(const GLint& wrapS, const GLint& wrapR, const GLint& magFilter, const GLint& minFilter) {
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