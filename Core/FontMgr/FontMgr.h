#include "msdfgen.h"
#include "msdfgen-ext.h"
#include "msdf-atlas-gen.h"


struct Configuration {
    msdf_atlas::ImageType imageType;
    msdf_atlas::ImageFormat imageFormat;
    msdf_atlas::YDirection yDirection;
    int width, height;
    double emSize;
    msdfgen::Range pxRange;
    double angleThreshold;
    double miterLimit;
    bool pxAlignOriginX, pxAlignOriginY;
    struct {
        int cellWidth, cellHeight;
        int cols, rows;
        bool fixedOriginX, fixedOriginY;
    } grid;
    void (*edgeColoring)(msdfgen::Shape&, double, unsigned long long);
    bool expensiveColoring;
    unsigned long long coloringSeed;
    msdf_atlas::GeneratorAttributes generatorAttributes;
    bool preprocessGeometry;
    bool kerning;
    int threadCount;
    const char* arteryFontFilename;
    const char* imageFilename;
    const char* jsonFilename;
    const char* csvFilename;
    const char* shadronPreviewFilename;
    const char* shadronPreviewText;
};

template <typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GEN_FN>
msdfgen::Bitmap<T, N> makeAtlas(const std::vector<msdf_atlas::GlyphGeometry>& glyphs, const std::vector<msdf_atlas::FontGeometry>& fonts, const Configuration& config) {
    msdf_atlas::ImmediateAtlasGenerator<S, N, GEN_FN, msdf_atlas::BitmapAtlasStorage<T, N> > generator(config.width, config.height);
    generator.setAttributes(config.generatorAttributes);
    generator.setThreadCount(config.threadCount);
    generator.generate(glyphs.data(), static_cast<int>(glyphs.size()));
    msdfgen::Bitmap<T, N> bitmap = (msdfgen::Bitmap<T, N>)generator.atlasStorage();
    return bitmap;
}

class FontMgr
{
public:
    void CreateFontAtlasBitMap();
    std::vector<int> GetGridLayout() const
    {
        return std::vector<int>({ m_FontConfig.grid.cellWidth, m_FontConfig.grid.cellHeight, m_FontConfig.grid.cols, m_FontConfig.grid.rows });
    }
    msdfgen::Bitmap<float, 4> GetFontAtlasBitmap() const
    {
        return m_FontAtlasBitmap;
    }
    msdfgen::GlyphIndex FindGlyphIndexFromChar(const char& c);
    FontMgr(std::string str = ""):
        m_FreetypeHandle{nullptr},
        m_FontHandle{nullptr},
        m_FontFilePath{str}
    {
        m_FreetypeHandle = msdfgen::initializeFreetype();

        if (m_FreetypeHandle)
        {
            m_FontHandle = msdfgen::loadFont(m_FreetypeHandle, m_FontFilePath.c_str());
        }

        m_FontConfig.imageType = msdf_atlas::ImageType::MTSDF;
        m_FontConfig.imageFormat = msdf_atlas::ImageFormat::FL32;
        m_FontConfig.yDirection = msdf_atlas::YDirection::BOTTOM_UP;
        m_FontConfig.width = 4096;
        m_FontConfig.height = 2048;
        m_FontConfig.emSize = 256;
        m_FontConfig.pxRange = 0;
        m_FontConfig.angleThreshold = 3.0;
        m_FontConfig.miterLimit = 1.0;
        m_FontConfig.pxAlignOriginX = false;
        m_FontConfig.pxAlignOriginY = true;
        m_FontConfig.grid.cellHeight = 64;
        m_FontConfig.grid.cellWidth = 64;
        m_FontConfig.grid.cols = 48;
        m_FontConfig.grid.rows = 48;
        m_FontConfig.grid.fixedOriginX = false;
        m_FontConfig.grid.fixedOriginY = true;
        //m_FontConfig.edgeColoring = &msdfgen::edgeColoringByDistance;
        m_FontConfig.edgeColoring = &msdfgen::edgeColoringInkTrap;
        m_FontConfig.expensiveColoring = true;
        m_FontConfig.coloringSeed = 2657987;

        m_FontConfig.generatorAttributes.scanlinePass = true;
        m_FontConfig.generatorAttributes.config.overlapSupport = true;
        m_FontConfig.generatorAttributes.config.errorCorrection.mode = msdfgen::ErrorCorrectionConfig::INDISCRIMINATE;
        m_FontConfig.preprocessGeometry = false;
        m_FontConfig.kerning = true;
        m_FontConfig.threadCount = 512;
        m_FontConfig.arteryFontFilename = NULL;
        m_FontConfig.imageFilename = NULL;
        m_FontConfig.csvFilename = NULL;
        m_FontConfig.jsonFilename = NULL;
        m_FontConfig.shadronPreviewFilename = NULL;
        m_FontConfig.shadronPreviewText = NULL;
    }
    ~FontMgr()
    {
        if (m_FontHandle)
        {
            msdfgen::destroyFont(m_FontHandle);
        }
        msdfgen::deinitializeFreetype(m_FreetypeHandle);
    }

private:
    msdfgen::FreetypeHandle * m_FreetypeHandle;
    msdfgen::FontHandle* m_FontHandle;
    std::string m_FontFilePath;
    msdfgen::Bitmap<float, 4> m_FontAtlasBitmap;
    Configuration m_FontConfig;
    std::vector<msdf_atlas::FontGeometry> m_FontGeometry;
    std::vector<msdf_atlas::GlyphGeometry> m_GlyphGeometry;
};