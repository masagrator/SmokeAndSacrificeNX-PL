#include "skyline/inlinehook/And64InlineHook.hpp"
#include "skyline/utils/cpputils.hpp"
#include "skyline/inlinehook/memcpy_controlled.hpp"
#define Unicode std::char_traits<char16_t>

uintptr_t TextRegionOffset = 0;

ptrdiff_t returnInstructionOffset(uintptr_t LR) {
	return LR - TextRegionOffset;
}

struct System_String {
	void* _class;
	void* _monitor;
	uint32_t string_size;
	char16_t lang_string[12];
	char reserved[2];
};

namespace StringDatabase {
	namespace SetCurrentLanguage {

		void (*original)(void* _this, System_String isoCode, void* MethodInfo);
		void hook(void* _this, System_String isoCode, void* MethodInfo) {
			const char16_t* language = u"pl-PL";
			
			isoCode.string_size = Unicode::length(language);
			Unicode::copy(isoCode.lang_string, language, Unicode::length(language) + 1);
			return original(_this, isoCode, MethodInfo);
		}

	}
}

void SAS_main()
{
	TextRegionOffset = (uintptr_t)skyline::utils::getRegionAddress(skyline::utils::region::Text);

	//Offset recovered with il2cppdumper, named as StringDatabase$$SetCurrentLanguage
	A64HookFunction((void*)(TextRegionOffset + 0x1BC110), reinterpret_cast<void*>(StringDatabase::SetCurrentLanguage::hook), (void**)&StringDatabase::SetCurrentLanguage::original);

}
