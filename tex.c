#include "tex.h"

AU_TextureRegion au_tex_region(AU_Texture tex) {
	return (AU_TextureRegion) {
		tex, (AU_Rectangle) {
			0, 0, tex.width, tex.height
		}
	};
}
