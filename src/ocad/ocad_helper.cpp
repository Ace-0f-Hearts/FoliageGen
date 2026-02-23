//
// Created by ace on 2026-01-27.
//
#include <ocad/ocad_helper.h>

#include <assert.h>
#include <limits>
#include <loguru.hpp>


template< class BlockType >
const BlockType* getBlockChecked(const std::vector<char>& byte_array, uint32_t pos)
{
    extern const void* getBlockCheckedRaw(const std::vector<char>& byte_array, uint32_t pos, uint32_t block_size);
    return reinterpret_cast<const BlockType*>(getBlockCheckedRaw(byte_array, pos, sizeof(BlockType)));
}

template< class BlockType >
BlockType* Ocad::GetBlockChecked(std::vector<char>& byte_array, uint32_t pos)
{
    return const_cast<BlockType*>(getBlockChecked<BlockType>(static_cast<const std::vector<char>&>(byte_array), pos));
}


/**
 * If `pos` is 0 or next block is out-of-bounds, returns `nullptr`, else returns the next valid block.
 * @param byteArray
 * @param pos
 * @param block_size
 * @return
 */
const void* Ocad::GetBlockCheckedRaw(const std::vector<char>& byteArray, uint32_t pos, uint32_t block_size)
{
    if (pos == 0)
    {
        return nullptr;
    }

    if ((static_cast<uint64_t>(pos) + block_size > std::numeric_limits<uint32_t>::max()
        || pos + block_size - 1 >= static_cast<uint32_t>(byteArray.size())))
    {
        LOG_F(WARNING,"Ocd::getBlockChecked: Requested data block is out of bounds");
        return nullptr;
    }

    return byteArray.data() + pos;
}

template <class V>
Ocad::OcadEntityIndexIterator<V>::OcadEntityIndexIterator(const std::vector<char>& byteArray, IndexBlock* firstBlock)
    : byteArray(&byteArray), block(firstBlock), index(0) {}

template <class V>
Ocad::OcadEntityIndexIterator<V>& Ocad::OcadEntityIndexIterator<V>::operator++()
{
    do
    {
        ++index;
        if (index == 256)
        {
            index = 0;
            block = getBlockChecked<IndexBlock>(*byteArray, block->next_block);
            if (!block)
                break;
        }
    }
    while (!isValidEntry());
    return *this;
}

template <class V>
Ocad::OcadEntityIndexIterator<V> Ocad::OcadEntityIndexIterator<V>::operator++(int)
{
    auto oldVal = *this;
    this->operator++();
    return oldVal;
}

template <class V>
typename Ocad::OcadEntityIndexIterator<V>::valueType Ocad::OcadEntityIndexIterator<V>::operator*() const
{
    return  { &block->entries[index], reinterpret_cast<const typename valueType::EntityType*>(byteArray->data()+block->entries[index].pos) };
}

template <class V>
bool Ocad::OcadEntityIndexIterator<V>::operator==(const OcadEntityIndexIterator<V>& rhs) const
{
    return block == rhs.block && index == rhs.index;
}

template <class V>
bool Ocad::OcadEntityIndexIterator<V>::operator!=(const OcadEntityIndexIterator<V>& rhs) const
{
    return !operator==(rhs);
}


template <class V>
bool Ocad::OcadEntityIndexIterator<V>::isValidEntry() const
{
    return getBlockChecked<typename valueType::EntityType>(*byteArray, block->entries[index].pos);
}

template <class F, class T>
template <class X, std::enable_if_t<std::is_same_v<X, Ocad::ParameterStringIndexEntry>, int>>
Ocad::OcadEntityIndex<F, T>::valueType::operator std::vector<char>() const
{
    //TODO: This needs to be tested and refined
    auto temp = reinterpret_cast<char*>(entity);
    std::vector<char> data(*temp,*temp + entry->size);
    return data;
}

template <class F, class T>
Ocad::OcadEntityIndex<F, T>::OcadEntityIndex(OcadFile<F>& file) noexcept : file(file)
{
}

template <class F, class T>
typename Ocad::OcadEntityIndex<F, T>::constIterator Ocad::OcadEntityIndex<F, T>::begin() const
{
    auto pos = firstBlock<typename T::IndexEntryType>();
    const auto& byteArray = file.constByteArray();
    auto firstBlock = Ocad::GetBlockChecked<typename constIterator::IndexBlock>(*byteArray, pos);
    return {byteArray,firstBlock};
}

template <class F, class T>
typename Ocad::OcadEntityIndex<F, T>::constIterator Ocad::OcadEntityIndex<F, T>::end() const noexcept
{
    return {};
}

/**
 * Adds padding so that the next data is appended at a multiple of 8.
 * @param byte_array
 * @return
 */
std::vector<char>& addPadding(std::vector<char>& byte_array)
{
    const std::vector padding ((0x7ffffff8 - byte_array.size()) % 8,'\0' );
    byte_array.insert(byte_array.end(),padding.cbegin(),padding.cend());
    return byte_array;
}

template <class F, class T>
typename Ocad::OcadEntityIndex<F, T>::EntryType& Ocad::OcadEntityIndex<F, T>::insert(const std::vector<char>& entity_data,
    const EntryType& entry)
{
    auto& byte_array = addPadding(file.byteArray());
    IndexBlock* block;
    auto next_block_pos = firstBlock<typename T::IndexEntryType>();
    auto block_pos = decltype(next_block_pos)(0);
    do
    {
        block_pos = next_block_pos;
        block = Ocad::GetBlockChecked<IndexBlock>(byte_array, block_pos);
        if (!block)
        {
            ///  \todo Throw exception
            LOG_F(ERROR,"OcdEntityIndexIterator: Next index block is out of bounds");
            throw std::runtime_error("OcdEntityIndexIterator: Next index block is out of bounds");
        }
        next_block_pos = block->next_block;
    }
    while (next_block_pos != 0);

    uint16_t index = 0;
    while (index < 256 && block->entries[index].pos)
        ++index;

    if (index == 256)
    {
        block_pos = decltype(block->next_block)(byte_array.size());
        block->next_block = block_pos;
        auto new_block = IndexBlock {};
        byte_array.append(reinterpret_cast<const char*>(&new_block), sizeof(IndexBlock));
        block = reinterpret_cast<IndexBlock*>(byte_array.data() + block_pos);
        index = 0;
    }

    auto entity_pos = decltype(block->entries[index].pos)(byte_array.size());
    byte_array.append(entity_data); // May reallocate! Re-calculate block pointer:
    block = Ocad::GetBlockChecked<IndexBlock>(byte_array, block_pos);
    Q_ASSERT(block);
    block->entries[index] = entry;
    block->entries[index].pos = entity_pos;
    return block->entries[index];
}

template <class F, class T>
typename Ocad::OcadEntityIndex<F, T>::EntryType& Ocad::OcadEntityIndex<F, T>::insert(const std::vector<char>& entity_data)
{
    return insert(entity_data);
}

template <class F, class T>
typename Ocad::OcadEntityIndex<F, T>::EntryType& Ocad::OcadEntityIndex<F, T>::insert(int32_t string_type,
    const std::vector<char>& string_data)
{
    uint32_t size = string_data.size() + 1;
    return insert(std::vector(string_data.begin(), string_data.begin() + size),{0,size,string_type,0});
}

template <class F, class T>
template <class X, typename std::enable_if<std::is_same<X, Ocad::ParameterString::IndexEntryType>::value, int>::type>
uint32_t Ocad::OcadEntityIndex<F, T>::firstBlock() const
{
    return file.header()->firstStringBlock;
}

template <class F, class T>
template <class X, typename std::enable_if<std::is_same<X, typename F::BaseSymbol::IndexEntryType>::value, int>::type>
uint32_t Ocad::OcadEntityIndex<F, T>::firstBlock() const
{
    return file.header()->firstSymbolBlock;
}

template <class F, class T>
template <class X, typename std::enable_if<std::is_same<X, typename F::Object::IndexEntryType>::value, int>::type>
uint32_t Ocad::OcadEntityIndex<F, T>::firstBlock() const
{
    return file.header()->firstObjectBlock;
}

template <class F>
Ocad::OcadFile<F>::OcadFile() {}

template <class F>
Ocad::OcadFile<F>::OcadFile(const std::vector<char>& data) noexcept
: byte_array_(data),
string_index_(*this),
symbol_index_(*this),
object_index_(*this)
{
    assert(data.data() == byte_array_.data());
}

template <class F>
const std::vector<char>& Ocad::OcadFile<F>::ConstByteArray() const
{
    return byte_array_;
}

template <class F>
const std::vector<char>& Ocad::OcadFile<F>::byte_array() const
{
    return byte_array_;
}

template <class F>
std::vector<char>& Ocad::OcadFile<F>::byte_array()
{
    return byte_array_;
}

template <class F>
const typename Ocad::OcadFile<F>::FileHeader* Ocad::OcadFile<F>::header() const
{
    return byte_array_.size() < sizeof(FileHeader) ? nullptr : reinterpret_cast<const FileHeader*>(byte_array_.data());
}

template <class F>
const typename Ocad::OcadFile<F>::StringIndex& Ocad::OcadFile<F>::strings() const
{
    return string_index_;
}

template <class F>
const typename Ocad::OcadFile<F>::SymbolIndex& Ocad::OcadFile<F>::symbols() const
{
    return symbol_index_;
}

template <class F>
const typename Ocad::OcadFile<F>::ObjectIndex& Ocad::OcadFile<F>::objects() const
{
    return object_index_;
}
