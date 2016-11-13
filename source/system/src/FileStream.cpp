//
// Created by Lectem on 12/11/2016.
//

#include "FileStream.h"

namespace WorldStone
{

FileStream::FileStream(const path& filename)
{
    open(filename);
}

FileStream::~FileStream()
{
    close();
}

bool FileStream::open(const path& filename)
{
    file = fopen(filename.c_str(), "rb");
    if (!file) setstate(failbit);
    return good();
}

bool FileStream::close()
{
    if (!file || fclose(file)) setstate(failbit);
    return good();
}

streamsize FileStream::read(void* buffer, size_t size, size_t count)
{
    const size_t readSize = fread(buffer, size, count, file);
    if (readSize != size) {
        if (feof(file))
            setstate(eofbit);
        else
            setstate(failbit);
    }
    return static_cast<streamsize>(readSize);
}

long FileStream::tell()
{
    return ftell(file);
}

// We assume there's no need for conversion, which is probably wrong on some platforms
static_assert(FileStream::beg == SEEK_SET, "");
static_assert(FileStream::cur == SEEK_CUR, "");
static_assert(FileStream::end == SEEK_END, "");
bool FileStream::seek(long offset, Stream::seekdir origin)
{
    if (fseek(file, offset, origin)) setstate(failbit);
    return good();
}

long FileStream::size()
{
    const long curPos = tell();
    if (curPos == -1) setstate(failbit);
    seek(0, end);
    const long size = tell();
    if (size == -1) setstate(failbit);
    // Reset the file position even if the previous commands failed. Do not fail if position was
    // invalid
    if (curPos != -1 && !seek(curPos, beg)) setstate(failbit);
    return size;
}
}
