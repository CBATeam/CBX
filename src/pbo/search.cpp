/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Search for PBOs. Originally developed for the ACRE2 project.
 */
#include "search.hpp"
#include "logging.hpp"

#include <algorithm>

using namespace cbx;

bool pbo::Search::index_files() {
    return index_files(".*");
}

bool pbo::Search::index_files(const std::string& filter_) {
    std::ifstream pbo_stream;
    std::regex txt_regex(filter_);

    if (_active_pbo_list.size() < 1) {
        return false;
    }

    for (auto& pbo_file_path : _active_pbo_list) {
        pbo_stream.open(pbo_file_path, std::ios::binary | std::ios::in);
        if (!pbo_stream.good()) {
            spd_logging::logger->error("Cannot open file: {0}", pbo_file_path);
            continue;
        }

        Archive _archive(pbo_stream);
        for (entry_p& entry : _archive.entries) {
            if (entry->filename != "") {
                if (std::regex_match(entry->filename, txt_regex)) {
                    std::string full_virtual_path = _archive.info->header["prefix"] + "\\" + entry->filename;
                    std::transform(std::cbegin(full_virtual_path), std::cend(full_virtual_path), std::begin(full_virtual_path), [](const unsigned char i) { return static_cast<char>(tolower(i)); });
                    _file_pbo_index[full_virtual_path] = pbo_file_path;
                }
            }
        }
        pbo_stream.close();
    }

    spd_logging::logger->info("PBO Index complete");

    return true;
}

pbo::Search::Search(const std::string& filter_) {
    generate_pbo_list();
    index_files(filter_);
}

pbo::Search::Search() {
    generate_pbo_list();
    index_files();
}

bool pbo::Search::generate_pbo_list() {
    spd_logging::logger->info("Generating PBO list");
    NTSTATUS status;
    PSYSTEM_HANDLE_INFORMATION_EX handleInfo;
    ULONG handleInfoSize = 0x10000;

    _NtQuerySystemInformation NtQuerySystemInformation =
        (_NtQuerySystemInformation) GetLibraryProcAddress("ntdll.dll", "NtQuerySystemInformation");
    _NtDuplicateObject NtDuplicateObject =
        (_NtDuplicateObject) GetLibraryProcAddress("ntdll.dll", "NtDuplicateObject");
    _NtQueryObject NtQueryObject =
        (_NtQueryObject) GetLibraryProcAddress("ntdll.dll", "NtQueryObject");

    if (!NtQuerySystemInformation || !NtDuplicateObject || !NtQueryObject)
        return false;

    //HANDLE pid = (HANDLE) GetCurrentProcessId();
    HANDLE pid = LongToHandle(GetCurrentProcessId());
    HANDLE processHandle = GetCurrentProcess();

    handleInfo = (PSYSTEM_HANDLE_INFORMATION_EX) malloc(handleInfoSize);
    while ((status = NtQuerySystemInformation(
        SystemHandleInformationEx,
        handleInfo,
        handleInfoSize,
        NULL
    )) == STATUS_INFO_LENGTH_MISMATCH)
        handleInfo = (PSYSTEM_HANDLE_INFORMATION_EX) realloc(handleInfo, handleInfoSize *= 2);

    /* NtQuerySystemInformation stopped giving us STATUS_INFO_LENGTH_MISMATCH. */
    if (!NT_SUCCESS(status)) {
        spd_logging::logger->error("Error opening object for pbo search");
        free(handleInfo);
        return false;
    }
    //LOG(INFO) << "Handles obtained!";
    for (ULONG i = 0; i < handleInfo->HandleCount; i++) {
        SYSTEM_HANDLE_EX handle = handleInfo->Handles[i];
        HANDLE dupHandle = NULL;
        POBJECT_TYPE_INFORMATION objectTypeInfo;
        PVOID objectNameInfo;
        UNICODE_STRING objectName;
        ULONG returnLength;

        /* Check if this handle belongs to the PID the user specified. */
        if (handle.ProcessId != pid) {
            //LOG(INFO) << "PID MISMATCH: " << (DWORD)handle.ProcessId << " != " << (DWORD)pid;
            continue;
        }


        /* Duplicate the handle so we can query it. */
        if (!NT_SUCCESS(NtDuplicateObject(
            processHandle,
            (HANDLE) handle.Handle,
            GetCurrentProcess(),
            &dupHandle,
            0,
            0,
            0
        ))) {
            //LOG(INFO) << "FAILED TO DUPLICATE OJBECT";
            continue;
        }

        /* Query the object type. */
        objectTypeInfo = (POBJECT_TYPE_INFORMATION) malloc(0x1000);
        if (!NT_SUCCESS(NtQueryObject(
            dupHandle,
            ObjectTypeInformation,
            objectTypeInfo,
            0x1000,
            NULL
        ))) {
            //LOG(INFO) << "FAILED TO QUERY OJBECT";
            CloseHandle(dupHandle);
            continue;
        }

        /* Query the object name (unless it has an access of
        0x0012019f, on which NtQueryObject could hang. */
        if (handle.GrantedAccess == 0x0012019f) {
            //LOG(INFO) << "ACCESS == 0x0012019f";
            free(objectTypeInfo);
            CloseHandle(dupHandle);
            continue;
        }

        objectNameInfo = malloc(0x1000);
        if (!NT_SUCCESS(NtQueryObject(
            dupHandle,
            ObjectNameInformation,
            objectNameInfo,
            0x1000,
            &returnLength
        ))) {
            //LOG(INFO) << "THE FUCK...";
            /* Reallocate the buffer and try again. */
            objectNameInfo = realloc(objectNameInfo, returnLength);
            if (!NT_SUCCESS(NtQueryObject(
                dupHandle,
                ObjectNameInformation,
                objectNameInfo,
                returnLength,
                NULL
            ))) {
                //LOG(INFO) << "... IS THIS SHIT?";
                free(objectTypeInfo);
                free(objectNameInfo);
                CloseHandle(dupHandle);
                continue;
            }
        }

        /* Cast our buffer into an UNICODE_STRING. */
        objectName = *(PUNICODE_STRING) objectNameInfo;

        /* Print the information! */
        if (objectName.Length) {
            std::wstring object_type(objectTypeInfo->Name.Buffer);
            std::wstring object_name(objectName.Buffer);

            if ((object_type == L"File") && (object_name.find(L".pbo") != object_name.npos)) {
                char buffer[MAX_PATH];
                GetFinalPathNameByHandle(dupHandle, buffer, sizeof(buffer), VOLUME_NAME_DOS);

                //LOG(INFO) << "Pbo: " << buffer;
                _active_pbo_list.push_back(std::string(buffer));
            }
        }

        free(objectTypeInfo);
        free(objectNameInfo);
        CloseHandle(dupHandle);
    }

    free(handleInfo);

    return true;
}
