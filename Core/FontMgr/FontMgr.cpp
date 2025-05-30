#include "FontMgr.h"
#include <iostream>

void FontMgr::CreateFontAtlasBitMap()
{
    constexpr unsigned long long LCG_MULTIPLIER = 6364136223846793005ull;
    msdf_atlas::GridAtlasPacker atlasPacker;
    msdfgen::Range emRange = 0, pxRange = 0;

    int fixedWidth = m_FontConfig.width;
    int fixedHeight = m_FontConfig.height;

    atlasPacker.setFixedOrigin(m_FontConfig.grid.fixedOriginX, m_FontConfig.grid.fixedOriginY);
    atlasPacker.setDimensions(fixedWidth, fixedHeight);
    //atlasPacker.setScale(config.emSize);

    atlasPacker.setPixelRange(pxRange);
    atlasPacker.setUnitRange(emRange);
    atlasPacker.setMiterLimit(m_FontConfig.miterLimit);
    atlasPacker.setOriginPixelAlignment(m_FontConfig.pxAlignOriginX, m_FontConfig.pxAlignOriginY);
    //atlasPacker.setInnerUnitPadding(innerEmPadding);
    //atlasPacker.setOuterUnitPadding(outerEmPadding);
    //atlasPacker.setInnerPixelPadding(innerPxPadding);
    //atlasPacker.setOuterPixelPadding(outerPxPadding);

    unsigned all_glyph_count = 0;
    msdfgen::getGlyphCount(all_glyph_count, m_FontHandle);
    int start_glyph = 0;
    int load_range = all_glyph_count;
    msdf_atlas::FontGeometry fontGeometry;
    msdf_atlas::GlyphGeometry glyphGeometry;
    auto num_loaded_glyph_range = fontGeometry.loadGlyphRange(m_FontHandle, 1.0, start_glyph, start_glyph + load_range);
    for (auto& glyph : fontGeometry.getGlyphs())
    {
        m_GlyphGeometry.push_back((msdf_atlas::GlyphGeometry&&)glyph);
    }
    m_FontGeometry.push_back((msdf_atlas::FontGeometry&&)fontGeometry);

    int remaining = atlasPacker.pack(m_GlyphGeometry.data(), static_cast<int>(m_GlyphGeometry.size()));
    if (remaining)
    {
        std::cout << remaining << std::endl;
    }

    unsigned long long glyphSeed = m_FontConfig.coloringSeed;
    for (msdf_atlas::GlyphGeometry& glyph : m_GlyphGeometry) 
    {
        glyphSeed *= LCG_MULTIPLIER;
        glyph.edgeColoring(m_FontConfig.edgeColoring, m_FontConfig.angleThreshold, glyphSeed);
    }

    m_FontConfig.emSize = atlasPacker.getScale();
    m_FontConfig.pxRange = atlasPacker.getPixelRange();
    atlasPacker.getCellDimensions(m_FontConfig.grid.cellWidth, m_FontConfig.grid.cellHeight);
    m_FontConfig.grid.cols = atlasPacker.getColumns();
    m_FontConfig.grid.rows = atlasPacker.getRows();

    atlasPacker.getDimensions(m_FontConfig.width, m_FontConfig.height);

    m_FontAtlasBitmap = makeAtlas<float, float, 4, msdf_atlas::mtsdfGenerator>(m_GlyphGeometry, m_FontGeometry, m_FontConfig);
}

msdfgen::GlyphIndex FontMgr::FindGlyphIndexFromChar(const char& c)
{
    msdfgen::GlyphIndex glyphIndex;
    msdfgen::getGlyphIndex(glyphIndex, m_FontHandle, c);
    return glyphIndex;
}

void FontMgr::SaveAtlas(const std::string& filename)
{
    msdfgen::saveRgba(m_FontAtlasBitmap, filename.c_str());
}
