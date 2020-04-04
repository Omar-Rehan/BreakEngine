#pragma once
#include "GL/glew.h"
#include "stb_image.h"
#include "physfs.h"
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_opengl.h"
#include "allegro5/allegro_physfs.h"

#include "ShaderProgram.h"

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
    Texture(const char* imagePath, ImageMapType imageMapType);
    Texture(const TextureInfo& parameters);

    void Activate(ShaderProgram& shaderProgram, unsigned int textureUnit, const std::string& uniformName) const;

    const ImageMapType GetImageMapType() const;
    const std::string& GetPath() const;

private:
    GLenum m_target;
    unsigned int m_id;
    std::string m_path;
    ImageMapType m_imageMapType;

    void LoadDataFromFileIntoTextureObject(const bool& bFlipVertically, const bool& bGenMipMap);
    void SetTextureObjectProperties(const GLint& wrapS, const GLint& wrapR, const GLint& magFilter, const GLint& minFilter);
};