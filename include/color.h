#ifndef __COLOR_H__
#define __COLOR_H__


//----------------------------------------------------------------------------
// Color
//----------------------------------------------------------------------------
class Color
{
public:
	Color() {
		argb = Black;
	}

	Color( unsigned int x ) {
		argb = x;
	}

	Color( char a, char r, char g, char b ) {
		argb =  a << AlphaOffset;
		argb |= r << RedOffset;
		argb |= g << GreenOffset;
		argb |= b << BlueOffset;
	}

	unsigned int GetA() {
		return (argb & AlphaMask) >> AlphaOffset;
	}

	unsigned int GetR() {
		return (argb & RedMask) >> RedOffset;
	}

	unsigned int GetG() {
		return (argb & GreenMask) >> GreenOffset;
	}

	unsigned int GetB() {
		return (argb & BlueMask) >> BlueOffset;
	}

public:
	static const unsigned int AliceBlue            = 0xFFF0F8FF;
	static const unsigned int AntiqueWhite         = 0xFFFAEBD7;
	static const unsigned int Aqua                 = 0xFF00FFFF;
	static const unsigned int Aquamarine           = 0xFF7FFFD4;
	static const unsigned int Azure                = 0xFFF0FFFF;
	static const unsigned int Beige                = 0xFFF5F5DC;
	static const unsigned int Bisque               = 0xFFFFE4C4;
	static const unsigned int Black                = 0xFF000000;
	static const unsigned int BlanchedAlmond       = 0xFFFFEBCD;
	static const unsigned int Blue                 = 0xFF0000FF;
	static const unsigned int BlueViolet           = 0xFF8A2BE2;
	static const unsigned int Brown                = 0xFFA52A2A;
	static const unsigned int BurlyWood            = 0xFFDEB887;
	static const unsigned int CadetBlue            = 0xFF5F9EA0;
	static const unsigned int Chartreuse           = 0xFF7FFF00;
	static const unsigned int Chocolate            = 0xFFD2691E;
	static const unsigned int Coral                = 0xFFFF7F50;
	static const unsigned int CornflowerBlue       = 0xFF6495ED;
	static const unsigned int Cornsilk             = 0xFFFFF8DC;
	static const unsigned int Crimson              = 0xFFDC143C;
	static const unsigned int Cyan                 = 0xFF00FFFF;
	static const unsigned int DarkBlue             = 0xFF00008B;
	static const unsigned int DarkCyan             = 0xFF008B8B;
	static const unsigned int DarkGoldenrod        = 0xFFB8860B;
	static const unsigned int DarkGray             = 0xFFA9A9A9;
	static const unsigned int DarkGreen            = 0xFF006400;
	static const unsigned int DarkKhaki            = 0xFFBDB76B;
	static const unsigned int DarkMagenta          = 0xFF8B008B;
	static const unsigned int DarkOliveGreen       = 0xFF556B2F;
	static const unsigned int DarkOrange           = 0xFFFF8C00;
	static const unsigned int DarkOrchid           = 0xFF9932CC;
	static const unsigned int DarkRed              = 0xFF8B0000;
	static const unsigned int DarkSalmon           = 0xFFE9967A;
	static const unsigned int DarkSeaGreen         = 0xFF8FBC8B;
	static const unsigned int DarkSlateBlue        = 0xFF483D8B;
	static const unsigned int DarkSlateGray        = 0xFF2F4F4F;
	static const unsigned int DarkTurquoise        = 0xFF00CED1;
	static const unsigned int DarkViolet           = 0xFF9400D3;
	static const unsigned int DeepPink             = 0xFFFF1493;
	static const unsigned int DeepSkyBlue          = 0xFF00BFFF;
	static const unsigned int DimGray              = 0xFF696969;
	static const unsigned int DodgerBlue           = 0xFF1E90FF;
	static const unsigned int Firebrick            = 0xFFB22222;
	static const unsigned int FloralWhite          = 0xFFFFFAF0;
	static const unsigned int ForestGreen          = 0xFF228B22;
	static const unsigned int Fuchsia              = 0xFFFF00FF;
	static const unsigned int Gainsboro            = 0xFFDCDCDC;
	static const unsigned int GhostWhite           = 0xFFF8F8FF;
	static const unsigned int Gold                 = 0xFFFFD700;
	static const unsigned int Goldenrod            = 0xFFDAA520;
	static const unsigned int Gray                 = 0xFF808080;
	static const unsigned int Green                = 0xFF008000;
	static const unsigned int GreenYellow          = 0xFFADFF2F;
	static const unsigned int Honeydew             = 0xFFF0FFF0;
	static const unsigned int HotPink              = 0xFFFF69B4;
	static const unsigned int IndianRed            = 0xFFCD5C5C;
	static const unsigned int Indigo               = 0xFF4B0082;
	static const unsigned int Ivory                = 0xFFFFFFF0;
	static const unsigned int Khaki                = 0xFFF0E68C;
	static const unsigned int Lavender             = 0xFFE6E6FA;
	static const unsigned int LavenderBlush        = 0xFFFFF0F5;
	static const unsigned int LawnGreen            = 0xFF7CFC00;
	static const unsigned int LemonChiffon         = 0xFFFFFACD;
	static const unsigned int LightBlue            = 0xFFADD8E6;
	static const unsigned int LightCoral           = 0xFFF08080;
	static const unsigned int LightCyan            = 0xFFE0FFFF;
	static const unsigned int LightGoldenrodYellow = 0xFFFAFAD2;
	static const unsigned int LightGray            = 0xFFD3D3D3;
	static const unsigned int LightGreen           = 0xFF90EE90;
	static const unsigned int LightPink            = 0xFFFFB6C1;
	static const unsigned int LightSalmon          = 0xFFFFA07A;
	static const unsigned int LightSeaGreen        = 0xFF20B2AA;
	static const unsigned int LightSkyBlue         = 0xFF87CEFA;
	static const unsigned int LightSlateGray       = 0xFF778899;
	static const unsigned int LightSteelBlue       = 0xFFB0C4DE;
	static const unsigned int LightYellow          = 0xFFFFFFE0;
	static const unsigned int Lime                 = 0xFF00FF00;
	static const unsigned int LimeGreen            = 0xFF32CD32;
	static const unsigned int Linen                = 0xFFFAF0E6;
	static const unsigned int Magenta              = 0xFFFF00FF;
	static const unsigned int Maroon               = 0xFF800000;
	static const unsigned int MediumAquamarine     = 0xFF66CDAA;
	static const unsigned int MediumBlue           = 0xFF0000CD;
	static const unsigned int MediumOrchid         = 0xFFBA55D3;
	static const unsigned int MediumPurple         = 0xFF9370DB;
	static const unsigned int MediumSeaGreen       = 0xFF3CB371;
	static const unsigned int MediumSlateBlue      = 0xFF7B68EE;
	static const unsigned int MediumSpringGreen    = 0xFF00FA9A;
	static const unsigned int MediumTurquoise      = 0xFF48D1CC;
	static const unsigned int MediumVioletRed      = 0xFFC71585;
	static const unsigned int MidnightBlue         = 0xFF191970;
	static const unsigned int MintCream            = 0xFFF5FFFA;
	static const unsigned int MistyRose            = 0xFFFFE4E1;
	static const unsigned int Moccasin             = 0xFFFFE4B5;
	static const unsigned int NavajoWhite          = 0xFFFFDEAD;
	static const unsigned int Navy                 = 0xFF000080;
	static const unsigned int OldLace              = 0xFFFDF5E6;
	static const unsigned int Olive                = 0xFF808000;
	static const unsigned int OliveDrab            = 0xFF6B8E23;
	static const unsigned int Orange               = 0xFFFFA500;
	static const unsigned int OrangeRed            = 0xFFFF4500;
	static const unsigned int Orchid               = 0xFFDA70D6;
	static const unsigned int PaleGoldenrod        = 0xFFEEE8AA;
	static const unsigned int PaleGreen            = 0xFF98FB98;
	static const unsigned int PaleTurquoise        = 0xFFAFEEEE;
	static const unsigned int PaleVioletRed        = 0xFFDB7093;
	static const unsigned int PapayaWhip           = 0xFFFFEFD5;
	static const unsigned int PeachPuff            = 0xFFFFDAB9;
	static const unsigned int Peru                 = 0xFFCD853F;
	static const unsigned int Pink                 = 0xFFFFC0CB;
	static const unsigned int Plum                 = 0xFFDDA0DD;
	static const unsigned int PowderBlue           = 0xFFB0E0E6;
	static const unsigned int Purple               = 0xFF800080;
	static const unsigned int Red                  = 0xFFFF0000;
	static const unsigned int RosyBrown            = 0xFFBC8F8F;
	static const unsigned int RoyalBlue            = 0xFF4169E1;
	static const unsigned int SaddleBrown          = 0xFF8B4513;
	static const unsigned int Salmon               = 0xFFFA8072;
	static const unsigned int SandyBrown           = 0xFFF4A460;
	static const unsigned int SeaGreen             = 0xFF2E8B57;
	static const unsigned int SeaShell             = 0xFFFFF5EE;
	static const unsigned int Sienna               = 0xFFA0522D;
	static const unsigned int Silver               = 0xFFC0C0C0;
	static const unsigned int SkyBlue              = 0xFF87CEEB;
	static const unsigned int SlateBlue            = 0xFF6A5ACD;
	static const unsigned int SlateGray            = 0xFF708090;
	static const unsigned int Snow                 = 0xFFFFFAFA;
	static const unsigned int SpringGreen          = 0xFF00FF7F;
	static const unsigned int SteelBlue            = 0xFF4682B4;
	static const unsigned int Tan                  = 0xFFD2B48C;
	static const unsigned int Teal                 = 0xFF008080;
	static const unsigned int Thistle              = 0xFFD8BFD8;
	static const unsigned int Tomato               = 0xFFFF6347;
	static const unsigned int Transparent          = 0x00FFFFFF;
	static const unsigned int Turquoise            = 0xFF40E0D0;
	static const unsigned int Violet               = 0xFFEE82EE;
	static const unsigned int Wheat                = 0xFFF5DEB3;
	static const unsigned int White                = 0xFFFFFFFF;
	static const unsigned int WhiteSmoke           = 0xFFF5F5F5;
	static const unsigned int Yellow               = 0xFFFFFF00;
	static const unsigned int YellowGreen          = 0xFF9ACD32;

private:
	static const unsigned int AlphaOffset = 24;
	static const unsigned int RedOffset   = 16;
	static const unsigned int GreenOffset = 8;
	static const unsigned int BlueOffset  = 0;
	static const unsigned int AlphaMask   = 0xFF000000;
	static const unsigned int RedMask     = 0x00FF0000;
	static const unsigned int GreenMask   = 0x0000FF00;
	static const unsigned int BlueMask    = 0x000000FF;
	unsigned int argb;
};

#endif
