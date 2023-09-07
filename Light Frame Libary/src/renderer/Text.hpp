#pragma once

static FT_Library s_ft;

static class FtInitializer
{ public: FtInitializer() { if (FT_Init_FreeType(&s_ft)) { printf("Failed to init ft\n"); } } ~FtInitializer() { FT_Done_FreeType(s_ft); } }
s_ftInitializer;

class Text
{
public:

	FT_Face load(std::string_view filepath)
	{
		FT_Face face;
		if (FT_New_Face(s_ft, filepath.data(), 0, &face))
		{
			printf("Failed to load %s\n", filepath.data());
		}

		FT_Set_Pixel_Sizes(face, 0, 48);

		return face;
	}
};

