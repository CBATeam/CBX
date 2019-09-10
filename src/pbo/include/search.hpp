/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Search for PBOs. Originally developed for the ACRE2 project.
 */
#pragma once

#include "archive.hpp"

#include <fstream>
#include <iterator>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "windows.h"

namespace cbx {
    namespace pbo {

#define NT_SUCCESS(x) ((x) >= 0)
        static constexpr int32_t STATUS_INFO_LENGTH_MISMATCH = 0xc0000004;

        static constexpr int32_t SystemHandleInformation = 16;
        static constexpr int32_t SystemHandleInformationEx = 64;

        static constexpr int32_t ObjectBasicInformation = 0;
        static constexpr int32_t ObjectNameInformation = 1;
        static constexpr int32_t ObjectTypeInformation = 2;

        typedef NTSTATUS(NTAPI* _NtQuerySystemInformation)(
            ULONG SystemInformationClass,
            PVOID SystemInformation,
            ULONG SystemInformationLength,
            PULONG ReturnLength
            );
        typedef NTSTATUS(NTAPI* _NtDuplicateObject)(
            HANDLE SourceProcessHandle,
            HANDLE SourceHandle,
            HANDLE TargetProcessHandle,
            PHANDLE TargetHandle,
            ACCESS_MASK DesiredAccess,
            ULONG Attributes,
            ULONG Options
            );
        typedef NTSTATUS(NTAPI* _NtQueryObject)(
            HANDLE ObjectHandle,
            ULONG ObjectInformationClass,
            PVOID ObjectInformation,
            ULONG ObjectInformationLength,
            PULONG ReturnLength
            );

        typedef struct _UNICODE_STRING {
            USHORT Length;
            USHORT MaximumLength;
            PWSTR Buffer;
        } UNICODE_STRING, * PUNICODE_STRING;

        typedef struct _SYSTEM_HANDLE {
            ULONG ProcessId;
            BYTE ObjectTypeNumber;
            BYTE Flags;
            USHORT Handle;
            PVOID Object;
            ACCESS_MASK GrantedAccess;
        } SYSTEM_HANDLE, * PSYSTEM_HANDLE;

        typedef struct _SYSTEM_HANDLE_EX {
            PVOID Object;
            HANDLE ProcessId;
            HANDLE Handle;
            ULONG GrantedAccess;
            USHORT CreatorBackTraceIndex;
            USHORT ObjectTypeIndex;
            ULONG HandleAttributes;
            ULONG Reserved;
        } SYSTEM_HANDLE_EX, * PSYSTEM_HANDLE_EX;

        typedef struct _SYSTEM_HANDLE_INFORMATION {
            ULONG HandleCount;
            SYSTEM_HANDLE Handles[1];
        } SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

        typedef struct _SYSTEM_HANDLE_INFORMATION_EX {
            ULONG_PTR HandleCount;
            ULONG_PTR Reserved;
            SYSTEM_HANDLE_EX Handles[1];
        } SYSTEM_HANDLE_INFORMATION_EX, * PSYSTEM_HANDLE_INFORMATION_EX;

        typedef enum _POOL_TYPE {
            NonPagedPool,
            PagedPool,
            NonPagedPoolMustSucceed,
            DontUseThisType,
            NonPagedPoolCacheAligned,
            PagedPoolCacheAligned,
            NonPagedPoolCacheAlignedMustS
        } POOL_TYPE, * PPOOL_TYPE;

        typedef struct _OBJECT_TYPE_INFORMATION {
            UNICODE_STRING Name;
            ULONG TotalNumberOfObjects;
            ULONG TotalNumberOfHandles;
            ULONG TotalPagedPoolUsage;
            ULONG TotalNonPagedPoolUsage;
            ULONG TotalNamePoolUsage;
            ULONG TotalHandleTableUsage;
            ULONG HighWaterNumberOfObjects;
            ULONG HighWaterNumberOfHandles;
            ULONG HighWaterPagedPoolUsage;
            ULONG HighWaterNonPagedPoolUsage;
            ULONG HighWaterNamePoolUsage;
            ULONG HighWaterHandleTableUsage;
            ULONG InvalidAttributes;
            GENERIC_MAPPING GenericMapping;
            ULONG ValidAccess;
            BOOLEAN SecurityRequired;
            BOOLEAN MaintainHandleCount;
            USHORT MaintainTypeList;
            POOL_TYPE PoolType;
            ULONG PagedPoolUsage;
            ULONG NonPagedPoolUsage;
        } OBJECT_TYPE_INFORMATION, * POBJECT_TYPE_INFORMATION;

        PVOID GetLibraryProcAddress(PSTR LibraryName, PSTR ProcName) {
            return GetProcAddress(GetModuleHandleA(LibraryName), ProcName);
        }

        class Search {
        public:
            Search();
            Search(const std::string& filter_);
            ~Search() {};

            const std::unordered_map<std::string, std::string>& file_index() { return _file_pbo_index; }
            const std::vector<std::string>& active_pbo_list() { return _active_pbo_list; }
        protected:
            bool index_files();
            bool index_files(const std::string& filter_);
            bool generate_pbo_list();

            std::unordered_map<std::string, std::string> _file_pbo_index;

            std::vector<std::string> _active_pbo_list;
            std::vector<archive_p> _archives;
        };
        using search_p = std::shared_ptr<Search>;
    }
}
