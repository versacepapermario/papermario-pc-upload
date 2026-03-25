#include "common.h"

// skips ahead in buffer by constant + count * size
API_CALLABLE(N(AdvanceBuffer)) {
    Bytecode* args = script->ptrReadPos;
    s32 constant = evt_get_variable(script, *args++);
    s32 size = evt_get_variable(script, *args++);
    s32 count = evt_get_variable(script, *args++);

#ifdef PORT
    port_buf_advance(&script->buffer, constant + size * count);
#else
    script->buffer = &script->buffer[constant + size * count];
#endif
    return ApiStatus_DONE2;
}
