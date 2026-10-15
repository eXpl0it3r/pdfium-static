// Links against the static PDFium library and renders the first page of a PDF.
// Including STL headers makes the MSVC linker check the RuntimeLibrary and
// _ITERATOR_DEBUG_LEVEL of every object file against the library's.

#include <fpdf_text.h>
#include <fpdfview.h>
#include <fpdf_scopers.h>

#include <cstdio>
#include <string>

int main(int argc, char* argv[])
{
    FPDF_InitLibrary();

    int result = 0;

    if (argc > 1)
    {
        const std::string path = argv[1];

        ScopedFPDFDocument document(FPDF_LoadDocument(path.c_str(), nullptr));
        if (!document)
        {
            std::fprintf(stderr, "Failed to load %s (error %lu)\n", path.c_str(), FPDF_GetLastError());
            result = 1;
        }
        else
        {
            ScopedFPDFPage page(FPDF_LoadPage(document.get(), 0));
            const int width = static_cast<int>(FPDF_GetPageWidthF(page.get()));
            const int height = static_cast<int>(FPDF_GetPageHeightF(page.get()));

            ScopedFPDFBitmap bitmap(FPDFBitmap_Create(width, height, 0));
            FPDFBitmap_FillRect(bitmap.get(), 0, 0, width, height, 0xFFFFFFFF);
            FPDF_RenderPageBitmap(bitmap.get(), page.get(), 0, 0, width, height, 0, 0);

            ScopedFPDFTextPage text(FPDFText_LoadPage(page.get()));

            std::printf("%s: %d page(s), first page %dx%d, %d characters\n",
                        path.c_str(),
                        FPDF_GetPageCount(document.get()),
                        width,
                        height,
                        FPDFText_CountChars(text.get()));
        }
    }

    FPDF_DestroyLibrary();
    return result;
}
