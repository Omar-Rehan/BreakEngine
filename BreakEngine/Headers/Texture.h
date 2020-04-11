#pragma once
#include "GL/glew.h"
#include "stb_image.h"

#include "ShaderProgram.h"
#include "FileManager.h"

enum class ImageMapType { IMT_NULL, IMT_DiffuseMap, IMT_SpecularMap, IMT_NormalMap, IMT_HeightMap, IMT_AmbientMap };

struct TextureInfo {
    GLenum target;
    GLint wrapOptionS, wrapOptionR;
    GLint filterOptionMin, filterOptionMag;

    bool bGenMipmap, bVerticalFlip;
    const char* imagePath;
    ImageMapType imageMapType;
};

class Texture {
public:
    Texture() = default;
    Texture(const char* imagePath, ImageMapType imageMapType, FileManager* pFileManager);
    Texture(const TextureInfo& parameters, FileManager* pFileManager);

    void Activate(ShaderProgram& shaderProgram, unsigned int textureUnit, const std::string& uniformName) const;

    const ImageMapType GetImageMapType() const;
    const std::string& GetPath() const;

private:
    GLenum m_target;
    unsigned int m_id;
    std::string m_path;
    ImageMapType m_imageMapType;

    void CreateTextureObject(const GLint& wrapS, const GLint& wrapR, const GLint& magFilter, const GLint& minFilter);
    unsigned char* LoadDataFromFile(FileManager* pFileManager, unsigned long long& readBytes);
    void ParseImageData(unsigned char* data, unsigned long long size, const bool& bFlipVertically, const bool& bGenMipMap);
};