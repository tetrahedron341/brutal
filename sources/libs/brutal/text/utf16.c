#include <brutal/io/buf.h>
#include <brutal/parse/scan.h>
#include <brutal/text/case.h>

uint16_t *str_to_cstr_utf16(Str str, Alloc *alloc)
{
    size_t size = (str.len + 1) * 2;
    uint16_t *utf16 = (uint16_t *)alloc_malloc(alloc, size);

    for (size_t i = 0; i < str.len; i++)
    {
        utf16[i] = str.buf[i];
    }

    utf16[str.len] = 0;

    return utf16;
}

uint16_t *str_to_cstr_utf16_dos(Str str, Alloc *alloc)
{
    Buf buf;
    buf_init(&buf, (str.len + 1) * 2, alloc);

    Scan scan;
    scan_init(&scan, str);

    while (!scan_ended(&scan))
    {
        if (scan_skip_word(&scan, str$("\r\n")))
        {
            buf_push(&buf, u'\r');
            buf_push(&buf, u'\n');
        }
        else if (scan_skip(&scan, '\n'))
        {
            buf_push(&buf, u'\r');
            buf_push(&buf, u'\n');
        }
        else
        {
            buf_push(&buf, (uint16_t)scan_next(&scan));
        }
    }

    buf_push(&buf, (uint16_t)0x0);

    return (uint16_t *)buf.data;
}
