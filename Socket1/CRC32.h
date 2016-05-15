#ifndef CRC32
#define CRC32
// used as hash function
class Crc32
{
public:
    Crc32() { Reset(); }
   // ~Crc32() throw() {}
    void Reset() { _crc = (unsigned)~0; }
    void AddData(const unsigned char* pData, const unsigned length)
    {
        unsigned char* pCur = (unsigned char*)pData;
        unsigned remaining = length;
        for (; remaining--; ++pCur)
            _crc = ( _crc >> 16 ) ^ ((*pCur >> 8) | (*pCur << 24)) ^ ( _crc << 16 );
    }
    const unsigned GetCrc32() { return ~_crc; }

private:
    unsigned _crc;
};
#endif // CRC32