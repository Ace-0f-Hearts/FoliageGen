//
// Created by ace on 2026-01-27.
//

#ifndef OCAD_OCDHELPER_H
#define OCAD_OCDHELPER_H
#include <cstdint>
#include <iterator>
#include <type_traits>
#include <vector>

#include "ocad_types.h"


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
    BlockType* GetBlockChecked(std::vector<char>& byte_array, uint32_t pos);

    const void* GetBlockCheckedRaw(const std::vector<char>& byteArray, uint32_t pos, uint32_t block_size);

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
        using valueType = V;
        using EntryType = V::EntryType;
        using IndexBlock = IndexBlock<EntryType>;

        OcadEntityIndexIterator() noexcept = default;
        OcadEntityIndexIterator(const OcadEntityIndexIterator&) noexcept = default;
        OcadEntityIndexIterator(OcadEntityIndexIterator&&) noexcept = default;

        OcadEntityIndexIterator(const std::vector<char>& byteArray, IndexBlock* firstBlock);

        OcadEntityIndexIterator& operator=(const OcadEntityIndexIterator&) noexcept = default;
        OcadEntityIndexIterator& operator=(OcadEntityIndexIterator&&) noexcept = default;

        OcadEntityIndexIterator& operator++();

        OcadEntityIndexIterator operator++(int);

        valueType operator*() const;

        bool operator==(const OcadEntityIndexIterator<V>& rhs) const;

        bool operator!=(const OcadEntityIndexIterator<V>& rhs) const;

    private:
        bool isValidEntry() const;

        const std::vector<char>* byteArray = nullptr;
        const IndexBlock* block = nullptr;
        uint16_t index = 0;
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

        struct valueType
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
        using constIterator = OcadEntityIndexIterator<valueType>;

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
        template <class X = EntryType, typename std::enable_if<
                      std::is_same<X, ParameterString::IndexEntryType>::value, int>::type = 0>
        [[nodiscard]] uint32_t firstBlock() const;

        template <class X = EntryType, typename std::enable_if<
                      std::is_same<X, typename F::BaseSymbol::IndexEntryType>::value, int>::type = 0>
        [[nodiscard]] uint32_t firstBlock() const;

        template <class X = EntryType, typename std::enable_if<
                      std::is_same<X, typename F::Object::IndexEntryType>::value, int>::type = 0>
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

        /**
         * Constructs a new empty file.
         *
         * The internal byte array is initialized with headers and the
         * first index blocks for parameter strings, symbols and objects.
         */
        OcadFile();

        /**
         * Constructs a new object for the Ocd file contents given by data.
         *
         * The data is not copied because of the implicit sharing provided
         * by QByteArray. Const member functions in this class do not cause
         * a deep copy.
         */
        OcadFile(const std::vector<char>& data) noexcept;

        OcadFile(const OcadFile&) = delete;

        OcadFile& operator=(const OcadFile&) = delete;

        /**
         * Destructs the object.
         */
        ~OcadFile() = default;


        /**
         * Returns the raw data.
         */
        [[nodiscard]] const std::vector<char>& ConstByteArray() const;

        /**
         * Returns the raw data.
         */
        [[nodiscard]] const std::vector<char>& byte_array() const;

        /**
         * Returns the raw data.
         */
        std::vector<char>& byte_array();


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
        std::vector<char> byte_array_;
        StringIndex string_index_;
        SymbolIndex symbol_index_;
        ObjectIndex object_index_;
    };


}

#endif //OCAD_OCDHELPER_H
