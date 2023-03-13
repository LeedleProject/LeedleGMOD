#pragma once

namespace game {
    
class DVariant;
class RecvTable;
class RecvProp;
class RecvProxyData;

using RecvVarProxyFn =
    void (*)(const RecvProxyData* data, void* struct_ptr, void* out_ptr);
using ArrayLengthRecvProxyFn =
    void (*)(void* struct_ptr, int object_id, int current_array_length);
using DataTableRecvVarProxyFn = void (*)(
    const RecvProp* prop,
    void** out_ptr,
    void* data_ptr,
    int object_id);

enum class KSendPropType {
    _int = 0,
    _float,
    _vec,
    _vec_xy,
    _string,
    _array,
    _data_table,
    _int_64,
};

class DVariant {
  public:
    union {
        float m_float;
        long m_int;
        char* m_string;
        void* m_data;
        float m_vector[3];
        __int64 m_int64;
    };
    KSendPropType type;
};

class RecvProxyData {
  public:
    const RecvProp* recv_prop;
    DVariant value;
    int element_index;
    int object_id;
};

class RecvProp {
  public:
    char* prop_name;
    KSendPropType prop_type;
    int prop_flags;
    int buffer_size;
    int is_inside_of_array;
    const void* extra_data_ptr;
    RecvProp* array_prop;
    ArrayLengthRecvProxyFn array_length_proxy;
    RecvVarProxyFn proxy_fn;
    DataTableRecvVarProxyFn data_table_proxy_fn;
    RecvTable* data_table;
    int offset;
    int element_stride;
    int elements_count;
    const char* parent_array_prop_name;
};

class RecvTable {
  public:
    RecvProp* props;
    int props_count;
    void* decoder_ptr;
    char* table_name;
    bool is_initialized;
    bool is_in_main_list;
};
}  // namespace game