#ifndef _METABUFFER_H_
#define _METABUFFER_H_
#include <stdint.h>
#include <string.h>
#include <map>
#include <assert.h>
using namespace std;

namespace peng {

#define CHECK_EQ(x,y)  assert((x) == (y))

class MetaData {
public:
    MetaData();
    virtual ~MetaData();
    MetaData(const MetaData &from);

    enum Keys {
        URL,
        SESSION,
        BUFFER_POINTER,
        BUFFER_LENGTH,
        SOURCE_PROVIDER,
        SOURCE_LISTENER,
        STREAM_TYPE,
        STREAM_CODEC,
        TIMESTAMP
    };

    enum Type {
        TYPE_NONE,
        TYPE_C_STRING,
        TYPE_INT32,
        TYPE_INT64,
        TYPE_FLOAT,
        TYPE_POINTER,
    };

    void clear();
    bool remove(uint32_t key);

    bool setCString(uint32_t key, const char *value);
    bool setInt32(uint32_t key, int32_t value);
    bool setInt64(uint32_t key, int64_t value);
    bool setFloat(uint32_t key, float value);
    bool setPointer(uint32_t key, void *value);

    bool findCString(uint32_t key, const char **value);
    bool findInt32(uint32_t key, int32_t *value);
    bool findInt64(uint32_t key, int64_t *value);
    bool findFloat(uint32_t key, float *value);
    bool findPointer(uint32_t key, void **value);

    bool setData(uint32_t key, uint32_t type, const void *data, size_t size);

    bool findData(uint32_t key, uint32_t *type, const void **data, size_t *size);

    void dumpToLog() const;

protected:

private:
    struct typed_data {
        typed_data();
        ~typed_data();

        typed_data(const MetaData::typed_data &);
        typed_data &operator=(const MetaData::typed_data &);

        void clear();
        void setData(uint32_t type, const void *data, size_t size);
        void getData(uint32_t *type, const void **data, size_t *size) const;

    private:
        uint32_t mType;
        size_t mSize;

        union {
            void *ext_data;
            float reservoir;
        } u;

        bool usesReservoir() const {
            return mSize <= sizeof(u.reservoir);
        }

        void allocateStorage(size_t size);
        void freeStorage();

        void *storage() {
            return usesReservoir() ? &u.reservoir : u.ext_data;
        }

        const void *storage() const {
            return usesReservoir() ? &u.reservoir : u.ext_data;
        }
    };
    map<uint32_t, typed_data> mItems;
   
};

}

#endif
