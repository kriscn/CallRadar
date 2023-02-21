//#include <Windows.h>
//#include <iostream>
//#include <vector>
//#include <memory>
//#include <cstring>
//
//using namespace std;
//
//class StructConverter
//{
//public:
//    template<typename T>
//    static vector<uint8_t> StructToBytes(T structure)
//    {
//        int size = sizeof(T);
//        cout << size << endl;
//        auto buffer = static_cast<uint8_t*>(malloc(size));
//        try
//        {
//            memcpy(buffer, &structure, size);
//            vector<uint8_t> bytes(buffer, buffer + size);
//            return bytes;
//        }
//        finally
//        {
//            free(buffer);
//        }
//    }
//
//    template<typename T>
//    static vector<uint8_t> StructToBytes(T* structure)
//    {
//        int size = sizeof(T);
//        int length = size * sizeof(structure);
//        auto buffer = static_cast<uint8_t*>(malloc(length));
//        try
//        {
//            memcpy(buffer, structure, length);
//            vector<uint8_t> bytes(buffer, buffer + length);
//            return bytes;
//        }
//        finally
//        {
//            free(buffer);
//        }
//    }
//
//    static vector<uint8_t> StructToBytes(void* structure)
//    {
//        int size = sizeof(structure);
//        cout << size << endl;
//        auto buffer = static_cast<uint8_t*>(malloc(size));
//        try
//        {
//            memcpy(buffer, structure, size);
//            vector<uint8_t> bytes(buffer, buffer + size);
//            return bytes;
//        }
//        catch (const std::exception& e) {
//            cout << "nothing" << endl;
//        }
//        /*finally
//        {
//            free(buffer);
//        }*/
//    }
//
//    static void* BytesToStruct(vector<uint8_t> bytes, type_info const& strcutType)
//    {
//        int size = strcutType.size();
//        auto buffer = static_cast<uint8_t*>(malloc(size));
//        try
//        {
//            memcpy(buffer, bytes.data(), size);
//            return buffer;
//        }
//        catch (const std::exception& e) {
//            cout << "nothing" << endl;
//        }
//        /*finally
//        {
//            free(buffer);
//        }*/
//    }
//
//    static void* BytesToStruct(ReadOnlySequence<uint8_t> memories, type_info const& strcutType)
//    {
//        int size = strcutType.size();
//        auto buffer = static_cast<uint8_t*>(malloc(size));
//        try
//        {
//            auto stackSpan = memories.ToArray();
//            memcpy(buffer, stackSpan.data(), size);
//            return buffer;
//        }
//        catch (const std::exception& e) {
//            cout << "nothing" << endl;
//        }
//        /*finally
//        {
//            free(buffer);
//        }*/
//    }
//};
