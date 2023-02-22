#include <Windows.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>

class StructConverter
{
public:
	template <typename T>
	static std::vector<uint8_t> StructToBytes(const T& structure)
	{
		const int size = sizeof(T);
		std::vector<uint8_t> bytes(size);
		memcpy_s(bytes.data(), size, &structure, size);
		return bytes;
	}
    template <typename T>
    static std::vector<uint8_t> StructToBytes(const std::vector<T>& structure)
    {
        const int size = sizeof(T) * structure.size();
        std::vector<uint8_t> bytes(size);
        memcpy_s(bytes.data(), size, structure.data(), size);
        return bytes;
    }

    static std::vector<uint8_t> StructToBytes(const void* structure, int size)
    {
        std::vector<uint8_t> bytes(size);
        memcpy_s(bytes.data(), size, structure, size);
        return bytes;
    }

    template <typename T>
    static T BytesToStruct(const std::vector<uint8_t>& bytes)
    {
        T structure;
        const int size = sizeof(T);
        memcpy_s(&structure, size, bytes.data(), size);
        return structure;
    }

    static void* BytesToStruct(const std::vector<uint8_t>& bytes, const std::type_info& structType)
    {
        const int size = static_cast<int>(bytes.size());
        void* structure = malloc(size);
        memcpy_s(structure, size, bytes.data(), size);
        return structure;
    }
};

int main_sample()
{
    // example usage:
    struct TestStruct
    {
        int a;
        double b;
        char c[10];
    };
    TestStruct test = { 123, 3.14, "hello" };
    auto bytes = StructConverter::StructToBytes(test);
    TestStruct result = StructConverter::BytesToStruct<TestStruct>(bytes);
    return 0;
}