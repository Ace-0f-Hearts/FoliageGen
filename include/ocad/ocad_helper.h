//
// Created by ace on 2026-01-27.
//

#ifndef OCAD_OCDHELPER_H
#define OCAD_OCDHELPER_H
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <loguru.hpp>
#include <type_traits>
#include <vector>



namespace Ocad
{
    enum SymbolType
    {
        SymbolTypePoint        = 1,
        SymbolTypeLine         = 2,
        SymbolTypeArea         = 3,
        SymbolTypeText         = 4,
        SymbolTypeLineText     = 6, /// \since V9
        SymbolTypeRectangle    = 7  /// \since V9
    };
    enum SymbolStatus
    {
        SymbolNormal = 0,
        SymbolProtected = 1,
        SymbolHidden = 2,
    };
    enum ObjectStatus
    {
        ObjectDeleted = 0,
        ObjectNormal = 1,
        ObjectHidden = 2,
        ObjectDeletedForUndo = 3
    };

    template <class E>
    struct IndexBlock;

    template <class F>
    class OcadFile;

    struct ParameterStringIndexEntry;

    template <class BlockType>
    BlockType* GetBlockChecked(std::vector<std::byte>& byte_array, uint32_t pos);

    template< class BlockType >
    const BlockType* GetBlockChecked(const std::vector<std::byte>& byte_array, uint32_t pos);

    const void* GetBlockCheckedRaw(const std::vector<std::byte>& byte_array, uint32_t pos, uint32_t block_size);

    struct ParameterStringIndexEntry
    {
        uint32_t pos;
        uint32_t size;
        int32_t type;
        uint32_t obj_index;
    };

    template <class E>
    struct IndexBlock
    {
        using IndexEntryType = E;

        uint32_t next_block;
        IndexEntryType entries[256];
    };

    template <class V>
    class OcadEntityIndexIterator : public std::iterator<std::input_iterator_tag, V, std::ptrdiff_t, void, V>
    {
    public:
        using ValueType = V;
        using EntryType = V::EntryType;
        using IndexBlock = IndexBlock<EntryType>;

        OcadEntityIndexIterator() noexcept = default;
        OcadEntityIndexIterator(const OcadEntityIndexIterator&) noexcept = default;
        OcadEntityIndexIterator(OcadEntityIndexIterator&&) noexcept = default;

        OcadEntityIndexIterator(const std::vector<std::byte>& byte_array, IndexBlock* first_block);

        OcadEntityIndexIterator& operator=(const OcadEntityIndexIterator&) noexcept = default;
        OcadEntityIndexIterator& operator=(OcadEntityIndexIterator&&) noexcept = default;

        OcadEntityIndexIterator& operator++();

        OcadEntityIndexIterator operator++(int);

        ValueType operator*() const;

        bool operator==(const OcadEntityIndexIterator<V>& rhs) const;

        bool operator!=(const OcadEntityIndexIterator<V>& rhs) const;


    private:
        [[nodiscard]] bool IsValidEntry() const;

        const std::vector<std::byte>* byte_array_ = nullptr;
        const IndexBlock* block_ = nullptr;
        uint16_t index_ = 0;
    };

    struct SymbolIndexEntry
    {
        uint32_t pos;
    };

    struct ParameterString
    {
        using IndexEntryType = ParameterStringIndexEntry;
    };


    template <class F, class T>
    class OcadEntityIndex
    {
    public:
        /** The actual file format version type, reexported. */
        using FileFormat = F;

        /** The actual entity type. */
        using EntityType = T;

        /** The index entry type for the entity type. */
        using EntryType = T::IndexEntryType;

        struct ValueType
        {
            using EntityType = T;
            using EntryType = T::IndexEntryType;
            const EntryType* entry;
            const EntityType* entity;

            template <class X = T::IndexEntryType, std::enable_if_t<
                          std::is_same_v<X, ParameterStringIndexEntry>, int> = 0>
            operator std::vector<char>() const;
        };

        /** The index block type */
        using IndexBlock = Ocad::IndexBlock<EntryType>;

        /** The index iterator type. */
        using constIterator = OcadEntityIndexIterator<ValueType>;

        /**
         * Constructs an entity index object.
         */
        OcadEntityIndex(OcadFile<F>& file) noexcept;

        OcadEntityIndex(const OcadEntityIndex&) = delete;

        OcadEntityIndex& operator=(const OcadEntityIndex&) = delete;

        /**
         * Destroys the object.
         */
        ~OcadEntityIndex() = default;

        /**
         * Returns a forward iterator to the beginning of the index.
         */
        constIterator begin() const;

        /**
         * Returns a forward iterator to the end.
         */
        constIterator end() const noexcept;


        /**
         * Inserts an object with the given entry prototype.
         *
         * In order to update the create index entry after insertion, pay attention
         * to capture the result as reference, not as copy:
         *
         *     auto& object_entry = object_index.insert(ocd_object, prototype);
         *
         */
        EntryType& insert(const std::vector<char>& entity_data, const EntryType& entry);

        /**
         * Inserts a symbol.
         */
        EntryType& insert(const std::vector<char>& entity_data);

        /**
         * Inserts a parameter string with the given type number.
         *
         * The string_data is expected to have the trailing '\0' maintained
         * by QByteArray at size(), i.e. it must not be constructed using
         * QByteArray::fromRawData().
         */
        EntryType& insert(int32_t string_type, const std::vector<char>& string_data);

    private:
        template <class X = EntryType, std::enable_if_t<
                      std::is_same_v<X, ParameterString::IndexEntryType>, int> = 0>
        [[nodiscard]] uint32_t firstBlock() const;

        template <class X = EntryType, std::enable_if_t<
                      std::is_same_v<X, typename F::BaseSymbol::IndexEntryType>, int> = 0>
        [[nodiscard]] uint32_t firstBlock() const;

        template <class X = EntryType, std::enable_if_t<
                      std::is_same_v<X, typename F::Object::IndexEntryType>, int> = 0>
        [[nodiscard]] uint32_t firstBlock() const;

        OcadFile<F>& file;
    };

    template <class F>
    class OcadFile
    {
    public:
        /** The actual file format version type, reexported. */
        using Format = F;

        /** The actual file header type. */
        using FileHeader = F::FileHeader;

        /** The actual string index type. */
        using StringIndex = OcadEntityIndex<F, ParameterString>;

        /** The actual symbol index type. */
        using SymbolIndex = OcadEntityIndex<F, typename F::BaseSymbol>;

        /** The actual object index type. */
        using ObjectIndex = OcadEntityIndex<F, typename F::Object>;

        OcadFile() = delete;

        /**
         * Constructs a new object for the Ocd file contents given by data.
         *
         * The data is not copied because of the implicit sharing provided
         * by QByteArray. Const member functions in this class do not cause
         * a deep copy.
         */
        OcadFile(const std::vector<std::byte>& data) noexcept;

        OcadFile(const OcadFile&) = delete;

        OcadFile& operator=(const OcadFile&) = delete;

        /**
         * Destructs the object.
         */
        ~OcadFile() = default;


        /**
         * Returns the raw data.
         */
        [[nodiscard]] const std::vector<std::byte>& byte_array() const;

        /**
         * Returns the raw data.
         */
        std::vector<std::byte>& byte_array();


        /**
         * Returns a pointer to the file header.
         */
        const FileHeader* header() const;

        /**
         * Returns a pointer to the file header.
         */
        FileHeader* header() { return const_cast<FileHeader*>(static_cast<const OcadFile*>(this)->header()); }


        /**
         * Returns a const reference to the parameter string index.
         */
        const StringIndex& strings() const;

        /**
         * Returns a reference to the parameter string index.
         */
        StringIndex& strings() { return string_index_; }

        /**
         * Returns a const reference to the symbol index.
         */
        const SymbolIndex& symbols() const;

        /**
         * Returns a reference to the symbol index.
         */
        SymbolIndex& symbols() noexcept { return symbol_index_; }

        /**
         * Returns a const reference to the object index.
         */
        const ObjectIndex& objects() const;

        /**
         * Returns a reference to the object index.
         */
        ObjectIndex& objects() { return object_index_; }

    private:
        std::vector<std::byte> byte_array_;
        StringIndex string_index_;
        SymbolIndex symbol_index_;
        ObjectIndex object_index_;
    };


}

template <class F, class T>
Ocad::OcadEntityIndex<F, T>::EntryType& Ocad::OcadEntityIndex<F, T>::insert(const std::vector<char>& entity_data,
    const EntryType& entry)
{
    auto& byte_array = AddPadding(file.byte_array_());
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
Ocad::OcadEntityIndex<F, T>::EntryType& Ocad::OcadEntityIndex<F, T>::insert(const std::vector<char>& entity_data)
{
    return insert(entity_data);
}

template <class F, class T>
Ocad::OcadEntityIndex<F, T>::EntryType& Ocad::OcadEntityIndex<F, T>::insert(int32_t string_type,
    const std::vector<char>& string_data)
{
    uint32_t size = string_data.size() + 1;
    return insert(std::vector(string_data.begin(), string_data.begin() + size),{0,size,string_type,0});
}

template <class F, class T>
template <class X, std::enable_if_t<std::is_same_v<X, Ocad::ParameterString::IndexEntryType>, int>>
uint32_t Ocad::OcadEntityIndex<F, T>::firstBlock() const
{
    return file.header()->firstStringBlock;
}

template <class F, class T>
template <class X, std::enable_if_t<std::is_same_v<X, typename F::BaseSymbol::IndexEntryType>, int>>
uint32_t Ocad::OcadEntityIndex<F, T>::firstBlock() const
{
    return file.header()->firstSymbolBlock;
}

template <class F, class T>
template <class X, std::enable_if_t<std::is_same_v<X, typename F::Object::IndexEntryType>, int>>
uint32_t Ocad::OcadEntityIndex<F, T>::firstBlock() const
{
    return file.header()->firstObjectBlock;
}



template <class F>
Ocad::OcadFile<F>::OcadFile(const std::vector<std::byte>& data) noexcept
: byte_array_(data),
string_index_(*this),
symbol_index_(*this),
object_index_(*this)
{
    std::cout<< byte_array_.size() << ":" << data.size() << std::endl;
    assert(data == byte_array_);
}

template <class F>
const std::vector<std::byte>& Ocad::OcadFile<F>::byte_array() const
{
    return byte_array_;
}

template <class F>
std::vector<std::byte>& Ocad::OcadFile<F>::byte_array()
{
    return byte_array_;
}

template <class F>
const Ocad::OcadFile<F>::FileHeader* Ocad::OcadFile<F>::header() const
{
    return byte_array_.size() < sizeof(FileHeader) ? nullptr : reinterpret_cast<const FileHeader*>(byte_array_.data());
}

template <class F>
const Ocad::OcadFile<F>::StringIndex& Ocad::OcadFile<F>::strings() const
{
    return string_index_;
}

template <class F>
const Ocad::OcadFile<F>::SymbolIndex& Ocad::OcadFile<F>::symbols() const
{
    return symbol_index_;
}

template <class F>
const Ocad::OcadFile<F>::ObjectIndex& Ocad::OcadFile<F>::objects() const
{
    return object_index_;
}

template <class V>
Ocad::OcadEntityIndexIterator<V>::OcadEntityIndexIterator(const std::vector<std::byte>& byte_array, IndexBlock* first_block)
    : byte_array_(&byte_array), block_(first_block), index_(0) {}

template <class V>
Ocad::OcadEntityIndexIterator<V>& Ocad::OcadEntityIndexIterator<V>::operator++()
{
    do
    {
        ++index_;
        if (index_ == 256)
        {
            index_ = 0;
            auto temp =(*byte_array_);
            block_ = GetBlockChecked<IndexBlock>(*byte_array_, block_->next_block);
            if (!block_)
                break;
        }
    }
    while (!IsValidEntry());
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
 Ocad::OcadEntityIndexIterator<V>::ValueType Ocad::OcadEntityIndexIterator<V>::operator*() const
{
    return  { &block_->entries[index_], reinterpret_cast<const ValueType::EntityType*>(byte_array_->data()+block_->entries[index_].pos) };
}

template <class V>
bool Ocad::OcadEntityIndexIterator<V>::operator==(const OcadEntityIndexIterator<V>& rhs) const
{
    return block_ == rhs.block_ && index_ == rhs.index_;
}

template <class V>
bool Ocad::OcadEntityIndexIterator<V>::operator!=(const OcadEntityIndexIterator<V>& rhs) const
{
    return !operator==(rhs);
}


template <class V>
bool Ocad::OcadEntityIndexIterator<V>::IsValidEntry() const
{
    return GetBlockChecked<typename ValueType::EntityType>(*byte_array_, block_->entries[index_].pos);
}

template <class F, class T>
template <class X, std::enable_if_t<std::is_same_v<X, Ocad::ParameterStringIndexEntry>, int>>
Ocad::OcadEntityIndex<F, T>::ValueType::operator std::vector<char>() const
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
 Ocad::OcadEntityIndex<F, T>::constIterator Ocad::OcadEntityIndex<F, T>::begin() const
{
    auto pos = firstBlock<typename T::IndexEntryType>();
    const auto& byteArray = file.byte_array();
    auto firstBlock = Ocad::GetBlockChecked<typename constIterator::IndexBlock>(file.byte_array(), pos);
    return {byteArray,firstBlock};
}

template <class F, class T>
 Ocad::OcadEntityIndex<F, T>::constIterator Ocad::OcadEntityIndex<F, T>::end() const noexcept
{
    return {};
}

template< class BlockType >
const BlockType* Ocad::GetBlockChecked(const std::vector<std::byte>& byte_array, uint32_t pos)
{
    extern const void* GetBlockCheckedRaw(const std::vector<std::byte>& byte_array_raw, uint32_t pos_raw, uint32_t block_size);
    return reinterpret_cast<const BlockType*>(GetBlockCheckedRaw(byte_array, pos, sizeof(BlockType)));
}

template< class BlockType >
BlockType* Ocad::GetBlockChecked(std::vector<std::byte>& byte_array, uint32_t pos)
{
    return const_cast<BlockType*>(GetBlockChecked<BlockType>(static_cast<const std::vector<std::byte>&>(byte_array), pos));
}




#endif //OCAD_OCDHELPER_H
