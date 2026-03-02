//
// Created by ace on 2026-01-27.
//
#include <ocad/ocad_helper.h>

#include <cassert>
#include <limits>
#include <loguru.hpp>



/**
 * If `pos` is 0 or next block is out-of-bounds, returns `nullptr`, else returns the next valid block.
 * @param byte_array
 * @param pos
 * @param block_size
 * @return
 */
const void* Ocad::GetBlockCheckedRaw(const std::vector<std::byte>& byte_array, uint32_t pos, uint32_t block_size)
{
    if (pos == 0)
    {
        return nullptr;
    }

    if ((static_cast<uint64_t>(pos) + block_size > std::numeric_limits<uint32_t>::max()
        || pos + block_size - 1 >= static_cast<uint32_t>(byte_array.size())))
    {
        LOG_F(WARNING,"Ocd::GetBlockChecked: Requested data block is out of bounds");
        return nullptr;
    }

    return byte_array.data() + pos;
}



/**
 * Adds padding so that the next data is appended at a multiple of 8.
 * @param byte_array
 * @return
 */
std::vector<char>& AddPadding(std::vector<char>& byte_array)
{
    const std::vector padding ((0x7ffffff8 - byte_array.size()) % 8,'\0' );
    byte_array.insert(byte_array.end(),padding.cbegin(),padding.cend());
    return byte_array;
}

